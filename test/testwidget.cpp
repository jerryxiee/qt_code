#include "testwidget.h"
#include <QDebug>

TestWidget::TestWidget(QWidget *parent) : QWidget(parent)
{

//    button = new QPushButton(tr("button"),this);

    exit = new QAction("exit",this);
    connect(exit, SIGNAL(triggered()), this, SLOT(ExitSlot()));
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//        if (engine.rootObjects().isEmpty())
//            return;

    mainmenu = new QMenu("exit");
    m_quickWidget = new QQuickWidget(this);//this基类为QWidget
    m_quickWidget->move(0,0);
    m_quickWidget->resize(1280,720);
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
//    m_quickWidget->setHidden(true);
    QUrl source("qrc:/main.qml");
    m_quickWidget->setSource(source);
}

TestWidget::~TestWidget()
{
    delete exit;
    delete mainmenu;
    delete m_quickWidget;
}
void TestWidget::ExitSlot()
{


    qDebug()<<"ExitSlot count:";

}

void TestWidget::contextMenuEvent(QContextMenuEvent* e)
{

//    qDebug()<<"menu event";
//    mainmenu->addAction(exit);
//    mainmenu->exec(e->globalPos());

}
