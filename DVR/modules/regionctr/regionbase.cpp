#include "regionbase.h"
#include <QtMath>
#include <QDebug>

RegionBase::RegionBase(REGIONTYPE type,QObject *parent) :
    QObject(parent),mIsInRegion(false),mRepeatEveryDay(false),mType(type)
{
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE");
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE&");
}

RegionBase::RegionBase(REGIONTYPE type, uint32_t id, uint32_t attr, QByteArray start, QByteArray end, uint topspeed, uint topspeeddur, QObject *parent):
    QObject(parent),mRegionId(id),mRegionAttr(attr),mStartTime(start),mEndTime(end),
    mTopSpeed(topspeed),mOverSpeedDuration(topspeeddur),mIsInRegion(false),mRepeatEveryDay(false),mType(type)
{
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE");
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE&");

    if(memcmp(mStartTime.data(),"000000",6) == 0 ||
            memcmp(mEndTime.data(),"000000",6) == 0){
        mRepeatEveryDay = true;
    }
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

QByteArray RegionBase::getStartTime() const
{
//    if(isEveryDay()){
//        QDateTime datetime(QDate(0,0,0),mStart.time());
//        return datetime;
//    }
    return mStartTime;
}

void RegionBase::setStartTime(QByteArray value)
{
    if(memcmp(value.data(),"000000",6) == 0){
        mRepeatEveryDay = true;
    }
    mStartTime = value;
}

QByteArray RegionBase::getEndTime() const
{
//    if(isEveryDay()){
//        QDateTime datetime(QDate(0,0,0),mEnd.time());
//        return datetime;
//    }
    return mEndTime;
}

void RegionBase::setEndTime(QByteArray value)
{
    if(memcmp(value.data(),"000000",6) == 0){
        mRepeatEveryDay = true;
    }
    mEndTime = value;
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
