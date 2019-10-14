#ifndef PLATFORMREGISTER_H
#define PLATFORMREGISTER_H

#include <QObject>
#include <QMetaType>
#include "../communication/remotethread.h"

typedef enum{
    DisConnect,
    Connecting,
    Connected
} PlatFormStatus;

Q_DECLARE_METATYPE(PlatFormStatus)

class PlatformRegister : public QObject
{
    Q_OBJECT
public:
    static PlatformRegister *getPlatformRegister() ;

    static void serverRegister(void *object);
    void serverRegister();
    void serverUnRegister();
    PlatFormStatus getMainServerStatus() const;
    void setMainServerStatus(PlatFormStatus status);

//    static void backupServerRegister(void *object);
//    void backupServerRegister();
    PlatFormStatus getBackupServerStatus() const;
    void setBackupServerStatus(PlatFormStatus status);
    bool readEnableRegister() const;

    bool sendServerInfo();

protected:
    explicit PlatformRegister(QObject *parent = nullptr);

signals:
    void mainServerStatusChanged(PlatFormStatus &status);
    void backupServerStatusChanged(PlatFormStatus &status);

public slots:
    void updateServerInfo(int type);
private:
    PlatFormStatus mMainStatus;
    PlatFormStatus mBackupStatus;
    TaskToken mRegisterToken;
    static PlatformRegister* mPlatformRegister;
    bool mRegisterEnable;
};

#endif // PLATFORMREGISTER_H
