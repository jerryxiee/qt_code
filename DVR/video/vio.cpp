#include "vio.h"
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QByteArray>
#include "SDL2/SDL.h"


Vio::Vio(QObject *parent):QObject(parent),
    m_ViChnCnt(VIDEO_MAX_NUM),m_enVoMode(VO_MODE_9MUX)
{
    Init();

    qDebug()<<"vio start";

}
Vio::Vio(VI_CHN ViChnCnt, SAMPLE_VO_MODE_E enVoMode):m_ViChnCnt(ViChnCnt),m_enVoMode(enVoMode)
{
    Init();
}

void Vio::Init()
{
    VO_DEV VoDev;
    VO_LAYER VoLayer;

    for(VI_CHN i = 0; i < m_ViChnCnt;i++){
        m_ViStatus.insert("channel"+QString::number(i),HI_FALSE);
    }

    Sample_Common_Sys::Get_Sys_VoDev(VoDev,VoLayer);
    m_Vo.SAMPLE_COMM_VO_SetDev(VoDev,VoLayer);

    connect(this,SIGNAL(VistatusChanged(VI_CHN,HI_BOOL)),this,SLOT(onChangeStatus(VI_CHN,HI_BOOL)));
//    connect(this,SIGNAL(MakeNewFile(VI_CHN)),this,SLOT(onMakeNewFile(VI_CHN)));

}

Vio::~Vio()
{
    HI_S32 i;

    m_Vi.SAMPLE_COMM_VI_UnBindVpss(enViMode,m_Vpss.m_Grp_Tab,m_ViChnCnt);
    for(i=0;i<m_ViChnCnt;i++){
        m_Vo.SAMPLE_COMM_VO_UnBindVpss(i,m_Vpss.m_Grp_Tab[i],m_VoBindVpss);
    }

//    m_Vpss.SAMPLE_COMM_VPSS_Stop();
//    delete m_pVpss;
    m_Vo.SAMPLE_COMM_VO_StopChn();
    m_Vo.SAMPLE_COMM_VO_StopLayer();
    m_Vi.SAMPLE_COMM_VI_Stop(enViMode);
//    delete m_ViDetect;

    qDebug()<<"exit Vio";
}


void Vio::onTimeHander()
{
    HI_S32 s32Ret;
    HI_S32 i;
    VI_CHN_LUM_S stLuma;
    HI_BOOL status;

//    QDateTime current_date_time =QDateTime::currentDateTime();
    mMutexTimeType.lock();
    QString current_date =QDateTime::currentDateTime().toString(mOverlayTimeType);
    if(!current_date.isEmpty()){
        QByteArray time = current_date.toLocal8Bit();
//        qDebug()<<mOverlayTimeType;
    //    qDebug("%s",time.data());
        mMutexTimeType.unlock();
        m_RegionCtr.SAMPLE_RGN_DispTextToOverlay(0,time.data());
    }

    for(i = 0; i < m_ViChnCnt;i++){
        s32Ret = m_Vi.SAMPLE_COMM_VI_GetChnLuma(enViMode,i, &stLuma);
        if (HI_SUCCESS != s32Ret){
            SAMPLE_PRT("HI_MPI_VI_GetChnLuma(%d) failed with %#x!\n", i,s32Ret);
        }
        status = m_ViStatus.value("channel"+QString::number(i));
        if(LUMCONST < stLuma.u32Lum){
            m_ViStatus["channel"+QString::number(i)] = HI_TRUE;
        }else{
            m_ViStatus["channel"+QString::number(i)] = HI_FALSE;
        }
        if(status != m_ViStatus.value("channel"+QString::number(i))){
            qDebug()<<"vi"<<i<<"status changed";
            emit VistatusChanged(i,m_ViStatus.value("channel"+QString::number(i)));
//            m_ViStatusChanged = HI_TRUE;
        }
    }

//    m_Record.checkFileSize();


}

