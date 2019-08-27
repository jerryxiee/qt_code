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

private:
    int findLeftIndex(QList<MP4FileInfo> &filelist, int stindex, int endindex, uint time);
    int findRightIndex(QList<MP4FileInfo> &filelist, int stindex, int endindex, uint time);
protected:
    MP4FileIndex(QString filename);

private:
    QFile mFile;
};

#endif // MP4FILEINDEX_H
