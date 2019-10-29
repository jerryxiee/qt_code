#include "circularregion.h"
#include "common/bcdtransform.h"
#include <QDateTime>

CircularRegion::CircularRegion(QObject *parent) : RegionBase(parent)
{

}

bool CircularRegion::isCircular() const
{
    return true;
}

bool CircularRegion::contaion(uint32_t latitude,uint32_t longitude)
{

    return false;
}

bool CircularRegion::updateRegion(CicularRegionAttr &attr)
{
    QByteArray bytearray;
    uint startT = 0,endT = 0 ;

    bytearray = BCDTransform::toArray(attr.startTime,sizeof (attr.startTime));
    if(bytearray.toInt() != 0){
        startT = QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.startTime,sizeof (attr.startTime))), "yyyyMMddhhmmss").toTime_t();
    }

    bytearray = BCDTransform::toArray(attr.endTime,sizeof (attr.endTime));
    if(bytearray.toInt() != 0){
        endT = QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime))), "yyyyMMddhhmmss").toTime_t();
    }

    mCenterLatitude = attr.centerLatitude;
    mCenterLongitude = attr.centerLongitude;
    mRegionRadius = attr.regionRadius;

    setStartTime(startT);
    setEndTime(endT);
    setRegionId(attr.regionId);
    setRegionAttr(attr.regionAttr);
    setTopSpeed(attr.topSpeed);
    setOverSpeedDuration(attr.overSpeedDuration);

    return true;
}

bool CircularRegion::modifyRegion(CicularRegionAttr &attr)
{
    return updateRegion(attr);

}
