#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtQuickWidgets>
#include <QEvent>
#include <QMouseEvent>
#include <QProcess>
#include <QMenu>
#include <QPainter>
#include <QList>
#include <QPushButton>
//#ifdef arm
#include "common/sample_comm.h"
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

private:
    void DispToWin(int Chn, int count);

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
//    void onNiWinShow();
    //void on_pushButton_clicked();

private:
    const int VI_CNT = 8;
    int mVo_Index;
    SAMPLE_VO_MODE_E mVoMode;
    QProcess *process;
    QPainter painter;
    bool mload_qml;
    QQuickWidget* m_quickWidget;
    QQuickView * m_quckView;
    Ui::Widget *ui;
    QQmlApplicationEngine engine;
    QMenu *mMenu;
    QMenu *mOneMenu;
    QMenu *mTwoMenu;
    QAction *mBack;
    QAction *mExit_Vo;
    QList<QAction *> mOneMenuAct;
    QList<QAction *> mTwoMenuAct;
    QActionGroup   *mOneActionGrp;
    QActionGroup   *mTwoActionGrp;
    QPushButton *mLeftButton;
    QPushButton *mRightButton;




};

#endif // WIDGET_H
