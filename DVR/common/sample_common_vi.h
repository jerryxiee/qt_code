#ifndef SAMPLE_COMMON_VI_H
#define SAMPLE_COMMON_VI_H

#include "sample_comm.h"

class Sample_Common_Vi
{
public:
    Sample_Common_Vi();

    VI_DEV SAMPLE_COMM_VI_GetDev(SAMPLE_VI_MODE_E enViMode, VI_CHN ViChn);
    HI_S32 SAMPLE_COMM_VI_ADStart(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm);
    HI_S32 SAMPLE_COMM_VI_Start(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm);
    HI_S32 SAMPLE_COMM_VI_Start(VI_CHN ViChn,SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm);
    HI_S32 SAMPLE_COMM_VI_SetFrameDepth(VI_CHN ViChn, HI_U32 u32Depth);
    HI_S32 SAMPLE_COMM_VI_GetFrame(VI_CHN ViChn, VIDEO_FRAME_INFO_S *pstFrameInfom,HI_S32 s32MilliSec);
    HI_S32 SAMPLE_COMM_VI_VI_ReleaseFrame(VI_CHN ViChn, VIDEO_FRAME_INFO_S*pstFrameInfo);
    HI_S32 SAMPLE_COMM_VI_GetChnLuma(SAMPLE_VI_MODE_E enViMode,VI_CHN ViChn, VI_CHN_LUM_S *pstLuma);
    HI_S32 SAMPLE_COMM_VI_Stop(SAMPLE_VI_MODE_E enViMode);
    HI_S32 SAMPLE_COMM_VI_BindVpss(SAMPLE_VI_MODE_E enViMode, VPSS_GRP *Vpss_GrpTab, HI_S32 Vpss_nGrp);
    HI_S32 SAMPLE_COMM_VI_UnBindVpss(SAMPLE_VI_MODE_E enViMode,VPSS_GRP *Vpss_GrpTab, HI_S32 Vpss_nGrp);
    HI_S32 SAMPLE_COMM_VI_BindVo(VI_CHN ViChn,VO_LAYER VoLayer, VO_CHN VoChn);
    HI_S32 SAMPLE_COMM_VI_UnBindVo(VI_CHN ViChn,VO_LAYER VoLayer,VO_CHN VoChn);

private:
    HI_VOID SAMPLE_COMM_VI_SetMask(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr);
    HI_S32 SAMPLE_TP2823_CfgV(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode);
    HI_S32 SAMPLE_NVP6134_CfgV(SAMPLE_VI_MODE_E enViMode,VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode);
    HI_S32 SAMPLE_TW2960_CfgV(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode);
    HI_S32 SAMPLE_TW2865_CfgV(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode);
    HI_S32 SAMPLE_COMM_VI_Mode2Param(SAMPLE_VI_MODE_E enViMode, SAMPLE_VI_PARAM_S *pstViParam);

    HI_S32 SAMPLE_COMM_VI_Mode2Size(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm, RECT_S *pstCapRect, SIZE_S *pstDestSize);
    HI_S32 SAMPLE_COMM_VI_StartDev(VI_DEV ViDev, SAMPLE_VI_MODE_E enViMode);
    HI_S32 SAMPLE_COMM_VI_StartChn(VI_CHN ViChn, RECT_S *pstCapRect, SIZE_S *pstTarSize,
        SAMPLE_VI_MODE_E enViMode, SAMPLE_VI_CHN_SET_E enViChnSet);
};

#endif // SAMPLE_COMMON_VI_H
