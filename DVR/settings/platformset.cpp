#include "platformset.h"
#include <QDebug>

PlatformSet::PlatformSet(QObject *parent) : QObject(parent)
{
//    connect(this,SIGNAL(serverInfoChanged(int)),PlatformRegister::getPlatformRegister(),SLOT(updateServerInfo(int)));
}

QByteArray PlatformSet::readDeviceParam(int msgid,int id,int offset) const
{
    QByteArray bytearray;

    bytearray = mPlatformConfig.getValue(msgid,id,offset).toByteArray();

    return bytearray;
}

QByteArray PlatformSet::readDeviceParam(int msgid, int id) const
{
    return readDeviceParam(msgid,id,0);
}

QList<DeviceParamInfo> PlatformSet::readDeviceParamList(int msgid,QList<int> &offlist) const
{
    QList<DeviceParamInfo> paramlist;
    QByteArray bytearray;

    for (int i = 0;i < offlist.count();i++) {
        bytearray = readDeviceParam(msgid,offlist.at(i));
        if(bytearray.isNull()||bytearray.length() > DEFAULTLEN)
            continue;
        DeviceParamInfo param;
        param.id = offlist.at(i);
        param.len = bytearray.length();
        memcpy(param.value,bytearray.data(),bytearray.length());
        paramlist.append(param);
    }

    return paramlist;
}

void PlatformSet::setDeviceParam(int msgid, int id, int offset, QVariant value)
{
    DeviceParam config;

    config.id = msgid;
    config.num = id;
    config.field = offset;
    config.value = value;

    mPlatformConfig.insertSignalData(config);
}

void PlatformSet::setDeviceParam(int msgid, int id, QVariant value)
{
//    DeviceParam config;

//    config.id = msgid;
//    config.num = id;
//    config.field = 0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(msgid,id,0,value);

}

void PlatformSet::setDeviceParamList(int msgid, QList<int> &offlist, QVariantList &valuelist)
{
    if(offlist.count()!= valuelist.count()){
        return;
    }

    for (int i = 0;i < offlist.count();i++) {
        setDeviceParam(msgid,offlist.at(i),valuelist[i]);
    }

}


QString PlatformSet::readHeartBeat() const
{
//    readDeviceParam(0x8103,0x01);

    return QString(readDeviceParam(0x8103,0x01));
//    return mPlatformConfig.getValue(0x8103,0x01,0).toString();
}

void PlatformSet::setHeartBeat(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x01;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x01,value);
}

