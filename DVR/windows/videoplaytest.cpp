#include "videoplaytest.h"
#include <QPainter>
#include <QDebug>

videoplaytest::videoplaytest(QWidget *parent) : QWidget(parent)
{
    this->resize(1280,720);
    mVideoExit = new QPushButton(tr("返回"),this);
    mVideoExit->setFixedSize(48,32);
    mVideoExit->move(this->width()-mVideoExit->width()*2,mVideoExit->height());
    connect(mVideoExit,SIGNAL(clicked()),this,SLOT(onVideoExitClickSlot()));

    VO_DEV VoDev;
    VO_LAYER VoLayer;
    Sample_Common_Sys::Get_Sys_VoDev(VoDev,VoLayer);
    mVideoVo.SAMPLE_COMM_VO_SetDev(VoDev,VoLayer);

}

videoplaytest::~videoplaytest()
{
#ifndef LUNUX_WIN
    for (int i = 0;i < mVideoPlayList.count();i++) {
        mVideoVo.SAMPLE_COMM_VO_UnBindVpss(i,mVideoPlayList.at(i).getVpssGrp(),mVideoPlayList.at(i).getVpssChn());
        mVideoPlayList[i].Stop_Vdec();
    }

    mVideoVo.SAMPLE_COMM_VO_StopChn();
#endif
    qDebug()<<"exit videoplaytest";

}

void videoplaytest::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);// 创建QPainter一个对象

#ifndef LUNUX_WIN
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        painter.fillRect( 0, 0, width(), height(), Qt::SolidPattern );
#endif

}

void videoplaytest::onSetWinNum(int num)
{
    qDebug()<<"start vo chn:"<<num;
#ifndef LUNUX_WIN
    mVideoPlayList.clear();
    if(num > 4){
        mVideoVo.SAMPLE_COMM_VO_StartChn(VO_MODE_9MUX);
    }else if(num > 1){
        mVideoVo.SAMPLE_COMM_VO_StartChn(VO_MODE_4MUX);
    }else {
        mVideoVo.SAMPLE_COMM_VO_StartChn(VO_MODE_1MUX);
    }
#endif

}

void videoplaytest::onVideoExitClickSlot()
{

    emit exitClicked();
#ifndef LUNUX_WIN
    for (int i = 0;i < mVideoPlayList.count();i++) {
        mVideoVo.SAMPLE_COMM_VO_UnBindVpss(i,mVideoPlayList.at(i).getVpssGrp(),mVideoPlayList.at(i).getVpssChn());
        mVideoPlayList[i].Stop_Vdec();
    }
    mVideoPlayList.clear();
    mVideoVo.SAMPLE_COMM_VO_StopChn();
#endif
    qDebug()<<"video clicked exit end";
}

void videoplaytest::onVideoDispListSlot(VideoFileList &filelist)
{
#ifndef LUNUX_WIN
    mVideoPlayList.append(VideoPlay());

    mVideoPlayList.last().setFileList(filelist);
    mVideoPlayList.last().Start_Vdec();

    mVideoVo.SAMPLE_COMM_VO_BindVpss(mVideoPlayList.count()-1,mVideoPlayList.last().getVpssGrp(),mVideoPlayList.last().getVpssChn());

#endif
    qDebug()<<"onVideoDispListSlot num:"<<filelist.count();
}


