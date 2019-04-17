#include "widget.h"
#include <QApplication>
#include "common/sample_common_sys.h"

int main(int argc, char *argv[])
{
    Sample_Common_Sys sys_init;

    sys_init.init();
    //sys_init.SAMPLE_VDEC_H264();

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
