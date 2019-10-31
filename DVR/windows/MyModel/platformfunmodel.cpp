#include "platformfunmodel.h"
#include "modules/positionctr.h"
#include <QDebug>

PlatformFunModel::PlatformFunModel(QObject *parent) : QObject(parent)
{
    connect(PlatformRegister::getPlatformRegister(),SIGNAL(mainServerStatusChanged(PlatFormStatus &)),this,SLOT(mainServerStatusChangedSlot(PlatFormStatus &)));
    connect(PlatformRegister::getPlatformRegister(),SIGNAL(backupServerStatusChanged(PlatFormStatus &)),this,SLOT(backupServerStatusChangedSlot(PlatFormStatus &)));
    connect(PlatformRegister::getPlatformRegister(),SIGNAL(authNumChanged(QString)),this,SLOT(authNumChangeSlot(QString)));
    connect(this,SIGNAL(serverInfoChanged(int)),PlatformRegister::getPlatformRegister(),SLOT(updateServerInfo(int)));
    connect(this,SIGNAL(positionSetChanged()),PositionCtr::getPositionCtr(),SLOT(onPositionSetChanged()));
}

PlatFormStatus PlatformFunModel::readMainServerStatus() const
{
    return PlatformRegister::getPlatformRegister()->getMainServerStatus();
}

PlatFormStatus PlatformFunModel::readBackupServerStatus() const
{
    return PlatformRegister::getPlatformRegister()->getBackupServerStatus();
}

QString PlatformFunModel::readAuthNum() const
{
    return PlatformRegister::getPlatformRegister()->readAuthNum();
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
void PlatformFunModel::authNumChangeSlot(QString value)
{
    emit authNumChanged(value);
}

void PlatformFunModel::updateServerInfo(int type)
{
    serverInfoChanged(type);
}

void PlatformFunModel::updatePositionSet()
{
    positionSetChanged();
}
