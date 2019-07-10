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

void AlarmSet::setRecordAlarm(int Chn,int type,bool open,bool ShowWin,bool SoundAlarm,bool Report)
{
    if(type == 0){

        setConfig(ODRootName+QString::number(Chn),OpenRecordAlarm,open);
        setConfig(ODRootName+QString::number(Chn),EnableAlarmWin,ShowWin);
        setConfig(ODRootName+QString::number(Chn),EnableSoundAlarm,SoundAlarm);
        setConfig(ODRootName+QString::number(Chn),EnableReport,Report);
    }else if(type == 1){
        setConfig(MDRootName+QString::number(Chn),OpenRecordAlarm,open);
        setConfig(MDRootName+QString::number(Chn),EnableAlarmWin,ShowWin);
        setConfig(MDRootName+QString::number(Chn),EnableSoundAlarm,SoundAlarm);
        setConfig(MDRootName+QString::number(Chn),EnableReport,Report);
        emit setRecordAlarmConfig(Chn);
    }

}

bool AlarmSet::getRecordDispEnable(int Chn,int type)
{
    if(type == 0){
        return getConfig(ODRootName+QString::number(Chn),EnableAlarmWin).toBool();
    }else if(type == 1){
        return getConfig(MDRootName+QString::number(Chn),EnableAlarmWin).toBool();
    }

    return true;
}

bool AlarmSet::getRecordSoundEnable(int Chn,int type)
{
    if(type == 0){
        return getConfig(ODRootName+QString::number(Chn),EnableSoundAlarm).toBool();
    }else if(type == 1){
        return getConfig(MDRootName+QString::number(Chn),EnableSoundAlarm).toBool();
    }

    return true;
}

bool AlarmSet::getRecordReportEnable(int Chn,int type)
{
    if(type == 0){
        return getConfig(ODRootName+QString::number(Chn),EnableReport).toBool();
    }else if (type == 1) {
        return getConfig(MDRootName+QString::number(Chn),EnableReport).toBool();
    }

    return true;
}

bool AlarmSet::getOpenRecord(int Chn,int type)
{
    if(type == 0){
        return getConfig(ODRootName+QString::number(Chn),OpenRecordAlarm).toBool();
    }else if (type == 1) {
        return getConfig(MDRootName+QString::number(Chn),OpenRecordAlarm).toBool();
    }

    return true;
}
