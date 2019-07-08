#include "alarmset.h"

AlarmSet::AlarmSet(QObject *parent, const QString &filename) : QObject(parent),Config(filename)
{

}

void AlarmSet::setIoAlarm(int io,QString alarmName,int trigLevel,int VideoChn,bool showdisp,bool soundalarm,bool report)
{
    setConfig(IoRootName+QString::number(io),AlarmName,alarmName);
    setConfig(IoRootName+QString::number(io),TrigLevel,trigLevel);
    setConfig(IoRootName+QString::number(io),ConnectVideo,VideoChn);
    setConfig(IoRootName+QString::number(io),EnableAlarmWin,showdisp);
    setConfig(IoRootName+QString::number(io),EnableSoundAlarm,soundalarm);
    setConfig(IoRootName+QString::number(io),EnableReport,report);
}

bool AlarmSet::getIoDispEnable(int io)
{
    return getConfig(IoRootName+QString::number(io),EnableAlarmWin).toBool();
}

bool AlarmSet::getIoSoundEnable(int io)
{
    return getConfig(IoRootName+QString::number(io),EnableSoundAlarm).toBool();
}

bool AlarmSet::getIoReportEnable(int io)
{
    return getConfig(IoRootName+QString::number(io),EnableReport).toBool();
}

int  AlarmSet::getTrigLevel(int io)
{
    return getConfig(IoRootName+QString::number(io),TrigLevel).toInt();
}

int  AlarmSet::getConnectVideoChn(int io)
{
    return getConfig(IoRootName+QString::number(io),ConnectVideo).toInt();
}

QString  AlarmSet::getAlarmName(int io)
{
    return getConfig(IoRootName+QString::number(io),AlarmName).toString();
}

void AlarmSet::setRecordAlarm(int Chn,bool ShowWin,bool SoundAlarm,bool Report)
{
    setConfig(RecordRootName+QString::number(Chn),EnableAlarmWin,ShowWin);
    setConfig(RecordRootName+QString::number(Chn),EnableSoundAlarm,SoundAlarm);
    setConfig(RecordRootName+QString::number(Chn),EnableReport,Report);
    setRecordAlarmConfig(Chn);
}

bool AlarmSet::getRecordDispEnable(int Chn)
{
    return getConfig(RecordRootName+QString::number(Chn),EnableAlarmWin).toBool();
}

bool AlarmSet::getRecordSoundEnable(int Chn)
{
    return getConfig(RecordRootName+QString::number(Chn),EnableSoundAlarm).toBool();
}

bool AlarmSet::getRecordReportEnable(int Chn)
{
    return getConfig(RecordRootName+QString::number(Chn),EnableReport).toBool();
}
