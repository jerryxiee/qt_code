#ifndef NETCONFIG_H
#define NETCONFIG_H

#include <QObject>
#include <QProcess>
#include <QtNetwork>
#include <QTimer>

class NetConfig : public QObject
{
    Q_OBJECT
public:
    explicit NetConfig(QObject *parent = nullptr);

    void netSet(bool dhcp);
    void stopAuto();
//    void netAuto();
    void netcfg(QString ip,QString mask,QString route,QString mac);
    bool isOnline();
    QString getNetIpv4FromName(QString ethname);



signals:
    void sigLookUpHostResult(bool);

public slots:
    void netAuto();
    void checkNetWorkOnline(QString ip);
    void onLookupHost(QHostInfo host);
    void onNetStateChangedSlot(bool isOnline);


private:
    QProcess *mProcess;
    QTimer *mTimer;
    QNetworkConfigurationManager mNetManage;
};

#endif // NETCONFIG_H
