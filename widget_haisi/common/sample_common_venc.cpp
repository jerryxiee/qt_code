#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "sample_common_venc.h"
#include "sample_common_sys.h"

HI_U32 Sample_Common_Venc::m_Venc_MaxTab[VENC_MAX_CHN_NUM] = {0};

Sample_Common_Venc::Sample_Common_Venc()
{
    HI_U32 Venc_Index = 0;

//    m_Venc_Tab[VENC_MAX_CHN_NUM] = {0};

    while(m_Venc_MaxTab[Venc_Index] != 0){
        Venc_Index++;
        if(Venc_Index >= VENC_MAX_CHN_NUM){
//            m_Venc_ChnNum = 0;
            return;
        }
    }
    m_Venc_MaxTab[Venc_Index] = 1;
    m_Venc_Chn = Venc_Index;
}

/******************************************************************************
* function : Set venc memory location
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_MemConfig(HI_VOID)
{
    HI_S32 i = 0;
    HI_S32 s32Ret;
    HI_CHAR* pcMmzName;
    MPP_CHN_S stMppChnVENC;
    /* group, venc max chn is 64*/
    for (i = 0; i < 64; i++)
    {
        stMppChnVENC.enModId = HI_ID_VENC;
        stMppChnVENC.s32DevId = 0;
        stMppChnVENC.s32ChnId = i;
        pcMmzName = nullptr;

        /*venc*/
        s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVENC, pcMmzName);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConf with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

