#include "modulescontrol.h"
#include "platformregister.h"
#include <QDebug>


ModulesControlTaskScheduler* ModulesControlTaskScheduler::createNew(unsigned maxSchedulerGranularity) {
    return new ModulesControlTaskScheduler(maxSchedulerGranularity);
}

ModulesControlTaskScheduler::ModulesControlTaskScheduler(unsigned maxSchedulerGranularity)
  : fMaxSchedulerGranularity(maxSchedulerGranularity)
#if defined(__WIN32__) || defined(_WIN32)
  , fDummySocketNum(-1)
#endif
{

  if (maxSchedulerGranularity > 0) schedulerTickTask(); // ensures that we handle events frequently
}

void ModulesControlTaskScheduler::schedulerTickTask(void* clientData) {
  ((ModulesControlTaskScheduler*)clientData)->schedulerTickTask();
}

void ModulesControlTaskScheduler::schedulerTickTask() {
  scheduleDelayedTask(fMaxSchedulerGranularity, schedulerTickTask, this);
//  qDebug()<<"RemoteTaskScheduler run";

}

ModulesControlTaskScheduler::~ModulesControlTaskScheduler() {

}

void ModulesControlTaskScheduler::SingleStep(unsigned maxDelayTime)
{

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

    usleep(tv_timeToDelay.tv_sec*1000000 + tv_timeToDelay.tv_usec);
    // Also handle any newly-triggered event (Note that we do this *after* calling a socket handler,
    // in case the triggered event handler modifies The set of readable sockets.)
    if (fTriggersAwaitingHandling != 0) {
    if (fTriggersAwaitingHandling == fLastUsedTriggerMask) {
      // Common-case optimization for a single event trigger:
      fTriggersAwaitingHandling &=~ fLastUsedTriggerMask;
      if (fTriggeredEventHandlers[fLastUsedTriggerNum] != nullptr) {
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
          if (fTriggeredEventHandlers[i] != nullptr) {
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


ModulesControl *ModulesControl::mModulesControl = nullptr;

ModulesControl *ModulesControl::getModulesControl()
{
    if(!mModulesControl){
        mModulesControl = new ModulesControl();
    }

    return mModulesControl;
}

ModulesControl::ModulesControl(QObject *parent) : QThread(parent)
{
    mModulesControlTask = ModulesControlTaskScheduler::createNew();

}

ModulesControl::~ModulesControl()
{
    mRun = false;
    mModulesControlTask->doExitLoop();
    wait();
    delete mModulesControlTask;
}

TaskScheduler &ModulesControl::getTaskScheduler() const
{
    return *mModulesControlTask;
}

void ModulesControl::initModules()
{
    PlatformRegister::getPlatformRegister();
}

void ModulesControl::run()
{
    mRun = true;

    while (mRun) {
//        sleep(1);
//        qDebug()<<"run";
        mModulesControlTask->doEventLoop(nullptr);
    }

}
