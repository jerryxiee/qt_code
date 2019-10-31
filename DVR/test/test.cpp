#include "test.h"
#include "QDebug"
#include <QDateTime>
#include "communication/remotethread.h"
#include "modules/regionctr/regioncontrol.h"
#include "common/bcdtransform.h"

Test::Test(QObject *parent) : QObject(parent)
{

}

void Test::setMoveDetect(int Chn,bool value)
{
    qDebug()<<"Test Move Detect Chn:"<<Chn<<"value:"<<value;
    emit videoAlarmEventChangedSignal(Chn,VIDEO_MOVEDETECT,value);

}

void Test::setIOAlarm(int Chn,int io,bool value)
{
    VIDEO_TYPE type;

    switch (io) {
    case 0:
        type = VIDEO_IO0;
        break;
    case 1:
        type = VIDEO_IO1;
        break;
    case 2:
        type = VIDEO_IO2;
        break;
    case 3:
        type = VIDEO_IO3;
        break;
    default:
        return;

    }
    qDebug()<<"Test IO Alarm Chn:"<<Chn<<"io:"<<io<<"value:"<<value;

    emit videoAlarmEventChangedSignal(Chn,type,value);

}

void Test::setWinNum(int num)
{
    emit setWinNumSignal(num);
}

void Test::setRtspSize(int mode)
{
    emit setRtspSizeSignal(mode);
}

void Test::sentSeg(int mode)
{
    MsgInfo msginfo;
    RecordFileMsg filemsg;

    memset(&filemsg,0x0,sizeof (RecordFileMsg));

    filemsg.logicChn = 0;
    filemsg.fileType = 0;

    msginfo.mSize = sizeof(RecordFileMsg);
    msginfo.mMsgType = 0x9205;
    msginfo.mMesgCache = (char *)&filemsg;
    RemoteThread *remotethread = RemoteThread::getRemoteThread();
    remotethread->msgQueueLocalSend(&msginfo,msginfo.mSize);
//    remotethread->msgQueueSendToNet(&msginfo,sizeof(int));
    qDebug()<<"send msg test";
}

void Test::createSession(int chn,bool isReal)
{
    StreamParam param;
    MsgInfo msginfo;

    memset(&param,0x0,sizeof (StreamParam));
    memcpy(param.ipAddr,"192.168.3.3",strlen("192.168.3.3"));
    param.tcpPort = 8000;
    param.logicChannel = chn;

    msginfo.mSize = sizeof(StreamParam);
    if(isReal ){
        msginfo.mMsgType = 0x9101;
        param.playType = 0;
    }else {
        msginfo.mMsgType = 0x9201;
        param.playType = 1;
    }

    msginfo.mMesgCache = (char *)&param;
    RemoteThread *remotethread = RemoteThread::getRemoteThread();
    remotethread->msgQueueLocalSend(&msginfo,msginfo.mSize);

}
void Test::deleteSession(int chn,bool isReal)
{
    StreamControl param;
    MsgInfo msginfo;

    memset(&param,0x0,sizeof (StreamControl));
    memcpy(param.ipAddr,"192.168.3.3",strlen("192.168.3.3"));
    param.port = 8000;
    param.logicChannel = chn;

    msginfo.mSize = sizeof(StreamControl);
    if(isReal ){
        param.streamType = 0;
        msginfo.mMsgType = 0x9102;
        param.orderCtr = 0x0;
    }else {
        msginfo.mMsgType = 0x9202;
        param.orderCtr = 0x02;
        param.streamType = 1;
    }

    msginfo.mMesgCache = (char *)&param;
    RemoteThread *remotethread = RemoteThread::getRemoteThread();
    remotethread->msgQueueLocalSend(&msginfo,msginfo.mSize);

}

void Test::stopSession(int chn,bool isReal)
{
    StreamControl param;
    MsgInfo msginfo;

    memset(&param,0x0,sizeof (StreamControl));
    memcpy(param.ipAddr,"192.168.3.3",strlen("192.168.3.3"));
    param.port = 8000;
    param.logicChannel = chn;

    msginfo.mSize = sizeof(StreamControl);
    if(isReal ){
        msginfo.mMsgType = 0x9102;
        param.streamType = 0;
        param.orderCtr = 0x02;
    }else {
        msginfo.mMsgType = 0x9202;
        param.streamType = 1;
        param.orderCtr = 0x01;
    }

    msginfo.mMesgCache = (char *)&param;
    RemoteThread *remotethread = RemoteThread::getRemoteThread();
    remotethread->msgQueueLocalSend(&msginfo,msginfo.mSize);

}
void Test::playSession(int chn,bool isReal)
{
    StreamControl param;
    MsgInfo msginfo;

    memset(&param,0x0,sizeof (StreamControl));
    memcpy(param.ipAddr,"192.168.3.3",strlen("192.168.3.3"));
    param.port = 8000;
    param.logicChannel = chn;

    msginfo.mSize = sizeof(StreamControl);
    if(isReal ){
        msginfo.mMsgType = 0x9102;
        param.streamType = 0;
        param.orderCtr = 0x03;
    }else {
        msginfo.mMsgType = 0x9202;
        param.streamType = 1;
    }

    msginfo.mMesgCache = (char *)&param;
    RemoteThread *remotethread = RemoteThread::getRemoteThread();
    remotethread->msgQueueLocalSend(&msginfo,msginfo.mSize);


}

void Test::play(int Chn,QString starttime,QString endtime)
{
    qDebug()<<"chn:"<<Chn<<" stime:"<<starttime<<" endt:"<<endtime;

//    return;
    QList<MP4FileInfo> filelist;
    uint sttime = QDateTime::fromString(starttime, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint entime = QDateTime::fromString(endtime, "yyyy/MM/dd hh:mm:ss").toTime_t();


    MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn);
    if(mp4fileindex){
        mp4fileindex->getFileList(filelist,sttime,entime);
        delete  mp4fileindex;
    }

    if(filelist.count() == 0){
        return;
    }

    emit videoPlayListSignal(filelist);

}

void Test::deleteRegion()
{

    RegionControl::getRegionControlObject()->deleteRegion(REGIONTYPE::Circular,1);
    RegionControl::getRegionControlObject()->deleteRegion(REGIONTYPE::Rectange,1);


}

void Test::addRegion()
{
    CicularRegionAttr cattr;
    RectangleRegionAttr rattr;

    RegionBase *region;

    memset(&cattr,0x0,sizeof (CicularRegionAttr));
    memset(&rattr,0x0,sizeof (RectangleRegionAttr));
    cattr.regionId = 1;
    rattr.regionId = 1;
//    memcpy(cattr.startTime,"123456",sizeof (cattr.startTime));
//    memcpy(rattr.startTime,"123456",sizeof (rattr.startTime));
    region = new CircularRegion(cattr);
    RegionControl::getRegionControlObject()->addRegion(region);
    region = new RectangleRegion(rattr);
    RegionControl::getRegionControlObject()->addRegion(region);

    qDebug()<<"add :"<<BCDTransform::toArray(cattr.startTime,sizeof (cattr.startTime));
}