/******************************************************************************
* function : venc bind vi
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_BindVi(VI_CHN ViChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = ViChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = m_Venc_Chn;
    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* function : venc unbind vi
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_UnBindVi(VI_CHN ViChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = ViChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = m_Venc_Chn;
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}


/******************************************************************************
* function : venc bind vpss
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_BindVpss(VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = m_Venc_Chn;
    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* function : venc unbind vpss
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_UnBindVpss(VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = m_Venc_Chn;
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* function : venc bind vo
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_BindVo(VO_DEV VoDev, VO_CHN VoChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VOU;
    stSrcChn.s32DevId = VoDev;
    stSrcChn.s32ChnId = VoChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = m_Venc_Chn;
    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* function : venc unbind vo
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_UnBindVo(VO_DEV VoDev, VO_CHN VoChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VOU;
    stSrcChn.s32DevId = VoDev;
    stSrcChn.s32ChnId = VoChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = m_Venc_Chn;
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* function : vdec bind venc
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VDEC_BindVenc(VDEC_CHN VdChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = m_Venc_Chn;
    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* function : venc unbind vo
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VDEC_UnBindVenc(VDEC_CHN VdChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdChn;
    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = m_Venc_Chn;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* funciton : get file postfix according palyload_type.
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_GetFilePostfix(PAYLOAD_TYPE_E enPayload, char* szFilePostfix)
{
    if (PT_H264 == enPayload)
    {
        strncpy(szFilePostfix, ".h264", 10);
    }
    else if (PT_H265 == enPayload)
    {
        strncpy(szFilePostfix, ".h265", 10);
    }
    else if (PT_JPEG == enPayload)
    {
        strncpy(szFilePostfix, ".jpg", 10);
    }
    else if (PT_MJPEG == enPayload)
    {
        strncpy(szFilePostfix, ".mjp", 10);
    }
    else if (PT_MP4VIDEO == enPayload)
    {
        strncpy(szFilePostfix, ".mp4", 10);
    }
    else
    {
        SAMPLE_PRT("payload type err!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_GetGopAttr(VENC_GOP_MODE_E enGopMode,VENC_GOP_ATTR_S *pstGopAttr,VIDEO_NORM_E enNorm)
{
    switch(enGopMode)
    {
        case VENC_GOPMODE_NORMALP:
        pstGopAttr->enGopMode  = VENC_GOPMODE_NORMALP;
        pstGopAttr->stNormalP.s32IPQpDelta = 2;
        break;

        case VENC_GOPMODE_SMARTP:
        pstGopAttr->enGopMode  = VENC_GOPMODE_SMARTP;
        pstGopAttr->stSmartP.s32BgQpDelta = 4;
        pstGopAttr->stSmartP.s32ViQpDelta = 2;
        pstGopAttr->stSmartP.u32BgInterval = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 75 : 90;
        break;
        case VENC_GOPMODE_DUALP:
        pstGopAttr->enGopMode  = VENC_GOPMODE_DUALP;
        pstGopAttr->stDualP.s32IPQpDelta  = 4;
        pstGopAttr->stDualP.s32SPQpDelta  = 2;
        pstGopAttr->stDualP.u32SPInterval = 3;
        break;
        default:
        SAMPLE_PRT("not support the gop mode !\n");
        return HI_FAILURE;
        break;
    }
    return HI_SUCCESS;
}
/******************************************************************************
* funciton : save mjpeg stream.
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_SaveMJpeg(FILE* fpMJpegFile, VENC_STREAM_S* pstStream)
{
    VENC_PACK_S*  pstData;
    HI_U32 i;

    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        pstData = &pstStream->pstPack[i];
        fwrite(pstData->pu8Addr + pstData->u32Offset, pstData->u32Len - pstData->u32Offset, 1, fpMJpegFile);
        fflush(fpMJpegFile);
    }
    return HI_SUCCESS;
}

/******************************************************************************
* funciton : save jpeg stream.
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_SaveJpeg(FILE* fpJpegFile, VENC_STREAM_S* pstStream)
{
    VENC_PACK_S*  pstData;
    HI_U32 i;
    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        pstData = &pstStream->pstPack[i];
        fwrite(pstData->pu8Addr + pstData->u32Offset, pstData->u32Len - pstData->u32Offset, 1, fpJpegFile);
        fflush(fpJpegFile);
    }
    return HI_SUCCESS;
}

/******************************************************************************
* funciton : save H264 stream
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_SaveH264(FILE* fpH264File, VENC_STREAM_S* pstStream)
{
    HI_S32 i;

    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        fwrite(pstStream->pstPack[i].pu8Addr + pstStream->pstPack[i].u32Offset,
        pstStream->pstPack[i].u32Len - pstStream->pstPack[i].u32Offset, 1, fpH264File);
        fflush(fpH264File);
    }
    return HI_SUCCESS;
}

/******************************************************************************
* funciton : save H265 stream
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_SaveH265(FILE* fpH265File, VENC_STREAM_S* pstStream)
{
    HI_S32 i;
    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        fwrite(pstStream->pstPack[i].pu8Addr + pstStream->pstPack[i].u32Offset,
               pstStream->pstPack[i].u32Len - pstStream->pstPack[i].u32Offset, 1, fpH265File);
        fflush(fpH265File);
    }
    return HI_SUCCESS;
}

/******************************************************************************
* funciton : save jpeg stream
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_SaveJPEG(FILE* fpJpegFile, VENC_STREAM_S* pstStream)
{
    VENC_PACK_S*  pstData;
    HI_U32 i;
    for (i = 0; i < pstStream->u32PackCount; i++)
    {
        pstData = &pstStream->pstPack[i];
        fwrite(pstData->pu8Addr + pstData->u32Offset, pstData->u32Len - pstData->u32Offset, 1, fpJpegFile);
        fflush(fpJpegFile);
    }
    return HI_SUCCESS;
}

/******************************************************************************
* funciton : save stream
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_SaveStream(PAYLOAD_TYPE_E enType, FILE* pFd, VENC_STREAM_S* pstStream)
{
    HI_S32 s32Ret;
    if (PT_H264 == enType)
    {
        s32Ret = SAMPLE_COMM_VENC_SaveH264(pFd, pstStream);
    }
    else if (PT_MJPEG == enType)
    {
        s32Ret = SAMPLE_COMM_VENC_SaveMJpeg(pFd, pstStream);
    }
    else if (PT_H265 == enType)
    {
        s32Ret = SAMPLE_COMM_VENC_SaveH265(pFd, pstStream);
    }
    else
    {
        return HI_FAILURE;
    }
    return s32Ret;
}

/******************************************************************************
* funciton : the process of physical address retrace
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_SaveFile(FILE* pFd, VENC_STREAM_BUF_INFO_S *pstStreamBuf, VENC_STREAM_S* pstStream)
{
    HI_U32 i;
    HI_U32 u32SrcPhyAddr;
    HI_U32 u32Left;
    HI_S32 s32Ret = 0;

    for(i=0; i<pstStream->u32PackCount; i++)
    {
        if (pstStream->pstPack[i].u32PhyAddr + pstStream->pstPack[i].u32Len >=
                pstStreamBuf->u32PhyAddr + pstStreamBuf->u32BufSize)
            {
                if (pstStream->pstPack[i].u32PhyAddr + pstStream->pstPack[i].u32Offset >=
                    pstStreamBuf->u32PhyAddr + pstStreamBuf->u32BufSize)
                {
                    /* physical address retrace in offset segment */
                    u32SrcPhyAddr = pstStreamBuf->u32PhyAddr +
                                    ((pstStream->pstPack[i].u32PhyAddr + pstStream->pstPack[i].u32Offset) -
                                    (pstStreamBuf->u32PhyAddr + pstStreamBuf->u32BufSize));

                    s32Ret = fwrite ((void *)u32SrcPhyAddr, pstStream->pstPack[i].u32Len - pstStream->pstPack[i].u32Offset, 1, pFd);
                    if(s32Ret<0)
                    {
                        SAMPLE_PRT("fwrite err %d\n", s32Ret);
                        return s32Ret;
                    }
                }
                else
                {
                    /* physical address retrace in data segment */
                    u32Left = (pstStreamBuf->u32PhyAddr + pstStreamBuf->u32BufSize) - pstStream->pstPack[i].u32PhyAddr;

                    s32Ret = fwrite((void *)(pstStream->pstPack[i].u32PhyAddr + pstStream->pstPack[i].u32Offset),
                                 u32Left - pstStream->pstPack[i].u32Offset, 1, pFd);
                    if(s32Ret<0)
                    {
                        SAMPLE_PRT("fwrite err %d\n", s32Ret);
                        return s32Ret;
                    }

                    s32Ret = fwrite((void *)pstStreamBuf->u32PhyAddr, pstStream->pstPack[i].u32Len - u32Left, 1, pFd);
                    if(s32Ret<0)
                    {
                        SAMPLE_PRT("fwrite err %d\n", s32Ret);
                        return s32Ret;
                    }
                }
            }
            else
            {
                /* physical address retrace does not happen */
                s32Ret = fwrite ((void *)(pstStream->pstPack[i].u32PhyAddr + pstStream->pstPack[i].u32Offset),
                              pstStream->pstPack[i].u32Len - pstStream->pstPack[i].u32Offset, 1, pFd);
                if(s32Ret<0)
                {
                    SAMPLE_PRT("fwrite err %d\n", s32Ret);
                    return s32Ret;
                }
            }
            fflush(pFd);
    }
    return HI_SUCCESS;
}

