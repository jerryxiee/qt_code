#ifndef MP4FILEINDEX_H
#define MP4FILEINDEX_H

#include <QFileInfo>
#include "hi_common.h"
#include <QString>

class MP4FileIndex
{
public:
    static MP4FileIndex *createNewFileIndex(int Chn);
    static MP4FileIndex *openFileIndex(int Chn);
//    static MP4FileIndex *createNewAlarmFileIndex(int Chn,VIDEO_TYPE type);
    static MP4FileIndex *openFileIndex(int Chn,VIDEO_TYPE type);
    MP4FileIndex();
    ~MP4FileIndex();

    int getFileNum();
    int64_t getDuration();
    bool addIndexToFile(MP4FileInfo &info);
    bool getFileInfo(MP4FileInfo &info,int index);
    bool readFileHead(MP4FileIndexHead &head);
    bool writeFileHead(MP4FileIndexHead &head);
    int getFileList(QList<MP4FileInfo> &filelist);
    int getFileList(QList<MP4FileInfo> &filelist,uint sttime,uint endtime);
    int searchLeft(QList<MP4FileInfo> &filelist, int stindex, int endindex, uint time);
    int searchRight(QList<MP4FileInfo> &filelist, int stindex, int endindex, uint time);

    void reset();
    void close();

    static int getFileNum(int Chn,VIDEO_TYPE type);
    static int64_t getDuration(int Chn,VIDEO_TYPE type);
    static bool addIndexToFile(MP4FileInfo &info,int Chn,VIDEO_TYPE type);
    static bool readFileHead(MP4FileIndexHead &head,int Chn,VIDEO_TYPE type);
    static bool writeFileHead(MP4FileIndexHead &head,int Chn,VIDEO_TYPE type);
    static int getFileList(QList<MP4FileInfo> &filelist,int Chn,VIDEO_TYPE type);
    static int getFileList(QList<MP4FileInfo> &filelist,uint sttime,uint endtime,int Chn,VIDEO_TYPE type);
    static bool getFileName(int Chn,VIDEO_TYPE type,QString &filename);

private:
    int findLeftIndex(QList<MP4FileInfo> &filelist, int stindex, int endindex, uint time);
    int findRightIndex(QList<MP4FileInfo> &filelist, int stindex, int endindex, uint time);
protected:
    MP4FileIndex(QString filename);

private:
    static QString ALARM_FILE_PATH ;
    static QString IO_FILE ;
    static QString MOVED_FILE ;
    static QString VENC_PATH;

    QFile mFile;
};

#endif // MP4FILEINDEX_H
