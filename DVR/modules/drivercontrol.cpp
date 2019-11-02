#include "drivercontrol.h"
#include "settings/platformset.h"

DriverControl::DriverControl(QObject *parent) : QObject(parent),mRegionType(REGIONTYPE::None)
{
    updateInfo();
}


void DriverControl::updateInfo()
{
    PlatformSet platformset;

    mOverSpeedDurationLimit = platformset.readOverspeedDuration().toUInt();
    mDriverDurationLimit = platformset.readDriverDuration().toUInt();
    mTopSpeed = platformset.readTopSpeed().toUInt();
    mOverSpeedSad = platformset.readSadOverSpeed().toUInt();
    mDriverDurationSad = platformset.readSadFatigueDriving().toUInt();
    mDayDriverDurationLimit = platformset.readDriverDayDuration().toUInt();

}

void DriverControl::driverParamChanged()
{
    updateInfo();
}

void DriverControl::enterRegionSlot(REGIONTYPE type,uint32_t id,bool enter)
{
    if(enter){
        mRegionType = type;
        mRegionId = id;
    }else {
        mRegionType = REGIONTYPE::None;
        mRegionId = 0;
        mCurSpeedLimit = mTopSpeed;
        mCurOverSpeedDurationLimit = mOverSpeedDurationLimit;
    }
}

void DriverControl::speedLimitChnaged(uint value, uint duration)
{
    mCurSpeedLimit = value;
    mCurOverSpeedDurationLimit = duration;
}
