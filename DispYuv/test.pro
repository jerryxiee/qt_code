QT += quick multimedia
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

contains(QT_ARCH, x86_64) {
        message("x86_64")
        DEFINES += LUNUX_WIN
    } else {

    }


SOURCES += \
        main.cpp \
    video/fameprovider.cpp \
    video/myvideoframe.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#HEADERS += \
#    common/loadbmp.h \
#    common/sample_comm.h \
#    common/sample_common_sys.h \
#    common/sample_common_vdec.h \
#    common/sample_common_venc.h \
#    common/sample_common_vio.h \
#    common/sample_common_vpss.h


INCLUDEPATH += /opt/hisi-linux/x86-arm/arm-hisiv600-linux/target/usr/include \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/include \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/nvp6134_ex \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/tlv320aic31


if(contains(DEFINES,LUNUX_WIN)){
    message("linux platform")
}else{
SOURCES +=common/sample_common_sys.cpp \
    common/sample_common_vdec.cpp \
    common/sample_common_venc.cpp \
    common/sample_common_vio.cpp \
    common/sample_common_vpss.cpp \
    common/loadbmp.c

HEADERS +=common/loadbmp.h \
    common/sample_comm.h \
    common/sample_common_sys.h \
    common/sample_common_vdec.h \
    common/sample_common_venc.h \
    common/sample_common_vio.h \
    common/sample_common_vpss.h


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

HEADERS += \
    video/fameprovider.h \
    video/myvideoframe.h

