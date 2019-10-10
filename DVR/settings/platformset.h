#ifndef PLATFORMSET_H
#define PLATFORMSET_H

#include <QObject>
//#include "config/deviceparamconfig.h"
#include "config/platformconfigini.h"

class PlatformSet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString heartBeat READ readHeartBeat WRITE setHeartBeat NOTIFY heartBeatChanged)
    Q_PROPERTY(QString tcpAnswerTimeout READ readTcpAnswerTimeout WRITE setTcpAnswerTimeout NOTIFY tcpAnswerTimeoutChanged)
    Q_PROPERTY(QString tcpRepeatTimes READ readTcpRepeatTimes WRITE setTcpRepeatTimes NOTIFY tcpRepeatTimesChanged)
    Q_PROPERTY(QString udpAnswerTimeout READ readUdpAnswerTimeout WRITE setUdpAnswerTimeout NOTIFY udpAnswerTimeoutChanged)
    Q_PROPERTY(QString udpRepeatTimes READ readUdpRepeatTimes WRITE setUdpRepeatTimes NOTIFY ucpRepeatTimesChanged)
    Q_PROPERTY(QString smsAnswerTimeout READ readSmsAnswerTimeout WRITE setSmsAnswerTimeout NOTIFY smsAnswerTimeoutChanged)
    Q_PROPERTY(QString smsRepeatTimes READ readSmsRepeatTimes WRITE setSmsRepeatTimes NOTIFY smsRepeatTimesChanged)
    Q_PROPERTY(QString mainServer READ readMainServer WRITE setMainServer NOTIFY mainServerChanged)
    Q_PROPERTY(QString mainServerIp READ readMainServerIp WRITE setMainServerIp NOTIFY mainServerIpChanged)
    Q_PROPERTY(QString mainServerTcpPort READ readMainServerTcpPort WRITE setMainServerTcpPort NOTIFY mainServerTcpPortChanged)
    Q_PROPERTY(QString mainServerUdpPort READ readMainServerUdpPort WRITE setMainServerUdpPort NOTIFY mainServerUdpPortChanged)
    Q_PROPERTY(QString backupServer READ readBackupServer WRITE setBackupServer NOTIFY backupServerChanged)
    Q_PROPERTY(QString backupServerIp READ readBackupServerIp WRITE setBackupServerIp NOTIFY backupServerIpChanged)
    Q_PROPERTY(QString backupServerTcpPort READ readBackupServerTcpPort WRITE setBackupServerTcpPort NOTIFY backupServerTcpPortChanged)
    Q_PROPERTY(QString backupServerUdpPort READ readBackupServerUdpPort WRITE setBackupServerUdpPort NOTIFY backupServerUdpPortChanged)
    Q_PROPERTY(QString icAuthServerIp READ readIcAuthServerIp WRITE setIcAuthServerIp NOTIFY icAuthServerIpChanged)
    Q_PROPERTY(QString icAuthBackupServerIp READ readIcAuthBackupServerIp WRITE setIcAuthBackupServerIp NOTIFY icAuthBackupServerIpChanged)
    Q_PROPERTY(QString icAuthServerTcpPort READ readIcAuthServerTcpPort WRITE setIcAuthServerTcpPort NOTIFY icAuthServerTcpPortChanged)
    Q_PROPERTY(QString icAuthServerUdpPort READ readIcAuthServerUdpPort WRITE setIcAuthServerUdpPort NOTIFY icAuthServerUdpPortChanged)
    Q_PROPERTY(QString positionReportingStrategy READ readPositionReportingStrategy WRITE setPositionReportingStrategy NOTIFY positionReportingStrategyChnaged)
    Q_PROPERTY(QString positionReportingProgramme READ readPositionReportingProgramme WRITE setPositionReportingProgramme NOTIFY positionReportingProgrammeChnaged)
    Q_PROPERTY(QString positionReportingSleepDistanceInterval READ readPositionReportingSleepDistanceInterval WRITE setPositionReportingSleepDistanceInterval NOTIFY positionReportingSleepDistanceIntervalChnaged)
    Q_PROPERTY(QString positionReportingUrgentDistanceInterval READ readPositionReportingUrgentDistanceInterval WRITE setPositionReportingUrgentDistanceInterval NOTIFY positionReportingUrgentDistanceIntervalChnaged)
    Q_PROPERTY(QString positionReportingDefaultDistanceInterval READ readPositionReportingDefaultDistanceInterval WRITE setPositionReportingDefaultDistanceInterval NOTIFY positionReportingDefaultDistanceIntervalChnaged)
    Q_PROPERTY(QString driverNoSignInDistanceInterval READ readDriverNoSignInDistanceInterval WRITE setDriverNoSignInDistanceInterval NOTIFY driverNoSignInDistanceIntervalChnaged)
    Q_PROPERTY(QString positionReportingSleepTimeInterval READ readPositionReportingSleepTimeInterval WRITE setPositionReportingSleepTimeInterval NOTIFY positionReportingSleepTimeIntervalChnaged)
    Q_PROPERTY(QString positionReportingUrgentTimeInterval READ readPositionReportingUrgentTimeInterval WRITE setPositionReportingUrgentTimeInterval NOTIFY positionReportingUrgentTimeIntervalChnaged)
    Q_PROPERTY(QString positionReportingDefaultTimeInterval READ readPositionReportingDefaultTimeInterval WRITE setPositionReportingDefaultTimeInterval NOTIFY positionReportingDefaultTimeIntervalChnaged)
    Q_PROPERTY(QString driverNoSignInTimeInterval READ readDriverNoSignInTimeInterval WRITE setDriverNoSignInTimeInterval NOTIFY driverNoSignInTimeIntervalChnaged)
    Q_PROPERTY(QString inflectionCompensationAngle READ readInflectionCompensationAngle WRITE setInflectionCompensationAngle NOTIFY inflectionCompensationAngleChnaged)
    Q_PROPERTY(QString electronicFenceRadius READ readElectronicFenceRadius WRITE setElectronicFenceRadius NOTIFY electronicFenceRadiusChnaged)
    Q_PROPERTY(QString telephoneOfMonitoringPlatform READ readTelephoneOfMonitoringPlatform WRITE setTelephoneOfMonitoringPlatform NOTIFY telephoneOfMonitoringPlatformChnaged)
    Q_PROPERTY(QString telephoneOfReset READ readTelephoneOfReset WRITE setTelephoneOfReset NOTIFY telephoneOfResetChnaged)
    Q_PROPERTY(QString telephoneOfFactory READ readTelephoneOfFactory WRITE setTelephoneOfFactory NOTIFY telephoneOfFactoryChnaged)
    Q_PROPERTY(QString smsTelephoneOfMonitoringPlatform READ readSmsTelephoneOfMonitoringPlatform WRITE setSmsTelephoneOfMonitoringPlatform NOTIFY smsTelephoneOfMonitoringPlatformChnaged)
    Q_PROPERTY(QString smsTelephoneOfTextAlarm READ readSmsTelephoneOfTextAlarm WRITE setSmsTelephoneOfTextAlarm NOTIFY smsTelephoneOfTextAlarmChnaged)
    Q_PROPERTY(QString telephoneReceptionStrategy READ readTelephoneReceptionStrategy WRITE setTelephoneReceptionStrategy NOTIFY telephoneReceptionStrategyChnaged)
    Q_PROPERTY(QString onesReceptionMaxTime READ readOnesReceptionMaxTime WRITE setOnesReceptionMaxTime NOTIFY onesReceptionMaxTimeChnaged)
    Q_PROPERTY(QString monthReceptionMaxTime READ readMonthReceptionMaxTime WRITE setMonthReceptionMaxTime NOTIFY monthReceptionMaxTimeChnaged)
    Q_PROPERTY(QString monitorTelephoneNumber READ readMonitorTelephoneNumber WRITE setMonitorTelephoneNumber NOTIFY monitorTelephoneNumberChnaged)
    Q_PROPERTY(QString platformSpecialNumber READ readPlatformSpecialNumber WRITE setPlatformSpecialNumber NOTIFY platformSpecialNumberChnaged)
    Q_PROPERTY(QString topSpeed READ readTopSpeed WRITE setTopSpeed NOTIFY topSpeedChnaged)
    Q_PROPERTY(QString overspeedDuration READ readOverspeedDuration WRITE setOverspeedDuration NOTIFY overspeedDurationChnaged)
    Q_PROPERTY(QString driverDuration READ readDriverDuration WRITE setDriverDuration NOTIFY driverDurationChnaged)
    Q_PROPERTY(QString driverDayDuration READ readDriverDayDuration WRITE setDriverDayDuration NOTIFY driverDayDurationChnaged)
    Q_PROPERTY(QString minRestTime READ readMinRestTime WRITE setMinRestTime NOTIFY minRestTimeChnaged)
    Q_PROPERTY(QString maxStopTime READ readMaxStopTime WRITE setMaxStopTime NOTIFY maxStopTimeChnaged)
    Q_PROPERTY(QString sadOverSpeed READ readSadOverSpeed WRITE setSadOverSpeed NOTIFY sadOverSpeedChnaged)
    Q_PROPERTY(QString sadFatigueDriving READ readSadFatigueDriving WRITE setSadFatigueDriving NOTIFY sadFatigueDrivingChnaged)
    Q_PROPERTY(QString collisionAlarm READ readCollisionAlarm WRITE setCollisionAlarm NOTIFY collisionAlarmChnaged)
    Q_PROPERTY(QString collisionAlarmTime READ readCollisionAlarmTime NOTIFY collisionAlarmTimeChnaged)
    Q_PROPERTY(QString collisionAlarmAcc READ readCollisionAlarmAcc NOTIFY collisionAlarmAccChnaged)
    Q_PROPERTY(QString rolloverAngle READ readRolloverAngle WRITE setRolloverAngle NOTIFY rolloverAngleChnaged)
    Q_PROPERTY(QString provienceID READ readProvienceID WRITE setProvienceID NOTIFY provienceIDChnaged)
    Q_PROPERTY(QString cityID READ readCityID WRITE setCityID NOTIFY cityIDChnaged)
    Q_PROPERTY(QString carLisence READ readCarLisence WRITE setCarLisence NOTIFY carLisenceChanged)
    Q_PROPERTY(int carColor READ readCarColor WRITE setCarColor NOTIFY carColorChnaged)
    Q_PROPERTY(QString carVinNumber READ readCarVinNumber WRITE setCarVinNumber NOTIFY carVinNumberChnaged)
    Q_PROPERTY(bool registerEnable READ readRegisterEnable WRITE setRegisterEnable NOTIFY registerEnableChnaged)
    Q_PROPERTY(int registerType READ readRegisterType WRITE setRegisterType NOTIFY registerTypeChnaged)
    Q_PROPERTY(QString phoneNumber READ readPhoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChnaged)
    Q_PROPERTY(bool mainEnableTcp READ readMainEnableTcp WRITE setMainEnableTcp NOTIFY mainEnableTcpChnaged)
    Q_PROPERTY(bool mainEnableUdp READ readMainEnableUdp WRITE setMainEnableUdp NOTIFY mainEnableUdpChnaged)
    Q_PROPERTY(bool secEnableTcp READ readSecEnableTcp WRITE setSecEnableTcp NOTIFY secEnableTcpChnaged)
    Q_PROPERTY(bool secEnableUdp READ readSecEnableUdp WRITE setMainEnableUdp NOTIFY secEnableUdpChnaged)
    Q_PROPERTY(QString produceID READ readProduceID WRITE setProduceID NOTIFY produceIDChnaged)
    Q_PROPERTY(int deviceType READ readDeviceType WRITE setDeviceType NOTIFY deviceTypeChnaged)
    Q_PROPERTY(QString deviceModel READ readDeviceModel WRITE setDeviceModel NOTIFY deviceModelChnaged)
    Q_PROPERTY(QString deviceID READ readDeviceID WRITE setDeviceID NOTIFY deviceIDChnaged)

