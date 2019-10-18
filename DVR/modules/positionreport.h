#ifndef POSITIONREPORT_H
#define POSITIONREPORT_H

#include <QObject>

class PositionReport : public QObject
{
    Q_OBJECT
    Q_ENUMS(StatusFlag)
public:
    enum StatusFlag{
        ACCFlag = 0x01,
        LocationFlag = 0x02,
        LatitudeFlag = 0x03,
        LongitudeFlag = 0x04,
        OperateFlag = 0x05,
        EncryFlag = 0x06,
        BrakeWarnFlag = 0x07,
        MigrationFlag = 0x08,
        CarStatusFlag = 0x09,
        CarOilStatusFlag = 0x0B,
        CarCircuitStatusFlag = 0x0C,
        CarLockFlag = 0x0D,
        FrontDoorFlag = 0x0E,
        MidDoorFlag = 0x0F,
        BackDoorFlag = 0x10,
        DriverDoorFlag = 0x11,
        ReserveDoorFlag = 0x12,
        GPSFlag = 0x13,
        BDFlag  = 0x14,
        GLONASSFlag = 0x15,
        GalieoFlag = 0x16,
        CarRunFlag = 0x17,
        MASK
    };

    explicit PositionReport(QObject *parent = nullptr);

    uint32_t readStatus() const;
    void setStatus(uint32_t status) ;

signals:
    void statusChanged(uint32_t status);

public slots:
    void onStatusChangedSlot(StatusFlag flag,int mask,int value);

private:
    uint32_t mPosStatus;
    uint32_t mAlarmFlag;
};

#endif // POSITIONREPORT_H
