#include "playvideo.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTimeEdit>
#include <QFont>



PlayVideo::PlayVideo(QWidget *parent) : QWidget(parent),mVdecStart(false)
{
//    mButton = new QPushButton(tr("button"),this);
//    mButton->move(100,100);

//    connect(mButton,SIGNAL(clicked()),this,SLOT(onStartVdecSlot()));

//    this->setWindowOpacity(0.3);
//    mQuickWidget = new QQuickWidget(this);//this基类为QWidget
//    mQuickWidget->move(0,0);
//    mQuickWidget->resize(1280,720);
//    mQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
////    mQuickWidget->setHidden(true);
//    QUrl source("qrc:/main.qml");
//    mQuickWidget->setSource(source);
    QFont font("SimSun",12,50);
#if 1

//    QPushButton *mBackButton1 = new QPushButton(tr("back"));
//    mBackButton1->setFont(font);
//    QPushButton *mHomeButton2 = new QPushButton(tr("home"));
//    mHomeButton2->setFont(font);

    AnimationButton *mBackButton = new AnimationButton;
    mBackButton->resize(40,40);
    mBackButton->move(0,0);
    mBackButton->setDuration(0);
    mBackButton->setImage(":/images/back.png");
    mBackButton->setImageSize(40,40);


    AnimationButton *mHomeButton = new AnimationButton;
    mHomeButton->resize(40,40);

    mHomeButton->setDuration(0);
    mHomeButton->setImage(":/images/home.png");
    mHomeButton->setImageSize(40,40);

    QHBoxLayout *ToolButtonLayout = new QHBoxLayout;
//    ToolButtonLayout->addWidget(mBackButton1);
//    ToolButtonLayout->addWidget(mHomeButton2);
    ToolButtonLayout->addWidget(mBackButton);
    ToolButtonLayout->addWidget(mHomeButton);
    ToolButtonLayout->setSpacing(1280 - mHomeButton->width() - mBackButton->width());


    QComboBox *FondTypeBox = new QComboBox;
    FondTypeBox->setFont(font);
    FondTypeBox->addItem("常规查询");
    QComboBox *ChnBox = new QComboBox;
    ChnBox->setFont(font);
    for(int i = 0;i < 8;i++)
    {
        ChnBox->addItem(QString::asprintf("Chn%d",i));
    }
    QComboBox *FileTypeBox = new QComboBox;
    FileTypeBox->setFont(font);
    FileTypeBox->addItem("全部文件类型");
    QGridLayout *BoxGrpLayout = new QGridLayout;
    BoxGrpLayout->setVerticalSpacing(40);

    QLabel *FindType = new QLabel(tr("查询方式"));
    FindType->setFont(font);

    BoxGrpLayout->addWidget(FindType, 0, 0);
    BoxGrpLayout->addWidget(FondTypeBox, 0, 1);
    QLabel *ChnSelect = new QLabel(tr("通道选择"));
    ChnSelect->setFont(font);
    BoxGrpLayout->addWidget(ChnSelect, 1, 0);
    BoxGrpLayout->addWidget(ChnBox,1,1);
    QLabel *FileType = new QLabel(tr("通道选择"));
    FileType->setFont(font);
    BoxGrpLayout->addWidget(FileType, 2, 0);
    BoxGrpLayout->addWidget(FileTypeBox,2,1);
    QLabel *StartTime = new QLabel(tr("通道选择"));
    StartTime->setFont(font);
    BoxGrpLayout->addWidget(StartTime, 3, 0);
    QDateTimeEdit *DateTime = new QDateTimeEdit(QDateTime::currentDateTime());
    DateTime->setFont(font);
    BoxGrpLayout->addWidget(DateTime, 3, 1);
    QLabel *EndtTime = new QLabel(tr("通道选择"));
    EndtTime->setFont(font);
    BoxGrpLayout->addWidget(EndtTime, 4, 0);
    QDateTimeEdit *DateTime1 = new QDateTimeEdit(QDateTime::currentDateTime());
    DateTime1->setFont(font);

    BoxGrpLayout->addWidget(DateTime1, 4, 1);

    QGridLayout *ButGrpLayout = new QGridLayout;
    ButGrpLayout->setHorizontalSpacing(30);
    ButGrpLayout->setVerticalSpacing(20);
    QPushButton *FindButton = new QPushButton(tr("查询"));
    FindButton->setFont(font);
    connect(FindButton,SIGNAL(clicked()),this,SLOT(onFindButtonClickedSlot()));
    QPushButton *WatchdButton = new QPushButton(tr("浏览"));
    WatchdButton->setFont(font);
    connect(WatchdButton,SIGNAL(clicked()),this,SLOT(onWatchdButtonClickedSlot()));
    QPushButton *OpenButton = new QPushButton(tr("打开"));
    OpenButton->setFont(font);
    connect(OpenButton,SIGNAL(clicked()),this,SLOT(onOpenButtonClickedSlot()));
    QPushButton *CloseButton = new QPushButton(tr("关闭"));
    CloseButton->setFont(font);
    connect(CloseButton,SIGNAL(clicked()),this,SLOT(onCloseButtonClickedSlot()));
    ButGrpLayout->addWidget(WatchdButton,0,0);
    ButGrpLayout->addWidget(FindButton,0,1);
    ButGrpLayout->addWidget(OpenButton,1,0);
    ButGrpLayout->addWidget(CloseButton,1,1);

    QVBoxLayout *findlayout = new QVBoxLayout;
    findlayout->addLayout(BoxGrpLayout);
    findlayout->addLayout(ButGrpLayout);
    findlayout->setStretchFactor(BoxGrpLayout,4);
    findlayout->setStretchFactor(ButGrpLayout,1);


//    QFileSystemModel *mFileModel = new QFileSystemModel;
//    mFileModel->setRootPath(QDir::currentPath());

//    fileListWidget = new QListWidget;
    mFileTabWidget = new QTableWidget(0, 3);
    mFileTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mFileTabWidget->setFont(font);
//    mFileTabWidget->setColumnWidth(1,150);

    QStringList labels;
    labels << QStringLiteral("名称")<< QStringLiteral("修改时间")<<QStringLiteral("文件大小");
    mFileTabWidget->setHorizontalHeaderLabels(labels);
    mFileTabWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    mFileTabWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    mFileTabWidget->verticalHeader()->hide();
    mFileTabWidget->setShowGrid(false);
    connect(mFileTabWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(oncellDoubleClickedSlot(int,int)));
    mFileTabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mFileTabWidget, &QTableWidget::customContextMenuRequested,
            this, &PlayVideo::oncontextMenuSlot);
