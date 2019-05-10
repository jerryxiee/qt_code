#include "widget.h"
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QString>
#include <signal.h>
#include <QFontDatabase>
//#ifndef LUNUX_WIN
#include "common/sample_common_sys.h"
#include "video/vio.h"
//#endif


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
    Vio vio;

    if(sys_init.Sys_init_Sucess < 0){
        return -1;
    }
    qDebug("system init sucess! dgf\n");

//    vio.Vi_Start();
//    vio.Vo_Start();
//    vio.Vi_Venc_Start();
//    vio.start();
    QObject::connect(&vio,SIGNAL(VistatusChanged(VI_CHN)),&vio,SLOT(onChangeStatus(VI_CHN)));
    QObject::connect(&vio,SIGNAL(MakeNewFile(VI_CHN)),&vio,SLOT(onMakeNewFile(VI_CHN)));
#endif

//    sys_init.VdecTest();
//    sys_init.Vio_8_1080P_Test();
    //sys_init.SAMPLE_VDEC_H264();
//    sys_init.Vio_Venc_Test();

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd-hh-mm-ss");
    qDebug()<<current_date;

    QApplication a(argc, argv);

    int nIndex = QFontDatabase::addApplicationFont(QString("/opt/Qt5.8-Arm/lib/fonts/my.ttf"));
    if(nIndex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
        QFont fontThis(strList.at(0));
        fontThis.setPointSize(9);
        a.setFont(fontThis);
        }
    }

    Widget w;
    w.show();
    signal(SIGINT,sign);
#ifndef LUNUX_WIN
    vio.Vi_Start();
    vio.Vo_Start();
    vio.Vi_Venc_Start();
    vio.start();
    vio.start_timer();
    QObject::connect(&w,SIGNAL(ChnDispToWinSignal(QMap<VO_CHN, RECT_S> &)),&vio,SLOT(onDispChnToWin(QMap<VO_CHN, RECT_S> &)));
    QObject::connect(&w,SIGNAL(Set_VoMode(SAMPLE_VO_MODE_E &)),&vio,SLOT(onSet_VoMode(SAMPLE_VO_MODE_E &)));
    QObject::connect(&w,SIGNAL(StopVoSignal()),&vio,SLOT(onStopVoSlot()));
#endif

//
//    QObject::connect(&a,SIGNAL(finished()),&vio,SLOT(onfinish()));

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;
    ret = a.exec();
#ifndef LUNUX_WIN
    vio.Venc_exit();
    vio.wait();
    qDebug()<<"exit main";
    QThread::sleep(2);
#endif
    return ret;
}
