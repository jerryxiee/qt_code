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
#include <QDir>

VO_DEV Sample_Common_Sys::m_FbVoDev = SAMPLE_VO_DEV_DHD0;
VO_DEV Sample_Common_Sys::m_FbVoLayer = 0;
VO_PUB_ATTR_S Sample_Common_Sys::m_stPubAttr = {0};


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

    QDir dir(CONFIG_PATH);
    if(!dir.exists()){
        if(!dir.mkpath(CONFIG_PATH)){
            SAMPLE_PRT("mkpath %s failed\n",CONFIG_PATH);
        }
    }


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
    m_stPubAttr.u32BgColor = 0x0;

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


Sample_Common_Sys::~Sample_Common_Sys()
{


//    delete m_Sys_Vdec;
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
