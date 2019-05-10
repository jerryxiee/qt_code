#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{

    AnimationButton * but = new AnimationButton(this);
//    connect(but,SIGNAL(clicked()),this,SLOT(onClickedSlot()));
    but->resize(100,100);
    but->setText("but");
    but->setDuration(200);
    but->setValue(0,20);
    but->setOpacity(0.5);
//    but->setWindowOpacity(0.5);
    but->setImage(":/images/back.png");
}
