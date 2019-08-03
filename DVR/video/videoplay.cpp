#include "videoplay.h"
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include <QDebug>
#include <float.h>
#include <QFileInfo>
#include <QDateTime>
#include "common/sample_common_sys.h"
#include <QtConcurrent>

#define USE_TAB

VideoPlay::VideoPlay(QObject *parent) : QThread(parent),m_pVdec(nullptr)
{
    connect(&mVideoPlayList,SIGNAL(positionChanged(quint32)),this,SLOT(onPositionChanged(quint32)));
    connect(&mVideoPlayList,SIGNAL(durationChanged(quint32)),this,SLOT(onSetDuration(quint32)));
    connect(&mVideoPlayList,SIGNAL(filefinished()),this,SLOT(onFileFinished()));
    connect(&mVideoPlayList,SIGNAL(frameRateChanged(uint)),this,SLOT(onFrameRateChanged(uint)));


}

VideoPlay::VideoPlay(const VideoPlay& video)
{

}

VideoPlay& VideoPlay::operator=(const VideoPlay&)
{

}

VideoPlay::~VideoPlay()
{
    qDebug("enter %s:%d",__FUNCTION__,__LINE__);
    if(m_pVdec){
        delete m_pVdec;
    }
    if(m_pVpss){
        delete m_pVpss;
    }

    qDebug("enter %s:%d exit",__FUNCTION__,__LINE__);
}


void VideoPlay::setFileList(VideoFileList &fileList)
{
    if(isRunning()){
        qDebug()<<"videoplay is running";
        return ;
    }

    if(fileList.first().getFrameRate() != 0){
        mPlayPts = 1000000/fileList.first().getFrameRate();
    }else {
        mPlayPts = NORMALPTS;
    }

    mRate = 1;
    mDuration = 1;
    m_enType = fileList.first().getEnType();
    setPosition(0);
    mVideoPlayList.setPlaylist(fileList);


}

void VideoPlay::setCurrentposition(int percent)
{
    HI_U32 position = percent * mDuration /100;

    mFileMutex.lock();
    mVideoPlayList.setPosition(position);
    mFileMutex.unlock();
    qDebug()<<"setCurrentposition:"<<position;
    play();


}

void VideoPlay::setPosition(uint value)
{
//    qDebug()<<"CurrentPrecent:"<<value;

    emit currentPrecentChanged(value);

}

void VideoPlay::Set_VdecAttr(VdecThreadParam &VdecAttr)
{
//    memcpy(&m_Thread_Attr,&VdecAttr,sizeof (VdecThreadParam));
//    qDebug("Set_VdecAttr  m_Thread_Attr.s32MinBufSize=%d\n",m_Thread_Attr.s32MinBufSize);
}

char* VideoPlay::Get_FileType(char *filename)
{
    char *name = filename;
    char *filetype = nullptr;

    while (name) {
        name = strchr(name+1,'.');
        if(name){
            filetype = name;
        }
    }

    return filetype;
}

int VideoPlay::getVpssGrp() const
{
    if(!m_pVpss){
        qDebug()<<"VPSS not init";
        return -1;
    }

    return m_pVpss->m_Grp_Tab[0];
}

int VideoPlay::getVpssChn() const
{
    return VPSS_CHN0;
}


HI_BOOL VideoPlay::Start_Vdec()
{
    HI_S32 s32Ret;
    SIZE_S  stSize;
    VB_CONF_S       stModVbConf;
    SIZE_S stRotateSize;
    VDEC_CHN_ATTR_S stVdecChnAttr;

    memset(&stVdecChnAttr,0x0,sizeof(VDEC_CHN_ATTR_S));

    stSize.u32Width = WIDTH;
    stSize.u32Height = HEIGHT;

    if(isRunning()){
        qDebug()<<"videoplay is running";
        return HI_FALSE;
    }
    if(mVideoPlayList.getFileListNum() == 0){
        qDebug()<<"no file";
        return HI_FALSE;
    }

    m_pVdec = new Sample_Common_Vdec(1,1,0);

    if(m_pVdec->m_Vdec_Tab[0] == 0){
        printf("init CommonPool\n");
        m_pVdec->SAMPLE_COMM_VDEC_ModCommPoolConf(&stModVbConf, m_enType, &stSize, 1, HI_FALSE);
        s32Ret = m_pVdec->SAMPLE_COMM_VDEC_InitModCommVb(&stModVbConf);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("init mod common vb fail for %#x!\n", s32Ret);
            return HI_FALSE;
        }
    }

    m_pVdec->SAMPLE_COMM_VDEC_ChnAttr(1, &stVdecChnAttr, m_enType, &stSize);

    m_pVdec->SAMPLE_COMM_VDEC_SetChnAttr(0,&stVdecChnAttr);


    stRotateSize.u32Width = stRotateSize.u32Height = MAX2(stVdecChnAttr.u32PicWidth, stVdecChnAttr.u32PicHeight);
    m_pVpss= new Sample_Common_Vpss(1,1,&stRotateSize,nullptr);
    if(!m_pVpss){
        printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
        return HI_FALSE;
    }else if( HI_FALSE == m_pVpss->SAMPLE_COMM_VPSS_CreatIsSucess()){

        printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
        goto END1;
    }


    s32Ret = m_pVdec->SAMPLE_COMM_VDEC_Start(1);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VDEC fail for %#x!\n", s32Ret);
        goto END2;
    }

    m_pVdec->SAMPLE_COMM_VDEC_BindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);


    mVideoPlayList.init();
    play();
    start();
    return HI_TRUE;