void Vio::onChangeStatus(VI_CHN ViChn,HI_BOOL status)
{

    qDebug()<<"video "<<ViChn<<"changed";
//    if(m_ViStatus.value("channel"+QString::number(ViChn)) == HI_TRUE){
////        Venc_CreatNewFile(ViChn);
////        m_Record.addChnToRecord(m_pVenc[ViChn]->m_Venc_Chn);
//    }else{
////        m_Record.deleteChnFromRecord(m_pVenc[ViChn]->m_Venc_Chn);
////        Venc_Save_file_Stop(ViChn);
//    }

    onRgnOverlayShowSlot(ViChn);
//    m_RegionCtr.SAMPLE_RGN_ShowOverlay(TIMEHAND,m_Vpss.m_Grp_Tab[ViChn],showtime);
//    m_RegionCtr.SAMPLE_RGN_ShowOverlay(NAMEHAND + ViChn,m_Vpss.m_Grp_Tab[ViChn],showname);

}
//void Vio::onMakeNewFile(VI_CHN ViChn)
//{
//    m_Record.onCreatNewFileSlot(m_pVenc[ViChn]->m_Venc_Chn);
////    Venc_CreatNewFile(ViChn);
//}


HI_BOOL Vio::Vo_Start()
{
    HI_S32 s32Ret = HI_SUCCESS;
//    HI_U32 u32WndNum;
    HI_S32 i;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    memset(&(stLayerAttr), 0 , sizeof(VO_VIDEO_LAYER_ATTR_S));
    s32Ret = m_Vo.SAMPLE_COMM_VO_GetWH(Sample_Common_Sys::m_stPubAttr.enIntfSync, \
        &stLayerAttr.stImageSize.u32Width, \
        &stLayerAttr.stImageSize.u32Height, \
        &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_GetWH failed!\n");
        return HI_FALSE;
    }

    stLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    stLayerAttr.stDispRect.u32Width   = stLayerAttr.stImageSize.u32Width;
    stLayerAttr.stDispRect.u32Height  = stLayerAttr.stImageSize.u32Height;
    s32Ret = m_Vo.SAMPLE_COMM_VO_StartLayer(&stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartLayer failed!\n");
        goto END_1;
    }

    s32Ret = m_Vo.SAMPLE_COMM_VO_StartChn(m_enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_2;
    }

    for(i=0;i<m_ViChnCnt;i++)
    {
        s32Ret = m_Vo.SAMPLE_COMM_VO_BindVpss(i,m_Vpss.m_Grp_Tab[i],m_VoBindVpss);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_3;
        }
    }


    return HI_TRUE;

END_3:
    for(i=0;i<m_ViChnCnt;i++){
        m_Vo.SAMPLE_COMM_VO_UnBindVpss(i,m_Vpss.m_Grp_Tab[i],m_VoBindVpss);
    }
END_2:
    m_Vo.SAMPLE_COMM_VO_StopChn();
END_1:
    m_Vo.SAMPLE_COMM_VO_StopLayer();

    return HI_FALSE;
}

HI_BOOL Vio::Vo_Start(VO_CHN Vo_Chn, RECT_S &pos)
{

    if(m_Vo.SAMPLE_COMM_VO_StartChn(Vo_Chn,pos) < 0){
        return HI_FALSE;
    }
    return (m_Vo.SAMPLE_COMM_VO_BindVpss(Vo_Chn,m_Vpss.m_Grp_Tab[Vo_Chn],m_VoBindVpss)) < 0 ?HI_FALSE:HI_TRUE;
}

HI_BOOL Vio::Vo_Stop(VO_CHN Vo_Chn)
{
    HI_S32 i;

    if(Vo_Chn >= 0){
        m_Vo.SAMPLE_COMM_VO_UnBindVpss(Vo_Chn,m_Vpss.m_Grp_Tab[Vo_Chn],m_VoBindVpss);
        return m_Vo.SAMPLE_COMM_VO_StopChn(Vo_Chn) > 0 ? HI_TRUE:HI_TRUE;
    }
    for(i = 0; i < m_ViChnCnt; i++){
        m_Vo.SAMPLE_COMM_VO_UnBindVpss(i,m_Vpss.m_Grp_Tab[i],m_VoBindVpss);
    }

    m_Vo.SAMPLE_COMM_VO_StopChn();

//    m_Vio.SAMPLE_COMM_VO_StopLayer();

    return HI_TRUE;
}
HI_S32 Vio::Vo_SetMode(SAMPLE_VO_MODE_E enVoMode)
{
    return m_Vo.SAMPLE_COMM_VO_SetMode(enVoMode);
}

