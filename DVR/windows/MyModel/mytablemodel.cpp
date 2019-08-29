#include "mytablemodel.h"
#include <QDebug>
#include <QDateTimeEdit>
#include "widget.h"
#include <sys/time.h>
#include <QThread>

MyTableModel::MyTableModel(QObject *parent):
    QAbstractTableModel(parent)
{

//#ifndef LUNUX_WIN
//    QDir rootDir("/mnt/sda1/venc/");
//#else
//    QDir rootDir("/home/abhw");
//#endif

//    QDir rootDir(RootPath);

//    mCurrentPath = rootDir.absolutePath();
//    QStringList string;
//    string<<"*";
//    QFileInfoList list = rootDir.entryInfoList(string,QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot);
//    showFileInfoList(list);

    connect(this,SIGNAL(refrushModelSignal()),this,SLOT(refrushModel()));
    connect(this,SIGNAL(filelistChangeSignal(QList<MP4FileInfo> &)),Widget::getVideoDisplayWin(),SLOT(onVideoDispListSlot(QList<MP4FileInfo> &)));
}

MyTableModel::~MyTableModel()
{
    disconnect(this,SIGNAL(filelistChangeSignal(QList<MP4FileInfo> &)),Widget::getVideoDisplayWin(),SLOT(onVideoDispListSlot(QList<MP4FileInfo> &)));
    mProcess_Run = false;
    mProcess.waitForFinished();
    qDebug()<<"~MyTableModel exit";
}

void MyTableModel::onShowSlot(QDir dir)
{
    QStringList string;
#ifndef LUNUX_WIN
    string<<"*.mp4";
#else
    string<<"*";
#endif
    QFileInfoList list = dir.entryInfoList(string, QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot , QDir::DirsFirst);
    showFileInfoList(list);
}

void MyTableModel::showVideoFileList(VideoFileList &list, int fromindex, int toindex)
{
    qDebug()<<"list.count ="<<list.count();
    if(toindex >= list.count()){
        qDebug()<<"beyond the list count";
        return;
    }

    QDateTime datetime;
    m_data.clear();
    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    for(int i = fromindex; i <= toindex ;i++){

        QVariantList list1;
        datetime = QDateTime::fromTime_t(list.at(i).mctime);
        list1.push_back(datetime.toString("yyyy-MM-dd-hh-mm-ss"));

        datetime = QDateTime::fromTime_t(list.at(i).mmtime);
        list1.push_back(datetime.toString("yyyy-MM-dd-hh-mm-ss"));

        list1.push_back(tr("%1 KB").arg(int((list.at(i).msize + 1023) / 1024)));
        m_data.push_back(list1);
        if(!mProcess_Run)
            break;
    }

    gettimeofday(&etv, &tz);

    qDebug()<<"list sec:"<<etv.tv_sec - stv.tv_sec<<" usec:"<<etv.tv_usec-stv.tv_usec;

    emit refrushModelSignal();
}

void MyTableModel::showVideoFileList(VideoFileList &list)
{
    showVideoFileList(list, 0, list.count()-1);
}

void MyTableModel::showMp4FileList(QList<MP4FileInfo> &list,int fromindex,int toindex)
{
    qDebug()<<"list.count ="<<list.count();
    if(toindex >= list.count()){
        qDebug()<<"beyond the list count";
        return;
    }

    QDateTime datetime;
    QDateTime datetime1;
    m_data.clear();
    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    for(int i = fromindex; i <= toindex ;i++){

        QVariantList list1;
        datetime = QDateTime::fromTime_t(list.at(i).sttime);
        list1.push_back(datetime.toString("yyyy-MM-dd-hh-mm-ss"));

        datetime1 = QDateTime::fromTime_t(list.at(i).endtime);
        list1.push_back(datetime1.toString("yyyy-MM-dd-hh-mm-ss"));

//        QString
//        list1.push_back(tr("%1 KB").arg(int((list.at(i).msize + 1023) / 1024)));
        uint duration = datetime1.toTime_t() - datetime.toTime_t();
        uint hour = duration/3600;
        uint min = (duration%3600)/60;
        uint sec = duration%60;
        QString strduration;
        if(hour)
            strduration=QString::number(hour)+"h";
        if(min)
            strduration+=QString::number(min)+"min";
        strduration+=QString::number(sec)+"s";
        list1.push_back(strduration);
        m_data.push_back(list1);
        if(!mProcess_Run)
            break;
    }

    gettimeofday(&etv, &tz);

    qDebug()<<"list sec:"<<etv.tv_sec - stv.tv_sec<<" usec:"<<etv.tv_usec-stv.tv_usec;

    emit refrushModelSignal();

}

