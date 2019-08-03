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

signals:
    void exitClicked();

public slots:
//    void onSetWinNum(int num);
    void onVideoExitClickSlot();
    void onVideoDispSignalSlot(QString path);
    void onVideoDispListSlot(VideoFileList & filelist);
    void onPositionChanged(uint value);
    void onShowControl(bool show);

private slots:
    void seek(int value);

private:
    QPushButton *mVideoExit;
#ifndef LUNUX_WIN
    Sample_Common_Vo mVideoVo;
    VideoPlay mVdec;
#endif
    int mWinNum;
    QSlider *slider;
    QLabel *labelDuration;
    PlayControls *controls;
    QWidget mControlWidget;
};

#endif // VIDEODISPLAY_H
