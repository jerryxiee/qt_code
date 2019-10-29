#ifndef CIRCULARREGION_H
#define CIRCULARREGION_H

#include "regionbase.h"
#include "communication/jtt808define.h"

class CircularRegion : public RegionBase
{
    Q_OBJECT
public:
    explicit CircularRegion(QObject *parent = nullptr);
    virtual bool contaion(uint32_t latitude,uint32_t longitude);
    bool modifyRegion(CicularRegionAttr &attr);
    bool updateRegion(CicularRegionAttr &attr);

private:
    virtual bool isCircular() const;
signals:

public slots:

private:
    uint32_t mCenterLatitude;
    uint32_t mCenterLongitude;
    uint32_t mRegionRadius;
};

#endif // CIRCULARREGION_H
