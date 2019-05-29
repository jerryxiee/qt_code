#ifndef SAMPLE_COMMON_VPSS_H
#define SAMPLE_COMMON_VPSS_H

#include "sample_comm.h"

class Sample_Common_Vpss
{
public:
    Sample_Common_Vpss();
    Sample_Common_Vpss(Sample_Common_Vpss &Sample_Vpss);
    Sample_Common_Vpss & operator = (const Sample_Common_Vpss & Sample_Vpss);
    Sample_Common_Vpss(HI_U32 u32GrpCnt,HI_U32 s32ChnCnt,SIZE_S *pstSize,VPSS_GRP_ATTR_S *pstVpssGrpAttr);
    ~Sample_Common_Vpss();

    HI_BOOL SAMPLE_COMM_VPSS_CreatIsSucess();
    HI_S32 SAMPLE_COMM_VPSS_SetChnMod(VPSS_GRP VpssGrpIndex,VPSS_CHN VpssChn,VPSS_CHN_MODE_S *pstVpssMode,HI_U32 u32Depth);
    HI_S32 SAMPLE_COMM_VPSS_GetChnMod(VPSS_GRP VpssGrpIndex,VPSS_CHN VpssChn,VPSS_CHN_MODE_S *pstVpssMode);
    HI_S32 SAMPLE_COMM_VPSS_Stop();
    HI_S32 SAMPLE_COMM_VPSS_MemConfig();

private:
    HI_S32 SAMPLE_COMM_VPSS_Start(HI_U32 u32GrpIndex, SIZE_S *pstSize, VPSS_GRP_ATTR_S *pstVpssGrpAttr);



public:
    HI_U32 m_Grp_Num;
    HI_U32 m_Chn_Num;
    VPSS_GRP m_Grp_Tab[VPSS_MAX_GRP_NUM];
    VPSS_GRP_ATTR_S *m_pstVpssGrpAttr = nullptr;


private:
    static HI_U32 m_Grp_MaxTable[VPSS_MAX_GRP_NUM];


};

#endif // SAMPLE_COMMON_VPSS_H
