#include "mp4play.h"

MP4VideoPlay::MP4VideoPlay(QObject *parent) : QThread(parent),mAVfmtCtx(nullptr)
{
    connect(this,SIGNAL(endOfStream()),this,SLOT(onEndOfStream()));

}

MP4VideoPlay::MP4VideoPlay(const MP4VideoPlay& video)
{

}

MP4VideoPlay& MP4VideoPlay::operator=(const MP4VideoPlay& video)
 {

}

MP4VideoPlay::~MP4VideoPlay()
{
//    stopPlay();

}

void MP4VideoPlay::setPlaylist(QList<MP4FileInfo> &playlist)
{
    mPlayList = playlist;
    mCurFileIndex = 0;
    mCurPosition = 0;
    mRate = 1;
    mAVfmtCtx = nullptr;
    if(mPlayList.count() == 1){
        mDuration = (playlist.first().endPts - playlist.first().stPts)*1000 / 90000;
    }else {
        mDuration = (playlist.last().duration - playlist.first().duration + playlist.first().endPts - playlist.first().stPts
                     - ((playlist.last().duration - playlist.at(playlist.count()-2).duration) - playlist.last().endPts))*1000/90000;
    }
    mStartPosition = playlist.first().duration + playlist.first().stPts - playlist.first().endPts;

    emit durationChanged(mDuration);

}

void MP4VideoPlay::setPosition(qint64 position)
{
    int index = 0;
    qint64 pos = position * 90000;         //position为秒,转换到ffmpeg时间 position*90000;
//    qint64 duration;

    pause();
    index = searchIndex(mPlayList,0,mPlayList.count()-1,mStartPosition+pos);
    mFileMutex.lock();
    if(index == mCurFileIndex){
        av_seek_frame(mAVfmtCtx,mVideoIndex,mPlayList.at(mCurFileIndex).stPts+pos,AVSEEK_FLAG_BACKWARD);
    }else {
        if(index != 0){
            mCurPosition = mPlayList.at(index - 1).duration - mStartPosition;

        }else {
            mCurPosition = 0;
        }

        mCurFileIndex = index;
        changeCurFile(mCurFileIndex);

        av_seek_frame(mAVfmtCtx,mVideoIndex,mPlayList.at(mCurFileIndex).stPts+pos - mCurPosition,AVSEEK_FLAG_BACKWARD);

    }
//    qDebug()<<"setpos:"<<position<<"fileindex:"<<index<<" mCurFileIndex:"<<mCurFileIndex;
    emit positionChanged(position);
    play();
    mFileMutex.unlock();
}

void MP4VideoPlay::setPlaybackRate(qreal rate)
{
    mRate = rate;
}

void MP4VideoPlay::previousFile()
{
//    mFileMutex.lock();
//    if(mCurFileIndex != 0){
//        mCurFileIndex--;
//    }


//    mFileMutex.unlock();

}

void MP4VideoPlay::nextFile()
{
//    mFileMutex.lock();


//    mFileMutex.unlock();

}

void MP4VideoPlay::pause(){
    qDebug()<<"video pause";
    mMediaState = QMediaPlayer::PausedState;
    emit stateChanged(QMediaPlayer::PausedState);
}

void MP4VideoPlay::play()
{
    qDebug()<<"video play";
    mMediaState = QMediaPlayer::PlayingState;
    emit stateChanged(QMediaPlayer::PlayingState);
}

void MP4VideoPlay::stop()
{
    qDebug()<<"video stop";
    setPosition(0);
    mMediaState = QMediaPlayer::StoppedState;
    emit stateChanged(QMediaPlayer::StoppedState);
}

void MP4VideoPlay::onEndOfStream()
{
    stop();
    emit positionChanged(mDuration/1000);
}

int MP4VideoPlay::getVpssGrp() const
{
    if(mpVpss)
        return mpVpss->m_Grp_Tab[0];
    return -1;

}

int MP4VideoPlay::getVpssChn() const
{
    return VPSS_CHN0;
}

int MP4VideoPlay::searchIndex(QList<MP4FileInfo> &playlist,int stindex,int endindex,qint64 position)
{
    int index = stindex + (endindex - stindex)/2;

    MP4FileInfo fileinfo = playlist.at(index);
    if((fileinfo.duration >  position) && (position >= fileinfo.duration + fileinfo.stPts - fileinfo.endPts)){
        return index;
    }

    if(index == stindex){
        if(fileinfo.duration <= position)
            return endindex;

        return stindex;
    }

    if(position >= fileinfo.duration){
        return searchIndex(playlist,index + 1,endindex,position);
    }else {
        return searchIndex(playlist,stindex ,index,position);
    }

}

