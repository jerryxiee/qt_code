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

#include "sample_common_vpss.h"

HI_U32 Sample_Common_Vpss::m_Grp_MaxTable[VPSS_MAX_GRP_NUM] = {0};

Sample_Common_Vpss::Sample_Common_Vpss()
{
//    SIZE_S stSize;
//    HI_U32 Grp_Index = 0;


//    while(m_Grp_MaxTable[Grp_Index] != 0){
//        Grp_Index++;
//        if(Grp_Index >= VPSS_MAX_GRP_NUM){
//            m_Grp_Num = 0;
//            m_Chn_Num = 0;
//            return;
//        }
//    }
//    m_Grp_Num = 1;
//    m_Grp_MaxTable[Grp_Index] = 1;
//    m_Grp_Tab[0] = Grp_Index;

//    stSize.u32Height = HD_HEIGHT;
//    stSize.u32Width = HD_WIDTH;
//    SAMPLE_COMM_VPSS_Start(Grp_Index, &stSize,nullptr);

//    printf("enter %s:%d Grp_Index = %d\n",__FUNCTION__,__LINE__,Grp_Index);

}

Sample_Common_Vpss::Sample_Common_Vpss(Sample_Common_Vpss &Sample_Vpss)
{
    m_Chn_Num   = Sample_Vpss.m_Chn_Num;
    m_Grp_Num   = Sample_Vpss.m_Grp_Num;
    memcpy(m_Grp_Tab,Sample_Vpss.m_Grp_Tab,sizeof (HI_U32) * m_Grp_Num);
//    memcpy(m_pstVpssGrpAttr,Sample_Vpss.m_pstVpssGrpAttr,sizeof (VPSS_GRP_ATTR_S) * m_Grp_Num);
}

Sample_Common_Vpss & Sample_Common_Vpss::operator = (const Sample_Common_Vpss & Sample_Vpss)
{
    if(this == &Sample_Vpss)
        return *this;

    m_Chn_Num   = Sample_Vpss.m_Chn_Num;
    m_Grp_Num   = Sample_Vpss.m_Grp_Num;
    memcpy(m_Grp_Tab,Sample_Vpss.m_Grp_Tab,sizeof (HI_U32) * m_Grp_Num);
//    memcpy(m_pstVpssGrpAttr,Sample_Vpss.m_pstVpssGrpAttr,sizeof (VPSS_GRP_ATTR_S) * m_Grp_Num);

}
Sample_Common_Vpss::Sample_Common_Vpss(HI_U32 u32GrpCnt,HI_U32 u32ChnCnt,SIZE_S *pstSize,VPSS_GRP_ATTR_S *pstVpssGrpAttr)
    :m_Chn_Num(u32ChnCnt)
{
    HI_U32 Grp_Index = 0;

    m_Grp_Num = 0;
    for(int i = 0;i < u32GrpCnt;i++){
        while(m_Grp_MaxTable[Grp_Index] != 0){
            Grp_Index++;
            if(Grp_Index >= VPSS_MAX_GRP_NUM){
                while(--m_Grp_Num){
                    m_Grp_MaxTable[m_Grp_Tab[m_Grp_Num]] = 0;
                }
                m_Chn_Num = 0;
                return;
            }
        }
        m_Grp_MaxTable[Grp_Index] = 1;
        m_Grp_Tab[m_Grp_Num] = Grp_Index;
        m_Grp_Num++;

        SAMPLE_COMM_VPSS_Start(Grp_Index,pstSize,pstVpssGrpAttr);

//        printf("enter %s:%d Grp_Index = %d m_Grp_Num=%d\n",__FUNCTION__,__LINE__,Grp_Index,m_Grp_Num);
    }

}

Sample_Common_Vpss::~Sample_Common_Vpss()
{
    for(int i = 0;i < m_Grp_Num;i++){
        m_Grp_MaxTable[m_Grp_Tab[i]] = 0;
    }
    printf("exit %s\n",__FUNCTION__);

}
HI_BOOL Sample_Common_Vpss::SAMPLE_COMM_VPSS_CreatIsSucess()
{
    return m_Grp_Num > 0 ? HI_TRUE:HI_FALSE;
}

