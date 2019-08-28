#include "mp4fileindex.h"
#include <QDebug>
#include <QDateTime>


MP4FileIndex *MP4FileIndex::createNewFileIndex(int Chn)
{
    const QString INDEXPATH = "/mnt/sda1/venc/channel";
    const QString INDEXNAME = "/mp4.index";
    bool isNew = false;

    QString indexfile = INDEXPATH+QString::number(Chn) + INDEXNAME;

    QFileInfo fileinfo(indexfile);
    if(!fileinfo.exists()){
        isNew = true;
    }

    MP4FileIndex *pMp4fileindex = new MP4FileIndex(indexfile);
    if(!pMp4fileindex){
        return nullptr;
    }

    if(isNew){
        pMp4fileindex->reset();
    }

    return pMp4fileindex;

}

MP4FileIndex *MP4FileIndex::openFileIndex(int Chn)
{
    const QString INDEXPATH = "/mnt/sda1/venc/channel";
    const QString INDEXNAME = "/mp4.index";

    QString indexfile = INDEXPATH+QString::number(Chn) + INDEXNAME;

    QFileInfo fileinfo(indexfile);
    if(!fileinfo.exists()){
        qDebug()<<"file not exist";
        return nullptr;
    }

    return  new MP4FileIndex(indexfile);
}

MP4FileIndex::MP4FileIndex(QString filename)
{
    mFile.setFileName(filename);
    if(!mFile.open(QIODevice::ReadWrite)){
        qDebug()<<"open tabfile error";
    }
    mFile.seek(mFile.size());

}

MP4FileIndex::~MP4FileIndex()
{
    qDebug()<<"exit MP4FileIndex";
    close();
}

void MP4FileIndex::close()
{
    if(mFile.isOpen())
        mFile.close();

}

void MP4FileIndex::reset()
{
    if(mFile.isWritable()){
        mFile.reset();
        MP4FileIndexHead mp4fileindex;
        mp4fileindex.num = 0;
        mp4fileindex.duration = 0;
        mp4fileindex.sttime = QDateTime::currentDateTime().toTime_t();
        mFile.write((char *)&mp4fileindex,sizeof (MP4FileIndexHead));
        mFile.flush();

    }
}

bool MP4FileIndex::readFileHead(MP4FileIndexHead &head)
{
    if(mFile.isReadable()){
        mFile.seek(0);
        mFile.read((char *)&head,sizeof (MP4FileIndexHead));
        mFile.seek(mFile.size());
        return true;
    }
    return false;

}

bool MP4FileIndex::writeFileHead(MP4FileIndexHead &head)
{
    if(mFile.isWritable()){
        mFile.seek(0);
        mFile.write((char *)&head,sizeof (MP4FileIndexHead));
        mFile.seek(mFile.size());
        mFile.flush();
        return true;
    }
    return false;
}

int MP4FileIndex::getFileNum()
{
    MP4FileIndexHead head;
    if(readFileHead(head)){
        return head.num;
    }

    return 0;
}

int64_t MP4FileIndex::getDuration()
{
    MP4FileIndexHead head;
    if(readFileHead(head)){
        return head.duration;
    }

    return 0;
}

bool MP4FileIndex::addIndexToFile(MP4FileInfo &info)
{
    if(mFile.isOpen()){
        mFile.write((char *)&info,sizeof (MP4FileInfo));
        MP4FileIndexHead head;
        if(readFileHead(head)){
            head.num++;
            head.endtime = info.endtime;
            head.duration = info.duration;
            writeFileHead(head);
        }
        qDebug()<<"add file sucess num:"<<head.num;
        return true;
    }
    return false;

}

bool MP4FileIndex::getFileInfo(MP4FileInfo &info,int index)
{
    if(mFile.isReadable()){
        qint64 offset = sizeof (MP4FileIndexHead) + index * sizeof (MP4FileInfo);
        mFile.seek(offset);
        mFile.read((char *)&info,sizeof (MP4FileInfo));
        mFile.seek(mFile.size());
        return true;
    }

    return false;
}

int MP4FileIndex::getFileList(QList<MP4FileInfo> &filelist)
{
    if(mFile.isReadable()){
        MP4FileIndexHead head;
        if(readFileHead(head)){
            if(head.num > 0){
                MP4FileInfo *filebuf = (MP4FileInfo *)malloc(head.num*sizeof (MP4FileInfo));
                if(!filebuf){
                    qDebug()<<"malloc buff error";
                    return 0;
                }
                mFile.seek(sizeof (MP4FileIndexHead));
                if(mFile.read((char *)filebuf,head.num*sizeof (MP4FileInfo)) > 0){
                    qDebug()<<"find file num:"<<head.num;
                    for (int i = 0;i < head.num;i++) {
                        filelist.append(filebuf[i]);
                    }
                    qDebug()<<"getfilelist sucess";
                    free(filebuf);
                    return filelist.count();
                }

                free(filebuf);
            }
        }
    }

    return 0;
}