void MyTableModel::showMp4FileList(QList<MP4FileInfo> &list)
{
    showMp4FileList(list, 0, list.count()-1);
}

void MyTableModel::showNormalFileInfoList(QFileInfoList &list,int fromindex,int toindex)
{
    qDebug()<<"list.count ="<<list.count();
    if(toindex >= list.count()){
        qDebug()<<"beyond the list count";
        return;
    }
    QFile file;
    MYVIDEO_HEAD videohead;
    m_data.clear();
    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    for(int i = fromindex; i <= toindex ;i++){
        QFileInfo tmpFileInfo = list.at(i);
        file.setFileName(tmpFileInfo.absoluteFilePath());
        file.open(QIODevice::ReadOnly);
        if(!file.isOpen()){
            continue;
        }
//        QString fileName = tmpFileInfo.fileName();
        QVariantList list1;

        QDateTime time = tmpFileInfo.created();
        list1.push_back(time.toString("yyyy-MM-dd-hh-mm-ss"));

        time = tmpFileInfo.lastModified();
        list1.push_back(time.toString("yyyy-MM-dd-hh-mm-ss"));

        file.read((char *)&videohead,sizeof (MYVIDEO_HEAD));
        const qint64 size = videohead.endoffset - videohead.stoffset;
        list1.push_back(tr("%1 KB").arg(int((size + 1023) / 1024)));

        file.close();

        m_data.push_back(list1);
        if(!mProcess_Run)
            break;
    }

    gettimeofday(&etv, &tz);

    qDebug()<<"list sec:"<<etv.tv_sec - stv.tv_sec<<" usec:"<<etv.tv_usec-stv.tv_usec;

    emit refrushModelSignal();

}

void MyTableModel::showNormalFileInfoList(QFileInfoList &list)
{
    showNormalFileInfoList(list,0,list.count()-1);
}

void MyTableModel::showFileInfoList(QFileInfoList &list,int fromindex,int toindex)
{
    qDebug()<<"list.count ="<<list.count();
    if(toindex >= list.count()){
        qDebug()<<"beyond the list count";
        return;
    }
    m_data.clear();
    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    for(int i = fromindex; i <= toindex ;i++){
        QFileInfo tmpFileInfo = list.at(i);
//        QString fileName = tmpFileInfo.fileName();
        QVariantList list1;
        QDateTime time = tmpFileInfo.created();
        list1.push_back(time.toString("yyyy-MM-dd-hh-mm-ss"));
//        list1.push_back(fileName);

        time = tmpFileInfo.lastModified();
        list1.push_back(time.toString("yyyy-MM-dd-hh-mm-ss"));

//        const qint64 size = tmpFileInfo.size();
        uint duration = tmpFileInfo.lastModified().toTime_t() - tmpFileInfo.created().toTime_t();
        uint hour = duration/3600;
        uint min = (duration%3600)/60;
        uint sec = duration%60;
        QString strduration;
        if(hour)
            strduration=QString::number(hour)+"h";
        if(min)
            strduration+=QString::number(min)+"m";
        strduration+=QString::number(sec)+"s";
        list1.push_back(strduration);

        m_data.push_back(list1);
        if(!mProcess_Run)
            break;
    }

    gettimeofday(&etv, &tz);

    qDebug()<<"list sec:"<<etv.tv_sec - stv.tv_sec<<" usec:"<<etv.tv_usec-stv.tv_usec;

    emit refrushModelSignal();
}

