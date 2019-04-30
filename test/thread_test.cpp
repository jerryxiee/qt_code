#include "thread_test.h"

thread_test::thread_test(QObject *parent) : QThread(parent)
{

    qDebug()<<"thread_test";
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(onMakefile()));
    timer->start(2000);
}
