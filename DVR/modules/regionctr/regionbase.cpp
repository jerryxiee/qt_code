#include "regionbase.h"
#include <QtMath>
#include <QDebug>
#include <QDateTime>
#include "common/bcdtransform.h"
#include "modules/modulescontrol.h"

RegionBase::RegionBase(REGIONTYPE type,QObject *parent) :
    QObject(parent),mIsInRegion(false),mRepeatEveryDay(false),
    mEnable(false),mType(type),mTaskToken(nullptr)
{
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE");
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE&");
}

RegionBase::RegionBase(REGIONTYPE type, uint32_t id, uint32_t attr, QByteArray start, QByteArray end, uint topspeed, uint topspeeddur, QObject *parent):
    QObject(parent),mRegionId(id),mRegionAttr(attr),mStartTime(start),mEndTime(end),
    mTopSpeed(topspeed),mOverSpeedDuration(topspeeddur),mIsInRegion(false),mRepeatEveryDay(false),
    mEnable(false),mType(type),mTaskToken(nullptr)
{
    qDebug()<<"enter RegionBase:"<<mStartTime.length();
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE");
    qRegisterMetaType<REGIONTYPE>("REGIONTYPE&");
    uint stdelay;
    uint enddelay;
    if((mStartTime.at(0) == 0&&mStartTime.at(1) == 0&&mStartTime.at(2) == 0 )||
            (mEndTime.at(0) == 0&&mEndTime.at(1) == 0&&mEndTime.at(2) == 0 )){
        mRepeatEveryDay = true;
        stdelay = calcDelay(mStartTime,true);
        enddelay = calcDelay(mEndTime,true);
        if(enddelay == 0){
            return;
        }
    }else {
        stdelay = calcDelay(mStartTime,false);
        enddelay = calcDelay(mEndTime,false);
        if(enddelay == 0){
            return;
        }
    }

    mTaskToken = ModulesControl::getModulesControl()->getTaskScheduler().scheduleDelayedTask(stdelay*1000000,enableRegionControl,this);
    qDebug()<<"exit RegionBase";
}

RegionBase::~RegionBase()
{
    if(mTaskToken){
        ModulesControl::getModulesControl()->getTaskScheduler().unscheduleDelayedTask(mTaskToken);
        mTaskToken = nullptr;
    }
}


uint RegionBase::calcDelay(QByteArray time,bool everyday)
{
    uint delay = 0;
    if(everyday){
        QByteArray timebyte = BCDTransform::BcdToHex(time).right(3);
        QTime sttime = QTime(timebyte[0],timebyte[1],timebyte[2]);

        if(QDateTime(QDate::currentDate(),sttime) > QDateTime::currentDateTime()){
            delay = (QDateTime(QDate::currentDate(),sttime).toTime_t() - QDateTime::currentDateTime().toTime_t());

        }else {
            delay = 0;
        }
    }else {
        QByteArray array = "20"+BCDTransform::toArray(time.data(),mStartTime.length());

        QDateTime datatime = QDateTime::fromString(array,"yyyyMMddhhmmss");
        if(datatime > QDateTime::currentDateTime()){
            delay = (datatime.toTime_t() - QDateTime::currentDateTime().toTime_t());

        }else {
            delay = 0;
        }

    }

    return delay;
}

double RegionBase::distance(uint32_t n1,uint32_t e1,uint32_t n2,uint32_t e2)
{
    double  jl_jd  =   102834.74258026089786013677476285 ;
    double  jl_wd  =   111712.69150641055729984301412873 ;
    double  b =  qAbs((e1  -  e2)  *  jl_jd/1000000);
    double  a  =  qAbs((n1  -  n2)  *  jl_wd/1000000);
    return  sqrt((a  *  a  +  b  *  b));
}

void RegionBase::enableRegionControl(void *object)
{
    static_cast<RegionBase *>(object)->enableRegionControl();
}

void RegionBase::enableRegionControl()
{
    mEnable = true;
    speedCtrChanged(mTopSpeed,mOverSpeedDuration);
    uint delay = calcDelay(mEndTime,mRepeatEveryDay);
    mTaskToken = ModulesControl::getModulesControl()->getTaskScheduler().scheduleDelayedTask(delay*1000000,disEnableRegionControl,this);

}
void RegionBase::disEnableRegionControl(void *object)
{
    static_cast<RegionBase *>(object)->disEnableRegionControl();
}
void RegionBase::disEnableRegionControl()
{
    mEnable = false;
    if(mRepeatEveryDay){
        //计算下一次使能
        QByteArray timebyte = BCDTransform::BcdToHex(mStartTime).right(3);
        QTime sttime = QTime(timebyte[0],timebyte[1],timebyte[2]);
        timebyte = BCDTransform::BcdToHex(mEndTime).right(3);
        QTime endtime = QTime(timebyte[0],timebyte[1],timebyte[2]);
        uint duration = QDateTime(QDate::currentDate(),sttime).toTime_t() - QDateTime(QDate::currentDate(),endtime).toTime_t();

        uint delay = 3600*24 - duration;

        mTaskToken = ModulesControl::getModulesControl()->getTaskScheduler().scheduleDelayedTask(delay*1000000,enableRegionControl,this);
        return;
    }

    mTaskToken = nullptr;
}

void RegionBase::repeatCalcFunc(uint32_t latitude,uint32_t longitude)
{
    if(mEnable){
        if(contaion(latitude,longitude) && !mIsInRegion){
            mIsInRegion = true;
            enterRegion(mType,mRegionId,true,mRegionAttr);
        }else if(mIsInRegion && !contaion(latitude,longitude)){
            enterRegion(mType,mRegionId,false,mRegionAttr);
            mIsInRegion = false;
        }
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
