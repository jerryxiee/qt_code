#include "positionreport.h"

PositionReport::PositionReport(QObject *parent) : QObject(parent)
{

}

void PositionReport::onStatusChangedSlot(StatusFlag flag,int mask,int value)
{
    mPosStatus |= (mask&value)<<flag;

    statusChanged(mPosStatus);
}

uint32_t PositionReport::readStatus() const
{
    return mPosStatus;
}

void PositionReport::setStatus(uint32_t status)
{
    mPosStatus = status;
    statusChanged(mPosStatus);
}