public:
    explicit PlatformSet(QObject *parent = nullptr);

    QString readHeartBeat() const;
    void setHeartBeat(QString value);
    QString readTcpAnswerTimeout() const;
    void setTcpAnswerTimeout(QString value);
    QString readTcpRepeatTimes() const;
    void setTcpRepeatTimes(QString value);
    QString readUdpAnswerTimeout() const;
    void setUdpAnswerTimeout(QString value);
    QString readUdpRepeatTimes() const;
    void setUdpRepeatTimes(QString value);
    QString readSmsAnswerTimeout() const;
    void setSmsAnswerTimeout(QString value);
    QString readSmsRepeatTimes() const;
    void setSmsRepeatTimes(QString value);
    QString readMainServer() const;
    void setMainServer(QString value);
    QString readMainServerIp() const ;
    void setMainServerIp(QString value);
    QString readMainServerTcpPort() const ;
    void setMainServerTcpPort(QString value);
    QString readMainServerUdpPort() const ;
    void setMainServerUdpPort(QString value);
    QString readBackupServer() const;
    void setBackupServer(QString value);
    QString readBackupServerIp() const ;
    void setBackupServerIp(QString value);
    QString readBackupServerTcpPort() const ;
    void setBackupServerTcpPort(QString value);
    QString readBackupServerUdpPort() const ;
    void setBackupServerUdpPort(QString value);
    QString readIcAuthServerIp() const;
    void setIcAuthServerIp(QString value);
    QString readIcAuthBackupServerIp() const;
    void setIcAuthBackupServerIp(QString value);
    QString readIcAuthServerTcpPort() const;
    void setIcAuthServerTcpPort(QString value);
    QString readIcAuthServerUdpPort() const;
    void setIcAuthServerUdpPort(QString value);
    QString readPositionReportingStrategy() const;
    void setPositionReportingStrategy(QString value);
    QString readPositionReportingProgramme() const;
    void setPositionReportingProgramme(QString value);
    QString readPositionReportingSleepDistanceInterval() const;
    void setPositionReportingSleepDistanceInterval(QString value);
    QString readPositionReportingUrgentDistanceInterval() const;
    void setPositionReportingUrgentDistanceInterval(QString value);
    QString readPositionReportingDefaultDistanceInterval() const;
    void setPositionReportingDefaultDistanceInterval(QString value);
    QString readDriverNoSignInDistanceInterval() const;
    void setDriverNoSignInDistanceInterval(QString value);
    QString readPositionReportingSleepTimeInterval() const;
    void setPositionReportingSleepTimeInterval(QString value);
    QString readPositionReportingUrgentTimeInterval() const;
    void setPositionReportingUrgentTimeInterval(QString value);
    QString readPositionReportingDefaultTimeInterval() const;
    void setPositionReportingDefaultTimeInterval(QString value);
    QString readDriverNoSignInTimeInterval() const;
    void setDriverNoSignInTimeInterval(QString value);

    QString readInflectionCompensationAngle() const;
    void setInflectionCompensationAngle(QString value);
    QString readElectronicFenceRadius() const;
    void setElectronicFenceRadius(QString value);
    QString readTelephoneOfMonitoringPlatform() const;
    void setTelephoneOfMonitoringPlatform(QString value);
    QString readTelephoneOfReset() const;
    void setTelephoneOfReset(QString value);
    QString readTelephoneOfFactory() const;
    void setTelephoneOfFactory(QString value);
    QString readSmsTelephoneOfMonitoringPlatform() const;
    void setSmsTelephoneOfMonitoringPlatform(QString value);
    QString readSmsTelephoneOfTextAlarm() const;
    void setSmsTelephoneOfTextAlarm(QString value);
    QString readTelephoneReceptionStrategy() const;
    void setTelephoneReceptionStrategy(QString value);

    QString readOnesReceptionMaxTime() const;
    void setOnesReceptionMaxTime(QString value);
    QString readMonthReceptionMaxTime() const;
    void setMonthReceptionMaxTime(QString value);
    QString readMonitorTelephoneNumber() const;
    void setMonitorTelephoneNumber(QString value);
    QString readPlatformSpecialNumber() const;
    void setPlatformSpecialNumber(QString value);
    QString readTopSpeed() const;
    void setTopSpeed(QString value);
    QString readOverspeedDuration() const;
    void setOverspeedDuration(QString value);
    QString readDriverDuration() const;
    void setDriverDuration(QString value);
    QString readDriverDayDuration() const;
    void setDriverDayDuration(QString value);
    QString readMinRestTime() const;
    void setMinRestTime(QString value);
    QString readMaxStopTime() const;
    void setMaxStopTime(QString value);
    QString readSadOverSpeed() const;
    void setSadOverSpeed(QString value);
    QString readSadFatigueDriving() const;
    void setSadFatigueDriving(QString value);
    QString readCollisionAlarm() const;
    void setCollisionAlarm(QString value);
    QString readCollisionAlarmTime() const;
    QString readCollisionAlarmAcc() const;
    QString readRolloverAngle() const;
    void setRolloverAngle(QString value);
    QString readProvienceID() const;
    void setProvienceID(QString value);
    QString readCityID() const;
    void setCityID(QString value);
    QString readCarLisence() const;
    void setCarLisence(QString value);
    int readCarColor() const;
    void setCarColor(int value);
    QString readCarVinNumber() const;
    void setCarVinNumber(QString value);
    bool readRegisterEnable() const;
    void setRegisterEnable(bool value);
    int readRegisterType() const;
    void setRegisterType(int value);
    QString readPhoneNumber() const;
    void setPhoneNumber(QString value);

    bool readMainEnableTcp() const;
    void setMainEnableTcp(bool value) ;
    bool readMainEnableUdp() const;
    void setMainEnableUdp(bool value) ;
    bool readSecEnableTcp() const;
    void setSecEnableTcp(bool value) ;
    bool readSecEnableUdp() const;
    void setSecEnableUdp(bool value) ;

    QString readProduceID() const;
    void setProduceID(QString value);
    int readDeviceType() const;
    void setDeviceType(int value);
    QString readDeviceModel() const;
    void setDeviceModel(QString value);
    QString readDeviceID() const;
    void setDeviceID(QString value);

