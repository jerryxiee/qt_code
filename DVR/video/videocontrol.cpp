#include "videocontrol.h"
#include <QDebug>
#include <QMetaType>
#include <QFileInfo>
#include <QSettings>

VideoControl::VideoControl(QObject *parent) : QObject(parent)
{

//    qRegisterMetaType<STREAMTYPE>("STREAMTYPE");
    loadVideoConfig();

    connect(this,SIGNAL(vencAttrChanged(VI_CHN,HI_U32)),this,SLOT(onVencAttrChangedSlot(VI_CHN,HI_U32)));

}

VideoControl::~VideoControl()
{
#ifndef LUNUX_WIN
    vio.Venc_exit();
    vio.wait();
#endif

}

QString VideoControl::vencToString(QString attr, QVariant &variant)
{
    QString ret;

    if(attr == "open"){
        if(variant.value<HI_BOOL>() == HI_TRUE){
            ret = "true";
        }else{
            ret = "false";
        }

    }else if(attr == "StreamType"){
        if(variant.value<STREAMTYPE>() == MAINTYPE){
            ret = "video";
        }else{
            ret = "complex";
        }
    }else if(attr == "enSize" ){
        if(variant.value<PIC_SIZE_E>() == PIC_CIF){
            ret = "PIC_CIF";
        }else{
            ret = "PIC_HD720";
        }

    }else if(attr == "enRcMode"){
        if(variant.value<SAMPLE_RC_E>() == SAMPLE_RC_FIXQP){
            ret = "SAMPLE_RC_FIXQP";
        }else if(variant.value<SAMPLE_RC_E>() == SAMPLE_RC_AVBR){
            ret = "SAMPLE_RC_AVBR";
        }else if(variant.value<SAMPLE_RC_E>() == SAMPLE_RC_VBR){
            ret = "SAMPLE_RC_VBR";
        }else{
            ret = "SAMPLE_RC_CBR";
        }

    }else if(attr == "Profile"){
        if(variant.value<HI_U32>() == 2){
            ret = "high" ;
        }else if(variant.value<HI_U32>() == 1){
            ret = "mid";
        }else{
            ret = "low";
        }
    }else if(attr == "FrameRate"){
        ret = QString::number(variant.value<HI_FR32>());
    }else if(attr == "BitRate"){
        ret = QString::number(variant.value<HI_U32>());
    }

    return ret;
}

QVariant VideoControl::stringToVenc(QString &str)
{
    QVariant ret;


    if(str == "false"){
        ret.setValue(HI_FALSE);
    }else if(str == "true"){
        ret.setValue(HI_TRUE);
    }else if(str == "complex" || str == "复合流" ){
        ret.setValue(CHILDTYPE);
    }else if(str == "video" || str == "视频流"){
        ret.setValue(MAINTYPE);
    }else if(str == "PIC_CIF" || str == "CIF"){
        ret.setValue(PIC_CIF);
    }else if(str == "PIC_HD720" || str == "720P"){
        ret.setValue(PIC_HD720);
//    }else if(str == "SAMPLE_RC_FIXQP"){
//        ret.setValue(SAMPLE_RC_FIXQP);
//    }else if(str == "SAMPLE_RC_AVBR"){
//        ret.setValue(SAMPLE_RC_AVBR);
    }else if(str == "SAMPLE_RC_VBR" || str == "变码率"){
        ret.setValue(SAMPLE_RC_VBR);
    }else if(str == "SAMPLE_RC_CBR" || str == "定码率"){
        ret.setValue(SAMPLE_RC_CBR);
    }else if(str == "高等" || str == "high"){
        ret.setValue(2);
    }else if(str == "中等"|| str == "mid"){
        ret.setValue(1);
    }else if(str == "低等"|| str == "low"){
        ret.setValue(0);
    }else if(str == "主码流"){
        ret.setValue(0);
    }else if(str == "子码流"){
        ret.setValue(1);
    }

    return ret;
}

