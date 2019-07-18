#include "mytablemodel.h"
#include <QDebug>
#include <QDateTimeEdit>
#include "widget.h"
#include <sys/time.h>
//#include <QThread>

MyTableModel::MyTableModel(QObject *parent):
    QAbstractTableModel(parent)
{

//#ifndef LUNUX_WIN
//    QDir rootDir("/mnt/sda1/venc/");
//#else
//    QDir rootDir("/home/abhw");
//#endif

    QDir rootDir(RootPath);

    mCurrentPath = rootDir.absolutePath();
    QStringList string;
    string<<"*";
    QFileInfoList list = rootDir.entryInfoList(string,QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot);
    showFileInfoList(list);

    connect(this,SIGNAL(filelistChangeSignal(QFileInfoList &)),Widget::getVideoDisplayWin(),SLOT(onVideoDispListSlot(QFileInfoList &)));
}

MyTableModel::~MyTableModel()
{
    disconnect(this,SIGNAL(filelistChangeSignal(QFileInfoList &)),Widget::getVideoDisplayWin(),SLOT(onVideoDispListSlot(QFileInfoList &)));
    qDebug()<<"~MyTableModel exit";
}

void MyTableModel::onShowSlot(QDir dir)
{
    QStringList string;
    string<<"*";
    QFileInfoList list = dir.entryInfoList(string, QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot , QDir::DirsFirst);
    showFileInfoList(list);
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
        QString fileName = tmpFileInfo.fileName();
        QVariantList list1;
        list1.push_back(fileName);

        QDateTime time = tmpFileInfo.lastModified();
        list1.push_back(time.toString("yyyy-MM-dd-hh-mm-ss"));

        const qint64 size = tmpFileInfo.size();
        list1.push_back(tr("%1 KB").arg(int((size + 1023) / 1024)));

        m_data.push_back(list1);
    }

    gettimeofday(&etv, &tz);

    qDebug()<<"list sec:"<<etv.tv_sec - stv.tv_sec<<" usec:"<<etv.tv_usec-stv.tv_usec;

    refrushModel();
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
        emit pathChanged();
         qDebug()<<"mCurrentPath"<<mCurrentPath;
    }

}

int MyTableModel::search(QFileInfoList &list,int startindex,int endindex, uint time)
{
    int index = startindex + (endindex - startindex)/2;

    QFileInfo tmpFileInfo = list.at(index);

//    qDebug()<<"filetime:"<<tmpFileInfo.lastModified().toTime_t()<<"sttime:"<<time;

    if(index == startindex){
        qDebug()<<"search end,index:"<<index;
        if(index < list.count()-1 && tmpFileInfo.lastModified().toTime_t() < time){
            return endindex;
        }
        return index;
    }

    if(tmpFileInfo.lastModified().toTime_t() >= time){
        return search(list,startindex,index,time);
    }else {
        return search(list,index,endindex,time);
    }
}

