#ifndef RECTANGLEREGION_H
#define RECTANGLEREGION_H

#include "regionbase.h"
#include "communication/jtt808define.h"

class RectangleRegion : public RegionBase
{
    Q_OBJECT
public:
    explicit RectangleRegion(QObject *parent = nullptr);
    virtual bool contaion(uint32_t latitude,uint32_t longitude);
    bool modifyRegion(RectangleRegionAttr &attr);
    bool updateRegion(RectangleRegionAttr &attr);

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

#endif // RECTANGLEREGION_H
