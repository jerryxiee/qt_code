#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QtMath>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),mVdecOn(false)
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
    }

    InitMenuButtom();

}

Widget::~Widget()
{
    delete ui;
#ifndef LUNUX_WIN
    mVdec.wait();
#endif

    qDebug("exit %s:%d",__FUNCTION__,__LINE__);

}

void Widget::InitMenuButtom()
{
    mVideoExit = new QPushButton(tr("返回"),this);
    mVideoExit->setFixedSize(48,32);
    mVideoExit->move(this->width()-mVideoExit->width()*2,mVideoExit->height());
    connect(mVideoExit,SIGNAL(clicked()),this,SLOT(onVideoExitClickSlot()));
    mVideoExit->hide();
//    mVideoExit->setFlat(true);


    mMenu = new QMenu(this);
    mBack = new QAction("主菜单",this);
//    mBack->setCheckable(true);
    connect(mBack, SIGNAL(triggered()), this, SLOT(onMainMenuSlot()));
    mExit_Vo = new QAction("返回",this);
//    mExit_Vo->setCheckable(true);
    connect(mExit_Vo, SIGNAL(triggered()), this, SLOT(on9MuxModeSlot()));

    mOneMenu = new QMenu("单窗口",this);
    mTwoMenu = new QMenu("多窗口",this);
    mOneActionGrp = new QActionGroup(this);
//    mTwoActionGrp = new QActionGroup(this);
    for(int i = 0;i < VI_CNT; i++){
        mOneMenuAct.append(new QAction(QString::number(i+1)+"Chn",this));
        mOneMenuAct[i]->setCheckable(true);
        mOneActionGrp->addAction(mOneMenuAct[i]);
        mOneMenu->addAction(mOneMenuAct[i]);
    }
    for(int i = 0; i < qSqrt(VI_CNT);i++){
        mTwoMenuAct.append(new QAction(QString::number((i+1)*(i+1))+"Win(s)",this));
        mTwoMenuAct[i]->setCheckable(true);
        mOneActionGrp->addAction(mTwoMenuAct[i]);
        mTwoMenu->addAction(mTwoMenuAct[i]);
    }

    connect(mOneMenuAct[0],SIGNAL(triggered()),this,SLOT(onOneWinShowSlot()));
    connect(mOneMenuAct[1],SIGNAL(triggered()),this,SLOT(onTwoWinShowSlot()));
    connect(mOneMenuAct[2],SIGNAL(triggered()),this,SLOT(onThrWinShowSlot()));
    connect(mOneMenuAct[3],SIGNAL(triggered()),this,SLOT(onForWinShowSlot()));
    connect(mOneMenuAct[4],SIGNAL(triggered()),this,SLOT(onFivWinShowSlot()));
    connect(mOneMenuAct[5],SIGNAL(triggered()),this,SLOT(onSixWinShowSlot()));
    connect(mOneMenuAct[6],SIGNAL(triggered()),this,SLOT(onSevWinShowSlot()));
    connect(mOneMenuAct[7],SIGNAL(triggered()),this,SLOT(onEigWinShowSlot()));

    connect(mTwoMenuAct[0],SIGNAL(triggered()),this,SLOT(on1MuxModeSlot()));
    connect(mTwoMenuAct[1],SIGNAL(triggered()),this,SLOT(on4MuxModeSlot()));
    connect(mTwoMenuAct[2],SIGNAL(triggered()),this,SLOT(on9MuxModeSlot()));


    mLeftButton = new AnimationButton(this);
//    connect(but,SIGNAL(clicked()),this,SLOT(onClickedSlot()));
    mLeftButton->resize(100,100);
    mLeftButton->setText("but");
    mLeftButton->setDuration(200);
    mLeftButton->setValue(0,20);
    mLeftButton->setOpacity(0.3);
//    but->setWindowOpacity(0.5);
    mLeftButton->setImage(":/images/back.png");
    mLeftButton->setImageSize(90,90);
    mLeftButton->hide();
    mLeftButton->setGeometry(0,(this->height() - mLeftButton->height())/2,mLeftButton->width(),mLeftButton->height());


    mRightButton = new AnimationButton(this);
//    connect(but,SIGNAL(clicked()),this,SLOT(onClickedSlot()));
    mRightButton->move(100,100);
    mRightButton->resize(100,100);
    mRightButton->setText("but");
    mRightButton->setDuration(200);
    mRightButton->setValue(0,20);
    mRightButton->setOpacity(0.3);
//    but->setWindowOpacity(0.5);
    mRightButton->setImage(":/images/back1.png");
    mRightButton->setImageSize(90,90);
    mRightButton->setGeometry(this->width() - mRightButton->width(),(this->height() - mRightButton->height())/2,mRightButton->width(),mRightButton->height());
    mRightButton->setHidden(true);
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);// 创建QPainter一个对象
//    QPen pen;
    QFont font;

#ifndef LUNUX_WIN
    if(mMainWin){
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        painter.fillRect( 0, 0, 1280, 720, Qt::SolidPattern );
    }
#endif

}

void Widget::contextMenuEvent(QContextMenuEvent* e)
{

    if(!mVdecOn){
        mMenu->clear();

        mMenu->addAction(mBack);
        mMenu->addMenu(mOneMenu);
        mMenu->addMenu(mTwoMenu);
        mMenu->addAction(mExit_Vo);

        mMenu->exec(e->globalPos());
    }

}

