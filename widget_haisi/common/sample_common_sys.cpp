#include "sample_common_sys.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/time.h>

#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>
#include "hifb.h"

VO_DEV Sample_Common_Sys::m_FbVoDev = SAMPLE_VO_DEV_DHD0;
VO_DEV Sample_Common_Sys::m_FbVoLayer = 0;
VO_PUB_ATTR_S Sample_Common_Sys::m_stPubAttr = {0};




#if 0
HI_S32 Sample_Common_Sys::SAMPLE_VDEC_H264(HI_VOID)
{
    VB_CONF_S stVbConf, stModVbConf;
    HI_S32 i, s32Ret = HI_SUCCESS;
    VDEC_CHN_ATTR_S stVdecChnAttr[VDEC_MAX_CHN_NUM];
    VdecThreadParam stVdecSend[VDEC_MAX_CHN_NUM];
    VPSS_GRP_ATTR_S stVpssGrpAttr[VDEC_MAX_CHN_NUM];
    SIZE_S stSize, stRotateSize;
    VO_DEV VoDev;
    VO_LAYER VoLayer;
    VO_PUB_ATTR_S stVoPubAttr;
    VO_VIDEO_LAYER_ATTR_S stVoLayerAttr;
    HI_U32 u32VdCnt = 1, u32GrpCnt = 1;
    pthread_t   VdecThread[2*VDEC_MAX_CHN_NUM];

    stSize.u32Width = HD_WIDTH;
    stSize.u32Height = HD_HEIGHT;

    /************************************************
    step1:  init SYS and common VB
    *************************************************/
    //SAMPLE_COMM_VDEC_Sysconf(&stVbConf, &stSize);
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("init sys fail for %#x!\n", s32Ret);
        goto END1;
    }

    /************************************************
    step2:  init mod common VB
    *************************************************/
    SAMPLE_COMM_VDEC_ModCommPoolConf(&stModVbConf, PT_H264, &stSize, u32VdCnt, HI_FALSE);
    s32Ret = SAMPLE_COMM_VDEC_InitModCommVb(&stModVbConf);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("init mod common vb fail for %#x!\n", s32Ret);
        goto END1;
    }

    /************************************************
    step3:  start VDEC
    *************************************************/
    SAMPLE_COMM_VDEC_ChnAttr(u32VdCnt, &stVdecChnAttr[0], PT_H264, &stSize);
    s32Ret = SAMPLE_COMM_VDEC_Start(u32VdCnt, &stVdecChnAttr[0],nullptr);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VDEC fail for %#x!\n", s32Ret);
        goto END2;
    }

    /************************************************
    step4:  start VPSS
    *************************************************/
    stRotateSize.u32Width = stRotateSize.u32Height = MAX2(stSize.u32Width, stSize.u32Height);
    SAMPLE_COMM_VDEC_VpssGrpAttr(u32GrpCnt, &stVpssGrpAttr[0], &stRotateSize);
    s32Ret = SAMPLE_COMM_VPSS_Start(u32GrpCnt, &stRotateSize, 1, &stVpssGrpAttr[0]);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VPSS fail for %#x!\n", s32Ret);
        goto END3;
    }
    /************************************************
    step5:  start VO
    *************************************************/
    VoDev = SAMPLE_VO_DEV_DHD0;
    VoLayer = SAMPLE_VO_DEV_DHD0;

#if HI_FPGA
    stVoPubAttr.enIntfSync = VO_OUTPUT_1080P30;
    stVoPubAttr.enIntfType = VO_INTF_VGA;
#else
    stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
    stVoPubAttr.enIntfType = VO_INTF_VGA | VO_INTF_HDMI;
#endif
    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("vdec bind vpss fail for %#x!\n", s32Ret);
        goto END4_1;
    }

#ifndef HI_FPGA
    if (HI_SUCCESS != m_Sys_Vo.SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
        goto END4_2;
    }
#endif

    stVoLayerAttr.bClusterMode = HI_FALSE;
    stVoLayerAttr.bDoubleFrame = HI_FALSE;
    stVoLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;

    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync, \
        &stVoLayerAttr.stDispRect.u32Width, &stVoLayerAttr.stDispRect.u32Height, &stVoLayerAttr.u32DispFrmRt);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto  END4_2;
    }
    stVoLayerAttr.stImageSize.u32Width = stVoLayerAttr.stDispRect.u32Width;
    stVoLayerAttr.stImageSize.u32Height = stVoLayerAttr.stDispRect.u32Height;
    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartLayer(VoLayer, &stVoLayerAttr);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartLayer fail for %#x!\n", s32Ret);
        goto END4_3;
    }

    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartChn(VoLayer, VO_MODE_1MUX);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("vdec bind vpss fail for %#x!\n", s32Ret);
        goto END4_4;
    }

    /************************************************
    step6:  VDEC bind VPSS
    *************************************************/
    for(i=0; i<u32GrpCnt; i++)
    {
        s32Ret = SAMPLE_COMM_VDEC_BindVpss(i, i);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("vdec bind vpss fail for %#x!\n", s32Ret);
            goto END5;
        }
    }

    /************************************************
    step7:  VPSS bind VO
    *************************************************/
    for(i=0; i<u32GrpCnt; i++)
    {
        s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_BindVpss(VoLayer, i, i, VPSS_CHN0);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("vpss bind vo fail for %#x!\n", s32Ret);
            goto END6;
        }
    }

    /************************************************
    step8:  send stream to VDEC
    *************************************************/
    SAMPLE_COMM_VDEC_ThreadParam(u32VdCnt, &stVdecSend[0], &stVdecChnAttr[0], SAMPLE_1080P_H264_PATH);
    SAMPLE_COMM_VDEC_StartSendStream(u32VdCnt, &stVdecSend[0], &VdecThread[0]);

    /***  get the stat info of luma pix  ***/
    SAMPLE_COMM_VDEC_StartGetLuma(u32VdCnt, &stVdecSend[0], &VdecThread[0]);

    /***  set the rotational angle of decode pic  ***/

    /***  control the send stream thread and get luma info thread  ***/
    SAMPLE_COMM_VDEC_CmdCtrl(u32VdCnt, &stVdecSend[0]);

    SAMPLE_COMM_VDEC_StopSendStream(u32VdCnt, &stVdecSend[0], &VdecThread[0]);

    SAMPLE_COMM_VDEC_StopGetLuma(u32VdCnt, &stVdecSend[0], &VdecThread[0]);

