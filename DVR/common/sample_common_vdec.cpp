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

HI_U32 Sample_Common_Vdec::m_Vdec_MaxTab[VDEC_MAX_CHN_NUM] = {0};
bool Sample_Common_Vdec::mModeInit = false;

//HI_S32 m_s32VBSource = 0;
//VB_POOL m_ahVbPool[VB_MAX_POOLS] ;//= {[0 ... (VB_MAX_POOLS-1)] = VB_INVALID_POOLID};

Sample_Common_Vdec::Sample_Common_Vdec():
    m_Vdec_ChnNum(1),m_s32VBSource(0)
{
    HI_U32 Vdec_ChnIndex = 0;

    m_ahVbPool[VB_MAX_POOLS] = {0} ;
    while (m_Vdec_MaxTab[Vdec_ChnIndex] != 0) {
        Vdec_ChnIndex++;
        if(Vdec_ChnIndex >= VDEC_MAX_CHN_NUM){
            m_Vdec_ChnNum = 0;
//            delete [] m_pstVdecChnAttr;
            return;
        }
    }

    m_Vdec_MaxTab[Vdec_ChnIndex] = 1;
    m_Vdec_Tab[0] = Vdec_ChnIndex;

//    printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
}

Sample_Common_Vdec::Sample_Common_Vdec(HI_S32 s32ChnNum,HI_S32 s32VpssChnNum ,HI_S32 s32VBSource):
    m_s32VBSource(s32VBSource)
{
    HI_U32 i = 0;
    HI_U32 Vdec_ChnIndex = 0;

    m_ahVbPool[VB_MAX_POOLS] = {0} ;
    m_Vdec_ChnNum = 0;
    for(;i < s32ChnNum;i++){
        while (m_Vdec_MaxTab[Vdec_ChnIndex] != 0) {
            Vdec_ChnIndex++;
            if(Vdec_ChnIndex >= VDEC_MAX_CHN_NUM){
                while(--m_Vdec_ChnNum){
                    m_Vdec_MaxTab[m_Vdec_Tab[m_Vdec_ChnNum]] = 0;
                }
                return;
            }
        }
        m_Vdec_ChnNum++;
        m_Vdec_MaxTab[Vdec_ChnIndex] = 1;
        m_Vdec_Tab[i] = Vdec_ChnIndex;
    }

//    printf(">>>>>>enter %s:%d\n",__FUNCTION__,__LINE__);
//    for(i = 0; i < m_Vdec_ChnNum;i++)
//        printf("m_Vdec_Tab[%d] = %d\n",i,m_Vdec_Tab[i]);
}

HI_BOOL Sample_Common_Vdec::Sample_Common_Vdec_CreatIsSucess()
{
    return m_Vdec_ChnNum > 0?HI_TRUE:HI_FALSE;
}

Sample_Common_Vdec::~Sample_Common_Vdec()
{
    for(int i = 0;i < m_Vdec_ChnNum;i++)
    {
        m_Vdec_MaxTab[m_Vdec_Tab[i]] = 0;
    }

}

HI_VOID	Sample_Common_Vdec::SAMPLE_COMM_VDEC_ChnAttr(HI_S32 s32ChnNum,
    VDEC_CHN_ATTR_S *pstVdecChnAttr, PAYLOAD_TYPE_E enType, SIZE_S *pstSize)
{
    HI_S32 i;

//    for(i=0; i<m_Vdec_ChnNum; i++)
    {
        pstVdecChnAttr->enType       = enType;
        pstVdecChnAttr->u32BufSize   = 3 * pstSize->u32Width * pstSize->u32Height;
        pstVdecChnAttr->u32Priority  = 5;
        pstVdecChnAttr->u32PicWidth  = pstSize->u32Width;
        pstVdecChnAttr->u32PicHeight = pstSize->u32Height;
        if (PT_H264 == enType || PT_MP4VIDEO == enType)
        {
            pstVdecChnAttr->stVdecVideoAttr.enMode=VIDEO_MODE_FRAME;
            pstVdecChnAttr->stVdecVideoAttr.u32RefFrameNum = 2;
            pstVdecChnAttr->stVdecVideoAttr.bTemporalMvpEnable = HI_FALSE;
        }
        else if (PT_JPEG == enType || PT_MJPEG == enType)
        {
            pstVdecChnAttr->stVdecJpegAttr.enMode = VIDEO_MODE_FRAME;
            pstVdecChnAttr->stVdecJpegAttr.enJpegFormat = JPG_COLOR_FMT_YCBCR420;
        }
        else if(PT_H265 == enType)
        {
            pstVdecChnAttr->stVdecVideoAttr.enMode=VIDEO_MODE_STREAM;
            pstVdecChnAttr->stVdecVideoAttr.u32RefFrameNum = 2;
            pstVdecChnAttr->stVdecVideoAttr.bTemporalMvpEnable = HI_TRUE;
        }
    }

}

