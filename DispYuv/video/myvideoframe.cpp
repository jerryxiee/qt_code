#include "myvideoframe.h"
#include <QDebug>

MyvideoFrame::MyvideoFrame(QObject *parent) : QObject(parent)
{

    mwidth = 1280;
    mheigh = 720;
    mformat = QVideoFrame::Format_YUV420P;

    mtimer = new QTimer(this);
    connect(mtimer,SIGNAL(timeout()),this,SLOT(onTimeHander()));
    mtimer->start(30);
    pdata = new char[mwidth * mheigh *3/2];

    mfile = fopen("/home/abhw/nfsroot/test.yuv","r");
}

MyvideoFrame::~MyvideoFrame()
{
    delete [] pdata;
}

void MyvideoFrame::onTimeHander()
{
//    qDebug()<<"timer interrupt";
    if(mfile){
        if(!feof(mfile)){
            fread(pdata,mwidth * mheigh *3/2,1,mfile);
            QVideoFrame f(mwidth * mheigh *3/2, QSize(mwidth, mheigh), mwidth, QVideoFrame::Format_YUV420P);
            if (f.map(QAbstractVideoBuffer::WriteOnly)) {
                memcpy(f.bits(), pdata, mwidth * mheigh *3/2);
                f.setStartTime(0);
                f.unmap();
                qDebug()<<"emit newFrameAvailable";
                emit newFrameAvailable(f);
            }
        }else {
            qDebug()<<"time stop";
            mtimer->stop();
        }
    }

}
