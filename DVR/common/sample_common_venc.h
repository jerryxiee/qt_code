#ifndef SAMPLE_COMMON_VENC_H
#define SAMPLE_COMMON_VENC_H

#include "sample_comm.h"

class Sample_Common_Venc
{
public:
    Sample_Common_Venc();

    HI_S32 SAMPLE_COMM_VENC_MemConfig(HI_VOID);
    HI_S32 SAMPLE_COMM_VENC_BindVi(VI_CHN ViChn);
    HI_S32 SAMPLE_COMM_VENC_UnBindVi(VI_CHN ViChn);
    HI_S32 SAMPLE_COMM_VENC_BindVpss(VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VENC_UnBindVpss(VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VENC_BindVo(VO_DEV VoDev, VO_CHN VoChn);
    HI_S32 SAMPLE_COMM_VENC_UnBindVo(VO_DEV VoDev, VO_CHN VoChn);
    HI_S32 SAMPLE_COMM_VDEC_BindVenc(VDEC_CHN VdChn);
    HI_S32 SAMPLE_COMM_VDEC_UnBindVenc(VDEC_CHN VdChn);
    HI_S32 SAMPLE_COMM_VENC_GetGopAttr(VENC_GOP_MODE_E enGopMode,VENC_GOP_ATTR_S *pstGopAttr,VIDEO_NORM_E enNorm);

    HI_S32 SAMPLE_COMM_VENC_SetAttr(PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32  u32Profile);
    HI_S32 SAMPLE_COMM_VENC_Start();
    HI_S32 SAMPLE_COMM_VENC_StartEx(PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32  u32Profile, VENC_GOP_ATTR_S *pstGopAttr);
    HI_S32 SAMPLE_COMM_VENC_Stop();

    static HI_S32 SAMPLE_COMM_VENC_GetFilePostfix(PAYLOAD_TYPE_E enPayload, char* szFilePostfix);
    static HI_S32 SAMPLE_COMM_VENC_SaveStream(PAYLOAD_TYPE_E enType, FILE* pFd, VENC_STREAM_S* pstStream);
    static HI_S32 SAMPLE_COMM_VENC_SaveMJpeg(FILE* fpMJpegFile, VENC_STREAM_S* pstStream);
    static HI_S32 SAMPLE_COMM_VENC_SaveJpeg(FILE* fpJpegFile, VENC_STREAM_S* pstStream);
    static HI_S32 SAMPLE_COMM_VENC_SaveH264(FILE* fpH264File, VENC_STREAM_S* pstStream);
    static HI_S32 SAMPLE_COMM_VENC_SaveH265(FILE* fpH265File, VENC_STREAM_S* pstStream);
    static HI_S32 SAMPLE_COMM_VENC_SaveJPEG(FILE* fpJpegFile, VENC_STREAM_S* pstStream);
    static HI_S32 SAMPLE_COMM_VENC_SaveFile(FILE* pFd, VENC_STREAM_BUF_INFO_S *pstStreamBuf, VENC_STREAM_S* pstStream);

public:
//    HI_U32 m_Venc_Tab[VENC_MAX_CHN_NUM];
//    HI_U32 m_Venc_ChnNum;
    VENC_CHN m_Venc_Chn;

private:
    const HI_U8 g_SOI[2] = {0xFF, 0xD8};
    const HI_U8 g_EOI[2] = {0xFF, 0xD9};

    PAYLOAD_TYPE_E m_enType;
    VIDEO_NORM_E m_enNorm;
    PIC_SIZE_E m_enSize;
    SAMPLE_RC_E m_enRcMode;
    HI_U32  m_u32Profile;

    static HI_U32 m_Venc_MaxTab[VENC_MAX_CHN_NUM];
};

#endif // SAMPLE_COMMON_VENC_H
