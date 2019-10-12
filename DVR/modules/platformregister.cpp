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

    registerMsg.ProvinceID = platformparam.readProvienceID().toShort();
    registerMsg.CountyID = platformparam.readCityID().toShort();
    array = platformparam.readProduceID().toLatin1();
    if(array.length() > sizeof (registerMsg.ManufacturerID)){
        len = sizeof (registerMsg.ManufacturerID);
    }else {
        len = array.length();
    }
    memcpy(registerMsg.ManufacturerID,array.data(),len);

    array = platformparam.readDeviceModel().toLatin1();
    if(array.length() > sizeof (registerMsg.DeviceModel)){
        len = sizeof (registerMsg.DeviceModel);
    }else {
        len = array.length();
    }
    memcpy(registerMsg.DeviceModel,array.data(),len);

    array = platformparam.readDeviceID().toLatin1();
    if(array.length() > sizeof (registerMsg.DeviceID)){
        len = sizeof (registerMsg.DeviceID);
    }else {
        len = array.length();
    }
    memcpy(registerMsg.DeviceID,array.data(),len);
    registerMsg.CarColor = platformparam.readCarColor();

    RemoteThread *remote = RemoteThread::getRemoteThread();
    msginfo.mMesgCache = (char *)(&registerMsg);
    msginfo.mSize = sizeof (RegisterMsg);
    msginfo.mMsgType = 0x0100;
    remote->msgQueueSendToNet(&msginfo,sizeof (RegisterMsg));
    qDebug()<<"sendMsgToRegister";
}

void PlatformRegister::serverUnRegister()
{
    MsgInfo msginfo;

    RemoteThread *remote = RemoteThread::getRemoteThread();
    msginfo.mMsgType = 0x3;
    msginfo.mSize = 0;
    remote->msgQueueSendToNet(&msginfo,0);

    if(mRegisterToken){
        RemoteThread::getRemoteThread()->unscheduleDelayedTask(mRegisterToken);
    }

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