END6:
    for(i=0; i<u32GrpCnt; i++)
    {
        s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_UnBindVpss(VoLayer, i, i, VPSS_CHN0);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("vpss unbind vo fail for %#x!\n", s32Ret);
        }
    }

END5:
    for(i=0; i<u32GrpCnt; i++)
    {
        s32Ret = SAMPLE_COMM_VDEC_UnBindVpss(i, i);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("vdec unbind vpss fail for %#x!\n", s32Ret);
        }
    }

END4_4:
    m_Sys_Vo.SAMPLE_COMM_VO_StopChn(VoLayer, VO_MODE_4MUX);
END4_3:
    m_Sys_Vo.SAMPLE_COMM_VO_StopLayer(VoLayer);
END4_2:
#ifndef HI_FPGA
    m_Sys_Vo.SAMPLE_COMM_VO_HdmiStop();
#endif
END4_1:
    m_Sys_Vo.SAMPLE_COMM_VO_StopDev(VoDev);

END3:
    SAMPLE_COMM_VPSS_Stop(u32GrpCnt, VPSS_CHN0);

END2:
    SAMPLE_COMM_VDEC_Stop(u32VdCnt);

END1:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

#endif


Sample_Common_Sys::Sample_Common_Sys(QObject *parent):QObject(parent)
{
#if 1
    HI_S32 s32Ret = HI_SUCCESS;
//    HI_U32 i;
    SIZE_S  stSize;

    VB_CONF_S       stVbConf;
    struct fb_var_screeninfo stVarInfo;
    HI_CHAR file[12] = "/dev/fb0";
    HI_BOOL bEnable;
    HIFB_DDRZONE_S stDDRZonePara;
    HIFB_LAYER_INFO_S stLayerinfo;

    stSize.u32Width = WIDTH;
    stSize.u32Height = HEIGHT;


    qDebug("Sample_Common_Sys init enter\n");
    /******************************************
     step  1: init variable
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));
    /******************************************
     step 2: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }

    /******************************************
     step 3:  start vo hd0.
    *****************************************/
    s32Ret = HI_MPI_VO_UnBindGraphicLayer(GRAPHICS_LAYER_HC0, m_FbVoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("UnBindGraphicLayer failed with 0x%x!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }

    s32Ret = HI_MPI_VO_BindGraphicLayer(GRAPHICS_LAYER_HC0, m_FbVoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("BindGraphicLayer failed with 0x%x!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_0;
    }
    m_stPubAttr.enIntfSync = VO_OUTPUT_720P50;
    m_stPubAttr.enIntfType = VO_INTF_VGA;
    m_stPubAttr.u32BgColor = 0xFF;

    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartDev(&m_stPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vo dev failed with %d!\n", s32Ret);
        goto SAMPLE_HIFB_NoneBufMode_1;
    }

    if (m_stPubAttr.enIntfType & VO_INTF_HDMI)
    {

#ifndef HI_FPGA
        s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_HdmiStart(m_stPubAttr.enIntfSync);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("start HDMI failed with %d!\n", s32Ret);
            goto SAMPLE_HIFB_NoneBufMode_2;
        }
#endif
    }


    /* 1. open framebuffer device overlay 0 */
    m_sys_hifb_fd = open(file, O_RDWR, 0);
    if(m_sys_hifb_fd < 0)
    {
        SAMPLE_PRT("open %s failed!\n",file);
        goto SAMPLE_HIFB_NoneBufMode_2;
    }

    s32Ret = ioctl (m_sys_hifb_fd, FBIOGET_VSCREENINFO, &stVarInfo);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOGET_VSCREENINFO failed!\n");
        close(m_sys_hifb_fd);
        goto SAMPLE_HIFB_NoneBufMode_2;
    }

    stVarInfo.red = s_r32;
    stVarInfo.green = s_g32;
    stVarInfo.blue = s_b32;
    stVarInfo.transp = s_a32;
    stVarInfo.bits_per_pixel = 32;
    stVarInfo.xres=WIDTH;
    stVarInfo.yres=HEIGHT;
    stVarInfo.activate=FB_ACTIVATE_NOW;
    stVarInfo.xres_virtual=WIDTH;
    stVarInfo.yres_virtual = HEIGHT;
    stVarInfo.xoffset=0;
    stVarInfo.yoffset=0;

    s32Ret = ioctl (m_sys_hifb_fd, FBIOPUT_VSCREENINFO, &stVarInfo);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_VSCREENINFO failed!\n");
        close(m_sys_hifb_fd);
        goto SAMPLE_HIFB_NoneBufMode_2;
    }

    s32Ret = ioctl(m_sys_hifb_fd, FBIOGET_LAYER_INFO, &stLayerinfo);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOGET_LAYER_INFO failed!\n");
        close(m_sys_hifb_fd);
        goto SAMPLE_HIFB_NoneBufMode_2;
    }

    stLayerinfo.u32Mask = 0;
    stLayerinfo.BufMode = HIFB_LAYER_BUF_NONE;
    stLayerinfo.u32Mask |= HIFB_LAYERMASK_BUFMODE;
    s32Ret = ioctl(m_sys_hifb_fd, FBIOPUT_LAYER_INFO, &stLayerinfo);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_LAYER_INFO failed!\n");
        close(m_sys_hifb_fd);
        goto SAMPLE_HIFB_NoneBufMode_2;
    }
    stDDRZonePara.u32StartSection = 0;
    stDDRZonePara.u32ZoneNums = 15;
    s32Ret = ioctl(m_sys_hifb_fd, FBIOPUT_MDDRDETECT_HIFB, &stDDRZonePara);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_MDDRDETECT_HIFB failed!\n");
        close(m_sys_hifb_fd);
        goto SAMPLE_HIFB_NoneBufMode_2;
    }

    /* 2. open compress */
    bEnable = HI_FALSE;
    s32Ret = ioctl(m_sys_hifb_fd, FBIOPUT_COMPRESSION_HIFB, &bEnable);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_COMPRESSION_HIFB failed!\n");
        close(m_sys_hifb_fd);
        goto SAMPLE_HIFB_NoneBufMode_2;
    }

    Sys_init_Sucess = s32Ret;
    qDebug("Sample_Common_Sys init sucess\n");

    return ;


SAMPLE_HIFB_NoneBufMode_2:
    m_Sys_Vo.SAMPLE_COMM_VO_HdmiStop();
//    m_Sys_Vo.SAMPLE_COMM_VO_StopLayer();
SAMPLE_HIFB_NoneBufMode_1:
    m_Sys_Vo.SAMPLE_COMM_VO_StopDev();
