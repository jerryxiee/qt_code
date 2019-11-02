#include "rectangleregion.h"
#include "common/bcdtransform.h"
#include <QDateTime>
#include <QDebug>

RectangleRegion::RectangleRegion(QObject *parent) : RegionBase(REGIONTYPE::Rectange,parent)
{
    RectangleRegionDataBase database;
    if(!database.isExists()){
        database.createTab();
    }

}

RectangleRegion::RectangleRegion(RectangleRegionAttr &attr,QObject *parent):
    RegionBase(REGIONTYPE::Rectange,attr.regionId,attr.regionAttr,BCDTransform::ChartoArray(attr.startTime,sizeof (attr.startTime)),
               BCDTransform::ChartoArray(attr.endTime,sizeof (attr.endTime)),attr.topSpeed,attr.overSpeedDuration,parent),
    mLeftTopLatitude(attr.leftTopLatitude),mLeftTopLongitude(attr.leftTopLongitude),
    mRightLowLatitude(attr.rightLowLatitude),mRightLowLongitude(attr.rightLowLongitude)
{
//    QByteArray bytearray;

//    if(attr.startTime[0] == 0x0&&attr.startTime[1] == 0x0&&attr.startTime[2] == 0x0){
//        setEveryDayEffect(true);
//        setStartTime(QDateTime(QDate::currentDate(),QTime::fromString(bytearray.right(6),"hhmmss")));
//        setEndTime(QDateTime(QDate::currentDate(),QTime::fromString(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime)).right(6),"hhmmss")));
//    }else {
//        setEveryDayEffect(false);
//        setStartTime(QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.startTime,sizeof (attr.startTime))), "yyyyMMddhhmmss"));
//        setEndTime(QDateTime::fromString("20"+QString(BCDTransform::toArray(attr.endTime,sizeof (attr.endTime))), "yyyyMMddhhmmss"));
//    }

    RectangleRegionDataBase database;
    if(!database.isExists()){
        database.createTab();
    }
}

bool RectangleRegion::isRectangle() const
{
    return true;
}

bool RectangleRegion::contaion(uint32_t latitude,uint32_t longitude)
{
    return false;
}

bool RectangleRegion::addToDataBase(RectangleRegionAttr &attr)
{
    RectangleRegionDataBase database;
    if(database.createTab()&&database.insertSignalData(attr)){
        return true;
    }

    return false;
}

bool RectangleRegion::addRegionToDataBase()
{
    RectangleRegionAttr attr;
    RectangleRegionDataBase database;
    QDateTime datetime;
    QByteArray bytearray;

    attr.regionId = getRegionId();
    attr.regionAttr = getRegionAttr();
    attr.leftTopLatitude = mLeftTopLatitude;
    attr.leftTopLongitude = mLeftTopLongitude;
    attr.rightLowLatitude = mRightLowLatitude;
    attr.rightLowLongitude = mRightLowLongitude;
    attr.topSpeed = getTopSpeed();
    attr.overSpeedDuration = getOverSpeedDuration();

    bytearray = getStartTime();
    memcpy(attr.startTime,bytearray.data(),sizeof (attr.startTime));
    bytearray = getEndTime();
    memcpy(attr.endTime,bytearray.data(),sizeof (attr.endTime));
    if(database.isExists()){
        database.insertSignalData(attr);
//        database.insertSignalData(getRegionId(),getRegionAttr(),mCenterLatitude,mCenterLongitude,mRegionRadius,
//                                  getStartTime(),getEndTime(),getTopSpeed(),getOverSpeedDuration());
        return true;
    }

    return false;
}

bool RectangleRegion::modifyRegion(RectangleRegionAttr &attr)
{
    return updateRegion(attr);
}
bool RectangleRegion::updateRegion(RectangleRegionAttr &attr)
{
    mLeftTopLatitude = attr.leftTopLatitude;
    mLeftTopLongitude = attr.leftTopLongitude;
    mRightLowLatitude = attr.rightLowLatitude;
    mRightLowLongitude = attr.rightLowLongitude;
    setStartTime(attr.startTime);
    setEndTime(attr.startTime);
    setRegionId(attr.regionId);
    setRegionAttr(attr.regionAttr);
    setTopSpeed(attr.topSpeed);
    setOverSpeedDuration(attr.overSpeedDuration);

    RectangleRegionDataBase database;
    if(database.createTab()){
        database.updateSignalData(attr);
    }
    return true;

}

bool RectangleRegion::deleteRegionFromDataBase()
{
    RectangleRegionDataBase database;
    if(database.isExists()){
        database.deleteSignalData(getRegionId());
        return true;
    }

    return false;
}






RectangleRegionDataBase::RectangleRegionDataBase()
{

}

RectangleRegionDataBase::~RectangleRegionDataBase()
{


}

