#include "videocontrol.h"
#include <QDebug>
#include <QMetaType>
#include <QFileInfo>
#include <QSettings>

VideoControl::VideoControl(QObject *parent) : QObject(parent)
{
    qDebug()<<"start videocontrol";
    mTimer = new QTimer(this);

#ifndef LUNUX_WIN
    connect(mTimer,SIGNAL(timeout()),&vio,SLOT(onTimeHander()));
    connect(mTimer,SIGNAL(timeout()),&m_Record,SLOT(onTimeHander()));
#endif


}

VideoControl::~VideoControl()
{
#ifndef LUNUX_WIN
    m_Record.RecordExit();
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

    vio.Vi_Start(VIDEO_ENCODING_MODE_PAL,m_pVpss);
    m_Record.setRecordSrc(m_pVpss);
    for (int i = 0;i < vio.m_ViChnCnt;i++) {
        m_Record.startRecordChn(i,VIDEO_ENCODING_MODE_PAL);
    }
    mTimer->start(TIMEOUT);
    vio.Vo_Start();

    connect(&vio,SIGNAL(VistatusChanged(VI_CHN,HI_BOOL)),&m_Record,SLOT(onViStatusChangedSlot(VI_CHN,HI_BOOL)));
    connect(this,SIGNAL(timePosChanged(int,QPoint)),&vio,SLOT(onMoveTimePosChanged(int,QPoint)));
    connect(this,SIGNAL(namePosChanged(int,QPoint)),&vio,SLOT(onMoveNamePosChanged(int,QPoint)));


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


void VideoControl::onOverlayTimePosChanged(int Chn,QPoint point)
{
    emit timePosChanged(Chn,point);
}

void VideoControl::onOverlayNamePosChanged(int Chn,QPoint point)
{
    emit namePosChanged(Chn,point);
}

void VideoControl::onRgnOverlayShowSlot(int Chn,bool enable)
{
#ifndef LUNUX_WIN
//    vio.onRgnOverlayShowSlot(Chn,enable);
#endif

}

void VideoControl::onTestVideoAlarmSlot(VI_CHN Chn,VIDEO_TYPE type,bool change)
{
#ifndef LUNUX_WIN
    m_Record.onVideoAlarmEventChangedSlot(Chn,type,change);
#endif
}
