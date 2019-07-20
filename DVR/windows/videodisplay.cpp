#include "videodisplay.h"
#include <QPainter>
#include <QDebug>
#include <QHBoxLayout>

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
    connect(&mVdec,SIGNAL(currentPrecentChanged(uint)),this,SLOT(onPositionChanged(uint)));
    connect(controls,SIGNAL(changeRate(qreal)),&mVdec,SLOT(onRateChanged(qreal)));
    connect(&mVdec,SIGNAL(stateChanged(QMediaPlayer::State)),controls,SLOT(setState(QMediaPlayer::State)));
    connect(controls,SIGNAL(play()),&mVdec,SLOT(play()));
    connect(controls,SIGNAL(pause()),&mVdec,SLOT(pause()));
    connect(controls,SIGNAL(stop()),&mVdec,SLOT(stop()));
#endif



}
VideoDisplay::~VideoDisplay()
{
#ifndef LUNUX_WIN
    mVdec.Stop_Vdec();
    mVdec.wait();
#endif
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


void VideoDisplay::onVideoExitClickSlot()
{

    emit exitClicked();
    controls->setPlaybackRate(1);
#ifndef LUNUX_WIN
    mVdec.Stop_Vdec();
    mVdec.wait();
#endif
    qDebug()<<"video clicked exit end";
}

void VideoDisplay::onVideoDispSignalSlot(QString path)
{
    QByteArray filename = path.toLatin1();
    qDebug()<<"onVideoDispSlot filename: "<<path;
#ifndef LUNUX_WIN
    QFileInfo fileinfo(path);
    QFileInfoList filelist;
    filelist.append(fileinfo);
    mVdec.setFileList(filelist);
    mVdec.Start_Vdec(filename.data(),-1,-1);
#endif
    onPositionChanged(0);

}

void VideoDisplay::onVideoDispListSlot(QFileInfoList & filelist)
{
#ifndef LUNUX_WIN
    mVdec.setFileList(filelist);
    mVdec.Start_Vdec("a.h264",-1,-1);
#endif
    onPositionChanged(0);
    qDebug()<<"onVideoDispListSlot num:"<<filelist.count();
}

void VideoDisplay::seek(int value)
{
#ifndef LUNUX_WIN
    mVdec.setCurrentposition(value);
#endif
}

void VideoDisplay::onPositionChanged(uint value)
{
    if (!slider->isSliderDown()) {
        slider->setValue(value);
    }

    QString tStr = QString::number(value)+"%";
    labelDuration->setText(tStr);
}
