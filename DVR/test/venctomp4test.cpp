#include "venctomp4test.h"

VencToMp4Test::VencToMp4Test(QObject *parent) :
    QThread(parent),mOutFmt_Ctx(nullptr),mFirstIDRFind(false),mPtsInc(0),mVi(-1)
{
    mIDRFramBuf = (unsigned char *)malloc(100*1024);
    if(!mIDRFramBuf){
        printf("malloc error\n");
    }
}

VencToMp4Test::~VencToMp4Test()
{
    mRun = false;
    wait();
    free(mIDRFramBuf);
    qDebug()<<"exit VencToMp4Test thread";

}

int VencToMp4Test::HI_PDT_CreateMp4(char *mp4name)
{
    int ret = 0; // 成功返回0，失败返回1
//    char pszFileName[256] = {0};
    AVOutputFormat *pOutFmt = nullptr;

//    sprintf(pszFileName,"tttt_%d.mp4",VeChn);

    av_register_all();

    avformat_alloc_output_context2(&mOutFmt_Ctx, nullptr, nullptr, mp4name);

    if (nullptr == mOutFmt_Ctx)
    {	//try default
        printf("Could not deduce output format from file extension: using mp4. \n");
        avformat_alloc_output_context2(&mOutFmt_Ctx, nullptr, "mp4", mp4name);
        if (nullptr == mOutFmt_Ctx)
        {
            printf("avformat_alloc_output_context2 failed  \n");
            return -1;
        }
    }

    pOutFmt = mOutFmt_Ctx->oformat;
    if (pOutFmt->video_codec == AV_CODEC_ID_NONE)
    {
        printf("add_stream ID =%d \n",pOutFmt->video_codec);
        goto exit_outFmt_failed;
    }

    ret = HI_PDT_Add_Stream(mOutFmt_Ctx);
    if(ret <0)
    {
         printf(" HI_PDT_Add_Stream Failed \n");
        goto exit_outFmt_failed;
    }

    printf("==========Output Information==========\n");
    av_dump_format(mOutFmt_Ctx, 0, mp4name, 1);


    /* open the output file, if needed */
    if (!(pOutFmt->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&mOutFmt_Ctx->pb, mp4name, AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            printf("could not open %s\n", mp4name);
            goto exit_avio_open_failed;
        }
    }

    /* Write the stream header, if any */
    ret = avformat_write_header(mOutFmt_Ctx, nullptr);
    if (ret < 0)
    {
        printf("Error occurred when opening output file \n");
        goto exit_writeheader_failed;
    }


    mFirstIDRFind = false;
    return HI_SUCCESS;

exit_writeheader_failed:
exit_avio_open_failed:
    if (mOutFmt_Ctx && !(mOutFmt_Ctx->flags & AVFMT_NOFILE))
        avio_close(mOutFmt_Ctx->pb);
exit_outFmt_failed:
    if(nullptr != mOutFmt_Ctx)
        avformat_free_context(mOutFmt_Ctx);
    return -1;
}

void VencToMp4Test::HI_PDT_CloseMp4(void)
{
    if (mOutFmt_Ctx)
        av_write_trailer(mOutFmt_Ctx);

    if (mOutFmt_Ctx && !(mOutFmt_Ctx->oformat->flags & AVFMT_NOFILE))
        avio_close(mOutFmt_Ctx->pb);

    if (mOutFmt_Ctx)
    {
        avformat_free_context(mOutFmt_Ctx);
        mOutFmt_Ctx = nullptr;
    }
    mVi = -1;
    mFirstIDRFind = false;
}