QString PlatformSet::readTcpAnswerTimeout() const
{
//    return mPlatformConfig.getValue(0x8103,0x02,0).toString();
    return QString(readDeviceParam(0x8103,0x02));
}
void PlatformSet::setTcpAnswerTimeout(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x02;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x02,value);
}
QString PlatformSet::readTcpRepeatTimes() const
{
    return QString(readDeviceParam(0x8103,0x03));
//    return mPlatformConfig.getValue(0x8103,0x03,0).toString();
}
void PlatformSet::setTcpRepeatTimes(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x03;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x03,value);
}
QString PlatformSet::PlatformSet::readUdpAnswerTimeout() const
{
//    return mPlatformConfig.getValue(0x8103,0x04,0).toString();
    return QString(readDeviceParam(0x8103,0x04));
}
void PlatformSet::setUdpAnswerTimeout(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x04;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x04,value);
}
QString PlatformSet::readUdpRepeatTimes() const
{
//    return mPlatformConfig.getValue(0x8103,0x05,0).toString();
    return QString(readDeviceParam(0x8103,0x05));
}
void PlatformSet::setUdpRepeatTimes(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x05;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x05,value);
}
QString PlatformSet::readSmsAnswerTimeout() const
{
//    return mPlatformConfig.getValue(0x8103,0x06,0).toString();
    return QString(readDeviceParam(0x8103,0x06));
}
void PlatformSet::setSmsAnswerTimeout(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x06;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x06,value);
}
QString PlatformSet::readSmsRepeatTimes() const
{
//    return mPlatformConfig.getValue(0x8103,0x07,0).toString();
    return QString(readDeviceParam(0x8103,0x07));
}
void PlatformSet::setSmsRepeatTimes(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x07;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x07,value);
}
QString PlatformSet::readMainServer() const
{
//    return mPlatformConfig.getValue(0x8103,0x13,0).toString();
    return QString(readDeviceParam(0x8103,0x13));
}
void PlatformSet::setMainServer(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x13;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x13,value);
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
//    return mPlatformConfig.getValue(0x8103,0x17,0).toString();
    return QString(readDeviceParam(0x8103,0x17));
}
void PlatformSet::setBackupServer(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x17;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x17,value);
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
//    return mPlatformConfig.getValue(0x8103,0x1a,0).toString();
    return QString(readDeviceParam(0x8103,0x1a));
}
void PlatformSet::setIcAuthServerIp(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x1a;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x1a,value);
}
QString PlatformSet::readIcAuthBackupServerIp() const
{
//    return mPlatformConfig.getValue(0x8103,0x1d,0).toString();
    return QString(readDeviceParam(0x8103,0x1d));
}
void PlatformSet::setIcAuthBackupServerIp(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x1d;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x1d,value);
}
QString PlatformSet::readIcAuthServerTcpPort() const
{
//    return mPlatformConfig.getValue(0x8103,0x1b,0).toString();
    return QString(readDeviceParam(0x8103,0x1b));
}
void PlatformSet::setIcAuthServerTcpPort(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x1b;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x1b,value);
}
QString PlatformSet::readIcAuthServerUdpPort() const
{
//    return mPlatformConfig.getValue(0x8103,0x1c,0).toString();
    return QString(readDeviceParam(0x8103,0x1c));
}
void PlatformSet::setIcAuthServerUdpPort(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x1c;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x1c,value);
}
int PlatformSet::readPositionReportingStrategy() const
{
//    return mPlatformConfig.getValue(0x8103,0x20,0).toInt();
    return readDeviceParam(0x8103,0x20).toInt();
}
void PlatformSet::setPositionReportingStrategy(int value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x20;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x20,value);
}
int PlatformSet::readPositionReportingProgramme() const
{
//    return mPlatformConfig.getValue(0x8103,0x21,0).toInt();
    return readDeviceParam(0x8103,0x21).toInt();
}
void PlatformSet::setPositionReportingProgramme(int value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x21;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x21,value);
}
QString PlatformSet::readPositionReportingSleepDistanceInterval() const
{
//    return mPlatformConfig.getValue(0x8103,0x2e,0).toString();
    return QString(readDeviceParam(0x8103,0x2e));
}
void PlatformSet::setPositionReportingSleepDistanceInterval(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x2e;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x2e,value);
}
QString PlatformSet::readPositionReportingUrgentDistanceInterval() const
{
//    return mPlatformConfig.getValue(0x8103,0x2f,0).toString();
    return QString(readDeviceParam(0x8103,0x2f));
}
void PlatformSet::setPositionReportingUrgentDistanceInterval(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x2f;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x2f,value);
}
QString PlatformSet::readPositionReportingDefaultDistanceInterval() const
{
//    return mPlatformConfig.getValue(0x8103,0x2c,0).toString();
    return QString(readDeviceParam(0x8103,0x2c));
}
void PlatformSet::setPositionReportingDefaultDistanceInterval(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x2c;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x2c,value);
}
QString PlatformSet::readDriverNoSignInDistanceInterval() const
{
//    return mPlatformConfig.getValue(0x8103,0x2d,0).toString();
    return QString(readDeviceParam(0x8103,0x2d));
}
void PlatformSet::setDriverNoSignInDistanceInterval(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x2d;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x2d,value);
}
QString PlatformSet::readPositionReportingSleepTimeInterval() const
{
//    return mPlatformConfig.getValue(0x8103,0x27,0).toString();
    return QString(readDeviceParam(0x8103,0x27));
}
void PlatformSet::setPositionReportingSleepTimeInterval(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x27;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x27,value);
}
QString PlatformSet::readPositionReportingUrgentTimeInterval() const
{
//    return mPlatformConfig.getValue(0x8103,0x28,0).toString();
    return QString(readDeviceParam(0x8103,0x28));
}
void PlatformSet::setPositionReportingUrgentTimeInterval(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x28;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x28,value);
}
QString PlatformSet::readPositionReportingDefaultTimeInterval() const
{
//    return mPlatformConfig.getValue(0x8103,0x29,0).toString();
    return QString(readDeviceParam(0x8103,0x29));
}
void PlatformSet::setPositionReportingDefaultTimeInterval(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x29;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x29,value);
}
QString PlatformSet::readDriverNoSignInTimeInterval() const
{
//    return mPlatformConfig.getValue(0x8103,0x22,0).toString();
    return QString(readDeviceParam(0x8103,0x22));
}
void PlatformSet::setDriverNoSignInTimeInterval(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x22;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x22,value);
}

