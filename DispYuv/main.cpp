#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>
#include <signal.h>
#include "video/fameprovider.h"
#include "video/myvideoframe.h"

#ifndef LUNUX_WIN
#include "common/sample_common_sys.h"
#endif


void sign(int signal)
{
    qDebug()<<"signal";
    QGuiApplication::quit();
}

int main(int argc, char *argv[])
{

#ifndef LUNUX_WIN
    Sample_Common_Sys sys_init;

    if(sys_init.Sys_init_Sucess < 0){
        return -1;
    }
    qDebug("system init sucess! dgf\n");

#endif

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    qmlRegisterType<FameProvider>("com.yourcompany.FrameProvider", 1, 0, "FrameProvider");

    QQmlApplicationEngine engine;

    FameProvider* frameProvider = new FameProvider();
    MyvideoFrame source;
    engine.rootContext()->setContextProperty("frameProvider", frameProvider);

    frameProvider->setFormat(source.mwidth,source.mheigh, source.mformat);
    // Connect your frame source with the provider
    QObject::connect(&source, SIGNAL(newFrameAvailable(const QVideoFrame &)), frameProvider, SLOT(onNewVideoContentReceived(const QVideoFrame &)));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

//    QObject *rootObject = engine.rootObjects().first();
//    QObject *display = rootObject->findChild<QObject *>("display");
//    auto provider = qvariant_cast<FameProvider *>(display->property("source"));




    signal(SIGINT,sign);
    return app.exec();
}
