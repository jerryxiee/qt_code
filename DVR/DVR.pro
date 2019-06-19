#-------------------------------------------------
#
# Project created by QtCreator 2019-05-15T16:53:27
#
#-------------------------------------------------

QT       += core gui quickwidgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DVR
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

#greaterThan(QT_VERSION, 5.9):DEFINES += LUNUX_WIN

contains(QT_ARCH, x86_64) {
        message("x86_64")
        DEFINES += LUNUX_WIN
    } else {

    }

SOURCES += \
        main.cpp \
        widget.cpp \
    windows/CustomControls/animationbutton.cpp \
    windows/MyModel/mytablemodel.cpp \
    video/videocontrol.cpp \
    settings/settings.cpp \
    settings/vencset.cpp \
    video/yuvframeprovider.cpp \
    windows/mainwindow.cpp \
    windows/videodisplay.cpp \
    windows/regionmanage.cpp \
    settings/dispset.cpp \
    settings/config.cpp \
    settings/systemset.cpp




HEADERS += \
        widget.h \
    windows/CustomControls/animationbutton.h \
    windows/MyModel/mytablemodel.h \
    video/videocontrol.h \
    settings/settings.h \
    settings/vencset.h \
    video/yuvframeprovider.h \
    windows/mainwindow.h \
    windows/videodisplay.h \
    windows/regionmanage.h \
    settings/dispset.h \
    settings/config.h \
    settings/systemset.h



INCLUDEPATH += /opt/hisi-linux/x86-arm/arm-hisiv600-linux/target/usr/include \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/include \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/nvp6134_ex \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/tlv320aic31


if(contains(DEFINES,LUNUX_WIN)){
        message("linux platform")
}else{
SOURCES +=common/sample_common_sys.cpp \
            common/loadbmp.c \
            common/sample_common_vpss.cpp \
            common/sample_common_vdec.cpp \
#            common/sample_common_vio.cpp \
            common/sample_common_venc.cpp \
            common/sample_common_vi.cpp \
            common/sample_common_vo.cpp \
            video/venc.cpp \
            video/vio.cpp \
            video/videoplay.cpp

HEADERS +=common/loadbmp.h \
            common/sample_common_sys.h \
            common/sample_common_vpss.h \
            common/sample_common_vdec.h \
#            common/sample_common_vio.h \
            common/sample_common_venc.h \
            common/sample_common_vi.h \
            common/sample_common_vo.h \
            video/venc.h \
            video/vio.h \
            video/videoplay.h


LIBS += -lpthread

LIBS += /usr/lib/libmpi.so \
        /usr/lib/libhdmi.so \
        /usr/lib/libjpeg.so  \
        /usr/lib/libVoiceEngine.so \
        /usr/lib/libupvqe.so  \
        /usr/lib/libdnvqe.so  \
        /usr/lib/libtde.so \
        /usr/lib/libive.so \
        /usr/lib/libmd.so \
        /usr/lib/libpciv.so \
        /usr/lib/libaacenc.so \
        /usr/lib/libaacdec.so

    message("arm platform")

}

FORMS += \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc
