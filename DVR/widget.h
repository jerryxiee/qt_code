#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QQuickWidget>
#include <QStackedWidget>
#include <QPainter>
#include <QMenu>
#include <QPushButton>
#include "windows/CustomControls/animationbutton.h"
#include "windows/mainwindow.h"
#include "windows/videodisplay.h"
#include "windows/regionmanage.h"
#include "video/videocontrol.h"
#include "common/sample_comm.h"
#include "test/test.h"


namespace Ui {
class Widget;
}


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *event);

    static VideoDisplay  *getVideoDisplayWin();
    static Test *getTestOperate();


signals:
    void Set_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void StopVoSignal();
    void hidQmlSignal();
    void VideoDispSignal(QString);
    void regionSetSignal(int,QString);
    void dispChnSignal(int);

private:
    void InitWindows();
//    void VideoWinShow();

public slots:
    void onVideoDispSlot(QString filepath);
    void onVideoWinShowSlot();
    void onVideoTestWinShowSlot();

private slots:
    void onHidQmlSlot();
    void onMainMenuSlot();
    void onVideoExitClickSlot();
    void onShowQml();
    void onRegionSetSlot(int Chn,QString type);

private:
    Ui::Widget *ui;

    VideoControl mvideoControl;
    MainWindow *mMainWidow;
    static VideoDisplay  *mVideoDisplay;
    RegionManage *mRegionManage;
    videoplaytest *mVideoPlayTestWin;
    QStackedWidget *mWindows;
    QQuickWidget* mQuickWidget = nullptr;

//    Test mTest;
    static Test *mPTest;

};

#endif // WIDGET_H
