#include "msgqueue.h"
#include <string.h>
#include <sys/time.h>
#include <stdio.h>

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
    if(size > 0){
        msgInfo.mMesgCache = new char[size];
        if(!msgInfo.mMesgCache){
            pthread_mutex_unlock(&mPthreadMutex);
            return false;
        }
        memcpy(msgInfo.mMesgCache,pInfo->mMesgCache,size);
    }
//    printf("enter %s:%d msgInfo.mSize:%d\n",__FUNCTION__,__LINE__,msgInfo.mSize);
    mMsgQueue.push(msgInfo);
    pthread_mutex_unlock(&mPthreadMutex);
    MsgInfo msginfo = mMsgQueue.front();
//    printf("enter %s:%d read mSize:%d\n",__FUNCTION__,__LINE__,msginfo.mSize);
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
//    printf("enter %s:%d\n",__FUNCTION__,__LINE__);
    pthread_mutex_lock(&mPthreadMutex);
    MsgInfo msginfo = mMsgQueue.front();
    if(msginfo.mSize > 0){
//        printf("enter %s:%d mSize=%d\n",__FUNCTION__,__LINE__,msginfo.mSize);
        if(msginfo.mSize > size){
            pthread_mutex_unlock(&mPthreadMutex);
            return false;

        }
//        printf("enter %s:%d\n",__FUNCTION__,__LINE__);
        pInfo->mSize = msginfo.mSize;
        memcpy(pInfo->mMesgCache,msginfo.mMesgCache,msginfo.mSize);
        delete [] msginfo.mMesgCache;
    }
    pInfo->mMsgType = msginfo.mMsgType;
    mMsgQueue.pop();

    pthread_mutex_unlock(&mPthreadMutex);
    return true;
}
