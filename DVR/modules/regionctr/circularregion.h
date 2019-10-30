#ifndef CIRCULARREGION_H
#define CIRCULARREGION_H

#include "regionbase.h"
#include "communication/jtt808define.h"
#include "common/sqlitedatebase.h"

class CircularRegionDataBase;

class CircularRegion : public RegionBase
{
    Q_OBJECT
public:
    explicit CircularRegion(QObject *parent = nullptr);
    CircularRegion(CicularRegionAttr &attr,QObject *parent = nullptr);
    virtual bool contaion(uint32_t latitude,uint32_t longitude);
    bool modifyRegion(CicularRegionAttr &attr);
    bool updateRegion(CicularRegionAttr &attr);
    virtual bool deleteRegionFromDataBase();
    virtual bool addRegionToDataBase();
    bool addToDataBase(CicularRegionAttr &attr);

private:
    virtual bool isCircular() const;
signals:

public slots:

private:
    uint32_t mCenterLatitude;
    uint32_t mCenterLongitude;
    uint32_t mRegionRadius;
};

class CircularRegionDataBase
{
public:
    CircularRegionDataBase();
    ~CircularRegionDataBase();

    bool createTab();
    bool deleteTab();
    bool isExists();
    int count();
    bool insertSignalData(CicularRegionAttr &info);
//    bool insertSignalData(uint32_t regionId,
//                            uint32_t regionAttr,
//                            uint32_t centerLatitude,
//                            uint32_t centerLongitude,
//                            uint32_t regionRadius,
//                            uint32_t  startTime,
//                            uint32_t  endTime,
//                            uint32_t topSpeed,
//                            uint32_t overSpeedDuration);
    bool deleteSignalData(uint id);
    bool updateSignalData(CicularRegionAttr &info);
    bool getSignalData(uint id,CicularRegionAttr &attr);
private:
    const QString DATABASETAB = "CircularRegionSet";
};

#endif // CIRCULARREGION_H
