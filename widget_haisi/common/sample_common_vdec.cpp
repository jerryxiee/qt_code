#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "common/sample_common_sys.h"
#include "sample_common_vdec.h"


//HI_S32 g_s32VBSource = 0;
//VB_POOL g_ahVbPool[VB_MAX_POOLS] ;//= {[0 ... (VB_MAX_POOLS-1)] = VB_INVALID_POOLID};

Sample_Common_Vdec::Sample_Common_Vdec():m_Vdec_ChnNum(1),g_s32VBSource(0)
{
    //g_ahVbPool[VB_MAX_POOLS] = {0} ;
}

HI_VOID	Sample_Common_Vdec::SAMPLE_COMM_VDEC_ChnAttr(HI_S32 s32ChnNum,
    VDEC_CHN_ATTR_S *pstVdecChnAttr, PAYLOAD_TYPE_E enType, SIZE_S *pstSize)
{
    HI_S32 i;

    for(i=0; i<s32ChnNum; i++)
    {
        pstVdecChnAttr[i].enType       = enType;
        pstVdecChnAttr[i].u32BufSize   = 3 * pstSize->u32Width * pstSize->u32Height;
        pstVdecChnAttr[i].u32Priority  = 5;
        pstVdecChnAttr[i].u32PicWidth  = pstSize->u32Width;
        pstVdecChnAttr[i].u32PicHeight = pstSize->u32Height;
        if (PT_H264 == enType || PT_MP4VIDEO == enType)
        {
            pstVdecChnAttr[i].stVdecVideoAttr.enMode=VIDEO_MODE_FRAME;
            pstVdecChnAttr[i].stVdecVideoAttr.u32RefFrameNum = 2;
            pstVdecChnAttr[i].stVdecVideoAttr.bTemporalMvpEnable = HI_FALSE;
        }
        else if (PT_JPEG == enType || PT_MJPEG == enType)
        {
            pstVdecChnAttr[i].stVdecJpegAttr.enMode = VIDEO_MODE_FRAME;
            pstVdecChnAttr[i].stVdecJpegAttr.enJpegFormat = JPG_COLOR_FMT_YCBCR420;
        }
        else if(PT_H265 == enType)
        {
            pstVdecChnAttr[i].stVdecVideoAttr.enMode=VIDEO_MODE_STREAM;
            pstVdecChnAttr[i].stVdecVideoAttr.u32RefFrameNum = 2;
            pstVdecChnAttr[i].stVdecVideoAttr.bTemporalMvpEnable = HI_TRUE;
        }
    }

}

HI_VOID	Sample_Common_Vdec::SAMPLE_COMM_VDEC_VpssGrpAttr(HI_S32 s32ChnNum, VPSS_GRP_ATTR_S *pstVpssGrpAttr, SIZE_S *pstSize)
{
    HI_S32 i;

    for(i=0; i<s32ChnNum; i++)
    {
        pstVpssGrpAttr->enDieMode = VPSS_DIE_MODE_NODIE;
        pstVpssGrpAttr->bIeEn     = HI_FALSE;
        pstVpssGrpAttr->bDciEn    = HI_TRUE;
        pstVpssGrpAttr->bNrEn     = HI_TRUE;
        pstVpssGrpAttr->bHistEn   = HI_FALSE;
        pstVpssGrpAttr->bEsEn	  = HI_FALSE;
        pstVpssGrpAttr->enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
        pstVpssGrpAttr->u32MaxW   = ALIGN_UP(pstSize->u32Width,  16);
        pstVpssGrpAttr->u32MaxH   = ALIGN_UP(pstSize->u32Height, 16);
    }
}

HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_Start(HI_S32 s32ChnNum, VDEC_CHN_ATTR_S *pstAttr)
{
    HI_S32  i;
    HI_U32 u32BlkCnt = 10;
    VDEC_CHN_POOL_S stPool;
    SIZE_S stRotateSize;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stRotateSize.u32Width = stRotateSize.u32Height = MAX2(pstAttr[0].u32PicWidth, pstAttr[0].u32PicHeight);
    m_pVpss = new Sample_Common_Vpss(s32ChnNum,1,&stRotateSize,nullptr);

    for(i=0; i<s32ChnNum; i++)
    {
        if(1 == g_s32VBSource)
        {
            CHECK_CHN_RET(HI_MPI_VDEC_SetChnVBCnt(i, u32BlkCnt), i, "HI_MPI_VDEC_SetChnVBCnt");
        }
        CHECK_CHN_RET(HI_MPI_VDEC_CreateChn(i, &pstAttr[i]), i, "HI_MPI_VDEC_CreateChn");
        if (2 == g_s32VBSource)
        {
            stPool.hPicVbPool = g_ahVbPool[0];
            stPool.hPmvVbPool = -1;
            CHECK_CHN_RET(HI_MPI_VDEC_AttachVbPool(i, &stPool), i, "HI_MPI_VDEC_AttachVbPool");
        }
        CHECK_CHN_RET(HI_MPI_VDEC_StartRecvStream(i), i, "HI_MPI_VDEC_StartRecvStream");
        //CHECK_CHN_RET(HI_MPI_VDEC_SetDisplayMode(i, VIDEO_DISPLAY_MODE_PREVIEW), i, "HI_MPI_VDEC_SetDisplayMode");
    }

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = m_pVpss->m_Grp_Tab[0];
    stDestChn.s32ChnId = 0;

    Sample_Common_Sys::SAMPLE_COMM_BindVpss(&stSrcChn,&stDestChn);
    return HI_SUCCESS;
}


HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_Stop(HI_S32 s32ChnNum)
{
    HI_S32 i;

    for(i=0; i<s32ChnNum; i++)
    {
        CHECK_CHN_RET(HI_MPI_VDEC_StopRecvStream(i), i, "HI_MPI_VDEC_StopRecvStream");
        CHECK_CHN_RET(HI_MPI_VDEC_DestroyChn(i), i, "HI_MPI_VDEC_DestroyChn");
    }

    return HI_SUCCESS;
}




HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_BindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = 0;

    CHECK_RET(HI_MPI_SYS_Bind(&stSrcChn, &stDestChn), "HI_MPI_SYS_Bind");

    return HI_SUCCESS;
}



HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_BindVo(VDEC_CHN VdChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    CHECK_RET(HI_MPI_SYS_Bind(&stSrcChn, &stDestChn), "HI_MPI_SYS_Bind");

    return HI_SUCCESS;
}



HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_UnBindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = 0;

    CHECK_RET(HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn), "HI_MPI_SYS_UnBind");

    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_UnBindVo(VDEC_CHN VdChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    CHECK_RET(HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn), "HI_MPI_SYS_UnBind");

    return HI_SUCCESS;
}


/******************************************************************************
* function : Set system memory location
******************************************************************************/
HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_MemConfig(HI_VOID)
{
    HI_S32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    HI_CHAR * pcMmzName;
    MPP_CHN_S stMppChnVDEC;

    /* VDEC chn max is 80*/
    for(i=0; i<80; i++)
    {
        stMppChnVDEC.enModId = HI_ID_VDEC;
        stMppChnVDEC.s32DevId = 0;
        stMppChnVDEC.s32ChnId = i;

        if(0 == (i%2))
        {
            pcMmzName = NULL;
        }
        else
        {
            pcMmzName = "ddr1";
        }

        s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVDEC,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}