void VencToMp4Test::HI_PDT_WriteVideo(VENC_STREAM_S *pstStream,PAYLOAD_TYPE_E enType)
{
    unsigned int i=0;
    unsigned char* pPackVirtAddr = nullptr;
    unsigned int u32PackLen = 0;
    unsigned int u32PackOffset = 0;
    H264E_NALU_TYPE_E enH264EType;
    H265E_NALU_TYPE_E enH265EType;
    int ret = 0;
    AVStream *pst = nullptr;
    AVPacket pkt;
    int isIDR =0;

    if(mVi<0)
    {
        printf("vi less than 0 \n");
        return;
    }

    if(nullptr == pstStream)
    {
        return;
    }

    pst = mOutFmt_Ctx->streams[mVi];

    if(pstStream->u32PackCount == 1){
        pPackVirtAddr = pstStream->pstPack[0].pu8Addr ;
        u32PackLen = pstStream->pstPack[0].u32Len;
        u32PackOffset = pstStream->pstPack[0].u32Offset;
        isIDR = 0;
        av_init_packet(&pkt);

        if(PT_H264 == enType)
        {
            enH264EType = pstStream->pstPack[0].DataType.enH264EType;
            isIDR = HI_PDT_CheckKeyFrame_H264(enH264EType);
        }
        else if (PT_H265 == enType)
        {
            enH265EType = pstStream->pstPack[0].DataType.enH265EType;
            isIDR = HI_PDT_CheckKeyFrame_H265(enH265EType);
        }
        else
        {
            printf("unsupport stream type!  \n");
            return ;
        }
    }else {
        for (i = 0 ; i < pstStream->u32PackCount; i++)
        {
            isIDR = 0;

            memcpy(&mIDRFramBuf[u32PackLen],pstStream->pstPack[i].pu8Addr+pstStream->pstPack[i].u32Offset,
                   pstStream->pstPack[i].u32Len - pstStream->pstPack[i].u32Offset);

            u32PackLen += pstStream->pstPack[i].u32Len - pstStream->pstPack[i].u32Offset;
            if(PT_H264 == enType)
            {
                enH264EType = pstStream->pstPack[i].DataType.enH264EType;
                isIDR = HI_PDT_CheckKeyFrame_H264(enH264EType);
            }
            else if (PT_H265 == enType)
            {
                enH265EType = pstStream->pstPack[i].DataType.enH265EType;
                isIDR = HI_PDT_CheckKeyFrame_H265(enH265EType);
            }
            else
            {
                printf("unsupport stream type!  \n");
                return ;
            }

        }
        pPackVirtAddr = mIDRFramBuf;
        u32PackOffset = 0;
    }
    av_init_packet(&pkt);

    pkt.flags |=  (isIDR==1) ? AV_PKT_FLAG_KEY : 0;
    pkt.stream_index = pst->index;
    pkt.data = (unsigned char*)(pPackVirtAddr + u32PackOffset);
    pkt.size = u32PackLen - u32PackOffset;

    if(!mFirstIDRFind)
    {
        if (0 == ( pkt.flags & AV_PKT_FLAG_KEY ))
            return;

        mFirstIDRFind = true;
    }

    pkt.pts = pkt.dts = av_rescale_q_rnd(mPtsInc++, pst->codec->time_base,pst->time_base,(enum AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
//        pkt.pts = av_rescale_q((mPtsInc++), pst->codec->time_base,pst->time_base);
//        printf(">>>>>>>pts:%lld\n",av_rescale_q_rnd(mPtsInc-1, pst->codec->time_base,pst->time_base,(enum AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX)));
//        pkt.dts=av_rescale_q_rnd(pkt.dts, pst->time_base,pst->time_base,(enum AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
//        pkt.dts = pkt.pts;
    pkt.duration = /*AV_TIME_BASE * av_q2d(pst->codec->time_base) ;*/av_rescale_q(pkt.duration,pst->codec->time_base, pst->time_base);
    pkt.pos = -1;
//        printf("mPtsInc:%d dts:%lld pts:%lld duration:%lld\n",mPtsInc,pkt.dts,pkt.pts,pkt.duration);


    ret = av_interleaved_write_frame(mOutFmt_Ctx, &pkt);
    if (ret < 0)
    {
        printf("cannot write frame");
    }



    return;

}

int VencToMp4Test::HI_PDT_CheckKeyFrame_H264(H264E_NALU_TYPE_E enH264EType)
{

    int isIDR = 0;
//    printf("enH264EType:%d\n",enH264EType);
    switch(enH264EType)
    {
        case H264E_NALU_IDRSLICE:
        case H264E_NALU_ISLICE:
        case H264E_NALU_SPS:
        case H264E_NALU_PPS:
            isIDR = 1;
            break;

        default:
            break;
    }

    return isIDR;
}

int VencToMp4Test::HI_PDT_CheckKeyFrame_H265(H265E_NALU_TYPE_E enH265EType)
{

    int isIDR = 0;

    switch(enH265EType)
    {
        case H265E_NALU_IDRSLICE:
        case H265E_NALU_ISLICE:
        case H265E_NALU_SPS:
        case H265E_NALU_PPS:

        case H265E_NALU_VPS:
            isIDR = 1;
            break;

        default:
            break;
    }

    return isIDR;
}


int VencToMp4Test::HI_PDT_Add_Stream(AVFormatContext *poutFmtCtx)
{
    AVOutputFormat *pOutFmt = nullptr;
    AVCodecContext *PAVCodecCtx = nullptr;
    AVStream *pAVStream = nullptr;
    AVCodec *pcodec = nullptr;

    pOutFmt = poutFmtCtx->oformat;

    /* find the encoder */
    pcodec = avcodec_find_encoder(pOutFmt->video_codec);
    if (nullptr == pcodec)
    {
        printf("could not find encoder for '%s' \n", avcodec_get_name(pOutFmt->video_codec));
        return -1;
    }

    pAVStream = avformat_new_stream(poutFmtCtx, pcodec);
    if (nullptr == pAVStream)
    {
       printf("could not allocate stream \n");
       return -1;
    }

    pAVStream->id = poutFmtCtx->nb_streams-1;
    PAVCodecCtx = pAVStream->codec;
    mVi = pAVStream->index;

    switch ((pcodec)->type)
    {
        case AVMEDIA_TYPE_AUDIO:
            printf("AVMEDIA_TYPE_AUDIO\n");
            PAVCodecCtx->sample_fmt = (pcodec)->sample_fmts ? (pcodec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
            PAVCodecCtx->bit_rate = 64000;
            PAVCodecCtx->sample_rate = 44100;
            PAVCodecCtx->channels = 2;
            break;

        case AVMEDIA_TYPE_VIDEO:
            printf("AVMEDIA_TYPE_VIDEO\n");
            PAVCodecCtx->codec_id = AV_CODEC_ID_H264;
            PAVCodecCtx->bit_rate = 0;
            PAVCodecCtx->width = 1280;
            PAVCodecCtx->height = 720;
            PAVCodecCtx->time_base.den = STREAM_FRAME_RATE;
            PAVCodecCtx->time_base.num = 1;
            PAVCodecCtx->gop_size = 1;
            PAVCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
            if (PAVCodecCtx->codec_id == AV_CODEC_ID_MPEG2VIDEO)
            {
                PAVCodecCtx->max_b_frames = 2;
            }
            if (PAVCodecCtx->codec_id == AV_CODEC_ID_MPEG1VIDEO)
            {
                PAVCodecCtx->mb_decision = 2;
            }
            break;

        default:
            break;
    }

    if (poutFmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
    {
        PAVCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    return HI_SUCCESS;
}


void VencToMp4Test::run()
{

#if 1
    HI_S32 i = 0;
    int maxFd;
    struct timeval TimeoutVal;
    fd_set read_fds;
    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    HI_S32 s32Ret;
    int VencChn;
    int ViChn = 0;
    QList<uint>start;
    QList<uint>end;
    unsigned char* pPackVirtAddr = nullptr;
    unsigned int u32PackLen = 0;
    unsigned int u32PackOffset = 0;
    bool isIDR = false;

    mRun = true;

//    FILE* vencfile = fopen("realtimeStream.h264","wb");
//    if(!vencfile){
//        qDebug()<<"open file error";
//        return;
//    }
    QSize size(1280,720);

    if(mMp4File.createMP4File("record.mp4",25,size) < 0){
        printf("create mp4 file error\n");
        return;
    }

    start.append(0);
    start.append(0);
    start.append(0);
    start.append(0);
    end.append(1565308799);
    end.append(1565308799);
    end.append(1565308799);
    end.append(1565308799);


    mStreamPro.setTime(start,end,VIDEO_NORMAL);
    if(!mStreamPro.startVenc(ViChn,REALTIME1)){
        return;
    }

    maxFd = mStreamPro.getVencFd();
    VencChn = mStreamPro.getVencChn();
    qDebug()<<"fd:"<<maxFd<<" VencChn:"<<VencChn;
    while (mRun) {

        FD_ZERO(&read_fds);
        FD_SET(maxFd, &read_fds);

        TimeoutVal.tv_sec  = 2;
        TimeoutVal.tv_usec = 0;
        s32Ret = select(maxFd + 1, &read_fds, nullptr, nullptr, &TimeoutVal);
        if (s32Ret < 0)
        {
            SAMPLE_PRT("select failed!\n");
            break;
        }
        else if (s32Ret == 0)
        {
            SAMPLE_PRT("get venc stream time out, exit thread\n");
//            continue;
        }
        else
        {


            {
                if (FD_ISSET(maxFd, &read_fds))
                {
                    /*******************************************************
                    step 2.1 : query how many packs in one-frame stream.
                    *******************************************************/
                    memset(&stStream, 0, sizeof(stStream));
                    s32Ret = HI_MPI_VENC_Query(VencChn, &stStat);
                    if (HI_SUCCESS != s32Ret)
                    {
                        SAMPLE_PRT("HI_MPI_VENC_Query chn[%d] failed with %#x!\n", i, s32Ret);
                        break;
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
                        continue;
                    }
                    /*******************************************************
                     step 2.3 : malloc corresponding number of pack nodes.
                    *******************************************************/
                    stStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
                    if (nullptr == stStream.pstPack)
                    {
                        SAMPLE_PRT("malloc stream pack failed!\n");
                        break;
                    }
                    /*******************************************************
                     step 2.4 : call mpi to get one-frame stream
                    *******************************************************/
                    stStream.u32PackCount = stStat.u32CurPacks;
                    s32Ret = HI_MPI_VENC_GetStream(VencChn, &stStream, HI_TRUE);
                    if (HI_SUCCESS != s32Ret)
                    {
                        free(stStream.pstPack);
                        stStream.pstPack = nullptr;
                        SAMPLE_PRT("HI_MPI_VENC_GetStream failed with %#x!\n", \
                                   s32Ret);
                        break;
                    }
                    /*******************************************************
                     step 2.5 : save frame to file
                    *******************************************************/

//                    s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_SaveStream(PT_H264, vencfile, &stStream);
//                    if (HI_SUCCESS != s32Ret)
//                    {
//                        free(stStream.pstPack);
//                        stStream.pstPack = nullptr;
//                        SAMPLE_PRT("save stream failed!\n");
//                        break;
//                    }

                    if(stStream.u32PackCount == 1){
                        pPackVirtAddr = stStream.pstPack[0].pu8Addr + stStream.pstPack[0].u32Offset;
                        u32PackLen = stStream.pstPack[0].u32Len - stStream.pstPack[0].u32Offset;
//                        u32PackOffset = stStream.pstPack[0].u32Offset;
                        isIDR = Sample_Common_Venc::isIDRFrame(&stStream,PAYLOAD_TYPE);

                    }else {
                        for (i = 0 ; i < stStream.u32PackCount; i++)
                        {
                            isIDR = Sample_Common_Venc::isIDRFrame(&stStream,PAYLOAD_TYPE);

                            memcpy(&mIDRFramBuf[u32PackLen],stStream.pstPack[i].pu8Addr+stStream.pstPack[i].u32Offset,
                                   stStream.pstPack[i].u32Len - stStream.pstPack[i].u32Offset);

                            u32PackLen += stStream.pstPack[i].u32Len - stStream.pstPack[i].u32Offset;


                        }
                        pPackVirtAddr = mIDRFramBuf;

                    }
                    mMp4File.writeFrame(pPackVirtAddr,u32PackLen,isIDR);

//                    HI_PDT_WriteVideo(&stStream,PAYLOAD_TYPE);

                    /*******************************************************
                     step 2.6 : release stream
                     *******************************************************/
                    s32Ret = HI_MPI_VENC_ReleaseStream(VencChn, &stStream);
                    if (HI_SUCCESS != s32Ret)
                    {
                        free(stStream.pstPack);
                        stStream.pstPack = nullptr;
                        break;
                    }
                    /*******************************************************
                     step 2.7 : free pack nodes
                    *******************************************************/
                    free(stStream.pstPack);
                    stStream.pstPack = nullptr;
                }
            }
        }
        usleep(20000);
    }

//    fclose(vencfile);
    mMp4File.closeMO4File();

    mStreamPro.stopVenc(ViChn);

#endif

}
