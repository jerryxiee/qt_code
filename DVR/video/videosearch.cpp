#include "videosearch.h"
#include <QDebug>
#include <QDir>

VideoSearch::VideoSearch()
{

}


VideoFileList VideoSearch::videofilelist(QFileInfoList &infolist)
{
    QFile file;
    MYVIDEO_HEAD videohead;
    VideoFileList videofilelist;

    for(int i = 0;i < infolist.count();i++){
        file.setFileName(infolist.at(i).absoluteFilePath());
        file.open(QIODevice::ReadOnly);
        if(!file.isOpen()){
            file.close();
            continue;
        }
        file.read((char *)&videohead,sizeof (MYVIDEO_HEAD));
        if((videohead.endoffset < videohead.stoffset) || (videohead.num == 0)){
            qDebug()<<"file error "<<file.fileName();
            file.close();
            if(file.remove()){
                qDebug()<<"remove file sucess";
            }
            continue;
        }
        VideoFile   videofile;
        videofile.setEnType(videohead.enType);
        videofile.setFileName(infolist.at(i).absoluteFilePath());
        videofile.setModTime(videohead.mtime);
        videofile.setCreatTime(videohead.ctime);
        videofile.setStartIndex(0);
        videofile.setEndIndex(videohead.num-1);
        videofile.setSize(videohead.endoffset - videohead.stoffset);
        videofile.setFrameRate(videohead.framerate);
        videofile.setStartOffset(videohead.stoffset);
        videofile.setEndOffset(videohead.endoffset);

        videofilelist.append(videofile);
        file.close();
    }

    return videofilelist;

}

VideoFileList VideoSearch::readFileList(int Chn, VIDEO_TYPE type)
{
    switch (type) {
        case 0:
        {
            return readNormalFileList(Chn);
        }
        default:
        {
            return readAlarmFileList(Chn,type);
        }
    }
}

VideoFileList VideoSearch::readAlarmFileList(int Chn, VIDEO_TYPE type)
{
    VideoFileList videofilelist;
    MYVIDEO_HEAD framhead;
    FRAME_INDEX framindex1,framindex2;
    qint64 readn;

    char alarmfilename[VIDEO_FILENAME_SIZE];
    ALARM_VIDEO_HEAD videohead;
    ALARM_FILE_INFO videoinfo;
//    QFile alarmfile;


    if(getAlarmFileName(Chn, type, alarmfilename, VIDEO_FILENAME_SIZE) < 0){
        return videofilelist;
    }
    QFile file(alarmfilename);
    if(!file.exists()){
        qDebug()<<"file not exist"<<"["<<file.fileName()<<"]";
        return videofilelist;
    }

    file.open(QIODevice::ReadOnly);
    file.read((char *)&videohead,sizeof (ALARM_VIDEO_HEAD));
    qDebug("num:%d ctime:%d mtime:%d",videohead.num,videohead.cTime,videohead.mtime);

    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    for (int i = 0;i < videohead.num;i++) {
        readn = file.read((char *)&videoinfo,sizeof (ALARM_FILE_INFO));
        if(readn < sizeof (ALARM_FILE_INFO)){
            qDebug()<<"read file error "<<file.fileName();
            break;
        }
        if(videoinfo.ctime > videoinfo.mtime || videoinfo.stframe > videoinfo.endframe){
            qDebug()<<"alarm file error ";
            continue;
        }

        VideoFile videofile;
        QFile alarmfile(videoinfo.filename);
        if(!alarmfile.open(QIODevice::ReadOnly)){
            qDebug()<<"can not open alalrm file "<<videoinfo.filename;
            continue;
        }

        readn = alarmfile.read((char *)&framhead,sizeof (MYVIDEO_HEAD));
        if(readn < sizeof (MYVIDEO_HEAD)){
            qDebug()<<"read head error "<<alarmfile.fileName();
            alarmfile.close();
            continue;
        }
        alarmfile.seek(sizeof (MYVIDEO_HEAD)+sizeof (FRAME_INDEX)*videoinfo.stframe);
        readn = alarmfile.read((char *)&framindex1,sizeof (FRAME_INDEX));
        if(readn < sizeof (FRAME_INDEX)){
            qDebug()<<"read frame info error "<<alarmfile.fileName();
            alarmfile.close();
            continue;
        }
        alarmfile.seek(sizeof (MYVIDEO_HEAD)+sizeof (FRAME_INDEX)*videoinfo.endframe);
        readn = alarmfile.read((char *)&framindex2,sizeof (FRAME_INDEX));
        if(readn < sizeof (FRAME_INDEX)){
            qDebug()<<"read frame2 info error "<<alarmfile.fileName();
            alarmfile.close();
            continue;
        }

        videofile.setEnType(framhead.enType);
        videofile.setSize(framindex2.offset - framindex1.offset);
        videofile.setModTime(videoinfo.mtime);
        videofile.setEndIndex(videoinfo.endframe);
        videofile.setFileName(videoinfo.filename);
        videofile.setCreatTime(videoinfo.ctime);
        videofile.setStartIndex(videoinfo.stframe);
        videofile.setFrameRate(framhead.framerate);
        videofile.setStartOffset(framindex1.offset);
        videofile.setEndOffset(framindex2.offset);

        videofilelist.append(videofile);
        alarmfile.close();
    }
    file.close();

    gettimeofday(&etv, &tz);

    qDebug()<<"readAlarmFileList usec:"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);

    return videofilelist;

}

