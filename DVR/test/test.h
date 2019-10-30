#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "common/sample_comm.h"
#include "windows/videoplaytest.h"
#include "video/videosearch.h"
#include "video/mp4fileindex.h"

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);
    Q_INVOKABLE void setMoveDetect(int Chn, bool value);
    Q_INVOKABLE void setIOAlarm(int Chn,int io,bool value);
    Q_INVOKABLE void setWinNum(int num);
    Q_INVOKABLE void play(int Chn,QString starttime,QString endtime);
    Q_INVOKABLE void setRtspSize(int mode);
    Q_INVOKABLE void sentSeg(int mode);
    Q_INVOKABLE void createSession(int chn,bool isReal);
    Q_INVOKABLE void stopSession(int chn,bool isReal);
    Q_INVOKABLE void playSession(int chn,bool isReal);
    Q_INVOKABLE void deleteSession(int chn,bool isReal);
    Q_INVOKABLE void deleteRegion();
    Q_INVOKABLE void addRegion();

signals:
    void videoAlarmEventChangedSignal(VI_CHN Chn,VIDEO_TYPE type,bool change);
    void videoPlayListSignal(QList<MP4FileInfo> &);
    void setWinNumSignal(int);
    void setRtspSizeSignal(int);

public slots:

private:
//    VideoSearch mVideoSearch;
//    VideoFileList mVideoFileList;
};

#endif // TEST_H
