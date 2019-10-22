#include "himediaserversession.h"
#include "video/mp4fileindex.h"
#include "HW/video/hivpsssource.h"
#include <QDebug>


HiMediaServerSession *HiMediaServerSession::createNew(const QString sessionName)
{
    return new HiMediaServerSession(sessionName);
}

HiMediaServerSession::HiMediaServerSession(const QString sessionName, QObject *parent) :
    QThread(parent),mTaskScheduler(nullptr),mSessionName(sessionName),mRun(false)
{
    mTaskScheduler = EncodeTaskScheduler::createNew();
}

HiMediaServerSession::~HiMediaServerSession()
{
    deleteAllSubSession();
    mRun = false;
    wait();
    if(mTaskScheduler){
        delete mTaskScheduler;
    }
}

QString HiMediaServerSession::generateSessionName(const char *ipAddr, int port)
{
    return QString(ipAddr)+":"+QString::number(port);
}
QString HiMediaServerSession::generateSubSessionName(int chn,bool isReal)
{
    if(isReal){
        return "real:"+QString::number(chn);
    }else {
        return "playback:"+QString::number(chn);
    }
}

HiMediaServerSubSession *HiMediaServerSession::lockupSubSessionByName(QString subSessionName)
{
    QHashIterator<QString,void *> hashtab(mSubSession);
    while (hashtab.hasNext()) {
        hashtab.next();
        if(hashtab.key() == subSessionName){
            return static_cast<HiMediaServerSubSession *>(hashtab.value());

        }

    }
    return nullptr;

}

bool HiMediaServerSession::addMediaServerSubSession(HiMediaServerSubSession *session)
{
    mMutex.lock();
    mSubSession.insert(session->getName(),session);
//#ifndef LUNUX_WIN
//    if(session)
//        mTaskScheduler->setBackgroundHandling(session->getSelectFd(),SOCKET_READABLE,HiFrameConsumer::doProcess,session->getHiVenc());
//#endif
    mMutex.unlock();
    qDebug()<<"addMediaServerSubSession start thread";
    start();
    return true;
}

void HiMediaServerSession::deleteSubSession(QString subSessionName)
{
    HiMediaServerSubSession *subsession = lockupSubSessionByName(subSessionName);
    mMutex.lock();
    if(subsession){
//#ifndef LUNUX_WIN
//        mTaskScheduler->turnOffBackgroundReadHandling(subsession->getSelectFd());
//#endif
        mSubSession.remove(subsession->getName());
        delete subsession;
    }
    mMutex.unlock();

}

void HiMediaServerSession::deleteAllSubSession()
{

    mMutex.lock();
    QHashIterator<QString,void *> hashtab(mSubSession);
    while (hashtab.hasNext()) {
        hashtab.next();
//#ifndef LUNUX_WIN
//        mTaskScheduler->turnOffBackgroundReadHandling(static_cast<HiMediaServerSubSession *>(hashtab.value())->getSelectFd());
//#endif
        delete static_cast<HiMediaServerSubSession *>(hashtab.value());
        mSubSession.remove(hashtab.key());

    }
    mMutex.unlock();
}


void HiMediaServerSession::run()
{
    mRun = true;
    while (mRun) {
        mTaskScheduler->SingleStep(0);
    }

}

HiMediaServerSubSession *HiMediaServerSubSession::createNew(EncodeTaskScheduler &taskScheduler,HiMediaServerSession &mediaServerSession, int chn, int type, bool isReal, uint startTime, uint endTime)
{
    HiMediaServerSubSession *subSession = new HiMediaServerSubSession(taskScheduler,mediaServerSession,chn,type,isReal, startTime, endTime);
    if(!subSession){
        return nullptr;
    }
    if(!subSession->isCreateSucess()){
        delete subSession;
        subSession = nullptr;
    }

    return subSession;
}

HiMediaServerSubSession::HiMediaServerSubSession(EncodeTaskScheduler &taskScheduler,HiMediaServerSession &mediaServerSession,int chn, int type, bool isReal, uint startTime, uint endTime):
#ifndef LUNUX_WIN
    mTestVenc(nullptr),mVideoPlay(nullptr),
#endif
    mCreateSucess(false),mTaskScheduler(taskScheduler),mOurServerSession(mediaServerSession)
{
#ifndef LUNUX_WIN
    HiVpssSource * source;

    if(!isReal){
        mVideoPlay = createVideoPlay(chn,type,startTime,endTime);
        source = HiVpssSource::createNew(mVideoPlay->getVpssGrp(),mVideoPlay->getVpssChn());
    }else {
        source = HiVpssSource::createNew(chn,VPSSCHN);
    }
    mTestVenc = HiVencToMp4::createNew(source,VIDEO_NORM,PIC_HD720,SAMPLE_RC_CBR,0,25,0,PAYLOAD_TYPE,"stareamtest.mp4");
    if(!mTestVenc){
        qDebug("%s:%d\n",__FUNCTION__,__LINE__);
        return;
    }
    mTaskScheduler.setBackgroundHandling(((HiVencConsumer *)mTestVenc)->getVencFd(),SOCKET_READABLE,HiFrameConsumer::doProcess,mTestVenc);
    mCreateSucess = true;
#endif

}

HiMediaServerSubSession::~HiMediaServerSubSession()
{
#ifndef LUNUX_WIN
    if(mTestVenc){
        mTaskScheduler.turnOffBackgroundReadHandling(mTestVenc->getVencFd());
        delete mTestVenc;
    }
    if(mVideoPlay){
        mVideoPlay->stopPlay();
        delete mVideoPlay;
    }
#endif
}

//int HiMediaServerSubSession::getSelectFd() const
//{
//#ifndef LUNUX_WIN
//    if(mTestVenc){
//        return mTestVenc->getVencFd();
//    }
//#endif
//    return -1;
//}

#ifndef LUNUX_WIN
MP4VideoPlay *HiMediaServerSubSession::createVideoPlay(int Chn,int type,uint stTime,uint endTime)
{
    QList<MP4FileInfo> filelist;

    switch (type) {
        case 0:
        {

            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn);
            mp4fileindex->getFileList(filelist,stTime,endTime);
            delete  mp4fileindex;
            break;
        }
        case 1:
        {
            MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn,VIDEO_MOVEDETECT);
            mp4fileindex->getFileList(filelist,stTime,endTime);
            delete  mp4fileindex;
            break;
        }
    }
    if(filelist.count() == 0){
         return nullptr;
     }

    MP4VideoPlay *videoplay = new MP4VideoPlay();
    videoplay->setPlaylist(filelist);
    if(videoplay->startPlay()){
        return videoplay;
    }else {
        delete videoplay;
        return nullptr;
    }
}
#endif