void MyTableModel::showFileInfoList(QFileInfoList &list)
{
    showFileInfoList(list,0,list.count()-1);
}

void MyTableModel::onDirShowSlot(QString &filename)
{
    qDebug()<<"enter slotDirShow";
    QDir dir;
    dir.setPath(mCurrentPath);
    mPath.push(mCurrentPath);
    dir.cd(filename);
    mCurrentPath = dir.absolutePath();

    emit pathChanged();
    onShowSlot(dir);
}

void MyTableModel::onBackButtonClickedSlot()
{
    QDir dir;

    if(!mPath.isEmpty()){
        mCurrentPath = mPath.pop();
        dir.setPath(mCurrentPath);
        onShowSlot(dir);
//        QStringList string;
//        string<<"*";
//        QFileInfoList list = dir.entryInfoList(string,QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot);
//        showFileInfoList(list);
        emit pathChanged();
         qDebug()<<"mCurrentPath"<<mCurrentPath;
    }

}

int MyTableModel::getFrameIndex(QFileInfo &fileinfo,uint time)
{
    QFile file(fileinfo.absoluteFilePath());
    MYVIDEO_HEAD videohead;

    if(!file.exists()){
        qDebug()<<"file not exists "<<fileinfo.absoluteFilePath();
        return -1;
    }
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()){
        qDebug()<<"open file failed "<<file.fileName();
        return -1;
    }

    file.read((char *)&videohead,sizeof (MYVIDEO_HEAD));
    if(videohead.ctime > time){
        return 0;
    }else if(videohead.mtime < time){
        return videohead.num-1;
    }else {
        return (time - videohead.ctime)*videohead.framerate;
    }


}

int MyTableModel::search(QFileInfoList &list,int startindex,int endindex, uint time)
{
    int index = startindex + (endindex - startindex)/2;

    QFileInfo tmpFileInfo = list.at(index);

//    qDebug()<<"filetime:"<<tmpFileInfo.lastModified().toTime_t()<<"sttime:"<<time;

    if(tmpFileInfo.lastModified().toTime_t() >= time && tmpFileInfo.created().toTime_t() <= time){
        qDebug()<<"index:"<<index;
        return index;
    }
    if(index == startindex){
        qDebug()<<"endindex:"<<endindex;
        return endindex;
    }

    if(tmpFileInfo.lastModified().toTime_t() >= time){
        return search(list,startindex,index,time);
    }else {
        return search(list,index,endindex,time);
    }
}

int MyTableModel::normalSearch(QFileInfoList &list, uint sttime, uint endtime, int flag)
{

     QFileInfo tmpFileInfo;

     if(flag == 0){
         tmpFileInfo = list.last();
         qDebug()<<"lasttime:"<<tmpFileInfo.lastModified().toTime_t()<<"time:"<<sttime;
         if(tmpFileInfo.lastModified().toTime_t() < sttime){
             qDebug()<<"after last file";
             return -1;
         }
         tmpFileInfo = list.first();
         if(tmpFileInfo.created().toTime_t() >= sttime){
             qDebug()<<"before first file";
             return 0;
         }

        return search(list,0,list.count()-1, sttime);
     }else{
         tmpFileInfo = list.first();
         qDebug()<<"firsttime:"<<tmpFileInfo.created().toTime_t()<<"time:"<<endtime;
         if(tmpFileInfo.created().toTime_t() > endtime){
             qDebug()<<"before first file";
             return -1;
         }

         tmpFileInfo = list.last();
         qDebug()<<"lasttime:"<<tmpFileInfo.lastModified().toTime_t()<<"time:"<<sttime;
         if(tmpFileInfo.lastModified().toTime_t() <= endtime){
             qDebug()<<"after last file";
             return list.count()-1;
         }

         return search(list,0,list.count()-1, endtime);
     }
}

