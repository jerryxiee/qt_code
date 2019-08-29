#ifndef MP4PLAY_H
#define MP4PLAY_H

#include <QThread>
#include <QMediaPlayer>
#include "hi_common.h"
#include <QList>
#include <QMutex>
#include "common/sample_common_vpss.h"
#include "common/sample_common_vdec.h"
extern "C"
{
#include "libavformat/avformat.h"
}

class MP4VideoPlay : public QThread
{
    Q_OBJECT
public:
    explicit MP4VideoPlay(QObject *parent = nullptr);
    MP4VideoPlay(const MP4VideoPlay& video);
    ~MP4VideoPlay();
    MP4VideoPlay& operator=(const MP4VideoPlay& video);

    int getVpssGrp() const;
    int getVpssChn() const;
    int setVpssToUser(SIZE_S  stSize);
    bool startPlay();
    void stopPlay();
protected:
    virtual void run();

private:
    int getFileIndex(qint64 position);
    int openFile(const char *name);
    void closeFile();
    int readFrame(AVPacket &pkt);
    int changeCurFile(int index);
    int searchIndex(QList<MP4FileInfo> &playlist,int stindex,int endindex,qint64 position);


signals:
    void durationChanged(qint64);
    void positionChanged(qint64);
    void stateChanged(QMediaPlayer::State);
    void endOfStream();

public slots:
    void setPlaylist(QList<MP4FileInfo> &playlist);
    void setPosition(qint64 position);
    void setPlaybackRate(qreal rate);
    void previousFile();
    void nextFile();
    void pause();
    void play();
    void stop();
    void onEndOfStream();


private:
    bool mRun;
    QList<MP4FileInfo> mPlayList;
    QMediaPlayer::State mMediaState;
    qint64 mDuration;
    qreal mRate;
    int mPlayPts;
    QMutex mFileMutex;
    int mCurFileIndex;
    qint64 mStartPosition;
    qint64 mCurPosition;
    Sample_Common_Vpss *mpVpss = nullptr;
    Sample_Common_Vdec *mpVdec = nullptr;
    AVFormatContext *mAVfmtCtx;
    int mVideoIndex;
};

#endif // MP4PLAY_H
