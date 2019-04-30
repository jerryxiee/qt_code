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
#ifdef arm
#include "common/sample_comm.h"
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

signals:
//    void Set_VoMode(SAMPLE_VO_MODE_E &enVoMode);

private slots:
    void onMainMenuSlot();
    void onExitVoSlot();
    void onOneWinShow(){ qDebug()<<"1";}
    void onTwoWinShow(){ qDebug()<<"1";}
    void onThrWinShow(){ qDebug()<<"1";}
    void onForWinShow(){ qDebug()<<"1";}
    void onFivWinShow();
    void onSixWinShow();
    void onSevWinShow();
    void onEigWinShow();
    void onNiWinShow();
    //void on_pushButton_clicked();

private:
    const int VI_CNT = 9;
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




};

#endif // WIDGET_H
