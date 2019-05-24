#include "videocontrol.h"
#include <QDebug>
#include <QMetaType>
#include <QFileInfo>
#include <QSettings>

VideoControl::VideoControl(QObject *parent) : QObject(parent)
{

    qRegisterMetaType<STREAMTYPE>("STREAMTYPE");
}

VideoControl::~VideoControl()
{
#ifndef LUNUX_WIN
    vio.Venc_exit();
    vio.wait();
#endif

}

//STREAMTYPE VideoControl::mainStrean() const
//{
//    qDebug()<<"read stream";
//    return m_mainStream;
//}
void VideoControl::setStream(STREAMTYPE &stream)
{
    m_mainStream = stream;
}

//STREAMTYPE VideoControl::streanType() const
//{
//    return m_streamType;
//}
void VideoControl::setStreamType(STREAMTYPE &streamType)
{
    m_streamType = streamType;
}

PIC_SIZE_E VideoControl::vencSize() const
{
    return m_vencSize;
}
void VideoControl::setVencSize(PIC_SIZE_E &streamType)
{
    m_vencSize = streamType;
}

SAMPLE_RC_E VideoControl::enRcMode() const
{
    return m_enRcMode;
}
void VideoControl::setEnRcMode(SAMPLE_RC_E &enRcMode)
{
    m_enRcMode = enRcMode;
}

HI_U32 VideoControl::bitRate() const
{
    return m_u32BitRate;
}
void VideoControl::setBitRate(HI_U32 &bitRate)
{
    m_u32BitRate = bitRate;
}

HI_U32 VideoControl::profile() const
{
    return m_u32Profile;
}
void VideoControl::setProfile(HI_U32 &profile)
{
    m_u32Profile = profile;
}


HI_FR32 VideoControl::frmRate() const
{
    return m_fr32DstFrmRate;
}
void VideoControl::setFrmRate(HI_FR32 &frmRate)
{
    m_fr32DstFrmRate = frmRate;
}

//HI_BOOL VideoControl::Load_Config()
//{
//    QFileInfo fileinfo(VIO_CONFIG_PATH);
//    if(!fileinfo.exists())
//    {
//        m_enViMode = SAMPLE_VI_MODE_8_720P;
//        m_enNorm = VIDEO_ENCODING_MODE_PAL;
//        m_enType = PT_H265;
//        m_enSize = PIC_HD720;
//        m_enRcMode = SAMPLE_RC_VBR;
//        m_u32Profile = 0;
//        m_enVoMode = VO_MODE_9MUX;
//        QSettings *vioConfig = new QSettings(VIO_CONFIG_PATH, QSettings::IniFormat);
//        for(int i = 0 ;i < m_ViChnCnt;i++){
//            vioConfig->setValue("channel"+QString::number(i)+"/open", "true");
//            vioConfig->setValue("channel"+QString::number(i)+"/enVoMode", "VO_MODE_9MUX");
//            vioConfig->setValue("channel"+QString::number(i)+"/enViMode", "SAMPLE_VI_MODE_8_720P");
//            vioConfig->setValue("channel"+QString::number(i)+"/enNorm", "VIDEO_ENCODING_MODE_PAL");
//            vioConfig->setValue("channel"+QString::number(i)+"/enType", "PT_H265");
//            vioConfig->setValue("channel"+QString::number(i)+"/enSize", "PIC_HD720");
//            vioConfig->setValue("channel"+QString::number(i)+"/enRcMode", "SAMPLE_RC_CBR");
//            vioConfig->setValue("channel"+QString::number(i)+"/Profile", "low");
//        }
//        delete vioConfig;
//        return HI_FALSE;
//    }

//    QSettings *vioConfig = new QSettings(VIO_CONFIG_PATH, QSettings::IniFormat);
//    QString result;

