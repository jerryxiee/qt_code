#include "msgqueue.h"
#include <string.h>

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

bool MsgQueue::msgQueueRecv(pMsgInfo pInfo,uint size,int timeout)
{
    struct timespec ts;
    ts.tv_sec=timeout/1000;
    ts.tv_nsec=timeout*1000000%1000000000;

    if (timeout == -1)
        sem_wait(&mSem);
    else
    {
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