int MyTableModel::normalSearch(QFileInfoList &list, uint sttime, uint endtime, int flag)
{

        QFileInfo tmpFileInfo = list.last();
        qDebug()<<"lasttime:"<<tmpFileInfo.lastModified().toTime_t()<<"time:"<<sttime;
        if(tmpFileInfo.lastModified().toTime_t() < sttime){
            return -1;
        }
        tmpFileInfo = list.first();
        qDebug()<<"firsttime:"<<tmpFileInfo.lastModified().toTime_t()<<"time:"<<sttime;
        if(tmpFileInfo.lastModified().toTime_t() > endtime){
            return -1;
        }
//    }else {
//        QFileInfo tmpFileInfo = list.first();
//        qDebug()<<"fasttime:"<<tmpFileInfo.lastModified().toTime_t()<<"time:"<<sttime;
//        if(tmpFileInfo.lastModified().toTime_t() > endtime){
//            return -1;
//        }

//        tmpFileInfo = list.last();
//        qDebug()<<"lasttime:"<<tmpFileInfo.lastModified().toTime_t()<<"time:"<<sttime;
//        if(tmpFileInfo.lastModified().toTime_t() < sttime){
//            return -1;
//        }
//    }
     if(flag == 0){
        return search(list,0,list.count(), sttime);
     }else{
         return search(list,0,list.count(), endtime);
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
    VIDEO_HEAD videohead;
    VIDEO_FILE_INFO videoinfo;


    if(getAlarmFileName(Chn, type, alarmfilename, VIDEO_FILENAME_SIZE) < 0){
        return 0;
    }
    QFile file(alarmfilename);
    if(!file.exists()){
        qDebug()<<"file not exist"<<"["<<file.fileName()<<"]";
        return 0;
    }

    file.open(QIODevice::ReadOnly);
    file.read((char *)&videohead,sizeof (VIDEO_HEAD));
    qDebug("num:%x ctime:%x mtime:%x",videohead.num,videohead.cTime,videohead.mtime);

    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    for (int i = 0;i < videohead.num;i++) {
        file.read((char *)&videoinfo,sizeof (VIDEO_FILE_INFO));
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

void MyTableModel::preViewFile()
{
    mCurrentPath = RootPath;
    QDir dir(mCurrentPath);

    emit pathChanged();
    onShowSlot(dir);
}

int MyTableModel::playVideoList(int type,int Chn,int filetype,QString starttime,QString endtime)
{
    int startindex = 0;
    int endindex = 0;
    QFileInfoList list;

    QDir dir(RootPath+"channel"+QString::number(Chn));
    uint sttime = QDateTime::fromString(starttime, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint entime = QDateTime::fromString(endtime, "yyyy/MM/dd hh:mm:ss").toTime_t();

    if(!dir.exists() || sttime > entime){
        qDebug()<<"search file error ,time not correct";
        return -1;
    }

    qDebug()<<"Chn:"<<Chn<<" filetype:"<<filetype<<" start:"<<starttime<<" end:"<<endtime;

    switch (filetype) {
        case 0:
        {
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            list = dir.entryInfoList();
            startindex = normalSearch(list,sttime,entime,0);
            endindex = normalSearch(list,sttime,entime,1);
            break;
        }
        case 1:
        {
            findAlarmFile(Chn ,VIDEO_MOVEDETECT,list);
            startindex = normalSearch(list,sttime,entime,0);
            endindex = normalSearch(list,sttime,entime,1);
            break;
        }

        default:
            qDebug()<<"file type error "<<filetype;
            return -1;
    }
//    m_data.clear();
    if(startindex == -1 && endindex == -1){
        qDebug()<<"can not found";
        return -1;
    }

    if(startindex == -1){
        startindex = 0;
    }
    if(endindex == -1){
        endindex = list.count() - 1;
    }

    mFileInfoList = list.mid(startindex,endindex -startindex+1);

    emit filelistChangeSignal(mFileInfoList);
    qDebug()<<"emit filelistChangeSignal file num:"<<mFileInfoList.count();

    return mFileInfoList.count();
//    mCurrentPath = RootPath+"channel"+QString::number(Chn);
//    emit pathChanged();
}

bool MyTableModel::searchFile(int type,int Chn,int filetype,QString starttime,QString endtime)
{
    int startindex = 0;
    int endindex = 0;
    QFileInfoList list;

    QDir dir(RootPath+"channel"+QString::number(Chn));
    uint sttime = QDateTime::fromString(starttime, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint entime = QDateTime::fromString(endtime, "yyyy/MM/dd hh:mm:ss").toTime_t();

    if(!dir.exists() || sttime > entime){
        qDebug()<<"search file error ,time not correct or dir not exists";
        return false;
    }

    qDebug()<<"Chn:"<<Chn<<" filetype:"<<filetype<<" start:"<<starttime<<" end:"<<endtime;

    switch (filetype) {
        case 0:
        {
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            list = dir.entryInfoList();
            startindex = normalSearch(list,sttime,entime,0);
            endindex = normalSearch(list,sttime,entime,1);
            break;
        }
        case 1:
        {
            findAlarmFile(Chn ,VIDEO_MOVEDETECT,list);
            startindex = normalSearch(list,sttime,entime,0);
            endindex = normalSearch(list,sttime,entime,1);
            break;
        }

        default:
            qDebug()<<"file type error "<<filetype;
            return false;
    }
//    m_data.clear();
    if(startindex == -1 && endindex == -1){
        qDebug()<<"can not found";
        return false;
    }

    if(startindex == -1){
        startindex = 0;
    }
    if(endindex == -1){
        endindex = list.count() - 1;
    }

    mCurrentPath = RootPath+"channel"+QString::number(Chn);
    emit pathChanged();

    showFileInfoList(list,startindex,endindex);
//    for(int i=startindex; i<endindex ;i++){
//        QFileInfo tmpFileInfo = list.at(i);
//        QString fileName = tmpFileInfo.fileName();
//        QVariantList list1;
//        list1.push_back(fileName);

//        QDateTime time = tmpFileInfo.lastModified();
//        list1.push_back(time.toString("yyyy-MM-dd-hh-mm-ss"));

//        const qint64 size = tmpFileInfo.size();
//        list1.push_back(tr("%1 KB").arg(int((size + 1023) / 1024)));

//        m_data.push_back(list1);
//    }
//    refrushModel();

    return true;
    qDebug()<<"startindex:"<<startindex<<"endindex"<<endindex;

}
void MyTableModel::oncellDoubleClickedSlot(int row,int column)
{
    QString text = m_data[row].at(column).toString();
    QString filename = mCurrentPath+"/"+text;
    QFileInfo fileinfo(filename);

    if(fileinfo.isFile()){
        qDebug()<<"display:"<<filename;
        mFileName = filename;
        emit fileNameChanged();
    }else{
        onDirShowSlot(text);
    }

    qDebug()<<"testSlot"<<filename;
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

