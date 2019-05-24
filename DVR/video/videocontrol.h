#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QObject>
#include <QWidget>
#include <QDebug>

#include "common/sample_comm.h"
#ifndef LUNUX_WIN
#include "video/vio.h"
#include <video/vdec.h>
#endif


class VideoControl : public QObject
{


    Q_OBJECT
    Q_ENUMS(STREAMTYPE)

    Q_PROPERTY(STREAMTYPE m_mainStream READ mainStrean WRITE setStream)
    Q_PROPERTY(STREAMTYPE m_streamType READ streanType WRITE setStreamType)
    Q_PROPERTY(PIC_SIZE_E m_vencSize READ vencSize WRITE setVencSize)
    Q_PROPERTY(SAMPLE_RC_E m_enRcMode READ enRcMode WRITE setEnRcMode)
    Q_PROPERTY(HI_U32      m_u32BitRate READ bitRate WRITE setBitRate)
    Q_PROPERTY(HI_U32      m_u32Profile READ profile WRITE setProfile)
    Q_PROPERTY(HI_FR32     m_fr32DstFrmRate READ frmRate WRITE setFrmRate)

public:

    typedef enum StreamType{
      MAINTYPE,
      CHILDTYPE
    }STREAMTYPE;

    explicit VideoControl(QObject *parent = nullptr);
    ~VideoControl();

    HI_BOOL videoStart();


//    HI_BOOL Load_Config();

    STREAMTYPE mainStrean() const{
        qDebug()<<"read stream";
        return m_mainStream;
    }
    void setStream(STREAMTYPE &stream);
    STREAMTYPE streanType() const{
        qDebug()<<"read stream";
        return m_streamType;

    }
    void setStreamType(STREAMTYPE &streamType);
    PIC_SIZE_E vencSize() const;
    void setVencSize(PIC_SIZE_E &size);
    SAMPLE_RC_E enRcMode() const;
    void setEnRcMode(SAMPLE_RC_E &enRcMode);
    HI_U32 bitRate() const;
    void setBitRate(HI_U32 &bitRate);
    HI_U32 profile() const;
    void setProfile(HI_U32 &profile);
    HI_FR32 frmRate() const;
    void setFrmRate(HI_FR32 &frmRate);


signals:

public slots:
    void onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void onDispChnToWin(QMap<VO_CHN, RECT_S> &);
    void onStopVoSlot();


public:


    STREAMTYPE m_mainStream;    //主子码流
    STREAMTYPE m_streamType;    //视频流、复合流
    PIC_SIZE_E m_vencSize;      //编码分辨率
    SAMPLE_RC_E m_enRcMode;     //码率控制类型
    HI_U32      m_u32BitRate;   //码率大小
    HI_U32      m_u32Profile;   //图像等级
    HI_FR32     m_fr32DstFrmRate ; //编码帧率



private:
    const HI_CHAR * VIO_CONFIG_PATH = "/usr/dvr_config/video.ini";
#ifndef LUNUX_WIN
    Vio vio;
//    Vdec mVdec;
#endif

};

#endif // VIDEOCONTROL_H