void Vio::onStopVoSlot()
{
    Vo_Stop(-1);
    qDebug()<<"stop vo";
}


void Vio::onOverlayTimeTypeChanged(QString type)
{
    qDebug()<<"TimeType:"<<type;
    mMutexTimeType.lock();
    if(!type.isEmpty())
        mOverlayTimeType = type;
    mMutexTimeType.unlock();
}

void Vio::onMoveTimePosChanged(int Chn, QPoint point)
{
    POINT_S stPoint;

    qDebug()<<"time pos change";
    stPoint.s32X = point.x();
    stPoint.s32Y = point.y();
    m_RegionCtr.SAMPLE_RGN_SetOverlayPosToVpss(TIMEHAND,m_Vpss.m_Grp_Tab[Chn],stPoint);
}

void Vio::onMoveNamePosChanged(int Chn, QPoint point)
{
    POINT_S stPoint;

    qDebug()<<"time pos change";
    stPoint.s32X = point.x();
    stPoint.s32Y = point.y();
    m_RegionCtr.SAMPLE_RGN_SetOverlayPosToVpss(NAMEHAND+Chn,m_Vpss.m_Grp_Tab[Chn],stPoint);
}

void Vio::onRgnOverlayShowSlot(int Chn)
{
    HI_BOOL showname = HI_FALSE;
    HI_BOOL showtime = HI_FALSE;

    if(Settings::getDispSetIni()->getNameShowEnabled(Chn) && m_ViStatus.value("channel"+QString::number(Chn))){
        showname = HI_TRUE;
    }
    if(Settings::getDispSetIni()->getTimeShowEnabled(Chn) && m_ViStatus.value("channel"+QString::number(Chn))){
        showtime = HI_TRUE;
    }

    m_RegionCtr.SAMPLE_RGN_ShowOverlay(TIMEHAND,m_Vpss.m_Grp_Tab[Chn],showtime);
    m_RegionCtr.SAMPLE_RGN_ShowOverlay(NAMEHAND + Chn,m_Vpss.m_Grp_Tab[Chn],showname);
}

void Vio::onOverlayNameChanged(int Chn, QString name)
{
    if(!name.isEmpty()){
        qDebug()<<"name change:"<<name;
        QByteArray namestr = name.toLocal8Bit();
        m_RegionCtr.SAMPLE_RGN_DispTextToOverlay(NAMEHAND+Chn,namestr.data());
    }
}