/******************************************************************************
* function : get picture size(w*h), according Norm and enPicSize
******************************************************************************/
//HI_S32 SAMPLE_COMM_SYS_GetPicSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize)
//{
//    switch (enPicSize)
//    {
//        case PIC_QCIF:
//            pstSize->u32Width = D1_WIDTH / 4;
//            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?144:120;
//            break;
//        case PIC_CIF:
//            pstSize->u32Width = D1_WIDTH / 2;
//            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?288:240;
//            break;
//        case PIC_D1:
//            pstSize->u32Width = D1_WIDTH;
//            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
//            break;
//        case PIC_960H:
//            pstSize->u32Width = 960;
//            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
//            break;
//        case PIC_2CIF:
//            pstSize->u32Width = D1_WIDTH / 2;
//            pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
//            break;
//        case PIC_QVGA:    /* 320 * 240 */
//            pstSize->u32Width = 320;
//            pstSize->u32Height = 240;
//            break;
//        case PIC_VGA:     /* 640 * 480 */
//            pstSize->u32Width = 640;
//            pstSize->u32Height = 480;
//            break;
//        case PIC_XGA:     /* 1024 * 768 */
//            pstSize->u32Width = 1024;
//            pstSize->u32Height = 768;
//            break;
//        case PIC_SXGA:    /* 1400 * 1050 */
//            pstSize->u32Width = 1400;
//            pstSize->u32Height = 1050;
//            break;
//        case PIC_UXGA:    /* 1600 * 1200 */
//            pstSize->u32Width = 1600;
//            pstSize->u32Height = 1200;
//            break;
//        case PIC_QXGA:    /* 2048 * 1536 */
//            pstSize->u32Width = 2048;
//            pstSize->u32Height = 1536;
//            break;
//        case PIC_WVGA:    /* 854 * 480 */
//            pstSize->u32Width = 854;
//            pstSize->u32Height = 480;
//            break;
//        case PIC_WSXGA:   /* 1680 * 1050 */
//            pstSize->u32Width = 1680;
//            pstSize->u32Height = 1050;
//            break;
//        case PIC_WUXGA:   /* 1920 * 1200 */
//            pstSize->u32Width = 1920;
//            pstSize->u32Height = 1200;
//            break;
//        case PIC_WQXGA:   /* 2560 * 1600 */
//            pstSize->u32Width = 2560;
//            pstSize->u32Height = 1600;
//            break;
//        case PIC_HD720:   /* 1280 * 720 */
//            pstSize->u32Width = 1280;
//            pstSize->u32Height = 720;
//            break;
//        case PIC_HD1080:  /* 1920 * 1080 */
//            pstSize->u32Width = 1920;
//            pstSize->u32Height = 1080;
//            break;
//        default:
//            return HI_FAILURE;
//    }
//    return HI_SUCCESS;
//}