//    for(int i = 0 ;i < m_ViChnCnt;i++){
//        result = vioConfig->value("channel"+QString::number(i)+"/open").toString();
//        if(result == "true"){
//            m_ChnOpenStatus.insert(i,true);
//        }else{
//            m_ChnOpenStatus.insert(i,false);
//        }

//        result = vioConfig->value("channel"+QString::number(i)+"/enVoMode").toString();
//        if(result == "VO_MODE_1MUX"){
//            m_enVoMode = VO_MODE_1MUX;
//        }else if(result == "VO_MODE_4MUX"){
//            m_enVoMode = VO_MODE_4MUX;
//        }else{
//            m_enVoMode = VO_MODE_9MUX;
//        }

//        result = vioConfig->value("channel"+QString::number(i)+"/enViMode").toString();
//        if(result == "SAMPLE_VI_MODE_8_1080P"){
//            m_enViMode = SAMPLE_VI_MODE_8_1080P;
//        }else{
//            m_enViMode = SAMPLE_VI_MODE_8_720P;
//        }
//        result = vioConfig->value("channel"+QString::number(i)+"/enNorm").toString();
//        if(result == "VIDEO_ENCODING_MODE_NTSC"){
//            m_enNorm = VIDEO_ENCODING_MODE_NTSC;
//        }else{
//            m_enNorm = VIDEO_ENCODING_MODE_PAL;
//        }
//        result = vioConfig->value("channel"+QString::number(i)+"/enType").toString();
//        if(result == "PT_H264"){
//            m_enType = PT_H264;
//        }else if(result == "PT_MJPEG"){
//            m_enType = PT_MJPEG;
//        }else{
//            m_enType = PT_H265;
//        }
//        result = vioConfig->value("channel"+QString::number(i)+"/enSize").toString();
//        if(result == "PIC_HD1080"){
//            m_enSize = PIC_HD1080;
//        }else{
//            m_enSize = PIC_HD720;
//        }
//        result = vioConfig->value("channel"+QString::number(i)+"/enRcMode").toString();
//        if(result == "SAMPLE_RC_FIXQP"){
//            m_enRcMode = SAMPLE_RC_FIXQP;
//        }else if(result == "SAMPLE_RC_AVBR"){
//            m_enRcMode = SAMPLE_RC_AVBR;
//        }else if(result == "SAMPLE_RC_VBR"){
//            m_enRcMode = SAMPLE_RC_VBR;
//        }else{
//            m_enRcMode = SAMPLE_RC_CBR;
//        }
//        result = vioConfig->value("channel"+QString::number(i)+"/Profile").toString();
//        if(result == "high" ){
//            m_u32Profile = 2;
//        }else if(result == "mid"){
//            m_u32Profile = 1;
//        }else{
//            m_u32Profile = 0;
//        }
//    }

//    delete vioConfig;

//    return HI_TRUE;
//}

HI_BOOL VideoControl::videoStart()
{
#ifndef LUNUX_WIN

    vio.Vi_Start(VIDEO_ENCODING_MODE_PAL);
    vio.Vo_Start();
    vio.Vi_Venc_Start();
    vio.start();
    vio.start_timer();

    QObject::connect(&vio,SIGNAL(VistatusChanged(VI_CHN)),&vio,SLOT(onChangeStatus(VI_CHN)));
    QObject::connect(&vio,SIGNAL(MakeNewFile(VI_CHN)),&vio,SLOT(onMakeNewFile(VI_CHN)));
#endif
    return HI_TRUE;
}


void VideoControl::onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode)
{
    //qDebug()<<"get vo_mode:"<<enVoMode<<endl;
#ifndef LUNUX_WIN
    return vio.onSet_VoMode(enVoMode);
#endif
}

void VideoControl::onDispChnToWin(QMap<VO_CHN, RECT_S> &ChnAttr)
{
#ifndef LUNUX_WIN
    return vio.onDispChnToWin(ChnAttr);
#endif

}

void VideoControl::onStopVoSlot()
{
#ifndef LUNUX_WIN
    return vio.onStopVoSlot();
#endif
}
