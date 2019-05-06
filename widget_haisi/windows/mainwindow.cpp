#include "mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowOpacity(0.3);
    mQuickWidget = new QQuickWidget(this);//this基类为QWidget
    mQuickWidget->move(0,0);
    mQuickWidget->resize(1280,720);
    mQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
//    mQuickWidget->setHidden(true);
    QUrl source("qrc:/main.qml");
    mQuickWidget->setSource(source);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* e)
{

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{

}
