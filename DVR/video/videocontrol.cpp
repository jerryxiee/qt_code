#include "videocontrol.h"
#include <QDebug>
#include <QMetaType>
#include <QFileInfo>
#include <QSettings>
#include <QMetaType>

Sample_Common_Vpss *VideoControl::m_pVpss = nullptr;

VideoControl::VideoControl(QObject *parent) : QObject(parent)
{
    qDebug()<<"start videocontrol";
    mTimer = new QTimer(this);

#ifndef LUNUX_WIN
    connect(mTimer,SIGNAL(timeout()),&vio,SLOT(onTimeHander()));
//    connect(mTimer,SIGNAL(timeout()),&m_Record,SLOT(onTimeHander()));   //定时任务
#endif

    qRegisterMetaType<VI_CHN>("VI_CHN");
    qRegisterMetaType<VIDEO_TYPE>("VIDEO_TYPE");


}

VideoControl::~VideoControl()
{
#ifndef LUNUX_WIN
//    m_Record.RecordExit();
    mRecordMp4->RecordExit();
    m_VideoDetect.VideoDetectExit();
    m_pVpss->SAMPLE_COMM_VPSS_Stop();
//    delete m_pVpss;
    qDebug()<<"exit videocontrol";
#endif

}

Sample_Common_Vpss *VideoControl::getVpss()
{
#ifndef LUNUX_WIN
    return m_pVpss;
#else
    return nullptr;
#endif
}

VPSS_GRP VideoControl::getVpssGrp(VI_CHN Chn)
{
#ifndef LUNUX_WIN
    if(Chn >= m_pVpss->m_Grp_Num){
        qDebug()<<"can not get vpss for Chn:"<<Chn;
        return -1;
    }

    return m_pVpss->m_Grp_Tab[Chn];
#else
    return -1;
#endif

}

HI_BOOL VideoControl::videoStart()
{

//    qDebug()<<"m_Vdec_Param[0].count()="<<m_Vdec_Param[0].count();
#ifndef LUNUX_WIN
    SIZE_S stSize = {1280,720};
    VPSS_GRP_ATTR_S stGrpAttr;
    VPSS_CHN_MODE_S stVpssMode;
    HI_S32 s32Ret;


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

    vio.Vi_Start(VIDEO_NORM,m_pVpss);

//    m_Record.setRecordSrc(m_pVpss);
    mRecordMp4 = new RecordToMP4(*m_pVpss);
    for (int i = 0;i < vio.m_ViChnCnt;i++) {
//        m_Record.startRecordChn(i,VIDEO_NORM);
        mRecordMp4->startRecordChn(i,VIDEO_NORM);
        m_VideoDetect.createMoveDetect(i,i*4,400,4);

        stVpssMode.enChnMode = VPSS_CHN_MODE_USER;
        stVpssMode.u32Width = stSize.u32Width;
        stVpssMode.u32Height = stSize.u32Height;
        stVpssMode.enPixelFormat = SAMPLE_PIXEL_FORMAT;
        s32Ret = m_pVpss->SAMPLE_COMM_VPSS_SetChnMod(i,VPSS_CHN1,&stVpssMode,6);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SetChnMod failed!\n");
        }

    }
    mTimer->start(TIMEOUT);
    vio.Vo_Start();

//    connect(&vio,SIGNAL(VistatusChanged(VI_CHN,HI_BOOL)),&m_Record,SLOT(onViStatusChangedSlot(VI_CHN,HI_BOOL)));
    connect(mTimer,SIGNAL(timeout()),mRecordMp4,SLOT(onTimeHander()));
    connect(&vio,SIGNAL(VistatusChanged(VI_CHN,HI_BOOL)),mRecordMp4,SLOT(onViStatusChangedSlot(VI_CHN,HI_BOOL)));
    connect(this,SIGNAL(timePosChanged(int,QPoint)),&vio,SLOT(onMoveTimePosChanged(int,QPoint)));
    connect(this,SIGNAL(namePosChanged(int,QPoint)),&vio,SLOT(onMoveNamePosChanged(int,QPoint)));
    connect(&vio,SIGNAL(VistatusChanged(VI_CHN,HI_BOOL)),&m_VideoDetect,SLOT(onViStatusChangedSlot(VI_CHN,HI_BOOL)));
    //移动检测录像
//    connect(&m_VideoDetect,SIGNAL(videoMoveDetectChangeSignal(VI_CHN,VIDEO_TYPE,bool)),&m_Record,SLOT(onVideoAlarmEventChangedSlot(VI_CHN,VIDEO_TYPE,bool)));
    connect(&m_VideoDetect,SIGNAL(videoMoveDetectChangeSignal(VI_CHN,VIDEO_TYPE,bool)),mRecordMp4,SLOT(onVideoAlarmEventChangedSlot(VI_CHN,VIDEO_TYPE,bool)));

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
    //报警测试
//    m_Record.onVideoAlarmEventChangedSlot(Chn,type,change);
#endif
}