HI_S32 MyTableModel::getAlarmFileName(int Chn, VIDEO_TYPE type, char *filename, int len)
{
    char file[VIDEO_FILENAME_SIZE];

    switch (type) {
        case VIDEO_MOVEDETECT:
        {
            sprintf(file,"%s/%s%d",ALARM_FILE_PATH,MOVED_FILE,Chn);
            break;
        }
        case VIDEO_IO0:
        case VIDEO_IO1:
        case VIDEO_IO2:
        case VIDEO_IO3:
        {
            sprintf(file,"%s/%s%d%d",ALARM_FILE_PATH,IO_FILE,type,Chn);
            break;
        }
    default:{
        return HI_FAILURE;
    }
    }
    if(len < strlen(file)){
        return HI_FAILURE;
    }
    strcpy(filename,file);

    return HI_SUCCESS;
}

int MyTableModel::findAlarmFile(int Chn ,VIDEO_TYPE type,QFileInfoList &list)
{
    char alarmfilename[VIDEO_FILENAME_SIZE];
    ALARM_VIDEO_HEAD videohead;
    ALARM_FILE_INFO videoinfo;


    if(getAlarmFileName(Chn, type, alarmfilename, VIDEO_FILENAME_SIZE) < 0){
        return 0;
    }
    QFile file(alarmfilename);
    if(!file.exists()){
        qDebug()<<"file not exist"<<"["<<file.fileName()<<"]";
        return 0;
    }

    file.open(QIODevice::ReadOnly);
    file.read((char *)&videohead,sizeof (ALARM_VIDEO_HEAD));
    qDebug("num:%x ctime:%x mtime:%x",videohead.num,videohead.cTime,videohead.mtime);

    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    for (int i = 0;i < videohead.num;i++) {
        file.read((char *)&videoinfo,sizeof (ALARM_FILE_INFO));
        QFileInfo fileinfo(videoinfo.filename);
//        qDebug()<<fileinfo.fileName()<<"time:"<<fileinfo.lastModified().toTime_t();
        list.append(fileinfo);
    }

    file.close();

    gettimeofday(&etv, &tz);

    qDebug()<<"sec:"<<etv.tv_sec - stv.tv_sec<<" usec:"<<etv.tv_usec-stv.tv_usec;

    qDebug()<<"file num:"<<list.count();
    return list.count();


}

void MyTableModel::preViewFile(int Chn ,int filetype)
{
    if(mProcess.isRunning()){
        qDebug()<<"process is running";
        return;
    }
    m_data.clear();
    refrushModel();
    mProcess_Run = true;
    mProcess = QtConcurrent::run(this,&MyTableModel::priPreViewFile,Chn,filetype);
//    priPreViewFile(Chn ,filetype);
}

void MyTableModel::priPreViewFile(int Chn ,int filetype)
{
    mMp4FileList.clear();

    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
#ifndef LUNUX_WIN
    switch (filetype) {
        case 0:
        {

            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn);
            mp4fileindex->getFileList(mMp4FileList);

            delete mp4fileindex;
            break;

        }
        case 1:
        {
            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn,VIDEO_MOVEDETECT);
            mp4fileindex->getFileList(mMp4FileList);

            delete mp4fileindex;
            break;
        }
    }
#endif
    gettimeofday(&etv, &tz);

    qDebug()<<"getfile sec:"<<etv.tv_sec - stv.tv_sec<<" usec:"<<etv.tv_usec-stv.tv_usec;

    showMp4FileList(mMp4FileList);

}

