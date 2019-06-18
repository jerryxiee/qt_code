#include "videodisplay.h"
#include <QPainter>
#include <QDebug>

VideoDisplay::VideoDisplay(QWidget *parent) : QWidget(parent)
{
    this->resize(1280,720);
    mVideoExit = new QPushButton(tr("返回"),this);
    mVideoExit->setFixedSize(48,32);
    mVideoExit->move(this->width()-mVideoExit->width()*2,mVideoExit->height());
    connect(mVideoExit,SIGNAL(clicked()),this,SLOT(onVideoExitClickSlot()));

}

void VideoDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);// 创建QPainter一个对象

#ifndef LUNUX_WIN
//    if(mMainWin){
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        painter.fillRect( 0, 0, width(), height(), Qt::SolidPattern );
//    }
#endif

}


void VideoDisplay::onVideoExitClickSlot()
{

    emit exitClicked();
#ifndef LUNUX_WIN
    mVdec.Stop_Vdec();
    mVdec.wait();
#endif

}

void VideoDisplay::onVideoDispSignalSlot(QString path)
{
    QByteArray filename = path.toLatin1();
    qDebug()<<"onVideoDispSlot filename: "<<path;
#ifndef LUNUX_WIN
    mVdec.Start_Vdec(filename.data(),-1,-1);
#endif

}
