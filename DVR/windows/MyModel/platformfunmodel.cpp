#include "platformfunmodel.h"
#include <QDebug>

PlatformFunModel::PlatformFunModel(QObject *parent) : QObject(parent)
{
    connect(PlatformRegister::getPlatformRegister(),SIGNAL(mainServerStatusChanged(PlatFormStatus &)),this,SLOT(mainServerStatusChangedSlot(PlatFormStatus &)));
    connect(PlatformRegister::getPlatformRegister(),SIGNAL(backupServerStatusChanged(PlatFormStatus &)),this,SLOT(backupServerStatusChangedSlot(PlatFormStatus &)));
    connect(this,SIGNAL(serverInfoChanged(int)),PlatformRegister::getPlatformRegister(),SLOT(updateServerInfo(int)));
}

PlatFormStatus PlatformFunModel::readMainServerStatus() const
{
    return PlatformRegister::getPlatformRegister()->getMainServerStatus();
}

PlatFormStatus PlatformFunModel::readBackupServerStatus() const
{
    return PlatformRegister::getPlatformRegister()->getBackupServerStatus();
}

bool PlatformFunModel::readEnable() const
{
    return PlatformRegister::getPlatformRegister()->readEnableRegister();
}

void PlatformFunModel::setEnable(bool enable)
{
    if(enable){
        if(readMainServerStatus() == DisConnect){
            qDebug()<<"register";
            PlatformRegister::serverRegister(PlatformRegister::getPlatformRegister());
//            PlatformRegister::getPlatformRegister()->serverRegister();
        }
    }else {
        PlatformRegister::getPlatformRegister()->serverUnRegister();
    }
}

void PlatformFunModel::mainServerStatusChangedSlot(PlatFormStatus &status)
{
    emit mainServerStatusChanged(status);
}

void PlatformFunModel::backupServerStatusChangedSlot(PlatFormStatus &status)
{
    emit backupServerStatusChanged(status);
}

void PlatformFunModel::updateServerInfo(int type)
{
    serverInfoChanged(type);
}

