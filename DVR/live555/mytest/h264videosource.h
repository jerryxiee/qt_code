#ifndef H264VIDEOSOURCE_H
#define H264VIDEOSOURCE_H

#include "ByteStreamFileSource.hh"
#include "InputFile.hh"
#include "GroupsockHelper.hh"
#include "video/videostreampro.h"

#define CACHELEN 102400

class H264VideoSource:public FramedSource
{
public:
    static H264VideoSource *createNew(UsageEnvironment& env,int Chn);

protected:
    H264VideoSource(UsageEnvironment& env,int Chn);
    ~H264VideoSource();

private:
  // redefined virtual functions:
  virtual void doGetNextFrame();
  virtual void doStopGettingFrames();

private:
    int mFd;
    int mVencChn;
    fd_set mRead_fds;
    VENC_CHN_STAT_S mStStat;
    VENC_STREAM_S mStStream;
    bool mHaveStartedReading;
    int mChn;
    VideoStreamPro mVideoStreamPro;
    uint mCurFrameSize;
    char *mCache;
    int mCacheLen;
    int mPos;
};

#endif // H264VIDEOSOURCE_H