int MyTableModel::playVideoList(int type,int Chn,int filetype,QString starttime,QString endtime)
{
    QList<MP4FileInfo> filelist;
    uint sttime = QDateTime::fromString(starttime, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint entime = QDateTime::fromString(endtime, "yyyy/MM/dd hh:mm:ss").toTime_t();

//    emit setWinNumSignal(1);
#ifndef LUNUX_WIN
    switch (filetype) {
        case 0:
        {

            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn);
            mp4fileindex->getFileList(filelist,sttime,entime);
            delete  mp4fileindex;



            break;
        }
        case 1:
        {
            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn,VIDEO_MOVEDETECT);
            mp4fileindex->getFileList(filelist,sttime,entime);
            delete  mp4fileindex;

            break;
        }
    }
#endif
    if(filelist.count() == 0){
         return 0;
     }

    emit filelistChangeSignal(filelist);

    qDebug()<<"playVideoList end";
    return filelist.count();

}

void MyTableModel::cancelProcess()
{
    mProcess_Run = false;
    mProcess.waitForFinished();
}

void MyTableModel::searchFile(int type,int Chn,int filetype,QString starttime,QString endtime)
{
    if(mProcess.isRunning()){
        qDebug()<<"process is running";
        return;
    }
    m_data.clear();
    refrushModel();
    mProcess_Run = true;
    mProcess = QtConcurrent::run(this,&MyTableModel::priSearchFile,type,Chn,filetype,starttime,endtime);

}

void MyTableModel::priSearchFile(int type,int Chn,int filetype,QString starttime,QString endtime)
{
    Q_UNUSED(type);
    VideoFileList filelist;
    uint sttime = QDateTime::fromString(starttime, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint entime = QDateTime::fromString(endtime, "yyyy/MM/dd hh:mm:ss").toTime_t();

    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    mMp4FileList.clear();
    switch (filetype) {
        case 0:
        {
    #ifndef LUNUX_WIN
            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn);
            mp4fileindex->getFileList(mMp4FileList,sttime,entime);
            delete  mp4fileindex;
    #endif
            break;
        }
        case 1:
        {
            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn,VIDEO_MOVEDETECT);
            mp4fileindex->getFileList(mMp4FileList,sttime,entime);
            delete  mp4fileindex;
            break;
        }
    }
    gettimeofday(&etv, &tz);

    qDebug()<<"search sec:"<<etv.tv_sec - stv.tv_sec<<" usec:"<<etv.tv_usec-stv.tv_usec;

    showMp4FileList(mMp4FileList);

    return ;

}
void MyTableModel::oncellDoubleClickedSlot(int row,int column)
{
    Q_UNUSED(column)


    QList<MP4FileInfo> fileinfolist;
    fileinfolist.append(mMp4FileList.at(row));
    filelistChangeSignal(fileinfolist);

    qDebug()<<"testSlot"<<mMp4FileList.at(row).filename;
}

QString MyTableModel::name() const
{
    return mFileName;
}
QString MyTableModel::pathname() const
{
    return mCurrentPath;
}
//QQmlListProperty<QFileInfo> MyTableModel::getFileInfoList() const
//{
////    return mFileInfoList;
//}
void MyTableModel::refrushModel()
{
     beginResetModel();
     endResetModel();

//     emit updateCount(this->rowCount(QModelIndex()));
 }

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(m_data.isEmpty())
        return 0;
    return m_data.at(0).size();
}


QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
//    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
//    qDebug()<<role;
    if (role == Qt::DisplayRole) {
        return m_data[index.row()].at(index.column());
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        return m_data[modelIndex.row()].at(modelIndex.column());
    }
    return QVariant();
}

QStringList MyTableModel::roles() const
{
//    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    return m_roleList;
}


void MyTableModel::setRoles(const QStringList roles)
{
//    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    if(!roles.isEmpty())
    {
        m_roleList = roles;
    }
}

QHash<int, QByteArray> MyTableModel::roleNames() const
{
//    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    QHash<int, QByteArray> roles;
    for(int i = 0;i < m_roleList.size();i++)
    {
        roles[Qt::UserRole+i+1] = m_roleList.at(i).toLocal8Bit();
    }
    return roles;
}

