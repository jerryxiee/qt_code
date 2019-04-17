#ifndef SAMPLE_COMMON_VDEC_H
#define SAMPLE_COMMON_VDEC_H

#include "sample_comm.h"
#include "common/sample_common_vpss.h"


#define PRINTF_VDEC_CHN_STATE(Chn, stChnStat) \
    do{\
            printf(" chn:%2d,  bStart:%2d,	DecodeFrames:%4d,  LeftPics:%3d,  LeftBytes:%10d,  LeftFrames:%4d,	RecvFrames:%6d\n",\
                Chn,\
                stChnStat.bStartRecvStream,\
                stChnStat.u32DecodeStreamFrames,\
                stChnStat.u32LeftPics,\
                stChnStat.u32LeftStreamBytes,\
                stChnStat.u32LeftStreamFrames,\
                stChnStat.u32RecvStreamFrames);\
        }while(0)

class Sample_Common_Vdec
{
public:
    Sample_Common_Vdec();
    ~Sample_Common_Vdec();
    Sample_Common_Vdec(HI_S32 s32ChnNum,HI_S32 s32VpssChnNum, HI_S32 s32VBSource = 0);


    HI_VOID	SAMPLE_COMM_VDEC_ChnAttr(HI_S32 s32ChnNum,VDEC_CHN_ATTR_S *pstVdecChnAttr, PAYLOAD_TYPE_E enType, SIZE_S *pstSize);
    HI_VOID	SAMPLE_COMM_VDEC_VpssGrpAttr(HI_S32 s32ChnNum, VPSS_GRP_ATTR_S *pstVpssGrpAttr, SIZE_S *pstSize);
    HI_S32 SAMPLE_COMM_VDEC_Start(HI_S32 s32ChnNum, VDEC_CHN_ATTR_S *pstAttr,VPSS_GRP_ATTR_S *pstVpssGrpAttr);
    HI_S32 SAMPLE_COMM_VDEC_Stop(HI_S32 s32ChnNum);
    HI_S32 SAMPLE_COMM_VDEC_BindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp);
    HI_S32 SAMPLE_COMM_VDEC_BindVo(VDEC_CHN VdChn, VO_LAYER VoLayer, VO_CHN VoChn);
    HI_S32 SAMPLE_COMM_VDEC_UnBindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp);
    HI_S32 SAMPLE_COMM_VDEC_UnBindVo(VDEC_CHN VdChn, VO_LAYER VoLayer, VO_CHN VoChn);
    HI_S32 SAMPLE_COMM_VDEC_MemConfig(HI_VOID);


public:
    HI_U32 m_Vdec_ChnNum;
    HI_U32 m_Vdec_Tab[VDEC_MAX_CHN_NUM];
    /* g_s32VBSource: 0 to module common vb, 1 to private vb, 2 to user vb
       And don't forget to set the value of VBSource file "load35xx" */
    HI_S32 g_s32VBSource ;
    VB_POOL g_ahVbPool[VB_MAX_POOLS] ;
    Sample_Common_Vpss *m_pVpss[VDEC_MAX_CHN_NUM];
    HI_U32 m_nVpssChn ;
    //HI_BOOL m_enVpss;

private:
    static HI_U32 m_Vdec_MaxTab[VDEC_MAX_CHN_NUM];

};

#endif // SAMPLE_COMMON_VDEC_H
