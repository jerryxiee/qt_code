#ifndef MP4FILETAB_H
#define MP4FILETAB_H

#include <QFileInfo>
#include "hi_common.h"
#include <QString>

class MP4FileTab
{
public:
    static MP4FileTab *createNewMP4FileTab(int Chn);
    static MP4FileTab *openMP4FileTab(int Chn);
//    static MP4FileTab *createNewAlarmMP4FileTab(int Chn);
//    static MP4FileTab *openAlarmMP4FileTab(int Chn);

    int getFileNum();
    int64_t getDuration();
    bool addIndexToFile(MP4PlayInfo &info);
    bool readFileHead(MP4FileIndexHead &head);
    bool writeFileHead(MP4FileIndexHead &head);
    void reset();
    void close();

    ~MP4FileTab();
protected:
    MP4FileTab(int Chn,bool isNew);

private:
    bool getAlarmFileName(int Chn,VIDEO_TYPE type,QString &filename);

private:
//    const QString ALARM_FILE_PATH = "/mnt/sda1/alarm/";
//    const QString IO_FILE = ".io_alarm";
//    const QString MOVED_FILE = ".move_alarm";
    const QString VENC_PATH = "/mnt/sda1/venc/";

    QFile mTimeTabFile;
    QFile mPlayInfoFile;
};

#endif // MP4FILETAB_H