SAMPLE_HIFB_NoneBufMode_0:
    SAMPLE_COMM_SYS_Exit();

    Sys_init_Sucess = s32Ret;
    qDebug("Sample_Common_Sys init failed\n");


    qDebug("create Sample_Common_Sys\n");
#endif
}

//HI_BOOL Sample_Common_Sys::SYS_INIT_ISSUCESS()
//{
//    return (Sys_init_Sucess > 0 ? HI_TRUE : HI_FALSE);
//}
void *vdec_stream(void *argv)
{

    HI_U32 Vdec_Chn = 9;
    //VDEC_CHN_ATTR_S stVdecChnAttr[VDEC_MAX_CHN_NUM];
    VdecThreadParam stVdecSend[VDEC_MAX_CHN_NUM];
    //VPSS_GRP_ATTR_S stVpssGrpAttr[VDEC_MAX_CHN_NUM];
    pthread_t   VdecThread[2*VDEC_MAX_CHN_NUM];

    VDEC_CHN_ATTR_S *pstVdecChnAttr  = (VDEC_CHN_ATTR_S *)argv;
    //struct vdec_attr *vedc = (struct vdec_attr*)argv;
    /************************************************
    step8:  send stream to VDEC
    *************************************************/
    SAMPLE_COMM_VDEC_ThreadParam(Vdec_Chn, &stVdecSend[0], pstVdecChnAttr, SAMPLE_1080P_H264_PATH);
    SAMPLE_COMM_VDEC_StartSendStream(Vdec_Chn, &stVdecSend[0], &VdecThread[0]);

    /***  get the stat info of luma pix  ***/
    //SAMPLE_COMM_VDEC_StartGetLuma(1, &stVdecSend[0], &VdecThread[0]);

    /***  set the rotational angle of decode pic  ***/

    /***  control the send stream thread and get luma info thread  ***/
    SAMPLE_COMM_VDEC_CmdCtrl(1, &stVdecSend[0]);
//    printf("enter key\n");
//    getchar();

    SAMPLE_COMM_VDEC_StopSendStream(Vdec_Chn, &stVdecSend[0], &VdecThread[0]);

//    SAMPLE_COMM_VDEC_StopGetLuma(Vdec_Chn, &stVdecSend[0], &VdecThread[0]);
}

HI_S32 Sample_Common_Sys::VdecTest()
{
    HI_S32 i;
    HI_S32 Vdec_Chn = 9;
    HI_S32 s32Ret;
    VDEC_CHN_ATTR_S stVdecChnAttr[VDEC_MAX_CHN_NUM];
    SIZE_S  stSize;
    VPSS_GRP_ATTR_S stVpssGrpAttr = {0};
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    SAMPLE_RC_E enRcMode= SAMPLE_RC_CBR;
    HI_U32 u32Profile = 0;
    VIDEO_NORM_E gs_enNorm_venc = VIDEO_ENCODING_MODE_NTSC;
    HI_U32 u32VoFrmRate;
    VB_CONF_S       stModVbConf;
    SIZE_S stRotateSize;
//    MPP_CHN_S stSrcChn;
//    MPP_CHN_S stDestChn;

    stSize.u32Width = HD_WIDTH;
    stSize.u32Height = HD_HEIGHT;

    m_Sys_Vdec = new Sample_Common_Vdec(Vdec_Chn,1,0);

    printf("init CommonPool\n");
    m_Sys_Vdec->SAMPLE_COMM_VDEC_ModCommPoolConf(&stModVbConf, PT_H264, &stSize, Vdec_Chn, HI_FALSE);
    s32Ret = m_Sys_Vdec->SAMPLE_COMM_VDEC_InitModCommVb(&stModVbConf);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("init mod common vb fail for %#x!\n", s32Ret);
        return HI_FALSE;
    }


    m_Sys_Vdec->SAMPLE_COMM_VDEC_ChnAttr(Vdec_Chn, &stVdecChnAttr[0], PT_H264, &stSize);
    for(int i = 0;i < Vdec_Chn;i++){
        m_Sys_Vdec->SAMPLE_COMM_VDEC_SetChnAttr(i,&stVdecChnAttr[i]);

        stRotateSize.u32Width = stRotateSize.u32Height = MAX2(stVdecChnAttr[i].u32PicWidth, stVdecChnAttr[i].u32PicHeight);
        m_pVpss[i] = new Sample_Common_Vpss(1,2,&stRotateSize,nullptr);
        if(!m_pVpss[i]){
            printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
            delete m_pVpss[i];
            return HI_FALSE;
        }else if( HI_FALSE == m_pVpss[i]->SAMPLE_COMM_VPSS_CreatIsSucess()){
            m_pVpss[i]->SAMPLE_COMM_VPSS_Stop();
            printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
            delete m_pVpss[i];
            return HI_FALSE;
        }
    }

    printf("start vdec\n");
    s32Ret = m_Sys_Vdec->SAMPLE_COMM_VDEC_Start(Vdec_Chn);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VDEC fail for %#x!\n", s32Ret);
        goto END2;
    }

    printf("start vo\n");

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_GetWH(m_stPubAttr.enIntfSync,&stSize.u32Width,\
        &stSize.u32Height,&u32VoFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get vo wh failed with %d!\n", s32Ret);
        goto END4_4;
    }
    memcpy(&stLayerAttr.stImageSize,&stSize,sizeof(stSize));

    stLayerAttr.u32DispFrmRt = 30 ;
    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    stLayerAttr.stDispRect.u32Width = stSize.u32Width;
    stLayerAttr.stDispRect.u32Height = stSize.u32Height;

    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartLayer(&stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vo layer failed with %d!\n", s32Ret);
        goto END4_4;
    }


    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartChn(VO_MODE_9MUX);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("vdec bind vpss fail for %#x!\n", s32Ret);
        goto END4_4;
    }

    printf("vdec bind vpss\n");
    for(i=0; i<Vdec_Chn; i++)
    {
        //bind vpss

//        stSrcChn.enModId = HI_ID_VDEC;
//        stSrcChn.s32DevId = 0;
//        stSrcChn.s32ChnId = m_Sys_Vdec->m_Vdec_Tab[i];

//        stDestChn.enModId = HI_ID_VPSS;
//        stDestChn.s32DevId = m_pVpss[i]->m_Grp_Tab[0];
//        stDestChn.s32ChnId = 0;
//        SAMPLE_COMM_BindVpss(&stSrcChn,&stDestChn);

        m_Sys_Vdec->SAMPLE_COMM_VDEC_BindVpss(m_Sys_Vdec->m_Vdec_Tab[i],m_pVpss[i]->m_Grp_Tab[0],0);

        s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_BindVpss(i, i, VPSS_CHN0);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("vpss bind vo fail for %#x!\n", s32Ret);
            goto END6;
        }


    }