void VideoControl::setVencAttr(QString Chn,QString mainStream,QString streamType,QString vencSize,QString enRcMode,
                               QString profile,QString dstFrmRate,QString bitRate)
{
    VI_CHN chn = Chn.right(1).toInt();
    HI_U32 stream = stringToVenc(mainStream).value<HI_U32>();
    HI_BOOL change = HI_FALSE;
    VDEC_PARAM attr = m_Vdec_Param[stream].at(chn);

    qDebug("set attr chn(%d) stream(%d)\n",chn,stream);
    if(stringToVenc(streamType).value<STREAMTYPE>() != attr.mstreamType ){
        attr.mstreamType = stringToVenc(streamType).value<STREAMTYPE>();
        change = HI_TRUE;
    }
    if(stringToVenc(vencSize).value<PIC_SIZE_E>() != attr.mvencSize){
        attr.mvencSize = stringToVenc(vencSize).value<PIC_SIZE_E>();
        change = HI_TRUE;
    }

    if(stringToVenc(enRcMode).value<SAMPLE_RC_E>() != attr.menRcMode){
        attr.menRcMode = stringToVenc(enRcMode).value<SAMPLE_RC_E>();
        change = HI_TRUE;
    }
    if(stringToVenc(profile).value<HI_U32>() != attr.mu32Profile){
        attr.mu32Profile = stringToVenc(profile).value<HI_U32>();
        change = HI_TRUE;
    }
    if(attr.mu32BitRate != bitRate.toUInt()){
        attr.mu32BitRate = bitRate.toUInt();
        change = HI_TRUE;
    }
    if(attr.mfr32DstFrmRate != dstFrmRate.toUInt()){
        attr.mfr32DstFrmRate = dstFrmRate.toUInt();
        change = HI_TRUE;
    }

    if(change){
        m_Vdec_Param[stream].replace(chn,attr);
        setVencConfig(chn,stream,attr);

    }


    qDebug()<<"Chn "+Chn<<endl
            <<"mainStream "+mainStream<<endl
            <<"streamType "+streamType<<endl
            <<"vencSize "+vencSize<<endl
            <<"enRcMode "+enRcMode<<endl
            <<"bitRate "+bitRate<<endl
            <<"profile "+profile<<endl
            <<"dstFrmRate "+dstFrmRate<<endl;
}
//STREAMTYPE VideoControl::mainStrean() const
//{
//    qDebug()<<"read stream";
//    return m_mainStream;
//}
//void VideoControl::setStream(STREAMTYPE &stream)
//{
//    m_mainStream = stream;
//}

////STREAMTYPE VideoControl::streanType() const
////{
////    return m_streamType;
////}
//void VideoControl::setStreamType(STREAMTYPE &streamType)
//{
//    m_streamType = streamType;
//}

//PIC_SIZE_E VideoControl::vencSize() const
//{
//    return m_vencSize;
//}
//void VideoControl::setVencSize(PIC_SIZE_E &streamType)
//{
//    m_vencSize = streamType;
//}

//SAMPLE_RC_E VideoControl::enRcMode() const
//{
//    return m_enRcMode;
//}
//void VideoControl::setEnRcMode(SAMPLE_RC_E &enRcMode)
//{
//    m_enRcMode = enRcMode;
//}

//HI_U32 VideoControl::bitRate() const
//{
//    return m_u32BitRate;
//}
//void VideoControl::setBitRate(HI_U32 &bitRate)
//{
//    m_u32BitRate = bitRate;
//}

//HI_U32 VideoControl::profile() const
//{
//    return m_u32Profile;
//}
//void VideoControl::setProfile(HI_U32 &profile)
//{
//    m_u32Profile = profile;
//}


//HI_FR32 VideoControl::frmRate() const
//{
//    return m_fr32DstFrmRate;
//}
//void VideoControl::setFrmRate(HI_FR32 &frmRate)
//{
//    m_fr32DstFrmRate = frmRate;
//}