int MP4FileIndex::getFileList(QList<MP4FileInfo> &filelist,uint sttime,uint endtime)
{
    QList<MP4FileInfo> list;
    int startindex;
    int endindex;

    if(sttime > endtime){
        return 0;
    }

    if(getFileList(list) == 0){
        return 0;
    }

    startindex = searchLeft(list, 0, list.count()-1, sttime);
    if(startindex < 0){
        return 0;
    }
    endindex = searchRight(list, startindex, list.count()-1, endtime);
    if(endindex < 0){
        return 0;
    }

    filelist = list.mid(startindex,endindex + 1 - startindex);
    if(filelist.first().sttime < sttime && filelist.first().endtime > sttime){
        filelist.first().stPts += (sttime - filelist.first().sttime)*(filelist.first().endPts - filelist.first().stPts)/(filelist.first().endtime - filelist.first().sttime);
//        qDebug()<<"pts:"<<filelist.first().stPts<<" endtime:"<<filelist.first().endtime<<" sttime:"<<filelist.first().sttime;
    }
    if(filelist.last().sttime < endtime && filelist.last().endtime > endtime){
        filelist.last().endPts = filelist.last().stPts + (endtime - filelist.last().sttime)*(filelist.last().endPts - filelist.last().stPts)/(filelist.last().endtime - filelist.last().sttime);
//        qDebug()<<"pts:"<<filelist.last().endPts<<" endtime:"<<filelist.last().endtime<<" sttime:"<<filelist.last().sttime;
    }

//    qDebug()<<"count:"<<filelist.count();
    return filelist.count();
}

int MP4FileIndex::findLeftIndex(QList<MP4FileInfo> &filelist,int stindex,int endindex,uint time)
{
    int index = stindex + ((endindex - stindex)>>1);

    MP4FileInfo fileinfo = filelist.at(index);
    if(fileinfo.sttime <= time && fileinfo.endtime > time ){
//        qDebug()<<"find index:"<<stindex;
//        qDebug()<<"filest:"<<fileinfo.sttime<<" fileet:"<<fileinfo.endtime<<" find t:"<<time;
        return index;
    }
    if(index == stindex){
        if(fileinfo.endtime < time){
//            qDebug()<<"find (end):"<<endindex;
            return endindex;
        }
//        qDebug()<<"find:"<<endindex;
        return index;

    }
    if(fileinfo.sttime < time){
        return findLeftIndex(filelist,index+1,endindex,time);
    }else {
        return findLeftIndex(filelist,stindex,index,time);
    }

}

int MP4FileIndex::findRightIndex(QList<MP4FileInfo> &filelist,int stindex,int endindex,uint time)
{
    int index = stindex + ((endindex - stindex)>>1);

    MP4FileInfo fileinfo = filelist.at(index);
    if(fileinfo.sttime <= time && fileinfo.endtime > time ){
//        qDebug()<<"find index:"<<stindex;
//        qDebug()<<"filest:"<<fileinfo.sttime<<" fileet:"<<fileinfo.endtime<<" find t:"<<time;
        return stindex;
    }
    if(index == stindex){
//        qDebug()<<"find :"<<stindex;
        return index;
    }

    if(fileinfo.endtime < time){
        return findLeftIndex(filelist,index,endindex,time);
    }else {
        return findLeftIndex(filelist,stindex,index-1,time);
    }

}

int MP4FileIndex::searchLeft(QList<MP4FileInfo> &filelist, int stindex, int endindex, uint time)
{
    MP4FileInfo fileinfo = filelist.last();

    if(fileinfo.endtime <= time){
        return -1;
    }
    fileinfo = filelist.first();
    if(fileinfo.sttime >= time){
        return 0;
    }

    return findLeftIndex(filelist, stindex, endindex,time);

}

int MP4FileIndex::searchRight(QList<MP4FileInfo> &filelist, int stindex, int endindex, uint time)
{
    MP4FileInfo fileinfo = filelist.last();

    if(fileinfo.endtime <= time){
        return filelist.count() - 1;
    }
    fileinfo = filelist.first();
    if(fileinfo.sttime >= time){
        return -1;
    }

    return findRightIndex(filelist, stindex, endindex,time);

}
