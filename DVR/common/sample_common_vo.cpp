#include "sample_common_vo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>

HI_HDMI_CALLBACK_FUNC_S Sample_Common_Vo::stCallbackFunc = {nullptr,nullptr};
HDMI_ARGS_S      Sample_Common_Vo::stHdmiArgs = {HI_HDMI_ID_0};


Sample_Common_Vo::Sample_Common_Vo():m_enVoMode(VO_MODE_9MUX)
{

}

Sample_Common_Vo::Sample_Common_Vo(VO_DEV VoDev, VO_LAYER VoLayer, SAMPLE_VO_MODE_E enVoMode):
    m_enVoMode(enVoMode),m_VoDev(VoDev),m_VoLayer(VoLayer)
{

}
Sample_Common_Vo::Sample_Common_Vo(const Sample_Common_Vo &Sample_Vo)
{

    this->m_VoDev        = Sample_Vo.m_VoDev;
    this->m_VoLayer      = Sample_Vo.m_VoLayer;
    this->m_enVoMode     = Sample_Vo.m_enVoMode;
//    this->stCallbackFunc = Sample_Vo.stCallbackFunc;
//    this->stHdmiArgs     = Sample_Vo.stHdmiArgs;
}

Sample_Common_Vo & Sample_Common_Vo::operator=(const Sample_Common_Vo &Sample_Vo)
{
    if(this == &Sample_Vo)
        return *this;

    m_VoDev        = Sample_Vo.m_VoDev;
    m_VoLayer      = Sample_Vo.m_VoLayer;
    m_enVoMode     = Sample_Vo.m_enVoMode;
//    stCallbackFunc = Sample_Vo.stCallbackFunc;
//    stHdmiArgs     = Sample_Vo.stHdmiArgs;

}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_SetDev(VO_DEV VoDev,VO_LAYER VoLayer,SAMPLE_VO_MODE_E enVoMode)
{
    m_VoDev = VoDev;
    m_VoLayer = VoLayer;
    m_enVoMode = enVoMode;


}

VO_DEV Sample_Common_Vo::getVoDev() const
{
    return m_VoDev;
}

VO_LAYER Sample_Common_Vo::getVoLayer() const
{
    return m_VoLayer;
}

