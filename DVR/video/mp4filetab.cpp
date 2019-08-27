#include "mp4filetab.h"


//bool MP4FileTab::getAlarmFileName(int Chn,VIDEO_TYPE type,QString &filename)
//{

//    switch (type) {
//        case VIDEO_MOVEDETECT:
//        {
//            filename = ALARM_FILE_PATH + MOVED_FILE + QString::number(Chn);
//            break;
//        }
//        case VIDEO_IO0:
//        case VIDEO_IO1:
//        case VIDEO_IO2:
//        case VIDEO_IO3:
//        {
//            filename = ALARM_FILE_PATH + IO_FILE + QString::number(type) + QString::number(Chn);
//            break;
//        }
//    default:{
//        return false;
//    }
//    }

//    return true;

//}

MP4FileTab *MP4FileTab::createNewMP4FileTab(int Chn)
{
    return new MP4FileTab(Chn, true);
}

MP4FileTab *MP4FileTab::openMP4FileTab(int Chn)
{
    return new MP4FileTab(Chn, false);
}

MP4FileTab::MP4FileTab(int Chn, bool isNew)
{
    const QString VENC_PATH = "/mnt/sda1/venc/channel";

    QString filename1 = "/mnt/sda1/venc/channel"+QString::number(Chn)+"/file.index";
    QString filename2 = "/mnt/sda1/venc/channel"+QString::number(Chn)+"/timetab.index";
}