#if 0
    //venc
    printf("start venc h264\n");
    m_Sys_Venc[0] = new Sample_Common_Venc();
    s32Ret = m_Sys_Venc[0]->SAMPLE_COMM_VENC_Start(PT_H264, \
                                   gs_enNorm_venc, PIC_HD1080, enRcMode,u32Profile);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
//            goto END_VENC_1080P_CLASSIC_4;
    }

    printf("venc bind vpss\n");
    s32Ret = m_Sys_Venc[0]->SAMPLE_COMM_VENC_BindVpss(m_pVpss[0]->m_Grp_Tab[0], 1);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
//            goto END_VENC_1080P_CLASSIC_4;
    }
    printf("start venc h265\n");
    m_Sys_Venc[1] = new Sample_Common_Venc();
    s32Ret = m_Sys_Venc[1]->SAMPLE_COMM_VENC_Start(PT_H265, \
                                   gs_enNorm_venc, PIC_HD1080, enRcMode,u32Profile);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
//            goto END_VENC_1080P_CLASSIC_4;
    }

    printf("venc bind vpss\n");
    s32Ret = m_Sys_Venc[1]->SAMPLE_COMM_VENC_BindVpss(m_pVpss[1]->m_Grp_Tab[0], 1);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
//            goto END_VENC_1080P_CLASSIC_4;
    }

    printf("start get stream\n");
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(2);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
//        goto END_VENC_1080P_CLASSIC_4;
    }

#endif

    pthread_t Vdec_Thread;
    pthread_create(&Vdec_Thread, 0, vdec_stream, (HI_VOID *)&stVdecChnAttr[0]);

    return 0;

END6:
    for(i=0; i<Vdec_Chn; i++)
    {
        s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_UnBindVpss(i, i, VPSS_CHN0);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("vpss unbind vo fail for %#x!\n", s32Ret);
        }
    }



END4_4:
    m_Sys_Vo.SAMPLE_COMM_VO_StopChn();

END2:
    m_Sys_Vdec->SAMPLE_COMM_VDEC_Stop(1);

    return 0;
}

HI_S32 Sample_Common_Sys::Vio_8_1080P_Test()
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_1080P;
    VIDEO_NORM_E enNorm = VIDEO_ENCODING_MODE_NTSC;

    HI_U32 u32ViChnCnt = 8;
    HI_S32 s32VpssGrpCnt = 8;

    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
    VPSS_CHN VpssChn_VoHD0 = VPSS_CHN1;
    VPSS_CHN VpssChn_VoHD1 = VPSS_CHN2;
    VPSS_CHN VpssChn_VoSD0 = VPSS_CHN3;

    VO_DEV VoDev;
    VO_LAYER VoLayer;
    VO_CHN VoChn;
//    VO_PUB_ATTR_S stVoPubAttr_hd0, stVoPubAttr_hd1, stVoPubAttr_sd;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    SAMPLE_VO_MODE_E enVoMode, enPreVoMode;

    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
//    HI_U32 u32BlkSize;
//    HI_S32 ch;
    SIZE_S stSize;
    HI_U32 u32WndNum;
//    HI_U32 vichn;

    /******************************************
     step 3: start vi dev & chn
    ******************************************/
    s32Ret = m_Sys_Vo.SAMPLE_COMM_VI_Start(enViMode, enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
//        goto END_8_1080P_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, PIC_HD1080, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
//        goto END_8_1080P_1;
    }

    memset(&stGrpAttr,0,sizeof(VPSS_GRP_ATTR_S));
    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    m_pVpss[0] = new Sample_Common_Vpss(s32VpssGrpCnt,VPSS_MAX_CHN_NUM,&stSize,&stGrpAttr);

//    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM, &stGrpAttr);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("Start Vpss failed!\n");
//        goto END_8_1080P_1;
//    }

    s32Ret = m_Sys_Vo.SAMPLE_COMM_VI_BindVpss(enViMode,m_pVpss[0]->m_Grp_Tab,s32VpssGrpCnt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
//        goto END_8_1080P_2;
    }

    /******************************************
     step 5: start vo HD0 (bt1120+VGA), multi-screen, you can switch mode
    ******************************************/
    printf("start vo HD0.\n");
//    VoDev = SAMPLE_VO_DEV_DHD0;
//    VoLayer = SAMPLE_VO_LAYER_VHD0;
    u32WndNum = 8;
    enVoMode = VO_MODE_9MUX;

//    stVoPubAttr_hd0.enIntfSync = VO_OUTPUT_1080P60;
//    stVoPubAttr_hd0.enIntfType = VO_INTF_BT1120|VO_INTF_VGA;
//    stVoPubAttr_hd0.u32BgColor = 0x0;
//    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr_hd0);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDev failed!\n");
//        goto END_8_1080P_3;
//    }

    memset(&(stLayerAttr), 0 , sizeof(VO_VIDEO_LAYER_ATTR_S));
    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_GetWH(m_stPubAttr.enIntfSync, \
        &stLayerAttr.stImageSize.u32Width, \
        &stLayerAttr.stImageSize.u32Height, \
        &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_GetWH failed!\n");
//        goto END_8_1080P_4;
    }

    stLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    stLayerAttr.stDispRect.u32Width   = stLayerAttr.stImageSize.u32Width;
    stLayerAttr.stDispRect.u32Height  = stLayerAttr.stImageSize.u32Height;
    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartLayer(&stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartLayer failed!\n");
//        goto END_8_1080P_4;
    }

    s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_StartChn(enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
//        goto END_8_1080P_5;
    }

    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;

        s32Ret = m_Sys_Vo.SAMPLE_COMM_VO_BindVpss(VoChn,m_pVpss[0]->m_Grp_Tab[i],VpssChn_VoHD0);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
//            goto END_8_1080P_5;
        }
    }

#if 0
    printf("enter key to exit\n");
    getchar();
    /******************************************
     step 8: exit process
    ******************************************/
END_8_1080P_9:
    VoDev = SAMPLE_VO_DEV_DSD0;
    VoLayer = SAMPLE_VO_LAYER_VSD0;
    u32WndNum = 8;
    enVoMode = VO_MODE_9MUX;
    m_Sys_Vo.SAMPLE_COMM_VO_StopChn(VoLayer, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        m_Sys_Vo.SAMPLE_COMM_VO_UnBindVpss(VoLayer,VoChn,VpssGrp,VpssChn_VoSD0);
    }
    m_Sys_Vo.SAMPLE_COMM_VO_StopLayer(VoLayer);
