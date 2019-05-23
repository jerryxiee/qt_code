#include "sample_common_vio.h"
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

VI_DEV_ATTR_S DEV_ATTR_BT656D1_4MUX =
{
    /*interface mode*/
    VI_MODE_BT656,
    /*work mode, 1/2/4 multiplex*/
    VI_WORK_MODE_4Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},

    /* for single/double edge, must be set when double edge*/
    VI_CLK_EDGE_SINGLE_UP,

    /*AdChnId*/
    {-1, -1, -1, -1},
    /*enDataSeq, just support yuv*/
    VI_INPUT_DATA_YVYU,
    /*sync info*/
    {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_FIELD, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_VALID_SINGAL,VI_VSYNC_VALID_NEG_HIGH,

    /*timing info*/
    /*hsync_hfb    hsync_act    hsync_hhb*/
    {0,            0,        0,
    /*vsync0_vhb vsync0_act vsync0_hhb*/
     0,            0,        0,
    /*vsync1_vhb vsync1_act vsync1_hhb*/
     0,            0,            0}
    },
    /*whether use isp*/
    VI_PATH_BYPASS,
    /*data type*/
    VI_DATA_TYPE_YUV
};

VI_DEV_ATTR_S DEV_ATTR_TP2823_720P_2MUX_BASE =
{
    /*interface mode*/
    VI_MODE_BT656,
    /*work mode, 1/2/4 multiplex*/
    VI_WORK_MODE_2Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},

    /* for single/double edge, must be set when double edge*/
    VI_CLK_EDGE_SINGLE_UP,

    /*AdChnId*/
    {-1, -1, -1, -1},
    /*enDataSeq, just support yuv*/
    VI_INPUT_DATA_YVYU,
    /*sync info*/
    {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_FIELD, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_VALID_SINGAL,VI_VSYNC_VALID_NEG_HIGH,

    /*timing info*/
    /*hsync_hfb    hsync_act    hsync_hhb*/
    {0,            0,        0,
    /*vsync0_vhb vsync0_act vsync0_hhb*/
     0,            0,        0,
    /*vsync1_vhb vsync1_act vsync1_hhb*/
     0,            0,            0}
    },
    /*whether use isp*/
    VI_PATH_BYPASS,
    /*data type*/
    VI_DATA_TYPE_YUV
};


VI_DEV_ATTR_S DEV_ATTR_TP2823_720P_1MUX_BASE =
{
    /*interface mode*/
    VI_MODE_BT656,
    /*work mode, 1/2/4 multiplex*/
    VI_WORK_MODE_1Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},

    /* for single/double edge, must be set when double edge*/
    VI_CLK_EDGE_SINGLE_UP,

    /*AdChnId*/
    {-1, -1, -1, -1},
    /*enDataSeq, just support yuv*/
    VI_INPUT_DATA_YVYU,
    /*sync info*/
    {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_FIELD, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_VALID_SINGAL,VI_VSYNC_VALID_NEG_HIGH,

    /*timing info*/
    /*hsync_hfb    hsync_act    hsync_hhb*/
    {0,            0,        0,
    /*vsync0_vhb vsync0_act vsync0_hhb*/
     0,            0,        0,
    /*vsync1_vhb vsync1_act vsync1_hhb*/
     0,            0,            0}
    },
    /*whether use isp*/
    VI_PATH_BYPASS,
    /*data type*/
    VI_DATA_TYPE_YUV
};

VI_DEV_ATTR_S DEV_ATTR_7441_BT1120_STANDARD_BASE =
{
    /*interface mode*/
    VI_MODE_BT1120_STANDARD,
    /*work mode, 1/2/4 multiplex*/
    VI_WORK_MODE_1Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0xFF0000},

    /* for single/double edge, must be set when double edge*/
    VI_CLK_EDGE_SINGLE_UP,

    /*AdChnId*/
    {-1, -1, -1, -1},
    /*enDataSeq, just support yuv*/
    VI_INPUT_DATA_UVUV,

    /*sync info*/
    {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_PULSE, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_NORM_PULSE,VI_VSYNC_VALID_NEG_HIGH,

    /*timing info*/
    /*hsync_hfb    hsync_act    hsync_hhb*/
    {0,            1920,        0,
    /*vsync0_vhb vsync0_act vsync0_hhb*/
     0,            1080,        0,
    /*vsync1_vhb vsync1_act vsync1_hhb*/
     0,            0,            0}
    },
    /*whether use isp*/
    VI_PATH_BYPASS,
    /*data type*/
    VI_DATA_TYPE_YUV

};

