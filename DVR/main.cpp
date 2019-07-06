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
using namespace std;


void sign(int signal)
{
    qDebug("signal(%#X)",signal);
    QApplication::quit();
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

//    vio.Vi_Start(VIDEO_ENCODING_MODE_PAL);
//    vio.Vo_Start();
//    vio.Vi_Venc_Start();
//    vio.start();
//    QObject::connect(&vio,SIGNAL(VistatusChanged(VI_CHN)),&vio,SLOT(onChangeStatus(VI_CHN)));
//    QObject::connect(&vio,SIGNAL(MakeNewFile(VI_CHN)),&vio,SLOT(onMakeNewFile(VI_CHN)));
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
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =QDateTime::currentDateTime().toString("ddd yyyy年MM月dd日 hh时mm分ss秒 AP");
    qDebug()<<current_date;
    QByteArray time = current_date.toLocal8Bit();
    qDebug("%s",time.data());
//    qDebug()<<current_date_time.toTime_t();
//    qDebug()<<QDateTime::fromString(current_date,"yyyy-MM-dd-hh-mm-ss").toTime_t();

    qmlRegisterType<MyTableModel>("MyTableViewModel", 1, 0, "MyTableViewModel");
    qmlRegisterType<YuvFrameProvider>("FrameProvider", 1, 0, "FrameProvider");
//    qmlRegisterType<SystemSet>("SystemConfig", 1, 0, "SystemConfig");

    try {
        Widget w;
        w.show();


        signal(SIGINT,sign);
//        signal(SIGSEGV, sign);
#ifndef LUNUX_WIN
//    vio.Vi_Start(VIDEO_ENCODING_MODE_PAL);
//    vio.Vo_Start();
//    vio.Vi_Venc_Start();
//    vio.start();
////    vio.start_timer();
//    QObject::connect(&w,SIGNAL(ChnDispToWinSignal(QMap<VO_CHN, RECT_S> &)),&vio,SLOT(onDispChnToWin(QMap<VO_CHN, RECT_S> &)));
//    QObject::connect(&w,SIGNAL(Set_VoMode(SAMPLE_VO_MODE_E &)),&vio,SLOT(onSet_VoMode(SAMPLE_VO_MODE_E &)));
//    QObject::connect(&w,SIGNAL(StopVoSignal()),&vio,SLOT(onStopVoSlot()));
#endif

        ret = a.exec();
    } catch(exception &e) {
        qDebug()<<"QApplication::quit";
        QApplication::quit();
    }
#ifndef LUNUX_WIN
//    vio.Venc_exit();
//    vio.wait();

//    QThread::sleep(2);
#endif
    qDebug()<<"exit main";
    return ret;
}
