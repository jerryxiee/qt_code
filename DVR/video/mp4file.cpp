#include "mp4file.h"
#include <QDebug>

MP4File::MP4File():mAVFmtCtx(nullptr),mVideoIndex(-1),mAudioIndex(-1),
    mPtsInc(0),mFirstIDRFrame(false)
{

}


int MP4File::addStream(enum AVCodecID codec_id)
{
    AVCodec *pcodec = nullptr;
    AVStream *pAVStream = nullptr;

    /* find the encoder */
    pcodec = avcodec_find_encoder(codec_id);
    if (nullptr == pcodec)
    {
        printf("could not find encoder for '%s' \n", avcodec_get_name(codec_id));
        return -1;
    }

    pAVStream = avformat_new_stream(mAVFmtCtx, pcodec);
    if (nullptr == pAVStream)
    {
       printf("could not allocate stream \n");
       return -1;
    }

    return 0;
}

int MP4File::addVideoStream(int framerate,QSize &size,int bitrate)
{
    int ret;
    int i = 0;
    AVOutputFormat *pOutFmt = nullptr;
//    AVCodec *pcodec = nullptr;

    pOutFmt = mAVFmtCtx->oformat;

    ret = addStream(pOutFmt->video_codec);
    if(ret < 0){
        qDebug()<<"addStream failed";
        return -1;
    }
    mVideoIndex = mAVFmtCtx->nb_streams-1;

    AVStream *pAVStream = mAVFmtCtx->streams[mVideoIndex];
    pAVStream->id = mVideoIndex;
    AVCodecContext *PAVCodecCtx = pAVStream->codec;

    printf("AVMEDIA_TYPE_VIDEO\n");
    PAVCodecCtx->codec_id = AV_CODEC_ID_H264;
    PAVCodecCtx->bit_rate = bitrate;
    PAVCodecCtx->width = size.width();
    PAVCodecCtx->height = size.height();
    PAVCodecCtx->time_base.den = framerate;
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

    if (mAVFmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
    {
        PAVCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    return 0;
}

bool MP4File::createMP4File(char *filename,int framerate,QSize &size,int bitrate,bool enAudio)
{
    int ret = 0;
    AVOutputFormat *pOutFmt = nullptr;

    avformat_alloc_output_context2(&mAVFmtCtx, nullptr, nullptr, filename);
    if (nullptr == mAVFmtCtx)
    {	//try default
        printf("Could not deduce output format from file extension: using mp4. \n");
        avformat_alloc_output_context2(&mAVFmtCtx, nullptr, "mp4", filename);
        if (nullptr == mAVFmtCtx)
        {
            printf("avformat_alloc_output_context2 failed  \n");
            return false;
        }
    }

    pOutFmt = mAVFmtCtx->oformat;
    if (pOutFmt->video_codec == AV_CODEC_ID_NONE)
    {
        printf("add_stream ID =%d \n",pOutFmt->video_codec);
        goto exit_outFmt_failed;
    }

    ret = addVideoStream(framerate,size ,bitrate);
    if(ret < 0){
        qDebug()<<"addVideoStream failed";
        goto exit_outFmt_failed;
    }

    if(enAudio){
        //创建音频流通道
    }

    printf("==========Output Information==========\n");
    av_dump_format(mAVFmtCtx, 0, filename, 1);

    /* open the output file, if needed */
    if (!(pOutFmt->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&mAVFmtCtx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            printf("could not open %s\n", filename);
            goto exit_avio_open_failed;
        }
    }
    if(enAudio){
        //打开音频通道
    }


    /* Write the stream header, if any */
    ret = avformat_write_header(mAVFmtCtx, nullptr);
    if (ret < 0)
    {
        printf("Error occurred when opening output file \n");
        goto exit_avio_open_failed;
    }

    mFirstIDRFrame = false;     //第一帧从IDR帧开始写

    return true;

exit_avio_open_failed:
    if (mAVFmtCtx && !(mAVFmtCtx->flags & AVFMT_NOFILE))
        avio_close(mAVFmtCtx->pb);

exit_outFmt_failed:
    if(nullptr != mAVFmtCtx)
        avformat_free_context(mAVFmtCtx);

    return false;
}

void MP4File::closeMO4File()
{
    if (mAVFmtCtx)
        av_write_trailer(mAVFmtCtx);

    if (mAVFmtCtx && !(mAVFmtCtx->oformat->flags & AVFMT_NOFILE))
        avio_close(mAVFmtCtx->pb);

    if (mAVFmtCtx)
    {
        avformat_free_context(mAVFmtCtx);
        mAVFmtCtx = nullptr;
    }
    mVideoIndex = -1;
    mAudioIndex = -1;
    mFirstIDRFrame = false;

}

void MP4File::writeFrame(unsigned char *buf,int len,bool isIDR)
{
    AVStream *pst = nullptr;
    AVPacket pkt;
    int ret;

    if(!mAVFmtCtx|| mVideoIndex < 0){
        qDebug()<<"can not write frame,file not create!";
        return;
    }

    pst = mAVFmtCtx->streams[mVideoIndex];

    av_init_packet(&pkt);

    pkt.flags |=  (isIDR == true)? AV_PKT_FLAG_KEY : 0;
    pkt.stream_index = pst->index;
    pkt.data = buf;
    pkt.size = len;

    if(!mFirstIDRFrame)
    {
        if (0 == ( pkt.flags & AV_PKT_FLAG_KEY ))
            return;
        mFirstIDRFrame = true;
    }

    pkt.pts = pkt.dts = av_rescale_q_rnd(mPtsInc++, pst->codec->time_base,pst->time_base,(enum AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));

    pkt.duration = /*AV_TIME_BASE * av_q2d(pst->codec->time_base) ;*/av_rescale_q(pkt.duration,pst->codec->time_base, pst->time_base);
    pkt.pos = -1;
//        printf("mPtsInc:%d dts:%lld pts:%lld duration:%lld\n",mPtsInc,pkt.dts,pkt.pts,pkt.duration);


    ret = av_interleaved_write_frame(mAVFmtCtx, &pkt);
    if (ret < 0)
    {
        printf("cannot write frame");
    }

    return;

}
