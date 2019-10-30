#include "regionbase.h"
#include <QtMath>

RegionBase::RegionBase(REGIONTYPE type,QObject *parent) :
    QObject(parent),mIsInRegion(false),mRepeatEveryDay(false),mType(type)
{
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE");
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE&");
}

RegionBase::RegionBase(REGIONTYPE type,uint32_t id,uint32_t attr,uint start,uint end,uint topspeed,uint topspeeddur,QObject *parent):
    QObject(parent),mRegionId(id),mRegionAttr(attr),mStartTime(start),mEndTime(end),
    mTopSpeed(topspeed),mOverSpeedDuration(topspeeddur),mIsInRegion(false),mRepeatEveryDay(false),mType(type)
{
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE");
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE&");

}

RegionBase::~RegionBase()
{

}

double RegionBase::distance(uint32_t n1,uint32_t e1,uint32_t n2,uint32_t e2)
{
    double  jl_jd  =   102834.74258026089786013677476285 ;
    double  jl_wd  =   111712.69150641055729984301412873 ;
    double  b =  qAbs((e1  -  e2)  *  jl_jd/1000000);
    double  a  =  qAbs((n1  -  n2)  *  jl_wd/1000000);
    return  sqrt((a  *  a  +  b  *  b));
}

void RegionBase::repeatCalcFunc(uint32_t latitude,uint32_t longitude)
{
    if(contaion(latitude,longitude) && !mIsInRegion){
        mIsInRegion = true;
        enterRegion(mType,mRegionId,true);
    }else if(mIsInRegion && !contaion(latitude,longitude)){
        enterRegion(mType,mRegionId,false);
        mIsInRegion = false;
    }
}

bool RegionBase::isRectangle() const
{
    return false;
}

bool RegionBase::isCircular() const
{
    return false;
}

void RegionBase::setRegionId(uint32_t id)
{
    mRegionId = id;
}

void RegionBase::setRegionAttr(uint32_t value)
{
    mRegionAttr = value;
}

QDateTime RegionBase::getStartTime() const
{
    if(isEveryDay()){
        QDateTime datetime(QDate(0,0,0),mStart.time());
        return datetime;
    }

    return mStart;
}

void RegionBase::setStartTime(QDateTime value)
{
    mStart = value;
}

QDateTime RegionBase::getEndTime() const
{
    if(isEveryDay()){
        QDateTime datetime(QDate(0,0,0),mEnd.time());
        return datetime;
    }

    return mEnd;
}

void RegionBase::setEndTime(QDateTime value)
{
    mEnd = value;
}

void RegionBase::setTopSpeed(uint value)
{
    mTopSpeed = value;
}

void RegionBase::setOverSpeedDuration(uint value)
{
    mOverSpeedDuration = value;
}

void RegionBase::setEveryDayEffect(bool enable)
{
    mRepeatEveryDay = enable;
}
