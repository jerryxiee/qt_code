#include "encodetaskscheduler.h"
#include <stdio.h>
#include "common/schedule/HandlerSet.hh"

//EncodeTaskScheduler::EncodeTaskScheduler(unsigned maxSchedulerGranularity)
//{

//}

EncodeTaskScheduler* EncodeTaskScheduler::createNew(unsigned maxSchedulerGranularity) {
    return new EncodeTaskScheduler(maxSchedulerGranularity);
}

EncodeTaskScheduler::EncodeTaskScheduler(unsigned maxSchedulerGranularity)
  : fMaxSchedulerGranularity(maxSchedulerGranularity), fMaxEncodeFd(0)
{
  FD_ZERO(&fReadSet);

  if (maxSchedulerGranularity > 0) schedulerTickTask(); // ensures that we handle events frequently
}

EncodeTaskScheduler::~EncodeTaskScheduler() {


}

void EncodeTaskScheduler::schedulerTickTask(void* clientData) {
  ((EncodeTaskScheduler*)clientData)->schedulerTickTask();
}

void EncodeTaskScheduler::schedulerTickTask() {
  scheduleDelayedTask(fMaxSchedulerGranularity, schedulerTickTask, this);
}

#ifndef MILLION
#define MILLION 1000000
#endif

void EncodeTaskScheduler::SingleStep(unsigned maxDelayTime) {
  fd_set readSet = fReadSet; // make a copy for this select() call


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

  int selectResult = select(fMaxEncodeFd, &readSet, nullptr, nullptr, &tv_timeToDelay);
  if (selectResult < 0) {

    if (errno != EINTR && errno != EAGAIN) {

    // Unexpected error - treat this as fatal:
#if !defined(_WIN32_WCE)
    perror("EncodeTaskScheduler::SingleStep(): select() fails");
    // Because this failure is often "Bad file descriptor" - which is caused by an invalid socket number (i.e., a socket number
    // that had already been closed) being used in "select()" - we print out the sockets that were being used in "select()",
    // to assist in debugging:
    fprintf(stderr, "socket numbers used in the select() call:");
    for (int i = 0; i < 10000; ++i) {
      if (FD_ISSET(i, &fReadSet)) {
        fprintf(stderr, " %d(", i);
        if (FD_ISSET(i, &fReadSet)) fprintf(stderr, "r");
        fprintf(stderr, ")");
      }
    }
    fprintf(stderr, "\n");
#endif
    internalError();
      }
  }

  // Call the handler function for one readable socket:
  HandlerIterator iter(*fHandlers);
  HandlerDescriptor* handler;
  // To ensure forward progress through the handlers, begin past the last
  // socket number that we handled:
  if (fLastHandledSocketNum >= 0) {
    while ((handler = iter.next()) != nullptr) {
      if (handler->socketNum == fLastHandledSocketNum) break;
    }
    if (handler == nullptr) {
      fLastHandledSocketNum = -1;
      iter.reset(); // start from the beginning instead
    }
  }
  while ((handler = iter.next()) != nullptr) {
    int sock = handler->socketNum; // alias
    int resultConditionSet = 0;
    if (FD_ISSET(sock, &readSet) && FD_ISSET(sock, &fReadSet)/*sanity check*/) resultConditionSet |= SOCKET_READABLE;

    if ((resultConditionSet&handler->conditionSet) != 0 && handler->handlerProc != nullptr) {
      fLastHandledSocketNum = sock;
          // Note: we set "fLastHandledSocketNum" before calling the handler,
          // in case the handler calls "doEventLoop()" reentrantly.
      (*handler->handlerProc)(handler->clientData, resultConditionSet);
      break;
    }
  }
  if (handler == nullptr && fLastHandledSocketNum >= 0) {
    // We didn't call a handler, but we didn't get to check all of them,
    // so try again from the beginning:
    iter.reset();
    while ((handler = iter.next()) != nullptr) {
      int sock = handler->socketNum; // alias
      int resultConditionSet = 0;
      if (FD_ISSET(sock, &readSet) && FD_ISSET(sock, &fReadSet)/*sanity check*/) resultConditionSet |= SOCKET_READABLE;

      if ((resultConditionSet&handler->conditionSet) != 0 && handler->handlerProc != nullptr) {
    fLastHandledSocketNum = sock;
        // Note: we set "fLastHandledSocketNum" before calling the handler,
            // in case the handler calls "doEventLoop()" reentrantly.
    (*handler->handlerProc)(handler->clientData, resultConditionSet);
    break;
      }
    }
    if (handler == nullptr) fLastHandledSocketNum = -1;//because we didn't call a handler
  }

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

void EncodeTaskScheduler
  ::setBackgroundHandling(int encodefd, int conditionSet, BackgroundHandlerProc* handlerProc, void* clientData) {
  if (encodefd < 0) return;
#if !defined(__WIN32__) && !defined(_WIN32) && defined(FD_SETSIZE)
  if (encodefd >= (int)(FD_SETSIZE)) return;
#endif
  FD_CLR((unsigned)encodefd, &fReadSet);

  if (conditionSet == 0) {
    fHandlers->clearHandler(encodefd);
    if (encodefd+1 == fMaxEncodeFd) {
      --fMaxEncodeFd;
    }
  } else {
    fHandlers->assignHandler(encodefd, conditionSet, handlerProc, clientData);
    if (encodefd+1 > fMaxEncodeFd) {
      fMaxEncodeFd = encodefd+1;
    }
    if (conditionSet&SOCKET_READABLE) FD_SET((unsigned)encodefd, &fReadSet);
  }
}

void EncodeTaskScheduler::moveSocketHandling(int oldFd, int newFd) {
  if (oldFd < 0 || newFd < 0) return; // sanity check
#if !defined(__WIN32__) && !defined(_WIN32) && defined(FD_SETSIZE)
  if (oldFd >= (int)(FD_SETSIZE) || newFd >= (int)(FD_SETSIZE)) return; // sanity check
#endif
  if (FD_ISSET(oldFd, &fReadSet)) {FD_CLR((unsigned)oldFd, &fReadSet); FD_SET((unsigned)newFd, &fReadSet);}

  fHandlers->moveHandler(oldFd, newFd);

  if (oldFd+1 == fMaxEncodeFd) {
    --fMaxEncodeFd;
  }
  if (newFd+1 > fMaxEncodeFd) {
    fMaxEncodeFd = newFd+1;
  }
}
