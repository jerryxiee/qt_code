#include "himediaserversession.h"
#include "video/mp4fileindex.h"
#include "HW/video/hivpsssource.h"
#include <QDebug>
#include <QDateTime>


HiMediaServerSession *HiMediaServerSession::createNew(const QString sessionName)
{
    return new HiMediaServerSession(sessionName);
}

HiMediaServerSession *HiMediaServerSession::createNew(const char *ipAddr,int port)
{
    QString sessionName = generateSessionName(ipAddr,port);

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
        return "real"+QString::number(chn);
    }else {
        return "playback"+QString::number(chn);
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

HiMediaServerSubSession *HiMediaServerSession::lockupSubSessionByChn(int chn,bool isReal)
{
    QString name = generateSubSessionName(chn,isReal);

    return lockupSubSessionByName(name);
}

bool HiMediaServerSession::addMediaServerSubSession(HiMediaServerSubSession *session)
{
//    qDebug()<<"addsession name:"<<session->getName();
//    HiMediaServerSubSession * subsession = lockupSubSessionByName(session->getName());
//    if(subsession&&subsession->isAlive()){    //判断是否已经建立并处于连接状态
//        qDebug("%s:%d \n",__FUNCTION__,__LINE__);
//        return false;
//    }
//    if(subsession&&!subsession->isAlive()){    //会话已经断连接
//        qDebug("%s:%d\n",__FUNCTION__,__LINE__);
//        deleteSubSession(subsession->getName());
//    }

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
    qDebug()<<"delete subsession:"<<subSessionName;
    HiMediaServerSubSession *subsession = lockupSubSessionByName(subSessionName);
    mMutex.lock();
    if(subsession){
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

bool HiMediaServerSession::isEmpty() const
{
    return mSubSession.isEmpty();
}

void HiMediaServerSession::run()
{
    mRun = true;
    while (mRun) {
        mTaskScheduler->SingleStep(0);
    }

}

HiMediaServerSubSession *HiMediaServerSubSession::createNew(EncodeTaskScheduler &taskScheduler, HiMediaServerSession &mediaServerSession)
{
//    HiMediaServerSubSession *subSession = new HiMediaServerSubSession(taskScheduler,mediaServerSession);
//    if(!subSession){
//        return nullptr;
//    }
//    if(!subSession->isCreateSucess()){
//        delete subSession;
//        subSession = nullptr;
//    }

    return new HiMediaServerSubSession(taskScheduler,mediaServerSession);
}

HiMediaServerSubSession::HiMediaServerSubSession(EncodeTaskScheduler &taskScheduler, HiMediaServerSession &mediaServerSession, QObject *parent):QObject (parent),
#ifndef LUNUX_WIN
    mConsumers(nullptr),mVideoPlay(nullptr),
#endif
    mCreateSucess(false),mTaskScheduler(taskScheduler),mOurServerSession(mediaServerSession)
{
//#ifndef LUNUX_WIN
//    HiVpssSource * source;

//    if(!isReal){
//        mVideoPlay = createVideoPlay(chn,type,startTime,endTime);
//        source = HiVpssSource::createNew(mVideoPlay->getVpssGrp(),mVideoPlay->getVpssChn());
//    }else {
//        source = HiVpssSource::createNew(chn,VPSSCHN);
//    }
//    mConsumers = HiVencToMp4::createNew(source,VIDEO_NORM,PIC_HD720,SAMPLE_RC_CBR,0,25,0,PAYLOAD_TYPE,"stareamtest.mp4");
//    if(!mConsumers){
//        qDebug("%s:%d\n",__FUNCTION__,__LINE__);
//        return;
//    }
//    mTaskScheduler.setBackgroundHandling(mConsumers->getVencFd(),SOCKET_READABLE,HiFrameConsumer::doProcess,mConsumers);
//    mCreateSucess = true;
//    mAlive = true;
//    mSubSessionName = HiMediaServerSession::generateSubSessionName(chn,isReal);
//#endif

}

HiMediaServerSubSession::~HiMediaServerSubSession()
{
    close();
}

void HiMediaServerSubSession::onEndOfPlaySlot()
{
    close();
}

void HiMediaServerSubSession::close()
{
#ifndef LUNUX_WIN
    if(mConsumers){
        mTaskScheduler.turnOffBackgroundReadHandling(mConsumers->getVencFd());
        delete mConsumers;
        mConsumers = nullptr;
    }
    if(mVideoPlay){
        mVideoPlay->stopPlay();
        delete mVideoPlay;
        mVideoPlay = nullptr;
    }
#endif
    mAlive = false;
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

bool HiMediaServerSubSession::createNewMediaSubSession(StreamParam &param)
{

    mSubSessionName = HiMediaServerSession::generateSubSessionName(param.logicChannel,param.playType == 0);
    qDebug()<<"create subsession:"<<mSubSessionName;
    //step1:创建流媒体连接


    //step2:创建码流通道
#ifndef LUNUX_WIN
    HiVpssSource * source;

    if(param.playType != 0){
        QByteArray bytearray;
        uint sttime = 0;
        uint entime = 0;
        bytearray = BCDTransform::toArray(param.startTime,sizeof (param.startTime));
        if(bytearray.toInt() != 0){
            sttime = QDateTime::fromString("20"+QString(BCDTransform::toArray(param.endTime,sizeof (param.endTime))), "yyyyMMddhhmmss").toTime_t();
        }

        bytearray = BCDTransform::toArray(param.endTime,sizeof (param.endTime));
        if(bytearray.toInt() != 0){
            entime = QDateTime::fromString("20"+QString(BCDTransform::toArray(param.endTime,sizeof (param.endTime))), "yyyyMMddhhmmss").toTime_t();
        }
        if(entime == 0){
            entime = QDateTime::currentDateTime().toTime_t();
        }
        mVideoPlay = createVideoPlay(param.logicChannel,0,sttime,entime);
        connect(mVideoPlay,SIGNAL(endOfStream()),this,SLOT(onEndOfPlaySlot()));
        source = HiVpssSource::createNew(mVideoPlay->getVpssGrp(),mVideoPlay->getVpssChn());
    }else {
        qDebug("%s:%d cahnnel:%d\n",__FUNCTION__,__LINE__,param.logicChannel);
        source = HiVpssSource::createNew(param.logicChannel,VPSSCHN);
    }
    QString mp4filename = mSubSessionName+".mp4";

    QByteArray namearray = mp4filename.toLatin1();
    char *namep = namearray.data();
    mConsumers = HiVencToMp4::createNew(source,VIDEO_NORM,PIC_HD720,SAMPLE_RC_CBR,0,25,0,PAYLOAD_TYPE,namep);
    if(!mConsumers){
        qDebug("%s:%d\n",__FUNCTION__,__LINE__);
        return false;
    }
    mTaskScheduler.setBackgroundHandling(mConsumers->getVencFd(),SOCKET_READABLE,HiFrameConsumer::doProcess,mConsumers);

#endif
    mCreateSucess = true;
    mAlive = true;
    qDebug("%s:%d exit\n",__FUNCTION__,__LINE__);
    return true;
}
void HiMediaServerSubSession::mediaSubSessionCtr(StreamControl &control)
{
    if(control.streamType == 0){
        if(control.orderCtr == 0x02){
            pause(true);
        }else if(control.orderCtr == 0x03){
            play(true);
        }else if(control.orderCtr == 0x0){
            stop(true);
        }
    }else {
        if(control.orderCtr == 0x01){
            pause(false);
        }else if(control.orderCtr == 0x0){
            play(false);
        }else if(control.orderCtr == 0x02){
            stop(false);
        }else if(control.orderCtr == 0x05){
            uint pos = 0;
            QByteArray bytearray = BCDTransform::toArray(control.posTime,sizeof (control.posTime));
            if(bytearray.toInt() != 0){
                pos = QDateTime::fromString("20"+QString(BCDTransform::toArray(control.posTime,sizeof (control.posTime))), "yyyyMMddhhmmss").toTime_t();
            }
            setCurPosition(pos);
        }
    }

}

void HiMediaServerSubSession::pause(bool isReal)
{
    if(isReal){
        if(mConsumers){
            static_cast<HiVencConsumer*>(mConsumers)->stopStreamRecv();
        }
    }else {
        if(mVideoPlay){
            mVideoPlay->pause();
        }
    }

}
void HiMediaServerSubSession::play(bool isReal)
{
    if(isReal){
        if(mConsumers){
            static_cast<HiVencConsumer*>(mConsumers)->startStreamRecv();
        }
    }else {
        if(mVideoPlay){
            mVideoPlay->play();
        }
    }

}

void HiMediaServerSubSession::stop(bool isReal)
{
    Q_UNUSED(isReal)
    close();
}

void HiMediaServerSubSession::setCurPosition(qint64 position)
{
    qint64 pos;

    if(mVideoPlay){
        if(position > mVideoPlay->getFileEndTime()){
            pos = mVideoPlay->getFileEndTime() - mVideoPlay->getFileStartTime();
        }else if(position < mVideoPlay->getFileStartTime()){
            pos = 0;
        }else {
            pos = position - mVideoPlay->getFileStartTime();
        }
        mVideoPlay->setPosition(pos);
    }

}
