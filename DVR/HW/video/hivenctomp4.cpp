#include "hivenctomp4.h"
#include <QDebug>


HiVencToMp4 *HiVencToMp4::createNew(HiFrameSource *Source,
                          VIDEO_NORM_E enNorm,
                          PIC_SIZE_E enSize,
                          SAMPLE_RC_E enRcMode,
                          uint32_t u32BitRate,
                          uint32_t frmRate,
                          uint32_t  u32Profile,
                          PAYLOAD_TYPE_E enType,
                       const char *mp4FileName)
{
    HiVencToMp4 *consumer = nullptr;

    QSize size(1280,720);
    MP4File *mp4file = new MP4File();
    mp4file->createMP4File(mp4FileName,frmRate,size);

    Sample_Common_Venc *Venc = new Sample_Common_Venc();
    if(Venc){
        Venc->SAMPLE_COMM_VENC_SetAttr(enType,enNorm,enSize,enRcMode,u32BitRate,frmRate,u32Profile);
        if(Venc->SAMPLE_COMM_VENC_Start() != HI_SUCCESS){
           goto END;
        }
    }

    consumer = new HiVencToMp4(Source,Venc,mp4file,0,Venc->m_Venc_Chn);
    if(!consumer){
        goto END;
    }
    if(!consumer->isSucessbindSource()){

        goto END1;
    }
    return consumer;

END1:
    delete consumer;
END:
    mp4file->closeMO4File();
    delete mp4file;
    if(Venc){
        Venc->SAMPLE_COMM_VENC_Stop();
        delete Venc;
        Venc = nullptr;
    }
    return nullptr;

}


HiVencToMp4::HiVencToMp4(HiFrameSource *Source, Sample_Common_Venc *Venc, MP4File *mp4file, int DevId, int Chn):
    HiVencConsumer (Source,Venc,DevId,Chn),mMp4File(mp4file)
{
    mIDRFramBuf = (unsigned char *)malloc(BUFLEN);
    if(!mIDRFramBuf){
        printf("malloc error\n");
    }

}

HiVencToMp4::~HiVencToMp4()
{
    if(mIDRFramBuf){
        free(mIDRFramBuf);
    }

    if(mMp4File){
        qDebug()<<"exit HiVencToMp4";
        mMp4File->closeMO4File();
        delete mMp4File;
    }
    qDebug()<<"exit HiVencToMp4 end";
}

bool HiVencToMp4::saveFile(const char *newFileName,QSize size,uint32_t frmRate)
{
    mMutex.lock();
    if(!newFileName){
        mMp4File->closeMO4File();
        return true;
    }

    bool result = mMp4File->createMP4File(newFileName,frmRate,size);
    mMutex.unlock();

    return result;
}

void HiVencToMp4::doProcess()
{
    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    int s32Ret;
    unsigned char* pPackVirtAddr = nullptr;
    unsigned int u32PackLen = 0;
    bool isIDR = false;

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

    //处理关键帧,关键帧有多个包
    u32PackLen = 0;
    if(stStream.u32PackCount == 1){
        pPackVirtAddr = stStream.pstPack[0].pu8Addr + stStream.pstPack[0].u32Offset;
        u32PackLen = stStream.pstPack[0].u32Len - stStream.pstPack[0].u32Offset;
//                        u32PackOffset = stStream.pstPack[0].u32Offset;
        isIDR = Sample_Common_Venc::isIDRFrame(&stStream,PAYLOAD_TYPE);

    }else {
        for (int n = 0 ; n < stStream.u32PackCount; n++)
        {
            isIDR = Sample_Common_Venc::isIDRFrame(&stStream,PAYLOAD_TYPE);

            memcpy(&mIDRFramBuf[u32PackLen],stStream.pstPack[n].pu8Addr+stStream.pstPack[n].u32Offset,
                   stStream.pstPack[n].u32Len - stStream.pstPack[n].u32Offset);

            u32PackLen += stStream.pstPack[n].u32Len - stStream.pstPack[n].u32Offset;
            if(u32PackLen > BUFLEN){
                qDebug()<<"frame size larger than bufsize";
                break;
            }


        }
        pPackVirtAddr = mIDRFramBuf;

    }
    mMutex.lock();
    mMp4File->writeFrame(pPackVirtAddr,u32PackLen,isIDR);
    mMutex.unlock();


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
