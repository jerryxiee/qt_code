#ifndef MSGQUEUE_H
#define MSGQUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include <queue>

using namespace std;

typedef struct{
    int mMsgType;      //消息类型
    uint mSize;        //消息大小
    char *mMesgCache;  //消息指针
}MsgInfo , *pMsgInfo;

class MsgQueue
{
public:
    MsgQueue();

    bool createMsgQueue();     //创建消息队列
    bool destroyMsgQueue();    //销毁消息队列

    bool msgQueueSend(pMsgInfo pInfo, uint size);   //发送消息到队列并发送信号 size:消息结构大小
    bool msgQueueRecv(pMsgInfo pInfo, uint size,int timeout);   //接收消息  timeout:单位ms


private:
    sem_t mSem;
    queue<MsgInfo> mMsgQueue;        //存储消息的队列
    pthread_mutex_t mPthreadMutex;
};

#endif // MSGQUEUE_H
