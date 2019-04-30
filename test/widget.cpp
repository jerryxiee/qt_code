#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QPainter>
#include <QRect>
#include <QBrush>
#include <QFile>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1280,720);
    mainmenu = new QMenu();
//    this->setWindowOpacity(0.5);

    mStackWidget = new QStackedWidget(this);
    testwidget = new TestWidget(this);
    testwidget->move(0,0);

    mStackWidget->addWidget(testwidget);

    mStackWidget->resize(1280,720);
//    mStackWidget->hide();
//    timer = new QTimer();

//    connect(timer,SIGNAL(timeout()),this,SLOT(onMakefile()));

//    timer->start(2000);
    exit = new QAction("exit_main",this);
//    connect(exit, SIGNAL(triggered()), this, SLOT(ExitSlot()));
//    mFileWatch = new QFileSystemWatcher(this);

//    mFileWatch->addPath(QString::fromLatin1("/home/abhw/Qt5.9.7/projects/test"));
//    mFileWatch->addPath(QString::fromLatin1("/home/abhw/Qt5.9.7/projects/test/file"));
//    connect(mFileWatch,SIGNAL(fileChanged(QString)),this,SLOT(changestatus(QString)));
//    connect(mFileWatch,SIGNAL(directoryChanged(QString)),this,SLOT(changestatus(QString)));

}

void Widget::onMakefile()
{
    for(int i = 0;i < 20;i++){
    QFile file("/home/abhw/Qt5.9.7/projects/test/file");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open failed." << endl;
        return ;
    }

//    QTextStream txtOutput(&file);


//        txtOutput<<"count="<<i<<endl;

    file.close();
    }

}

Widget::~Widget()
{
    qDebug()<<"exit";
    delete ui;
}


void Widget::changestatus(const QString &path)
{
    qDebug()<<"file changed"+path;

}
void Widget::mousePressEvent(QMouseEvent * event)
{

    if(event->button() == Qt::RightButton){
        qDebug("press right ");

    }else if(event->button() == Qt::LeftButton){
        qDebug("press left ");

    }else {
        qDebug()<<"send emit set vo_mode"<<endl;

    }
}

void Widget::ExitSlot()
{
    int count = mStackWidget->count();

    qDebug()<<"ExitSlot count:"<<count;

}

void Widget::contextMenuEvent(QContextMenuEvent* e)
{

    mainmenu->addAction(exit);
    mainmenu->exec(e->globalPos());

}

void Widget::paintEvent(QPaintEvent *event)
{

//    QPainter painter(this);// 创建QPainter一个对象

//    painter.setCompositionMode( QPainter::CompositionMode_Clear );
//    painter.fillRect( 0, 0, 1280, 7200, Qt::SolidPattern );

}