END_8_1080P_8:

    m_Sys_Vo.SAMPLE_COMM_VO_StopDev(VoDev);

END_8_1080P_7:
    #ifdef HDMI_SUPPORT
    if (stVoPubAttr_hd1.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    #endif
    VoDev = SAMPLE_VO_DEV_DHD1;
    VoLayer = SAMPLE_VO_LAYER_VHD1;
    u32WndNum = 8;
    enVoMode = VO_MODE_9MUX;
    m_Sys_Vo.SAMPLE_COMM_VO_StopChn(VoLayer, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        m_Sys_Vo.SAMPLE_COMM_VO_UnBindVpss(VoLayer,VoChn,VpssGrp,VpssChn_VoHD1);
    }
    m_Sys_Vo.SAMPLE_COMM_VO_StopLayer(VoLayer);

END_8_1080P_6:
    m_Sys_Vo.SAMPLE_COMM_VO_StopDev(VoDev);

END_8_1080P_5:

    #ifdef HDMI_SUPPORT
    if (stVoPubAttr_hd0.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    #endif

    VoDev = SAMPLE_VO_DEV_DHD0;
    VoLayer = SAMPLE_VO_LAYER_VHD0;
    u32WndNum = 8;
    enVoMode = VO_MODE_9MUX;
    m_Sys_Vo.SAMPLE_COMM_VO_StopChn(VoLayer, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        m_Sys_Vo.SAMPLE_COMM_VO_UnBindVpss(VoLayer,VoChn,VpssGrp,VpssChn_VoHD0);
    }
    m_Sys_Vo.SAMPLE_COMM_VO_StopLayer(VoLayer);
END_8_1080P_4:
    m_Sys_Vo.SAMPLE_COMM_VO_StopDev(VoDev);
END_8_1080P_3:	//vi unbind vpss
    m_Sys_Vo.SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_8_1080P_2:	//vpss stop
    m_pVpss[0]->SAMPLE_COMM_VPSS_Stop();
END_8_1080P_1:	//vi stop
    SAMPLE_COMM_VI_Stop(enViMode);
END_8_1080P_0:	//system exit
    SAMPLE_COMM_SYS_Exit();
#endif
    return s32Ret;

}

//static HI_S32 SAMPLE_COMM_VENC_StartGetStream(HI_S32 s32Cnt)
//{
//    SAMPLE_VENC_GETSTREAM_PARA_S gs_stPara;
//    gs_stPara.bThreadStart = HI_TRUE;
//    gs_stPara.s32Cnt = s32Cnt;
//    return pthread_create(&gs_VencPid, 0, SAMPLE_COMM_VENC_GetVencStreamProc, (HI_VOID*)&gs_stPara);
//}


HI_S32 Sample_Common_Sys::Vio_Venc_Test()
{
    PAYLOAD_TYPE_E enPayLoad[2]= {PT_H265, PT_H264};
    PIC_SIZE_E enSize[2] = {PIC_HD1080, PIC_HD1080};
    HI_U32 u32Profile = 0;
    VIDEO_NORM_E gs_enNorm_venc = VIDEO_ENCODING_MODE_NTSC;

//    VB_CONF_S stVbConf;
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_1080P;
    HI_S32 s32VpssGrpCnt = 8;

    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VPSS_GRP_ATTR_S stVpssGrpAttr = {0};


    VENC_CHN VencChn;
    SAMPLE_RC_E enRcMode= SAMPLE_RC_CBR;

    HI_S32 s32ChnNum=2;

    HI_S32 s32Ret = HI_SUCCESS;
//    HI_U32 u32BlkSize;
    SIZE_S stSize;
//    char c;


//    /******************************************
//     step  1: init sys variable
//    ******************************************/
//    memset(&stVbConf,0,sizeof(VB_CONF_S));

//    stVbConf.u32MaxPoolCnt = 128;
//    /*video buffer*/
//	if(s32ChnNum >= 1)
//    {
//        u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm_venc,\
//	                enSize[0], SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH,COMPRESS_MODE_SEG);
//	    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
//	    stVbConf.astCommPool[0].u32BlkCnt = 32;
//	}
//	if(s32ChnNum >= 2)
//    {
//        u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm_venc,\
//	                enSize[1], SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH,COMPRESS_MODE_SEG);
//	    stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
//	    stVbConf.astCommPool[1].u32BlkCnt  =32;
//	}


//    /******************************************
//     step 2: mpp system init.
//    ******************************************/
//    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
//        goto END_VENC_1080P_CLASSIC_0;
//    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = m_Sys_Vo.SAMPLE_COMM_VI_Start(enViMode,gs_enNorm_venc);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
//        goto END_VENC_1080P_CLASSIC_1;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm_venc, enSize[0], &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
//        goto END_VENC_1080P_CLASSIC_1;
    }

    if(s32ChnNum >= 1)
    {
        VpssGrp = 0;
        memset(&stVpssGrpAttr,0,sizeof(VPSS_GRP_ATTR_S));
        stVpssGrpAttr.u32MaxW 	= stSize.u32Width;
        stVpssGrpAttr.u32MaxH 	= stSize.u32Height;
        stVpssGrpAttr.bNrEn 	= HI_TRUE;
        stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
        stVpssGrpAttr.enPixFmt 	= SAMPLE_PIXEL_FORMAT;
        m_pVpss[0] = new Sample_Common_Vpss(s32VpssGrpCnt,VPSS_MAX_CHN_NUM,&stSize,&stVpssGrpAttr);
//        s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize,s32ChnNum , &stVpssGrpAttr);
//        if (HI_SUCCESS != s32Ret)
//        {
//            SAMPLE_PRT("Start Vpss failed!\n");
//            goto END_VENC_1080P_CLASSIC_2;
//        }

        s32Ret = m_Sys_Vo.SAMPLE_COMM_VI_BindVpss(enViMode,m_pVpss[0]->m_Grp_Tab,s32VpssGrpCnt);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Vi bind Vpss failed!\n");
//            goto END_VENC_1080P_CLASSIC_3;
        }

    }