QString PlatformSet::readInflectionCompensationAngle() const
{
//    return mPlatformConfig.getValue(0x8103,0x30,0).toString();
    return QString(readDeviceParam(0x8103,0x30));
}
void PlatformSet::setInflectionCompensationAngle(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x30;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x30,value);
}
QString PlatformSet::readElectronicFenceRadius() const
{
//    return mPlatformConfig.getValue(0x8103,0x31,0).toString();
    return QString(readDeviceParam(0x8103,0x31));
}
void PlatformSet::setElectronicFenceRadius(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x31;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x31,value);
}
QString PlatformSet::readTelephoneOfMonitoringPlatform() const
{
//    return mPlatformConfig.getValue(0x8103,0x40,0).toString();
    return QString(readDeviceParam(0x8103,0x40));
}
void PlatformSet::setTelephoneOfMonitoringPlatform(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x40;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x40,value);
}
QString PlatformSet::readTelephoneOfReset() const
{
//    return mPlatformConfig.getValue(0x8103,0x41,0).toString();
    return QString(readDeviceParam(0x8103,0x41));
}
void PlatformSet::setTelephoneOfReset(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x41;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x41,value);
}
QString PlatformSet::readTelephoneOfFactory() const
{
//    return mPlatformConfig.getValue(0x8103,0x42,0).toString();
    return QString(readDeviceParam(0x8103,0x42));
}
void PlatformSet::setTelephoneOfFactory(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x42;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x42,value);
}
QString PlatformSet::readSmsTelephoneOfMonitoringPlatform() const
{
//    return mPlatformConfig.getValue(0x8103,0x43,0).toString();
    return QString(readDeviceParam(0x8103,0x43));
}
void PlatformSet::setSmsTelephoneOfMonitoringPlatform(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x43;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x43,value);
}
QString PlatformSet::readSmsTelephoneOfTextAlarm() const
{
//    return mPlatformConfig.getValue(0x8103,0x44,0).toString();
    return QString(readDeviceParam(0x8103,0x44));
}
void PlatformSet::setSmsTelephoneOfTextAlarm(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x44;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x44,value);
}
int PlatformSet::readTelephoneReceptionStrategy() const
{
//    return mPlatformConfig.getValue(0x8103,0x45,0).toInt();
    return readDeviceParam(0x8103,0x45).toInt();
}
void PlatformSet::setTelephoneReceptionStrategy(int value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x45;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x45,value);
}

