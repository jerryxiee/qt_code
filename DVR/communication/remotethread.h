#ifndef REMOTETHREAD_H
#define REMOTETHREAD_H

#include "common/msgqueue.h"
#include <QThread>

#include "BasicUsageEnvironment0.hh"

class RemoteTaskScheduler: public BasicTaskScheduler0 {
public:
  static RemoteTaskScheduler* createNew(MsgQueue &msgqueue,unsigned maxSchedulerGranularity = 10000/*microseconds*/);
    // "maxSchedulerGranularity" (default value: 10 ms) specifies the maximum time that we wait (in "select()") before
    // returning to the event loop to handle non-socket or non-timer-based events, such as 'triggered events'.
    // You can change this is you wish (but only if you know what you're doing!), or set it to 0, to specify no such maximum time.
    // (You should set it to 0 only if you know that you will not be using 'event triggers'.)
  virtual ~RemoteTaskScheduler();

  #define CMDMAXLEN  1024

protected:
  RemoteTaskScheduler(MsgQueue &msgqueue,unsigned maxSchedulerGranularity);
      // called only by "createNew()"

  static void schedulerTickTask(void* clientData);
  void schedulerTickTask();

protected:
  // Redefined virtual functions:
  virtual void SingleStep(unsigned maxDelayTime);

  virtual void setBackgroundHandling(int socketNum, int conditionSet, BackgroundHandlerProc* handlerProc, void* clientData){}
  virtual void moveSocketHandling(int oldSocketNum, int newSocketNum){}

protected:
  unsigned fMaxSchedulerGranularity;

  // To implement background operations:

private:
    MsgQueue &mMsgQueue;
    char mCmdBuf[CMDMAXLEN];
};

class RemoteThread : public QThread
{
    Q_OBJECT
public:
    static RemoteThread *getRemoteThread();
    ~RemoteThread();

    bool msgQueueSendToNet(pMsgInfo pInfo, uint size);    //通过网络发送至服务器
    bool msgQueueLocalSend(pMsgInfo pInfo, uint size);   //本地信号处理
    TaskToken scheduleDelayedTask(int64_t microseconds, TaskFunc* proc,
                    void* clientData);                          //注册延时任务
    void unscheduleDelayedTask(TaskToken& prevTask);            //移除延时任务

protected:
    explicit RemoteThread(QObject *parent = nullptr);
    virtual void run();

signals:

public slots:

private:
    bool mRun;
    MsgQueue mReceMsgQueue;
    MsgQueue mSendMsgQueue;
    RemoteTaskScheduler *mRemoteTask;
    static RemoteThread *mRemoteThread;

};

#endif // REMOTESCHEULER_H
