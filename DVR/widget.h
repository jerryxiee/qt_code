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


signals:
    void Set_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void StopVoSignal();
    void hidQmlSignal();
    void VideoDispSignal(QString);
    void regionSetSignal(int,QString);
    void dispChnSignal(int);

private:
    void InitWindows();

public slots:
    void onVideoDispSlot(QString filepath);

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
    VideoDisplay  *mVideoDisplay;
    RegionManage *mRegionManage;

};

#endif // WIDGET_H
