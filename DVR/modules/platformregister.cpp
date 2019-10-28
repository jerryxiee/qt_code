#include "platformregister.h"
#include "communication/jtt808define.h"
#include "settings/platformset.h"
#include <QDebug>

PlatformRegister* PlatformRegister::mPlatformRegister = nullptr;

PlatformRegister *PlatformRegister::getPlatformRegister()
{
    if(!mPlatformRegister){
        mPlatformRegister = new PlatformRegister();
    }

    return mPlatformRegister;
}

PlatformRegister::PlatformRegister(QObject *parent) : QObject(parent),mMainStatus(DisConnect),mBackupStatus(DisConnect)
  ,mRegisterToken(nullptr)
{
    qRegisterMetaType<PlatFormStatus>("PlatFormStatus");
    qRegisterMetaType<PlatFormStatus>("PlatFormStatus&");

    PlatformSet platformparam;

    mRegisterEnable = platformparam.readRegisterEnable();

    if(!sendServerInfo()){
        qDebug()<<"send server info error";
    }

    setMainServerStatus(mMainStatus);
}


bool PlatformRegister::readEnableRegister() const
{
    return mRegisterEnable;
}

void PlatformRegister::serverRegister(void *object)
{
    PlatformRegister *platformObject = static_cast<PlatformRegister *>(object);
    if(platformObject->mRegisterEnable)
        platformObject->serverRegister();
}

void PlatformRegister::serverRegister()
{
    MsgInfo msginfo;
    RegisterMsg registerMsg;
    QByteArray array;
    int len;

    mMainStatus = Connecting;
    mainServerStatusChanged(mMainStatus);

    PlatformSet platformparam;

    registerMsg.provinceID = platformparam.readProvienceID().toShort();
    registerMsg.countyID = platformparam.readCityID().toShort();
    array = platformparam.readProduceID().toLatin1();
    if(array.length() > sizeof (registerMsg.manufacturerID)){
        len = sizeof (registerMsg.manufacturerID);
    }else {
        len = array.length();
    }
    memcpy(registerMsg.manufacturerID,array.data(),len);

    array = platformparam.readDeviceModel().toLatin1();
    if(array.length() > sizeof (registerMsg.deviceModel)){
        len = sizeof (registerMsg.deviceModel);
    }else {
        len = array.length();
    }
    memcpy(registerMsg.deviceModel,array.data(),len);

    array = platformparam.readDeviceID().toLatin1();
    if(array.length() > sizeof (registerMsg.deviceID)){
        len = sizeof (registerMsg.deviceID);
    }else {
        len = array.length();
    }
    memcpy(registerMsg.deviceID,array.data(),len);
    registerMsg.carColor = platformparam.readCarColor();

    array = platformparam.readCarLisence().toLocal8Bit();
    if(array.length() > sizeof (registerMsg.carLicense)){
        len = sizeof (registerMsg.carLicense);
    }else {
        len = array.length();
    }
    memcpy(registerMsg.carLicense,array.data(),len);

    RemoteThread *remote = RemoteThread::getRemoteThread();
    msginfo.mMesgCache = (char *)(&registerMsg);
    msginfo.mSize = sizeof (RegisterMsg);
    msginfo.mMsgType = 0x0100;
    remote->msgQueueSendToNet(&msginfo,sizeof (RegisterMsg),nullptr,nullptr);
    qDebug()<<"sendMsgToRegister";
}

void PlatformRegister::serverUnRegister()
{
    MsgInfo msginfo;

    RemoteThread *remote = RemoteThread::getRemoteThread();
    msginfo.mMsgType = 0x3;
    msginfo.mSize = 0;
    remote->msgQueueSendToNet(&msginfo,0,nullptr,nullptr);

    if(mRegisterToken){
        RemoteThread::getRemoteThread()->unscheduleDelayedTask(mRegisterToken);
    }

    setAuthNum(QString());
    mMainStatus = DisConnect;
    mBackupStatus = DisConnect;
    mainServerStatusChanged(mMainStatus);
    backupServerStatusChanged(mBackupStatus);

}

PlatFormStatus PlatformRegister::getMainServerStatus() const
{
    return mMainStatus;
}
void PlatformRegister::setMainServerStatus(PlatFormStatus status)
{
    mMainStatus = status;
    if(mMainStatus != Connected){
        mRegisterToken = RemoteThread::getRemoteThread()->scheduleDelayedTask(3000000,serverRegister,this);
    }

    mainServerStatusChanged(mMainStatus);
}


//void PlatformRegister::backupServerRegister(void *object)
//{
//    PlatformRegister *platformObject = static_cast<PlatformRegister *>(object);
//    platformObject->backupServerRegister();
//}

//void PlatformRegister::backupServerRegister()
//{
//    MsgInfo msginfo;
//    RegisterMsg registerMsg;
//    QByteArray array;
//    int len;

//    mMainStatus = Connecting;
//    backupServerStatusChanged(mBackupStatus);

//    PlatformSet platformparam;

