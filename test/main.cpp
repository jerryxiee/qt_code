#include "widget.h"
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <limits>
#include <signal.h>
#include <QTimer>
#include <stdlib.h>
#include "QDebug"
#include <QDateTime>
#include <QByteArray>
#include <QVector>
#include <QList>
#include "thread_test.h"

//class thresd_test:public QThread
//{
//    Q_OBJECT
//public:
//    thresd_test(QThread *parent = nullptr);
//    ~thresd_test();

//protected:
//    virtual void run();
//public slots:
//    void onMakefile();
//private:
//    QTimer *timer;

//};

//thresd_test::thresd_test(QThread *parent):QThread (parent)
//{
//    qDebug()<<"thread_test";
//    timer = new QTimer();

//    connect(timer,SIGNAL(timeout()),this,SLOT(onMakefile()));

//    timer->start(2000);
//}
//thresd_test::~thresd_test()
//{
//    qDebug()<<"~thresd_test end";
//}

//void thresd_test::onMakefile()
//{
//    qDebug()<<"time out thread";
//}
//void thresd_test::run()
//{
//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return;
//    while(1);
//}



void sign(int signal)
{
    qDebug()<<"signal";
    QApplication::quit();
}

typedef struct{
    int a;
    int b;
}test;

QList<test> list;

void listtest(int n)
{
    test a;
    a.a = n;
    a.b = 100;
    list.append(a);
}
int main(int argc, char *argv[])
{
    int ret;

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd-hh-mm-ss");
    QByteArray ba = current_date.toLatin1();
    qDebug()<<current_date;
    qDebug("%s\n",ba.data());

//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    Widget w;
    w.show();

    QVector<test > abc;

    test ab = {0};
    abc.push_back(ab);


    qDebug()<<"abc.size()="<<abc.size()<<abc[0].a;
    qDebug()<<"list.count="<<list.count() ;

    listtest(1);
    qDebug()<<"list.count="<<list.count()<<list[0].a << list[0].b;
    list.removeAt(0);
    qDebug()<<"list.count="<<list.count() ;

    abc.erase(abc.begin(),abc.begin());
    qDebug()<<"abc.size()="<<abc.size();
    signal(SIGINT,sign);


//    thresd_test test;
//    test.start();

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;


    ret = a.exec();
//    test.quit();
    qDebug()<<"exit main";
    return ret;
}
