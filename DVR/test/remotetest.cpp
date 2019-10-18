#include "remotetest.h"

#include <QDebug>


void RemoteTestTaskScheduler::check(unsigned char *cmd,int len)
{
    char tmp = 0x0;
    for (int i = 1;i < len - 2;i++) {
        tmp ^= cmd[i];
    }
    cmd[len-2] = tmp;
}

void RemoteTestTaskScheduler::sendHeartBeat(void* clientData)
{
    ((RemoteTestTaskScheduler*)clientData)->sendHeartBeat();
}
void RemoteTestTaskScheduler::sendHeartBeat()
{
    unsigned char heartCmd[] = {0x7E,0x00,0x02,0x00,0x00,0x01,0x36,0x99,0x99,0x99,0x99,0x3F,0xEF,0xE5,0x7E};
    check(heartCmd,sizeof (heartCmd));
    write(mSockfd,heartCmd,sizeof (heartCmd));
    scheduleDelayedTask(20000000, sendHeartBeat, this);
}
void RemoteTestTaskScheduler::sendPos(void* clientData)
{
    ((RemoteTestTaskScheduler*)clientData)->sendPos();
}
void RemoteTestTaskScheduler::sendPos()
{
    unsigned char posCmd[]   = {0x7E,0x02,0x00,0x00,0x5A,0x01,0x36,0x99,0x99,0x99,0x99,0x3F,0xF2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0x10,0x17,0x11,0x42,0x04,0x01,0x04,0x00,0x00,0x02,0x48,0x02,0x02,0x00,0x00,0x03,0x02,0x00,0x00,0x14,0x04,0x00,0x00,0x00,0x00,0x15,0x04,0x00,0x00,0x00,0x00,0x16,0x04,0x00,0x00,0x00,0x00,0x17,0x02,0x00,0x00,0x18,0x02,0x00,0x00,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x00,0x00,0x00,0x00,0x30,0x01,0x00,0x31,0x01,0x00,0x9F,0x7E};
    check(posCmd,sizeof (posCmd));
    write(mSockfd,posCmd,sizeof (posCmd));
    scheduleDelayedTask(30000000, sendPos, this);
}



RemoteTestTaskScheduler* RemoteTestTaskScheduler::createNew(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue,unsigned maxSchedulerGranularity) {
    return new RemoteTestTaskScheduler(recvMsgQueue,sendMsgQueue,maxSchedulerGranularity);
}

RemoteTestTaskScheduler::RemoteTestTaskScheduler(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue,unsigned maxSchedulerGranularity)
  : fMaxSchedulerGranularity(maxSchedulerGranularity), mRecvMsgQueue(recvMsgQueue),mSendMsgQueue(sendMsgQueue)
#if defined(__WIN32__) || defined(_WIN32)
  , fDummySocketNum(-1)
#endif
{
    unsigned char cmd[]      = {0x7E,0x01,0x00,0x00,0x2E,0x01,0x36,0x99,0x99,0x99,0x99,0x3F,0x00,0x00,0x21,0x00,0x6C,0x37,0x31,0x31,0x30,0x32,0x4D,0x44,0x4A,0x37,0x31,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xD5,0xE3,0x41,0x38,0x38,0x38,0x38,0x38,0x30,0x25,0x7E};
    unsigned char authCmd[]  = {0x7E,0x01,0x02,0x00,0x0B,0x01,0x36,0x99,0x99,0x99,0x99,0x3F,0xB3,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x41,0xF3,0x7E};
    struct sockaddr_in serv_addr;

    mSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (mSockfd == -1)
        return ;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("47.100.112.218");
    serv_addr.sin_port = htons(8801);

    if (connect(mSockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        return;
    printf("connect sucess\n");
    check(cmd,sizeof (cmd));
    write(mSockfd,cmd,sizeof (cmd));
    check(authCmd,sizeof (authCmd));
    write(mSockfd,authCmd,sizeof (authCmd));

    sendHeartBeat();
    sendPos();
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
    close(mSockfd);
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
    fd_set fdset;
    FD_ZERO(&fdset);

    FD_SET(mSockfd,&fdset);

    int str_len;
    unsigned char recvbuf[1024];

    select(mSockfd+1,&fdset,NULL,NULL,&tv_timeToDelay);
    if(FD_ISSET(mSockfd,&fdset)){
        str_len = read(mSockfd,recvbuf,1024);

        qDebug("read data len:%ld\n",str_len);
        if(!(recvbuf[1] == 0x80&&recvbuf[2] == 0x01))
            for (int i =0;i < str_len;i++) {
                qDebug("%x\t",recvbuf[i]);
            }

        qDebug("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

    }

    QString cmdstr;
    SourceFileInfo *fileinfolist = nullptr;

    memset(mCmdBuf,0x0,CMDMAXLEN);
    MsgInfo msginfo;
    msginfo.mMesgCache = mCmdBuf;
    msginfo.mMsgType = -1;
    mRecvMsgQueue.msgQueueRecv(&msginfo,CMDMAXLEN,100);
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
    case 0x0104:   //查询终端参数
//        QString str(mCmdBuf);
        qDebug("recv param list:%s\n",mCmdBuf);
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
    case 0x1205://查询录像资源列表

        fileinfolist = (SourceFileInfo *)mCmdBuf;
        qDebug()<<"rece file num:"<<msginfo.mSize/sizeof (SourceFileInfo)<<"filesize:"<<fileinfolist[0].fileSize;
//        for (int i = 0;i < msginfo.mSize/sizeof (SourceFileInfo);i++) {

//        }
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
