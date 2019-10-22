#include "hivenctofile.h"

HiVencToFile *HiVencToFile::createNew(HiFrameSource *Source,
                          VIDEO_NORM_E enNorm,
                          PIC_SIZE_E enSize,
                          SAMPLE_RC_E enRcMode,
                          uint32_t u32BitRate,
                          uint32_t frmRate,
                          uint32_t  u32Profile,
                          PAYLOAD_TYPE_E enType,
                       const char *fileName)
{
    HiVencToFile *consumer = nullptr;

    FILE *fp = fopen(fileName,"wb+");
    if(!fp){
        if(Source)
            delete Source;
        return nullptr;
    }

    consumer = new HiVencToFile(Source,fp,enNorm,enSize,enRcMode,u32BitRate,frmRate,u32Profile,enType);
    if(!consumer){
        if(Source)
            delete Source;
        goto END;
    }
    if(!consumer->isSucessbindSource()){

        goto END1;
    }
    return consumer;

END1:
    delete consumer;
END:
    fclose(fp);
    return nullptr;

}

HiVencToFile::HiVencToFile(HiFrameSource *Source, FILE *fp, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, uint32_t u32BitRate, uint32_t frmRate, uint32_t u32Profile, PAYLOAD_TYPE_E enType):
    HiVencConsumer (Source,enNorm,enSize,enRcMode,u32BitRate,frmRate,u32Profile,enType),mFile(fp)
{

}

HiVencToFile::~HiVencToFile()
{
    if(mFile){
        fclose(mFile);
    }
}

void HiVencToFile::doProcess()
{
    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    int s32Ret;

    /*******************************************************
    step 2.1 : query how many packs in one-frame stream.
    *******************************************************/
    memset(&stStream, 0, sizeof(stStream));
    s32Ret = HI_MPI_VENC_Query(getChn(), &stStat);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_Query chn[%d] failed with %#x!\n", getChn(),s32Ret);
        return;
    }
    /*******************************************************
    step 2.2 :suggest to check both u32CurPacks and u32LeftStreamFrames at the same time,for example:					 if(0 == stStat.u32CurPacks || 0 == stStat.u32LeftStreamFrames)
     {						SAMPLE_PRT("NOTE: Current  frame is NULL!\n");
        continue;
     }
     *******************************************************/
    if (0 == stStat.u32CurPacks)
    {
        SAMPLE_PRT("NOTE: Current  frame is NULL!\n");
        return;
    }
    /*******************************************************
     step 2.3 : malloc corresponding number of pack nodes.
    *******************************************************/
    stStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
    if (nullptr == stStream.pstPack)
    {
        SAMPLE_PRT("malloc stream pack failed!\n");
        return;
    }
    /*******************************************************
     step 2.4 : call mpi to get one-frame stream
    *******************************************************/
    stStream.u32PackCount = stStat.u32CurPacks;
    s32Ret = HI_MPI_VENC_GetStream(getChn(), &stStream, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        free(stStream.pstPack);
        stStream.pstPack = nullptr;
        SAMPLE_PRT("HI_MPI_VENC_GetStream failed with %#x!\n", \
                   s32Ret);
        return;
    }

    s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_SaveStream(getPlaylodType(), mFile, &stStream);
    if (HI_SUCCESS != s32Ret)
    {
//        free(stStream.pstPack);
//        stStream.pstPack = nullptr;
        SAMPLE_PRT("save stream failed!\n");
//        return;
    }

    /*******************************************************
     step 2.6 : release stream
     *******************************************************/
    s32Ret = HI_MPI_VENC_ReleaseStream(getChn(), &stStream);
    if (HI_SUCCESS != s32Ret)
    {
        free(stStream.pstPack);
        stStream.pstPack = nullptr;
        return;
    }
    /*******************************************************
     step 2.7 : free pack nodes
    *******************************************************/
    free(stStream.pstPack);
    stStream.pstPack = nullptr;

}
