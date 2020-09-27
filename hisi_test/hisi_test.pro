TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    audio/sample_audio.cpp \
    common/sample_comm_audio.cpp \
    common/sample_comm_ivs.cpp \
    common/sample_comm_vdec.c \
        main.cpp \
    common/loadbmp.cpp \
    common/sample_comm_sys.cpp \
    common/sample_comm_venc.cpp \
    common/sample_comm_vo.cpp \
    common/sample_comm_vpss.cpp \
    common/sample_comm_vi.cpp \
    hifb/sample_hifb.cpp \
#    pciv/sample_common.c \
#    pciv/sample_pciv_host.c \
#    pciv/sample_pciv_slave.c \
    region/sample_region.c \
    tde/sample_tde.c \
    vio/sample_vio.cpp \
    common/sample_comm_vda.cpp \
    vdec/sample_vdec.cpp \
    venc/sample_venc.cpp \
    vda/sample_vda.cpp \
    vgs/sample_vgs.cpp \
    ive/sample/sample_ive_bg_model.cpp \
    ive/sample/sample_ive_canny.cpp \
    ive/sample/sample_ive_gmm.cpp \
    ive/sample/sample_ive_od.cpp \
    ive/sample/sample_ive_sobel_with_cached_mem.cpp \
    ive/sample/sample_ive_st_and_lk.cpp \
    ive/sample/sample_ive_test_memory.cpp \
    ive/sample/sample_ivs_md.cpp \
    ive/sample_ive_main.cpp \
#    pciv/pciv_msg.cpp \
#    pciv/pciv_trans.cpp \
    cascade/sample_cascade_master.cpp \
    cascade/sample_cascade_slave.cpp \
    audio/adp/audio_aac_adp.cpp


INCLUDEPATH += /opt/hisi-linux/x86-arm/arm-hisiv600-linux/target/usr/include \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/include \
                /home/abhw/git/qt_code/hisi_test/common  \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/nvp6134_ex \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/tlv320aic31 \
                /home/abhw/git/qt_code/hisi_test/audio/adp \
                /home/abhw/git/qt_code/hisi_test/audio/tp2853c \
                lib/include \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/drv/wtdg

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

LIBS +=-L$$PWD/lib/lib -lmp4v2


#LIBS += /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libmpi.a \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libhdmi.a \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libjpeg.a  \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libVoiceEngine.a \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libupvqe.a  \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libdnvqe.a  \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libtde.a \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libive.a \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libmd.a \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libpciv.a \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libaacenc.a \
#        /home/abhw/haisi/hi3531/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/lib/libaacdec.a

HEADERS += \
    common/loadbmp.h \
    common/sample_comm.h \
    common/sample_comm_ivs.h \
    common/loadbmp.h \
    common/sample_comm.h \
    common/loadbmp.h \
    common/sample_comm.h \
    ive/sample/sample_ive_main.h \
    pciv/hi_mcc_usrdev.h \
    pciv/pciv_msg.h \
    pciv/pciv_trans.h \
    pciv/sample_common.h \
    pciv/sample_pciv_comm.h \
    audio/adp/audio_aac_adp.h