SAMPLE_VO_MODE_E Sample_Common_Vo::getVoMode() const
{
    return m_enVoMode;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_GetWH(VO_INTF_SYNC_E enIntfSync, HI_U32 *pu32W,HI_U32 *pu32H, HI_U32 *pu32Frm)
{
    switch (enIntfSync)
    {
        case VO_OUTPUT_PAL       :  *pu32W = 720;  *pu32H = 576;  *pu32Frm = 25; break;
        case VO_OUTPUT_NTSC      :  *pu32W = 720;  *pu32H = 480;  *pu32Frm = 30; break;
        case VO_OUTPUT_576P50    :  *pu32W = 720;  *pu32H = 576;  *pu32Frm = 50; break;
        case VO_OUTPUT_480P60    :  *pu32W = 720;  *pu32H = 480;  *pu32Frm = 60; break;
        case VO_OUTPUT_800x600_60:  *pu32W = 800;  *pu32H = 600;  *pu32Frm = 60; break;
        case VO_OUTPUT_720P50    :  *pu32W = 1280; *pu32H = 720;  *pu32Frm = 50; break;
        case VO_OUTPUT_720P60    :  *pu32W = 1280; *pu32H = 720;  *pu32Frm = 60; break;
        case VO_OUTPUT_1080I50   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 25; break;
        case VO_OUTPUT_1080I60   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 30; break;
        case VO_OUTPUT_1080P24   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 24; break;
        case VO_OUTPUT_1080P25   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 25; break;
        case VO_OUTPUT_1080P30   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 30; break;
        case VO_OUTPUT_1080P50   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 50; break;
        case VO_OUTPUT_1080P60   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 60; break;
        case VO_OUTPUT_1024x768_60:  *pu32W = 1024; *pu32H = 768;  *pu32Frm = 60; break;
        case VO_OUTPUT_1280x1024_60: *pu32W = 1280; *pu32H = 1024; *pu32Frm = 60; break;
        case VO_OUTPUT_1366x768_60:  *pu32W = 1366; *pu32H = 768;  *pu32Frm = 60; break;
        case VO_OUTPUT_1440x900_60:  *pu32W = 1440; *pu32H = 900;  *pu32Frm = 60; break;
        case VO_OUTPUT_1280x800_60:  *pu32W = 1280; *pu32H = 800;  *pu32Frm = 60; break;
        case VO_OUTPUT_1600x1200_60: *pu32W = 1600; *pu32H = 1200; *pu32Frm = 60; break;
        case VO_OUTPUT_1680x1050_60: *pu32W = 1680; *pu32H = 1050; *pu32Frm = 60; break;
        case VO_OUTPUT_1920x1200_60: *pu32W = 1920; *pu32H = 1200; *pu32Frm = 60; break;
        case VO_OUTPUT_3840x2160_30: *pu32W = 3840; *pu32H = 2160; *pu32Frm = 30; break;
        case VO_OUTPUT_3840x2160_60: *pu32W = 3840; *pu32H = 2160; *pu32Frm = 60; break;
        case VO_OUTPUT_USER    :     *pu32W = 720;  *pu32H = 576;  *pu32Frm = 25; break;
        default:
            SAMPLE_PRT("vo enIntfSync not support!\n");
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/******************************************************************************
* function : Set system memory location
******************************************************************************/
HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_MemConfig( HI_CHAR *pcMmzName)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stMppChnVO;

    /* config vo dev */
    stMppChnVO.enModId  = HI_ID_VOU;
    stMppChnVO.s32DevId = m_VoDev;
    stMppChnVO.s32ChnId = 0;
    s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVO, pcMmzName);
    if (s32Ret)
    {
        SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StartDev(VO_PUB_ATTR_S *pstPubAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_VO_SetPubAttr(m_VoDev, pstPubAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_Enable(m_VoDev);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StopDev()
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_VO_Disable(m_VoDev);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StartLayer(const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    s32Ret = HI_MPI_VO_SetVideoLayerAttr(m_VoLayer, pstLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_EnableVideoLayer(m_VoLayer);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StopLayer()
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_VO_DisableVideoLayer(m_VoLayer);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_GetLayerAttr(VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
{
    return HI_MPI_VO_GetVideoLayerAttr(m_VoLayer,pstLayerAttr);
}
HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_SetChnFrameRate(int Chn, int rate)
{
    return HI_MPI_VO_SetChnFrameRate(m_VoLayer, Chn, rate);
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StartChn(SAMPLE_VO_MODE_E enMode)
{
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32WndNum = 0;
    HI_U32 u32Square = 0;
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    VO_CHN_ATTR_S stChnAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
//    VO_BORDER_S stBorder;

    switch (enMode)
    {
        case VO_MODE_1MUX:
            u32WndNum = 1;
            u32Square = 1;
            break;
        case VO_MODE_4MUX:
            u32WndNum = 4;
            u32Square = 2;
            break;
        case VO_MODE_9MUX:
            u32WndNum = 9;
            u32Square = 3;
            break;
        case VO_MODE_16MUX:
            u32WndNum = 16;
            u32Square = 4;
            break;
        default:
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_GetVideoLayerAttr(m_VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    u32Width = stLayerAttr.stImageSize.u32Width;
    u32Height = stLayerAttr.stImageSize.u32Height;

    for (i=0; i<u32WndNum; i++)
    {
        stChnAttr.stRect.s32X       = ALIGN_BACK((u32Width/u32Square) * (i%u32Square), 2);
        stChnAttr.stRect.s32Y       = ALIGN_BACK((u32Height/u32Square) * (i/u32Square), 2);
        stChnAttr.stRect.u32Width   = ALIGN_BACK(u32Width/u32Square, 2);
        stChnAttr.stRect.u32Height  = ALIGN_BACK(u32Height/u32Square, 2);
        stChnAttr.u32Priority       = 0;
        stChnAttr.bDeflicker        = (SAMPLE_VO_LAYER_VSD0 == m_VoLayer) ? HI_TRUE : HI_FALSE;

        s32Ret = HI_MPI_VO_SetChnAttr(m_VoLayer, i, &stChnAttr);
        if (s32Ret != HI_SUCCESS)
        {
            printf("%s(%d):failed with %#x!\n",\
                   __FUNCTION__,__LINE__,  s32Ret);
            return HI_FAILURE;
        }
//        stBorder.bBorderEn = HI_FALSE;
//        s32Ret = HI_MPI_VO_SetChnBorder(m_VoLayer,i,&stBorder);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_PRT("failed with %#x!\n", s32Ret);
//            return HI_FAILURE;
//        }

        s32Ret = HI_MPI_VO_EnableChn(m_VoLayer, i);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
    }

    m_enVoMode = enMode;

    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StartChn(VO_CHN VoChn,RECT_S &pos)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    VO_CHN_ATTR_S stChnAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
//    VO_BORDER_S stBorder;


    s32Ret = HI_MPI_VO_GetVideoLayerAttr(m_VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    u32Width = stLayerAttr.stImageSize.u32Width;
    u32Height = stLayerAttr.stImageSize.u32Height;

    if(u32Width < pos.u32Width || u32Height < pos.u32Height){
        SAMPLE_PRT("failed pos and size lager than ImageSize!\n");
        return HI_FAILURE;
    }

    stChnAttr.stRect.s32X       = ALIGN_BACK(pos.s32X,2);
    stChnAttr.stRect.s32Y       = ALIGN_BACK(pos.s32Y,2);
    stChnAttr.stRect.u32Width   = ALIGN_BACK(pos.u32Width,2);
    stChnAttr.stRect.u32Height  = ALIGN_BACK(pos.u32Height,2);
    stChnAttr.u32Priority       = 0;
    stChnAttr.bDeflicker        = (SAMPLE_VO_LAYER_VSD0 == m_VoLayer) ? HI_TRUE : HI_FALSE;

    s32Ret = HI_MPI_VO_SetChnAttr(m_VoLayer, VoChn, &stChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        printf("%s(%d):failed with %#x!\n",__FUNCTION__,__LINE__,  s32Ret);
        return HI_FAILURE;
    }

//    stBorder.bBorderEn = HI_FALSE;
//    s32Ret = HI_MPI_VO_SetChnBorder(m_VoLayer,VoChn,&stBorder);
//    if (s32Ret != HI_SUCCESS)
//    {
//        SAMPLE_PRT("failed with %#x!\n", s32Ret);
//        return HI_FAILURE;
//    }

    s32Ret = HI_MPI_VO_EnableChn(m_VoLayer, VoChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }


//    m_enVoMode = VO_MODE_1MUX;
    return s32Ret;
}

#if 0
HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StartChn(SAMPLE_VO_MODE_E enMode,VO_CHN StartChn)
{
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32WndNum = 0;
    HI_U32 u32Square = 0;
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    VO_CHN_ATTR_S stChnAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
//    VO_BORDER_S stBorder;

    switch (enMode)
    {
        case VO_MODE_1MUX:
            u32WndNum = 1;
            u32Square = 1;
            break;
        case VO_MODE_4MUX:
            u32WndNum = 4;
            u32Square = 2;
            break;
        case VO_MODE_9MUX:
            u32WndNum = 9;
            u32Square = 3;
            break;
        case VO_MODE_16MUX:
            u32WndNum = 16;
            u32Square = 4;
            break;
        default:
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_GetVideoLayerAttr(m_VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    u32Width = stLayerAttr.stImageSize.u32Width;
    u32Height = stLayerAttr.stImageSize.u32Height;

    for (i=0; i<u32WndNum; i++)
    {
        stChnAttr.stRect.s32X       = ALIGN_BACK((u32Width/u32Square) * (i%u32Square), 2);
        stChnAttr.stRect.s32Y       = ALIGN_BACK((u32Height/u32Square) * (i/u32Square), 2);
        stChnAttr.stRect.u32Width   = ALIGN_BACK(u32Width/u32Square, 2);
        stChnAttr.stRect.u32Height  = ALIGN_BACK(u32Height/u32Square, 2);
        stChnAttr.u32Priority       = 0;
        stChnAttr.bDeflicker        = (SAMPLE_VO_LAYER_VSD0 == m_VoLayer) ? HI_TRUE : HI_FALSE;

        if(StartChn+i >= 8)
            return HI_SUCCESS;

        s32Ret = HI_MPI_VO_SetChnAttr(m_VoLayer, StartChn+i, &stChnAttr);
        if (s32Ret != HI_SUCCESS)
        {
            printf("%s(%d):failed with %#x!\n",\
                   __FUNCTION__,__LINE__,  s32Ret);
            return HI_FAILURE;
        }
//        stBorder.bBorderEn = HI_FALSE;
//        s32Ret = HI_MPI_VO_SetChnBorder(m_VoLayer,i,&stBorder);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_PRT("failed with %#x!\n", s32Ret);
//            return HI_FAILURE;
//        }

        s32Ret = HI_MPI_VO_EnableChn(m_VoLayer, StartChn+i);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
    }

    m_enVoMode = enMode;

    return HI_SUCCESS;
}
#endif

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_SetChnDispPos(VO_CHN VoChn, const POINT_S *pstDispPos)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VO_CHN_ATTR_S stChnAttr;
    POINT_S stDispPos;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;


    s32Ret = HI_MPI_VO_GetVideoLayerAttr(m_VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    stLayerAttr.bClusterMode = HI_TRUE;
    s32Ret = HI_MPI_VO_SetVideoLayerAttr(m_VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_GetChnAttr(m_VoLayer, VoChn, &stChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        printf("Set channel attr failed with errno %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    if(pstDispPos->s32X + stChnAttr.stRect.u32Width > stLayerAttr.stDispRect.u32Width
            || pstDispPos->s32Y + stChnAttr.stRect.u32Height > stLayerAttr.stDispRect.u32Height){
        printf("Set pos failed %#x!\n");
        return HI_FAILURE;
    }

    stDispPos.s32X = ALIGN_BACK(pstDispPos->s32X,2);
    stDispPos.s32Y = ALIGN_BACK(pstDispPos->s32Y,2);

    s32Ret = HI_MPI_VO_SetChnDispPos (m_VoLayer, VoChn, &stDispPos);
    if (s32Ret != HI_SUCCESS)
    {
        printf("Set channel display position failed with errno %#x!\n",s32Ret);
        return HI_FAILURE;
    }


    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_SetMode(SAMPLE_VO_MODE_E enMode)
{
    HI_S32 s32Ret;

    s32Ret= HI_MPI_VO_SetAttrBegin(m_VoLayer);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_VO_StopChn();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(enMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
        return HI_FAILURE;
    }
    s32Ret= HI_MPI_VO_SetAttrEnd(m_VoLayer);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start VO failed!\n");
        return HI_FAILURE;
    }


    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StopChn()
{
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32WndNum = 0;

    switch (m_enVoMode)
    {
        case VO_MODE_1MUX:
        {
            u32WndNum = 1;
            break;
        }
        case VO_MODE_4MUX:
        {
            u32WndNum = 4;
            break;
        }
        case VO_MODE_9MUX:
        {
            u32WndNum = 9;
            break;
        }
        case VO_MODE_16MUX:
        {
            u32WndNum = 16;
            break;
        }

        default:
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
    }

    for (i=0; i<u32WndNum; i++)
    {
        s32Ret = HI_MPI_VO_DisableChn(m_VoLayer, i);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
    }
    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_StopChn(VO_CHN VoChn)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_VO_DisableChn(m_VoLayer, VoChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return s32Ret;
}

//HI_S32 Sample_Common_Vo::SAMPLE_COMM_Vpss_BindVpss(VPSS_GRP VpssDestGrp,VO_CHN VoChn,VPSS_GRP VpssSrcGrp,VPSS_CHN VpssChn)
//{
//    HI_S32 s32Ret = HI_SUCCESS;
//    MPP_CHN_S stSrcChn;
//    MPP_CHN_S stDestChn;

//    stSrcChn.enModId = HI_ID_VPSS;
//    stSrcChn.s32DevId = VpssSrcGrp;
//    stSrcChn.s32ChnId = VpssChn;

//    stDestChn.enModId = HI_ID_VPSS;
//    stDestChn.s32DevId = VpssDestGrp;
//    stDestChn.s32ChnId = VoChn;

//    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
//    if (s32Ret != HI_SUCCESS)
//    {
//        SAMPLE_PRT("failed with %#x!\n", s32Ret);
//        return HI_FAILURE;
//    }

//    return s32Ret;
//}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_BindToVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn)
{

    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = VpssChn;

    stSrcChn.enModId = HI_ID_VOU;
    stSrcChn.s32DevId = m_VoLayer;
    stSrcChn.s32ChnId = VoChn;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_UnBindFromVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = VpssChn;

    stSrcChn.enModId = HI_ID_VOU;
    stSrcChn.s32DevId = m_VoLayer;
    stSrcChn.s32ChnId = VoChn;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_BindVpss(VO_CHN VoChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = m_VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_UnBindVpss(VO_CHN VoChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = m_VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_BindVi(VO_CHN VoChn, VI_CHN ViChn)
{
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId    = HI_ID_VIU;
    stSrcChn.s32DevId   = 0;
    stSrcChn.s32ChnId   = ViChn;

    stDestChn.enModId   = HI_ID_VOU;
    stDestChn.s32ChnId  = VoChn;
    stDestChn.s32DevId  = m_VoLayer;

    return HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_UnBindVi(VO_CHN VoChn)
{
    MPP_CHN_S stDestChn;

    stDestChn.enModId   = HI_ID_VOU;
    stDestChn.s32DevId  = m_VoLayer;
    stDestChn.s32ChnId  = VoChn;

    return HI_MPI_SYS_UnBind(NULL, &stDestChn);
}

HI_VOID Sample_Common_Vo::SAMPLE_COMM_VO_HdmiConvertSync(VO_INTF_SYNC_E enIntfSync,
    HI_HDMI_VIDEO_FMT_E *penVideoFmt)
{
    switch (enIntfSync)
    {
        case VO_OUTPUT_PAL:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_PAL;
            break;
        case VO_OUTPUT_NTSC:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_NTSC;
            break;
        case VO_OUTPUT_1080P24:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_24;
            break;
        case VO_OUTPUT_1080P25:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_25;
            break;
        case VO_OUTPUT_1080P30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_30;
            break;
        case VO_OUTPUT_720P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_720P_50;
            break;
        case VO_OUTPUT_720P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_720P_60;
            break;
        case VO_OUTPUT_1080I50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080i_50;
            break;
        case VO_OUTPUT_1080I60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080i_60;
            break;
        case VO_OUTPUT_1080P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_50;
            break;
        case VO_OUTPUT_1080P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_1080P_60;
            break;
        case VO_OUTPUT_576P50:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_576P_50;
            break;
        case VO_OUTPUT_480P60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_480P_60;
            break;
        case VO_OUTPUT_800x600_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_800X600_60;
            break;
        case VO_OUTPUT_1024x768_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1024X768_60;
            break;
        case VO_OUTPUT_1280x1024_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1280X1024_60;
            break;
        case VO_OUTPUT_1366x768_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1366X768_60;
            break;
        case VO_OUTPUT_1440x900_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1440X900_60;
            break;
        case VO_OUTPUT_1280x800_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1280X800_60;
            break;
        case VO_OUTPUT_1600x1200_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_VESA_1600X1200_60;
            break;
        case VO_OUTPUT_2560x1440_30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_2560x1440_30;
            break;
        case VO_OUTPUT_2560x1600_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_2560x1600_60;
            break;
        case VO_OUTPUT_3840x2160_30:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_3840X2160P_30;
            break;
        case VO_OUTPUT_3840x2160_60:
            *penVideoFmt = HI_HDMI_VIDEO_FMT_3840X2160P_60;
            break;
        default :
            SAMPLE_PRT("Unkonw VO_INTF_SYNC_E value!\n");
            break;
    }

    return;
}


HI_VOID Sample_Common_Vo::HDMI_HotPlug_Proc(HI_VOID *pPrivateData)
{
    HDMI_ARGS_S    stArgs;
    HI_S32         s32Ret = HI_FAILURE;
    HI_HDMI_ATTR_S stAttr;
    HI_HDMI_SINK_CAPABILITY_S stCaps;

    //printf("EVENT: HPD\n");

    CHECK_POINTER_NO_RET(pPrivateData);

    memset(&stAttr, 0, sizeof(HI_HDMI_ATTR_S));
    memset(&stArgs, 0, sizeof(HDMI_ARGS_S));
    memcpy(&stArgs, pPrivateData, sizeof(HDMI_ARGS_S));
    memset(&stCaps, 0, sizeof(HI_HDMI_SINK_CAPABILITY_S));


    s32Ret = HI_MPI_HDMI_GetSinkCapability(stArgs.enHdmi, &stCaps);
    if(s32Ret != HI_SUCCESS)
    {
        printf("get sink caps failed!\n");
    }
    else
    {
        printf("get sink caps success!\n");
    }

    s32Ret = HI_MPI_HDMI_GetAttr(stArgs.enHdmi, &stAttr);
    CHECK_RET_SUCCESS_NO_RET(s32Ret);

    s32Ret = HI_MPI_HDMI_SetAttr(stArgs.enHdmi, &stAttr);
    CHECK_RET_SUCCESS_NO_RET(s32Ret);

    HI_MPI_HDMI_Start(stArgs.enHdmi);

    return;
}

HI_VOID Sample_Common_Vo::HDMI_UnPlug_Proc(HI_VOID *pPrivateData)
{
    HDMI_ARGS_S  stArgs;

    //printf("EVENT: UN-HPD\n");

    CHECK_POINTER_NO_RET(pPrivateData);

    memset(&stArgs, 0, sizeof(HDMI_ARGS_S));
    memcpy(&stArgs, pPrivateData, sizeof(HDMI_ARGS_S));

    HI_MPI_HDMI_Stop(stArgs.enHdmi);

    return;
}

HI_VOID Sample_Common_Vo::HDMI_EventCallBack(HI_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData)
{
    switch ( event )
    {
        case HI_HDMI_EVENT_HOTPLUG:
            HDMI_HotPlug_Proc(pPrivateData);
            break;
        case HI_HDMI_EVENT_NO_PLUG:
            HDMI_UnPlug_Proc(pPrivateData);
            break;
        default:
            break;
    }

    return;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_HdmiStart(VO_INTF_SYNC_E enIntfSync)
{
    HI_HDMI_ATTR_S      stAttr;
    HI_HDMI_VIDEO_FMT_E enVideoFmt;

    SAMPLE_COMM_VO_HdmiConvertSync(enIntfSync, &enVideoFmt);

    HI_MPI_HDMI_Init();

    stHdmiArgs.enHdmi = HI_HDMI_ID_0;
    stCallbackFunc.pfnHdmiEventCallback = HDMI_EventCallBack;
    stCallbackFunc.pPrivateData = &stHdmiArgs;

    HI_MPI_HDMI_Open(HI_HDMI_ID_0);

    HI_MPI_HDMI_RegCallbackFunc(HI_HDMI_ID_0, &stCallbackFunc);

    HI_MPI_HDMI_GetAttr(HI_HDMI_ID_0, &stAttr);

    stAttr.bEnableHdmi = HI_TRUE;

    stAttr.bEnableVideo = HI_TRUE;
    stAttr.enVideoFmt = enVideoFmt;

    stAttr.enVidOutMode = HI_HDMI_VIDEO_MODE_YCBCR444;
    stAttr.enDeepColorMode = HI_HDMI_DEEP_COLOR_OFF;
    stAttr.bxvYCCMode = HI_FALSE;

    stAttr.bEnableAudio = HI_FALSE;
    stAttr.enSoundIntf = HI_HDMI_SND_INTERFACE_I2S;
    stAttr.bIsMultiChannel = HI_FALSE;

    stAttr.enBitDepth = HI_HDMI_BIT_DEPTH_16;

    stAttr.bEnableAviInfoFrame = HI_TRUE;
    stAttr.bEnableAudInfoFrame = HI_TRUE;
    stAttr.bEnableSpdInfoFrame = HI_FALSE;
    stAttr.bEnableMpegInfoFrame = HI_FALSE;

    stAttr.bDebugFlag = HI_FALSE;
    stAttr.bHDCPEnable = HI_FALSE;

    stAttr.b3DEnable = HI_FALSE;

    HI_MPI_HDMI_SetAttr(HI_HDMI_ID_0, &stAttr);

    //HI_MPI_HDMI_Start(HI_HDMI_ID_0);

    printf("HDMI start success.\n");
    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vo::SAMPLE_COMM_VO_HdmiStop(HI_VOID)
{
    HI_MPI_HDMI_Stop(HI_HDMI_ID_0);
    HI_MPI_HDMI_Close(HI_HDMI_ID_0);
    HI_MPI_HDMI_DeInit();

    return HI_SUCCESS;
}