HI_S32 Vio::OverlayInit()
{
    HI_S32 s32Ret = HI_SUCCESS;
    VI_CHN i;
    RGN_ATTR_S stRgnAttrSet;
    RGN_CHN_ATTR_S stChnAttr;
    SDL_Color textColor = {0x80,0x0,0xff,0xff};
    QDateTime current_date_time;
    QString current_date;
    QByteArray time;
//    SIZE_S overlaysize = {0,0};
    QString str;
    DispSet *OverlayDisp = Settings::getDispSetIni();
    QRect rgnRect;
//    QRect nameRect;

    connect(OverlayDisp,SIGNAL(overlayTimeTypeChange(QString)),this,SLOT(onOverlayTimeTypeChanged(QString)));
    connect(OverlayDisp,SIGNAL(overlayNameChange(int, QString)),this,SLOT(onOverlayNameChanged(int,QString)));
    connect(OverlayDisp,SIGNAL(rgnOverlayShow(int)),this,SLOT(onRgnOverlayShowSlot(int)));

    str = OverlayDisp->getConfig(OverlayDisp->RootName+QString::number(0),OverlayDisp->DateType).toString();
    if(str.length() >= strlen("yyyy-MM-dd hh:mm:ss")){
        if(OverlayDisp->getConfig(OverlayDisp->RootName+QString::number(0),OverlayDisp->DispWeek).toBool())
            mOverlayTimeType = "ddd ";
        if(OverlayDisp->getConfig(OverlayDisp->RootName+QString::number(0),OverlayDisp->DispDate).toBool()){
            mOverlayTimeType.append(str);
            if(OverlayDisp->getConfig(OverlayDisp->RootName+QString::number(0),OverlayDisp->DateType).toInt() == 1){
                mOverlayTimeType.append(" AP");
            }
        }
    }

    qDebug()<<mOverlayTimeType;


    s32Ret = m_RegionCtr.SAMPLE_RGN_DispTextInit("/opt/Qt5.9.8-Arm/lib/fonts/my.ttf",32,textColor,m_OverlayPixFmt);
    if(s32Ret == HI_SUCCESS){
        current_date =QDateTime::currentDateTime().toString(mOverlayTimeType);
        time = current_date.toLocal8Bit();
//        m_RegionCtr.SAMPLE_RGN_GetFontSize(time.data(),overlaysize);

//        qDebug("w:%d h:%d\n",overlaysize.u32Width,overlaysize.u32Height);
        stRgnAttrSet.enType = OVERLAY_RGN;
        stRgnAttrSet.unAttr.stOverlay.enPixelFmt       = m_OverlayPixFmt;//PIXEL_FORMAT_RGB_1555;
        stRgnAttrSet.unAttr.stOverlay.stSize.u32Width  = ALIGN_UP(OVERLAYRGN_TIMEW,2);
        stRgnAttrSet.unAttr.stOverlay.stSize.u32Height = ALIGN_UP(OVERLAYRGN_TIMEH,2);
        stRgnAttrSet.unAttr.stOverlay.u32BgColor       = 0x0;

        stChnAttr.bShow  = m_ViStatus.value("channel0");
        stChnAttr.enType = OVERLAY_RGN;
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 100;
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 50;
        stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 255;
        stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = 255;
        stChnAttr.unChnAttr.stOverlayChn.u32Layer     = 0;

        //display time
        s32Ret = m_RegionCtr.SAMPLE_RGN_CreateRegion(TIMEHAND,&stRgnAttrSet);
//        s32Ret = m_RegionCtr.SAMPLE_RGN_CreateRegionForVpss(0,0,TIMEHAND,&stRgnAttrSet,&stChnAttr);
        if(s32Ret == HI_SUCCESS){
            for (i = 0;i < m_ViChnCnt;i++) {
                rgnRect = OverlayDisp->getConfig(OverlayDisp->RootName+QString::number(i),OverlayDisp->TimePos).toRect();
                if(rgnRect.isEmpty()){
                    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = OVERLAYRGN_TIMEPOSX;
                    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = OVERLAYRGN_TIMEPOSY;

                    rgnRect.setRect(OVERLAYRGN_TIMEPOSX,OVERLAYRGN_TIMEPOSY,stRgnAttrSet.unAttr.stOverlay.stSize.u32Width,stRgnAttrSet.unAttr.stOverlay.stSize.u32Height);
                    OverlayDisp->setConfig(OverlayDisp->RootName+QString::number(i),OverlayDisp->TimePos,rgnRect);
                }else{
                    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = ALIGN_UP(rgnRect.x(),2);
                    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = ALIGN_UP(rgnRect.y(),2);

                }
                stChnAttr.bShow = m_ViStatus.value("channel"+QString::number(i));
                m_RegionCtr.SAMPLE_RGN_OverlayAttachToChn(TIMEHAND,m_Vpss.m_Grp_Tab[i],&stChnAttr);
            }
            m_RegionCtr.SAMPLE_RGN_DispTextToOverlay(TIMEHAND,time.data());
        }else{
            qDebug()<<"time SAMPLE_RGN_CreateRegion failed";
        }



        //display name
//        m_RegionCtr.SAMPLE_RGN_GetFontSize("aaaaaaaaaa",overlaysize);
        stRgnAttrSet.unAttr.stOverlay.stSize.u32Width  = ALIGN_UP(OVERLAYRGN_NAMEW,2);
        stRgnAttrSet.unAttr.stOverlay.stSize.u32Height = ALIGN_UP(OVERLAYRGN_NAMEH,2);

        stChnAttr.unChnAttr.stOverlayChn.u32Layer     = 1;

        for(i = 0;i < m_ViChnCnt;i++){
            s32Ret = m_RegionCtr.SAMPLE_RGN_CreateRegion(NAMEHAND+i,&stRgnAttrSet);
            if(s32Ret == HI_SUCCESS){
                rgnRect = OverlayDisp->getConfig(OverlayDisp->RootName+QString::number(i),OverlayDisp->NamePos).toRect();
//                qDebug()<<"rgnRect:"<<rgnRect;
                if(rgnRect.isEmpty()){
                    qDebug("rgnRect isEmpty");
                    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = OVERLAYRGN_NAMEPOSX;
                    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = OVERLAYRGN_NAMEPOSY;


                    rgnRect.setRect(OVERLAYRGN_NAMEPOSX,OVERLAYRGN_NAMEPOSY,
                                    OVERLAYRGN_NAMEW,OVERLAYRGN_NAMEH);
                    OverlayDisp->setConfig(OverlayDisp->RootName+QString::number(i),OverlayDisp->NamePos,rgnRect);
                }else {
                    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = ALIGN_UP(rgnRect.x(),2);
                    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = ALIGN_UP(rgnRect.y(),2);

                }
                m_RegionCtr.SAMPLE_RGN_OverlayAttachToChn(NAMEHAND+i,m_Vpss.m_Grp_Tab[i],&stChnAttr);
                str = OverlayDisp->getConfig(OverlayDisp->RootName+QString::number(i),OverlayDisp->ChnName).toString();
                if(str.isEmpty()){
                    str = "video"+QString::number(i);
                }
                QByteArray namestr = str.toLocal8Bit();
                m_RegionCtr.SAMPLE_RGN_DispTextToOverlay(NAMEHAND+i,namestr.data());
            }else {
                qDebug()<<"name SAMPLE_RGN_CreateRegion failed";
            }
        }


    }


    return s32Ret;

}


