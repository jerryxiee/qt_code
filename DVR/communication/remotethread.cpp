#include "remotethread.h"
#include <QDebug>
#include "test/remotetest.h"
#include "modules/platformregister.h"
#include "settings/platformset.h"
#include "video/videofile.h"
#include "video/mp4fileindex.h"
#include <QDateTime>
#include "common/bcdtransform.h"


RemoteTaskScheduler* RemoteTaskScheduler::createNew(MsgQueue &msgqueue,unsigned maxSchedulerGranularity) {
    return new RemoteTaskScheduler(msgqueue,maxSchedulerGranularity);
}

RemoteTaskScheduler::RemoteTaskScheduler(MsgQueue &msgqueue,unsigned maxSchedulerGranularity)
  : fMaxSchedulerGranularity(maxSchedulerGranularity), mMsgQueue(msgqueue)
#if defined(__WIN32__) || defined(_WIN32)
  , fDummySocketNum(-1)
#endif
{

  if (maxSchedulerGranularity > 0) schedulerTickTask(); // ensures that we handle events frequently
}

void RemoteTaskScheduler::schedulerTickTask(void* clientData) {
  ((RemoteTaskScheduler*)clientData)->schedulerTickTask();
}

void RemoteTaskScheduler::schedulerTickTask() {
  scheduleDelayedTask(fMaxSchedulerGranularity, schedulerTickTask, this);
//  qDebug()<<"RemoteTaskScheduler run";

}

RemoteTaskScheduler::~RemoteTaskScheduler() {

}

void RemoteTaskScheduler::setDeviceParam(int msgid, const char *cmd)
{
    QString cmdstr(cmd);
    QStringList valuestr;
    QStringList cmdlist = cmdstr.split("/");
    QList<int> idlist;
    QVariantList valuelist;

    for (int i = 0;i < cmdlist.count();i++) {
        valuestr = cmdlist.at(i).split(":");
        if(valuestr.count() > 1){
            idlist.append(valuestr.at(0).toInt());
            valuelist.append(valuestr.at(1));
        }
    }
    PlatformSet paltfromset;
    paltfromset.setDeviceParamList(msgid,idlist,valuelist);

}

void RemoteTaskScheduler::sendDiveceParam(int msgid,const char *cmd)
{

}

void RemoteTaskScheduler::sendDiveceParam(int msgid,QList<int> &idlist)
{
    QByteArray cmdArray;
    QByteArrayList valuelist;

    PlatformSet paltfromset;
    valuelist = paltfromset.readDeviceParamList(msgid,idlist);

    for (int i = 1;i < valuelist.count();i++) {
        cmdArray += valuelist.at(i);
        if(i != valuelist.count() -1){
            cmdArray +="/";
        }
    }

    MsgInfo msginfo;
    msginfo.mMsgType = 0x0104;
    msginfo.mSize = cmdArray.length();
    msginfo.mMesgCache = cmdArray.data();
    RemoteThread::getRemoteThread()->msgQueueSendToNet(&msginfo, cmdArray.length());
}

void RemoteTaskScheduler::sendDiveceParam(int msgid, const char *cmd,int cmdlen)
{
    QList<int> idlist;
    int i = 0;
    int len = cmdlen/4;
    int dat = 0;

    for (i = 0;i < len;i += 4) {
        dat = cmd[i]<<24|cmd[i+1]<<16|cmd[i+2]<<8|cmd[i+3];
        idlist.append(dat);
    }

    sendDiveceParam(msgid,idlist);
}

void RemoteTaskScheduler::sendDiveceParamAll()
{
    QList<int> idlist;

    for (int i = 0;i < 0x84;i++) {
        idlist.append(i);
    }

    sendDiveceParam(0x8103,idlist);
}

