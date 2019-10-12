#ifndef REMOTETEST_H
#define REMOTETEST_H

#include <QThread>
#include "common/msgqueue.h"
#include "communication/jtt808define.h"

#include "BasicUsageEnvironment0.hh"

class RemoteTestTaskScheduler: public BasicTaskScheduler0 {
public:
  static RemoteTestTaskScheduler* createNew(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue,unsigned maxSchedulerGranularity = 10000/*microseconds*/);
    // "maxSchedulerGranularity" (default value: 10 ms) specifies the maximum time that we wait (in "select()") before
    // returning to the event loop to handle non-socket or non-timer-based events, such as 'triggered events'.
    // You can change this is you wish (but only if you know what you're doing!), or set it to 0, to specify no such maximum time.
    // (You should set it to 0 only if you know that you will not be using 'event triggers'.)
  virtual ~RemoteTestTaskScheduler();

  #define CMDMAXLEN  1024

protected:
  RemoteTestTaskScheduler(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue,unsigned maxSchedulerGranularity);
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
    MsgQueue &mRecvMsgQueue;
    MsgQueue &mSendMsgQueue;
    char mCmdBuf[CMDMAXLEN];
};

class RemoteTest : public QThread
{
    Q_OBJECT
public:
    static RemoteTest* createRemoteTest(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue);

protected:
    explicit RemoteTest(MsgQueue &recvMsgQueue,MsgQueue &sendMsgQueue,QObject *parent = nullptr);
    ~RemoteTest();
    virtual void run();

signals:

public slots:

private:
    bool mRun;
    RemoteTestTaskScheduler *mRemoteTestTaskScheduler;
    static RemoteTest * mRemoteTest;

};

#endif // REMOTETEST_H
