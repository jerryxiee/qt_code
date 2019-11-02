#ifndef DRIVERCONTROL_H
#define DRIVERCONTROL_H

#include <QObject>
#include "regionctr/regioncontrol.h"

class DriverControl : public QObject
{
    Q_OBJECT
public:
    explicit DriverControl(QObject *parent = nullptr);


    void process();
private:
    void updateInfo();
signals:

public slots:
    void driverParamChanged();
    void enterRegionSlot(REGIONTYPE type,uint32_t id,bool enter);
    void speedLimitChnaged(uint value,uint duration);

private:
    REGIONTYPE mRegionType;
    uint32_t mRegionId;
    uint mCurSpeed;
    uint mCurSpeedLimit;
    uint mCurOverSpeedDurationLimit;
    uint mOverSpeedDurationLimit;
    uint mDriverDurationLimit;
    uint mTopSpeed;
    uint mOverSpeedSad;
    uint mDriverDurationSad;
    uint mDayDriverDurationLimit;
};

#endif // DRIVERCONTROL_H
