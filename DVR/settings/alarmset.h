#ifndef ALARMSET_H
#define ALARMSET_H

#include <QObject>
#include "config.h"

class AlarmSet : public QObject,public Config
{
    Q_OBJECT
//    Q_PROPERTY(bool enableIoAlarm WRITE setIoAlarm READ getIoAlarm)
public:
    explicit AlarmSet(QObject *parent = nullptr,const QString &filename = ".alarmset.ini");

    Q_INVOKABLE void setIoAlarm(int io,QString alarmName,int trigLevel,int VideoChn,bool showdisp,bool soundalarm,bool report);
    Q_INVOKABLE bool getIoDispEnable(int io);
    Q_INVOKABLE bool getIoSoundEnable(int io);
    Q_INVOKABLE bool getIoReportEnable(int io);
    Q_INVOKABLE int  getTrigLevel(int io);
    Q_INVOKABLE int  getConnectVideoChn(int io);
    Q_INVOKABLE QString  getAlarmName(int io);

    Q_INVOKABLE void setRecordAlarm(int Chn,bool open,bool ShowWin,bool SoundAlarm,bool Report);
    Q_INVOKABLE bool getRecordDispEnable(int Chn);
    Q_INVOKABLE bool getRecordSoundEnable(int Chn);
    Q_INVOKABLE bool getRecordReportEnable(int Chn);
    Q_INVOKABLE bool getOpenRecord(int Chn);
signals:
    void setRecordAlarmConfig(int Chn);

public slots:

public:
    const QString IoRootName = "IO";
    const QString RecordRootName = "Record";
    const QString AlarmName = "AlarmName";
    const QString TrigLevel = "TrigLevel";
    const QString ConnectVideo = "ConnectVideo";
    const QString EnableAlarmWin = "EnableAlarmWin";
    const QString EnableSoundAlarm = "EnableSoundAlarm";
    const QString EnableReport = "EnableReport";
    const QString OpenRecordAlarm = "OpenRecordAlarm";
    const QString EnableRecordAlarmWin = "EnableRecordAlarmWin";
    const QString EnableRecordSoundAlarm = "EnableRecordSoundAlarm";
    const QString EnableRecordReport = "EnableRecordReport";

};

#endif // ALARMSET_H
