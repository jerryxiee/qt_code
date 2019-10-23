#ifndef HIMEDIASERVERMANAGE_H
#define HIMEDIASERVERMANAGE_H

#include <QObject>
#include "himediaserversession.h"
#include "communication/jtt808define.h"
#include <QHash>

class HiMediaServerManage : public QObject
{
    Q_OBJECT
public:
    HiMediaServerSession *lockupMediaServerSession(QString sessionName);
    HiMediaServerSession *lockupMediaServerSession(const QString ipAddr,int port);
    void addServerMediaSession(HiMediaServerSession* serverMediaSession);
    void removeServerMediaSession(HiMediaServerSession* serverMediaSession);
    void removeServerMediaSession(QString sessionName);
    void removeAllServerMediaSession();
    bool createNewServerMediaSession(StreamParam &param);
    bool serverMediaSessionCtr(StreamControl &control);

    explicit HiMediaServerManage(QObject *parent = nullptr);
    ~HiMediaServerManage();

signals:

public slots:

private:
    QHash<QString,void *> mMediaServerSession;
//    bool mFirstCreate;
};

#endif // HIMEDIASERVERMANAGE_H