VideoFileList VideoSearch::readNormalFileList(int Chn)
{
    QDir filedir(RootPath+QString::number(Chn));
    QFileInfoList fileinfolist;
    QStringList string;

    string<<"*.index";
    fileinfolist = filedir.entryInfoList(string,QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot);

    return videofilelist(fileinfolist);


}

int VideoSearch::getFrameIndex(VideoFile &videofile, uint time)
{
//    QFile file(videofile.getFileName());
//    MYVIDEO_HEAD videohead;

//    if(!file.exists()){
//        qDebug()<<"file not exists "<<videofile.getFileName();
//        return -1;
//    }
//    file.open(QIODevice::ReadOnly);
//    if(!file.isOpen()){
//        qDebug()<<"open file failed "<<file.fileName();
//        return -1;
//    }

//    file.read((char *)&videohead,sizeof (MYVIDEO_HEAD));
    if(videofile.getCreatTime() > time){
        qDebug()<<"getFrameIndex1";
        return videofile.getStartIndex();
    }else if(videofile.getModTime() < time){
        qDebug()<<"getFrameIndex2";
        return videofile.getEndIndex();
    }else {
        qDebug()<<"getFrameIndex3 time:"<<time<<" StartIndex:"<<videofile.getStartIndex()<<" endindex:"<<videofile.getEndIndex();
        return videofile.getStartIndex() + (time - videofile.getCreatTime());
    }

}

uint VideoSearch::getOffset(VideoFile &videofile,quint32 time)
{

    QString filename = videofile.getFileName();
    int offset = sizeof(MYVIDEO_HEAD) + time *sizeof(FRAME_INDEX);
    FRAME_INDEX frameindex;
    qint64 readlen;

    QFile tabfile(filename);
    if(tabfile.exists()){
        tabfile.open(QIODevice::ReadOnly);
        if(!tabfile.isOpen()){
            qDebug()<<"can not open file "<<tabfile.fileName();
            return -1;
        }
        tabfile.seek(offset);
        readlen = tabfile.read((char *)&frameindex,sizeof (FRAME_INDEX));
        if(readlen <= 0){
            tabfile.close();
            return -1;
        }
        tabfile.close();
    }else {
        return -1;
    }

    return frameindex.offset;
}


