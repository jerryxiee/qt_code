#ifndef POSITIONCTR_H
#define POSITIONCTR_H

#include <QObject>
#include "regionctr/regioncontrol.h"

class PositionCtr : public QObject
{
    Q_OBJECT
public:
    explicit PositionCtr(QObject *parent = nullptr);

    void setAlarmFlag(uint32_t flag);
    void setStatus(uint32_t status);
    static void reportPosition(void *object);

signals:
    void alarmFalgChanged();
    void statusChanged();

public slots:
    //报警标志改变
    void urgentAlarmFlagChanged(bool enable);
    void overSpeedAlarmFlagChanged(bool enable);
    void fatigueDrivingAlarmFlagChanged(bool enable);
    void dangerWarningAlarmFlagChanged(bool enable);
    void gnssFaultAlarmFlagChanged(bool enable);
    void antennaNotConnectAlarmFlagChanged(bool enable);
    void urgentCircuitFlagChanged(bool enable);
    void undervoltageAlarmFlagChanged(bool enable);
    void powerOffAlarmFlagChanged(bool enable);
    void lcdFaultAlarmFlagChanged(bool enable);
    void ttsFaultAlarmFlagChanged(bool enable);
    void cameraFaultAlarmFlagChanged(bool enable);
    void icFaultAlarmFlagChanged(bool enable);
    void overSpeedWarningFlagChanged(bool enable);
    void fatigueDrivingWarningFlagChanged(bool enable);
    void driverTimeoutAlarmFlagChanged(bool enable);
    void stopTimeoutAlarmFlagChanged(bool enable);
    void inOrOutRegionAlarmFlagChanged(bool enable);
    void inOrOutRouteAlarmFlagChanged(bool enable);
    void driveLowTimeAlarmFlagChanged(bool enable);
    void routeDeviationAlarmFlagChanged(bool enable);
    void vssFaultAlarmFlagChanged(bool enable);
    void oilAbnormalAlarmFlagChanged(bool enable);
    void vehicleTheftAlarmFlagChanged(bool enable);
    void illegalIgnitionAlarmFlagChanged(bool enable);
    void illegalDisplacementAlarmFlagChanged(bool enable);
    void collisionWarningFlagChanged(bool enable);
    void rolloverWarningFlagChanged(bool enable);
    void illegalOpeningAlarmFlagChanged(bool enable);

    //状态改变
    void accStatusChanged(bool status);
    void locationStatusChanged(bool status);
    void latitudeStatusChanged(bool status);
    void longitudeStatusChanged(bool status);
    void operateStatusChanged(bool status);
    void encryptionStatusChanged(bool status);
    void ladenCargoStatusChanged(bool status);
    void oilCircuitStatusChanged(bool status);
    void circuitStatusChanged(bool status);
    void doorLockStatusChanged(bool status);
    void door1LockStatusChanged(bool status);
    void door2LockStatusChanged(bool status);
    void door3LockStatusChanged(bool status);
    void door4LockStatusChanged(bool status);
    void door5LockStatusChanged(bool status);
    void gpsEnableStatusChanged(bool status);
    void bdEnableStatusChanged(bool status);
    void glonassEnableStatusChanged(bool status);
    void galileoEnableStatusChanged(bool status);

    void reportPosition(QList<PositionExtensionInfo> &infolist);
    void setLatitude(uint32_t value);
    void setLongitude(uint32_t value);
    void setSpeed(uint16_t value);
    void setAtitude(uint16_t value);
    void setDirectionAngle(uint16_t value);



private:

private:
    int mReportStrategy;
    int mReportPlay;
    int mUrgentReportTime;
    int mUrgentReportDistance;
    int mDriverSignoutReportTime;
    int mDriverSignoutReporDistance;
    int mSleepReportTime;
    int mSleepReporDistance;
    int mDefaultReportTime;
    int mDefaultReporDistance;
    uint32_t mAlarmFlag;
    uint32_t mPositionStatus;
    uint32_t mCurRegionId;         //当前所在区域ID
    uint32_t mLatitude;            //纬度
    uint32_t mLongitude;           //经度
    uint16_t mSpeed;               //速度
    uint16_t mAtitude;             //海拔
    uint16_t mDirectionAngle;      //方向角
    RegionControl *mRegionControl;
};

#endif // POSITIONCTR_H
