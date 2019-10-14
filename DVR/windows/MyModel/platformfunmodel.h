#ifndef PLATFORMFUNMODEL_H
#define PLATFORMFUNMODEL_H

#include <QObject>
#include "modules/platformregister.h"

class PlatformFunModel : public QObject
{
    Q_OBJECT
    Q_ENUMS(PlatFormStatus)
    Q_PROPERTY(PlatFormStatus mianServerStatus READ readMainServerStatus NOTIFY mainServerStatusChanged)
    Q_PROPERTY(PlatFormStatus backupServerStatus READ readBackupServerStatus NOTIFY backupServerStatusChanged)
    Q_PROPERTY(bool enable READ readEnable WRITE setEnable NOTIFY enabledChanged)
public:
    Q_INVOKABLE void updateServerInfo(int type);

    explicit PlatformFunModel(QObject *parent = nullptr);

    PlatFormStatus readMainServerStatus() const;
    PlatFormStatus readBackupServerStatus() const;
    bool readEnable() const;
    void setEnable(bool enable);
signals:
    void serverInfoChanged(int type);
    void mainServerStatusChanged(PlatFormStatus &status);
    void backupServerStatusChanged(PlatFormStatus &status);
    void enabledChanged();

public slots:
    void mainServerStatusChangedSlot(PlatFormStatus &status);
    void backupServerStatusChangedSlot(PlatFormStatus &status);
};

#endif // PLATFORMFUNMODEL_H
