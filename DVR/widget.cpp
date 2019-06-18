#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QtQml>
#include <QtMath>
#include "settings/settings.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1280,720);

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
    }
    QQmlContext *context = mQuickWidget->rootContext();
    if(context){
        context->setContextProperty("VencControl",Settings::getVencIni());
    }


#ifndef LUNUX_WIN
    mvideoControl.videoStart();
//    QObject::connect(this,SIGNAL(ChnDispToWinSignal(QMap<VO_CHN, RECT_S> &)),&mvideoControl,SLOT(onDispChnToWin(QMap<VO_CHN, RECT_S> &)));
    QObject::connect(this,SIGNAL(Set_VoMode(SAMPLE_VO_MODE_E &)),&mvideoControl,SLOT(onSet_VoMode(SAMPLE_VO_MODE_E &)));
//    QObject::connect(this,SIGNAL(StopVoSignal()),&mvideoControl,SLOT(onStopVoSlot()));
#endif

    InitWindows();

}

Widget::~Widget()
{
    delete ui;

    qDebug("exit %s:%d",__FUNCTION__,__LINE__);

}

void Widget::InitWindows()
{
    mWindows = new QStackedWidget(this);

    mMainWidow = new MainWindow;
    connect(mMainWidow, SIGNAL(enterMainMenuSignal()), this, SLOT(onMainMenuSlot()));
    mWindows->addWidget(mMainWidow);

    mVideoDisplay = new VideoDisplay;
    connect(mVideoDisplay, SIGNAL(exitClicked()), this, SLOT(onVideoExitClickSlot()));

    mWindows->addWidget(mVideoDisplay);


#ifndef LUNUX_WIN
    connect(mMainWidow,SIGNAL(ChnDispToWinSignal(QMap<VO_CHN, RECT_S> &)),&mvideoControl,SLOT(onDispChnToWin(QMap<VO_CHN, RECT_S> &)));
    connect(mMainWidow,SIGNAL(StopVoSignal()),&mvideoControl,SLOT(onStopVoSlot()));
    connect(this,SIGNAL(VideoDispSignal(QString)),mVideoDisplay,SLOT(onVideoDispSignalSlot(QString)));

#endif
    connect(this,SIGNAL(hidQmlSignal()),mMainWidow,SLOT(onHidQmlSlot()));
    mWindows->resize(this->width(),this->height());
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

void Widget::onVideoDispSlot(QString filepath)
{

    mWindows->setCurrentWidget(mVideoDisplay);
    mWindows->show();
    mQuickWidget->setHidden(true);
    emit VideoDispSignal(filepath);

}

void Widget::onMainMenuSlot()
{
    mWindows->hide();
    mQuickWidget->show();

    emit StopVoSignal();
}


