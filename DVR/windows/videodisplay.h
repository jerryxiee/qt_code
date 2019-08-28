#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>
#include <QPushButton>
#include <QFileInfoList>
#include <QSlider>
#include <QLabel>
#include "playcontrols.h"
#include "video/videofile.h"
#ifndef LUNUX_WIN
#include <video/videoplay.h>
#include "common/sample_common_sys.h"
#include "video/mp4play.h"
//#include "video/mp4fileindex.h"
#endif

class VideoDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDisplay(QWidget *parent = nullptr);
    ~VideoDisplay();

//    int BindVideoPlay(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
//    int UnBindVideoPlay(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);

    void paintEvent(QPaintEvent *event);

private:
    void updateDurationInfo(qint64 currentInfo);

signals:
    void exitClicked();

public slots:
//    void onSetWinNum(int num);
    void onVideoExitClickSlot();
    void onVideoDispSignalSlot(QString path);
    void onVideoDispListSlot(VideoFileList & filelist);
    void onVideoDispListSlot(QList<MP4FileInfo> & filelist);
    void onPositionChanged(qint64 value);
    void onDurationChanged(qint64 value);
    void onShowControl(bool show);

private slots:
    void seek(int value);

private:
    QPushButton *mVideoExit;
#ifndef LUNUX_WIN
    Sample_Common_Vo mVideoVo;
    VideoPlay mVdec;
    MP4VideoPlay mMp4VideoPlay;
#endif
    qint64 duration;
    int mWinNum;
    QSlider *slider;
    QLabel *labelDuration;
    PlayControls *controls;
    QWidget mControlWidget;
};

#endif // VIDEODISPLAY_H
