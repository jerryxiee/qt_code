#ifndef MP4FILE_H
#define MP4FILE_H

#include <QRect>
extern "C"
{
#include "libavformat/avformat.h"
}

class MP4File
{
public:
    MP4File();
    ~MP4File();

    bool createMP4File(char *filename,int framerate,QSize &size,int bitrate = 0,bool enAudio = false);  //创建MP4文件，默认无音频
    void closeMO4File();
    bool isOpen();
//    uint getFileSize();
    int64_t getCurPts();    //根据帧帧率及帧数计算结果,单位为s
    void writeFrame(unsigned char *buf,int len,bool isIDR);
    const char *getMP4FileName();
    void printinfo();
private:

    int addStream(enum AVCodecID codec_id);
    int addVideoStream(int framerate,QSize &size,int bitrate = 0);


private:
    AVFormatContext* mAVFmtCtx;
    int mVideoIndex;
    int mAudioIndex;
    int mPtsInc;
    bool mFirstIDRFrame;
//    AVFormatContext* mAVFmtCtxAudio;
};

#endif // MP4FILE_H