int MP4VideoPlay::getFileIndex(qint64 position)
{


}

int MP4VideoPlay::openFile(const char *name)
{
    int ret;
    mVideoIndex = -1;

    if(mAVfmtCtx){
        closeFile();
    }

    ret = avformat_open_input(&mAVfmtCtx, name, nullptr, nullptr);
    if(ret < 0){
        qDebug()<<"avformat_open_input failed";
        goto end;
    }

    ret = avformat_find_stream_info(mAVfmtCtx, nullptr);
    if(ret < 0){
        qDebug()<<"avformat_open_input failed";
        goto end;
    }

    for (uint i = 0;i <mAVfmtCtx->nb_streams ;i++) {
        if(mAVfmtCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            mVideoIndex = i;
        }
    }

    mPlayPts = 1000000/(mAVfmtCtx->streams[mVideoIndex]->r_frame_rate.num);

    return ret;

    end:

    avformat_close_input(&mAVfmtCtx);
    return -1;
}

void MP4VideoPlay::closeFile()
{
    if(mAVfmtCtx){
        avformat_close_input(&mAVfmtCtx);
        mAVfmtCtx = nullptr;
    }

}

int MP4VideoPlay::changeCurFile(int index)
{
    if(index >= mPlayList.count()){
        emit endOfStream();
        return -1;
    }

    if(mPlayList.at(mCurFileIndex).filename == mAVfmtCtx->filename){
        qDebug()<<"file have opened";
        goto END;
    }

    closeFile();
    if(openFile(mPlayList.at(index).filename) < 0){
        qDebug()<<"open file error";
        return -1;
    }
END:
    if(mPlayList.at(mCurFileIndex).stPts > 0){
        av_seek_frame(mAVfmtCtx,mVideoIndex,mPlayList.at(mCurFileIndex).stPts,AVSEEK_FLAG_BACKWARD);
    }

    return 0;
}

int MP4VideoPlay::readFrame(AVPacket &pkt)
{
    int ret;

    if(!mAVfmtCtx){
        return -1;
    }

    ret = av_read_frame(mAVfmtCtx, &pkt);
    if(pkt.pts >= mPlayList.at(mCurFileIndex).endPts || ret < 0){
        mCurPosition += mPlayList.at(mCurFileIndex).endPts - mPlayList.at(mCurFileIndex).stPts;
        mCurFileIndex++;
        ret = changeCurFile(mCurFileIndex);
        if(ret < 0){
            qDebug()<<"changeCurFile error";
            return ret;
        }else {
            qDebug()<<"changeCurFile sucess";
        }
    }
//    mCurPosition = pkt.pts;

    if((mCurPosition + pkt.pts - mPlayList.at(mCurFileIndex).stPts)%90000 == 0){
        emit positionChanged((mCurPosition + pkt.pts - mPlayList.at(mCurFileIndex).stPts)/90000);
    }


    return ret;

}

int MP4VideoPlay::setVpssToUser(SIZE_S  stSize)
{
    if(!mpVpss){
        qDebug()<<"vpss not init";
        return -1;
    }

    VPSS_CHN_MODE_S stVpssMode;

    stVpssMode.enChnMode = VPSS_CHN_MODE_USER;
    stVpssMode.u32Width = stSize.u32Width;
    stVpssMode.u32Height = stSize.u32Height;
    stVpssMode.enPixelFormat = SAMPLE_PIXEL_FORMAT;

    return  mpVpss->SAMPLE_COMM_VPSS_SetChnMod(0,VPSS_CHN0,&stVpssMode,8);

}


