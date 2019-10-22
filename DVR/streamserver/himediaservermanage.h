#ifndef HIMEDIASERVERMANAGE_H
#define HIMEDIASERVERMANAGE_H

#include <QObject>
#include "himediaserversession.h"
#include <QHash>

class HiMediaServerManage : public QObject
{
    Q_OBJECT
public:
    HiMediaServerSession *lockupMediaServerSession(QString sessionName);
    void addServerMediaSession(HiMediaServerSession* serverMediaSession);
    void removeServerMediaSession(HiMediaServerSession* serverMediaSession);
    void removeServerMediaSession(QString sessionName);
    void removeAllServerMediaSession();
    bool createNewServerMediaSession();

    explicit HiMediaServerManage(QObject *parent = nullptr);

signals:

public slots:

private:
    QHash<QString,void *> mMediaServerSession;
};

#endif // HIMEDIASERVERMANAGE_H
