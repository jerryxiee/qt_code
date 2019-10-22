#include "himediaservermanage.h"

HiMediaServerManage::HiMediaServerManage(QObject *parent) : QObject(parent)
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
}


bool createNewServerMediaSession()
{

}
