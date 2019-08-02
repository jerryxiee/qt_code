#ifndef VIDEOSEARCH_H
#define VIDEOSEARCH_H

#include <QString>
#include <QFileInfoList>
#include "videofile.h"
#include "common/sample_comm.h"

class VideoSearch
{
public:
    VideoSearch();

    VideoFileList videofilelist(QFileInfoList &infolist);
    VideoFileList readFileList(int Chn, VIDEO_TYPE type);
    int getFrameIndex(VideoFile &videofile, uint time);
    uint getOffset(VideoFile &videofile,quint32 time);
    int search(VideoFileList &list, int startindex, int endindex, uint time);
    int normalSearch(VideoFileList &list, uint sttime, uint endtime, int flag);
    VideoFileList searchFile(VideoFileList &list,uint sttime,uint endtime);


private:
    VideoFileList readAlarmFileList(int Chn, VIDEO_TYPE type);
    VideoFileList readNormalFileList(int Chn);
    bool getAlarmFileName(int Chn, VIDEO_TYPE type, char *filename, int len);
private:
#ifndef LUNUX_WIN
    const QString RootPath = "/mnt/sda1/venc/channel";
#else
    const QString RootPath = "/home/abhw/venc/";
#endif
#ifndef LUNUX_WIN
    const char *ALARM_FILE_PATH = "/mnt/sda1/alarm";
#else
    const char *ALARM_FILE_PATH = "/home/abhw/nfsroot";
#endif
    const char *IO_FILE = ".io_alarm";
    const char *MOVED_FILE = ".move_alarm";
};

#endif // VIDEOSEARCH_H
