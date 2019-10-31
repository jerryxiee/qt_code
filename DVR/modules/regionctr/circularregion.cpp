#include "circularregion.h"
#include "common/bcdtransform.h"
#include <QDateTime>
#include <QDebug>

CircularRegion::CircularRegion(QObject *parent) : RegionBase(REGIONTYPE::Circular,parent)
{
    CircularRegionDataBase database;
    if(!database.isExists()){
        database.createTab();
    }

}

CircularRegion::CircularRegion(CicularRegionAttr &attr,QObject *parent):
    RegionBase(REGIONTYPE::Circular,attr.regionId,attr.regionAttr,BCDTransform::toArray(attr.startTime,sizeof (attr.startTime)),
               BCDTransform::toArray(attr.endTime,sizeof (attr.endTime)),attr.topSpeed,attr.overSpeedDuration,parent),
    mCenterLatitude(attr.centerLatitude),mCenterLongitude(attr.centerLongitude),mRegionRadius(attr.regionRadius)
{
//    QByteArray bytearray;
////    uint startT = 0,endT = 0 ;

////    bytearray = BCDTransform::toArray(attr.startTime,sizeof (attr.startTime));
//    if(attr.startTime[0] == 0x0&&attr.startTime[1] == 0x0&&attr.startTime[2] == 0x0){
//        setEveryDayEffect(true);
//        setStartTime(QDateTime(QDate::currentDate(),QTime::fromString(bytearray.right(6),"hhmmss")));
//        setEndTime(QDateTime(QDate::currentDate(),QTime::fromString(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime)).right(6),"hhmmss")));
//    }else {
//        setEveryDayEffect(false);
//        setStartTime(QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.startTime,sizeof (attr.startTime))), "yyyyMMddhhmmss"));
//        setEndTime(QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime))), "yyyyMMddhhmmss"));
//    }
    CircularRegionDataBase database;
    if(!database.isExists()){
        database.createTab();
    }



//    if(bytearray.toInt() != 0){
//        startT = QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.startTime,sizeof (attr.startTime))), "yyyyMMddhhmmss").toTime_t();
//    }

//    bytearray = BCDTransform::toArray(attr.endTime,sizeof (attr.endTime));
//    if(bytearray.toInt() != 0){
//        endT = QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime))), "yyyyMMddhhmmss").toTime_t();
//    }

//    setStartTime(startT);
//    setEndTime(endT);

}

bool CircularRegion::isCircular() const
{
    return true;
}

bool CircularRegion::contaion(uint32_t latitude,uint32_t longitude)
{

    return false;
}

bool CircularRegion::addToDataBase(CicularRegionAttr &attr)
{
    CircularRegionDataBase database;
    if(database.createTab()&&database.insertSignalData(attr)){
        return true;
    }

    return false;
}

bool CircularRegion::updateRegion(CicularRegionAttr &attr)
{
//    QByteArray bytearray;

//    bytearray = BCDTransform::toArray(attr.startTime,sizeof (attr.startTime));
//    if(attr.startTime[0] == 0x0&&attr.startTime[1] == 0x0&&attr.startTime[2] == 0x0){
//        setEveryDayEffect(true);
//        setStartTime(QDateTime(QDate::currentDate(),QTime::fromString(bytearray.right(6),"hhmmss")));
//        setEndTime(QDateTime(QDate::currentDate(),QTime::fromString(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime)).right(6),"hhmmss")));
//    }else {
//        setEveryDayEffect(false);
//        setStartTime(QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.startTime,sizeof (attr.startTime))), "yyyyMMddhhmmss"));
//        setEndTime(QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime))), "yyyyMMddhhmmss"));
//    }


    mCenterLatitude = attr.centerLatitude;
    mCenterLongitude = attr.centerLongitude;
    mRegionRadius = attr.regionRadius;
    setStartTime(BCDTransform::toArray(attr.startTime,sizeof (attr.startTime)));
    setEndTime(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime)));
    setRegionId(attr.regionId);
    setRegionAttr(attr.regionAttr);
    setTopSpeed(attr.topSpeed);
    setOverSpeedDuration(attr.overSpeedDuration);

    CircularRegionDataBase database;
    if(database.createTab()){
        database.updateSignalData(attr);
    }

    return true;
}


bool CircularRegion::modifyRegion(CicularRegionAttr &attr)
{
    return updateRegion(attr);

}

bool CircularRegion::addRegionToDataBase()
{
    CicularRegionAttr attr;
    CircularRegionDataBase database;
    QDateTime datetime;
    QByteArray bytearray;

    attr.regionId = getRegionId();
    attr.regionAttr = getRegionAttr();
    attr.centerLatitude = mCenterLatitude;
    attr.centerLongitude = mCenterLongitude;
    attr.regionRadius = mRegionRadius;
    attr.topSpeed = getTopSpeed();
    attr.overSpeedDuration = getOverSpeedDuration();

    bytearray = BCDTransform::toBcd(getStartTime());
    memcpy(attr.startTime,bytearray.data(),sizeof (attr.startTime));

    bytearray = BCDTransform::toBcd(getEndTime());
    memcpy(attr.endTime,bytearray.data(),sizeof (attr.endTime));

    if(database.isExists()){
        database.insertSignalData(attr);
        return true;
    }

    return false;
}