signals:
    void heartBeatChanged();
    void tcpAnswerTimeoutChanged();
    void tcpRepeatTimesChanged();
    void udpAnswerTimeoutChanged();
    void ucpRepeatTimesChanged();
    void smsAnswerTimeoutChanged();
    void  smsRepeatTimesChanged();
    void mainServerChanged();
    void mainServerIpChanged();
    void mainServerTcpPortChanged();
    void mainServerUdpPortChanged();
    void backupServerChanged();
    void backupServerIpChanged();
    void backupServerTcpPortChanged();
    void backupServerUdpPortChanged();
    void icAuthServerIpChanged();
    void icAuthBackupServerIpChanged();
    void icAuthServerTcpPortChanged();
    void icAuthServerUdpPortChanged();
    void positionReportingStrategyChnaged();
    void positionReportingProgrammeChnaged();
    void positionReportingSleepDistanceIntervalChnaged();
    void positionReportingUrgentDistanceIntervalChnaged();
    void positionReportingDefaultDistanceIntervalChnaged();
    void driverNoSignInDistanceIntervalChnaged();
    void positionReportingSleepTimeIntervalChnaged();
    void positionReportingUrgentTimeIntervalChnaged();
    void positionReportingDefaultTimeIntervalChnaged();
    void driverNoSignInTimeIntervalChnaged();
    void inflectionCompensationAngleChnaged();
    void electronicFenceRadiusChnaged();
    void telephoneOfMonitoringPlatformChnaged();
    void telephoneOfResetChnaged();
    void telephoneOfFactoryChnaged();
    void smsTelephoneOfMonitoringPlatformChnaged();
    void smsTelephoneOfTextAlarmChnaged();
    void telephoneReceptionStrategyChnaged();
    void onesReceptionMaxTimeChnaged();
    void monthReceptionMaxTimeChnaged();
    void monitorTelephoneNumberChnaged();
    void platformSpecialNumberChnaged();
    void topSpeedChnaged();
    void overspeedDurationChnaged();
    void driverDurationChnaged();
    void driverDayDurationChnaged();
    void minRestTimeChnaged();
    void maxStopTimeChnaged();
    void sadOverSpeedChnaged();
    void sadFatigueDrivingChnaged();
    void collisionAlarmChnaged();
    void collisionAlarmTimeChnaged();
    void collisionAlarmAccChnaged();
    void rolloverAngleChnaged();
    void provienceIDChnaged();
    void cityIDChnaged();
    void carLisenceChanged();
    void carColorChnaged();
    void registerEnableChnaged();
    void registerTypeChnaged();
    void phoneNumberChnaged();
    void mainEnableTcpChnaged();
    void mainEnableUdpChnaged();
    void secEnableTcpChnaged();
    void secEnableUdpChnaged();
    void produceIDChnaged();
    void deviceTypeChnaged();
    void deviceModelChnaged();
    void deviceIDChnaged();
    void carVinNumberChnaged();

public slots:

private:
    PlatformConfigIni mPlatformConfig ;
};

#endif // PLATFORMSET_H
