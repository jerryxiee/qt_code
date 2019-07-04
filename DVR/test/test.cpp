#include "test.h"
#include "QDebug"

Test::Test(QObject *parent) : QObject(parent)
{

}

void Test::setMoveDetect(int Chn,bool value)
{
    qDebug()<<"Test Move Detect Chn:"<<Chn<<"value:"<<value;
    emit videoAlarmEventChangedSignal(Chn,VIDEO_MOVEDETECT,value);

}

void Test::setIOAlarm(int Chn,int io,bool value)
{
    VIDEO_TYPE type;

    switch (io) {
    case 0:
        type = VIDEO_IO0;
        break;
    case 1:
        type = VIDEO_IO1;
        break;
    case 2:
        type = VIDEO_IO2;
        break;
    case 3:
        type = VIDEO_IO3;
        break;
    default:
        return;

    }
    qDebug()<<"Test IO Alarm Chn:"<<Chn<<"io:"<<io<<"value:"<<value;

    emit videoAlarmEventChangedSignal(Chn,type,value);

}