#ifdef arm
    QDir rootDir("/mnt/sda1/venc/");
#else
    QDir rootDir("/home/abhw");
#endif

    mCurrentPath = rootDir.absolutePath();
    QStringList string;
    string<<"*";
    QFileInfoList list = rootDir.entryInfoList(string,QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot);
    showFileInfoList(list);

//    QTreeView *mTreeView = new QTreeView(this);
//    mTreeView->setModel(mFileModel);
//    mTreeView->setFont(font);
//    mTreeView->setColumnWidth(0,200);
//#ifdef arm
//    mTreeView->setRootIndex(mFileModel->index("/mnt/sda1/venc"));
//#else
//    mTreeView->setRootIndex(mFileModel->index("/home/abhw"));
//#endif

//    QPushButton *mkdirButton = new QPushButton(tr("Make Directory..."), this);
//    mkdirButton->setFixedSize(QSize(48,32));
//    QPushButton *rmButton = new QPushButton(tr("Remove"), this);
//    rmButton->setFixedSize(48,32);
//    QHBoxLayout *buttonLayout = new QHBoxLayout;
//    buttonLayout->addWidget(mkdirButton);
//    buttonLayout->addWidget(rmButton,Qt::AlignRight | Qt::AlignVCenter);


    mback = new QPushButton(tr("返回"));
    mback->setFont(font);
    mback->setFixedSize(48,24);
//    back->hide();
    connect(mback,SIGNAL(clicked()),this,SLOT(onBackButtonClickedSlot()));
    mMainLab = new QLabel(rootDir.absolutePath());
    mMainLab->setFont(font);
    QHBoxLayout *BackLayout = new QHBoxLayout;
    BackLayout->addWidget(mback);
    BackLayout->addWidget(mMainLab);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(BackLayout);
    layout->addWidget(mFileTabWidget);
//    layout->addLayout(buttonLayout);

    QHBoxLayout *MainLayout = new QHBoxLayout;
    MainLayout->addLayout(findlayout);
    MainLayout->addLayout(layout);
    MainLayout->setStretchFactor(findlayout,1);
    MainLayout->setStretchFactor(layout,4);

//    setLayout(MainLayout);

    QVBoxLayout *Windowlayout = new QVBoxLayout;
    Windowlayout->addLayout(ToolButtonLayout);
    Windowlayout->addLayout(MainLayout);
    Windowlayout->setStretchFactor(ToolButtonLayout,1);
    Windowlayout->setStretchFactor(MainLayout,20);
    setLayout(Windowlayout);

#endif




}

void PlayVideo::onBackButtonClickedSlot()
{
    QDir dir;

    if(!mPath.isEmpty()){
        mCurrentPath = mPath.pop();
        dir.setPath(mCurrentPath);
        onShowSlot(dir);
        mMainLab->setText(mCurrentPath);
         qDebug()<<"mCurrentPath"<<mCurrentPath;
    }

}