//    /******************************************
//     step 5: start stream venc
//    ******************************************/
//    /*** HD1080P **/
//    printf("\t c) cbr.\n");
//    printf("\t v) vbr.\n");
//    printf("\t a) avbr.\n");
//    printf("\t f) fixQp\n");
//    printf("please input choose rc mode!\n");
//    c = (char)getchar();
//    switch(c)
//    {
//        case 'c':
//            enRcMode = SAMPLE_RC_CBR;
//            break;
//        case 'v':
//            enRcMode = SAMPLE_RC_VBR;
//            break;
//        case 'a':
//            enRcMode = SAMPLE_RC_AVBR;
//            break;
//        case 'f':
//            enRcMode = SAMPLE_RC_FIXQP;
//            break;
//        default:
//            printf("rc mode! is invaild!\n");
//            goto END_VENC_1080P_CLASSIC_3;
//    }

    /*** enSize[0] **/
    if(s32ChnNum >= 1)
    {
        VpssGrp = 0;
        VpssChn = 0;
        VencChn = 0;

        m_Sys_Venc[0] = new Sample_Common_Venc();
        s32Ret = m_Sys_Venc[0]->SAMPLE_COMM_VENC_Start(enPayLoad[0],\
                                       gs_enNorm_venc, enSize[0], enRcMode,u32Profile);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
//            goto END_VENC_1080P_CLASSIC_4;
        }

        s32Ret = m_Sys_Venc[0]->SAMPLE_COMM_VENC_BindVpss(m_pVpss[0]->m_Grp_Tab[VpssGrp], VpssChn);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
//            goto END_VENC_1080P_CLASSIC_4;
        }
    }

    /*** enSize[1] **/
    if(s32ChnNum >= 2)
    {
        VpssChn = 1;
        VencChn = 1;
        m_Sys_Venc[1] = new Sample_Common_Venc();
        s32Ret = m_Sys_Venc[1]->SAMPLE_COMM_VENC_Start( enPayLoad[1], \
                                       gs_enNorm_venc, enSize[1], enRcMode,u32Profile);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
//            goto END_VENC_1080P_CLASSIC_4;
        }

        s32Ret = m_Sys_Venc[1]->SAMPLE_COMM_VENC_BindVpss(VpssGrp, VpssChn);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
//            goto END_VENC_1080P_CLASSIC_4;
        }
    }
    /******************************************
     step 6: stream venc process -- get stream, then save it to file.
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(s32ChnNum);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
//        goto END_VENC_1080P_CLASSIC_4;
    }

//    printf("please press ENTER to exit this sample\n");
//    getchar();
//    getchar();

//    /******************************************
//     step 7: exit process
//    ******************************************/
//    SAMPLE_COMM_VENC_StopGetStream();

//END_VENC_1080P_CLASSIC_4:

//    VpssGrp = 0;
//    switch(s32ChnNum)
//    {
//        case 2:
//            VpssChn = 1;
//            VencChn = 1;
//            m_Sys_Venc->SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
//            m_Sys_Venc->SAMPLE_COMM_VENC_Stop(VencChn);
//        case 1:
//            VpssChn = 0;
//            VencChn = 0;
//            m_Sys_Venc->SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
//            m_Sys_Venc->SAMPLE_COMM_VENC_Stop(VencChn);
//            break;

//    }

//END_VENC_1080P_CLASSIC_3:    //unbind vpss and vi
//    m_Sys_Vo.SAMPLE_COMM_VI_UnBindVpss(enViMode);
//END_VENC_1080P_CLASSIC_2:    //vpss stop
//    m_pVpss[0]->SAMPLE_COMM_VPSS_Stop();
//END_VENC_1080P_CLASSIC_1:	//vi stop
//    m_Sys_Vo.SAMPLE_COMM_VI_Stop(enViMode);
//END_VENC_1080P_CLASSIC_0:	//system exit
//    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

Sample_Common_Sys::~Sample_Common_Sys()
{


    delete m_Sys_Vdec;
//    delete [] m_Sys_Venc;
//    delete [] m_pVpss;
      qDebug("Sample_Common_Sys exit\n");
//    close(m_sys_hifb_fd);
//    SAMPLE_COMM_VO_StopLayer(m_FbVoLayer);
    m_Sys_Vo.SAMPLE_COMM_VO_StopDev();
    SAMPLE_COMM_SYS_Exit();

}


HI_S32 Sample_Common_Sys::SAMPLE_COMM_BindVpss(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn)
{
    CHECK_RET(HI_MPI_SYS_Bind(pstSrcChn, pstDestChn), "HI_MPI_SYS_Bind");

    return HI_SUCCESS;

}
HI_S32 Sample_Common_Sys::SAMPLE_SYS_SetReg(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32 *pu32Addr = nullptr;
    HI_U32 u32MapLen = sizeof(u32Value);

    pu32Addr = (HI_U32 *)HI_MPI_SYS_Mmap(u32Addr, u32MapLen);
    if(nullptr == pu32Addr)
    {
        return HI_FAILURE;
    }

    *pu32Addr = u32Value;

    return HI_MPI_SYS_Munmap(pu32Addr, u32MapLen);
}

HI_S32 Sample_Common_Sys::SAMPLE_SYS_GetReg(HI_U32 u32Addr, HI_U32 *pu32Value)
{
    HI_U32 *pu32Addr = nullptr;
    HI_U32 u32MapLen;

    if (nullptr == pu32Value)
    {
        return HI_ERR_SYS_NULL_PTR;
    }

    u32MapLen = sizeof(*pu32Value);
    pu32Addr = (HI_U32 *)HI_MPI_SYS_Mmap(u32Addr, u32MapLen);
    if(nullptr == pu32Addr)
    {
        return HI_FAILURE;
    }

    *pu32Value = *pu32Addr;

    return HI_MPI_SYS_Munmap(pu32Addr, u32MapLen);
}

