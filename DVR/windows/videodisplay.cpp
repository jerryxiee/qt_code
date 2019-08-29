#include "videodisplay.h"
#include <QPainter>
#include <QDebug>
#include <QHBoxLayout>
#include <QDateTime>

VideoDisplay::VideoDisplay(QWidget *parent) : QWidget(parent)
{
    this->resize(1280,720);
    mVideoExit = new QPushButton(tr("返回"),this);
    mVideoExit->setFixedSize(48,32);
    mVideoExit->move(this->width()-mVideoExit->width()*2,mVideoExit->height());
    connect(mVideoExit,SIGNAL(clicked()),this,SLOT(onVideoExitClickSlot()));


    controls = new PlayControls(this);


    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);

    labelDuration = new QLabel(this);
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

    QBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(slider);
    hLayout->addWidget(labelDuration);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addStretch();
    controlLayout->addWidget(controls);
    controlLayout->addStretch();
    controlLayout->setStretch(0,3);
    controlLayout->setStretch(1,2);
    controlLayout->setStretch(2,3);

    layout->addStretch();
    layout->addLayout(hLayout);
    layout->addLayout(controlLayout);
    layout->setStretch(0,16);
    layout->setStretch(1,1);
    layout->setStretch(2,1);

    setLayout(layout);

#ifndef LUNUX_WIN
    VO_DEV VoDev;
    VO_LAYER VoLayer;
    Sample_Common_Sys::Get_Sys_VoDev(VoDev,VoLayer);
    mVideoVo.SAMPLE_COMM_VO_SetDev(VoDev,VoLayer);
    connect(&mMp4VideoPlay,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
    connect(&mMp4VideoPlay,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));
    connect(controls,SIGNAL(changeRate(qreal)),&mMp4VideoPlay,SLOT(setPlaybackRate(qreal)));
    connect(&mMp4VideoPlay,SIGNAL(stateChanged(QMediaPlayer::State)),controls,SLOT(setState(QMediaPlayer::State)));
    connect(controls,SIGNAL(play()),&mMp4VideoPlay,SLOT(play()));
    connect(controls,SIGNAL(pause()),&mMp4VideoPlay,SLOT(pause()));
    connect(controls,SIGNAL(stop()),&mMp4VideoPlay,SLOT(stop()));
#endif



}
VideoDisplay::~VideoDisplay()
{
    qDebug()<<"enter ~VideoDisplay";
#ifndef LUNUX_WIN
//    mVideoVo.SAMPLE_COMM_VO_UnBindVpss(0,mMp4VideoPlay.getVpssGrp(),mMp4VideoPlay.getVpssChn());
//    mMp4VideoPlay.stopPlay();
//    mVideoVo.SAMPLE_COMM_VO_StopChn();
#endif
    qDebug()<<"exit videoplay";
}

void VideoDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);// 创建QPainter一个对象

#ifndef LUNUX_WIN
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        painter.fillRect( 0, 0, width(), height(), Qt::SolidPattern );
#endif

}

//int VideoDisplay::BindVideoPlay(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn)
//{
//    return mVideoVo.SAMPLE_COMM_VO_BindVpss(VoChn,VpssGrp,VpssChn);
//}

//int VideoDisplay::UnBindVideoPlay(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn)
//{
//    return mVideoVo.SAMPLE_COMM_VO_UnBindVpss(VoChn,VpssGrp,VpssChn);
//}

void VideoDisplay::onShowControl(bool show)
{
    if(show){
        if(slider){
            slider->show();
        }
        if(labelDuration){
            labelDuration->show();
        }
        if(controls){
            controls->show();
        }
    }else {
        if(slider){
            slider->hide();
        }
        if(labelDuration){
            labelDuration->hide();
        }
        if(controls){
            controls->hide();
        }
    }

}

//void VideoDisplay::onSetWinNum(int num)
//{
//    qDebug()<<"start vo chn:"<<num;
//    if(num > 4){
//        mVideoVo.SAMPLE_COMM_VO_StartChn(VO_MODE_9MUX);
//    }else if(num > 1){
//        mVideoVo.SAMPLE_COMM_VO_StartChn(VO_MODE_4MUX);
//    }else {
//        mVideoVo.SAMPLE_COMM_VO_StartChn(VO_MODE_1MUX);
//    }
//}

void VideoDisplay::onVideoExitClickSlot()
{

    emit exitClicked();
    controls->setPlaybackRate(1);
#ifndef LUNUX_WIN

    mVideoVo.SAMPLE_COMM_VO_UnBindVpss(0,mMp4VideoPlay.getVpssGrp(),mMp4VideoPlay.getVpssChn());
    mMp4VideoPlay.stopPlay();
    mVideoVo.SAMPLE_COMM_VO_StopChn();
#endif
    qDebug()<<"video clicked exit end";
}

void VideoDisplay::onVideoDispSignalSlot(QString path)
{
//    QByteArray filename = path.toLatin1();
//    qDebug()<<"onVideoDispSlot filename: "<<path;
//#ifndef LUNUX_WIN
//    QFileInfo fileinfo(path);
//    QFileInfoList filelist;
//    filelist.append(fileinfo);
//    mVdec.setFileList(filelist);
//    mVdec.Start_Vdec("a.h264",-1,-1);
//#endif
//    onPositionChanged(0);

}

void VideoDisplay::onVideoDispListSlot(VideoFileList &filelist)
{
#ifndef LUNUX_WIN


#endif
    onPositionChanged(0);
    qDebug()<<"onVideoDispListSlot num:"<<filelist.count();
}

void VideoDisplay::onVideoDispListSlot(QList<MP4FileInfo> & filelist)
{
#ifndef LUNUX_WIN
    mMp4VideoPlay.setPlaylist(filelist);
    mMp4VideoPlay.startPlay();
    mVideoVo.SAMPLE_COMM_VO_StartChn(VO_MODE_1MUX);
    mVideoVo.SAMPLE_COMM_VO_BindVpss(0,mMp4VideoPlay.getVpssGrp(),mMp4VideoPlay.getVpssChn());

#endif

}

void VideoDisplay::seek(int value)
{
#ifndef LUNUX_WIN
    mMp4VideoPlay.setPosition(value);
#endif
}

void VideoDisplay::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    labelDuration->setText(tStr);
}


void VideoDisplay::onPositionChanged(qint64 value)
{
    if (!slider->isSliderDown()) {
        slider->setValue(value);
    }

    updateDurationInfo(value);
}

void VideoDisplay::onDurationChanged(qint64 value)
{
    this->duration = value/1000;
    slider->setMaximum(this->duration);
}

