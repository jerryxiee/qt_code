#ifndef SYSTEMSET_H
#define SYSTEMSET_H

#include <QObject>
#include "config.h"
#include "netconfig.h"

class SystemSet : public QObject ,public Config
{
    Q_OBJECT
    Q_PROPERTY(QString UTC READ readUtc WRITE setUtc)
    Q_PROPERTY(bool dhcp READ readDhcp WRITE setDhcp)
public:
    explicit SystemSet(QObject *parent = nullptr, const QString filename = ".system.ini");

    Q_INVOKABLE void setSystemTime(QString datetime);
    Q_INVOKABLE void setSystemNet(bool dhcp,QString ip,QString mask,QString route,QString dns,QString mac);

    QString readUtc() const;
    void setUtc(const QString utc);

    bool readDhcp() const;
    void setDhcp(const bool value);


signals:


public slots:

private:
    const QString ROOTTIME = "Time";
    const QString ROOTNET = "network";
    const QString UTC = "UTC";
    const QString IPADDR = "ip";
    const QString MASK  = "mask";
    const QString ROUTE = "route";
    const QString DNS  = "dns";
    const QString MAC = "mac";
    const QString DHCP = "dhcp";

    NetConfig netcfg;
};

#endif // SYSTEMSET_H