HI_BOOL Vio::Vi_Start(VIDEO_NORM_E enNorm,Sample_Common_Vpss *vpss)
{
    HI_S32 s32Ret = HI_SUCCESS;

    m_enNorm = enNorm;
    m_Vpss = vpss;

    m_OverlayPixFmt = PIXEL_FORMAT_RGB_8888;
    /*** Start AD ***/
    s32Ret = m_Vi.SAMPLE_COMM_VI_ADStart(enViMode, enNorm);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("Start AD failed!\n");
        return HI_FALSE;
    }

    s32Ret = m_Vi.SAMPLE_COMM_VI_Start(enViMode, enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_VI_START;
    }

    s32Ret = m_Vi.SAMPLE_COMM_VI_BindVpss(enViMode,m_Vpss.m_Grp_Tab,m_ViChnCnt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_VI_START;
    }

    OverlayInit();

    return HI_TRUE;

END_VI_START:
    m_Vi.SAMPLE_COMM_VI_Stop(enViMode);

    return HI_FALSE;
}

void Vio::onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode)
{
    //qDebug()<<"get vo_mode:"<<enVoMode<<endl;
    Vo_SetMode(enVoMode);
}

void Vio::onDispChnToWin(QMap<VO_CHN,RECT_S> &ChnAttr)
{
    QMap<VO_CHN,RECT_S>::iterator i;

    Vo_Stop(-1);
    for(i = ChnAttr.begin();i != ChnAttr.end();i++){
        Vo_Start(i.key(),i.value());
        qDebug()<<"channel:"<<i.key();
    }


}