HI_BOOL VideoControl::loadVideoConfig()
{
#ifndef LUNUX_WIN
    int vicnt = vio.m_ViChnCnt;
#else
    int vicnt = 8;
#endif

    VDEC_PARAM vencAttr;

    QFileInfo fileinfo(VIO_CONFIG_PATH);
    if(!fileinfo.exists())
    {
//        m_enViMode = SAMPLE_VI_MODE_8_720P;
//        m_enNorm = VIDEO_ENCODING_MODE_PAL;
//        m_enType = PT_H265;
//        m_enSize = PIC_HD720;
//        m_enRcMode = SAMPLE_RC_VBR;
//        m_u32Profile = 0;
//        m_enVoMode = VO_MODE_9MUX;
        QSettings *vioConfig = new QSettings(VIO_CONFIG_PATH, QSettings::IniFormat);
        for(HI_U32 n = 0;n < 2;n++){
            m_Vdec_Param[n].clear();
            for(int i = 0 ;i < vicnt;i++){

                vencAttr.mopen = HI_TRUE;
                vencAttr.mainStream =n;
                vencAttr.mstreamType = MAINTYPE;
                vencAttr.mvencSize = PIC_HD720;
                vencAttr.menRcMode = SAMPLE_RC_CBR;
                vencAttr.mu32BitRate = 0;
                vencAttr.mu32Profile = 0;
                vencAttr.mfr32DstFrmRate = 25;

                m_Vdec_Param[n].append(vencAttr);

                vioConfig->setValue("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/open", "true");
                vioConfig->setValue("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/StreamType", "video");
                vioConfig->setValue("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/enSize", "PIC_HD720");
                vioConfig->setValue("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/enRcMode", "SAMPLE_RC_CBR");
                vioConfig->setValue("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/Profile", "low");
                vioConfig->setValue("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/FrameRate", "25");
                vioConfig->setValue("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/BitRate", "0");
            }

        }
        delete vioConfig;
        return HI_FALSE;
    }

    QSettings *vioConfig = new QSettings(VIO_CONFIG_PATH, QSettings::IniFormat);
    QString result;
    for(HI_U32 n = 0;n < 2;n++){
        for(int i = 0 ;i < vicnt;i++){
            result = vioConfig->value("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/open").toString();
            if(result == "false"){
                vencAttr.mopen = HI_FALSE;
            }else{
                vencAttr.mopen = HI_TRUE;
            }

            result = vioConfig->value("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/StreamType").toString();
            if(result == "complex"){
                vencAttr.mstreamType = CHILDTYPE;
            }else{
                vencAttr.mstreamType = MAINTYPE;
            }

            result = vioConfig->value("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/enSize").toString();
            if(result == "PIC_CIF"){
                vencAttr.mvencSize = PIC_CIF;
            }else{
                vencAttr.mvencSize = PIC_HD720;
            }

            result = vioConfig->value("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/enRcMode").toString();
            if(result == "SAMPLE_RC_FIXQP"){
                vencAttr.menRcMode = SAMPLE_RC_FIXQP;
            }else if(result == "SAMPLE_RC_AVBR"){
                vencAttr.menRcMode = SAMPLE_RC_AVBR;
            }else if(result == "SAMPLE_RC_VBR"){
                vencAttr.menRcMode = SAMPLE_RC_VBR;
            }else{
                vencAttr.menRcMode = SAMPLE_RC_CBR;
            }
            result = vioConfig->value("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/Profile").toString();
            if(result == "high" ){
                vencAttr.mu32Profile = 2;
            }else if(result == "mid"){
                vencAttr.mu32Profile = 1;
            }else{
                vencAttr.mu32Profile = 0;
            }

            result = vioConfig->value("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/FrameRate").toString();
            vencAttr.mfr32DstFrmRate = result.toUInt();

            result = vioConfig->value("Channel"+QString::number(i)+"/stream"+QString::number(n)+"/BitRate").toString();
            vencAttr.mu32BitRate = result.toUInt();

            m_Vdec_Param[n].append(vencAttr);

        }
    }

    delete vioConfig;
//#endif

    return HI_TRUE;
}
/*******************
输入
    chn:通道
    stream:0，1
    str:读取配置参数
输出
    返回配置参数值
*********************/
QString VideoControl::readVencConfig(VI_CHN Chn,HI_U32 stream,QString str)
{
//#ifndef LUNUX_WIN
    QFileInfo fileinfo(VIO_CONFIG_PATH);
    if(!fileinfo.exists())
    {
        return "";
    }else{
        QSettings vioConfig(VIO_CONFIG_PATH, QSettings::IniFormat);
        return vioConfig.value("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/"+str).toString();

    }
//#endif

}

/*******************
输入
    chn:通道
    stream:0，1
    str:配置参数
    value:参数值

*********************/

void VideoControl::setVencConfig(VI_CHN Chn,HI_U32 stream,QString str,QString value)
{
    QSettings vioConfig(VIO_CONFIG_PATH, QSettings::IniFormat);
    vioConfig.setValue("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/"+str,value);
}

/*******************
输入
    chn:通道
    stream:0，1
    str:配置参数

*********************/

