#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QQuickWidget>
#include <QPainter>
#include <QMenu>
#include <QPushButton>
#include "windows/CustomControls/animationbutton.h"
#include "video/videocontrol.h"
//#ifndef LUNUX_WIN
#include "common/sample_comm.h"
#ifndef LUNUX_WIN
#include <video/videoplay.h>
#endif


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

protected:
//    void mousePressEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent* e);
    void mouseDoubleClickEvent(QMouseEvent *event);


signals:
    void Set_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void ChnDispToWinSignal(QMap<VO_CHN, RECT_S> &);
    void StopVoSignal();

private:
    void DispToWin(int Chn, int count);
    void InitMenuButtom();

public slots:
    void onVideoDispSlot(QString filepath);

private slots:
    void onHidQmlSlot();
    void onMainMenuSlot();
//    void onExitVoSlot();
    void onOneWinShowSlot();
    void onTwoWinShowSlot();
    void onThrWinShowSlot();
    void onForWinShowSlot();
    void onFivWinShowSlot();
    void onSixWinShowSlot();
    void onSevWinShowSlot();
    void onEigWinShowSlot();
    void on1MuxModeSlot();
    void on4MuxModeSlot();
    void on9MuxModeSlot();
    void onLeftButtonClickSlot();
    void onRightButtonClickSlot();
    void onVideoExitClickSlot();

private:
    Ui::Widget *ui;
    QQuickWidget* mQuickWidget = nullptr;
    const int VI_CNT = 8;
    int mVo_Index;
    bool mMainWin;
    SAMPLE_VO_MODE_E mVoMode;
    QPainter painter;
    bool mload_qml;
    QMenu *mMenu;
    QMenu *mOneMenu;
    QMenu *mTwoMenu;
    QAction *mBack;
    QAction *mExit_Vo;
    QList<QAction *> mOneMenuAct;
    QList<QAction *> mTwoMenuAct;
    QActionGroup   *mOneActionGrp;
    QActionGroup   *mTwoActionGrp;
    AnimationButton *mLeftButton;
    AnimationButton *mRightButton;
    QPushButton *mVideoExit;
    bool mVdecOn;
    VideoControl mvideoControl;
#ifndef LUNUX_WIN
    VideoPlay mVdec;
#endif

};

#endif // WIDGET_H
