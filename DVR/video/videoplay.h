#ifndef VDECPLAY_H
#define VDECPLAY_H

#include <QObject>
#include <QThread>
#include "common/sample_common_vo.h"
#include "common/sample_common_vdec.h"
#include <QFileInfoList>
#include <QMediaPlayer>
#include <QMutex>
#include <QtConcurrent/QtConcurrentRun>
#include "videofile.h"
#include "videoplaylist.h"

class VideoPlay : public QThread
{
    Q_OBJECT
public:
    explicit VideoPlay(QObject *parent = nullptr);
    VideoPlay(const VideoPlay& video);
    ~VideoPlay();
    VideoPlay& operator=(const VideoPlay& video);

    void Set_VdecAttr(VdecThreadParam &VdecAttr);
    HI_BOOL Start_Vdec();
    bool Start_Vdec(int ChnNum);
    void Stop_Vdec();
    int getVpssGrp() const;
    int getVpssChn() const;

    void setFileList(VideoFileList &fileList);
    void setCurrentposition(int percent);
    void videoStart();
    void setRate(qreal rate);

private:
    char* Get_FileType(char *filename);
    void setPosition(uint value);
    void endofStream();

protected:
    virtual void run();

signals:
    void currentPrecentChanged(uint);
    void stateChanged(QMediaPlayer::State);

public slots:
    void onRateChanged(qreal rate);
    void onFrameRateChanged(uint framrate);
    void onSetDuration(quint32 duration);
    void onPositionChanged(quint32 pos);
    void onFileFinished();
    void pause();
    void play();
    void stop();



public:


private:
    const HI_U64 NORMALPTS = 40000;

    Sample_Common_Vpss *m_pVpss = nullptr;
    Sample_Common_Vdec *m_pVdec = nullptr;
    bool m_Vdec_Run;
    PAYLOAD_TYPE_E m_enType;
    QMutex mFileMutex;
    int mPlayPts;
    qreal mRate;
    VideoPlayList mVideoPlayList;
    quint32 mDuration;
    QMediaPlayer::State mState;

};

#endif // VDECPLAY_H