/******************************************************************************
* function : Set vpss system memory location
******************************************************************************/
HI_S32 Sample_Common_Vpss::SAMPLE_COMM_VPSS_MemConfig()
{
    HI_CHAR * pcMmzName;
    MPP_CHN_S stMppChnVpss;
    HI_S32 s32Ret, i;

    /*vpss group max is VPSS_MAX_GRP_NUM, not need config vpss chn.*/
    for(i=0;i<VPSS_MAX_GRP_NUM;i++)
    {
        stMppChnVpss.enModId  = HI_ID_VPSS;
        stMppChnVpss.s32DevId = i;
        stMppChnVpss.s32ChnId = 0;

        if(0 == (i%2))
        {
            pcMmzName = nullptr;
        }
        else
        {
            pcMmzName = "ddr1";
        }

        /*vpss*/
        s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVpss, pcMmzName);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Vpss HI_MPI_SYS_SetMemConf ERR !\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* function : start vpss. VPSS chn with frame
*****************************************************************************/
HI_S32 Sample_Common_Vpss::SAMPLE_COMM_VPSS_Start(HI_U32 u32GrpIndex, SIZE_S *pstSize, VPSS_GRP_ATTR_S *pstVpssGrpAttr)
{
//    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VPSS_GRP_ATTR_S stGrpAttr = {0};
    VPSS_CHN_ATTR_S stChnAttr = {0};
    VPSS_GRP_PARAM_S stVpssParam = {0};
    HI_S32 s32Ret;
    HI_S32 i, j;

    /*** Set Vpss Grp Attr ***/

    if(nullptr == pstVpssGrpAttr)
    {
        stGrpAttr.u32MaxW = ALIGN_UP(pstSize->u32Width,16);
        stGrpAttr.u32MaxH = ALIGN_UP(pstSize->u32Height,16);
        stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

        stGrpAttr.bIeEn = HI_FALSE;
        stGrpAttr.bNrEn = HI_TRUE;
        stGrpAttr.bDciEn = HI_TRUE;
        stGrpAttr.bHistEn = HI_FALSE;
        stGrpAttr.bEsEn = HI_FALSE;
        stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    }
    else
    {
        memcpy(&stGrpAttr,pstVpssGrpAttr,sizeof(VPSS_GRP_ATTR_S));
    }


//    SAMPLE_PRT("HI_MPI_VPSS_CreateGrp\n");
//    for(i=0; i<s32GrpCnt; i++)
//    {
//        VpssGrp = i;
        /*** create vpss group ***/
        s32Ret = HI_MPI_VPSS_CreateGrp(u32GrpIndex, &stGrpAttr);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("HI_MPI_VPSS_CreateGrp failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }

        /*** set vpss param ***/
        s32Ret = HI_MPI_VPSS_GetGrpParam(u32GrpIndex, &stVpssParam);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }

        stVpssParam.u32IeStrength = 0;
        s32Ret = HI_MPI_VPSS_SetGrpParam(u32GrpIndex, &stVpssParam);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }

        /*** enable vpss chn, with frame ***/
        for(j=0; j<m_Chn_Num; j++)
        {
            VpssChn = j;
            /* Set Vpss Chn attr */
            stChnAttr.bSpEn = HI_FALSE;
            stChnAttr.bUVInvert = HI_FALSE;
            stChnAttr.bBorderEn = HI_TRUE;
            stChnAttr.stBorder.u32Color = 0xffffff;
            stChnAttr.stBorder.u32LeftWidth = 2;
            stChnAttr.stBorder.u32RightWidth = 2;
            stChnAttr.stBorder.u32TopWidth = 2;
            stChnAttr.stBorder.u32BottomWidth = 2;

            s32Ret = HI_MPI_VPSS_SetChnAttr(u32GrpIndex, VpssChn, &stChnAttr);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("HI_MPI_VPSS_SetChnAttr failed with %#x\n", s32Ret);
                return HI_FAILURE;
            }

            s32Ret = HI_MPI_VPSS_EnableChn(u32GrpIndex, VpssChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("HI_MPI_VPSS_EnableChn failed with %#x\n", s32Ret);
                return HI_FAILURE;
            }
        }

        /*** start vpss group ***/
        s32Ret = HI_MPI_VPSS_StartGrp(u32GrpIndex);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("HI_MPI_VPSS_StartGrp failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }

  //  }
//        SAMPLE_PRT("SAMPLE_COMM_VPSS_Start sucess group:%d\n",u32GrpIndex);
    return HI_SUCCESS;
}

/*****************************************************************************
* function : disable vi dev
*****************************************************************************/
HI_S32 Sample_Common_Vpss::SAMPLE_COMM_VPSS_Stop()
{
    HI_S32 i, j;
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;

    //m_Grp_Table[m_Grp_Index] = 0;

    for(i=0; i<m_Grp_Num; i++)
    {
        VpssGrp = m_Grp_Tab[i];
        m_Grp_MaxTable[m_Grp_Tab[i]] = 0;

        s32Ret = HI_MPI_VPSS_StopGrp(VpssGrp);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
        for(j=0; j<m_Chn_Num; j++)
        {
            VpssChn = j;
            s32Ret = HI_MPI_VPSS_DisableChn(VpssGrp, VpssChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("failed with %#x!\n", s32Ret);
                return HI_FAILURE;
            }
        }

        s32Ret = HI_MPI_VPSS_DestroyGrp(VpssGrp);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 Sample_Common_Vpss::SAMPLE_COMM_VPSS_SetChnMod(VPSS_GRP VpssGrpIndex, VPSS_CHN VpssChn, VPSS_CHN_MODE_S *pstVpssMode, HI_U32 u32Depth)
{
    HI_S32 s32Ret;
    //HI_S32 s32MilliSec = 200;
    VPSS_CHN_MODE_S stVpssMode;

    if(VpssChn >= m_Chn_Num || VpssGrpIndex > m_Grp_Num)
        return -1;

    s32Ret = HI_MPI_VPSS_GetChnMode(m_Grp_Tab[VpssGrpIndex],VpssChn,&stVpssMode);
    if(s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    stVpssMode.enChnMode = pstVpssMode->enChnMode;
    stVpssMode.enPixelFormat = pstVpssMode->enPixelFormat;
    stVpssMode.u32Width = pstVpssMode->u32Width;
    stVpssMode.u32Height = pstVpssMode->u32Height;

    s32Ret = HI_MPI_VPSS_SetChnMode(m_Grp_Tab[VpssGrpIndex],VpssChn,&stVpssMode);
    if(s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    s32Ret = HI_MPI_VPSS_SetDepth(m_Grp_Tab[VpssGrpIndex],VpssChn,u32Depth);
    if(s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }


    return HI_SUCCESS;
}