void RemoteTaskScheduler::sendDeviceAttr()
{
    DeviceAttrMsg deviceattr;
    size_t len;
    PlatformSet paltfromset;
    QByteArray bytearray;

    deviceattr.DeviceType = paltfromset.readDeviceType();
    bytearray = paltfromset.readProduceID().toLatin1();
    if(bytearray.length() > 5){
        len = 5;
    }else {
        len = bytearray.length();
    }
    memcpy(deviceattr.ManufacturerID,bytearray.data(),len);
    bytearray = paltfromset.readDeviceModel().toLatin1();
    if(bytearray.length() > 30){
        len = 30;
    }else {
        len = bytearray.length();
    }
    memcpy(deviceattr.DeviceModel,bytearray.data(),len);

    bytearray = paltfromset.readDeviceID().toLatin1();
    if(bytearray.length() > 30){
        len = 30;
    }else {
        len = bytearray.length();
    }
    memcpy(deviceattr.DeviceID,bytearray.data(),len);

    bytearray = paltfromset.readSIMICCID().toLatin1();
    if(bytearray.length() > 10){
        len = 10;
    }else {
        len = bytearray.length();
    }
    memcpy(deviceattr.ICCID,bytearray.data(),len);

    deviceattr.HWVersionLen = DEFAULTLEN;
    deviceattr.HWVersionLen1 = DEFAULTLEN;

    bytearray = paltfromset.readHWVersion().toLatin1();
    if(bytearray.length() > DEFAULTLEN){
        len = 30;
    }else {
        len = bytearray.length();
    }
    memcpy(deviceattr.HwVersion,bytearray.data(),len);

    bytearray = paltfromset.readFWVersion().toLatin1();
    if(bytearray.length() > DEFAULTLEN){
        len = 10;
    }else {
        len = bytearray.length();
    }
    memcpy(deviceattr.HwVersion1,bytearray.data(),len);

    deviceattr.GNSSAttr = static_cast<char>(paltfromset.readGNSSAttr()) ;
    deviceattr.ComModuleAttr = static_cast<char>(paltfromset.readCommunicatModule()) ;

    MsgInfo msginfo;
    msginfo.mMsgType = 0x0107;
    msginfo.mSize = sizeof (DeviceAttrMsg);
    msginfo.mMesgCache = (char *)&deviceattr;
    RemoteThread::getRemoteThread()->msgQueueSendToNet(&msginfo, sizeof (DeviceAttrMsg));

}

void RemoteTaskScheduler::reportRecordFileList(SourceFileMsg &fileattr)
{
    QList<MP4FileInfo> filelist;
    SourceFileInfo *fileinfolist = nullptr;
    QDateTime datetime;
    QByteArray bytearray;
    uint sttime = 0;
    uint entime = 0;

    bytearray = BCDTransform::toArray(fileattr.StartTime,sizeof (fileattr.StartTime));
    if(bytearray.toInt() != 0){
        sttime = QDateTime::fromString("20"+QString(BCDTransform::toArray(fileattr.StartTime,sizeof (fileattr.StartTime))), "yyyyMMddhhmmss").toTime_t();
    }

    bytearray = BCDTransform::toArray(fileattr.EndTime,sizeof (fileattr.EndTime));
    if(bytearray.toInt() != 0){
        entime = QDateTime::fromString("20"+QString(BCDTransform::toArray(fileattr.EndTime,sizeof (fileattr.EndTime))), "yyyyMMddhhmmss").toTime_t();
    }


    if(entime == 0){
        entime = QDateTime::currentDateTime().toTime_t();
    }

    qDebug()<<"sttime:"<<sttime<<" endtime:"<<entime<<QDateTime::currentDateTime().toString("yyMMddhhmmss");
    filelist.clear();
    switch (fileattr.FileType) {
        case 0:
        {
    #ifndef LUNUX_WIN
            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(fileattr.LogicChn);
            mp4fileindex->getFileList(filelist,sttime,entime);
            delete  mp4fileindex;
    #endif
            break;
        }
        case 1:
        {
        #ifndef LUNUX_WIN
            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(fileattr.LogicChn,VIDEO_MOVEDETECT);
            mp4fileindex->getFileList(filelist,sttime,entime);
            delete  mp4fileindex;
        #endif
            break;
        }
    }

    qDebug()<<"find file num:"<<filelist.count();

    fileinfolist = (SourceFileInfo*)malloc(sizeof (SourceFileInfo)*filelist.count());
    if(!fileinfolist){
        return;
    }
    for (int i =0 ;i < filelist.count();i++) {
        fileinfolist[i].file.LogicChn = fileattr.LogicChn;
        fileinfolist[i].file.FileType = fileattr.FileType;
        fileinfolist[i].file.StreamType = fileattr.StreamType;
        fileinfolist[i].file.StoreType = fileattr.StoreType;
        datetime = QDateTime::fromTime_t(filelist.at(i).sttime);
//        bytearray = datetime.toString("yyMMddhhmmss").toLatin1();
        memcpy(fileinfolist[i].file.StartTime,BCDTransform::toBcd(datetime.toString("yyMMddhhmmss").toLong()).data(),6);
        datetime = QDateTime::fromTime_t(filelist.at(i).endtime);
        memcpy(fileinfolist[i].file.EndTime,BCDTransform::toBcd(datetime.toString("yyMMddhhmmss").toLong()).data(),6);
        QFileInfo fileinfo(filelist.at(i).filename);
        fileinfolist[i].fileSize = fileinfo.size();
    }


    MsgInfo msginfo;
    msginfo.mMsgType = 0x1205;
    msginfo.mSize = sizeof (SourceFileInfo)*filelist.count();
    msginfo.mMesgCache = (char *)fileinfolist;
    RemoteThread::getRemoteThread()->msgQueueSendToNet(&msginfo, msginfo.mSize);

}


