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
    Q_PROPERTY(QString authNum READ readAuthNum)
public:
    Q_INVOKABLE void updateServerInfo(int type);
    Q_INVOKABLE void updatePositionSet();

    explicit PlatformFunModel(QObject *parent = nullptr);

    PlatFormStatus readMainServerStatus() const;
    PlatFormStatus readBackupServerStatus() const;
    QString readAuthNum() const;
    bool readEnable() const;
    void setEnable(bool enable);
signals:
    void serverInfoChanged(int type);
    void positionSetChanged();
    void mainServerStatusChanged(PlatFormStatus &status);
    void backupServerStatusChanged(PlatFormStatus &status);
    void enabledChanged();
    void authNumChanged(QString value);

public slots:
    void mainServerStatusChangedSlot(PlatFormStatus &status);
    void backupServerStatusChangedSlot(PlatFormStatus &status);
    void authNumChangeSlot(QString value);
};

#endif // PLATFORMFUNMODEL_H