bool CircularRegion::deleteRegionFromDataBase()
{
    CircularRegionDataBase database;
    if(database.isExists()){
        database.deleteSignalData(getRegionId());
        return true;
    }

    return false;
}


CircularRegionDataBase::CircularRegionDataBase()
{

}

CircularRegionDataBase::~CircularRegionDataBase()
{


}

bool CircularRegionDataBase::createTab()
{
    QString tabinfo = "(ID INTEGER PRIMARY KEY NOT NULL, \
            attribute INTEGER , \
            centerlat INTEGER NOT NULL, \
            centerlng INTEGER NOT NULL, \
            radius INTEGER NOT NULL, \
            start TEXT,\
            end TEXT,\
            topSpeed INTEGER,\
            overspeedduration INTEGER)";

    return SqliteDateBase::getSqliteDateBase()->createTable(DATABASETAB,tabinfo);

}
bool CircularRegionDataBase::deleteTab()
{
    return SqliteDateBase::getSqliteDateBase()->deleteTable(DATABASETAB);
}

bool CircularRegionDataBase::isExists()
{
    return SqliteDateBase::getSqliteDateBase()->isTabExists(DATABASETAB);
}

int CircularRegionDataBase::count()
{
    return SqliteDateBase::getSqliteDateBase()->countInTab(DATABASETAB);
}

//bool CircularRegionDataBase::insertSignalData(uint32_t regionId,
//                        uint32_t regionAttr,
//                        uint32_t centerLatitude,
//                        uint32_t centerLongitude,
//                        uint32_t regionRadius,
//                        uint32_t  startTime,
//                        uint32_t  endTime,
//                        uint32_t topSpeed,
//                        uint32_t overSpeedDuration)
//{
//    QSqlQuery query;

//    query.prepare("INSERT INTO "+DATABASETAB+" VALUES(:ID,:attribute,:centerlat,:centerlng,:radius,:start,:end,:topSpeed,:overspeedduration)");
//    query.bindValue(":ID",regionId);

//    query.bindValue(":attribute",regionAttr);
//    query.bindValue(":centerlat",centerLatitude);
//    query.bindValue(":centerlng",centerLongitude);
//    query.bindValue(":radius",regionRadius);
//    query.bindValue(":start",startTime);
//    query.bindValue(":end",endTime);
//    query.bindValue(":topSpeed",topSpeed);
//    query.bindValue(":overspeedduration",overSpeedDuration);

////    }

//    if(query.exec()){
//        return true;
//    }

//    qDebug()<<"insertSignalData failed "<<query.lastError();

//    return false;

//}

bool CircularRegionDataBase::insertSignalData(CicularRegionAttr &info)
{
    QSqlQuery query;

    query.prepare("INSERT INTO "+DATABASETAB+" VALUES(:ID,:attribute,:centerlat,:centerlng,:radius,:start,:end,:topSpeed,:overspeedduration)");
    query.bindValue(":ID",info.regionId);

    query.bindValue(":attribute",info.regionAttr);
    query.bindValue(":centerlat",info.centerLatitude);
    query.bindValue(":centerlng",info.centerLongitude);
    query.bindValue(":radius",info.regionRadius);
//qDebug()<<"circular write start:"<<BCDTransform::toArray(info.startTime,sizeof (info.startTime))<<" end:"<<BCDTransform::toArray(info.startTime,sizeof (info.endTime));
    query.bindValue(":start",BCDTransform::toArray(info.startTime,sizeof (info.startTime)));
    query.bindValue(":end",BCDTransform::toArray(info.startTime,sizeof (info.endTime)));
    query.bindValue(":topSpeed",info.topSpeed);
    query.bindValue(":overspeedduration",info.overSpeedDuration);

    if(query.exec()){
        return true;
    }

    qDebug()<<"insertSignalData failed "<<query.lastError();

    return false;

}

bool CircularRegionDataBase::deleteSignalData(uint id)
{
    QString where = "ID="+QString::number(id);

    return SqliteDateBase::getSqliteDateBase()->deleteTabData(DATABASETAB,where);
}

bool CircularRegionDataBase::updateSignalData(CicularRegionAttr &info)
{
    if(deleteSignalData(info.regionId)){
        if(insertSignalData(info)){
            return true;
        }
    }

    return false;
}

bool CircularRegionDataBase::getSignalData(uint id, CicularRegionAttr &attr)
{

    QSqlQuery query;
    QString where = "ID="+QString::number(id);
    QByteArray bytearray;
    QByteArray tmp;

    if(SqliteDateBase::getSqliteDateBase()->getTabData(query,DATABASETAB,where)&&query.next()){
        attr.regionId = query.value(0).toUInt();
        attr.regionAttr = query.value(1).toUInt();
        attr.centerLatitude = query.value(2).toUInt();
        attr.centerLongitude = query.value(3).toUInt();
        attr.regionRadius = query.value(4).toUInt();
        tmp = query.value(5).toByteArray();
        bytearray = BCDTransform::toBcd(tmp);
        memcpy(attr.startTime,bytearray.data(),sizeof (attr.startTime));
        tmp = query.value(6).toByteArray();
        bytearray = BCDTransform::toBcd(tmp);
        memcpy(attr.endTime,bytearray.data(),sizeof (attr.endTime));
        attr.topSpeed = query.value(7).toUInt();
        attr.overSpeedDuration = query.value(8).toUInt();
        return true;
    }

    return false;
}
