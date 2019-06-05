#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QList>

#include "settings/settings.h"
#include "common/sample_comm.h"
#ifndef LUNUX_WIN
#include "video/vio.h"
#include <video/videoplay.h>
#endif

//Q_DECLARE_METATYPE(VDEC_PARAM)
//Q_DECLARE_METATYPE(STREAMTYPE)
//Q_DECLARE_METATYPE(PIC_SIZE_E)
//Q_DECLARE_METATYPE(SAMPLE_RC_E)
//Q_DECLARE_METATYPE(HI_BOOL)


class VideoControl : public QObject
{


    Q_OBJECT
//    Q_ENUMS(STREAMTYPE)

//    Q_PROPERTY(STREAMTYPE m_mainStream READ mainStrean WRITE setStream)
//    Q_PROPERTY(STREAMTYPE m_streamType READ streanType WRITE setStreamType)
//    Q_PROPERTY(PIC_SIZE_E m_vencSize READ vencSize WRITE setVencSize)
//    Q_PROPERTY(SAMPLE_RC_E m_enRcMode READ enRcMode WRITE setEnRcMode)
//    Q_PROPERTY(HI_U32      m_u32BitRate READ bitRate WRITE setBitRate)
//    Q_PROPERTY(HI_U32      m_u32Profile READ profile WRITE setProfile)
//    Q_PROPERTY(HI_FR32     m_fr32DstFrmRate READ frmRate WRITE setFrmRate)

public:



    explicit VideoControl(QObject *parent = nullptr);
    ~VideoControl();

    HI_BOOL videoStart();


    HI_BOOL loadVideoConfig();
    QString readVencConfig(VI_CHN Chn,HI_U32 stream,QString str);
    HI_BOOL readVencConfig(VI_CHN Chn,HI_U32 stream,VDEC_PARAM &str);

    void setVencConfig(VI_CHN Chn,HI_U32 stream,QString str,QString value);
    void setVencConfig(VI_CHN Chn,HI_U32 stream,VDEC_PARAM &str);

    Q_INVOKABLE void setVencAttr(QString Chn,QString mainStream,QString streamType,QString vencSize,QString enRcMode,
                                 QString profile,QString dstFrmRate,QString bitRate);

//    STREAMTYPE mainStrean() const;
//    void setStream(STREAMTYPE &stream);
//    STREAMTYPE streanType() const;
//    void setStreamType(STREAMTYPE &streamType);
//    PIC_SIZE_E vencSize() const;
//    void setVencSize(PIC_SIZE_E &size);
//    SAMPLE_RC_E enRcMode() const;
//    void setEnRcMode(SAMPLE_RC_E &enRcMode);
//    HI_U32 bitRate() const;
//    void setBitRate(HI_U32 &bitRate);
//    HI_U32 profile() const;
//    void setProfile(HI_U32 &profile);
//    HI_FR32 frmRate() const;
//    void setFrmRate(HI_FR32 &frmRate);


signals:
    void vencAttrChanged(VI_CHN Chn,HI_U32 stream);

public slots:
    void onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void onDispChnToWin(QMap<VO_CHN, RECT_S> &);
    void onStopVoSlot();
    void onVencAttrChangedSlot(VI_CHN Chn,HI_U32 stream);

private:
    QString vencToString(QString attr,QVariant &variant);
    QVariant stringToVenc(QString &str);
    const QString VENC_Open = "open";
    const QString VENC_StreamType = "StreamType";
    const QString VENC_EnSize = "enSize";
    const QString VENC_EnRcMode  = "enRcMode";
    const QString VENC_Profile = "Profile";
    const QString VENC_FrameRate = "FrameRate";
    const QString VENC_BitRate = "BitRate";


public:


//    STREAMTYPE m_mainStream;    //主子码流
//    STREAMTYPE m_streamType;    //视频流、复合流
//    PIC_SIZE_E m_vencSize;      //编码分辨率
//    SAMPLE_RC_E m_enRcMode;     //码率控制类型
//    HI_U32      m_u32BitRate;   //码率大小
//    HI_U32      m_u32Profile;   //图像等级
//    HI_FR32     m_fr32DstFrmRate ; //编码帧率

    QList<VDEC_PARAM> m_Vdec_Param[2];
//    QList<VDEC_PARAM> m_ChildVdec_Param;


private:

#ifndef LUNUX_WIN
    const HI_CHAR * VIO_CONFIG_PATH = "/usr/dvr_config/video.ini";

    VIDEO_NORM_E m_enNorm;
    Vio vio;
//    Vdec mVdec;
#else
    const HI_CHAR * VIO_CONFIG_PATH = "/home/abhw/nfsroot/dvr_config/video.ini";
#endif
    VencSet *m_VencSet;

};

#endif // VIDEOCONTROL_H