HI_BOOL Sample_Common_Vdec::SAMPLE_COMM_VDEC_GetPayload_Type(char* szFilePostfix,PAYLOAD_TYPE_E *enPayload)
{
    if(strcmp(szFilePostfix,".h264") == 0){
        *enPayload = PT_H264;
    }else if(strcmp(szFilePostfix,".h265") == 0){
        *enPayload = PT_H265;
    }else if(strcmp(szFilePostfix,".jpg") == 0){
        *enPayload = PT_JPEG;
    }else if(strcmp(szFilePostfix,".mjp") == 0){
        *enPayload = PT_MJPEG;
    }else if(strcmp(szFilePostfix,".mp4") == 0){
        *enPayload = PT_MP4VIDEO;
    }else{
        SAMPLE_PRT("payload type err!\n");
        return HI_FALSE;
    }


    return HI_TRUE;
}

HI_VOID	Sample_Common_Vdec::SAMPLE_COMM_VDEC_SetChnAttr(HI_S32 s32ChnIndex,VDEC_CHN_ATTR_S *pstVdecChnAttr)
{
//    printf("enter %s:%d s32ChnIndex = %d\n",__FUNCTION__,__LINE__,s32ChnIndex);
    memcpy(&m_stVdecChnAttr[s32ChnIndex],pstVdecChnAttr,sizeof (VDEC_CHN_ATTR_S));
}