END4:
    m_pVdec->SAMPLE_COMM_VDEC_UnBindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);

END2:
    m_pVdec->SAMPLE_COMM_VDEC_Stop(1);

END1:
    m_pVpss->SAMPLE_COMM_VPSS_Stop();

    return HI_FALSE;
}

void VideoPlay::Stop_Vdec()
{
    m_Vdec_Run = false;
    wait();
    if(m_pVdec){
        qDebug()<<"stop vdec:"<<m_pVdec->m_Vdec_Tab[0];
        m_pVdec->SAMPLE_COMM_VDEC_UnBindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);
        m_pVdec->SAMPLE_COMM_VDEC_Stop(1);
        m_pVpss->SAMPLE_COMM_VPSS_Stop();

        delete m_pVdec;
    }

    m_pVdec = nullptr;
    qDebug()<<"stop vdec end";

}


void VideoPlay::endofStream()
{
    VDEC_STREAM_S stStream;

    memset(&stStream, 0, sizeof(VDEC_STREAM_S) );
    stStream.bEndOfStream = HI_TRUE;
    HI_MPI_VDEC_SendStream(m_pVdec->m_Vdec_Tab[0], &stStream, -1);

}

void VideoPlay::pause(){
    qDebug()<<"video pause";
    mState = QMediaPlayer::PausedState;
    emit stateChanged(QMediaPlayer::PausedState);
}

void VideoPlay::play()
{
    qDebug()<<"video play";
    mState = QMediaPlayer::PlayingState;
    emit stateChanged(QMediaPlayer::PlayingState);
}

void VideoPlay::stop()
{
    qDebug()<<"video stop";
    mVideoPlayList.setPosition(0);
    mState = QMediaPlayer::StoppedState;
    emit stateChanged(QMediaPlayer::StoppedState);
}

void VideoPlay::videoStart()
{
//    mState = QMediaPlayer::PlayingState;
//    emit stateChanged(QMediaPlayer::PlayingState);
    play();
}

void VideoPlay::setRate(qreal rate)
{
    if(rate < FLT_EPSILON){
        return;
    }

    mRate = rate;
    qDebug()<<"mRate:"<<mRate;
}

void VideoPlay::onRateChanged(qreal rate)
{
    qDebug()<<"rate change";
    setRate(rate);
}

void VideoPlay::onFrameRateChanged(uint framrate)
{
    if(framrate != 0){
        mPlayPts = 1000000/framrate;
    }
}

void VideoPlay::onSetDuration(quint32 duration)
{
    mDuration = duration;

    setPosition(mVideoPlayList.position() * 100 /mDuration);
    qDebug()<<"[VideoPlay::]setDuration:"<<duration<<" mDuration"<<mDuration;
}

void VideoPlay::onPositionChanged(quint32 pos){
    setPosition(pos * 100 /mDuration);
    qDebug()<<"PositionChanged:"<<pos<<" mDuration:"<<mDuration;
}
void VideoPlay::onFileFinished()
{
    pause();
    setPosition(100);
}

void VideoPlay::run()
{
    VDEC_STREAM_S stStream;
    HI_S32 s32Ret;
    qint64 s32ReadLen = 0;
    HI_U64 u64pts = 0;
    char *buff;

    VDEC_CHN_STAT_S stStat;

    QString filename;
//    struct timeval stv;
//    struct timeval etv;
//    struct timezone tz;

    u64pts = 0;
     m_Vdec_Run = true;
     qDebug()<<"vdec run";
    while (m_Vdec_Run)
    {
        if (mState != QMediaPlayer::PlayingState)
        {
//            qDebug()<<"pause";
            usleep(200000);
            continue;
        }

        mFileMutex.lock();
//        gettimeofday(&stv, &tz);
        HI_MPI_VDEC_Query(m_pVdec->m_Vdec_Tab[0],&stStat);
        if(stStat.u32LeftStreamFrames > 0){
            mFileMutex.unlock();
            usleep(10000);
            continue;
        }
        s32ReadLen = mVideoPlayList.getFrame(&buff,VIDEO_MODE_FRAME);
        if(s32ReadLen == 0){
            mFileMutex.unlock();
            qDebug()<<"read frame error";
            usleep(100000);
            continue;
        }
//        gettimeofday(&etv, &tz);

//        qDebug()<<"get frame usec:"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);
        stStream.u64PTS  = u64pts;
        stStream.pu8Addr = (HI_U8 *)buff;
        stStream.u32Len  = s32ReadLen;
        stStream.bEndOfFrame  = HI_TRUE;//(m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME)? HI_TRUE: HI_FALSE;
        stStream.bEndOfStream = HI_FALSE;

        s32Ret=HI_MPI_VDEC_SendStream(m_pVdec->m_Vdec_Tab[0], &stStream, -1);
        if (HI_SUCCESS != s32Ret)
        {
            usleep(100);
            qDebug("send error s32Ret:%#x",s32Ret);
        }
        else
        {
            u64pts += (HI_U64)(mPlayPts / mRate);

        }
//        gettimeofday(&etv, &tz);

//        qDebug()<<"send frame usec:"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);

        mFileMutex.unlock();
       usleep(10000);
    }
    fflush(stdout);
    qDebug()<<"exit paly thread";
}
