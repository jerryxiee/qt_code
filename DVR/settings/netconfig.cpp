#include "netconfig.h"

NetConfig::NetConfig(QObject *parent) : QObject(parent),mTimer(nullptr)
{
    connect(&mNetManage,SIGNAL(onlineStateChanged(bool)),this,SLOT(onNetStateChangedSlot(bool)));

}


void NetConfig::netAuto()
{
    mProcess = new QProcess(this);
    mProcess->startDetached("udhcpc -i eth0 -n 2");
    delete mProcess;

}

void NetConfig::netSet(bool dhcp)
{
//    if(!isOnline()){
        if(dhcp){
            mTimer = new QTimer(this);
            connect(mTimer,SIGNAL(timeout()),this,SLOT(netAuto()));
            netAuto();
            mTimer->start(10000);
        }
//    }else {
//        stopAuto();
//    }

}
void NetConfig::stopAuto()
{
    if(mTimer){
        mTimer->stop();
        disconnect(mTimer,SIGNAL(timeout()),this,SLOT(netAuto()));
        mTimer = nullptr;
    }
}

void NetConfig::netcfg(QString ip,QString mask,QString route,QString mac)
{
    mProcess = new QProcess(this);

    if(!mac.isEmpty()){
        mProcess->start("ifconfig eth0 down");
        mProcess->waitForFinished();
        mProcess->start("ifconfig eth0 hw ether "+mac);
        mProcess->waitForFinished();
    }

    if(!ip.isEmpty() && !mask.isEmpty()){
        mProcess->start("ifconfig eth0 "+ip+" netmask "+mask+" up");
        mProcess->waitForFinished();
    }

    if(!route.isEmpty()){
        mProcess->start("route add default gw "+route);
        mProcess->waitForFinished();
    }

    delete mProcess;

}


bool NetConfig::isOnline()
{
    return mNetManage.isOnline();

}

QString NetConfig::getNetIpv4FromName(QString ethname)
{
    QList<QNetworkInterface> allinterface = QNetworkInterface::allInterfaces();
    QNetworkInterface netinterface;

    for (int i = 0;i < allinterface.size();i++) {
        if(allinterface.at(i).name() == ethname){
            netinterface = allinterface.at(i);
            QList<QNetworkAddressEntry> addr = netinterface.addressEntries();
            for (i = 0;i < addr.size();i++) {
                if(addr.at(i).ip().protocol() == QAbstractSocket::IPv4Protocol){
                    return  addr.at(i).ip().toString();
                }
            }
        }
    }

    return nullptr;

}

void NetConfig::checkNetWorkOnline(QString ip)
{
    QHostInfo::lookupHost(ip,this,SLOT(onLookupHost(QHostInfo)));
}

void NetConfig::onLookupHost(QHostInfo host)
{
    if (host.error() != QHostInfo::NoError) {
        qDebug() << "Lookup failed:" << host.errorString();
        //网络未连接，发送信号通知
        emit sigLookUpHostResult(false);
    }
    else{
        emit sigLookUpHostResult(true);
    }
}

void NetConfig::onNetStateChangedSlot(bool isOnline)
{
    qDebug()<<"onNetStateChangedSlot";
    if(isOnline)
        stopAuto();
}