/******************************************************************************
* funciton : Start venc stream mode (h264, mjpeg)
* note      : rate control parameter need adjust, according your case.
******************************************************************************/
#if 1
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_Start(PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32  u32Profile)
{
    HI_S32 s32Ret;
    VENC_CHN_ATTR_S stVencChnAttr;
    VENC_ATTR_H264_S stH264Attr;
    VENC_ATTR_H264_CBR_S    stH264Cbr;
    VENC_ATTR_H264_VBR_S    stH264Vbr;
    VENC_ATTR_H264_AVBR_S    stH264AVbr;
    VENC_ATTR_H264_FIXQP_S  stH264FixQp;
    VENC_ATTR_H265_S        stH265Attr;
    VENC_ATTR_H265_CBR_S    stH265Cbr;
    VENC_ATTR_H265_VBR_S    stH265Vbr;
    VENC_ATTR_H265_AVBR_S    stH265AVbr;
    VENC_ATTR_H265_FIXQP_S  stH265FixQp;
    VENC_ATTR_MJPEG_S stMjpegAttr;
    VENC_ATTR_MJPEG_FIXQP_S stMjpegeFixQp;
    VENC_ATTR_JPEG_S stJpegAttr;
    SIZE_S stPicSize;
    s32Ret = Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(enNorm, enSize, &stPicSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Get picture size failed!\n");
        return HI_FAILURE;
    }


    /******************************************
     step 1:  Create Venc Channel
     ******************************************/
    stVencChnAttr.stVeAttr.enType = enType;
    switch (enType)
    {
        case PT_H264:
        {
            stH264Attr.u32MaxPicWidth = stPicSize.u32Width;
            stH264Attr.u32MaxPicHeight = stPicSize.u32Height;
            stH264Attr.u32PicWidth = stPicSize.u32Width;/*the picture width*/
            stH264Attr.u32PicHeight = stPicSize.u32Height;/*the picture height*/
            stH264Attr.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
            stH264Attr.u32Profile  = u32Profile;/*0: baseline; 1:MP; 2:HP;  3:svc_t */
            stH264Attr.bByFrame = HI_TRUE;/*get stream mode is slice mode or frame mode?*/
            //stH264Attr.u32BFrameNum = 0;/* 0: not support B frame; >=1: number of B frames */
            //stH264Attr.u32RefNum = 1;/* 0: default; number of refrence frame*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrH264e, &stH264Attr, sizeof(VENC_ATTR_H264_S));
            if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
                stH264Cbr.u32Gop            = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Cbr.u32StatTime       = 1; /* stream rate statics time(s) */
                stH264Cbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* input (vi) frame rate */
                stH264Cbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* target frame rate */
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH264Cbr.u32BitRate = 256; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH264Cbr.u32BitRate = 512;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH264Cbr.u32BitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH264Cbr.u32BitRate = 1024 * 2;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH264Cbr.u32BitRate = 1024 * 4;
                        break;
                    case PIC_UHD4K:
                        stH264Cbr.u32BitRate = 1024 * 8;
                        break;
                    default :
                        stH264Cbr.u32BitRate = 1024 * 4;
                        break;
                }
                stH264Cbr.u32FluctuateLevel = 1; /* average bit rate */
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264Cbr, &stH264Cbr, sizeof(VENC_ATTR_H264_CBR_S));
            }
            else if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;
                stH264FixQp.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264FixQp.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264FixQp.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264FixQp.u32IQp = 20;
                stH264FixQp.u32PQp = 23;
                stH264FixQp.u32BQp = 23;
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264FixQp, &stH264FixQp, sizeof(VENC_ATTR_H264_FIXQP_S));
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
                stH264Vbr.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Vbr.u32StatTime = 1;
                stH264Vbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Vbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Vbr.u32MinQp = 10;
                stH264Vbr.u32MinIQp = 10;
                stH264Vbr.u32MaxQp = 40;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH264Vbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH264Vbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH264Vbr.u32MaxBitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH264Vbr.u32MaxBitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH264Vbr.u32MaxBitRate = 1024 * 4;
                        break;
                    case PIC_UHD4K:
                        stH264Vbr.u32MaxBitRate = 1024 * 8;
                        break;
                    default :
                        stH264Vbr.u32MaxBitRate = 1024 * 4;
                        break;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264Vbr, &stH264Vbr, sizeof(VENC_ATTR_H264_VBR_S));
            }
            else if (SAMPLE_RC_AVBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264AVBR;
                stH264AVbr.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264AVbr.u32StatTime = 1;
                stH264AVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264AVbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH264AVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH264AVbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH264AVbr.u32MaxBitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH264AVbr.u32MaxBitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH264AVbr.u32MaxBitRate = 1024 * 4;
                        break;
                    case PIC_UHD4K:
                        stH264AVbr.u32MaxBitRate = 1024 * 8;
                        break;
                    default :
                        stH264AVbr.u32MaxBitRate = 1024 * 4;
                        break;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264AVbr, &stH264AVbr, sizeof(VENC_ATTR_H264_AVBR_S));
            }
            else
            {
                return HI_FAILURE;
            }
        }
        break;
        case PT_MJPEG:
        {
            stMjpegAttr.u32MaxPicWidth = stPicSize.u32Width;
            stMjpegAttr.u32MaxPicHeight = stPicSize.u32Height;
            stMjpegAttr.u32PicWidth = stPicSize.u32Width;
            stMjpegAttr.u32PicHeight = stPicSize.u32Height;
            stMjpegAttr.u32BufSize = stPicSize.u32Width * stPicSize.u32Height * 3;
            stMjpegAttr.bByFrame = HI_TRUE;  /*get stream mode is field mode  or frame mode*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrMjpege, &stMjpegAttr, sizeof(VENC_ATTR_MJPEG_S));
            if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGFIXQP;
                stMjpegeFixQp.u32Qfactor        = 90;
                stMjpegeFixQp.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stMjpegeFixQp.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                memcpy(&stVencChnAttr.stRcAttr.stAttrMjpegeFixQp, &stMjpegeFixQp,
                       sizeof(VENC_ATTR_MJPEG_FIXQP_S));
            }
            else if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32StatTime       = 1;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32FluctuateLevel = 1;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 384 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 768 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 3 * 3;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 5 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 10 * 3;
                        break;
                    default :
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 10 * 3;
                        break;
                }
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGVBR;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32StatTime = 1;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.fr32DstFrmRate = 5;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MinQfactor = 50;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxQfactor = 95;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 2 * 3;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 3 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 6 * 3;
                        break;
                    default :
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 4 * 3;
                        break;
                }
            }
            else
            {
                SAMPLE_PRT("cann't support other mode in this version!\n");
                return HI_FAILURE;
            }
        }
        break;
        case PT_JPEG:
            stJpegAttr.u32PicWidth  = stPicSize.u32Width;
            stJpegAttr.u32PicHeight = stPicSize.u32Height;
            stJpegAttr.u32MaxPicWidth  = stPicSize.u32Width;
            stJpegAttr.u32MaxPicHeight = stPicSize.u32Height;
            stJpegAttr.u32BufSize   = stPicSize.u32Width * stPicSize.u32Height * 3;
            stJpegAttr.bByFrame     = HI_TRUE;/*get stream mode is field mode  or frame mode*/
            stJpegAttr.bSupportDCF  = HI_FALSE;
            memcpy(&stVencChnAttr.stVeAttr.stAttrJpege, &stJpegAttr, sizeof(VENC_ATTR_JPEG_S));
            break;
        case PT_H265:
        {
            stH265Attr.u32MaxPicWidth = stPicSize.u32Width;
            stH265Attr.u32MaxPicHeight = stPicSize.u32Height;
            stH265Attr.u32PicWidth = stPicSize.u32Width;/*the picture width*/
            stH265Attr.u32PicHeight = stPicSize.u32Height;/*the picture height*/
            stH265Attr.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
            if (u32Profile >= 1)
            {
                stH265Attr.u32Profile = 0;
            }/*0:MP; */
            else
            {
                stH265Attr.u32Profile  = u32Profile;
            }/*0:MP*/
            stH265Attr.bByFrame = HI_TRUE;/*get stream mode is slice mode or frame mode?*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrH265e, &stH265Attr, sizeof(VENC_ATTR_H265_S));
            if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
                stH265Cbr.u32Gop            = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Cbr.u32StatTime       = 1; /* stream rate statics time(s) */
                stH265Cbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* input (vi) frame rate */
                stH265Cbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* target frame rate */
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH265Cbr.u32BitRate = 256; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH265Cbr.u32BitRate = 512;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH265Cbr.u32BitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH265Cbr.u32BitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH265Cbr.u32BitRate = 1024 * 4;
                        break;
                    default :
                        stH265Cbr.u32BitRate = 1024 * 4;
                        break;
                }
                stH265Cbr.u32FluctuateLevel = 1; /* average bit rate */
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265Cbr, &stH265Cbr, sizeof(VENC_ATTR_H265_CBR_S));
            }
            else if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265FIXQP;
                stH265FixQp.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265FixQp.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265FixQp.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265FixQp.u32IQp = 20;
                stH265FixQp.u32PQp = 23;
                stH265FixQp.u32BQp = 25;
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265FixQp, &stH265FixQp, sizeof(VENC_ATTR_H265_FIXQP_S));
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
                stH265Vbr.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Vbr.u32StatTime = 1;
                stH265Vbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Vbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Vbr.u32MinQp  = 10;
                stH265Vbr.u32MinIQp = 10;
                stH265Vbr.u32MaxQp  = 40;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH265Vbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH265Vbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH265Vbr.u32MaxBitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH265Vbr.u32MaxBitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH265Vbr.u32MaxBitRate = 1024 * 6;
                        break;
                    default :
                        stH265Vbr.u32MaxBitRate = 1024 * 4;
                        break;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265Vbr, &stH265Vbr, sizeof(VENC_ATTR_H265_VBR_S));
            }
            else if (SAMPLE_RC_AVBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265AVBR;
                stH265AVbr.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265AVbr.u32StatTime = 1;
                stH265AVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265AVbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH265AVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH265AVbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH265AVbr.u32MaxBitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH265AVbr.u32MaxBitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH265AVbr.u32MaxBitRate = 1024 * 4;
                        break;
                    case PIC_UHD4K:
                        stH265AVbr.u32MaxBitRate = 1024 * 8;
                        break;
                    default :
                        stH265AVbr.u32MaxBitRate = 1024 * 4;
                        break;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264AVbr, &stH265AVbr, sizeof(VENC_ATTR_H265_AVBR_S));
            }
            else
            {
                return HI_FAILURE;
            }
        }
        break;
        default:
            return HI_ERR_VENC_NOT_SUPPORT;
    }
    stVencChnAttr.stGopAttr.enGopMode  = VENC_GOPMODE_NORMALP;
    stVencChnAttr.stGopAttr.stNormalP.s32IPQpDelta = 0;
    s32Ret = HI_MPI_VENC_CreateChn(m_Venc_Chn, &stVencChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_CreateChn [%d] faild with %#x! ===\n", \
                   m_Venc_Chn, s32Ret);
        return s32Ret;
    }
    s32Ret = HI_MPI_VENC_StartRecvPic(m_Venc_Chn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_StartRecvPic faild with%#x! \n", s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
#endif

/******************************************************************************
* funciton : Start venc stream mode (h264, mjpeg)
* note      : rate control parameter need adjust, according your case.
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_StartEx(PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32  u32Profile, VENC_GOP_ATTR_S *pstGopAttr)
{
    HI_S32 s32Ret;
    VENC_CHN_ATTR_S stVencChnAttr;
    VENC_ATTR_H264_S stH264Attr;
    VENC_ATTR_H264_CBR_S    stH264Cbr;
    VENC_ATTR_H264_VBR_S    stH264Vbr;
    VENC_ATTR_H264_AVBR_S    stH264AVbr;
    VENC_ATTR_H264_FIXQP_S  stH264FixQp;
    VENC_ATTR_H265_S        stH265Attr;
    VENC_ATTR_H265_CBR_S    stH265Cbr;
    VENC_ATTR_H265_VBR_S    stH265Vbr;
    VENC_ATTR_H265_AVBR_S    stH265AVbr;
    VENC_ATTR_H265_FIXQP_S  stH265FixQp;
    VENC_ATTR_MJPEG_S stMjpegAttr;
    VENC_ATTR_MJPEG_FIXQP_S stMjpegeFixQp;
    VENC_ATTR_JPEG_S stJpegAttr;
    SIZE_S stPicSize;
    s32Ret = Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(enNorm, enSize, &stPicSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Get picture size failed!\n");
        return HI_FAILURE;
    }
    /******************************************
     step 1:  Create Venc Channel
    ******************************************/
    stVencChnAttr.stVeAttr.enType = enType;
    switch (enType)
    {
        case PT_H264:
        {
            stH264Attr.u32MaxPicWidth = stPicSize.u32Width;
            stH264Attr.u32MaxPicHeight = stPicSize.u32Height;
            stH264Attr.u32PicWidth = stPicSize.u32Width;/*the picture width*/
            stH264Attr.u32PicHeight = stPicSize.u32Height;/*the picture height*/
            stH264Attr.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
            stH264Attr.u32Profile  = u32Profile;/*0: baseline; 1:MP; 2:HP;  3:svc_t */
            stH264Attr.bByFrame = HI_TRUE;/*get stream mode is slice mode or frame mode?*/
            //stH264Attr.u32BFrameNum = 0;/* 0: not support B frame; >=1: number of B frames */
            //stH264Attr.u32RefNum = 1;/* 0: default; number of refrence frame*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrH264e, &stH264Attr, sizeof(VENC_ATTR_H264_S));
            if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
                stH264Cbr.u32Gop            = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Cbr.u32StatTime       = 1; /* stream rate statics time(s) */
                stH264Cbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* input (vi) frame rate */
                stH264Cbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* target frame rate */
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH264Cbr.u32BitRate = 256; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH264Cbr.u32BitRate = 512;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH264Cbr.u32BitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH264Cbr.u32BitRate = 1024 * 2;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH264Cbr.u32BitRate = 1024 * 4;
                        break;
                    default :
                        stH264Cbr.u32BitRate = 1024 * 4;
                        break;
                }
                stH264Cbr.u32FluctuateLevel = 1; /* average bit rate */
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264Cbr, &stH264Cbr, sizeof(VENC_ATTR_H264_CBR_S));
            }
            else if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;
                stH264FixQp.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264FixQp.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264FixQp.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264FixQp.u32IQp = 20;
                stH264FixQp.u32PQp = 23;
                stH264FixQp.u32BQp = 23;
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264FixQp, &stH264FixQp, sizeof(VENC_ATTR_H264_FIXQP_S));
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
                stH264Vbr.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Vbr.u32StatTime = 1;
                stH264Vbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Vbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Vbr.u32MinQp = 10;
                stH264Vbr.u32MinIQp = 10;
                stH264Vbr.u32MaxQp = 40;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH264Vbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH264Vbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH264Vbr.u32MaxBitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH264Vbr.u32MaxBitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH264Vbr.u32MaxBitRate = 1024 * 6;
                        break;
                    default :
                        stH264Vbr.u32MaxBitRate = 1024 * 4;
                        break;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264Vbr, &stH264Vbr, sizeof(VENC_ATTR_H264_VBR_S));
            }
            else if (SAMPLE_RC_AVBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264AVBR;
                stH264AVbr.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264AVbr.u32StatTime = 1;
                stH264AVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264AVbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH264AVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH264AVbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH264AVbr.u32MaxBitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH264AVbr.u32MaxBitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH264AVbr.u32MaxBitRate = 1024 * 4;
                        break;
                    case PIC_UHD4K:
                        stH264AVbr.u32MaxBitRate = 1024 * 8;
                        break;
                    default :
                        stH264AVbr.u32MaxBitRate = 1024 * 4;
                        break;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264AVbr, &stH264AVbr, sizeof(VENC_ATTR_H264_AVBR_S));
            }
            else
            {
                return HI_FAILURE;
            }
        }
        break;
        case PT_MJPEG:
        {
            stMjpegAttr.u32MaxPicWidth = stPicSize.u32Width;
            stMjpegAttr.u32MaxPicHeight = stPicSize.u32Height;
            stMjpegAttr.u32PicWidth = stPicSize.u32Width;
            stMjpegAttr.u32PicHeight = stPicSize.u32Height;
            stMjpegAttr.u32BufSize = stPicSize.u32Width * stPicSize.u32Height * 3;
            stMjpegAttr.bByFrame = HI_TRUE;  /*get stream mode is field mode  or frame mode*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrMjpege, &stMjpegAttr, sizeof(VENC_ATTR_MJPEG_S));
            if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGFIXQP;
                stMjpegeFixQp.u32Qfactor        = 90;
                stMjpegeFixQp.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stMjpegeFixQp.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                memcpy(&stVencChnAttr.stRcAttr.stAttrMjpegeFixQp, &stMjpegeFixQp,
                       sizeof(VENC_ATTR_MJPEG_FIXQP_S));
            }
            else if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32StatTime       = 1;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32FluctuateLevel = 1;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 384 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 768 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 3 * 3;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 5 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 10 * 3;
                        break;
                    default :
                        stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 10 * 3;
                        break;
                }
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGVBR;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32StatTime = 1;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.fr32DstFrmRate = 5;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MinQfactor = 50;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxQfactor = 95;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 2 * 3;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 3 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 6 * 3;
                        break;
                    case PIC_UHD4K:
                        stH264Cbr.u32BitRate 								 = 1024 * 12 * 3;
                        break;
                    default :
                        stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 4 * 3;
                        break;
                }
            }
            else
            {
                SAMPLE_PRT("cann't support other mode in this version!\n");
                return HI_FAILURE;
            }
        }
        break;
        case PT_JPEG:
            stJpegAttr.u32PicWidth  = stPicSize.u32Width;
            stJpegAttr.u32PicHeight = stPicSize.u32Height;
            stJpegAttr.u32MaxPicWidth  = stPicSize.u32Width;
            stJpegAttr.u32MaxPicHeight = stPicSize.u32Height;
            stJpegAttr.u32BufSize   = stPicSize.u32Width * stPicSize.u32Height * 3;
            stJpegAttr.bByFrame     = HI_TRUE;/*get stream mode is field mode  or frame mode*/
            stJpegAttr.bSupportDCF  = HI_FALSE;
            memcpy(&stVencChnAttr.stVeAttr.stAttrJpege, &stJpegAttr, sizeof(VENC_ATTR_JPEG_S));
            break;
        case PT_H265:
        {
            stH265Attr.u32MaxPicWidth = stPicSize.u32Width;
            stH265Attr.u32MaxPicHeight = stPicSize.u32Height;
            stH265Attr.u32PicWidth = stPicSize.u32Width;/*the picture width*/
            stH265Attr.u32PicHeight = stPicSize.u32Height;/*the picture height*/
            stH265Attr.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
            if (u32Profile >= 1)
            { stH265Attr.u32Profile = 0; }/*0:MP; */
            else
            { stH265Attr.u32Profile  = u32Profile; }/*0:MP*/
            stH265Attr.bByFrame = HI_TRUE;/*get stream mode is slice mode or frame mode?*/
            //stH265Attr.u32BFrameNum = 0;/* 0: not support B frame; >=1: number of B frames */
            //stH265Attr.u32RefNum = 1;/* 0: default; number of refrence frame*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrH265e, &stH265Attr, sizeof(VENC_ATTR_H265_S));
            if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
                stH265Cbr.u32Gop            = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Cbr.u32StatTime       = 1; /* stream rate statics time(s) */
                stH265Cbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* input (vi) frame rate */
                stH265Cbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* target frame rate */
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH265Cbr.u32BitRate = 256; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH265Cbr.u32BitRate = 512;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH265Cbr.u32BitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH265Cbr.u32BitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH265Cbr.u32BitRate = 1024 * 4;
                        break;
                    case PIC_UHD4K:
                        stH265Cbr.u32BitRate = 1024 * 8;
                        break;
                    default :
                        stH265Cbr.u32BitRate = 1024 * 4;
                        break;
                }
                stH265Cbr.u32FluctuateLevel = 1; /* average bit rate */
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265Cbr, &stH265Cbr, sizeof(VENC_ATTR_H265_CBR_S));
            }
            else if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265FIXQP;
                stH265FixQp.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265FixQp.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265FixQp.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265FixQp.u32IQp = 20;
                stH265FixQp.u32PQp = 23;
                stH265FixQp.u32BQp = 25;
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265FixQp, &stH265FixQp, sizeof(VENC_ATTR_H265_FIXQP_S));
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
                stH265Vbr.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Vbr.u32StatTime = 1;
                stH265Vbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Vbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Vbr.u32MinQp  = 10;
                stH265Vbr.u32MinIQp = 10;
                stH265Vbr.u32MaxQp  = 40;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH265Vbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH265Vbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH265Vbr.u32MaxBitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH265Vbr.u32MaxBitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH265Vbr.u32MaxBitRate = 1024 * 6;
                        break;
                    case PIC_UHD4K:
                        stH265Vbr.u32MaxBitRate = 1024 * 8;
                        break;
                    default :
                        stH265Vbr.u32MaxBitRate = 1024 * 4;
                        break;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265Vbr, &stH265Vbr, sizeof(VENC_ATTR_H265_VBR_S));
            }
            else if (SAMPLE_RC_AVBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265AVBR;
                stH265AVbr.u32Gop = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265AVbr.u32StatTime = 1;
                stH265AVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265AVbr.fr32DstFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                switch (enSize)
                {
                    case PIC_QCIF:
                        stH265AVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                        break;
                    case PIC_QVGA:    /* 320 * 240 */
                    case PIC_CIF:
                        stH265AVbr.u32MaxBitRate = 512 * 3;
                        break;
                    case PIC_D1:
                    case PIC_VGA:	   /* 640 * 480 */
                        stH265AVbr.u32MaxBitRate = 1024 * 2;
                        break;
                    case PIC_HD720:   /* 1280 * 720 */
                        stH265AVbr.u32MaxBitRate = 1024 * 3;
                        break;
                    case PIC_HD1080:  /* 1920 * 1080 */
                        stH265AVbr.u32MaxBitRate = 1024 * 4;
                        break;
                    case PIC_UHD4K:
                        stH265AVbr.u32MaxBitRate = 1024 * 8;
                        break;
                    default :
                        stH265AVbr.u32MaxBitRate = 1024 * 4;
                        break;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264AVbr, &stH265AVbr, sizeof(VENC_ATTR_H265_AVBR_S));
            }
            else
            {
                return HI_FAILURE;
            }
        }
        break;
        default:
            return HI_ERR_VENC_NOT_SUPPORT;
    }

    if(PT_MJPEG == enType || PT_JPEG == enType )
    {
            stVencChnAttr.stGopAttr.enGopMode  = VENC_GOPMODE_NORMALP;
            stVencChnAttr.stGopAttr.stNormalP.s32IPQpDelta = 0;
    }
    else
    {
        memcpy(&stVencChnAttr.stGopAttr,pstGopAttr,sizeof(VENC_GOP_ATTR_S));
    }

    s32Ret = HI_MPI_VENC_CreateChn(m_Venc_Chn, &stVencChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_CreateChn [%d] faild with %#x! ===\n", \
                   m_Venc_Chn, s32Ret);
        return s32Ret;
    }    /******************************************
     step 2:  Start Recv Venc Pictures
    ******************************************/
    s32Ret = HI_MPI_VENC_StartRecvPic(m_Venc_Chn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_StartRecvPic faild with%#x! \n", s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/******************************************************************************
* funciton : Stop venc ( stream mode -- H264, MJPEG )
******************************************************************************/
HI_S32 Sample_Common_Venc::SAMPLE_COMM_VENC_Stop()
{
    HI_S32 s32Ret;
    /******************************************
     step 1:  Stop Recv Pictures
    ******************************************/
    m_Venc_MaxTab[m_Venc_Chn] = 1;
    s32Ret = HI_MPI_VENC_StopRecvPic(m_Venc_Chn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_StopRecvPic vechn[%d] failed with %#x!\n", \
                   m_Venc_Chn, s32Ret);
        return HI_FAILURE;
    }    /******************************************
     step 2:  Distroy Venc Channel
    ******************************************/
    s32Ret = HI_MPI_VENC_DestroyChn(m_Venc_Chn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_DestroyChn vechn[%d] failed with %#x!\n", \
                   m_Venc_Chn, s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