QString PlatformSet::readOnesReceptionMaxTime() const
{
//    return mPlatformConfig.getValue(0x8103,0x46,0).toString();
    return QString(readDeviceParam(0x8103,0x46));
}
void PlatformSet::setOnesReceptionMaxTime(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x46;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x46,value);
}
QString PlatformSet::readMonthReceptionMaxTime() const
{
//    return mPlatformConfig.getValue(0x8103,0x47,0).toString();
    return QString(readDeviceParam(0x8103,0x47));
}
void PlatformSet::setMonthReceptionMaxTime(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x47;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x47,value);
}
QString PlatformSet::readMonitorTelephoneNumber() const
{
//    return mPlatformConfig.getValue(0x8103,0x48,0).toString();
    return QString(readDeviceParam(0x8103,0x48));
}
void PlatformSet::setMonitorTelephoneNumber(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x48;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x48,value);
}
QString PlatformSet::readPlatformSpecialNumber() const
{
//    return mPlatformConfig.getValue(0x8103,0x49,0).toString();
    return QString(readDeviceParam(0x8103,0x49));
}
void PlatformSet::setPlatformSpecialNumber(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x49;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x49,value);
}
QString PlatformSet::readTopSpeed() const
{
//    return mPlatformConfig.getValue(0x8103,0x55,0).toString();
    return QString(readDeviceParam(0x8103,0x55));
}
void PlatformSet::setTopSpeed(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x55;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x55,value);
}
QString PlatformSet::readOverspeedDuration() const
{
//    return mPlatformConfig.getValue(0x8103,0x56,0).toString();
    return QString(readDeviceParam(0x8103,0x56));
}
void PlatformSet::setOverspeedDuration(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x56;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x56,value);
}
QString PlatformSet::readDriverDuration() const
{
//    return mPlatformConfig.getValue(0x8103,0x57,0).toString();
    return QString(readDeviceParam(0x8103,0x57));
}
void PlatformSet::setDriverDuration(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x57;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x57,value);
}
QString PlatformSet::readDriverDayDuration() const
{
//    return mPlatformConfig.getValue(0x8103,0x58,0).toString();
    return QString(readDeviceParam(0x8103,0x58));
}
void PlatformSet::setDriverDayDuration(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x58;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x58,value);
}
QString PlatformSet::readMinRestTime() const
{
//    return mPlatformConfig.getValue(0x8103,0x59,0).toString();
    return QString(readDeviceParam(0x8103,0x59));
}
void PlatformSet::setMinRestTime(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x59;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x59,value);
}
QString PlatformSet::readMaxStopTime() const
{
//    return mPlatformConfig.getValue(0x8103,0x5a,0).toString();
    return QString(readDeviceParam(0x8103,0x5a));
}
void PlatformSet::setMaxStopTime(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x5a;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x5a,value);
}
QString PlatformSet::readSadOverSpeed() const
{
//    return mPlatformConfig.getValue(0x8103,0x5b,0).toString();
    return QString(readDeviceParam(0x8103,0x5b));
}
void PlatformSet::setSadOverSpeed(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x5b;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x5b,value);
}

QString PlatformSet::readSadFatigueDriving() const
{
//    return mPlatformConfig.getValue(0x8103,0x5c,0).toString();
    return QString(readDeviceParam(0x8103,0x5c));
}
void PlatformSet::setSadFatigueDriving(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x5c;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x5c,value);
}

