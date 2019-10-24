#ifndef HIMEDIASERVERSESSION_H
#define HIMEDIASERVERSESSION_H

#include <QThread>
#include <QHash>
#include "video/encodetaskscheduler.h"
#include <QMutex>
#include "communication/jtt808define.h"
#include "common/bcdtransform.h"
#ifndef LUNUX_WIN
#include "HW/video/hivenctomp4.h"
#include "video/mp4play.h"
#endif

class HiMediaServerSubSession;

class HiMediaServerSession : public QThread
{
    Q_OBJECT
public:
    static HiMediaServerSession *createNew(const QString sessionName);
    static HiMediaServerSession *createNew(const char *ipAddr,int port);
    HiMediaServerSubSession *lockupSubSessionByName(QString subSessionName);
    HiMediaServerSubSession *lockupSubSessionByChn(int chn,bool isReal);
    bool addMediaServerSubSession(HiMediaServerSubSession *session);
    void deleteSubSession(QString subSessionName);
    void deleteAllSubSession();
    bool isEmpty() const;
    ~HiMediaServerSession();

    static QString generateSessionName(const char *ipAddr,int port);
    static QString generateSubSessionName(int chn,bool isReal);

    QString getSessionName() const {return mSessionName;}
    EncodeTaskScheduler &getTaskScheduler() const{return *mTaskScheduler;}
protected:
    explicit HiMediaServerSession(const QString sessionName,QObject *parent = nullptr);
    virtual void run();
private:


signals:

public slots:


private:
    EncodeTaskScheduler *mTaskScheduler;
    QString mSessionName;
    QHash<QString,void *> mSubSession;
    QMutex mMutex;
    bool mRun;
//    bool mFirstCreate;
};


class HiMediaServerSubSession:public QObject
{
    Q_OBJECT
public:
    static HiMediaServerSubSession *createNew(EncodeTaskScheduler &taskScheduler,
                                       HiMediaServerSession &mediaServerSession);

    QString getName() const {return mSubSessionName;}
    bool isCreateSucess() const{return mCreateSucess;}
    bool isAlive() const {return mAlive;}

    bool createNewMediaSubSession(StreamParam &param);
    void mediaSubSessionCtr(StreamControl &control);
    void pause(bool isReal);
    void play(bool isReal);
    void stop(bool isReal);
    void setCurPosition(qint64 position);

    ~HiMediaServerSubSession();
protected:
    HiMediaServerSubSession(EncodeTaskScheduler &taskScheduler,
                            HiMediaServerSession &mediaServerSession,QObject *parent = nullptr);

private:
#ifndef LUNUX_WIN
    MP4VideoPlay *createVideoPlay(int Chn,int type,uint stTime,uint endTime);
#endif
    void close();

signals:

public slots:
    void onEndOfPlaySlot();

private:
    QString mSubSessionName;

#ifndef LUNUX_WIN
    const int VPSSCHN = VPSS_CHN1;
    HiVencConsumer *mConsumers;
    MP4VideoPlay *mVideoPlay;
    uint mFileStartTime;
    uint mFileEndTime;
#endif
    bool mCreateSucess;
    EncodeTaskScheduler &mTaskScheduler;
    HiMediaServerSession &mOurServerSession;
    bool mAlive;

};

#endif // HIMEDIASERVERSESSION_H