//    registerMsg.ProvinceID = platformparam.readProvienceID().toShort();
//    registerMsg.CountyID = platformparam.readCityID().toShort();
//    array = platformparam.readProduceID().toLatin1();
//    if(array.length() > sizeof (registerMsg.ManufacturerID)){
//        len = sizeof (registerMsg.ManufacturerID);
//    }else {
//        len = array.length();
//    }
//    memcpy(registerMsg.ManufacturerID,array.data(),len);

//    array = platformparam.readDeviceModel().toLatin1();
//    if(array.length() > sizeof (registerMsg.DeviceModel)){
//        len = sizeof (registerMsg.DeviceModel);
//    }else {
//        len = array.length();
//    }
//    memcpy(registerMsg.DeviceModel,array.data(),len);

//    array = platformparam.readDeviceID().toLatin1();
//    if(array.length() > sizeof (registerMsg.DeviceID)){
//        len = sizeof (registerMsg.DeviceID);
//    }else {
//        len = array.length();
//    }
//    memcpy(registerMsg.DeviceID,array.data(),len);
//    registerMsg.CarColor = platformparam.readCarColor();

//    RemoteThread *remote = RemoteThread::getRemoteThread();
//    msginfo.mMesgCache = (char *)(&registerMsg);
//    msginfo.mSize = sizeof (RegisterMsg);
//    msginfo.mMsgType = 0x0100;
//    remote->msgQueueSendToNet(&msginfo,sizeof (RegisterMsg));
//}

PlatFormStatus PlatformRegister::getBackupServerStatus() const
{
    return mBackupStatus;
}

void PlatformRegister::setBackupServerStatus(PlatFormStatus status)
{
    mBackupStatus = status;

    backupServerStatusChanged(mBackupStatus);
}

void PlatformRegister::setAuthNum(QString value)
{
    mAuthNum = value;

    emit authNumChanged(value);
}
QString PlatformRegister::readAuthNum() const
{
    return mAuthNum;
}

bool PlatformRegister::sendServerInfo()
{
    MsgInfo msginfo;
    ServerInfo serverinfo;
    QByteArray array;
    int len;
    QString str;

    PlatformSet platformparam;

    serverinfo.registerType = platformparam.readRegisterType();
    str = platformparam.readMainServerIp();
    array = str.toLatin1();
    if(array.length() > sizeof (serverinfo.mainServerIp)){
        len = sizeof (serverinfo.mainServerIp);
    }else {
        len = array.length();
    }
    serverinfo.mainIpLen = array.length();
    memcpy(serverinfo.mainServerIp,array.data(),len);
//    serverinfo.mainServerIp = platformparam.readMainServerIp().toUInt();
    serverinfo.mainTcpPort = platformparam.readMainServerTcpPort().toUShort();
    serverinfo.mainUdpPort = platformparam.readMainServerUdpPort().toUShort();
    serverinfo.mainServerUseTcp = platformparam.readMainEnableTcp();
//    serverinfo.backupServerIp = platformparam.readBackupServerIp().toUInt();
    str = platformparam.readBackupServerIp();
    array = str.toLatin1();
    if(array.length() > sizeof (serverinfo.backupServerIp)){
        len = sizeof (serverinfo.backupServerIp);
    }else {
        len = array.length();
    }
    serverinfo.backupIpLen = array.length();
    memcpy(serverinfo.backupServerIp,array.data(),len);
    serverinfo.backupTcpPort = platformparam.readBackupServerTcpPort().toUShort();
    serverinfo.backupUdpPort = platformparam.readBackupServerUdpPort().toUShort();
    serverinfo.backupServerUseTcp = platformparam.readSecEnableTcp();
    array = platformparam.readPhoneNumber().toLatin1();
    if(array.length() > 12){
        len = 12;
    }else {
        len = array.length();
    }
    memcpy(serverinfo.telephoneNum,array.data(),len);
    serverinfo.tcpTimeout = platformparam.readTcpAnswerTimeout().toUShort();
    serverinfo.tcpRepeatTimes = platformparam.readTcpRepeatTimes().toUShort();
    serverinfo.udpTimeout = platformparam.readUdpAnswerTimeout().toUShort();
    serverinfo.udpRepeatTimes = platformparam.readUdpRepeatTimes().toUShort();
    serverinfo.smsTimeout = platformparam.readSmsAnswerTimeout().toUShort();
    serverinfo.smsRepeatTimes = platformparam.readSmsRepeatTimes().toUShort();
    serverinfo.heratBeatTime = platformparam.readHeartBeat().toUInt();


    RemoteThread *remote = RemoteThread::getRemoteThread();
//    msginfo.mMesgCache = static_cast<char *>(&serverinfo);
    msginfo.mMesgCache = (char *)(&serverinfo);
    msginfo.mSize = sizeof (ServerInfo);
    msginfo.mMsgType = SERVERINFOMSGTYPE;
    return remote->msgQueueSendToNet(&msginfo,sizeof (RegisterMsg),nullptr,nullptr);
}

void PlatformRegister::updateServerInfo(int type)
{
    qDebug()<<"updateserverinfo type:"<<type;
    sendServerInfo();

    if(type == 0){
        serverUnRegister();
        setMainServerStatus(DisConnect);
    }
}
