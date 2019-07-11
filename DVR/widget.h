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

private slots:
    void onHidQmlSlot();
    void onMainMenuSlot();
    void onVideoExitClickSlot();
    void onShowQml();
    void onRegionSetSlot(int Chn,QString type);

private:
    Ui::Widget *ui;
    QQuickWidget* mQuickWidget = nullptr;

    VideoControl mvideoControl;
    QStackedWidget *mWindows;
    MainWindow *mMainWidow;
    static VideoDisplay  *mVideoDisplay;
    RegionManage *mRegionManage;

    Test Alarmtest;

};

#endif // WIDGET_H
