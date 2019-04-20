#include "widget.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include "common/sample_common_sys.h"

int main(int argc, char *argv[])
{
    Sample_Common_Sys sys_init;

    if(sys_init.Sys_init_Sucess < 0){
        return -1;
    }
    qDebug("system init sucess!\n");

    sys_init.VdecTest();
//    sys_init.Vio_8_1080P_Test();
    //sys_init.SAMPLE_VDEC_H264();
//    sys_init.Vio_Venc_Test();

    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}