HI_HDMI_CALLBACK_FUNC_S Sample_Common_Vio::stCallbackFunc = {nullptr,nullptr};
HDMI_ARGS_S      Sample_Common_Vio::stHdmiArgs = {HI_HDMI_ID_0};

Sample_Common_Vio::Sample_Common_Vio():
    m_enVoMode(VO_MODE_9MUX),m_VoDev(SAMPLE_VO_DEV_DHD0),m_VoLayer(SAMPLE_VO_LAYER_VHD0)
{

}

Sample_Common_Vio::Sample_Common_Vio(VO_DEV VoDev,VO_LAYER VoLayer):
    m_enVoMode(VO_MODE_9MUX),m_VoDev(VoDev),m_VoLayer(VoLayer)
{

}
Sample_Common_Vio::Sample_Common_Vio(Sample_Common_Vio &Sample_Vo)
{

    this->m_VoDev        = Sample_Vo.m_VoDev;
    this->m_VoLayer      = Sample_Vo.m_VoLayer;
    this->m_enVoMode     = Sample_Vo.m_enVoMode;
    this->stCallbackFunc = Sample_Vo.stCallbackFunc;
    this->stHdmiArgs     = Sample_Vo.stHdmiArgs;
}

Sample_Common_Vio & Sample_Common_Vio::operator=(const Sample_Common_Vio &Sample_Vo)
{
    if(this == &Sample_Vo)
        return *this;

    m_VoDev        = Sample_Vo.m_VoDev;
    m_VoLayer      = Sample_Vo.m_VoLayer;
    m_enVoMode     = Sample_Vo.m_enVoMode;
    stCallbackFunc = Sample_Vo.stCallbackFunc;
    stHdmiArgs     = Sample_Vo.stHdmiArgs;

}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_GetWH(VO_INTF_SYNC_E enIntfSync, HI_U32 *pu32W,HI_U32 *pu32H, HI_U32 *pu32Frm)
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
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_MemConfig( HI_CHAR *pcMmzName)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StartDev(VO_PUB_ATTR_S *pstPubAttr)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StopDev()
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StartLayer(const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StopLayer()
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StartChn(SAMPLE_VO_MODE_E enMode)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StartChn(VO_CHN VoChn,RECT_S &pos)
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
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StartChn(SAMPLE_VO_MODE_E enMode,VO_CHN StartChn)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_SetChnDispPos(VO_CHN VoChn, const POINT_S *pstDispPos)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_SetMode(SAMPLE_VO_MODE_E enMode)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StopChn()
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_StopChn(VO_CHN VoChn)
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

//HI_S32 Sample_Common_Vio::SAMPLE_COMM_Vpss_BindVpss(VPSS_GRP VpssDestGrp,VO_CHN VoChn,VPSS_GRP VpssSrcGrp,VPSS_CHN VpssChn)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_BindVpss(VO_CHN VoChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_UnBindVpss(VO_CHN VoChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_BindVi(VO_CHN VoChn, VI_CHN ViChn)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_UnBindVi(VO_CHN VoChn)
{
    MPP_CHN_S stDestChn;

    stDestChn.enModId   = HI_ID_VOU;
    stDestChn.s32DevId  = m_VoLayer;
    stDestChn.s32ChnId  = VoChn;

    return HI_MPI_SYS_UnBind(NULL, &stDestChn);
}

HI_VOID Sample_Common_Vio::SAMPLE_COMM_VO_HdmiConvertSync(VO_INTF_SYNC_E enIntfSync,
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


HI_VOID Sample_Common_Vio::HDMI_HotPlug_Proc(HI_VOID *pPrivateData)
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

HI_VOID Sample_Common_Vio::HDMI_UnPlug_Proc(HI_VOID *pPrivateData)
{
    HDMI_ARGS_S  stArgs;

    //printf("EVENT: UN-HPD\n");

    CHECK_POINTER_NO_RET(pPrivateData);

    memset(&stArgs, 0, sizeof(HDMI_ARGS_S));
    memcpy(&stArgs, pPrivateData, sizeof(HDMI_ARGS_S));

    HI_MPI_HDMI_Stop(stArgs.enHdmi);

    return;
}

HI_VOID Sample_Common_Vio::HDMI_EventCallBack(HI_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_HdmiStart(VO_INTF_SYNC_E enIntfSync)
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

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VO_HdmiStop(HI_VOID)
{
    HI_MPI_HDMI_Stop(HI_HDMI_ID_0);
    HI_MPI_HDMI_Close(HI_HDMI_ID_0);
    HI_MPI_HDMI_DeInit();

    return HI_SUCCESS;
}


HI_S32 Sample_Common_Vio::SAMPLE_TW2865_CfgV(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode)
{
#if 0
    int fd, i;
    int video_mode;
    tw2865_video_norm stVideoMode;
    tw2865_work_mode work_mode;

    int chip_cnt = 4;

    fd = open(TW2865_FILE, O_RDWR);
    if (fd < 0)
    {
        SAMPLE_PRT("open 2865 (%s) fail\n", TW2865_FILE);
        return -1;
    }

    video_mode = (VIDEO_ENCODING_MODE_PAL == enVideoMode) ? TW2865_PAL : TW2865_NTSC ;

    for (i=0; i<chip_cnt; i++)
    {
        stVideoMode.chip    = i;
        stVideoMode.mode    = video_mode;
        if (ioctl(fd, TW2865_SET_VIDEO_NORM, &stVideoMode))
        {
            SAMPLE_PRT("set tw2865(%d) video mode fail\n", i);
            close(fd);
            return -1;
        }
    }

    for (i=0; i<chip_cnt; i++)
    {
        work_mode.chip = i;
        if (VI_WORK_MODE_4Multiplex== enWorkMode)
        {
            work_mode.mode = TW2865_4D1_MODE;
        }
        else if (VI_WORK_MODE_2Multiplex== enWorkMode)
        {
            work_mode.mode = TW2865_2D1_MODE;
        }
        else if (VI_WORK_MODE_1Multiplex == enWorkMode)
        {
            work_mode.mode = TW2865_1D1_MODE;
        }
        else
        {
            SAMPLE_PRT("work mode not support\n");
            return -1;
        }
        ioctl(fd, TW2865_SET_WORK_MODE, &work_mode);
    }

    close(fd);
#endif
    return 0;
}

HI_S32 Sample_Common_Vio::SAMPLE_TW2960_CfgV(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode)
{
#if 0
    int fd, i;
    int video_mode;
    tw2865_video_norm stVideoMode;
    tw2865_work_mode work_mode;

    int chip_cnt = 4;

    fd = open(TW2960_FILE, O_RDWR);
    if (fd < 0)
    {
        SAMPLE_PRT("open 2960(%s) fail\n", TW2960_FILE);
        return -1;
    }

    video_mode = (VIDEO_ENCODING_MODE_PAL == enVideoMode) ? TW2960_PAL : TW2960_NTSC ;

    for (i=0; i<chip_cnt; i++)
    {
        stVideoMode.chip    = i;
        stVideoMode.mode    = video_mode;
        if (ioctl(fd, TW2960_SET_VIDEO_NORM, &stVideoMode))
        {
            SAMPLE_PRT("set tw2865(%d) video mode fail\n", i);
            close(fd);
            return -1;
        }
    }

    for (i=0; i<chip_cnt; i++)
    {
        work_mode.chip = i;
        if (VI_WORK_MODE_4Multiplex== enWorkMode)
        {
            work_mode.mode = TW2960_4D1_MODE;
        }
        else if (VI_WORK_MODE_2Multiplex== enWorkMode)
        {
            work_mode.mode = TW2960_2D1_MODE;
        }
        else if (VI_WORK_MODE_1Multiplex == enWorkMode)
        {
            work_mode.mode = TW2960_1D1_MODE;
        }
        else
        {
            SAMPLE_PRT("work mode not support\n");
            return -1;
        }
        ioctl(fd, TW2960_SET_WORK_MODE, &work_mode);
    }

    close(fd);
#endif
    return 0;
}


HI_S32 Sample_Common_Vio::SAMPLE_NVP6134_CfgV(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enVideoMode, VI_WORK_MODE_E enWorkMode)
{
#ifndef HI_FPGA
    int fd, i,j;
    int video_mode;
    nvp6134_opt_mode optmode;
    nvp6134_chn_mode schnmode;

    int chip_cnt = 2;

    fd = open(NVP6134_FILE, O_RDWR);

    if (fd < 0)
    {
        printf("open nvp6134 (%s) fail\n", NVP6134_FILE);
        return -1;
    }

    video_mode = (VIDEO_ENCODING_MODE_PAL == enVideoMode) ? 1 : 0 ;

    switch(enWorkMode)
    {
        case VI_WORK_MODE_4Multiplex:
            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode =  NVP6134_VI_960H;
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                for(j=0;j<4;j++)
                {
                    optmode.chipsel = i;
                    optmode.portsel = j;
                    optmode.portmode = NVP6134_OUTMODE_4MUX_SD;
                    optmode.chid = j%2;
                    ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                }
            }
            break;
        case VI_WORK_MODE_1Multiplex:
            if(enViMode == SAMPLE_VI_MODE_8_720P){
                for(i=0;i<chip_cnt*4;i++)
                {
                    schnmode.ch = i;
                    schnmode.vformat = video_mode;
                    schnmode.chmode = NVP6134_VI_720P_2530;//NVP6134_VI_1080P_2530;
                    ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
                }
                for(i=0;i<chip_cnt;i++)
                {
                    for(j=0;j<4;j++)
                    {
                        optmode.chipsel = i;
                        optmode.portsel = j;
                        optmode.portmode = NVP6134_OUTMODE_1MUX_HD;//NVP6134_OUTMODE_1MUX_FHD;
                        optmode.chid = j;
                        ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                    }

                }
            }else if(enViMode == SAMPLE_VI_MODE_8_1080P){
                for(i=0;i<chip_cnt*4;i++)
                {
                    schnmode.ch = i;
                    schnmode.vformat = video_mode;
                    schnmode.chmode = NVP6134_VI_1080P_2530;
                    ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
                }
                for(i=0;i<chip_cnt;i++)
                {
                    for(j=0;j<4;j++)
                    {
                        optmode.chipsel = i;
                        optmode.portsel = j;
                        optmode.portmode = NVP6134_OUTMODE_1MUX_FHD;
                        optmode.chid = j;
                        ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                    }

                }
            }
            break;
        case VI_WORK_MODE_2Multiplex:
            for(i=0;i<chip_cnt*4;i++)
            {
                schnmode.ch = i;
                schnmode.vformat = video_mode;
                schnmode.chmode = i%2?NVP6134_VI_720P_2530:NVP6134_VI_1920H;
                ioctl(fd, IOC_VDEC_SET_CHNMODE, &schnmode);
            }
            for(i=0;i<chip_cnt;i++)
            {
                for(j=0;j<4;j++)
                {
                    optmode.chipsel = i;
                    optmode.portsel = j;
                    optmode.portmode = NVP6134_OUTMODE_2MUX_HD;
                    optmode.chid = j%2;
                    ioctl(fd, IOC_VDEC_SET_OUTPORTMODE, &optmode);
                }
            }
            break;
        default:
            printf("enViMode %d not supported\n", enWorkMode);
            break;
    }

    close(fd);
    return 0;
#else
    return 0;
#endif
}




HI_S32 Sample_Common_Vio::SAMPLE_TP2823_CfgV(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode)
{
    return 0;
}

/*****************************************************************************
* function : set vi mask.
*****************************************************************************/
HI_VOID Sample_Common_Vio::SAMPLE_COMM_VI_SetMask(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr)
{
    switch (ViDev % 2)
      {
          case 0:
            pstDevAttr->au32CompMask[0] = 0xFF000000;
            if (VI_MODE_BT1120_STANDARD == pstDevAttr->enIntfMode)
            {
                pstDevAttr->au32CompMask[0] = 0xFF000000;
                pstDevAttr->au32CompMask[1] = 0xFF000000>>8;
            }
            else
            {
                pstDevAttr->au32CompMask[1] = 0x0;
            }
            break;

          case 1:
             pstDevAttr->au32CompMask[0] = 0xFF000000>>8;
             pstDevAttr->au32CompMask[1] = 0x0;
              break;
          default:
              HI_ASSERT(0);
      }

}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_Mode2Param(SAMPLE_VI_MODE_E enViMode, SAMPLE_VI_PARAM_S *pstViParam)
{
    switch (enViMode)
    {
        case SAMPLE_VI_MODE_32_D1:
        case SAMPLE_VI_MODE_32_960H:
        case SAMPLE_VI_MODE_32_1280H:
        case SAMPLE_VI_MODE_32_HALF720P:
            pstViParam->s32ViDevCnt = 4;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 16;
            pstViParam->s32ViChnInterval = 1;
            break;
        case SAMPLE_VI_MODE_16_720P:
        case SAMPLE_VI_MODE_16_1080P:
            /* use chn 0,2,4,6,8,10,12,14,16,18,20,22,24,28 */
            pstViParam->s32ViDevCnt = 8;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 16;
            pstViParam->s32ViChnInterval = 2;
            break;
        case SAMPLE_VI_MODE_8_720P:
        case SAMPLE_VI_MODE_8_1080P:
            /* use chn 0,4,8,12,16,20,24,28 */
            pstViParam->s32ViDevCnt = 8;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 8;
            pstViParam->s32ViChnInterval = 4;
            break;
        default:
            SAMPLE_PRT("ViMode invaild!\n");
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* function : get vi parameter, according to vi type
*****************************************************************************/
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_ADStart(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm)
{
    VI_WORK_MODE_E enWorkMode;
    HI_S32 s32Ret;

    switch (enViMode)
    {
        case SAMPLE_VI_MODE_32_D1:
            enWorkMode = VI_WORK_MODE_4Multiplex;
            s32Ret = SAMPLE_TW2865_CfgV(enNorm, enWorkMode);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("SAMPLE_TW2865_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        case SAMPLE_VI_MODE_32_960H:
            enWorkMode = VI_WORK_MODE_4Multiplex;
            s32Ret = SAMPLE_TW2960_CfgV(enNorm, enWorkMode);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("SAMPLE_TW2960_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        case SAMPLE_VI_MODE_8_720P:
        case SAMPLE_VI_MODE_8_1080P:
            enWorkMode= VI_WORK_MODE_1Multiplex;
            s32Ret = SAMPLE_NVP6134_CfgV(enViMode,enNorm, enWorkMode);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("SAMPLE_NVP6134_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;

        case SAMPLE_VI_MODE_16_720P:
            enWorkMode = VI_WORK_MODE_2Multiplex;
            s32Ret =  SAMPLE_TP2823_CfgV(enNorm,enWorkMode);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("SAMPLE_TP2823_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        default:
            SAMPLE_PRT("AD not support!\n");
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/*****************************************************************************
* function : get vi parameter, according to vi type
*****************************************************************************/
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_Mode2Size(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm, RECT_S *pstCapRect, SIZE_S *pstDestSize)
{
    pstCapRect->s32X = 0;
    pstCapRect->s32Y = 0;
    switch (enViMode)
    {
        case SAMPLE_VI_MODE_32_D1:
            pstDestSize->u32Width = D1_WIDTH;
            pstDestSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            pstCapRect->u32Width = D1_WIDTH;
            pstCapRect->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case SAMPLE_VI_MODE_32_960H:
            pstDestSize->u32Width = 960;
            pstDestSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            pstCapRect->u32Width = 960;
            pstCapRect->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case SAMPLE_VI_MODE_8_720P:
        case SAMPLE_VI_MODE_16_720P:
            pstDestSize->u32Width  = _720P_WIDTH;
            pstDestSize->u32Height = _720P_HEIGHT;
            pstCapRect->u32Width  = _720P_WIDTH;
            pstCapRect->u32Height = _720P_HEIGHT;
            break;

        case SAMPLE_VI_MODE_8_1080P:
        case SAMPLE_VI_MODE_16_1080P:
            pstDestSize->u32Width  = HD_WIDTH;
            pstDestSize->u32Height = HD_HEIGHT;
            pstCapRect->u32Width  = HD_WIDTH;
            pstCapRect->u32Height = HD_HEIGHT;
            break;
        case SAMPLE_VI_MODE_4_4Kx2K:
            pstDestSize->u32Width  = _4K_WIDTH;
            pstDestSize->u32Height = _4K_HEIGHT;
            pstCapRect->u32Width  = _4K_WIDTH;
            pstCapRect->u32Height = _4K_HEIGHT;
            break;

        default:
            SAMPLE_PRT("vi mode invaild!\n");
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : Get Vi Dev No. according to Vi_Chn No.
*****************************************************************************/
VI_DEV Sample_Common_Vio::SAMPLE_COMM_VI_GetDev(SAMPLE_VI_MODE_E enViMode, VI_CHN ViChn)
{
    HI_S32 s32Ret, s32ChnPerDev;
    SAMPLE_VI_PARAM_S stViParam;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return (VI_DEV)-1;
    }

    s32ChnPerDev = stViParam.s32ViChnCnt / stViParam.s32ViDevCnt;
    return (VI_DEV)(ViChn /stViParam.s32ViChnInterval / s32ChnPerDev * stViParam.s32ViDevInterval);
}

/*****************************************************************************
* function : star vi dev (cfg vi_dev_attr; set_dev_cfg; enable dev)
*****************************************************************************/
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_StartDev(VI_DEV ViDev, SAMPLE_VI_MODE_E enViMode)
{
    HI_S32 s32Ret;
    VI_DEV_ATTR_S stViDevAttr;
    memset(&stViDevAttr,0,sizeof(stViDevAttr));

    switch (enViMode)
    {
        case SAMPLE_VI_MODE_32_D1:
            memcpy(&stViDevAttr,&DEV_ATTR_BT656D1_4MUX,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        case SAMPLE_VI_MODE_32_960H:
            memcpy(&stViDevAttr,&DEV_ATTR_BT656D1_4MUX,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        case SAMPLE_VI_MODE_8_720P:
        case SAMPLE_VI_MODE_8_1080P:
            memcpy(&stViDevAttr,&DEV_ATTR_TP2823_720P_1MUX_BASE,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        case SAMPLE_VI_MODE_16_720P:
            memcpy(&stViDevAttr,&DEV_ATTR_TP2823_720P_2MUX_BASE,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        case SAMPLE_VI_MODE_16_1080P:
            memcpy(&stViDevAttr,&DEV_ATTR_7441_BT1120_STANDARD_BASE,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        default:
            SAMPLE_PRT("vi input type[%d] is invalid!\n", enViMode);
            return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_SetDevAttr(ViDev, &stViDevAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_EnableDev(ViDev);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : star vi chn
*****************************************************************************/
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_StartChn(VI_CHN ViChn, RECT_S *pstCapRect, SIZE_S *pstTarSize,
    SAMPLE_VI_MODE_E enViMode, SAMPLE_VI_CHN_SET_E enViChnSet)
{
    HI_S32 s32Ret;
    VI_CHN_ATTR_S stChnAttr;

    /* step  5: config & start vicap dev */
    memcpy(&stChnAttr.stCapRect, pstCapRect, sizeof(RECT_S));
    /* to show scale. this is a sample only, we want to show dist_size = D1 only */
    stChnAttr.stDestSize.u32Width = pstTarSize->u32Width;
    stChnAttr.stDestSize.u32Height = pstTarSize->u32Height;
    stChnAttr.enCapSel = VI_CAPSEL_BOTH;
    stChnAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;   /* sp420 or sp422 */
    stChnAttr.bMirror = (VI_CHN_SET_MIRROR == enViChnSet)?HI_TRUE:HI_FALSE;
    stChnAttr.bFlip = (VI_CHN_SET_FILP == enViChnSet)?HI_TRUE:HI_FALSE;
    stChnAttr.s32SrcFrameRate = -1;
    stChnAttr.s32DstFrameRate = -1;
    switch (enViMode)
    {
        case SAMPLE_VI_MODE_32_D1:
        case SAMPLE_VI_MODE_32_960H:
        case SAMPLE_VI_MODE_32_1280H:
            stChnAttr.enScanMode = VI_SCAN_INTERLACED;
            break;
        case SAMPLE_VI_MODE_32_HALF720P:
        case SAMPLE_VI_MODE_8_720P:
        case SAMPLE_VI_MODE_16_720P:
        case SAMPLE_VI_MODE_8_1080P:
        case SAMPLE_VI_MODE_16_1080P:
        case SAMPLE_VI_MODE_4_4Kx2K:
            stChnAttr.enScanMode = VI_SCAN_PROGRESSIVE;
            break;
        default:
            SAMPLE_PRT("ViMode invaild!\n");
            return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_EnableChn(ViChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : star vi according to product type
*            if vi input is hd, we will start sub-chn for cvbs preview
*****************************************************************************/
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_Start(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm)
{
    VI_DEV ViDev;
    VI_CHN ViChn;
    HI_S32 i;
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;
    SIZE_S stTargetSize;
    RECT_S stCapRect;
    VI_CHN_BIND_ATTR_S stChnBindAttr;

    /*** get parameter from Sample_Vi_Mode ***/
    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_COMM_VI_Mode2Size(enViMode, enNorm, &stCapRect, &stTargetSize);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get size failed!\n");
        return HI_FAILURE;
    }

    /*** Start AD ***/
//    s32Ret = SAMPLE_COMM_VI_ADStart(enViMode, enNorm);
//    if (HI_SUCCESS !=s32Ret)
//    {
//        SAMPLE_PRT("Start AD failed!\n");
//        return HI_FAILURE;
//    }

    /*** Start VI Dev ***/
    for(i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;
        s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, enViMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StartDev failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    /*** Start VI Chn ***/
    for(i=0; i<stViParam.s32ViChnCnt; i++)
    {
        ViChn = i * stViParam.s32ViChnInterval;

        if (SAMPLE_VI_MODE_16_1080P == enViMode
         || SAMPLE_VI_MODE_16_720P == enViMode)
        {
            /* When in the 16x1080p mode, bind chn 2,6,10,14 to way1 is needed */
            if (ViChn%4 != 0)
            {
                s32Ret = HI_MPI_VI_GetChnBind(ViChn, &stChnBindAttr);
                if (HI_ERR_VI_FAILED_NOTBIND == s32Ret)
                {
                    stChnBindAttr.ViDev = ViChn/4;
                    stChnBindAttr.ViWay = 1;
                    s32Ret = HI_MPI_VI_BindChn(ViChn, &stChnBindAttr);
                    if (HI_SUCCESS != s32Ret)
                    {
                        SAMPLE_PRT("call HI_MPI_VI_BindChn failed with %#x\n", s32Ret);
                        return HI_FAILURE;
                    }
                }
            }
        }

        s32Ret = SAMPLE_COMM_VI_StartChn(ViChn, &stCapRect, &stTargetSize, enViMode, VI_CHN_SET_NORMAL);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("call SAMPLE_COMM_VI_StarChn failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_Start(VI_CHN ViChn,SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm)
{
    VI_DEV ViDev;
    VI_CHN Chn;
//    HI_S32 i;
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;
    SIZE_S stTargetSize;
    RECT_S stCapRect;
    VI_CHN_BIND_ATTR_S stChnBindAttr;

    /*** get parameter from Sample_Vi_Mode ***/
    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_COMM_VI_Mode2Size(enViMode, enNorm, &stCapRect, &stTargetSize);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get size failed!\n");
        return HI_FAILURE;
    }

    /*** Start VI Dev ***/

    ViDev = ViChn * stViParam.s32ViDevInterval;
    s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_StartDev failed with %#x\n", s32Ret);
        return HI_FAILURE;
    }


    /*** Start VI Chn ***/

    Chn = ViChn * stViParam.s32ViChnInterval;

    if (SAMPLE_VI_MODE_16_1080P == enViMode
     || SAMPLE_VI_MODE_16_720P == enViMode)
    {
        /* When in the 16x1080p mode, bind chn 2,6,10,14 to way1 is needed */
        if (Chn%4 != 0)
        {
            s32Ret = HI_MPI_VI_GetChnBind(Chn, &stChnBindAttr);
            if (HI_ERR_VI_FAILED_NOTBIND == s32Ret)
            {
                stChnBindAttr.ViDev = Chn/4;
                stChnBindAttr.ViWay = 1;
                s32Ret = HI_MPI_VI_BindChn(Chn, &stChnBindAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_PRT("call HI_MPI_VI_BindChn failed with %#x\n", s32Ret);
                    return HI_FAILURE;
                }
            }
        }


        s32Ret = SAMPLE_COMM_VI_StartChn(Chn, &stCapRect, &stTargetSize, enViMode, VI_CHN_SET_NORMAL);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("call SAMPLE_COMM_VI_StarChn failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;

}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_SetFrameDepth(VI_CHN ViChn, HI_U32 u32Depth)
{
    HI_S32 s32ret;

    s32ret = HI_MPI_VI_SetFrameDepth(ViChn, u32Depth);
    if (HI_SUCCESS != s32ret)
    {
        printf("set max depth err:0x%x\n", s32ret);
        return s32ret;
    }

    return HI_SUCCESS;

}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_GetFrame(VI_CHN ViChn, VIDEO_FRAME_INFO_S *pstFrameInfom,HI_S32 s32MilliSec)
{
    HI_S32 s32ret;

    s32ret = HI_MPI_VI_GetFrame(ViChn, pstFrameInfom,s32MilliSec);
    if (HI_SUCCESS != s32ret)
    {
        printf("get vi frame err:0x%x\n", s32ret);
        return s32ret;
    }

    return HI_SUCCESS;

}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_VI_ReleaseFrame(VI_CHN ViChn, VIDEO_FRAME_INFO_S*pstFrameInfo)
{
    HI_S32 s32ret;

    s32ret = HI_MPI_VI_ReleaseFrame(ViChn, pstFrameInfo);
    if (HI_SUCCESS != s32ret)
    {
        printf("get vi frame err:0x%x\n", s32ret);
        return s32ret;
    }

    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_GetChnLuma(SAMPLE_VI_MODE_E enViMode,VI_CHN ViChn, VI_CHN_LUM_S *pstLuma)
{
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_GetChnLuma(ViChn*stViParam.s32ViChnInterval,pstLuma);
    if (HI_SUCCESS != s32Ret){
        SAMPLE_PRT("HI_MPI_VI_GetChnLuma(%d) failed with %#x!\n", ViChn*stViParam.s32ViChnInterval,s32Ret);
        return HI_FAILURE;
    }


    return HI_SUCCESS;
}
/*****************************************************************************
* function : stop vi accroding to product type
*****************************************************************************/
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_Stop(SAMPLE_VI_MODE_E enViMode)
{
    VI_DEV ViDev;
    VI_CHN ViChn;
    HI_S32 i;
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;

    /*** get parameter from Sample_Vi_Mode ***/
    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }

    /*** Stop VI Chn ***/
    for(i=0;i<stViParam.s32ViChnCnt;i++)
    {
        /* Stop vi phy-chn */
        ViChn = i * stViParam.s32ViChnInterval;
        s32Ret = HI_MPI_VI_DisableChn(ViChn);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StopChn failed with %#x\n",s32Ret);
            return HI_FAILURE;
        }
    }

    /*** Stop VI Dev ***/
    for(i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;
        s32Ret = HI_MPI_VI_DisableDev(ViDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StopDev failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : Vi chn bind vpss group
*****************************************************************************/
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_BindVpss(SAMPLE_VI_MODE_E enViMode, VPSS_GRP *Vpss_GrpTab, HI_S32 Vpss_nGrp)
{
    HI_S32 j, s32Ret;
//    VPSS_GRP VpssGrp;
    VPSS_GRP nVpssGrp;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    SAMPLE_VI_PARAM_S stViParam;
    VI_CHN ViChn;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }

    //VpssGrp = 0;
    nVpssGrp = MIN2(Vpss_nGrp,stViParam.s32ViChnCnt);
    for (j=0; j<nVpssGrp; j++)
    {
        ViChn = j * stViParam.s32ViChnInterval;

        stSrcChn.enModId = HI_ID_VIU;
        stSrcChn.s32DevId = 0;
        stSrcChn.s32ChnId = ViChn;

        stDestChn.enModId = HI_ID_VPSS;
        stDestChn.s32DevId = Vpss_GrpTab[j];
        stDestChn.s32ChnId = 0;

        s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }

       // VpssGrp ++;
    }
    return HI_SUCCESS;
}


/*****************************************************************************
* function : Vi chn unbind vpss group
*****************************************************************************/
HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_UnBindVpss(SAMPLE_VI_MODE_E enViMode, VPSS_GRP *Vpss_GrpTab, HI_S32 Vpss_nGrp)
{
    HI_S32 i, j, s32Ret;
    VPSS_GRP VpssGrp;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    SAMPLE_VI_PARAM_S stViParam;
    VI_DEV ViDev;
    VI_CHN ViChn;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }

    VpssGrp = MIN2(Vpss_nGrp,stViParam.s32ViDevCnt);
    for (i=0; i<VpssGrp; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;

        for (j=0; j<stViParam.s32ViChnCnt; j++)
        {
            ViChn = j * stViParam.s32ViChnInterval;

            stSrcChn.enModId = HI_ID_VIU;
            stSrcChn.s32DevId = ViDev;
            stSrcChn.s32ChnId = ViChn;

            stDestChn.enModId = HI_ID_VPSS;
            stDestChn.s32DevId = Vpss_GrpTab[i];
            stDestChn.s32ChnId = 0;

            s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("failed with %#x!\n", s32Ret);
                return HI_FAILURE;
            }

        }
    }
    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_BindVo(VI_CHN ViChn, VO_CHN VoChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = m_VoLayer;
    stDestChn.s32ChnId = VoChn;

    return HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
}

HI_S32 Sample_Common_Vio::SAMPLE_COMM_VI_UnBindVo(VI_CHN ViChn, VO_CHN VoChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = ViChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = m_VoLayer;
    stDestChn.s32ChnId = VoChn;

    return HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
}

