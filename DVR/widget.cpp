#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QtQml>
#include <QtMath>
#include "settings/settings.h"
#include "windows/MyModel/mytablemodel.h"

VideoDisplay  *Widget::mVideoDisplay = nullptr;
Test * Widget::mPTest = new Test();

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1280,720);
    mPLogDb = LogTabData::getLogRecoedPoint();
    mPLogDb->writeOneData(INFO,SYSTEMSTART,"开机");

    mQuickWidget = new QQuickWidget(this);//this基类为QWidget
    mQuickWidget->move(0,0);
    mQuickWidget->resize(this->width(),this->height());
    mQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mQuickWidget->setHidden(true);
    QUrl source("qrc:/qml/main.qml");
    mQuickWidget->setSource(source);
    QObject *pRoot = (QObject*)mQuickWidget->rootObject();
    if(pRoot){
        connect(pRoot,SIGNAL(hidqmlsignal()),this,SLOT(onHidQmlSlot()));
        connect(pRoot,SIGNAL(videoDispSignal(QString)),this,SLOT(onVideoDispSlot(QString)));
        connect(pRoot,SIGNAL(videoDispList()),this,SLOT(onVideoWinShowSlot()));
        connect(pRoot,SIGNAL(videoTestSignal()),this,SLOT(onVideoTestWinShowSlot()));
        connect(pRoot,SIGNAL(regionSetSignal(int,QString)),this,SLOT(onRegionSetSlot(int,QString)));
    }
    QQmlContext *context = mQuickWidget->rootContext();
    if(context){
        context->setContextProperty("VencControl",Settings::getVencIni());
        context->setContextProperty("DispSet",Settings::getDispSetIni());
        context->setContextProperty("SystemConfig",Settings::getSystemSetIni());
        context->setContextProperty("AlarmConfig",Settings::getAlarmSetIni());
        context->setContextProperty("VideoTest",mPTest);
    }


#ifndef LUNUX_WIN
    mvideoControl.videoStart();
    connect(mPTest,SIGNAL(videoAlarmEventChangedSignal(VI_CHN,VIDEO_TYPE,bool)),&mvideoControl,SLOT(onTestVideoAlarmSlot(VI_CHN,VIDEO_TYPE,bool)));
//    connect(this,SIGNAL(Set_VoMode(SAMPLE_VO_MODE_E &)),&mvideoControl,SLOT(onSet_VoMode(SAMPLE_VO_MODE_E &)));
#endif

    InitWindows();

}

Widget::~Widget()
{
    delete ui;
//    delete mPTest;
    qDebug("exit %s:%d",__FUNCTION__,__LINE__);
    mPLogDb->writeOneData(INFO,SYSTEMEND,"关机");

}

void Widget::InitWindows()
{
    mWindows = new QStackedWidget(this);

    mMainWidow = new MainWindow;
    connect(mMainWidow, SIGNAL(enterMainMenuSignal()), this, SLOT(onMainMenuSlot()));
    mWindows->addWidget(mMainWidow);

    if(!mVideoDisplay){
        mVideoDisplay = new VideoDisplay;
    }
    connect(mVideoDisplay, SIGNAL(exitClicked()), this, SLOT(onShowQml()));
    mWindows->addWidget(mVideoDisplay);

    mRegionManage = new RegionManage;
    connect(this, SIGNAL(regionSetSignal(int,QString)), mRegionManage, SLOT(onRegionSetSlot(int,QString)));
    connect(mRegionManage, SIGNAL(exitSignal()), this, SLOT(onShowQml()));
    mWindows->addWidget(mRegionManage);

    mVideoPlayTestWin = new videoplaytest();
    connect(mVideoPlayTestWin, SIGNAL(exitClicked()), this, SLOT(onShowQml()));
    connect(mPTest,SIGNAL(setWinNumSignal(int)),mVideoPlayTestWin,SLOT(onSetWinNum(int)));
    connect(mPTest,SIGNAL(videoPlayListSignal(QList<MP4FileInfo> &)),mVideoPlayTestWin,SLOT(onVideoDispListSlot(QList<MP4FileInfo> &)));
    mWindows->addWidget(mVideoPlayTestWin);


#ifndef LUNUX_WIN
    connect(mMainWidow,SIGNAL(ChnDispToWinSignal(QMap<VO_CHN, RECT_S> &)),&mvideoControl,SLOT(onDispChnToWin(QMap<VO_CHN, RECT_S> &)));
    connect(this,SIGNAL(dispChnSignal(int)),mMainWidow,SLOT(onDispChnSlot(int)));
    connect(mMainWidow,SIGNAL(StopVoSignal()),&mvideoControl,SLOT(onStopVoSlot()));
//    connect(this,SIGNAL(VideoDispSignal(QString)),mVideoDisplay,SLOT(onVideoDispSignalSlot(QString)));
    connect(mRegionManage,SIGNAL(timePosChange(int,QPoint)),&mvideoControl,SLOT(onOverlayTimePosChanged(int,QPoint)));
    connect(mRegionManage,SIGNAL(namePosChange(int,QPoint)),&mvideoControl,SLOT(onOverlayNamePosChanged(int,QPoint)));


#endif
    connect(this,SIGNAL(hidQmlSignal()),mMainWidow,SLOT(onHidQmlSlot()));
    connect(this,SIGNAL(hidQmlSignal()),mRegionManage,SLOT(onHidQmlSlot()));
    mWindows->resize(this->width(),this->height());
}

VideoDisplay  *Widget::getVideoDisplayWin()
{
    if(!mVideoDisplay){
        mVideoDisplay = new VideoDisplay();
    }

    return mVideoDisplay;
}

Test *Widget::getTestOperate()
{
    if(!mPTest){
        mPTest = new Test();
    }

    return mPTest;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);// 创建QPainter一个对象
//    QPen pen;

#ifndef LUNUX_WIN
//    if(mMainWin){
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        painter.fillRect( 0, 0, 1280, 720, Qt::SolidPattern );
//    }
#endif

}


void Widget::onHidQmlSlot()
{
    mQuickWidget->setHidden(true);
    mWindows->setCurrentWidget(mMainWidow);
    mWindows->show();
    emit hidQmlSignal();
}



void Widget::onVideoExitClickSlot()
{

    mWindows->hide();
    mQuickWidget->show();
}
void Widget::onShowQml()
{
    mWindows->hide();
    mQuickWidget->show();
//    emit StopVoSignal();

}

void Widget::onVideoWinShowSlot()
{
    mWindows->setCurrentWidget(mVideoDisplay);
    mWindows->show();
    mQuickWidget->setHidden(true);
}

void Widget::onVideoTestWinShowSlot()
{
    mWindows->setCurrentWidget(mVideoPlayTestWin);
    mWindows->show();
    mQuickWidget->setHidden(true);
}

void Widget::onVideoDispSlot(QString filepath)
{
    onVideoWinShowSlot();
    emit VideoDispSignal(filepath);

}

void Widget::onRegionSetSlot(int Chn, QString type)
{
    mWindows->setCurrentWidget(mRegionManage);
    mWindows->show();
    mQuickWidget->setHidden(true);
    emit dispChnSignal(Chn);
    emit regionSetSignal(Chn,type);

}

void Widget::onMainMenuSlot()
{
    mWindows->hide();
    mQuickWidget->show();

//    emit StopVoSignal();
}