#ifndef MILLION
#define MILLION 1000000
#endif

void RemoteTaskScheduler::SingleStep(unsigned maxDelayTime) {

    DelayInterval const& timeToDelay = fDelayQueue.timeToNextAlarm();
    struct timeval tv_timeToDelay;
    tv_timeToDelay.tv_sec = timeToDelay.seconds();
    tv_timeToDelay.tv_usec = timeToDelay.useconds();
    // Very large "tv_sec" values cause select() to fail.
    // Don't make it any larger than 1 million seconds (11.5 days)
    const long MAX_TV_SEC = MILLION;
    if (tv_timeToDelay.tv_sec > MAX_TV_SEC) {
        tv_timeToDelay.tv_sec = MAX_TV_SEC;
    }
    // Also check our "maxDelayTime" parameter (if it's > 0):
    if (maxDelayTime > 0 &&
        (tv_timeToDelay.tv_sec > (long)maxDelayTime/MILLION ||
        (tv_timeToDelay.tv_sec == (long)maxDelayTime/MILLION &&
        tv_timeToDelay.tv_usec > (long)maxDelayTime%MILLION))) {
        tv_timeToDelay.tv_sec = maxDelayTime/MILLION;
        tv_timeToDelay.tv_usec = maxDelayTime%MILLION;
    }

    MsgInfo msginfo;
    msginfo.mMesgCache = mCmdBuf;
    msginfo.mMsgType = -1;
    mMsgQueue.msgQueueRecv(&msginfo,CMDMAXLEN,tv_timeToDelay.tv_sec*1000+tv_timeToDelay.tv_usec/1000);
    switch (msginfo.mMsgType) {
    case 0:
        qDebug()<<"recv test msg";
        sendDeviceAttr();
        break;
    case 0x8004:  //查询服务器时间应答
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8100: //终端注册应答
        qDebug()<<"rece:register result";
//        RegisterResult *msg = (RegisterResult *)msginfo.mMesgCache;
        if(((RegisterResult *)msginfo.mMesgCache)->result != 0){
            PlatformRegister::getPlatformRegister()->setMainServerStatus(DisConnect);
        }else {
            PlatformRegister::getPlatformRegister()->setMainServerStatus(Connected);
            PlatformSet platformset;
            platformset.setAuthNumber(QString(((RegisterResult *)msginfo.mMesgCache)->authNum));
        }
        break;
    case 0x8103://设置终端参数
//        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        setDeviceParam(msginfo.mMsgType, mCmdBuf);
        break;
    case 0x8104:   //查询终端参数
        sendDiveceParamAll();
        break;
    case 0x8106:   //查询指定参数
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        sendDiveceParam(0x8103, mCmdBuf,msginfo.mSize);
        break;
    case 0x8105: //终端控制
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8017: //查询终端属性
        sendDeviceAttr();
        break;
    case 0x8108://下发升级包
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8201://位置信息查询
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8202://临时位置跟踪控制
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8203://人工确认报警信息
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8204://链路检查
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8300://文本信息下发
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8500://车辆控制
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8702://驾驶员信息上传
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8804://录音开始
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9101://实时音视频传输
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9102://实时音视频控制
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9105://实时音视频状态通知
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9201://录像回放请求
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9202://回放控制
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9205://查询录像资源列表
        reportRecordFileList((*(SourceFileMsg*)mCmdBuf));
        break;
    case 0x9206://文件上传
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9207://文件上传控制
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9301://云台控制
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9302://云台调焦
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9303://云台调光圈
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9304://云台雨刷
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9305://红外补光
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x9306://云台变倍
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;

    default:
        ;
    }



    // Also handle any newly-triggered event (Note that we do this *after* calling a socket handler,
    // in case the triggered event handler modifies The set of readable sockets.)
    if (fTriggersAwaitingHandling != 0) {
    if (fTriggersAwaitingHandling == fLastUsedTriggerMask) {
      // Common-case optimization for a single event trigger:
      fTriggersAwaitingHandling &=~ fLastUsedTriggerMask;
      if (fTriggeredEventHandlers[fLastUsedTriggerNum] != NULL) {
        (*fTriggeredEventHandlers[fLastUsedTriggerNum])(fTriggeredEventClientDatas[fLastUsedTriggerNum]);
      }
    } else {
      // Look for an event trigger that needs handling (making sure that we make forward progress through all possible triggers):
      unsigned i = fLastUsedTriggerNum;
      EventTriggerId mask = fLastUsedTriggerMask;

      do {
        i = (i+1)%MAX_NUM_EVENT_TRIGGERS;
        mask >>= 1;
        if (mask == 0) mask = 0x80000000;

        if ((fTriggersAwaitingHandling&mask) != 0) {
          fTriggersAwaitingHandling &=~ mask;
          if (fTriggeredEventHandlers[i] != NULL) {
            (*fTriggeredEventHandlers[i])(fTriggeredEventClientDatas[i]);
          }

          fLastUsedTriggerMask = mask;
          fLastUsedTriggerNum = i;
          break;
        }
      } while (i != fLastUsedTriggerNum);
    }
    }

    // Also handle any delayed event that may have come due.
    fDelayQueue.handleAlarm();
}


