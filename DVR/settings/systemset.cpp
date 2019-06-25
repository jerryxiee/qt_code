#include "systemset.h"
#include "QDebug"
#include <QRegExp>

SystemSet::SystemSet(QObject *parent, const QString &filename) : QObject(parent),Config (filename)
{

}

void SystemSet::setSystemTime(QString datetime)
{
    qDebug()<<"set time:"<<datetime;
    QByteArray tmp = ("date -s "+datetime).toLatin1();
    system(tmp.data());
}


QString SystemSet::readUtc() const
{
    return Config::getConfig(ROOTTIME,UTC).toString();
}

void SystemSet::setUtc(const QString utc)
{
    if(readUtc() != utc){
        Config::setConfig(ROOTTIME,UTC,utc);
        qDebug()<<"UTC:"<<utc;
    }
}

void SystemSet::setSystemNet(bool dhcp,QString ip,QString mask,QString route,QString dns,QString mac)
{
    qDebug()<<"setNet";

    Config::setConfig(ROOTNET,DHCP,dhcp);
    Config::setConfig(ROOTNET,IPADDR,ip);
    Config::setConfig(ROOTNET,MASK,mask);
    Config::setConfig(ROOTNET,ROUTE,route);
    Config::setConfig(ROOTNET,DNS,dns);
    Config::setConfig(ROOTNET,MAC,mac);

    netcfg.stopAuto();
    netcfg.netcfg(ip,mask,route,mac);


}

bool SystemSet::readDhcp() const
{
    return Config::getConfig(ROOTNET,DHCP).toBool();
}

void SystemSet::setDhcp(const bool value)
{
    if(readDhcp() != value){
        Config::setConfig(ROOTNET,DHCP,value);
        netcfg.netSet(value);
        qDebug()<<"DHCP:"<<value;
    }

}
