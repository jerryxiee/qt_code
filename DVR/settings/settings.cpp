#include "settings.h"

VencSet *Settings::mVencIni = nullptr;
DispSet *Settings::mDispSetIni = nullptr;
SystemSet *Settings::mSystemSetIni = nullptr;
AlarmSet *Settings::mAlarmSetIni = nullptr;

Settings::Settings(QObject *parent) : QObject(parent)
{

}

VencSet *Settings::getVencIni()
{
    if(mVencIni == nullptr){
        mVencIni = new VencSet();
    }

    return mVencIni;
}

DispSet *Settings::getDispSetIni()
{
    if(mDispSetIni == nullptr){
        mDispSetIni = new DispSet();
    }

    return mDispSetIni;
}

SystemSet *Settings::getSystemSetIni()
{
    if(mSystemSetIni == nullptr){
        mSystemSetIni = new  SystemSet();
    }

    return mSystemSetIni;

}

AlarmSet *Settings::getAlarmSetIni()
{
    if(mAlarmSetIni == nullptr){
        mAlarmSetIni = new  AlarmSet();
    }

    return mAlarmSetIni;
}