void Widget::onHidQmlSlot()
{
    mQuickWidget->setHidden(true);
    on9MuxModeSlot();
}


void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void Widget::onVideoExitClickSlot()
{
    mMainWin = false;
    mVdecOn = false;
    mVideoExit->hide();
#ifndef LUNUX_WIN
    mVdec.Stop_Vdec();
#endif
}

void Widget::onVideoDispSlot(QString &filepath)
{
    QByteArray filename = filepath.toLatin1();

    qDebug()<<"onVideoDispSlot filename: "<<filepath;
    mMainWin = true;
    mVdecOn = true;
    mRightButton->setHidden(true);
    mLeftButton->setHidden(true);
    mVideoExit->show();
#ifndef LUNUX_WIN
    mVdec.Start_Vdec(filename.data());
#endif
}

void Widget::onMainMenuSlot()
{
    mMainWin = false;
    mRightButton->setHidden(true);
    mLeftButton->setHidden(true);

    mQuickWidget->show();
    emit StopVoSignal();
//    if(!mload_qml){
//        mload_qml = true;
//        m_quickWidget->setHidden(false);
//    }
}

//void Widget::onExitVoSlot()
//{
//    SAMPLE_VO_MODE_E vo_mode = VO_MODE_9MUX;
//    emit Set_VoMode(vo_mode);
////    if(mload_qml){
////        mload_qml = false;
////        m_quickWidget->setHidden(true);
////    }
//}

void Widget::on1MuxModeSlot()
{
    mVo_Index = 0;
     mVoMode = VO_MODE_1MUX;
    DispToWin(mVo_Index,1);
}

void Widget::on4MuxModeSlot()
{
    mVo_Index = 0;
    mVoMode = VO_MODE_4MUX;
    DispToWin(mVo_Index,4);
}

void Widget::on9MuxModeSlot()
{
    mMainWin = true;
    mVo_Index = 0;
    mVoMode = VO_MODE_9MUX;
    DispToWin(mVo_Index,8);
}

void Widget::DispToWin(int StartChn,int count)
{
    QMap<VO_CHN, RECT_S> ChnPos;
    RECT_S pos;
    int index;
    int s32Square = 0;

    switch (mVoMode) {
        case VO_MODE_1MUX:
            s32Square = 1;
            break;
        case VO_MODE_4MUX:
            s32Square = 2;
            break;
        case VO_MODE_9MUX:
            s32Square = 3;
            break;
    }
    mVo_Index = StartChn%VI_CNT;
    for(int i = 0;i < count;i++){

        pos.s32X       = (this->width()/s32Square) * (i%s32Square);
        pos.s32Y       = (this->height()/s32Square) * (i/s32Square);
        pos.u32Height  = this->height()/s32Square ;
        pos.u32Width   = this->width()/s32Square;
        index = mVo_Index+i;
        ChnPos.insert(index%VI_CNT,pos);
        qDebug()<<"display channel:"<<StartChn<<"height"<<pos.u32Height<<"width"<<pos.u32Width;
    }

    if(s32Square < 3){
        mRightButton->show();
        mLeftButton->show();
    }else{
        mRightButton->hide();
        mLeftButton->hide();
    }
    emit ChnDispToWinSignal(ChnPos);
}

void Widget::onOneWinShowSlot()
{
    mVoMode = VO_MODE_1MUX;
    DispToWin(0,1);
}

void Widget::onTwoWinShowSlot()
{
    mVoMode = VO_MODE_1MUX;
    DispToWin(1,1);
}

void Widget::onThrWinShowSlot()
{
    mVoMode = VO_MODE_1MUX;
    DispToWin(2,1);

}
void Widget::onForWinShowSlot()
{
    mVoMode = VO_MODE_1MUX;
    DispToWin(3,1);
}

void Widget::onFivWinShowSlot()
{
    mVoMode = VO_MODE_1MUX;
    DispToWin(4,1);
}

void Widget::onSixWinShowSlot()
{
    mVoMode = VO_MODE_1MUX;
    DispToWin(5,1);
}
void Widget::onSevWinShowSlot()
{
    mVoMode = VO_MODE_1MUX;
    DispToWin(6,1);
}

void Widget::onEigWinShowSlot()
{
    mVoMode = VO_MODE_1MUX;
    DispToWin(7,1);
}

void Widget::onRightButtonClickSlot()
{
    qDebug()<<"left button";
    switch (mVoMode) {
        case VO_MODE_1MUX:
            mVo_Index++;
            DispToWin(mVo_Index,1);
            break;
        case VO_MODE_4MUX:
            mVo_Index += 4;
            DispToWin(mVo_Index,4);
            break;
    }
}

void Widget::onLeftButtonClickSlot()
{
    qDebug()<<"right button";
    switch (mVoMode) {
        case VO_MODE_1MUX:
            mVo_Index--;
            if(mVo_Index < 0)
                mVo_Index = VI_CNT - 1;
            DispToWin(mVo_Index,1);
            break;
        case VO_MODE_4MUX:
            mVo_Index -= 4;
            if(mVo_Index < 0)
                mVo_Index += VI_CNT;
            DispToWin(mVo_Index,4);
            break;
    }
}



