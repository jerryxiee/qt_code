#include "platformset.h"
#include <QDebug>

PlatformSet::PlatformSet(QObject *parent) : QObject(parent)
{
//    connect(this,SIGNAL(serverInfoChanged(int)),PlatformRegister::getPlatformRegister(),SLOT(updateServerInfo(int)));
}

QString PlatformSet::readHeartBeat() const
{

    return mPlatformConfig.getValue(0x8103,0x01,0).toString();
}

void PlatformSet::setHeartBeat(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x01;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readTcpAnswerTimeout() const
{
    return mPlatformConfig.getValue(0x8103,0x02,0).toString();
}
void PlatformSet::setTcpAnswerTimeout(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x02;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readTcpRepeatTimes() const
{
    return mPlatformConfig.getValue(0x8103,0x03,0).toString();
}
void PlatformSet::setTcpRepeatTimes(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x03;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::PlatformSet::readUdpAnswerTimeout() const
{
    return mPlatformConfig.getValue(0x8103,0x04,0).toString();
}
void PlatformSet::setUdpAnswerTimeout(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x04;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readUdpRepeatTimes() const
{
    return mPlatformConfig.getValue(0x8103,0x05,0).toString();
}
void PlatformSet::setUdpRepeatTimes(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x05;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readSmsAnswerTimeout() const
{
    return mPlatformConfig.getValue(0x8103,0x06,0).toString();
}
void PlatformSet::setSmsAnswerTimeout(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x06;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readSmsRepeatTimes() const
{
    return mPlatformConfig.getValue(0x8103,0x07,0).toString();
}
void PlatformSet::setSmsRepeatTimes(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x07;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readMainServer() const
{
    return mPlatformConfig.getValue(0x8103,0x13,0).toString();
}
void PlatformSet::setMainServer(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x13;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readMainServerIp() const
{
    QString str = readMainServer();

    QStringList list = str.split(":");

    if(list.count() > 0){
        return list[0];
    }else {
        return QString();
    }
}
void PlatformSet::setMainServerIp(QString value)
{}
QString PlatformSet::readMainServerTcpPort() const
{
    QString str = readMainServer();

    QStringList list = str.split(":");


    if(list.count() > 1){
        return list[1];
    }else {
        return QString();
    }
}
void PlatformSet::setMainServerTcpPort(QString value)
{}
QString PlatformSet::readMainServerUdpPort() const
{
    QString str = readMainServer();

    QStringList list = str.split(":");


    if(list.count() > 2){
        return list[2];
    }else {
        return QString();
    }
}
void PlatformSet::setMainServerUdpPort(QString value)
{}
QString PlatformSet::readBackupServer() const
{
    return mPlatformConfig.getValue(0x8103,0x17,0).toString();
}
void PlatformSet::setBackupServer(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x17;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readBackupServerIp() const
{
    QString str = readBackupServer();

    QStringList list = str.split(":");

    if(list.count() > 0){
        return list[0];
    }else {
        return QString();
    }
}
void PlatformSet::setBackupServerIp(QString value)
{}
QString PlatformSet::readBackupServerTcpPort() const
{
    QString str = readBackupServer();

    QStringList list = str.split(":");

    if(list.count() > 1){
        return list[1];
    }else {
        return QString();
    }
}
void PlatformSet::setBackupServerTcpPort(QString value)
{}
QString PlatformSet::readBackupServerUdpPort() const
{
    QString str = readBackupServer();

    QStringList list = str.split(":");

    if(list.count() > 2){
        return list[2];
    }else {
        return QString();
    }
}
void PlatformSet::setBackupServerUdpPort(QString value)
{}
QString PlatformSet::readIcAuthServerIp() const
{
    return mPlatformConfig.getValue(0x8103,0x1a,0).toString();
}
void PlatformSet::setIcAuthServerIp(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x1a;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readIcAuthBackupServerIp() const
{
    return mPlatformConfig.getValue(0x8103,0x1d,0).toString();
}
void PlatformSet::setIcAuthBackupServerIp(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x1d;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readIcAuthServerTcpPort() const
{
    return mPlatformConfig.getValue(0x8103,0x1b,0).toString();
}
void PlatformSet::setIcAuthServerTcpPort(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x1b;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readIcAuthServerUdpPort() const
{
    return mPlatformConfig.getValue(0x8103,0x1c,0).toString();
}
void PlatformSet::setIcAuthServerUdpPort(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x1c;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
int PlatformSet::readPositionReportingStrategy() const
{
    return mPlatformConfig.getValue(0x8103,0x20,0).toInt();
}
void PlatformSet::setPositionReportingStrategy(int value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x20;
    config.field = 0x0;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}
int PlatformSet::readPositionReportingProgramme() const
{
    return mPlatformConfig.getValue(0x8103,0x21,0).toInt();
}
void PlatformSet::setPositionReportingProgramme(int value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x21;
    config.field = 0x0;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readPositionReportingSleepDistanceInterval() const
{
    return mPlatformConfig.getValue(0x8103,0x2e,0).toString();
}
void PlatformSet::setPositionReportingSleepDistanceInterval(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x2e;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readPositionReportingUrgentDistanceInterval() const
{
    return mPlatformConfig.getValue(0x8103,0x2f,0).toString();
}
void PlatformSet::setPositionReportingUrgentDistanceInterval(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x2f;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readPositionReportingDefaultDistanceInterval() const
{
    return mPlatformConfig.getValue(0x8103,0x2c,0).toString();
}
void PlatformSet::setPositionReportingDefaultDistanceInterval(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x2c;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readDriverNoSignInDistanceInterval() const
{
    return mPlatformConfig.getValue(0x8103,0x2d,0).toString();
}
void PlatformSet::setDriverNoSignInDistanceInterval(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x2d;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readPositionReportingSleepTimeInterval() const
{
    return mPlatformConfig.getValue(0x8103,0x27,0).toString();
}
void PlatformSet::setPositionReportingSleepTimeInterval(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x27;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readPositionReportingUrgentTimeInterval() const
{
    return mPlatformConfig.getValue(0x8103,0x28,0).toString();
}
void PlatformSet::setPositionReportingUrgentTimeInterval(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x28;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readPositionReportingDefaultTimeInterval() const
{
    return mPlatformConfig.getValue(0x8103,0x29,0).toString();
}
void PlatformSet::setPositionReportingDefaultTimeInterval(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x29;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readDriverNoSignInTimeInterval() const
{
    return mPlatformConfig.getValue(0x8103,0x22,0).toString();
}
void PlatformSet::setDriverNoSignInTimeInterval(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x22;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readInflectionCompensationAngle() const
{
    return mPlatformConfig.getValue(0x8103,0x30,0).toString();
}
void PlatformSet::setInflectionCompensationAngle(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x30;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readElectronicFenceRadius() const
{
    return mPlatformConfig.getValue(0x8103,0x31,0).toString();
}
void PlatformSet::setElectronicFenceRadius(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x31;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readTelephoneOfMonitoringPlatform() const
{
    return mPlatformConfig.getValue(0x8103,0x40,0).toString();
}
void PlatformSet::setTelephoneOfMonitoringPlatform(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x40;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readTelephoneOfReset() const
{
    return mPlatformConfig.getValue(0x8103,0x41,0).toString();
}
void PlatformSet::setTelephoneOfReset(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x41;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readTelephoneOfFactory() const
{
    return mPlatformConfig.getValue(0x8103,0x42,0).toString();
}
void PlatformSet::setTelephoneOfFactory(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x42;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readSmsTelephoneOfMonitoringPlatform() const
{
    return mPlatformConfig.getValue(0x8103,0x43,0).toString();
}
void PlatformSet::setSmsTelephoneOfMonitoringPlatform(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x43;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readSmsTelephoneOfTextAlarm() const
{
    return mPlatformConfig.getValue(0x8103,0x44,0).toString();
}
void PlatformSet::setSmsTelephoneOfTextAlarm(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x44;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
int PlatformSet::readTelephoneReceptionStrategy() const
{
    return mPlatformConfig.getValue(0x8103,0x45,0).toInt();
}
void PlatformSet::setTelephoneReceptionStrategy(int value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x45;
    config.field = 0x0;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readOnesReceptionMaxTime() const
{
    return mPlatformConfig.getValue(0x8103,0x46,0).toString();
}
void PlatformSet::setOnesReceptionMaxTime(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x46;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readMonthReceptionMaxTime() const
{
    return mPlatformConfig.getValue(0x8103,0x47,0).toString();
}
void PlatformSet::setMonthReceptionMaxTime(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x47;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readMonitorTelephoneNumber() const
{
    return mPlatformConfig.getValue(0x8103,0x48,0).toString();
}
void PlatformSet::setMonitorTelephoneNumber(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x48;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readPlatformSpecialNumber() const
{
    return mPlatformConfig.getValue(0x8103,0x49,0).toString();
}
void PlatformSet::setPlatformSpecialNumber(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x49;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readTopSpeed() const
{
    return mPlatformConfig.getValue(0x8103,0x55,0).toString();
}
void PlatformSet::setTopSpeed(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x55;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readOverspeedDuration() const
{
    return mPlatformConfig.getValue(0x8103,0x56,0).toString();
}
void PlatformSet::setOverspeedDuration(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x56;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readDriverDuration() const
{
    return mPlatformConfig.getValue(0x8103,0x57,0).toString();
}
void PlatformSet::setDriverDuration(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x57;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readDriverDayDuration() const
{
    return mPlatformConfig.getValue(0x8103,0x58,0).toString();
}
void PlatformSet::setDriverDayDuration(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x58;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readMinRestTime() const
{
    return mPlatformConfig.getValue(0x8103,0x59,0).toString();
}
void PlatformSet::setMinRestTime(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x59;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readMaxStopTime() const
{
    return mPlatformConfig.getValue(0x8103,0x5a,0).toString();
}
void PlatformSet::setMaxStopTime(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x5a;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readSadOverSpeed() const
{
    return mPlatformConfig.getValue(0x8103,0x5b,0).toString();
}
void PlatformSet::setSadOverSpeed(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x5b;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readSadFatigueDriving() const
{
    return mPlatformConfig.getValue(0x8103,0x5c,0).toString();
}
void PlatformSet::setSadFatigueDriving(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x5c;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readCollisionAlarm() const
{
    return mPlatformConfig.getValue(0x8103,0x5d,0).toString();
}
void PlatformSet::setCollisionAlarm(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x5d;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readCollisionAlarmTime() const
{
//    int str = readCollisionAlarm().toInt()&0x000000ff;

//    return QString::number(str);
    return mPlatformConfig.getValue(0x8103,0x5d,0).toString();
}

void PlatformSet::setCollisionAlarmTime(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x5d;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readCollisionAlarmAcc() const
{
//    int str = readCollisionAlarm().toInt()>>8;

//    return QString::number(str);
    return mPlatformConfig.getValue(0x8103,0x5d,1).toString();
}


void PlatformSet::setCollisionAlarmAcc(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x5d;
    config.field = 0x1;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readRolloverAngle() const
{
    return mPlatformConfig.getValue(0x8103,0x5e,0).toString();
}
void PlatformSet::setRolloverAngle(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x5e;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readProvienceID() const
{
    return mPlatformConfig.getValue(0x8103,0x81,0).toString();
}
void PlatformSet::setProvienceID(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x81;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readCityID() const
{
    return mPlatformConfig.getValue(0x8103,0x82,0).toString();
}
void PlatformSet::setCityID(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x82;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readCarLisence() const
{
    return mPlatformConfig.getValue(0x8103,0x83,0).toString();
}
void PlatformSet::setCarLisence(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x83;
    config.field = 0x0;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}
int PlatformSet::readCarColor() const
{
    return mPlatformConfig.getValue(0x8103,0x84,0).toInt();
}
void PlatformSet::setCarColor(int value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0x84;
    config.field = 0x0;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}

bool PlatformSet::readRegisterEnable() const
{
    return mPlatformConfig.getValue(0x8103,0,0).toBool();

}
void PlatformSet::setRegisterEnable(bool value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 0;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);

}
int PlatformSet::readRegisterType() const
{
    return mPlatformConfig.getValue(0x8103,0,1).toInt();
}
void PlatformSet::setRegisterType(int value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 1;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}
QString PlatformSet::readPhoneNumber() const
{
    return mPlatformConfig.getValue(0x8103,0,2).toString();
}
void PlatformSet::setPhoneNumber(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 2;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

bool PlatformSet::readMainEnableTcp() const
{
    return mPlatformConfig.getValue(0x8103,0,3).toBool();
}
void PlatformSet::setMainEnableTcp(bool value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 3;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}
bool PlatformSet::readMainEnableUdp() const
{
    return mPlatformConfig.getValue(0x8103,0,4).toBool();
}
void PlatformSet::setMainEnableUdp(bool value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 4;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}
bool PlatformSet::readSecEnableTcp() const
{
    return mPlatformConfig.getValue(0x8103,0,5).toBool();
}
void PlatformSet::setSecEnableTcp(bool value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 5;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}
bool PlatformSet::readSecEnableUdp() const
{
    return mPlatformConfig.getValue(0x8103,0,6).toBool();
}
void PlatformSet::setSecEnableUdp(bool value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 6;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readCarVinNumber() const
{
    return mPlatformConfig.getValue(0x8103,0,7).toString();
}
void PlatformSet::setCarVinNumber(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 7;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readAuthNumber() const
{
    return mPlatformConfig.getValue(0x8103,0,8).toString();
}
void PlatformSet::setAuthNumber(QString value)
{
    DeviceParam config;
    config.id = 0x8103;
    config.num = 0;
    config.field = 8;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}


int PlatformSet::readDeviceType() const
{
    return mPlatformConfig.getValue(0x8107,0,0).toInt();
}
void PlatformSet::setDeviceType(int value)
{
    DeviceParam config;
    config.id = 0x8107;
    config.num = 0;
    config.field = 1;
    config.value = QString::number(value);

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readProduceID() const
{
    return mPlatformConfig.getValue(0x8107,0,1).toString();
}
void PlatformSet::setProduceID(QString value)
{
    DeviceParam config;
    config.id = 0x8107;
    config.num = 0;
    config.field = 1;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readDeviceModel() const
{
    return mPlatformConfig.getValue(0x8107,0,2).toString();
}
void PlatformSet::setDeviceModel(QString value)
{
    DeviceParam config;
    config.id = 0x8107;
    config.num = 0;
    config.field = 2;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

QString PlatformSet::readDeviceID() const
{
    return mPlatformConfig.getValue(0x8107,0,3).toString();
}
void PlatformSet::setDeviceID(QString value)
{
    DeviceParam config;
    config.id = 0x8107;
    config.num = 0;
    config.field = 3;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

//void PlatformSet::updateServerInfo(int type)
//{
//    serverInfoChanged(type);
//}