/******************************************************************************
* function : get picture size(w*h), according Norm and enPicSize
******************************************************************************/
HI_S32 Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize)
{
    switch (enPicSize)
    {
        case PIC_QCIF:
            pstSize->u32Width = D1_WIDTH / 4;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?144:120;
            break;
        case PIC_CIF:
            pstSize->u32Width = D1_WIDTH / 2;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?288:240;
            break;
        case PIC_D1:
            pstSize->u32Width = D1_WIDTH;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case PIC_960H:
            pstSize->u32Width = 960;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case PIC_2CIF:
            pstSize->u32Width = D1_WIDTH / 2;
            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case PIC_QVGA:    /* 320 * 240 */
            pstSize->u32Width = 320;
            pstSize->u32Height = 240;
            break;
        case PIC_VGA:     /* 640 * 480 */
            pstSize->u32Width = 640;
            pstSize->u32Height = 480;
            break;
        case PIC_XGA:     /* 1024 * 768 */
            pstSize->u32Width = 1024;
            pstSize->u32Height = 768;
            break;
        case PIC_SXGA:    /* 1400 * 1050 */
            pstSize->u32Width = 1400;
            pstSize->u32Height = 1050;
            break;
        case PIC_UXGA:    /* 1600 * 1200 */
            pstSize->u32Width = 1600;
            pstSize->u32Height = 1200;
            break;
        case PIC_QXGA:    /* 2048 * 1536 */
            pstSize->u32Width = 2048;
            pstSize->u32Height = 1536;
            break;
        case PIC_WVGA:    /* 854 * 480 */
            pstSize->u32Width = 854;
            pstSize->u32Height = 480;
            break;
        case PIC_WSXGA:   /* 1680 * 1050 */
            pstSize->u32Width = 1680;
            pstSize->u32Height = 1050;
            break;
        case PIC_WUXGA:   /* 1920 * 1200 */
            pstSize->u32Width = 1920;
            pstSize->u32Height = 1200;
            break;
        case PIC_WQXGA:   /* 2560 * 1600 */
            pstSize->u32Width = 2560;
            pstSize->u32Height = 1600;
            break;
        case PIC_HD720:   /* 1280 * 720 */
            pstSize->u32Width = 1280;
            pstSize->u32Height = 720;
            break;
        case PIC_HD1080:  /* 1920 * 1080 */
            pstSize->u32Width = 1920;
            pstSize->u32Height = 1080;
            break;
        default:
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/******************************************************************************
* function : calculate VB Block size of Histogram.
******************************************************************************/
HI_U32 Sample_Common_Sys::SAMPLE_COMM_SYS_CalcHistVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstHistBlkSize, HI_U32 u32AlignWidth)
{
    HI_S32 s32Ret;
    SIZE_S stPicSize;

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, enPicSize, &stPicSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get picture size[%d] failed!\n", enPicSize);
            return HI_FAILURE;
    }

   SAMPLE_PRT("stPicSize.u32Width%d,pstHistBlkSize->u32Width%d\n,stPicSize.u32Height%d,pstHistBlkSize->u32Height%d\n",
    stPicSize.u32Width,pstHistBlkSize->u32Width,
    stPicSize.u32Height,pstHistBlkSize->u32Height );
    return (CEILING_2_POWER(44, u32AlignWidth)*CEILING_2_POWER(44, u32AlignWidth)*16*4);

    return HI_SUCCESS;
}

/******************************************************************************
* function : calculate VB Block size of picture.
******************************************************************************/
HI_U32 Sample_Common_Sys::SAMPLE_COMM_SYS_CalcPicVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, PIXEL_FORMAT_E enPixFmt, HI_U32 u32AlignWidth,COMPRESS_MODE_E enCompFmt)
{
    HI_S32 s32Ret      		= HI_FAILURE;
    SIZE_S stSize;
    HI_U32 u32Width 		= 0;
    HI_U32 u32Height 		= 0;
    HI_U32 u32BlkSize 		= 0;
    HI_U32 u32HeaderSize 	= 0;

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get picture size[%d] failed!\n", enPicSize);
            return HI_FAILURE;
    }

    if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 != enPixFmt && PIXEL_FORMAT_YUV_SEMIPLANAR_420 != enPixFmt)
    {
        SAMPLE_PRT("pixel format[%d] input failed!\n", enPixFmt);
            return HI_FAILURE;
    }

    if (16!=u32AlignWidth && 32!=u32AlignWidth && 64!=u32AlignWidth)
    {
        SAMPLE_PRT("system align width[%d] input failed!\n",\
               u32AlignWidth);
            return HI_FAILURE;
    }
    if (704 == stSize.u32Width)
    {
        stSize.u32Width = 720;
    }
    //SAMPLE_PRT("w:%d, u32AlignWidth:%d\n", CEILING_2_POWER(stSize.u32Width,u32AlignWidth), u32AlignWidth);

    u32Width  = CEILING_2_POWER(stSize.u32Width, u32AlignWidth);
    u32Height = CEILING_2_POWER(stSize.u32Height,u32AlignWidth);

    if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 == enPixFmt)
    {
        u32BlkSize = u32Width * u32Height * 2;
    }
    else
    {
        u32BlkSize = u32Width * u32Height * 3 / 2;
    }


    if(COMPRESS_MODE_SEG == enCompFmt)
    {
        VB_PIC_HEADER_SIZE(u32Width,u32Height,enPixFmt,u32HeaderSize);
    }

    u32BlkSize += u32HeaderSize;

    return u32BlkSize;
}


/******************************************************************************
* function : Set system memory location
******************************************************************************/
HI_S32 Sample_Common_Sys::SAMPLE_COMM_SYS_MemConfig(HI_VOID)
{
    HI_S32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    HI_CHAR * pcMmzName = nullptr;
    MPP_CHN_S stMppChnVO;
    MPP_CHN_S stMppChnVPSS;
    MPP_CHN_S stMppChnVENC;
    MPP_CHN_S stMppChnVDEC;

    /* vdec chn config to mmz 'null' */
    for(i=0; i<VDEC_MAX_CHN_NUM; i++)
    {
        stMppChnVDEC.enModId = HI_ID_VDEC;
        stMppChnVDEC.s32DevId = 0;
        stMppChnVDEC.s32ChnId = i;

        /*vdec*/
        s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVDEC,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
            return HI_FAILURE;
        }
    }

    /* vpss group config to mmz 'null' */
    for(i=0;i<VPSS_MAX_GRP_NUM;i++)
    {
        stMppChnVPSS.enModId  = HI_ID_VPSS;
        stMppChnVPSS.s32DevId = i;
        stMppChnVPSS.s32ChnId = 0;


        /*vpss*/
        s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVPSS, pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
            return HI_FAILURE;
        }
    }

    /* venc chn config to mmz 'null' */
    for (i = 0;i < VENC_MAX_CHN_NUM; i++)
    {
        stMppChnVENC.enModId = HI_ID_VENC;
        stMppChnVENC.s32DevId = 0;
        stMppChnVENC.s32ChnId = i;

        /*venc*/
        s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVENC,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
            return HI_FAILURE;
        }

    }

    /* vo config to mmz 'null' */
    stMppChnVO.enModId  = HI_ID_VOU;
    stMppChnVO.s32DevId = 0;
    stMppChnVO.s32ChnId = 0;
    s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVO, pcMmzName);
    if (s32Ret)
    {
        SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
        return HI_FAILURE;
    }

    return s32Ret;
}

