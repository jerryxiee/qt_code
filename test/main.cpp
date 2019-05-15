#include "widget.h"
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
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
#include <QDir>
#include <iostream>
#include <string.h>
#include "thread_test.h"
#include "sources/model/mytableviewmodel.h"
#ifndef LUNUX_WIN
#include "common/sample_common_sys.h"
#endif



void sign(int signal)
{
    qDebug()<<"signal";
    QApplication::quit();
}


int main(int argc, char *argv[])
{
    int ret;

#ifndef LUNUX_WIN
    Sample_Common_Sys sys_init;

    if(sys_init.Sys_init_Sucess < 0){
        return -1;
    }
    qDebug("system init sucess! dgf\n");

#endif

    qmlRegisterType<MyTableViewModel>("MyTableViewModel", 1, 0, "MyTableViewModel");
    QApplication a(argc, argv);
    Widget w;
    w.show();

//    QQmlApplicationEngine engine;

//    engine.load(QUrl(QStringLiteral("qrc:/qml/test.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;

    signal(SIGINT,sign);

    ret = a.exec();
//    test.quit();
    qDebug()<<"exit main";
    return ret;
}