bool RectangleRegionDataBase::createTab()
{
    QString tabinfo = "(ID INTEGER PRIMARY KEY NOT NULL, \
            attribute INTEGER , \
            leftTopLatitude INTEGER NOT NULL, \
            leftTopLongitude INTEGER NOT NULL, \
            rightLowLatitude INTEGER NOT NULL, \
            rightLowLongitude INTEGER NOT NULL, \
            start TEXT,\
            end TEXT,\
            topSpeed INTEGER,\
            overspeedduration INTEGER)";

    return SqliteDateBase::getSqliteDateBase()->createTable(DATABASETAB,tabinfo);

}
bool RectangleRegionDataBase::deleteTab()
{
    return SqliteDateBase::getSqliteDateBase()->deleteTable(DATABASETAB);
}

bool RectangleRegionDataBase::isExists()
{
    return SqliteDateBase::getSqliteDateBase()->isTabExists(DATABASETAB);
}

int RectangleRegionDataBase::count()
{
    return SqliteDateBase::getSqliteDateBase()->countInTab(DATABASETAB);
}

//bool RectangleRegionDataBase::insertSignalData(uint32_t regionId,
//                        uint32_t regionAttr,
//                        uint32_t leftTopLatitude,
//                        uint32_t leftTopLongitude,
//                        uint32_t rightLowLatitude,
//                        uint32_t rightLowLongitude,
//                        uint32_t  startTime,
//                        uint32_t  endTime,
//                        uint32_t topSpeed,
//                        uint32_t overSpeedDuration)
//{
//    QSqlQuery query;

//    query.prepare("INSERT INTO "+DATABASETAB+" VALUES(:ID,:attribute,:leftTopLatitude,:leftTopLongitude,:rightLowLatitude,:rightLowLongitude,:start,:end,:topSpeed,:overspeedduration)");
//    query.bindValue(":ID",regionId);

//    query.bindValue(":attribute",regionAttr);
//    query.bindValue(":leftTopLatitude",leftTopLatitude);
//    query.bindValue(":leftTopLongitude",leftTopLongitude);
//    query.bindValue(":rightLowLatitude",rightLowLatitude);
//    query.bindValue(":rightLowLongitude",rightLowLongitude);
//    query.bindValue(":start",startTime);
//    query.bindValue(":end",endTime);
//    query.bindValue(":topSpeed",topSpeed);
//    query.bindValue(":overspeedduration",overSpeedDuration);

//    if(query.exec()){
//        return true;
//    }

//    qDebug()<<"insertSignalData failed "<<query.lastError();

//    return false;
//}

bool RectangleRegionDataBase::insertSignalData(RectangleRegionAttr &info)
{
    QSqlQuery query;

    QByteArray bytearray;

    query.prepare("INSERT INTO "+DATABASETAB+" VALUES(:ID,:attribute,:leftTopLatitude,:leftTopLongitude,:rightLowLatitude,:rightLowLongitude,:start,:end,:topSpeed,:overspeedduration)");
    query.bindValue(":ID",info.regionId);

    query.bindValue(":attribute",info.regionAttr);
    query.bindValue(":leftTopLatitude",info.leftTopLatitude);
    query.bindValue(":leftTopLongitude",info.leftTopLongitude);
    query.bindValue(":rightLowLatitude",info.rightLowLatitude);
    query.bindValue(":rightLowLongitude",info.rightLowLongitude);
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

bool RectangleRegionDataBase::deleteSignalData(uint id)
{
    QString where = "ID="+QString::number(id);

    return SqliteDateBase::getSqliteDateBase()->deleteTabData(DATABASETAB,where);
}

bool RectangleRegionDataBase::updateSignalData(RectangleRegionAttr &info)
{
    if(deleteSignalData(info.regionId)){
        if(insertSignalData(info)){
            return true;
        }
    }

    return false;
}

bool RectangleRegionDataBase::getSignalData(uint id, RectangleRegionAttr &attr)
{
    QSqlQuery query;
    QString where = "ID="+QString::number(id);
    QByteArray bytearray;
    QByteArray tmp;

    if(SqliteDateBase::getSqliteDateBase()->getTabData(query,DATABASETAB,where)&&query.next()){
        attr.regionId = query.value(0).toUInt();
        attr.regionAttr = query.value(1).toUInt();
        attr.leftTopLatitude = query.value(2).toUInt();
        attr.leftTopLongitude = query.value(3).toUInt();
        attr.rightLowLatitude= query.value(4).toUInt();
        attr.rightLowLongitude = query.value(5).toUInt();

        tmp = query.value(6).toByteArray();
        bytearray = BCDTransform::toArray(tmp.data(),tmp.size());
        memcpy(attr.startTime,bytearray.data(),sizeof (attr.startTime));
        tmp = query.value(7).toByteArray();
        bytearray = BCDTransform::toArray(tmp.data(),tmp.size());
        memcpy(attr.endTime,bytearray.data(),sizeof (attr.endTime));
        attr.topSpeed = query.value(8).toUInt();
        attr.overSpeedDuration = query.value(9).toUInt();
        return true;
    }

    return false;
}

