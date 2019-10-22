#ifndef HIMEDIASERVERSESSION_H
#define HIMEDIASERVERSESSION_H

#include <QThread>
#include <QHash>
#include "video/encodetaskscheduler.h"
#include <QMutex>
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
    HiMediaServerSubSession *lockupSubSessionByName(QString subSessionName);
    bool addMediaServerSubSession(HiMediaServerSubSession *session);
    void deleteSubSession(QString subSessionName);
    void deleteAllSubSession();
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
};


class HiMediaServerSubSession
{
public:
    static HiMediaServerSubSession *createNew(EncodeTaskScheduler &taskScheduler,
                                       HiMediaServerSession &mediaServerSession,
                                       int chn,int type,bool isReal = true,
                                       uint startTime = 0,uint endTime = 0);

    QString getName() const {return mSubSessionName;}
//    int getSelectFd() const;
    bool isCreateSucess() const{return mCreateSucess;}
//#ifndef LUNUX_WIN
//    HiVencToMp4 *getHiVenc() const{return mTestVenc;}
//#endif

    ~HiMediaServerSubSession();
protected:
    HiMediaServerSubSession(EncodeTaskScheduler &taskScheduler,
                            HiMediaServerSession &mediaServerSession,
                            int chn,int type,bool isReal,
                            uint startTime,uint endTime);

private:
#ifndef LUNUX_WIN
    MP4VideoPlay *createVideoPlay(int Chn,int type,uint stTime,uint endTime);
#endif

private:
    QString mSubSessionName;

#ifndef LUNUX_WIN
    const int VPSSCHN = VPSS_CHN1;
    HiVencToMp4 *mTestVenc;
    MP4VideoPlay *mVideoPlay;
#endif
    bool mCreateSucess;
    EncodeTaskScheduler &mTaskScheduler;
    HiMediaServerSession &mOurServerSession;

};

#endif // HIMEDIASERVERSESSION_H
