#include "videocontrol.h"
#include <QDebug>
#include <QMetaType>
#include <QFileInfo>
#include <QSettings>

VideoControl::VideoControl(QObject *parent) : QObject(parent)
{

//    qRegisterMetaType<STREAMTYPE>("STREAMTYPE");
//    loadVideoConfig();
    qDebug()<<"start videocontrol";

    m_VencSet = Settings::getVencIni();

    connect(m_VencSet,SIGNAL(vencAttrChanged(VI_CHN,HI_U32)),this,SLOT(onVencAttrChangedSlot(VI_CHN,HI_U32)));

}

VideoControl::~VideoControl()
{
#ifndef LUNUX_WIN
    vio.Venc_exit();
    vio.wait();
#endif

}

HI_BOOL VideoControl::videoStart()
{

//    qDebug()<<"m_Vdec_Param[0].count()="<<m_Vdec_Param[0].count();
#ifndef LUNUX_WIN
    SIZE_S stSize = {1280,720};
    VPSS_GRP_ATTR_S stGrpAttr;


    memset(&stGrpAttr,0,sizeof(VPSS_GRP_ATTR_S));
    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    m_pVpss = new Sample_Common_Vpss(vio.m_ViChnCnt,VPSS_MAX_CHN_NUM,&stSize,&stGrpAttr);
    if(!m_pVpss){
        goto END_VPSS_START;
    }
    if(HI_FALSE == m_pVpss->SAMPLE_COMM_VPSS_CreatIsSucess()){
        goto END_VPSS_START;
    }

    vio.Vi_Start(VIDEO_ENCODING_MODE_AUTO,m_pVpss);
    vio.Vo_Start();
    for(int i = 0; i < m_VencSet->m_Vdec_Param[0].count();i++){
        vio.Vi_Venc_Start(i,m_VencSet->m_Vdec_Param[0][i].mvencSize,m_VencSet->m_Vdec_Param[0][i].menRcMode,
                m_VencSet->m_Vdec_Param[0][i].mu32BitRate,m_VencSet->m_Vdec_Param[0][i].mfr32DstFrmRate,m_VencSet->m_Vdec_Param[0][i].mu32Profile);
    }
//    vio.Vi_Venc_Start();
    vio.start();
    vio.start_timer();

    QObject::connect(&vio,SIGNAL(VistatusChanged(VI_CHN)),&vio,SLOT(onChangeStatus(VI_CHN)));
    QObject::connect(&vio,SIGNAL(MakeNewFile(VI_CHN)),&vio,SLOT(onMakeNewFile(VI_CHN)));


#endif
    return HI_TRUE;

#ifndef LUNUX_WIN
END_VPSS_START:
    m_pVpss->SAMPLE_COMM_VPSS_Stop();
    return HI_FALSE;
#endif
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

    qDebug()<<"mvencSize"<<m_VencSet->m_Vdec_Param[stream][Chn].mvencSize<<endl
            <<"menRcMode"<<m_VencSet->m_Vdec_Param[stream][Chn].menRcMode<<endl
            <<"mu32BitRate"<<m_VencSet->m_Vdec_Param[stream][Chn].mu32BitRate<<endl
            <<"mu32Profile"<<m_VencSet->m_Vdec_Param[stream][Chn].mu32Profile<<endl;

    if(stream == 0){
#ifndef LUNUX_WIN
        vio.Set_VencAttr(Chn,m_VencSet->m_Vdec_Param[stream][Chn].mvencSize,m_VencSet->m_Vdec_Param[stream][Chn].menRcMode,
                     m_VencSet->m_Vdec_Param[stream][Chn].mu32BitRate,m_VencSet->m_Vdec_Param[stream][Chn].mfr32DstFrmRate,
                         m_VencSet->m_Vdec_Param[stream][Chn].mu32Profile);
#endif
    }
}