void PlayVideo::oncellDoubleClickedSlot(int row,int column)
{
    QTableWidgetItem * item1 = new QTableWidgetItem;
    item1 = mFileTabWidget->item(row,0);
    QString text = item1->text();
    QString filename = mCurrentPath+"/"+text;
    QFileInfo fileinfo(filename);

    if(fileinfo.isFile()){
        qDebug()<<"send VideoDispSignal:"<<item1->data(absoluteFileNameRole).toString();
        emit VideoDispSignal(filename);
    }else{
        onDirShowSlot(text);
    }

    qDebug()<<"testSlot"<<filename;
}

void PlayVideo::onShowSlot(QDir dir)
{
    QStringList string;
    string<<"*";
    QFileInfoList list = dir.entryInfoList(string, QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot , QDir::DirsFirst);
    showFileInfoList(list);
}

void PlayVideo::showFileInfoList(QFileInfoList list)
{
    mFileTabWidget->setRowCount(0);
    mFileTabWidget->clearContents();
    qDebug()<<"list.count ="<<list.count();
    for(unsigned int i=0; i<list.count() ;i++){
        QFileInfo tmpFileInfo = list.at(i);
//        if(tmpFileInfo.isDir()){
//            QIcon icon("dir.png");
            QString fileName = tmpFileInfo.fileName();
            QTableWidgetItem *name = new QTableWidgetItem(/*icon,*/ fileName);
            name->setFlags(name->flags() ^ Qt::ItemIsEditable);
//            name->setData(absoluteFileNameRole,fileName);

            QDateTime time = tmpFileInfo.lastModified();
            QTableWidgetItem *modfilytime = new QTableWidgetItem(/*icon,*/ time.toString("yyyy-MM-dd-hh-mm-ss"));
            modfilytime->setFlags(modfilytime->flags() ^ Qt::ItemIsEditable);

            const qint64 size = tmpFileInfo.size();
            QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB")
                                                 .arg(int((size + 1023) / 1024)));
            sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

            int row = mFileTabWidget->rowCount();
            mFileTabWidget->insertRow(row);
            mFileTabWidget->setItem(row, 0, name);
            mFileTabWidget->setItem(row, 1, modfilytime);
            mFileTabWidget->setItem(row, 2, sizeItem);
//        }
//        else if(tmpFileInfo.isFile()){
////            QIcon icon("file.png");
//            QString fileName = tmpFileInfo.fileName();
//            QListWidgetItem *tmp = new QListWidgetItem(/*icon,*/ fileName);
//            fileListWidget->addItem(tmp);
//        }
    }
}

void PlayVideo::onDirShowSlot(QString &filename)
{
    qDebug()<<"enter slotDirShow";
//    QString str = item->text();
    QDir dir;
    dir.setPath(mCurrentPath);
    mPath.push(mCurrentPath);
    dir.cd(filename);
    mCurrentPath = dir.absolutePath();
    mMainLab->setText(mCurrentPath);

    onShowSlot(dir);
}


PlayVideo::~PlayVideo()
{
//    mVdec.Stop_Vdec();
//    mVdec.wait();
    qDebug()<<"exit PlayVideo";
}

void PlayVideo::oncontextMenuSlot(const QPoint &pos)
{

}
void PlayVideo::onFindButtonClickedSlot()
{
//    QStringList labels;
//    labels << tr("文件名")<< tr("开始时间") <<tr("结束时间")<<tr("文件大小");
//    mFileTabWidget->setColumnCount(4);
//    mFileTabWidget->setHorizontalHeaderLabels(labels);
}

void PlayVideo::onWatchdButtonClickedSlot()
{

}

void PlayVideo::onOpenButtonClickedSlot()
{

}

void PlayVideo::onCloseButtonClickedSlot()
{

}

void PlayVideo::onStartVdecSlot()
{

//    if(!mVdecStart){
//        mVdecStart = true;
//        mVdec.Start_Vdec("/mnt/sda1/venc/channel0/1970-01-01-06-25-00.h265");
//    }else{
//        mVdecStart = false;
//        mVdec.Stop_Vdec();
//        mVdec.wait();
//    }

}

void PlayVideo::contextMenuEvent(QContextMenuEvent* e)
{

}

void PlayVideo::mouseDoubleClickEvent(QMouseEvent *event)
{

}

//void PlayVideo::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);// 创建QPainter一个对象
////    QPen pen;
//    QFont font;

//    if(mVdecStart){
//        painter.setCompositionMode( QPainter::CompositionMode_Clear );
//        painter.fillRect( 0, 0, 1280, 720, Qt::SolidPattern );
//    }

//}
