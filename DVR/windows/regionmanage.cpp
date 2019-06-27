#include "regionmanage.h"
#include <QPainter>
#include <QDebug>


RegionManage::RegionManage(QWidget *parent) : QWidget(parent),mSetDsipName(false),mSetDispTime(false),mMove(false),mChange(false)
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

    mTimeLabel = new QLabel(this);
    mTimeLabel->setStyleSheet("background-color: red");
    mTimeLabel->hide();
    mNameLabel = new QLabel(this);
    mNameLabel->setStyleSheet("background-color: red");
    mNameLabel->hide();



}


void RegionManage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);// 创建QPainter一个对象

#ifndef LUNUX_WIN
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        painter.fillRect( 0, 0, width(), height(), Qt::SolidPattern );
#endif

//        if(mSetDispTime){
//            painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
//            painter.setPen(QColor(Qt::red));
//            painter.drawRect(mTimeRect);

//        }
//        if(mSetDsipName){
//            painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
//            painter.setPen(QColor(Qt::red));
//            painter.drawRect(mNameRect);
//        }

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
//            emit timePosChange(mChn,mEndPoint);
            mTimeLabel->move(mEndPoint);
            mTimeRect.setRect(mEndPoint.x(),mEndPoint.y(),mTimeRect.width(),mTimeRect.height());
        }else if(mSetDsipName){
//            emit namePosChange(mChn,mEndPoint);
            mNameLabel->move(mEndPoint);
            mNameRect.setRect(mEndPoint.x(),mEndPoint.y(),mNameRect.width(),mNameRect.height());
            qDebug()<<"Move mNameRect"<<mNameRect;
        }

        mChange = true;
        update();
    }
}

void RegionManage::onRegionSetSlot(int Chn,QString type)
{
    qDebug()<<"chn:"<<Chn<<"region type:"<<type;

    update();

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

//            mTimeRect = mDispSetIni->getConfig(mDispSetIni->RootName+QString::number(Chn),mDispSetIni->TimePos).toRect();
            mDispSetIni->getTimeRect(Chn,mTimeRect);
            if(mTimeRect.isEmpty()){
                qDebug()<<"onRegionSetSlot mTimeRect isEmpty";
                mTimeRect.setRect(OVERLAYRGN_TIMEPOSX,OVERLAYRGN_TIMEPOSY,OVERLAYRGN_TIMEW,OVERLAYRGN_TIMEH);
            }
            qDebug()<<"TimeRect"<<mTimeRect;
//            mNameRect = mDispSetIni->getConfig(mDispSetIni->RootName+QString::number(Chn),mDispSetIni->NamePos).toRect();
            mDispSetIni->getNameRect(Chn,mNameRect);
            if(mNameRect.isEmpty()){
                qDebug()<<"onRegionSetSlot mNameRect isEmpty";
                mNameRect.setRect(OVERLAYRGN_NAMEPOSX,OVERLAYRGN_NAMEPOSY,OVERLAYRGN_NAMEW,OVERLAYRGN_NAMEH);
            }

            mTimeLabel->resize(mTimeRect.width(),mTimeRect.height());
            mTimeLabel->move(mTimeRect.x(),mTimeRect.y());
            mNameLabel->resize(mNameRect.width(),mNameRect.height());
            mNameLabel->move(mNameRect.x(),mNameRect.y());
            qDebug()<<"mNameRect"<<mNameRect;
        }
    }

}

void RegionManage::onExitSlot()
{
    mSetDsipName = false;
    mSetDispTime = false;
    mNameLabel->hide();
    mTimeLabel->hide();
    emit exitSignal();
}

void RegionManage::onSetNameSlot()
{
    mEndPoint.setX(mNameRect.x());
    mEndPoint.setY(mNameRect.y());
    mTempPoint = mEndPoint;
    mSetDsipName = true;
    mSetDispTime = false;
    mNameLabel->show();
    mTimeLabel->hide();
    update();
}

void RegionManage::onSetTimeSlot()
{
    mEndPoint.setX(mTimeRect.x());
    mEndPoint.setY(mTimeRect.y());
    mTempPoint = mEndPoint;
    mSetDsipName = false;
    mSetDispTime = true;
    mTimeLabel->show();
    mNameLabel->hide();
    update();

}

void RegionManage::onOverlaySetPosSlot(int Chn)
{
    qDebug()<<"onOverlaySetPosSlot";
    if(mChange){
        if(mSetType == SET_REGION){

        }else if(!mSetType.isEmpty() ){
            mDispSetIni->setTimeRect(Chn,mTimeRect);
            mDispSetIni->setNameRect(Chn,mNameRect);
            emit timePosChange(mChn,QPoint(mTimeRect.x(),mTimeRect.y()));
            emit namePosChange(mChn,QPoint(mNameRect.x(),mNameRect.y()));
//            mDispSetIni->setConfig(mDispSetIni->RootName+QString::number(Chn),mDispSetIni->TimePos,mTimeRect);
//            mDispSetIni->setConfig(mDispSetIni->RootName+QString::number(Chn),mDispSetIni->NamePos,mNameRect);
        }

        mChange = false;
    }



}

void RegionManage::onSetRgnChangedSlot()
{
    qDebug()<<"SetRgnChanged";
    mChange = false;
}

void RegionManage::onHidQmlSlot()
{

}
