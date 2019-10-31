#include "positionctr.h"
#include "communication/remotethread.h"

PositionCtr::PositionCtr(QObject *parent) : QObject(parent),mCurRegionId(0)
{
    mRegionControl = RegionControl::getRegionControlObject();
}

void PositionCtr::setAlarmFlag(uint32_t flag)
{
    mAlarmFlag = flag;
    alarmFalgChanged();
}

void PositionCtr::setStatus(uint32_t status)
{
    mPositionStatus = status;
    statusChanged();
}

void PositionCtr::urgentAlarmFlagChanged(bool enable)
{

}
void PositionCtr::overSpeedAlarmFlagChanged(bool enable)
{

}
void PositionCtr::fatigueDrivingAlarmFlagChanged(bool enable)
{}
void PositionCtr::dangerWarningAlarmFlagChanged(bool enable)
{}
void PositionCtr::gnssFaultAlarmFlagChanged(bool enable)
{

}
void PositionCtr::antennaNotConnectAlarmFlagChanged(bool enable)
{

}
void PositionCtr::urgentCircuitFlagChanged(bool enable)
{

}
void PositionCtr::undervoltageAlarmFlagChanged(bool enable)
{

}
void PositionCtr::powerOffAlarmFlagChanged(bool enable)
{

}
void PositionCtr::lcdFaultAlarmFlagChanged(bool enable)
{

}
void PositionCtr::ttsFaultAlarmFlagChanged(bool enable)
{

}
void PositionCtr::cameraFaultAlarmFlagChanged(bool enable)
{

}
void PositionCtr::icFaultAlarmFlagChanged(bool enable)
{

}
void PositionCtr::overSpeedWarningFlagChanged(bool enable)
{

}
void PositionCtr::fatigueDrivingWarningFlagChanged(bool enable)
{

}
void PositionCtr::driverTimeoutAlarmFlagChanged(bool enable)
{

}
void PositionCtr::stopTimeoutAlarmFlagChanged(bool enable)
{

}
void PositionCtr::inOrOutRegionAlarmFlagChanged(bool enable)
{

}
void PositionCtr::inOrOutRouteAlarmFlagChanged(bool enable)
{

}
void PositionCtr::driveLowTimeAlarmFlagChanged(bool enable)
{

}
void PositionCtr::routeDeviationAlarmFlagChanged(bool enable)
{

}
void PositionCtr::vssFaultAlarmFlagChanged(bool enable)
{

}
void PositionCtr::oilAbnormalAlarmFlagChanged(bool enable)
{

}
void PositionCtr::vehicleTheftAlarmFlagChanged(bool enable)
{

}
void PositionCtr::illegalIgnitionAlarmFlagChanged(bool enable)
{

}
void PositionCtr::illegalDisplacementAlarmFlagChanged(bool enable)
{

}
void PositionCtr::collisionWarningFlagChanged(bool enable)
{

}
void PositionCtr::rolloverWarningFlagChanged(bool enable)
{

}
void PositionCtr::illegalOpeningAlarmFlagChanged(bool enable)
{

}
void PositionCtr::accStatusChanged(bool status)
{

}
void PositionCtr::locationStatusChanged(bool status)
{

}
void PositionCtr::latitudeStatusChanged(bool status)
{

}
void PositionCtr::longitudeStatusChanged(bool status)
{

}
void PositionCtr::operateStatusChanged(bool status)
{

}
void PositionCtr::encryptionStatusChanged(bool status)
{

}
void PositionCtr::ladenCargoStatusChanged(bool status)
{

}
void PositionCtr::oilCircuitStatusChanged(bool status)
{

}
void PositionCtr::circuitStatusChanged(bool status)
{

}
void PositionCtr::doorLockStatusChanged(bool status)
{

}
void PositionCtr::door1LockStatusChanged(bool status)
{

}
void PositionCtr::door2LockStatusChanged(bool status)
{

}
void PositionCtr::door3LockStatusChanged(bool status)
{

}
void PositionCtr::door4LockStatusChanged(bool status)
{

}
void PositionCtr::door5LockStatusChanged(bool status)
{

}
void PositionCtr::gpsEnableStatusChanged(bool status)
{

}
void PositionCtr::bdEnableStatusChanged(bool status)
{

}
void PositionCtr::glonassEnableStatusChanged(bool status)
{

}
void PositionCtr::galileoEnableStatusChanged(bool status)
{

}

void PositionCtr::setLatitude(uint32_t value)
{
    mLatitude = value;
}
void PositionCtr::setLongitude(uint32_t value)
{
    mLongitude = value;
}
void PositionCtr::setSpeed(uint16_t value)
{
    mSpeed = value;
}
void PositionCtr::setAtitude(uint16_t value)
{
    mAtitude = value;
}
void PositionCtr::setDirectionAngle(uint16_t value)
{
    mDirectionAngle = value;
}

void reportPosition(void *object)
{

}

void PositionCtr::reportPosition(QList<PositionExtensionInfo> &infolist)
{
    PositionBaseInfo baseinfo;
    char *msgbuf = nullptr;
    char *pBuf;
    MsgInfo msginfo;
    int msglen;

    memset(&baseinfo,0x0,sizeof (PositionBaseInfo));

    if(infolist.count() != 0){
        msglen = sizeof (PositionBaseInfo)+infolist.count()*sizeof (PositionExtensionInfo);
        pBuf = msgbuf = (char *)malloc(msglen*2);
        if(!msgbuf){
            return;
        }
        memcpy(pBuf,&baseinfo,sizeof (PositionBaseInfo));
        pBuf += sizeof (PositionBaseInfo);
        for (int i = 0;i < infolist.count();i++) {
            memcpy(pBuf,&infolist[i],sizeof (PositionExtensionInfo));
            pBuf += sizeof (PositionExtensionInfo);
        }
        msginfo.mSize = msglen;
        msginfo.mMesgCache = msgbuf;
    }else {
        msginfo.mSize = sizeof (PositionBaseInfo);
        msginfo.mMesgCache = (char *)&baseinfo;
    }

    msginfo.mMsgType = 0x0200;
    RemoteThread::getRemoteThread()->msgQueueSendToNet(&msginfo, msginfo.mSize,nullptr,nullptr);
    if(!msgbuf){
        free(msgbuf);
    }

}
