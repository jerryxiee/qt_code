#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QPainter>
#include <QRect>
#include <QBrush>
#include <QFile>
#include <QStringList>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1280,720);
    mainmenu = new QMenu();

    AnimationButton * but = new AnimationButton(this);
    connect(but,SIGNAL(clicked()),this,SLOT(onClickedSlot()));
//    but->resize(30,30);
//    but->setText("but");
    but->setWindowOpacity(0.5);
    but->setImage(":/images/back.png");

//    this->setWindowOpacity(0.5);
//    setWindowFlag(Qt::CustomizeWindowHint,true);
//    setWindowFlag(Qt::FramelessWindowHint,true);

    mStackWidget = new QStackedWidget(this);
    testwidget = new TestWidget(this);
    testwidget->move(0,0);

    mStackWidget->addWidget(testwidget);

    mStackWidget->resize(1280,720);
    mStackWidget->hide();

    m_quickWidget = new QQuickWidget(this);//this基类为QWidget
    m_quickWidget->move(0,0);
    m_quickWidget->resize(1280,720);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
//    m_quickWidget->setHidden(true);
    QUrl source("qrc:/qml/main.qml");
    m_quickWidget->setSource(source);

    QObject *pRoot = (QObject*)m_quickWidget->rootObject();
    if(pRoot){
        connect(pRoot,SIGNAL(hidqmlsignal()),this,SLOT(onHidQmlSlot()));
    }
    QQmlContext *context = (QQmlContext *)m_quickWidget->rootContext();
    if(context){
        context->setContextProperty("test",this);
    }

//    timer = new QTimer();

//    connect(timer,SIGNAL(timeout()),this,SLOT(onMakefile()));

//    timer->start(2000);
    exit = new QAction("exit_main",this);
    connect(exit, SIGNAL(triggered()), this, SLOT(ExitSlot()));
//    mFileWatch = new QFileSystemWatcher(this);

//    mFileWatch->addPath(QString::fromLatin1("/home/abhw/Qt5.9.7/projects/test"));
//    mFileWatch->addPath(QString::fromLatin1("/home/abhw/Qt5.9.7/projects/test/file"));
//    connect(mFileWatch,SIGNAL(fileChanged(QString)),this,SLOT(changestatus(QString)));
//    connect(mFileWatch,SIGNAL(directoryChanged(QString)),this,SLOT(changestatus(QString)));

//        mFileModel = new QFileSystemModel;
//        mFileModel->setRootPath(QDir::currentPath());


//        mTreeView = new QTreeView(this);
//        mTreeView->setModel(mFileModel);
//        mTreeView->setColumnWidth(0,200);
//        mTreeView->setRootIndex(mFileModel->index("/home/abhw"));

//        QPushButton *mkdirButton = new QPushButton(tr("Make Directory..."), this);
//        QPushButton *rmButton = new QPushButton(tr("Remove"), this);
//        QHBoxLayout *buttonLayout = new QHBoxLayout;
//        buttonLayout->addWidget(mkdirButton);
//        buttonLayout->addWidget(rmButton);

//        QVBoxLayout *layout = new QVBoxLayout;
//        layout->addWidget(mTreeView);
//        layout->addLayout(buttonLayout);

//        setLayout(layout);
//        setWindowTitle("File System Model");

//        connect(mkdirButton, SIGNAL(clicked()),
//                this, SLOT(mkdir()));
//        connect(rmButton, SIGNAL(clicked()),
//                this, SLOT(rm()));

//    fileLineEdit = new QLineEdit(tr("/"));
//    fileListWidget = new QListWidget;
//    mainLayout = new QVBoxLayout(this);
//    mainLayout->addWidget(fileLineEdit);
//    mainLayout->addWidget(fileListWidget);
//    connect(fileLineEdit,SIGNAL(returnPressed()),this,SLOT(slotShow(QDir)));
//    connect(fileListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(slotDirShow(QListWidgetItem*)));
//    QString root = "/";    //可以更改盘符
//    QDir rootDir(root);
//    QStringList string;
//    string<<"*";
//    QFileInfoList list = rootDir.entryInfoList(string,QDir::AllEntries | QDir::NoDotAndDotDot);
//    showFileInfoList(list);


}

void Widget::test(QString str)
{
    qDebug()<<"str:"<<str;
}
void Widget::onClickedSlot()
{
    qDebug()<<"press";
    m_quickWidget->setHidden(false);
}

void Widget::onHidQmlSlot()
{
    m_quickWidget->setHidden(true);
}

void Widget::slotShow(QDir dir)
{
    QStringList string;
    string<<"*";
    QFileInfoList list = dir.entryInfoList(string, QDir::AllEntries | QDir::NoDotAndDotDot , QDir::DirsFirst);
    showFileInfoList(list);
}

void Widget::showFileInfoList(QFileInfoList list)
{
    fileListWidget->clear();
    for(unsigned int i=0; i<list.count() ;i++){
        QFileInfo tmpFileInfo = list.at(i);
        if(tmpFileInfo.isDir()){
//            QIcon icon("dir.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(/*icon,*/ fileName);
            fileListWidget->addItem(tmp);
        }else if(tmpFileInfo.isFile()){
//            QIcon icon("file.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(/*icon,*/ fileName);
            fileListWidget->addItem(tmp);
        }
    }
}

void Widget::slotDirShow(QListWidgetItem *item)
{
    QString str = item->text();
    QDir dir;
    dir.setPath(fileLineEdit->text());
    dir.cd(str);
    fileLineEdit->setText(dir.absolutePath());
    slotShow(dir);
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

//    mainmenu->addAction(exit);
//    mainmenu->exec(e->globalPos());

}

void Widget::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);// 创建QPainter一个对象

//    painter.setOpacity(0);

//    painter.setCompositionMode( QPainter::CompositionMode_Clear );
//    painter.fillRect( 0, 0, 1280, 720, Qt::SolidPattern );

}
