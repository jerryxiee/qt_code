#include "remotetest.h"

#include <QDebug>


RemoteTestTaskScheduler* RemoteTestTaskScheduler::createNew(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue,unsigned maxSchedulerGranularity) {
    return new RemoteTestTaskScheduler(recvMsgQueue,sendMsgQueue,maxSchedulerGranularity);
}

RemoteTestTaskScheduler::RemoteTestTaskScheduler(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue,unsigned maxSchedulerGranularity)
  : fMaxSchedulerGranularity(maxSchedulerGranularity), mRecvMsgQueue(recvMsgQueue),mSendMsgQueue(sendMsgQueue)
#if defined(__WIN32__) || defined(_WIN32)
  , fDummySocketNum(-1)
#endif
{

  if (maxSchedulerGranularity > 0) schedulerTickTask(); // ensures that we handle events frequently
}

void RemoteTestTaskScheduler::schedulerTickTask(void* clientData) {
  ((RemoteTestTaskScheduler*)clientData)->schedulerTickTask();
}

void RemoteTestTaskScheduler::schedulerTickTask() {
  scheduleDelayedTask(fMaxSchedulerGranularity, schedulerTickTask, this);
//  qDebug()<<"RemoteTaskScheduler run";

}

RemoteTestTaskScheduler::~RemoteTestTaskScheduler() {

}


#ifndef MILLION
#define MILLION 1000000
#endif

void RemoteTestTaskScheduler::SingleStep(unsigned maxDelayTime) {

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
    mRecvMsgQueue.msgQueueRecv(&msginfo,CMDMAXLEN,tv_timeToDelay.tv_sec*1000+tv_timeToDelay.tv_usec/1000);
    switch (msginfo.mMsgType) {
    case SERVERINFOMSGTYPE:
        qDebug()<<"recv server info";
        break;
    case 0x0100:  //
        qDebug()<<"register paltform";
        RegisterResult msg;
        msg.reportNum = 0;
        msg.result = 0;
        sleep(4);
        msginfo.mMesgCache = (char *)&msg;
        msginfo.mMsgType = 0x8100;
        msginfo.mSize = sizeof (RegisterResult);
        mSendMsgQueue.msgQueueSend(&msginfo,sizeof (RegisterResult));
        break;
    case 0x3: //
        qDebug()<<"rece unregister";
        break;
    case 0x8103://设置终端参数
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8104:   //查询终端参数
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8106:   //查询指定参数
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8105: //终端控制
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
        break;
    case 0x8017: //查询终端属性
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
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
        qDebug()<<"rece:"<<*((int *)mCmdBuf);
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
//        qDebug()<<"msgtype:"<<msginfo.mMsgType;
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

RemoteTest * RemoteTest::mRemoteTest = nullptr;

RemoteTest* RemoteTest::createRemoteTest(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue)
{
    if(!mRemoteTest){
        mRemoteTest = new RemoteTest(recvMsgQueue,sendMsgQueue);
    }

    return mRemoteTest;
}

RemoteTest::RemoteTest(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue,QObject *parent) : QThread(parent)
{
    mRemoteTestTaskScheduler = RemoteTestTaskScheduler::createNew(recvMsgQueue,sendMsgQueue);

}

RemoteTest::~RemoteTest()
{
    if(isRunning()){
        mRun = false;
        wait();
        if(mRemoteTestTaskScheduler){
            delete mRemoteTestTaskScheduler;
        }
    }
}

void RemoteTest::run()
{
    mRun = true;
    qDebug()<<"start RemoteTest";
    while (mRun) {
        if(mRemoteTestTaskScheduler){
            mRemoteTestTaskScheduler->doEventLoop(nullptr);
        }
    }

}
