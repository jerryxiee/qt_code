/******************************************************************************
  A simple program of Hisilicon mpp audio input/output/encoder/decoder implementation.
  Copyright (C), 2010-2018, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2013-7 Created
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

#include "sample_comm.h"
//#include "acodec.h"
#include "tlv320aic31.h"
#include "audio_aac_adp.h"


static PAYLOAD_TYPE_E gs_enPayloadType = PT_AAC;
//static HI_BOOL gs_bMicIn = HI_FALSE;
static HI_BOOL gs_bAiVqe= HI_FALSE;
static HI_BOOL gs_bAioReSample = HI_FALSE;
static HI_BOOL gs_bUserGetMode = HI_FALSE;
static HI_BOOL gs_bAoVolumeCtrl = HI_FALSE;
static AUDIO_RESAMPLE_ATTR_S *gs_pstAiReSmpAttr = NULL;
static AUDIO_RESAMPLE_ATTR_S *gs_pstAoReSmpAttr = NULL;
static HI_U32 g_u32AiCnt = 0;
static HI_U32 g_u32AoCnt = 0;
static HI_U32 g_u32AencCnt = 0;
static HI_U32 g_u32Adec = 0;
static HI_U32 g_u32AiDev = 0;
static HI_U32 g_u32AoDev = 0;

#define SAMPLE_DBG(s32Ret)\
do{\
    printf("s32Ret=%#x,fuc:%s,line:%d\n", s32Ret, __FUNCTION__, __LINE__);\
}while(0)

/******************************************************************************
* function : PT Number to String
******************************************************************************/
static char* SAMPLE_AUDIO_Pt2Str(PAYLOAD_TYPE_E enType)
{
    if (PT_G711A == enType)
    {
        return "g711a";
    }
    else if (PT_G711U == enType)
    {
        return "g711u";
    }
    else if (PT_ADPCMA == enType)
    {
        return "adpcm";
    }
    else if (PT_G726 == enType)
    {
        return "g726";
    }
    else if (PT_LPCM == enType)
    {
        return "pcm";
    }
    else if (PT_AAC == enType)
    {
        return "aac";
    }
    else
    {
        return "data";
    }
}

/******************************************************************************
* function : Open Aenc File
******************************************************************************/
static FILE * SAMPLE_AUDIO_OpenAencFile(AENC_CHN AeChn, PAYLOAD_TYPE_E enType)
{
    FILE *pfd;
    HI_CHAR aszFileName[128];

    /* create file for save stream*/
    sprintf(aszFileName, "audio_chn%d.%s", AeChn, SAMPLE_AUDIO_Pt2Str(enType));
    pfd = fopen(aszFileName, "w+");
    if (NULL == pfd)
    {
        printf("%s: open file %s failed\n", __FUNCTION__, aszFileName);
        return NULL;
    }
    printf("open stream file:\"%s\" for aenc ok\n", aszFileName);
    return pfd;
}

/******************************************************************************
* function : Open Adec File
******************************************************************************/
static FILE *SAMPLE_AUDIO_OpenAdecFile(ADEC_CHN AdChn, PAYLOAD_TYPE_E enType)
{
    FILE *pfd;
    HI_CHAR aszFileName[128];

    /* create file for save stream*/
    sprintf(aszFileName, "audio_chn%d.%s", AdChn, SAMPLE_AUDIO_Pt2Str(enType));
    pfd = fopen(aszFileName, "rb");
    if (NULL == pfd)
    {
        printf("%s: open file %s failed\n", __FUNCTION__, aszFileName);
        return NULL;
    }
    printf("open stream file:\"%s\" for adec ok\n", aszFileName);
    return pfd;
}

