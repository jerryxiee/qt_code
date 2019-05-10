#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QProcess>
#include <QMenu>
#include <QPainter>
#include <QList>
#include <QPushButton>
#include <QStackedWidget>
#include "windows/CustomControls/animationbutton.h"
//#include <QQuickWidget>
#include "windows/playvideo.h"
#include "windows/mainwindow.h"
//#ifndef LUNUX_WIN
#include "common/sample_comm.h"
#ifndef LUNUX_WIN
#include <video/vdec.h>
#endif

//#endif

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
    void InitWin();
public slots:
    void onVideoDispSlot(QString &filepath);

private slots:
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
//    void onNiWinShow();
    //void on_pushButton_clicked();

private:
    const int VI_CNT = 8;
    int mVo_Index;
    bool mMainWin;
    SAMPLE_VO_MODE_E mVoMode;
    QProcess *process;
    QPainter painter;
    bool mload_qml;
    QStackedWidget *mStackWidget;
    PlayVideo *mPlayVideo;
    MainWindow *mMainWindow;
    Ui::Widget *ui;
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
#ifndef LUNUX_WIN
    Vdec mVdec;
#endif




};

#endif // WIDGET_H
