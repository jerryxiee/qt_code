TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    common/loadbmp.c \
    common/sample_comm_sys.c \
    common/sample_comm_venc.c \
    common/sample_comm_vo.c \
    common/sample_comm_vpss.c \
    common/sample_comm_vi.c \
    hifb/sample_hifb.c \
    vio/sample_vio.c \
    common/sample_comm_vdec.c \
    common/sample_comm_vda.c \
    common/sample_comm_ivs.c \
    vdec/sample_vdec.c \
    venc/sample_venc.c \
    vda/sample_vda.c \
    vgs/sample_vgs.c \
    ive/sample/sample_ive_bg_model.c \
    ive/sample/sample_ive_canny.c \
    ive/sample/sample_ive_gmm.c \
    ive/sample/sample_ive_od.c \
    ive/sample/sample_ive_sobel_with_cached_mem.c \
    ive/sample/sample_ive_st_and_lk.c \
    ive/sample/sample_ive_test_memory.c \
    ive/sample/sample_ivs_md.c \
    ive/sample_ive_main.c \
    region/sample_region.c \
    tde/sample_tde.c \
    pciv/pciv_msg.c \
    pciv/pciv_trans.c \
    pciv/sample_common.c \
    pciv/sample_pciv_host.c \
    pciv/sample_pciv_slave.c \
    cascade/sample_cascade_master.c \
    cascade/sample_cascade_slave.c \
    audio/adp/audio_aac_adp.c \
    audio/sample_audio.c \
    common/sample_comm_audio.c


INCLUDEPATH += /opt/hisi-linux/x86-arm/arm-hisiv600-linux/target/usr/include \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/include \
                /home/abhw/git/qt_code/hisi_test/common  \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/nvp6134_ex \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/tlv320aic31 \
                /home/abhw/git/qt_code/hisi_test/audio/adp \
                /home/abhw/git/qt_code/hisi_test/audio/tp2853c

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

