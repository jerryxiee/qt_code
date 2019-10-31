#include "widget.h"
#include <QApplication>
#include <QGuiApplication>
#include <QtQml>
#include <signal.h>
#include <QDateTime>
#include <QFontDatabase>
#include <QTextCodec>
#include "windows/MyModel/mytablemodel.h"
#include "video/yuvframeprovider.h"
//#ifndef LUNUX_WIN
#include "common/sample_common_sys.h"
#include "video/vio.h"
//#endif
#include <iostream>
#include <exception>
#include <execinfo.h>
#include "communication/remotethread.h"
#include "settings/platformset.h"
#include "windows/MyModel/platformfunmodel.h"
#include "modules/moduleinit.h"
#include "windows/test/testwidget.h"
#include "modules/regionctr/regionbase.h"
#include "modules/modulescontrol.h"

#ifndef LUNUX_WIN
#include "test/venctomp4test.h"
#endif
using namespace std;

#ifndef LUNUX_WIN
#include "video/realtimestream.h"
#endif

void dump(int signo)
{
    void *buffer[30] = {0};
    size_t size = 0;
    size_t i = 0;
    char ** strings = NULL;

    size = backtrace(buffer,30);

    strings = backtrace_symbols(buffer,size);
    if(strings == NULL)
    {
        return;
    }

    for (i = 0 ; i< size; i++)
    {
        printf("[%02d] %s\n", i, strings[i]);
    }

    fflush(stdout);
    free(strings);
    exit(0);
}

void sign(int signal)
{
    qDebug("signal(%#X)",signal);
    QApplication::quit();
}

void test(void *arg)
{
    qDebug()<<"test run";
    ModulesControl::getModulesControl()->getTaskScheduler().scheduleDelayedTask(1000000,test,nullptr);
}
int main(int argc, char *argv[])
{

    int ret;
#ifndef LUNUX_WIN
    Sample_Common_Sys sys_init;
//    Vio vio;

    if(sys_init.Sys_init_Sucess < 0){
        return -1;
    }
    qDebug("system init sucess! dgf\n");

#endif



    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QApplication a(argc, argv);

    int nIndex = QFontDatabase::addApplicationFont(QString("/nfsroot/Qt5.9.8-Arm/lib/fonts/my.ttf"));
    if(nIndex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
        QFont fontThis(strList.at(0));
        fontThis.setPointSize(12);
        a.setFont(fontThis);
        }
    }
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    signal(SIGINT,sign);
    signal(SIGSEGV, dump);

    RemoteThread *remotethread = RemoteThread::getRemoteThread();
    remotethread->start();

//    module_init();
    ModulesControl *moduleinit = ModulesControl::getModulesControl();
    moduleinit->initModules();
//    moduleinit->getTaskScheduler().scheduleDelayedTask(1000000,test,nullptr);
    moduleinit->start();

//    QDateTime datetime;
//    datetime.setDate(QDate::currentDate());
//    datetime.setTime(QTime::fromString("000050","hhmmss"));
//    qDebug()<<datetime;
//    qDebug()<<QTime::fromString("000150","hhmmss").second();
//    QString time = QDateTime::currentDateTime().toString();
//    qDebug()<<QDateTime::currentDateTime().toTime_t();
//    qDebug()<<QDateTime::fromString(time.right(6),"hhmmss");
//    qDebug()<<QDateTime::currentDateTime().toString("yyMMddhhmmss");

    qmlRegisterType<MyTableModel>("MyTableViewModel", 1, 0, "MyTableViewModel");
    qmlRegisterType<YuvFrameProvider>("FrameProvider", 1, 0, "FrameProvider");
    qmlRegisterType<PlatformSet>("PlatformSet", 1, 0, "PlatformSet");
    qmlRegisterType<PlatformFunModel>("PlatformFunModel", 1, 0, "PlatformFunModel");
//    qmlRegisterType<QFileInfo>("FileInfo", 1, 0, "FileInfo");
//    qmlRegisterType<SystemSet>("SystemConfig", 1, 0, "SystemConfig");

    Widget w;
//        TestWidget w;
        w.show();
//        qDebug()<<"distance:"<<RegionBase::distance(22514519,113380301,22511962,113380301);

#ifndef LUNUX_WIN
//    RealTimeStream realtest;
//    realtest.start();

#endif
        ret = a.exec();
    qDebug()<<"exit main";
    delete remotethread;
    delete moduleinit;
    return ret;
}
