#include "himediaservermanage.h"

HiMediaServerManage::HiMediaServerManage(QObject *parent) : QObject(parent)
{

}

HiMediaServerManage::~HiMediaServerManage()
{

}

HiMediaServerSession *HiMediaServerManage::lockupMediaServerSession(QString sessionName)
{
    QHashIterator<QString,void *> hashtab(mMediaServerSession);
    while (hashtab.hasNext()) {
        hashtab.next();
        if(hashtab.key() == sessionName){
            return static_cast<HiMediaServerSession *>(hashtab.value());
        }
    }
    return nullptr;
}

HiMediaServerSession *HiMediaServerManage::lockupMediaServerSession(const QString ipAddr,int port)
{
    return lockupMediaServerSession(ipAddr+":"+QString::number(port));
}

void HiMediaServerManage::addServerMediaSession(HiMediaServerSession* serverMediaSession)
{
    mMediaServerSession.insert(serverMediaSession->getSessionName(),serverMediaSession);
}

void HiMediaServerManage::removeServerMediaSession(HiMediaServerSession* serverMediaSession)
{
    if(serverMediaSession){
        delete serverMediaSession;
        mMediaServerSession.remove(serverMediaSession->getSessionName());
    }
//    if(mMediaServerSession.count() == 0){
////        mFirstCreate = true;
//    }
}

void HiMediaServerManage::removeServerMediaSession(QString sessionName)
{
    HiMediaServerSession * session = lockupMediaServerSession(sessionName);
    removeServerMediaSession(session);
}

void HiMediaServerManage::removeAllServerMediaSession()
{
    QHashIterator<QString,void *> hashtab(mMediaServerSession);
    while (hashtab.hasNext()) {
        hashtab.next();
        delete static_cast<HiMediaServerSession *>(hashtab.value());
        mMediaServerSession.remove(hashtab.key());
    }
//    mFirstCreate = true;
}


bool HiMediaServerManage::createNewServerMediaSession(StreamParam &param)
{
    HiMediaServerSession *session = lockupMediaServerSession(param.ipAddr,param.tcpPort);
    if(!session){
        session = HiMediaServerSession::createNew(param.ipAddr,param.tcpPort);
        if(session){
            addServerMediaSession(session);
        }else {
            return false;
        }
    }

    if(session){
        HiMediaServerSubSession *subSession = session->lockupSubSessionByChn(param.logicChannel,param.playType == 0);
        if(!subSession)
            subSession = HiMediaServerSubSession::createNew(session->getTaskScheduler(),*session);
        else{
            if(subSession->isAlive()){
                return false;
            }else {
                session->deleteSubSession(subSession->getName());
            }
        }

        if(subSession->createNewMediaSubSession(param)){
            session->addMediaServerSubSession(subSession);
        }else {
            delete subSession;
            if(session->isEmpty()){
                removeServerMediaSession(session);
            }
            return false;
        }
    }

//    if(mFirstCreate){
//        mFirstCreate = false;
//    }
    return true;
}

bool HiMediaServerManage::serverMediaSessionCtr(StreamControl &control)
{
    HiMediaServerSession *session = lockupMediaServerSession(control.ipAddr,control.port);
    if(!session){
        return false;
    }

    HiMediaServerSubSession *subSession = session->lockupSubSessionByChn(control.logicChannel,control.streamType == 0);
    if(!subSession){
        return false;
    }
    subSession->mediaSubSessionCtr(control);

    if((control.streamType == 0&&control.orderCtr == 0)
        ||(control.streamType == 1&&control.orderCtr == 2) ){
        session->deleteSubSession(subSession->getName());
        if(session->isEmpty()){
            removeServerMediaSession(session);
        }
    }

    return true;
}
