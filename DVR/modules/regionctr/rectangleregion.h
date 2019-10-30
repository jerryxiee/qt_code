#ifndef RECTANGLEREGION_H
#define RECTANGLEREGION_H

#include "regionbase.h"
#include "communication/jtt808define.h"
#include "common/sqlitedatebase.h"

class RectangleRegionDataBase;

class RectangleRegion : public RegionBase
{
    Q_OBJECT
public:
    explicit RectangleRegion(QObject *parent = nullptr);
    RectangleRegion(RectangleRegionAttr &attr,QObject *parent = nullptr);
    virtual bool contaion(uint32_t latitude,uint32_t longitude);
    bool modifyRegion(RectangleRegionAttr &attr);
    bool updateRegion(RectangleRegionAttr &attr);
    bool addToDataBase(RectangleRegionAttr &attr);
    virtual bool deleteRegionFromDataBase();
    virtual bool addRegionToDataBase();

private:
    virtual bool isRectangle() const;

signals:

public slots:

private:
    uint32_t mLeftTopLatitude;
    uint32_t mLeftTopLongitude;
    uint32_t mRightLowLatitude;
    uint32_t mRightLowLongitude;
};

class RectangleRegionDataBase
{
public:
    RectangleRegionDataBase();
    ~RectangleRegionDataBase();

    bool createTab();
    bool deleteTab();
    bool isExists();
    int count();
    bool insertSignalData(RectangleRegionAttr &info);
//    bool insertSignalData(uint32_t regionId,
//                            uint32_t regionAttr,
//                            uint32_t leftTopLatitude,
//                            uint32_t leftTopLongitude,
//                            uint32_t rightLowLatitude,
//                            uint32_t rightLowLongitude,
//                            uint32_t  startTime,
//                            uint32_t  endTime,
//                            uint32_t topSpeed,
//                            uint32_t overSpeedDuration);
    bool deleteSignalData(uint id);
    bool updateSignalData(RectangleRegionAttr &info);
    bool getSignalData(uint id,RectangleRegionAttr &attr);
private:
    const QString DATABASETAB = "RectangleRegionSet";
};

#endif // RECTANGLEREGION_H