int VideoSearch::search(VideoFileList &list, int startindex, int endindex, uint time)
{
    int index = startindex + (endindex - startindex)/2;

    VideoFile videofile = list.at(index);

//    qDebug()<<"filetime:"<<tmpFileInfo.lastModified().toTime_t()<<"sttime:"<<time;

    if(videofile.mmtime >= time && videofile.mctime <= time){
        qDebug()<<"index:"<<index;
        return index;
    }
    if(index == startindex){
//        videofile = list.at(endindex);
//        if(videofile.mctime > time){
//            qDebug()<<"index:"<<index;
//            return index;
//        }
        qDebug()<<"endindex:"<<endindex;
        return endindex;
    }

    if(videofile.mmtime >= time){
        return search(list,startindex,index,time);
    }else {
        return search(list,index,endindex,time);
    }

}

int VideoSearch::normalSearch(VideoFileList &list, uint sttime, uint endtime, int flag)
{
    VideoFile tmpFileInfo;

    if(flag == 0){
        tmpFileInfo = list.last();
        qDebug()<<"lasttime:"<<tmpFileInfo.mmtime<<"time:"<<sttime;
        if(tmpFileInfo.mmtime < sttime){
            qDebug()<<"after last file";
            return -1;
        }
        tmpFileInfo = list.first();
        if(tmpFileInfo.mctime >= sttime){
            qDebug()<<"before first file";
            return 0;
        }

       return search(list,0,list.count()-1, sttime);
    }else{
        tmpFileInfo = list.first();
        qDebug()<<"firsttime:"<<tmpFileInfo.mctime<<"time:"<<endtime;
        if(tmpFileInfo.mctime > endtime){
            qDebug()<<"before first file";
            return -1;
        }

        tmpFileInfo = list.last();
        qDebug()<<"lasttime:"<<tmpFileInfo.mmtime<<"time:"<<sttime;
        if(tmpFileInfo.mmtime < sttime ){
            qDebug()<<"not file";
            return -1;
        }
        else if(tmpFileInfo.mmtime <= endtime){
            qDebug()<<"after last file";
            return list.count()-1;
        }

        return search(list,0,list.count()-1, endtime);
    }

}

VideoFileList VideoSearch::searchFile(VideoFileList &list,uint sttime,uint endtime)
{
    int startindex = 0;
    int endindex = 0;
    int frameindex;
    VideoFileList returnlist;

    if(sttime > endtime || list.count() == 0){
        qDebug()<<"search error";

        return returnlist;
    }

    startindex = normalSearch(list,sttime,endtime,0);
    endindex = normalSearch(list,sttime,endtime,1);

    if((startindex == -1 && endindex == -1)
            ||((startindex == endindex)&&(list.at(startindex).mctime > endtime||list.at(startindex).mmtime < sttime))){
        qDebug()<<"can not found";
        return returnlist;
    }
    if(startindex == -1){
        startindex = 0;
    }
    if(endindex == -1){
        endindex = list.count() - 1;
    }
    if(list.at(startindex).mmtime < sttime){
        startindex++;
    }
    if(list.at(endindex).mctime > endtime){
        endindex--;
    }

    returnlist = list.mid(startindex,endindex -startindex+1);

    frameindex = getFrameIndex(returnlist.first(),sttime);
    qDebug()<<"startindex:"<<frameindex <<" endindex:"<<returnlist[0].getEndIndex();
    returnlist[0].setStartIndex(frameindex);
    uint offset = getOffset(returnlist[0],frameindex);
    if(offset > 0){
        returnlist[0].setStartOffset(offset);
    }

    frameindex = getFrameIndex(returnlist.last(),endtime);
    qDebug()<<"startindex:"<<returnlist.last().getStartIndex() <<" endindex:"<<frameindex;
    returnlist.last().setEndIndex(frameindex);
    offset = getOffset(returnlist.last(),frameindex);
    if(offset > 0){
        returnlist.last().setEndOffset(offset);
    }

    qDebug()<<"searchFile end";
    return returnlist;

}

bool VideoSearch::getAlarmFileName(int Chn, VIDEO_TYPE type, char *filename, int len)
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
        return false;
    }
    }
    if(len < strlen(file)){
        return false;
    }
    strcpy(filename,file);

    return true;
}
