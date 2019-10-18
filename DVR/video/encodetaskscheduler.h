#ifndef ENCODETASKSCHEDULER_H
#define ENCODETASKSCHEDULER_H

#include "BasicUsageEnvironment0.hh"

class EncodeTaskScheduler:public BasicTaskScheduler0 {
public:
  static EncodeTaskScheduler* createNew(unsigned maxSchedulerGranularity = 10000/*microseconds*/);
    // "maxSchedulerGranularity" (default value: 10 ms) specifies the maximum time that we wait (in "select()") before
    // returning to the event loop to handle non-socket or non-timer-based events, such as 'triggered events'.
    // You can change this is you wish (but only if you know what you're doing!), or set it to 0, to specify no such maximum time.
    // (You should set it to 0 only if you know that you will not be using 'event triggers'.)
  virtual ~EncodeTaskScheduler();

protected:
  EncodeTaskScheduler(unsigned maxSchedulerGranularity);
      // called only by "createNew()"

  static void schedulerTickTask(void* clientData);
  void schedulerTickTask();

protected:
  // Redefined virtual functions:
  virtual void SingleStep(unsigned maxDelayTime);

  virtual void setBackgroundHandling(int encodefd, int conditionSet, BackgroundHandlerProc* handlerProc, void* clientData);
  virtual void moveSocketHandling(int oldFd, int newFd);

protected:
  unsigned fMaxSchedulerGranularity;

  // To implement background operations:
  int fMaxEncodeFd;
  fd_set fReadSet;

private:

};

#endif // ENCODETASKSCHEDULER_H
