#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QRect>
#include <QBrush>
#include <QFont>
#include <QtMath>
#include <QPixmap>
#include <QMap>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),mMainWin(true),mVdecOn(false)
{
    ui->setupUi(this);
    this->resize(1280,720);
#ifndef arm
    this->setWindowOpacity(1);
#endif

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

//    QPixmap *pixmap = new QPixmap(260, 260);
//    pixmap->load("images/back@2x.png");
    QIcon icon(":/images/back.png");

    mLeftButton = new QPushButton("",this);
    mLeftButton->setStyleSheet("QPushButton:hover{border-image: url(:/images/back.png);}""QPushButton:pressed{border-image: url(:/images/back.png);}");
//    mLeftButton->setIcon(icon);
    mLeftButton->resize(100,100);
    mLeftButton->setGeometry(0,(this->height() - mLeftButton->height())/2,mLeftButton->width(),mLeftButton->height());
    mLeftButton->setIconSize(QSize(mLeftButton->width()-10,mLeftButton->height()-10));
    mLeftButton->setWindowOpacity(0);
    mLeftButton->setFlat(true);
//    mLeftButton->setHidden(true);
//    mLeftButton->setStyleSheet("background-color:transparent");
    connect(mLeftButton,SIGNAL(clicked()),this,SLOT(onLeftButtonClickSlot()));


    icon = QIcon(":/images/back1.png");
    mRightButton = new QPushButton("",this);
    mRightButton->setStyleSheet("QPushButton:hover{border-image: url(:/images/back1.png);}""QPushButton:pressed{border-image: url(:/images/back1.png);}");
//    mRightButton->setIcon(icon);
    mRightButton->resize(100,100);
    mRightButton->setGeometry(this->width() - mLeftButton->width(),(this->height() - mLeftButton->height())/2,mLeftButton->width(),mLeftButton->height());
    mRightButton->setIconSize(QSize(mLeftButton->width()-10,mLeftButton->height()-10));
    mRightButton->setWindowOpacity(0);
    mRightButton->setFlat(true);
    connect(mRightButton,SIGNAL(clicked()),this,SLOT(onRightButtonClickSlot()));

//    mRightButton->setHidden(true);





    InitWin();
//    m_quickWidget = new QQuickWidget(this);//this基类为QWidget
//    m_quickWidget->move(0,0);
//    m_quickWidget->resize(1280,720);
//    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
//    m_quickWidget->setHidden(true);
//    QUrl source("qrc:/main.qml");
//    m_quickWidget->setSource(source);

}

void Widget::InitWin()
{
    mStackWidget = new QStackedWidget(this);
//    mMainWindow = new MainWindow;

//    mStackWidget->addWidget(mMainWindow);

    mPlayVideo = new PlayVideo;
    connect(mPlayVideo,SIGNAL(VideoDispSignal(QString &)),this,SLOT(onVideoDispSlot(QString &)));

    mStackWidget->addWidget(mPlayVideo);
    mStackWidget->resize(this->width(),this->height());
    mStackWidget->hide();
    qDebug()<<"mPlayVideo index:"<< mStackWidget->indexOf(mPlayVideo)<<"current index:"<<mStackWidget->currentIndex();
}

Widget::~Widget()
{
//    delete mLeftButton;
//    delete mRightButton;
//    delete mMenu;
//    delete mOneMenu;
//    delete mTwoMenu;
//    delete mOneActionGrp;
//    delete mTwoActionGrp;
//    delete mBack;
//    delete mExit_Vo;
//    delete mPlayVideo;
//    if(mStackWidget)
//        delete mStackWidget;
//    qDebug()<<"exit mStackWidget";
#ifdef arm
    mVdec.wait();
#endif
//    delete m_quickWidget;
//    for(int i = 0;i < mOneMenuAct.count(); i++){
//        delete mOneMenuAct[i];
//        mOneMenuAct.removeAt(i);
//    }
//    for(int i = 0; i < mTwoMenuAct.count();i++){
//        delete mTwoMenuAct[i];
//        mTwoMenuAct.removeAt(i);
//    }
    qDebug()<<"exit widget";
    delete ui;
}


void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);// 创建QPainter一个对象
//    QPen pen;
    QFont font;

//#ifdef arm
    if(mMainWin){
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        painter.fillRect( 0, 0, 1280, 720, Qt::SolidPattern );
    }
//#endif
//    pen.setColor(Qt::red);
//    QString text = "无信号";
//    font.setPixelSize(24);
//    QFontMetrics metrics(font);
//    int w=metrics.width(text); //获取显示文本的宽度
//    int h=metrics.height(); //获取显示文本的高度
//    painter.setFont(font);
//    painter.setPen(Qt::red);
//    painter.drawText((300-w)/2,(300-h)/2,text);

    // 画一条直线