RemoteThread *RemoteThread::mRemoteThread = nullptr;

RemoteThread *RemoteThread::getRemoteThread()
{
    if(!mRemoteThread){
        mRemoteThread = new RemoteThread();
    }

    return mRemoteThread;
}

RemoteThread::RemoteThread(QObject *parent) : QThread(parent),mRemoteTask(nullptr)
{
    mReceMsgQueue.createMsgQueue();
    mSendMsgQueue.createMsgQueue();
    mRemoteTask = RemoteTaskScheduler::createNew(mReceMsgQueue);
    RemoteTest *remotetest = RemoteTest::createRemoteTest(mSendMsgQueue,mReceMsgQueue);
    remotetest->start();

}

bool RemoteThread::msgQueueSendToNet(pMsgInfo pInfo, uint size)
{
    return mSendMsgQueue.msgQueueSend(pInfo,size);
}

bool RemoteThread::msgQueueLocalSend(pMsgInfo pInfo, uint size)
{
    return mReceMsgQueue.msgQueueSend(pInfo,size);
}

TaskToken RemoteThread::scheduleDelayedTask(int64_t microseconds, TaskFunc* proc,
                void* clientData)
{
    return mRemoteTask->scheduleDelayedTask(microseconds,proc,clientData);
}

void RemoteThread::unscheduleDelayedTask(TaskToken& prevTask)
{
    return mRemoteTask->unscheduleDelayedTask(prevTask);
}

RemoteThread::~RemoteThread()
{
    if(isRunning()){
        mRun = false;
        wait();
        if(mRemoteTask)
            delete mRemoteTask;
        mReceMsgQueue.destroyMsgQueue();
        mSendMsgQueue.destroyMsgQueue();
    }

}


void RemoteThread::run()
{

    mRun = true;
    qDebug()<<"start remotethread";
    while (mRun) {
        if(mRemoteTask){
            mRemoteTask->doEventLoop(nullptr);
        }
    }

}