void VideoControl::setVencConfig(VI_CHN Chn,HI_U32 stream,VDEC_PARAM &str)
{
    QSettings *vioConfig = new QSettings(VIO_CONFIG_PATH, QSettings::IniFormat);
    QString result;

    if(str.mopen == HI_FALSE){
        result = "false";
    }else{
        result = "true";
    }
    vioConfig->setValue("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/open",result);

    if(str.mstreamType == CHILDTYPE){
        result = "complex";
    }else{
        result = "video";
    }
    vioConfig->setValue("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/StreamType",result);


    if(str.mvencSize == PIC_CIF){
        result = "PIC_CIF";
    }else{
        result = "PIC_HD720";
    }
    vioConfig->setValue("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/enSize",result);

    if(str.menRcMode == SAMPLE_RC_FIXQP){
        result = "SAMPLE_RC_FIXQP";

    }else if(str.menRcMode == SAMPLE_RC_AVBR){
        result = "SAMPLE_RC_AVBR";
    }else if(str.menRcMode == SAMPLE_RC_VBR){
        result = "SAMPLE_RC_VBR";
    }else{
        result = "SAMPLE_RC_CBR";
    }
    vioConfig->setValue("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/enRcMode",result);

    if(str.mu32Profile == 2){
        result = "high" ;
    }else if(str.mu32Profile == 1){
        result = "mid";
    }else{
        result = "low";
    }
    vioConfig->setValue("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/Profile",result);

    result = QString::number(str.mfr32DstFrmRate);
    vioConfig->setValue("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/FrameRate",result);


    result = QString::number(str.mu32BitRate);
    vioConfig->setValue("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/BitRate",result);

    emit vencAttrChanged(Chn,stream);
    delete vioConfig;
}
/*******************
输入
    chn:通道
    stream:0，1

输出
    str:读取配置参数
    成功返回true
*********************/
HI_BOOL VideoControl::readVencConfig(VI_CHN Chn,HI_U32 stream,VDEC_PARAM &str)
{
    QFileInfo fileinfo(VIO_CONFIG_PATH);
    if(!fileinfo.exists())
    {
        return HI_FALSE;
    }else {
        QSettings *vioConfig = new QSettings(VIO_CONFIG_PATH, QSettings::IniFormat);
        QString result;

        result = vioConfig->value("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/open").toString();
        if(result == "false"){
            str.mopen = HI_FALSE;
        }else{
            str.mopen = HI_TRUE;
        }

        result = vioConfig->value("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/StreamType").toString();
        if(result == "complex"){
            str.mstreamType = CHILDTYPE;
        }else{
            str.mstreamType = MAINTYPE;
        }

        result = vioConfig->value("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/enSize").toString();
        if(result == "PIC_CIF"){
            str.mvencSize = PIC_CIF;
        }else{
            str.mvencSize = PIC_HD720;
        }

        result = vioConfig->value("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/enRcMode").toString();
        if(result == "SAMPLE_RC_FIXQP"){
            str.menRcMode = SAMPLE_RC_FIXQP;
        }else if(result == "SAMPLE_RC_AVBR"){
            str.menRcMode = SAMPLE_RC_AVBR;
        }else if(result == "SAMPLE_RC_VBR"){
            str.menRcMode = SAMPLE_RC_VBR;
        }else{
            str.menRcMode = SAMPLE_RC_CBR;
        }
        result = vioConfig->value("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/Profile").toString();
        if(result == "high" ){
            str.mu32Profile = 2;
        }else if(result == "mid"){
            str.mu32Profile = 1;
        }else{
            str.mu32Profile = 0;
        }

        result = vioConfig->value("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/FrameRate").toString();
        str.mfr32DstFrmRate = result.toUInt();

        result = vioConfig->value("Channel"+QString::number(Chn)+"/stream"+QString::number(stream)+"/BitRate").toString();
        str.mu32BitRate = result.toUInt();

        delete vioConfig;
        return HI_TRUE;
    }

}

HI_BOOL VideoControl::videoStart()
{

    qDebug()<<"m_Vdec_Param[0].count()="<<m_Vdec_Param[0].count();
#ifndef LUNUX_WIN

    vio.Vi_Start(VIDEO_ENCODING_MODE_AUTO);
    vio.Vo_Start();
    for(int i = 0; i < m_Vdec_Param[0].count();i++){
        vio.Vi_Venc_Start(i,m_Vdec_Param[0][i].mvencSize,m_Vdec_Param[0][i].menRcMode,
                m_Vdec_Param[0][i].mu32BitRate,m_Vdec_Param[0][i].mfr32DstFrmRate,m_Vdec_Param[0][i].mu32Profile);
    }
//    vio.Vi_Venc_Start();
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

void VideoControl::onVencAttrChangedSlot(VI_CHN Chn,HI_U32 stream)
{
    qDebug()<<"onVencAttrChangedSlot";

    qDebug()<<"mvencSize"<<m_Vdec_Param[stream][Chn].mvencSize<<endl
            <<"menRcMode"<<m_Vdec_Param[stream][Chn].menRcMode<<endl
            <<"mu32BitRate"<<m_Vdec_Param[stream][Chn].mu32BitRate<<endl
            <<"mu32Profile"<<m_Vdec_Param[stream][Chn].mu32Profile<<endl;

    if(stream == 0){
#ifndef LUNUX_WIN
        vio.Set_VencAttr(Chn,m_Vdec_Param[stream][Chn].mvencSize,m_Vdec_Param[stream][Chn].menRcMode,
                     m_Vdec_Param[stream][Chn].mu32BitRate,m_Vdec_Param[stream][Chn].mfr32DstFrmRate,m_Vdec_Param[stream][Chn].mu32Profile);
#endif
    }
}
