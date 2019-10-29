#include "rectangleregion.h"
#include "common/bcdtransform.h"
#include <QDateTime>

RectangleRegion::RectangleRegion(QObject *parent) : RegionBase(parent)
{

}

bool RectangleRegion::isRectangle() const
{
    return true;
}

bool RectangleRegion::contaion(uint32_t latitude,uint32_t longitude)
{
    return false;
}

bool RectangleRegion::modifyRegion(RectangleRegionAttr &attr)
{
    return updateRegion(attr);
}
bool RectangleRegion::updateRegion(RectangleRegionAttr &attr)
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

    mLeftTopLatitude = attr.leftTopLatitude;
    mLeftTopLongitude = attr.leftTopLongitude;
    mRightLowLatitude = attr.rightLowLatitude;
    mRightLowLongitude = attr.rightLowLongitude;

    setStartTime(startT);
    setEndTime(endT);
    setRegionId(attr.regionId);
    setRegionAttr(attr.regionAttr);
    setTopSpeed(attr.topSpeed);
    setOverSpeedDuration(attr.overSpeedDuration);

    return true;

}
