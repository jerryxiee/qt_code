#include "test.h"
#include "QDebug"
#include <QDateTime>

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

void Test::setWinNum(int num)
{
    emit setWinNumSignal(num);
}


void Test::play(int Chn,QString starttime,QString endtime)
{
    qDebug()<<"chn:"<<Chn<<" stime:"<<starttime<<" endt:"<<endtime;

//    return;
    VideoFileList filelist;
    uint sttime = QDateTime::fromString(starttime, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint entime = QDateTime::fromString(endtime, "yyyy/MM/dd hh:mm:ss").toTime_t();


    filelist = mVideoSearch.readFileList(Chn,VIDEO_NORMAL);

    mVideoFileList = mVideoSearch.searchFile(filelist,sttime,entime);
    if(mVideoFileList.count() == 0){
        return;
    }

    emit videoPlayListSignal(mVideoFileList);

}
