#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtQuickWidgets>
#include <QEvent>
#include <QMouseEvent>
#include <QProcess>
#include <QMenu>
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

protected:
//    void mousePressEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent* e);

signals:
    void Set_VoMode(SAMPLE_VO_MODE_E &enVoMode);

private slots:
    void firstChannelSlot(){ qDebug()<<"1";}
    void firstChannelSlot1(){ qDebug()<<"1";}
    void firstChannelSlot2(){ qDebug()<<"1";}
    void firstChannelSlot3(){ qDebug()<<"1";}
    void firstChannelSlot4(){ qDebug()<<"1";}
    void firstChannelSlot5(){ qDebug()<<"1";}
    //void on_pushButton_clicked();

private:
    QProcess *process;
    bool load_qml;
    QQuickWidget* m_quickWidget;
    QQuickView * m_quckView;
    Ui::Widget *ui;
    QQmlApplicationEngine engine;
    QMenu *mainmenu;
    QMenu *onemenu;
    QMenu *twomenu;
    QAction *firstChannel;
    QAction *firstChannel1;
    QAction *firstChannel2;
    QAction *firstChannel3;
    QAction *firstChannel4;
    QAction *firstChannel5;
    QActionGroup   *channels;
    QActionGroup   *sections;




};

#endif // WIDGET_H