//    QPen pen;
//    pen.setColor(Qt::yellow);           // 设置画笔为黄色
//    painter.setPen(pen);                // 设置画笔
//    painter.drawLine(rect().topLeft(), rect().bottomRight());
//    // 画一个空心矩形
//    pen.setColor(Qt::red);
//    painter.setPen(pen);
//    painter.drawRect(QRect(1, 1, 100, 100));
//    // 画一个实心矩形
//    QBrush bruch(Qt::FDiagPattern);     // 画刷
//    painter.setBrush(bruch);            // 设置画刷
//    painter.drawRect(QRect(105, 1, 100, 100));
//    // 画一个多点线
//    pen.setColor(Qt::white);
//    painter.setPen(pen);
//    bruch.setStyle(Qt::NoBrush);
//    // 将画刷设置成null
//    painter.setBrush(bruch);
//    static const QPointF points[4] = {QPointF(210.0, 1), QPointF(220.0, 50.3), QPointF(300, 100.4), QPointF(260.4, 120.0)};
//    painter.drawPolyline(points, 4);
//    // 画多个点
//    QPointF pointf[10];
//    for (int i=0; i<10; ++i)
//    {
//        pointf[i].setX(2.0+i*10.0);
//        pointf[i].setY(130.0);
//    }
//    painter.drawPoints(pointf, 10);
//    // 画多条线
//    QLineF linef[5];
//    for (int j=0; j<5; ++j)
//    {
//        linef[j].setP1(QPointF(110.9+j*10, 120.0));
//        linef[j].setP2(QPointF(120.8+j*12, 200.0));
//    }    painter.drawLines(linef, 5);

//    // 画一个圆角矩形

//    QRectF rectangle(290.0, 110.0, 50, 50);
//    bruch.setStyle(Qt::SolidPattern);
//    painter.setBrush(bruch);
//    painter.drawRoundedRect(rectangle, 20.0, 15.0);
//    // 画一个QString
//    painter.drawText(50, 300, "Hello DevDiv!");
//    //ps：下面这两个有点绿啊。。。。。。如果挡住了上面的图形的话就把他们两个注释掉吧
//    //画一个椭圆
//    painter.setRenderHint(QPainter::Antialiasing, true);//启用反走样，告诉QPainter用不同颜色强度绘制边框以减少视觉扭曲，这种扭曲一般
//    //会在边框转换为像素的时候发生。由此生成的结果是的到一条平滑的曲线
//    painter.setPen(QPen(Qt::black, 12, Qt::DashDotDotLine, Qt::RoundCap));
//    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
//    painter.drawEllipse(80, 80, 400, 240);
//    //绘制三次贝塞尔曲线
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    QPainterPath path;    path.moveTo(80, 320);
//    path.cubicTo(200, 80, 320, 80, 480, 320);
//    painter.setPen(QPen(Qt::black, 8));
//    painter.drawPath(path);
}


//void Widget::mousePressEvent(QMouseEvent * event)
//{
//    SAMPLE_VO_MODE_E enVoMode = VO_MODE_9MUX;
//    qDebug()<<event->pos();
//    if(event->button() == Qt::RightButton){
//        qDebug("press right ");
//        if(!mload_qml){
//            load_qml = true;
//            qDebug("load main.qml");

////            process = new QProcess();
////            process->start("/nfsroot/quck_test");
//            m_quickWidget = new QQuickWidget(this);//this基类为QWidget
//            m_quickWidget->move(0,0);
//            m_quickWidget->resize(1280,720);
//            m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
//            QUrl source("qrc:/main.qml");
//            m_quickWidget->setSource(source);
//            m_quickWidget->show();
//        }
//    }else if(event->button() == Qt::LeftButton){
//        qDebug("press left ");
//        if(mload_qml){
//            load_qml = false;
//            qDebug("unload main.qml");
////            process->kill();
////            delete  process;
//            m_quickWidget->deleteLater();
//            delete m_quickWidget;
//        }

//    }else {
//        qDebug()<<"send emit set vo_mode"<<endl;
//        emit Set_VoMode(enVoMode);
//    }
//}

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

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void Widget::onVideoExitClickSlot()
{
    mMainWin = false;
    mVdecOn = false;
    mVideoExit->hide();
    mStackWidget->show();
#ifdef arm
    mVdec.Stop_Vdec();
#endif
}

void Widget::onVideoDispSlot(QString &filepath)
{
    QByteArray filename = filepath.toLatin1();

    qDebug()<<"onVideoDispSlot filename: "<<filepath;
    mMainWin = true;
    mVdecOn = true;
    mRightButton->hide();
    mLeftButton->hide();
    mStackWidget->hide();
    mVideoExit->show();
#ifdef arm
    mVdec.Start_Vdec(filename.data());
#endif
}

void Widget::onMainMenuSlot()
{
    mMainWin = false;
    mStackWidget->show();
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
    mStackWidget->hide();
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