QString PlatformSet::readCollisionAlarm() const
{
//    return mPlatformConfig.getValue(0x8103,0x5d,0).toString();
    return QString(readDeviceParam(0x8103,0x5d));
}
void PlatformSet::setCollisionAlarm(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x5d;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x5d,value);
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
//    return mPlatformConfig.getValue(0x8103,0x5e,0).toString();
    return QString(readDeviceParam(0x8103,0x5e));
}
void PlatformSet::setRolloverAngle(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x5e;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x5e,value);
}
QString PlatformSet::readProvienceID() const
{
//    return mPlatformConfig.getValue(0x8103,0x81,0).toString();
    return QString(readDeviceParam(0x8103,0x81));
}
void PlatformSet::setProvienceID(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x81;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x81,value);
}
QString PlatformSet::readCityID() const
{
//    return mPlatformConfig.getValue(0x8103,0x82,0).toString();
    return QString(readDeviceParam(0x8103,0x82));
}
void PlatformSet::setCityID(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x82;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x82,value);
}
QString PlatformSet::readCarLisence() const
{
//    return mPlatformConfig.getValue(0x8103,0x83,0).toString();
    return QString(readDeviceParam(0x8103,0x83));
}
void PlatformSet::setCarLisence(QString value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x83;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x83,value);
}
int PlatformSet::readCarColor() const
{
//    return mPlatformConfig.getValue(0x8103,0x84,0).toInt();
    return readDeviceParam(0x8103,0x84).toInt();
}
void PlatformSet::setCarColor(int value)
{
//    DeviceParam config;
//    config.id = 0x8103;
//    config.num = 0x84;
//    config.field = 0x0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8103,0x84,value);
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
    config.value = value;

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
    emit authNumberChnaged();
}


int PlatformSet::readDeviceType() const
{
//    return mPlatformConfig.getValue(0x8107,0,0).toInt();
    return readDeviceParam(0x8107,0,0).toInt();
}
void PlatformSet::setDeviceType(int value)
{
//    DeviceParam config;
//    config.id = 0x8107;
//    config.num = 0;
//    config.field = 0;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8107,0,0,value);
}

QString PlatformSet::readProduceID() const
{
//    return mPlatformConfig.getValue(0x8107,0,1).toString();
    return QString(readDeviceParam(0x8107,0,1));
}
void PlatformSet::setProduceID(QString value)
{
//    DeviceParam config;
//    config.id = 0x8107;
//    config.num = 0;
//    config.field = 1;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8107,0,1,value);
}

QString PlatformSet::readDeviceModel() const
{
//    return mPlatformConfig.getValue(0x8107,0,2).toString();
    return QString(readDeviceParam(0x8107,0,2));
}
void PlatformSet::setDeviceModel(QString value)
{
//    DeviceParam config;
//    config.id = 0x8107;
//    config.num = 0;
//    config.field = 2;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8107,0,2,value);
}

QString PlatformSet::readDeviceID() const
{
//    return mPlatformConfig.getValue(0x8107,0,3).toString();
    return QString(readDeviceParam(0x8107,0,3));
}
void PlatformSet::setDeviceID(QString value)
{
//    DeviceParam config;
//    config.id = 0x8107;
//    config.num = 0;
//    config.field = 3;
//    config.value = value;

//    mPlatformConfig.insertSignalData(config);
    setDeviceParam(0x8107,0,3,value);
}

QString PlatformSet::readSIMICCID() const
{
    return QString(readDeviceParam(0x8107,0,4));
}
void PlatformSet::setSIMICCID(QString value)
{
    setDeviceParam(0x8107,0,4,value);
}
QString PlatformSet::readHWVersion() const
{
    return QString(readDeviceParam(0x8107,0,5));
}
void PlatformSet::setHWVersion(QString value)
{
    setDeviceParam(0x8107,0,5,value);
}
QString PlatformSet::readFWVersion() const
{
    return QString(readDeviceParam(0x8107,0,6));
}
void PlatformSet::setFWVersion(QString value)
{
    setDeviceParam(0x8107,0,6,value);
}
int PlatformSet::readGNSSAttr() const
{
    return readDeviceParam(0x8107,0,7).toInt();
}
void PlatformSet::setGNSSAttr(int value)
{
    setDeviceParam(0x8107,0,7,value);
}

int PlatformSet::readCommunicatModule() const
{
    return readDeviceParam(0x8107,0,8).toInt();
}

void PlatformSet::setCommunicatModule(int value)
{
    setDeviceParam(0x8107,0,8,value);
}

//void PlatformSet::updateServerInfo(int type)
//{
//    serverInfoChanged(type);
//}

