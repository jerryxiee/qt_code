#include "regionmanage.h"
#include <QPainter>
#include <QDebug>

RegionManage::RegionManage(QWidget *parent) : QWidget(parent)
{
    this->resize(1280,720);

    mMenu = new QMenu(this);
    mExit = new QAction("返回",this);
    connect(mExit, SIGNAL(triggered()), this, SLOT(onExitSlot()));
}


void RegionManage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);// 创建QPainter一个对象

#ifndef LUNUX_WIN
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        painter.fillRect( 0, 0, width(), height(), Qt::SolidPattern );
#endif

}

void RegionManage::contextMenuEvent(QContextMenuEvent* e)
{
        mMenu->clear();

        mMenu->addAction(mExit);
        mMenu->exec(e->globalPos());
        qDebug()<<"mouse in region";

}

void RegionManage::onRegionSetSlot(int Chn,QString type)
{
    qDebug()<<"chn:"<<Chn<<"region type:"<<type;
}

void RegionManage::onExitSlot()
{
    emit exitSignal();
}