bool MP4VideoPlay::startPlay()
{
    HI_S32 s32Ret;
    SIZE_S  stSize;
    VB_CONF_S       stModVbConf;
    SIZE_S stRotateSize;
    VDEC_CHN_ATTR_S stVdecChnAttr;
//    VPSS_CHN_MODE_S stVpssMode;

    memset(&stVdecChnAttr,0x0,sizeof(VDEC_CHN_ATTR_S));

    stSize.u32Width = WIDTH;
    stSize.u32Height = HEIGHT;

    if(isRunning()){
        qDebug()<<"videoplay is running";
        return false;
    }
    if(mPlayList.count() == 0){
        qDebug()<<"no file";
        return false;
    }

    mpVdec = new Sample_Common_Vdec(1,1,0);

    if(mpVdec->m_Vdec_Tab[0] == 0){
        printf("init CommonPool\n");
        mpVdec->SAMPLE_COMM_VDEC_ModCommPoolConf(&stModVbConf, PT_H264, &stSize, 1, HI_FALSE);
        s32Ret = mpVdec->SAMPLE_COMM_VDEC_InitModCommVb(&stModVbConf);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("init mod common vb fail for %#x!\n", s32Ret);
            return HI_FALSE;
        }
    }

    mpVdec->SAMPLE_COMM_VDEC_ChnAttr(1, &stVdecChnAttr, PT_H264, &stSize);

    mpVdec->SAMPLE_COMM_VDEC_SetChnAttr(0,&stVdecChnAttr);


    stRotateSize.u32Width = stRotateSize.u32Height = MAX2(stVdecChnAttr.u32PicWidth, stVdecChnAttr.u32PicHeight);
    mpVpss= new Sample_Common_Vpss(1,1,&stRotateSize,nullptr);
    if(!mpVpss){
        printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
        return HI_FALSE;
    }else if( HI_FALSE == mpVpss->SAMPLE_COMM_VPSS_CreatIsSucess()){

        printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
        goto END1;
    }

    s32Ret = mpVdec->SAMPLE_COMM_VDEC_Start(1);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VDEC fail for %#x!\n", s32Ret);
        goto END2;
    }

    s32Ret = mpVdec->SAMPLE_COMM_VDEC_BindVpss(mpVdec->m_Vdec_Tab[0],mpVpss->m_Grp_Tab[0],0);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VDEC fail for %#x!\n", s32Ret);
        goto END4;
    }

    if(openFile(mPlayList.first().filename) < 0){
        goto END4;
    }

    if(mPlayList.first().stPts > 0){
        av_seek_frame(mAVfmtCtx,mVideoIndex,mPlayList.first().stPts,AVSEEK_FLAG_BACKWARD);
    }
    qDebug()<<"start play";
    play();
    start();
    return true;
END4:
    mpVdec->SAMPLE_COMM_VDEC_UnBindVpss(mpVdec->m_Vdec_Tab[0],mpVpss->m_Grp_Tab[0],0);

END2:
    mpVdec->SAMPLE_COMM_VDEC_Stop(1);

END1:
    mpVpss->SAMPLE_COMM_VPSS_Stop();
    mpVdec = nullptr;
    return false;

}

void MP4VideoPlay::stopPlay()
{
    qDebug()<<"enter stopPlay";
    mRun = false;
    wait();
    if(mpVdec){
        qDebug()<<"stop vdec:"<<mpVdec->m_Vdec_Tab[0];
        mpVdec->SAMPLE_COMM_VDEC_UnBindVpss(mpVdec->m_Vdec_Tab[0],mpVpss->m_Grp_Tab[0],0);
        mpVdec->SAMPLE_COMM_VDEC_Stop(1);
        mpVpss->SAMPLE_COMM_VPSS_Stop();

        delete mpVdec;
    }

    mpVdec = nullptr;
    qDebug()<<"stop vdec end";


}

void MP4VideoPlay::run()
{
    VDEC_STREAM_S stStream;
    int s32Ret;
//    qint64 s32ReadLen = 0;
    HI_U64 u64pts = 0;
    VDEC_CHN_STAT_S stStat;
    AVPacket pkt;

    mRun = true;

    while (mRun) {
        if (mMediaState != QMediaPlayer::PlayingState)
        {
//            qDebug()<<"pause";
            usleep(200000);
            continue;
        }

        mFileMutex.lock();
//        gettimeofday(&stv, &tz);
        HI_MPI_VDEC_Query(mpVdec->m_Vdec_Tab[0],&stStat);
        if(stStat.u32LeftStreamFrames > 0){
            mFileMutex.unlock();
            usleep(10000);
            continue;
        }

        s32Ret = readFrame(pkt);
        if(s32Ret < 0){
            mFileMutex.unlock();
            qDebug()<<"read frame error";
            usleep(100000);
            pause();
            continue;
        }

        if(pkt.stream_index == mVideoIndex){
            stStream.u64PTS  = u64pts;
            stStream.pu8Addr = (HI_U8 *)pkt.data;
            stStream.u32Len  = pkt.size;
            stStream.bEndOfFrame  = HI_TRUE;//(m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME)? HI_TRUE: HI_FALSE;
            stStream.bEndOfStream = HI_FALSE;

            s32Ret=HI_MPI_VDEC_SendStream(mpVdec->m_Vdec_Tab[0], &stStream, -1);
            if (HI_SUCCESS != s32Ret)
            {
                usleep(100);
                qDebug("send error s32Ret:%#x",s32Ret);
            }
            else
            {
                u64pts += (HI_U64)(mPlayPts / mRate);

            }
        }
        av_packet_unref(&pkt);


//        gettimeofday(&etv, &tz);

//        qDebug()<<"send frame usec:"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);

        mFileMutex.unlock();
        usleep(10000);

     }
}
