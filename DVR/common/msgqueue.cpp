#include "msgqueue.h"
#include <string.h>
#include <sys/time.h>

MsgQueue::MsgQueue()
{

}

bool MsgQueue::createMsgQueue()
{
    pthread_mutex_init(&mPthreadMutex,nullptr);
    sem_init(&mSem,0,0);
    return true;
}

bool MsgQueue::destroyMsgQueue()
{
    pthread_mutex_lock(&mPthreadMutex);
    sem_destroy(&mSem);

    while (mMsgQueue.size()) {
        MsgInfo msginfo = mMsgQueue.front();
        delete [] msginfo.mMesgCache;
        mMsgQueue.pop();
    }
    pthread_mutex_unlock(&mPthreadMutex);
    pthread_mutex_destroy(&mPthreadMutex);
    return true;
}


bool MsgQueue::msgQueueSend(pMsgInfo pInfo, uint size)
{
    pthread_mutex_lock(&mPthreadMutex);
    MsgInfo msgInfo;

    msgInfo.mMsgType = pInfo->mMsgType;
    msgInfo.mSize = size;
    msgInfo.mMesgCache = new char[size];
    if(!msgInfo.mMesgCache){
        pthread_mutex_unlock(&mPthreadMutex);
        return false;
    }
    memcpy(msgInfo.mMesgCache,pInfo->mMesgCache,size);

    mMsgQueue.push(msgInfo);
    pthread_mutex_unlock(&mPthreadMutex);

    sem_post(&mSem);
    return true;
}

void MsgQueue::time_add_ms(struct timeval *time, int ms)
{
        time->tv_usec += ms * 1000; //
        if(time->tv_usec >= 1000000) //
        {
            time->tv_sec += time->tv_usec / 1000000;
            time->tv_usec %= 1000000;
        }
}


bool MsgQueue::msgQueueRecv(pMsgInfo pInfo,uint size,int timeout)
{
    if (timeout == -1)
        sem_wait(&mSem);
    else
    {
        struct timespec ts;
        struct timeval time;
        gettimeofday(&time, nullptr);

        time_add_ms(&time, timeout);

        ts.tv_sec=time.tv_sec;
        ts.tv_nsec=time.tv_usec*1000;

        int sts =sem_timedwait(&mSem, &ts);
        if (-1 == sts)
        {
            return false;
        }
    }

    pthread_mutex_lock(&mPthreadMutex);
    MsgInfo msginfo = mMsgQueue.front();
    if(msginfo.mSize > size){
        pthread_mutex_unlock(&mPthreadMutex);
        return false;

    }
    memcpy(pInfo->mMesgCache,msginfo.mMesgCache,msginfo.mSize);
    delete [] msginfo.mMesgCache;
    mMsgQueue.pop();

    pthread_mutex_unlock(&mPthreadMutex);
    return true;
}