/******************************************************************************
* function : vb init & MPI system init
******************************************************************************/
HI_S32 Sample_Common_Sys::SAMPLE_COMM_SYS_Init(VB_CONF_S *pstVbConf)
{
    MPP_SYS_CONF_S stSysConf = {0};
    HI_S32 s32Ret = HI_FAILURE;
    VB_CONF_S       stVbConf;
    HI_U32 u32PicWidth         = WIDTH;
    HI_U32 u32PicHeight        = HEIGHT;
    HI_U32 i;

    memset(&stVbConf,0,sizeof(VB_CONF_S));

    stVbConf.u32MaxPoolCnt = 128;
    stVbConf.astCommPool[0].u32BlkSize = CEILING_2_POWER(u32PicWidth,SAMPLE_SYS_ALIGN_WIDTH)\
                                                * CEILING_2_POWER(u32PicHeight,SAMPLE_SYS_ALIGN_WIDTH) *2;;
    stVbConf.astCommPool[0].u32BlkCnt =  16;

    stVbConf.astCommPool[1].u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(VIDEO_ENCODING_MODE_NTSC,\
                                                                          PIC_HD1080, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH,COMPRESS_MODE_SEG);
    stVbConf.astCommPool[1].u32BlkCnt = 64;


    HI_MPI_SYS_Exit();

    for(i=0;i<VB_MAX_USER;i++)
    {
         HI_MPI_VB_ExitModCommPool(i);
    }
    for(i=0; i<VB_MAX_POOLS; i++)
    {
         HI_MPI_VB_DestroyPool(i);
    }
    HI_MPI_VB_Exit();

//    if (nullptr == pstVbConf)
//    {
//        SAMPLE_PRT("input parameter is null, it is invaild!\n");
//        return HI_FAILURE;
//    }

    s32Ret = HI_MPI_VB_SetConf(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VB_SetConf failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_Init();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VB_Init failed!\n");
        return HI_FAILURE;
    }

    stSysConf.u32AlignWidth = SAMPLE_SYS_ALIGN_WIDTH;
    s32Ret = HI_MPI_SYS_SetConf(&stSysConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_SYS_SetConf failed\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_SYS_Init();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_SYS_Init failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/******************************************************************************
* function : vb init & MPI system init
******************************************************************************/
HI_S32 Sample_Common_Sys::SAMPLE_COMM_SYS_Payload2FilePostfix(PAYLOAD_TYPE_E enPayload, HI_CHAR* szFilePostfix)
{
    if (PT_H264 == enPayload)
    {
        strcpy(szFilePostfix, ".h264");
    }
    else if (PT_JPEG == enPayload)
    {
        strcpy(szFilePostfix, ".jpg");
    }
    else if (PT_MJPEG == enPayload)
    {
        strcpy(szFilePostfix, ".mjp");
    }
    else
    {
        SAMPLE_PRT("payload type err!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/******************************************************************************
* function : vb exit & MPI system exit
******************************************************************************/
HI_VOID Sample_Common_Sys::SAMPLE_COMM_SYS_Exit(void)
{

    HI_U32 i;

    HI_MPI_SYS_Exit();
    for(i=0;i<VB_MAX_USER;i++)
    {
         HI_MPI_VB_ExitModCommPool(i);
    }
    for(i=0; i<VB_MAX_POOLS; i++)
    {
         HI_MPI_VB_DestroyPool(i);
    }
    HI_MPI_VB_Exit();
    return;
}

#if 0
void Sample_Common_Sys::m_exit()
{
    close(m_sys_hifb_fd);
        m_Sys_Vo.SAMPLE_COMM_VO_StopLayer(m_FbVoLayer);
        m_Sys_Vo.SAMPLE_COMM_VO_StopDev(m_FbVoDev);
        SAMPLE_COMM_SYS_Exit();

}

HI_BOOL Sample_Common_Sys::Set_Alpha(HIFB_ALPHA_S *pstAlpha)
{
    qDebug("enter %s\n",__FUNCTION__);
    if (ioctl(m_sys_hifb_fd, FBIOPUT_ALPHA_HIFB,  pstAlpha) < 0)
    {
        SAMPLE_PRT("Set alpha failed!\n");
        //close(pstInfo->fd);
        return HI_FALSE;
    }

    return HI_TRUE;
}

void Sample_Common_Sys::Enable_Alpha()
{
    HIFB_ALPHA_S stAlpha;

    qDebug("enter %s\n",__FUNCTION__);
    stAlpha.bAlphaEnable = HI_TRUE;
    stAlpha.u8Alpha0 = 0x0;
    stAlpha.u8Alpha1 = 0x0;
    if (ioctl(m_sys_hifb_fd, FBIOPUT_ALPHA_HIFB,  &stAlpha) < 0)
    {
        SAMPLE_PRT("Set alpha failed!\n");
        //close(sys_hifb_fd);
        //return HI_FALSE;
    }

    //return HI_TRUE;
}

void Sample_Common_Sys::DisEnable_Alpha()
{
    HIFB_ALPHA_S stAlpha;

    qDebug("enter %s\n",__FUNCTION__);
    stAlpha.bAlphaEnable = HI_TRUE;
    stAlpha.u8Alpha0 = 0x0;
    stAlpha.u8Alpha1 = 0xFF;
    if (ioctl(m_sys_hifb_fd, FBIOPUT_ALPHA_HIFB,  &stAlpha) < 0)
    {
        SAMPLE_PRT("Set alpha failed!\n");
        //close(m_sys_hifb_fd);
        //return HI_FALSE;
    }

    //return HI_TRUE;
}

void Sample_Common_Sys::Set_ColorKey()
{
    HI_S32 s32Ret;
    HIFB_COLORKEY_S colorKey;

    qDebug("enter %s\n",__FUNCTION__);

    s32Ret = ioctl(m_sys_hifb_fd, FBIOGET_COLORKEY_HIFB, &colorKey);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_COMPRESSION_HIFB failed!\n");
        //close(m_sys_hifb_fd);

    }
    qDebug("colorKey.u32Key = %#x",colorKey.u32Key);

    colorKey.bKeyEnable = HI_TRUE;
    //colorKey.u32Key = 0xffffffff;
    s32Ret = ioctl(m_sys_hifb_fd, FBIOPUT_COLORKEY_HIFB, &colorKey);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("FBIOPUT_COMPRESSION_HIFB failed!\n");
        //close(m_sys_hifb_fd);

    }



}

#endif
