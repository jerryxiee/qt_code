#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMenu>
#include "CustomControls/animationbutton.h"
#include "../common/sample_comm.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

protected:
//    void mousePressEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent* e);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    void InitMenuButtom();
    void DispToWin(int Chn, int count);

signals:
    void ChnDispToWinSignal(QMap<VO_CHN, RECT_S> &);
    void enterMainMenuSignal();
    void StopVoSignal();
//    void hidQmlSignal();

public slots:
    void onDispChnSlot(int Chn);

private slots:
    void onHidQmlSlot();
    void onMainMenuSlot();
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


private:
    const int VI_CNT = 8;
    int mVo_Index;
    SAMPLE_VO_MODE_E mVoMode;
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
};

#endif // MAINWINDOW_H