HI_S32 Audio_Eecode(){

    HI_S32      s32Ret;
    PAYLOAD_TYPE_E enPayloadType = PT_G711A;
    HI_U32 u32Len = 320;
    AIO_ATTR_S stAioAttr;
    HI_S32      s32AencChnCnt = 1;
    AENC_CHN AeChn = 0;
    AUDIO_FRAME_S stFrm;
    int seq = 0;

    stFrm.enSoundmode = AUDIO_SOUND_MODE_MONO;
    stFrm.enBitwidth = AUDIO_BIT_WIDTH_16;
    stFrm.u64TimeStamp = 0;


    stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_8000;
    stAioAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stAioAttr.enWorkmode     = AIO_MODE_I2S_MASTER;
    stAioAttr.enSoundmode    = AUDIO_SOUND_MODE_MONO;
    stAioAttr.u32EXFlag      = 0;
    stAioAttr.u32FrmNum      = 30;
    stAioAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;
    stAioAttr.u32ChnCnt      = 1;
    stAioAttr.u32ClkChnCnt   = 20;
    stAioAttr.u32ClkSel      = 0;

    s32Ret = SAMPLE_COMM_AUDIO_StartAenc(s32AencChnCnt, &stAioAttr, enPayloadType);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    FILE *inFile = fopen("test.pcm","rb");
    if(!inFile){
        return 0;
    }

    FILE *outFile = fopen("test.g711a","wb");
    if(!outFile){
        return 0;
    }

    AUDIO_STREAM_S stStream;

    HI_U8 *pu8AudioStream = NULL;

    size_t u32ReadLen;
    pu8AudioStream = (HI_U8*)malloc(sizeof(HI_U8)*MAX_AUDIO_STREAM_LEN);
    if (NULL == pu8AudioStream)
    {
        printf("%s: malloc failed!\n", __FUNCTION__);
        return 0;
    }

//    VB_POOL poolid = HI_MPI_VB_CreatePool(1024, 1, NULL);
//    stFrm.u32PoolId[0] = poolid;

//    s32Ret = HI_MPI_VB_MmapPool(poolid);
//    if(s32Ret != 0){
//        goto err;
//    }

//    VB_BLK blk = HI_MPI_VB_GetBlock(poolid, 1024,NULL);
//    HI_U32 phyaddr = HI_MPI_VB_Handle2PhysAddr(blk);
//    if(phyaddr == 0){
//        printf("%s:%d\n",__FUNCTION__,__LINE__);
//        goto err1;
//    }

//    s32Ret = HI_MPI_VB_GetBlkVirAddr(poolid, phyaddr, &stFrm.pVirAddr[0]);
//    if(s32Ret != 0){
//        printf("HI_MPI_VB_GetBlkVirAddr error\n");
//        goto err2;
//    }

    printf("start aenc\n");
    while (!feof(inFile))
    {
        stFrm.pVirAddr[0] = pu8AudioStream;
        u32ReadLen = fread(stFrm.pVirAddr[0], 1, u32Len, inFile);
        if (u32ReadLen <= 0)
        {
            printf("file end\n");
//            fseek(audiofile, 0, SEEK_SET);/*read file again*/
            break;
        }
        stFrm.u32Len = u32ReadLen;
        stFrm.u32Seq = seq++;
        stFrm.u64TimeStamp += 50000;
        printf("send frame size:%d\n",u32ReadLen);
        HI_MPI_AENC_SendFrame(AeChn, &stFrm,NULL);
        printf("send frame size:%d\n",u32ReadLen);
            /* get stream from aenc chn */
        s32Ret = HI_MPI_AENC_GetStream(AeChn, &stStream, 100000);
        if (HI_SUCCESS != s32Ret )
        {
            printf("HI_MPI_AENC_GetStream error\n");
            continue;
        }

        printf("getframe size:%d\n",stStream.u32Len);
        /* save audio stream to file */
        fwrite(stStream.pStream,1,stStream.u32Len, outFile);

            /* finally you must release the stream */
        s32Ret = HI_MPI_AENC_ReleaseStream(AeChn, &stStream);
        if (HI_SUCCESS != s32Ret )
        {
            printf("%s: HI_MPI_AENC_ReleaseStream(%d), failed with %#x!\n",\
                   __FUNCTION__, AeChn, s32Ret);

            return 0;
        }

    }

    fclose(inFile);
    fclose(outFile);

//    err2:
//    HI_MPI_VB_ReleaseBlock(blk);
//    err1:
//    HI_MPI_VB_MunmapPool(poolid);
    err:
    SAMPLE_COMM_AUDIO_StopAenc(s32AencChnCnt);
//    HI_MPI_VB_DestroyPool(poolid);
    return 0;
}