HI_VOID	Sample_Common_Vdec::SAMPLE_COMM_VDEC_VpssGrpAttr(HI_S32 s32ChnNum, VPSS_GRP_ATTR_S *pstVpssGrpAttr, SIZE_S *pstSize)
{
    HI_S32 i;

    for(i=0; i<m_Vdec_ChnNum; i++)
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

HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_Start(HI_S32 s32ChnNum)
{
    HI_S32  i;
    HI_U32 u32BlkCnt = 10;
    VDEC_CHN_POOL_S stPool;
    VDEC_MOD_PARAM_S stModParam;
//    SIZE_S stRotateSize;
//    MPP_CHN_S stSrcChn;
//    MPP_CHN_S stDestChn;

    for(i=0; i<m_Vdec_ChnNum; i++)
    {
//        printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
//        if(m_nVpssChn > 0){
//            stRotateSize.u32Width = stRotateSize.u32Height = MAX2(m_stVdecChnAttr[i].u32PicWidth, m_stVdecChnAttr[i].u32PicHeight);
//            m_pVpss[i] = new Sample_Common_Vpss(1,m_nVpssChn,&stRotateSize,nullptr);
//            if(!m_pVpss[i]){
//                printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
//                delete m_pVpss[i];
//                return HI_FALSE;
//            }else if( HI_FALSE == m_pVpss[i]->Creat_Vpss_IsSucess(1)){
//                m_pVpss[i]->SAMPLE_COMM_VPSS_Stop();
//                printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
//                delete m_pVpss[i];
//                return HI_FALSE;
//            }
//        }

        if(1 == m_s32VBSource)
        {
            if(!mModeInit){
                mModeInit = true;
                stModParam.u32VBSource = 1;
                stModParam.u32MiniBufMode = 0;
                HI_MPI_VDEC_SetModParam(&stModParam);
            }

            CHECK_CHN_RET(HI_MPI_VDEC_SetChnVBCnt(m_Vdec_Tab[i], u32BlkCnt), m_Vdec_Tab[i], "HI_MPI_VDEC_SetChnVBCnt");
        }
        CHECK_CHN_RET(HI_MPI_VDEC_CreateChn(m_Vdec_Tab[i], &m_stVdecChnAttr[i]), m_Vdec_Tab[i], "HI_MPI_VDEC_CreateChn");
        if (2 == m_s32VBSource)
        {
            if(!mModeInit){
                mModeInit = true;
                stModParam.u32VBSource = 2;
                stModParam.u32MiniBufMode = 0;
                HI_MPI_VDEC_SetModParam(&stModParam);
            }

            stPool.hPicVbPool = m_ahVbPool[0];
            stPool.hPmvVbPool = -1;
            CHECK_CHN_RET(HI_MPI_VDEC_AttachVbPool(m_Vdec_Tab[i], &stPool), m_Vdec_Tab[i], "HI_MPI_VDEC_AttachVbPool");
        }
        CHECK_CHN_RET(HI_MPI_VDEC_StartRecvStream(m_Vdec_Tab[i]), m_Vdec_Tab[i], "HI_MPI_VDEC_StartRecvStream");
        //CHECK_CHN_RET(HI_MPI_VDEC_SetDisplayMode(i, VIDEO_DISPLAY_MODE_PREVIEW), i, "HI_MPI_VDEC_SetDisplayMode");
//        //bind vpss
//        printf(">>>>>>%s:%dm_Vdec_Tab[%d] = %d m_pVpss[%d]->m_Grp_Tab[0] = %d\n",__FUNCTION__,__LINE__,i,m_Vdec_Tab[i],i,m_pVpss[i]->m_Grp_Tab[0]);
//        stSrcChn.enModId = HI_ID_VDEC;
//        stSrcChn.s32DevId = 0;
//        stSrcChn.s32ChnId = m_Vdec_Tab[i];

//        stDestChn.enModId = HI_ID_VPSS;
//        stDestChn.s32DevId = m_pVpss[i]->m_Grp_Tab[0];
//        stDestChn.s32ChnId = 0;

//        Sample_Common_Sys::SAMPLE_COMM_BindVpss(&stSrcChn,&stDestChn);
    }

    return HI_SUCCESS;
}


HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_Stop(HI_S32 s32ChnNum)
{
    HI_S32 i;

    for(i=0; i<m_Vdec_ChnNum; i++)
    {
        m_Vdec_MaxTab[m_Vdec_Tab[i]] = 0;
        CHECK_CHN_RET(HI_MPI_VDEC_StopRecvStream(m_Vdec_Tab[i]), m_Vdec_Tab[i], "HI_MPI_VDEC_StopRecvStream");
        CHECK_CHN_RET(HI_MPI_VDEC_DestroyChn(m_Vdec_Tab[i]), m_Vdec_Tab[i], "HI_MPI_VDEC_DestroyChn");
    }

    return HI_SUCCESS;
}




HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_BindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp,VPSS_CHN VpssChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = VpssChn;

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



HI_S32 Sample_Common_Vdec::SAMPLE_COMM_VDEC_UnBindVpss(VDEC_CHN VdChn, VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = VpssChn;

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
            pcMmzName = nullptr;
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

HI_VOID	Sample_Common_Vdec::SAMPLE_COMM_VDEC_ModCommPoolConf(VB_CONF_S *pstModVbConf,
    PAYLOAD_TYPE_E enType, SIZE_S *pstSize, HI_S32 s32ChnNum, HI_BOOL bCompress)
{
    HI_S32 PicSize, PmvSize;

    memset(pstModVbConf, 0, sizeof(VB_CONF_S));
    pstModVbConf->u32MaxPoolCnt = 2;

    VB_PIC_BLK_SIZE(pstSize->u32Width, pstSize->u32Height, enType, PicSize);
    /***********vdec compressed vb needs header****************/
    pstModVbConf->astCommPool[0].u32BlkSize = PicSize;
    pstModVbConf->astCommPool[0].u32BlkCnt  = 45;

    /* NOTICE:
    1. if the VDEC channel is H264 channel and support to decode B frame, then you should allocate PmvBuffer
    2. if the VDEC channel is MPEG4 channel, then you should allocate PmvBuffer.
    */
    if(PT_H265 == enType)
    {
        VB_PMV_BLK_SIZE(pstSize->u32Width, pstSize->u32Height, enType, PmvSize);
        pstModVbConf->astCommPool[1].u32BlkSize = PmvSize;
        pstModVbConf->astCommPool[1].u32BlkCnt  = 5*s32ChnNum;
    }
}

HI_S32	Sample_Common_Vdec::SAMPLE_COMM_VDEC_InitModCommVb(VB_CONF_S *pstModVbConf)
{
    HI_S32 i;
    HI_S32 s32Ret;

    HI_MPI_VB_ExitModCommPool(VB_UID_VDEC);

    if(0 == m_s32VBSource)
    {
        CHECK_RET(HI_MPI_VB_SetModPoolConf(VB_UID_VDEC, pstModVbConf), "HI_MPI_VB_SetModPoolConf");
        CHECK_RET(HI_MPI_VB_InitModCommPool(VB_UID_VDEC), "HI_MPI_VB_InitModCommPool");
    }
    else if (2 == m_s32VBSource)
    {
        if (pstModVbConf->u32MaxPoolCnt > VB_MAX_POOLS)
        {
            printf("vb pool num(%d) is larger than VB_MAX_POOLS. \n", pstModVbConf->u32MaxPoolCnt);
            return HI_FAILURE;
        }
        for (i = 0; i < pstModVbConf->u32MaxPoolCnt; i++)
        {
            if (pstModVbConf->astCommPool[i].u32BlkSize && pstModVbConf->astCommPool[i].u32BlkCnt)
            {
                m_ahVbPool[i] = HI_MPI_VB_CreatePool(pstModVbConf->astCommPool[i].u32BlkSize,
                    pstModVbConf->astCommPool[i].u32BlkCnt, nullptr);
                if (VB_INVALID_POOLID == m_ahVbPool[i])
                    goto fail;
            }
        }
        return HI_SUCCESS;

    fail:
        for (;i>=0;i--)
        {
            if (VB_INVALID_POOLID != m_ahVbPool[i])
            {
                s32Ret = HI_MPI_VB_DestroyPool(m_ahVbPool[i]);
                HI_ASSERT(HI_SUCCESS == s32Ret);
                m_ahVbPool[i] = VB_INVALID_POOLID;
            }
        }
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
