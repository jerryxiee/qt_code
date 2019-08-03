#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "common/sample_comm.h"
#include "windows/videoplaytest.h"
#include "video/videosearch.h"

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);
    Q_INVOKABLE void setMoveDetect(int Chn, bool value);
    Q_INVOKABLE void setIOAlarm(int Chn,int io,bool value);
    Q_INVOKABLE void setWinNum(int num);
    Q_INVOKABLE void play(int Chn,QString starttime,QString endtime);

signals:
    void videoAlarmEventChangedSignal(VI_CHN Chn,VIDEO_TYPE type,bool change);
    void videoPlayListSignal(VideoFileList &);
    void setWinNumSignal(int);

public slots:

private:
    VideoSearch mVideoSearch;
    VideoFileList mVideoFileList;
};

#endif // TEST_H
