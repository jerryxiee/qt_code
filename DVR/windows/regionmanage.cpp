#include "regionmanage.h"
#include <QPainter>
#include <QDebug>

RegionManage::RegionManage(QWidget *parent) : QWidget(parent),mMove(false),mSetDispTime(false),mChange(false)
{
    this->resize(1280,720);



    mMenu = new QMenu(this);

    mSetTime = new QAction("时间位置",this);
    mSetTime->setCheckable(true);
    connect(mSetTime,SIGNAL(triggered()),this,SLOT(onSetTimeSlot()));
    mSetName = new QAction("名称位置",this);
    mSetName->setCheckable(true);
    connect(mSetName,SIGNAL(triggered()),this,SLOT(onSetNameSlot()));

    mActionGrp = new QActionGroup(this);
    mActionGrp->addAction(mSetName);
    mActionGrp->addAction(mSetTime);

    mExit = new QAction("返回",this);
    connect(mExit, SIGNAL(triggered()), this, SLOT(onExitSlot()));

    mDispSetIni = Settings::getDispSetIni();
    connect(mDispSetIni,SIGNAL(overlaySetPos(int)),this,SLOT(onOverlaySetPosSlot(int)));

    connect(this,SIGNAL(setRgnChange()),this,SLOT(onSetRgnChangedSlot()));


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

void RegionManage::mousePressEvent(QMouseEvent * event)
{
//    qDebug()<<"press"<<event->pos();
    if((mSetDispTime&&mTimeRect.contains(event->pos())) || (mSetDsipName&&mNameRect.contains(event->pos())))
    {
//        qDebug()<<"point in TimeRect:"<<mTimeRect;
        mMove = true;
        mStartPoint = event->pos();
    }
}

void RegionManage::mouseReleaseEvent(QMouseEvent * event)
{
    if(mMove){
        mMove = false;
            mTempPoint = mEndPoint;
    }



}


void RegionManage::mouseMoveEvent(QMouseEvent * event)
{
    qDebug()<<event->pos();
    if(mMove){
        mEndPoint = mTempPoint + event->pos() - mStartPoint;
        if(mSetDispTime){
            emit timePosChange(mChn,mEndPoint);
            mTimeRect.setRect(mEndPoint.x(),mEndPoint.y(),mTimeRect.width(),mTimeRect.height());
        }else if(mSetDsipName){
            emit namePosChange(mChn,mEndPoint);
            mNameRect.setRect(mEndPoint.x(),mEndPoint.y(),mNameRect.width(),mNameRect.height());
            qDebug()<<"Move mNameRect"<<mNameRect;
        }

        mChange = true;
    }
}

void RegionManage::onRegionSetSlot(int Chn,QString type)
{
    qDebug()<<"chn:"<<Chn<<"region type:"<<type;

    if(mChn != Chn || type != mSetType){
        mChange = false;
        emit setRgnChange();
    }

    mChn = Chn;
    mSetType = type;

    qDebug()<<"onRegionSetSlot";

    if(mChange == false){
        if(type == SET_REGION){

        }else {
    //        DispSet *dispSetIni = Settings::getDispSetIni();

            mTimeRect = mDispSetIni->getConfig(mDispSetIni->RootName+QString::number(Chn),mDispSetIni->TimePos).toRect();
            if(mTimeRect.isEmpty()){
                qDebug()<<"onRegionSetSlot mTimeRect isEmpty";
                mTimeRect.setRect(OVERLAYRGN_TIMEPOSX,OVERLAYRGN_TIMEPOSY,OVERLAYRGN_TIMEW,OVERLAYRGN_TIMEH);
            }
            qDebug()<<"TimeRect"<<mTimeRect;
            mNameRect = mDispSetIni->getConfig(mDispSetIni->RootName+QString::number(Chn),mDispSetIni->NamePos).toRect();
            if(mNameRect.isEmpty()){
                qDebug()<<"onRegionSetSlot mNameRect isEmpty";
                mNameRect.setRect(OVERLAYRGN_NAMEPOSX,OVERLAYRGN_NAMEPOSY,OVERLAYRGN_NAMEW,OVERLAYRGN_NAMEH);
            }
            qDebug()<<"mNameRect"<<mNameRect;
        }
    }

}

void RegionManage::onExitSlot()
{
    emit exitSignal();
}

void RegionManage::onSetNameSlot()
{
    mEndPoint.setX(mNameRect.x());
    mEndPoint.setY(mNameRect.y());
    mTempPoint = mEndPoint;
    mSetDsipName = true;
    mSetDispTime = false;
}

void RegionManage::onSetTimeSlot()
{
    mEndPoint.setX(mTimeRect.x());
    mEndPoint.setY(mTimeRect.y());
    mTempPoint = mEndPoint;
    mSetDsipName = false;
    mSetDispTime = true;

}

void RegionManage::onOverlaySetPosSlot(int Chn)
{
    qDebug()<<"onOverlaySetPosSlot";
    if(mChange){
        if(mSetType == SET_REGION){

        }else if(!mSetType.isEmpty() ){
            mDispSetIni->setConfig(mDispSetIni->RootName+QString::number(Chn),mDispSetIni->TimePos,mTimeRect);
            mDispSetIni->setConfig(mDispSetIni->RootName+QString::number(Chn),mDispSetIni->NamePos,mNameRect);
        }

        mChange = false;
    }



}

void RegionManage::onSetRgnChangedSlot()
{
    qDebug()<<"SetRgnChanged";
    mChange = false;
}

