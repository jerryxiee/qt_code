#include "testwidget.h"
#include <QLabel>
#include <QToolButton>
#include <QToolBar>
#include <QVBoxLayout>
#include "../mainwindow.h"

TestWidget::TestWidget(QWidget *parent) : QWidget(parent)
{

    this->resize(1280,720);
    QFont font;
    font.setPointSize(16);
    QLabel *label = new QLabel(this);
    label->setText("测试");
    label->setFont(font);
    label->setContentsMargins(20,0,0,0);
    label->setFixedSize(this->width(),64);
//    label->resize(QSize(this->width(),64));
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(50, 100,100));
    label->setAutoFillBackground(true);
    label->setPalette(palette);
    label->setStyleSheet("color:white");

//    MainWindow *mainwin = new MainWindow();
//    QVBoxLayout *vboxlayout = new QVBoxLayout(this);
//    vboxlayout->addWidget(label);
//    vboxlayout->addWidget(mainwin);


//    QToolButton *but = new QToolButton;
//    QToolBar *toolbar = new QToolBar(this);
//    toolbar->addWidget(but);


}
