#ifndef POSITIONCTR_H
#define POSITIONCTR_H

#include <QObject>
#include "regionctr/regioncontrol.h"
#include "platformregister.h"

class PositionCtr : public QObject
{
    Q_OBJECT
public:
    static PositionCtr *getPositionCtr();

    void setAlarmFlag(uint32_t flag);
    void setStatus(uint32_t status);
    uint getReportStrategy() const;
    void setCurReportTime(uint value);
    uint getCurReportTime() const;
    void setCurReportDistance(uint value);
    uint getCurReportDistance() const;
    static void reportPosition(void *object);
    void reportPositionA();   //定时
    void reportPositionB();   //定距


protected:
    explicit PositionCtr(QObject *parent = nullptr);

private:
    void updateinfo();
signals:
    void alarmFalgChanged();
    void statusChanged();

public slots:
    void setReportStrategy(int value);
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
    void onServerConnectStatusChanged(PlatFormStatus &status);
    void onPositionSetChanged();
    void startReport();
    void stopReport();

public:
    PlatFormStatus mServerConnectStatus;

private:
    uint mCurrentReportTime;
    uint mCurrentReportDistance;
    uint mReportStrategy;
    uint mReportPlay;
    uint mUrgentReportTime;
    uint mUrgentReportDistance;
    uint mDriverSignoutReportTime;
    uint mDriverSignoutReporDistance;
    uint mSleepReportTime;
    uint mSleepReporDistance;
    uint mDefaultReportTime;
    uint mDefaultReporDistance;
    uint mAlarmFlag;
    uint mPositionStatus;
    uint mCurRegionId;         //当前所在区域ID
    uint mLatitude;            //纬度
    uint mLongitude;           //经度
    uint mLastLatitude;        //最近上报纬度
    uint mLastLongitude;       //最近上报经度
    uint mSpeed;               //速度
    uint mAtitude;             //海拔
    uint16_t mDirectionAngle;      //方向角
    RegionControl *mRegionControl;
    uint mAlarmMask;            //报警屏蔽字

    TaskToken mTaskToken;
    static PositionCtr * mPosCtr;
};

#endif // POSITIONCTR_H