HI_S32 Audio_Decode(){
    ADEC_CHN    AdChn = 0;
    HI_S32      s32Ret;
    PAYLOAD_TYPE_E enPayloadType = PT_G711A;
    HI_U32 u32Len = 320;
    AUDIO_FRAME_INFO_S framInfo;


    s32Ret = SAMPLE_COMM_AUDIO_StartAdec(AdChn, enPayloadType);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    FILE *audiofile = fopen("test.g711a","rb");
    if(!audiofile){
        goto err;
    }

    FILE *outFile = fopen("testa.pcm","wb");
    if(!outFile){
        goto err;
    }

    HI_U8 *pu8AudioStream = NULL;
    AUDIO_STREAM_S stAudioStream;
    size_t u32ReadLen;
    pu8AudioStream = (HI_U8*)malloc(sizeof(HI_U8)*MAX_AUDIO_STREAM_LEN);
    if (NULL == pu8AudioStream)
    {
        printf("%s: malloc failed!\n", __FUNCTION__);
        goto err;
    }

    while (!feof(audiofile))
    {
        /* read from file */
        stAudioStream.pStream = pu8AudioStream;
        stAudioStream.pStream[0] = 0x0;
        stAudioStream.pStream[1] = 0x1;

        stAudioStream.pStream[3] = 0x0;
        u32ReadLen = fread(&stAudioStream.pStream[4], 1, u32Len, audiofile);
        if (u32ReadLen <= 0)
        {
            HI_MPI_ADEC_SendEndOfStream (AdChn, HI_FALSE);
            printf("file end\n");
//            fseek(audiofile, 0, SEEK_SET);/*read file again*/
            break;
        }
        stAudioStream.pStream[2] = u32ReadLen>>1;
        printf("send frame size:%d\n",u32ReadLen);
        /* here only demo adec streaming sending mode, but pack sending mode is commended */
        stAudioStream.u32Len = u32ReadLen + 4;
        s32Ret = HI_MPI_ADEC_SendStream(AdChn, &stAudioStream, HI_TRUE);
        if(HI_SUCCESS != s32Ret)
        {
            printf("%s: HI_MPI_ADEC_SendStream(%d) failed with %#x!\n",\
                   __FUNCTION__, AdChn, s32Ret);
            break;
        }

        s32Ret = HI_MPI_ADEC_GetFrame(AdChn,&framInfo, HI_TRUE);
        if(HI_SUCCESS != s32Ret){
//            printf("HI_MPI_ADEC_GetFrame error\n");
            continue;
        }

        printf("write frame size:%d\n",framInfo.pstFrame->u32Len);
        fwrite(framInfo.pstFrame->pVirAddr[0],1,framInfo.pstFrame->u32Len,outFile);

        HI_MPI_ADEC_ReleaseFrame( AdChn, &framInfo);

    }

    free(pu8AudioStream);
    fclose(audiofile);
    fclose(outFile);

    err:
    s32Ret = SAMPLE_COMM_AUDIO_StopAdec(AdChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    return HI_FAILURE;
}

/******************************************************************************
* function : file -> Adec -> Ao
******************************************************************************/
HI_S32 SAMPLE_AUDIO_AdecAo(HI_VOID)
{
    HI_S32      s32Ret;
    AUDIO_DEV   AoDev = SAMPLE_AUDIO_AO_DEV;
    AO_CHN      AoChn = 0;
    ADEC_CHN    AdChn = 0;
    FILE        *pfd = NULL;
    HI_U32      AoChnCnt = 0;
    AIO_ATTR_S stAioAttr;

    stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_8000;
    stAioAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stAioAttr.enWorkmode     = AIO_MODE_I2S_MASTER;
    stAioAttr.enSoundmode    = AUDIO_SOUND_MODE_MONO;
    stAioAttr.u32EXFlag      = 1;
    stAioAttr.u32FrmNum      = 30;
    stAioAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;
    stAioAttr.u32ChnCnt      = 1;
    stAioAttr.u32ClkChnCnt   = 2;
    stAioAttr.u32ClkSel      = 0;

    if(PT_AAC == gs_enPayloadType)
    {
        stAioAttr.u32PtNumPerFrm = AACLC_SAMPLES_PER_FRAME;
    }

#ifdef HI_ACODEC_TYPE_TLV320AIC31
        stAioAttr.u32ClkSel      = 1;
#endif

    gs_pstAiReSmpAttr = NULL;
    gs_pstAoReSmpAttr = NULL;

    s32Ret = SAMPLE_COMM_AUDIO_CfgAcodec(&stAioAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

	g_u32Adec = AdChn;
    s32Ret = SAMPLE_COMM_AUDIO_StartAdec(AdChn, gs_enPayloadType);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    AoChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
	g_u32AoCnt = AoChnCnt;
	g_u32AoDev = AoDev;
    s32Ret = SAMPLE_COMM_AUDIO_StartAo(AoDev, AoChnCnt, &stAioAttr, AUDIO_SAMPLE_RATE_BUTT, HI_FALSE, HI_NULL, 0);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_AUDIO_AoBindAdec(AoDev, AoChn, AdChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    pfd = SAMPLE_AUDIO_OpenAdecFile(AdChn, gs_enPayloadType);
    if (!pfd)
    {
        SAMPLE_DBG(HI_FAILURE);
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_COMM_AUDIO_CreatTrdFileAdec(AdChn, pfd);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    printf("bind adec:%d to ao(%d,%d) ok \n", AdChn, AoDev, AoChn);

    printf("\nplease press twice ENTER to exit this sample\n");
    getchar();
    getchar();

    s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdFileAdec(AdChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }


    s32Ret = SAMPLE_COMM_AUDIO_StopAdec(AdChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

	s32Ret = SAMPLE_COMM_AUDIO_StopAo(AoDev, AoChnCnt, gs_bAioReSample, HI_FALSE);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_AUDIO_AoUnbindAdec(AoDev, AoChn, AdChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/******************************************************************************
* function : Ai -> Aenc -> file
*                       -> Adec -> Ao
******************************************************************************/
HI_S32 SAMPLE_AUDIO_AiAenc(HI_VOID)
{
    HI_S32 i, s32Ret;
    AUDIO_DEV   AiDev = SAMPLE_AUDIO_AI_DEV;
    AI_CHN      AiChn;
    AUDIO_DEV   AoDev = SAMPLE_AUDIO_AO_DEV;
    AO_CHN      AoChn = 0;
    ADEC_CHN    AdChn = 0;
    HI_S32      s32AiChnCnt;
    HI_S32      s32AencChnCnt;
    AENC_CHN    AeChn;
    HI_BOOL     bSendAdec = HI_TRUE;
    FILE        *pfd = NULL;
    AIO_ATTR_S stAioAttr;

    stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_8000;
    stAioAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stAioAttr.enWorkmode     = AIO_MODE_I2S_MASTER;
    stAioAttr.enSoundmode    = AUDIO_SOUND_MODE_MONO;
    stAioAttr.u32EXFlag      = 1;
    stAioAttr.u32FrmNum      = 30;
    stAioAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;
    stAioAttr.u32ChnCnt      = 2;
    stAioAttr.u32ClkChnCnt   = 8;
    stAioAttr.u32ClkSel      = 0;

    if(PT_AAC == gs_enPayloadType)
    {
        stAioAttr.u32PtNumPerFrm = AACLC_SAMPLES_PER_FRAME;
    }

#ifdef HI_ACODEC_TYPE_TLV320AIC31
	stAioAttr.u32ClkSel      = 1;
#endif

    /********************************************
    step 1: config audio codec
    ********************************************/
    s32Ret = SAMPLE_COMM_AUDIO_CfgAcodec(&stAioAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /********************************************
    step 2: start Ai
    ********************************************/
    s32AiChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
    s32AencChnCnt = s32AiChnCnt ;
    g_u32AiCnt = s32AiChnCnt;
    g_u32AiDev = AiDev;
    s32Ret = SAMPLE_COMM_AUDIO_StartAi(AiDev, s32AiChnCnt, &stAioAttr, AUDIO_SAMPLE_RATE_BUTT, HI_FALSE, NULL, 0);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /********************************************
    step 3: start Aenc
    ********************************************/
    g_u32AencCnt = s32AencChnCnt;
    s32Ret = SAMPLE_COMM_AUDIO_StartAenc(s32AencChnCnt, &stAioAttr, gs_enPayloadType);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /********************************************
    step 4: Aenc bind Ai Chn
    ********************************************/
    for (i=0; i<s32AencChnCnt; i++)
    {
        AeChn = i;
        AiChn = i;

        if (HI_TRUE == gs_bUserGetMode)
        {
            s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAiAenc(AiDev, AiChn, AeChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_DBG(s32Ret);
                return HI_FAILURE;
            }
        }
        else
        {
            s32Ret = SAMPLE_COMM_AUDIO_AencBindAi(AiDev, AiChn, AeChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_DBG(s32Ret);
                return s32Ret;
            }
        }
        printf("Ai(%d,%d) bind to AencChn:%d ok!\n",AiDev , AiChn, AeChn);
    }

    /********************************************
    step 5: start Adec & Ao. ( if you want )
    ********************************************/
    if (HI_TRUE == bSendAdec)
    {
    	g_u32Adec = AdChn;
        s32Ret = SAMPLE_COMM_AUDIO_StartAdec(AdChn, gs_enPayloadType);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

		g_u32AoCnt = s32AiChnCnt;
		g_u32AoDev = AoDev;
        s32Ret = SAMPLE_COMM_AUDIO_StartAo(AoDev, s32AiChnCnt, &stAioAttr, AUDIO_SAMPLE_RATE_BUTT, HI_FALSE, NULL, 0);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

        pfd = SAMPLE_AUDIO_OpenAencFile(AdChn, gs_enPayloadType);
        if (!pfd)
        {
            SAMPLE_DBG(HI_FAILURE);
            return HI_FAILURE;
        }
        s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAencAdec(AeChn, AdChn, pfd);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

        s32Ret = SAMPLE_COMM_AUDIO_AoBindAdec(AoDev, AoChn, AdChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

        printf("bind adec:%d to ao(%d,%d) ok \n", AdChn, AoDev, AoChn);
     }

    printf("\nplease press twice ENTER to exit this sample\n");
    getchar();
    getchar();

    /********************************************
    step 6: exit the process
    ********************************************/
    if (HI_TRUE == bSendAdec)
    {
    	s32Ret = SAMPLE_COMM_AUDIO_AoUnbindAdec(AoDev, AoChn, AdChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

        s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAencAdec(AdChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

        s32Ret = SAMPLE_COMM_AUDIO_StopAo(AoDev, s32AiChnCnt, HI_FALSE, HI_FALSE);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

        s32Ret = SAMPLE_COMM_AUDIO_StopAdec(AdChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

    }

    for (i=0; i<s32AencChnCnt; i++)
    {
        AeChn = i;
        AiChn = i;

        if (HI_TRUE == gs_bUserGetMode)
        {
            s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAi(AiDev, AiChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_DBG(s32Ret);
                return HI_FAILURE;
            }
        }
        else
        {
            s32Ret = SAMPLE_COMM_AUDIO_AencUnbindAi(AiDev, AiChn, AeChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_DBG(s32Ret);
                return HI_FAILURE;
            }
        }
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAenc(s32AencChnCnt);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAi(AiDev, s32AiChnCnt, HI_FALSE, HI_FALSE);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/******************************************************************************
* function : Ai -> Ao(with fade in/out and volume adjust)
******************************************************************************/
HI_S32 SAMPLE_AUDIO_NVP6134AiAo(HI_VOID)
{
    HI_S32 s32Ret, s32AiChnCnt, s32AoChnCnt;
    AUDIO_DEV   AiDev = SAMPLE_AUDIO_AI_DEV;
    AI_CHN      AiChn = 0;
    AUDIO_DEV   AoDev = SAMPLE_AUDIO_AO_DEV;
    AO_CHN      AoChn = 0;
    AIO_ATTR_S stAioAttr;
    AUDIO_RESAMPLE_ATTR_S stAiReSampleAttr;
    AUDIO_RESAMPLE_ATTR_S stAoReSampleAttr;

    stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_8000;
    stAioAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stAioAttr.enWorkmode     = AIO_MODE_I2S_MASTER;
    stAioAttr.enSoundmode    = AUDIO_SOUND_MODE_MONO;
    stAioAttr.u32EXFlag      = 1;
    stAioAttr.u32FrmNum      = 30;
    stAioAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;
    stAioAttr.u32ChnCnt      = 4;
    stAioAttr.u32ClkChnCnt   = 4;
    stAioAttr.u32ClkSel      = 0;

	gs_bAioReSample = HI_FALSE;

    /* config ao resample attr if needed */
    if (HI_TRUE == gs_bAioReSample)
    {
        stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_32000;
        stAioAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM * 2;

        /* ai 32k -> 8k */
        stAiReSampleAttr.u32InPointNum  = SAMPLE_AUDIO_PTNUMPERFRM * 2;
        stAiReSampleAttr.enInSampleRate = AUDIO_SAMPLE_RATE_32000;
        stAiReSampleAttr.enOutSampleRate = AUDIO_SAMPLE_RATE_16000;
        gs_pstAiReSmpAttr = &stAiReSampleAttr;

        /* ao 8k -> 32k */
        stAoReSampleAttr.u32InPointNum  = SAMPLE_AUDIO_PTNUMPERFRM;
        stAoReSampleAttr.enInSampleRate = AUDIO_SAMPLE_RATE_16000;
        stAoReSampleAttr.enOutSampleRate = AUDIO_SAMPLE_RATE_32000;
        gs_pstAoReSmpAttr = &stAoReSampleAttr;
    }
    else
    {
        gs_pstAiReSmpAttr = NULL;
        gs_pstAoReSmpAttr = NULL;
    }


    /* resample and anr should be user get mode */
    gs_bUserGetMode = (HI_TRUE == gs_bAioReSample || HI_TRUE == gs_bAiVqe) ? HI_TRUE : HI_FALSE;

    /* config audio codec */
    s32Ret = SAMPLE_COMM_AUDIO_CfgAcodec(&stAioAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /* enable AI channle */
    s32AiChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
	g_u32AiCnt = s32AiChnCnt;
	g_u32AiDev = AiDev;
    s32Ret = SAMPLE_COMM_AUDIO_StartAi(AiDev, s32AiChnCnt, &stAioAttr, (gs_pstAiReSmpAttr != NULL ? gs_pstAiReSmpAttr->enOutSampleRate : AUDIO_SAMPLE_RATE_BUTT),
        (gs_pstAiReSmpAttr != NULL ? HI_TRUE : HI_FALSE), NULL, 0);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /* enable AO channle */
    stAioAttr.u32ChnCnt = 1;
    s32AoChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
	g_u32AoCnt = s32AoChnCnt;
	g_u32AoDev = AoDev;
    s32Ret = SAMPLE_COMM_AUDIO_StartAo(AoDev, s32AoChnCnt, &stAioAttr, (gs_pstAoReSmpAttr != NULL ? gs_pstAoReSmpAttr->enInSampleRate : AUDIO_SAMPLE_RATE_BUTT),
    (gs_pstAoReSmpAttr != NULL ? HI_TRUE : HI_FALSE), NULL, 0);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /* bind AI to AO channle */
    if (HI_TRUE == gs_bUserGetMode)
    {
        s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAiAo(AiDev, AiChn, AoDev, AoChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }
    }
    else
    {
        s32Ret = SAMPLE_COMM_AUDIO_AoBindAi(AiDev, AiChn, AoDev, AoChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }
    }
    printf("ai(%d,%d) bind to ao(%d,%d) ok\n", AiDev, AiChn, AoDev, AoChn);

    if(HI_TRUE == gs_bAoVolumeCtrl)
    {
        s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAoVolCtrl(AoDev);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }
    }

    printf("\nplease press twice ENTER to exit this sample\n");
    getchar();
    getchar();

    if(HI_TRUE == gs_bAoVolumeCtrl)
    {
        s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAoVolCtrl(AoDev);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }
    }

    if (HI_TRUE == gs_bUserGetMode)
    {
        s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAi(AiDev, AiChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }
    }
    else
    {
        s32Ret = SAMPLE_COMM_AUDIO_AoUnbindAi(AiDev, AiChn, AoDev, AoChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAi(AiDev, s32AiChnCnt, gs_bAioReSample, HI_FALSE);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAo(AoDev, s32AoChnCnt, gs_bAioReSample, HI_FALSE);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/******************************************************************************
* function : Ai -> Ao(Hdmi)
******************************************************************************/
HI_S32 SAMPLE_AUDIO_AiHdmiAo(HI_VOID)
{
    HI_S32 s32Ret, s32AiChnCnt;
    AUDIO_DEV   AiDev = SAMPLE_AUDIO_AI_DEV;
    AI_CHN      AiChn = 0;
    AUDIO_DEV   AoDev = SAMPLE_AUDIO_INNER_HDMI_AO_DEV;
    AO_CHN      AoChn = 0;
	HI_U32 		i;

    AIO_ATTR_S stAioAttr;
    AIO_ATTR_S stHdmiAoAttr;
    AUDIO_RESAMPLE_ATTR_S stAoReSampleAttr;

    stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_16000;
    stAioAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stAioAttr.enWorkmode     = AIO_MODE_I2S_MASTER;
    stAioAttr.enSoundmode    = AUDIO_SOUND_MODE_MONO;
    stAioAttr.u32EXFlag      = 1;
    stAioAttr.u32FrmNum      = 30;
    stAioAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;
    stAioAttr.u32ChnCnt      = 2;
	stAioAttr.u32ClkChnCnt   = 2;
    stAioAttr.u32ClkSel      = 0;

    stHdmiAoAttr.enSamplerate   = AUDIO_SAMPLE_RATE_48000;
    stHdmiAoAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stHdmiAoAttr.enWorkmode     = AIO_MODE_I2S_MASTER;
    stHdmiAoAttr.enSoundmode    = AUDIO_SOUND_MODE_MONO;
    stHdmiAoAttr.u32EXFlag      = 1;
    stHdmiAoAttr.u32FrmNum      = 30;
    stHdmiAoAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;
    stHdmiAoAttr.u32ChnCnt      = 2;
	stHdmiAoAttr.u32ClkChnCnt   = 2;
    stHdmiAoAttr.u32ClkSel      = 0;

    /* ao 8k -> 48k */
    stAoReSampleAttr.u32InPointNum  = SAMPLE_AUDIO_PTNUMPERFRM;
    stAoReSampleAttr.enInSampleRate = AUDIO_SAMPLE_RATE_16000;
    stAoReSampleAttr.enOutSampleRate = AUDIO_SAMPLE_RATE_48000;
    gs_pstAoReSmpAttr = &stAoReSampleAttr;
    gs_bAioReSample = HI_TRUE;
    /* resample and anr should be user get mode */
    gs_bUserGetMode = (HI_TRUE == gs_bAioReSample || HI_TRUE == gs_bAiVqe) ? HI_TRUE : HI_FALSE;

    /* config audio codec */
    s32Ret = SAMPLE_COMM_AUDIO_CfgAcodec(&stAioAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /* enable AI channle */
    s32AiChnCnt = stAioAttr.u32ChnCnt;
	g_u32AiCnt = s32AiChnCnt;
	g_u32AiDev = AiDev;
    s32Ret = SAMPLE_COMM_AUDIO_StartAi(AiDev, s32AiChnCnt, &stAioAttr, AUDIO_SAMPLE_RATE_BUTT, HI_FALSE, NULL, 0);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /* enable AO channle */
	g_u32AoCnt = stHdmiAoAttr.u32ChnCnt;
	g_u32AoDev = AoDev;
    s32Ret = SAMPLE_COMM_AUDIO_StartAo(AoDev, stHdmiAoAttr.u32ChnCnt, &stHdmiAoAttr, gs_pstAoReSmpAttr->enInSampleRate, HI_TRUE, NULL, 0);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /* AI to AO channel */
	for (i=0; i<g_u32AoCnt; i++)
	{
		AiChn = i;
		AoChn = i;
		s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAiAo(AiDev, AiChn, AoDev, AoChn);
	    if (s32Ret != HI_SUCCESS)
	    {
	        SAMPLE_DBG(s32Ret);
	        return HI_FAILURE;
	    }
	}

    printf("\nplease press twice ENTER to exit this sample\n");
    getchar();
    getchar();

	for (i=0; i<g_u32AoCnt; i++)
	{
		AiChn = i;
	    s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAi(AiDev, AiChn);
	    if (s32Ret != HI_SUCCESS)
	    {
	        SAMPLE_DBG(s32Ret);
	        return HI_FAILURE;
	    }
	}

    s32Ret = SAMPLE_COMM_AUDIO_StopAi(AiDev, s32AiChnCnt, HI_FALSE, HI_FALSE);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAo(AoDev, stHdmiAoAttr.u32ChnCnt, HI_TRUE, HI_FALSE);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ai2Mp4()
{
    HI_S32 i, s32Ret;
    AUDIO_DEV   AiDev = SAMPLE_AUDIO_AI_DEV;
    AI_CHN      AiChn;
    AUDIO_DEV   AoDev = SAMPLE_AUDIO_AO_DEV;
    AO_CHN      AoChn = 0;
    ADEC_CHN    AdChn = 0;
    HI_S32      s32AiChnCnt;
    HI_S32      s32AencChnCnt;
    AENC_CHN    AeChn;
    HI_BOOL     bSendAdec = HI_TRUE;
    FILE        *pfd = NULL;
    AIO_ATTR_S stAioAttr;

    stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_8000;
    stAioAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stAioAttr.enWorkmode     = AIO_MODE_I2S_MASTER;
    stAioAttr.enSoundmode    = AUDIO_SOUND_MODE_MONO;
    stAioAttr.u32EXFlag      = 1;
    stAioAttr.u32FrmNum      = 30;
    stAioAttr.u32PtNumPerFrm = SAMPLE_AUDIO_PTNUMPERFRM;
    stAioAttr.u32ChnCnt      = 2;
    stAioAttr.u32ClkChnCnt   = 8;
    stAioAttr.u32ClkSel      = 0;

    if(PT_AAC == gs_enPayloadType)
    {
        stAioAttr.u32PtNumPerFrm = AACLC_SAMPLES_PER_FRAME;
    }

#ifdef HI_ACODEC_TYPE_TLV320AIC31
    stAioAttr.u32ClkSel      = 1;
#endif

    /********************************************
    step 1: config audio codec
    ********************************************/
    s32Ret = SAMPLE_COMM_AUDIO_CfgAcodec(&stAioAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /********************************************
    step 2: start Ai
    ********************************************/
    s32AiChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
    s32AencChnCnt = s32AiChnCnt ;
    g_u32AiCnt = s32AiChnCnt;
    g_u32AiDev = AiDev;
    s32Ret = SAMPLE_COMM_AUDIO_StartAi(AiDev, s32AiChnCnt, &stAioAttr, AUDIO_SAMPLE_RATE_BUTT, HI_FALSE, NULL, 0);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /********************************************
    step 3: start Aenc
    ********************************************/
    g_u32AencCnt = s32AencChnCnt;
    s32Ret = SAMPLE_COMM_AUDIO_StartAenc(s32AencChnCnt, &stAioAttr, gs_enPayloadType);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    /********************************************
    step 4: Aenc bind Ai Chn
    ********************************************/
    for (i=0; i<s32AencChnCnt; i++)
    {
        AeChn = i;
        AiChn = i;

        if (HI_TRUE == gs_bUserGetMode)
        {
            s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAiAenc(AiDev, AiChn, AeChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_DBG(s32Ret);
                return HI_FAILURE;
            }
        }
        else
        {
            s32Ret = SAMPLE_COMM_AUDIO_AencBindAi(AiDev, AiChn, AeChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_DBG(s32Ret);
                return s32Ret;
            }
        }
        printf("Ai(%d,%d) bind to AencChn:%d ok!\n",AiDev , AiChn, AeChn);
    }

    /********************************************
    step 5: start Adec & Ao. ( if you want )
    ********************************************/
    if (HI_TRUE == bSendAdec)
    {
//        g_u32Adec = AdChn;
//        s32Ret = SAMPLE_COMM_AUDIO_StartAdec(AdChn, gs_enPayloadType);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_DBG(s32Ret);
//            return HI_FAILURE;
//        }

//        g_u32AoCnt = s32AiChnCnt;
//        g_u32AoDev = AoDev;
//        s32Ret = SAMPLE_COMM_AUDIO_StartAo(AoDev, s32AiChnCnt, &stAioAttr, AUDIO_SAMPLE_RATE_BUTT, HI_FALSE, NULL, 0);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_DBG(s32Ret);
//            return HI_FAILURE;
//        }

        pfd = SAMPLE_AUDIO_OpenAencFile(AdChn, gs_enPayloadType);
        if (!pfd)
        {
            SAMPLE_DBG(HI_FAILURE);
            return HI_FAILURE;
        }
        s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAencToMp4(AeChn, AdChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

//        s32Ret = SAMPLE_COMM_AUDIO_AoBindAdec(AoDev, AoChn, AdChn);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_DBG(s32Ret);
//            return HI_FAILURE;
//        }

        printf("bind adec:%d to ao(%d,%d) ok \n", AdChn, AoDev, AoChn);
     }

    printf("\nplease press twice ENTER to exit this sample\n");
    getchar();
    getchar();

    /********************************************
    step 6: exit the process
    ********************************************/
    if (HI_TRUE == bSendAdec)
    {
//        s32Ret = SAMPLE_COMM_AUDIO_AoUnbindAdec(AoDev, AoChn, AdChn);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_DBG(s32Ret);
//            return HI_FAILURE;
//        }

        s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAencAdec(AdChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_DBG(s32Ret);
            return HI_FAILURE;
        }

//        s32Ret = SAMPLE_COMM_AUDIO_StopAo(AoDev, s32AiChnCnt, HI_FALSE, HI_FALSE);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_DBG(s32Ret);
//            return HI_FAILURE;
//        }

//        s32Ret = SAMPLE_COMM_AUDIO_StopAdec(AdChn);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_DBG(s32Ret);
//            return HI_FAILURE;
//        }

    }

    for (i=0; i<s32AencChnCnt; i++)
    {
        AeChn = i;
        AiChn = i;

        if (HI_TRUE == gs_bUserGetMode)
        {
            s32Ret = SAMPLE_COMM_AUDIO_DestoryTrdAi(AiDev, AiChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_DBG(s32Ret);
                return HI_FAILURE;
            }
        }
        else
        {
            s32Ret = SAMPLE_COMM_AUDIO_AencUnbindAi(AiDev, AiChn, AeChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_DBG(s32Ret);
                return HI_FAILURE;
            }
        }
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAenc(s32AencChnCnt);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAi(AiDev, s32AiChnCnt, HI_FALSE, HI_FALSE);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_DBG(s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID SAMPLE_AUDIO_Usage(HI_VOID)
{
    printf("\n\n/************************************/\n");
    printf("please choose the case which you want to run:\n");
    printf("\t1:  start AI 8 chns to AO 1chn loop(NVP6134)\n");
    printf("\t2:  send audio frame to AENC channel from AI, save them\n");
    printf("\t3:  read audio stream from file, decode and send AO\n");
    printf("\t4:  start AI to AO(Hdmi) loop\n");
    printf("\tq:  quit whole audio sample\n\n");
    printf("sample command:");
}

/******************************************************************************
* function : to process abnormal case
******************************************************************************/
void SAMPLE_AUDIO_HandleSig(HI_S32 signo)
{
	static HI_BOOL s_bProcess = HI_FALSE;
    if (SIGINT == signo || SIGTERM == signo)
    {
    	if (!s_bProcess)
    	{
    		s_bProcess = HI_TRUE;

	        SAMPLE_COMM_AUDIO_DestoryAllTrd();

            SAMPLE_COMM_AUDIO_StopAdec(g_u32Adec);

			SAMPLE_COMM_AUDIO_StopAi(g_u32AiDev, g_u32AiCnt, gs_bAioReSample, HI_FALSE);

	    	SAMPLE_COMM_AUDIO_StopAo(g_u32AoDev, g_u32AoCnt, gs_bAioReSample, HI_FALSE);

			SAMPLE_COMM_AUDIO_StopAenc(g_u32AencCnt);

	        SAMPLE_COMM_SYS_Exit();

			s_bProcess = HI_FALSE;

	        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
		}
		else
		{
			return ;
		}

    }

    exit(0);
}

/******************************************************************************
* function : main
******************************************************************************/
int audio_main(int argc, char *argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR ch;
    HI_BOOL bExit = HI_FALSE;
    VB_CONF_S stVbConf;

    signal(SIGINT, SAMPLE_AUDIO_HandleSig);
    signal(SIGTERM, SAMPLE_AUDIO_HandleSig);

    memset(&stVbConf, 0, sizeof(VB_CONF_S));
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s: system init failed with %d!\n", __FUNCTION__, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_AENC_AacInit();
    if(HI_SUCCESS != s32Ret)
    {
        printf("%s: aac aenc init failed with %d!\n", __FUNCTION__, s32Ret);
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_ADEC_AacInit();
    if(HI_SUCCESS != s32Ret)
    {
        printf("%s: aac adec init failed with %d!\n", __FUNCTION__, s32Ret);
        return HI_FAILURE;
    }

    /******************************************
    1 choose the case
    ******************************************/
    while (1)
    {
        SAMPLE_COMM_AUDIO_RESET();
        SAMPLE_AUDIO_Usage();
        ch = getchar();
        if (10 == ch)
        {
            continue;
        }
        getchar();
        switch (ch)
        {
            case '1':
            {
//                Audio_Eecode();
                SAMPLE_AUDIO_NVP6134AiAo();
                break;
            }
            case '2':
            {
//                Audio_Decode();
//                SAMPLE_AUDIO_AiAenc();
            ai2Mp4();
                break;
            }
            case '3':
            {
                SAMPLE_AUDIO_AdecAo();
                break;
            }
            case '4':
            {
				SAMPLE_AUDIO_AiHdmiAo();
                break;
            }
            case 'q':
            case 'Q':
            {
                bExit = HI_TRUE;
                break;
            }
            default :
            {
                printf("input invaild! please try again.\n");
                break;
            }
        }

        if (bExit)
        {
            break;
        }
    }

    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}


