#include "regionmanage.h"
#include <QPainter>
#include <QDebug>

RegionManage::RegionManage(QWidget *parent) : QWidget(parent)
{
    this->resize(1280,720);

    mMenu = new QMenu(this);

    mSetTime = new QAction("时间位置",this);
    connect(mSetTime,SIGNAL(triggered()),this,SLOT(onSetTimeSlot()));
    mSetName = new QAction("名称位置",this);
    connect(mSetName,SIGNAL(triggered()),this,SLOT(onSetNameSlot()));
    mExit = new QAction("返回",this);
    connect(mExit, SIGNAL(triggered()), this, SLOT(onExitSlot()));

//    mDispSetIni = Settings::getDispSetIni();
//    mStartPoint = mDispSetIni->getConfig(mDispSetIni->)


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
    if(mSetType == SET_REGION){

    }else{

        mMenu->addAction(mSetName);
        mMenu->addAction(mSetTime);

    }

    mMenu->addAction(mExit);
    mMenu->exec(e->globalPos());
    qDebug()<<"mouse in region";

}

void RegionManage::onRegionSetSlot(int Chn,QString type)
{
    qDebug()<<"chn:"<<Chn<<"region type:"<<type;
    mChn = Chn;
    mSetType = type;
}

void RegionManage::onExitSlot()
{
    emit exitSignal();
}

void RegionManage::onSetNameSlot()
{
    mSetDsipName = true;
    mSetDispTime = false;
}

void RegionManage::onSetTimeSlot()
{
    mSetDsipName = false;
    mSetDispTime = true;

}
