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

VideoPlay::VideoPlay(QObject *parent) : QThread(parent)
{
    VO_DEV VoDev;
    VO_LAYER VoLayer;

    Sample_Common_Sys::Get_Sys_VoDev(VoDev,VoLayer);
    m_Vdec_Vio.SAMPLE_COMM_VO_SetDev(VoDev,VoLayer);

    memset(&m_Thread_Attr,0x0,sizeof (VdecThreadParam));
    memset(&m_stVdecChnAttr,0x0,sizeof(VDEC_CHN_ATTR_S));
    connect(&mVideoPlayList,SIGNAL(positionChanged(quint32)),this,SLOT(onPositionChanged(quint32)));
    connect(&mVideoPlayList,SIGNAL(durationChanged(quint32)),this,SLOT(onSetDuration(quint32)));
    connect(&mVideoPlayList,SIGNAL(filefinished()),this,SLOT(onFileFinished()));


//    mFileCache = nullptr;
//    pFramTab = nullptr;
    m_pVdec = nullptr;
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
//    if(pFramTab){
//        HI_MPI_SYS_MmzFree(u32PhyAddr, pFramTab);
//    }
//    if(mFileCache){
//        HI_MPI_SYS_MmzFree(mFileCachePhyAddr, mFileCache);
//        qDebug()<<"HI_MPI_SYS_MmzFree mFileCache";
//    }
//    if(mVideoFileList.count())
//        mVideoFileList.clear();

    qDebug("enter %s:%d exit",__FUNCTION__,__LINE__);
}

int VideoPlay::serach(HI_U32 *value,int start,int end,int num)
{
//    int index = start + (end - start)/2;

//    if(start == index){
//        if(value[index] < num && index < mVideoFileList.count() - 1){
//            if(num <= value[index + 1]){
//                return index+1;
//            }else {
//                qDebug()<<"beyond current frame tab";
//                return -1;
//            }
//        }else{
//            return index;
//        }
//    }

//    if(value[index] >= num){
//        serach(value, start,index,num);
//    }else {
//        serach(value, index,end,num);
//    }
    return 0;

}

int VideoPlay::getFileIndex(HI_U32 framenum)
{
//    if(mTotalFileNum == 0){
//        return -1;
//    }

//    return serach(pFramTab,0,mTotalFileNum-1,framenum);
    return 0;
}

off_t VideoPlay::getFrameOffset(int fileindex,int frame)
{
//    if(frame <= 0){
//        return 0;
//    }
//    QString filename = mVideoFileList.at(fileindex).getFileName();
//    int offset = sizeof(MYVIDEO_HEAD) + (mVideoFileList.at(fileindex).getStartIndex()+frame - 1) *sizeof(FRAME_INDEX);
//    FRAME_INDEX frameindex;

//    QFile tabfile(filename);
//    if(tabfile.exists()){
//        tabfile.open(QIODevice::ReadOnly);
//        if(!tabfile.isOpen()){
//            qDebug()<<"can not open file "<<tabfile.fileName();
//            return -1;
//        }
//        tabfile.seek(offset);
//        tabfile.read((char *)&frameindex,sizeof (FRAME_INDEX));
//        tabfile.close();
//    }else {
//        return -1;
//    }

//    return frameindex.offset;
    return 0;
}

void VideoPlay::caclFramNum()
{
//    int i;
//    HI_U32 *pTab = pFramTab;

//    struct timeval stv;
//    struct timeval etv;
//    struct timezone tz;
//    gettimeofday(&stv, &tz);
//    qDebug()<<"start calc frame num";

//    for(i = 0; i < mVideoFileList.count();i++){
//        if(i != 0){
//            pTab[i] = pTab[i-1] + mVideoFileList.at(i).getEndIndex() - mVideoFileList.at(i).getStartIndex() +1;

//        }else {
//            pTab[i] = mVideoFileList.at(i).getEndIndex() - mVideoFileList.at(i).getStartIndex() +1;
//        }
//        mTotalFileNum++;
//        if(!mCalcFram){
//            break;
//        }
//    }
//    gettimeofday(&etv, &tz);

//    qDebug()<<"search time(us):"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);
//    qDebug()<<"i:"<<i<<"mTotalFramNum:"<<pTab[i - 1];

//    mTotalFramNum = pTab[i - 1];

}

void VideoPlay::setFileList(VideoFileList &fileList)
{
    HI_S32 s32Ret = -1;

//    mCurrentFileIndex = 0;
    mPlayPts = NORMALPTS;
    mRate = 1;
    mDuration = 1;

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
    memcpy(&m_Thread_Attr,&VdecAttr,sizeof (VdecThreadParam));
    qDebug("Set_VdecAttr  m_Thread_Attr.s32MinBufSize=%d\n",m_Thread_Attr.s32MinBufSize);
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

bool VideoPlay::dealFileHead(char *filename)
{

}

HI_BOOL VideoPlay::Start_Vdec(char *filename, VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    char* filetype = nullptr;
    HI_S32 s32Ret;
    SIZE_S  stSize;
    VB_CONF_S       stModVbConf;
    SIZE_S stRotateSize;
    PAYLOAD_TYPE_E enType;
    RECT_S pos;
    VdecThreadParam stVdecParam;

    memset(&stVdecParam,0x0,sizeof(VdecThreadParam));

    stSize.u32Width = WIDTH;
    stSize.u32Height = HEIGHT;

//    if(mVideoFileList.count() == 0){
//        qDebug()<<"no file";
//        return HI_FALSE;
//    }

    filetype = Get_FileType(filename);
    if(!filetype){
        SAMPLE_PRT("get file type failed!\n");
        return HI_FALSE;
    }
    if(!Sample_Common_Vdec::SAMPLE_COMM_VDEC_GetPayload_Type(filetype,&enType)){
        SAMPLE_PRT("get enType failed!\n");
        return HI_FALSE;
    }

    m_pVdec = new Sample_Common_Vdec(1,1,0);

    printf("init CommonPool\n");
    m_pVdec->SAMPLE_COMM_VDEC_ModCommPoolConf(&stModVbConf, enType, &stSize, 1, HI_FALSE);
    s32Ret = m_pVdec->SAMPLE_COMM_VDEC_InitModCommVb(&stModVbConf);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("init mod common vb fail for %#x!\n", s32Ret);
        return HI_FALSE;
    }

    m_pVdec->SAMPLE_COMM_VDEC_ChnAttr(1, &m_stVdecChnAttr, enType, &stSize);

    m_pVdec->SAMPLE_COMM_VDEC_SetChnAttr(0,&m_stVdecChnAttr);

    if(VpssGrp < 0){
        stRotateSize.u32Width = stRotateSize.u32Height = MAX2(m_stVdecChnAttr.u32PicWidth, m_stVdecChnAttr.u32PicHeight);
        m_pVpss= new Sample_Common_Vpss(1,1,&stRotateSize,nullptr);
        if(!m_pVpss){
            printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
            return HI_FALSE;
        }else if( HI_FALSE == m_pVpss->SAMPLE_COMM_VPSS_CreatIsSucess()){

            printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
            goto END1;
        }
    }

    s32Ret = m_pVdec->SAMPLE_COMM_VDEC_Start(1);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VDEC fail for %#x!\n", s32Ret);
        goto END2;
    }

    if(VpssGrp < 0){
        pos.s32X = 0;
        pos.s32Y = 0;
        pos.u32Height = HEIGHT;
        pos.u32Width = WIDTH;
        s32Ret = m_Vdec_Vio.SAMPLE_COMM_VO_StartChn(VDEC_VO,pos);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("start vo fail for %#x!\n", s32Ret);
            goto END3;
        }

        m_pVdec->SAMPLE_COMM_VDEC_BindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);

        s32Ret = m_Vdec_Vio.SAMPLE_COMM_VO_BindVpss(VDEC_VO, m_pVpss->m_Grp_Tab[0], VPSS_CHN0);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("vpss bind vo fail for %#x!\n", s32Ret);
            goto END4;
        }
    }else{
        m_pVdec->SAMPLE_COMM_VDEC_BindVpss(m_pVdec->m_Vdec_Tab[0],VpssGrp,VpssChn);
    }

    sprintf(stVdecParam.cFileName, filename);
    stVdecParam.s32MilliSec     = -1;
//        stVdecParam.s32ChnId        = i;
    stVdecParam.s32IntervalTime = 1;
    stVdecParam.u64PtsInit      = 0;
    stVdecParam.u64PtsIncrease  = NORMALPTS;
    stVdecParam.eCtrlSinal      = VDEC_CTRL_START;
    stVdecParam.bLoopSend       = HI_FALSE;
    stVdecParam.bManuSend       = HI_FALSE;
    stVdecParam.enType          = m_stVdecChnAttr.enType;
    stVdecParam.s32MinBufSize   = (m_stVdecChnAttr.u32PicWidth * m_stVdecChnAttr.u32PicHeight * 3)>>1;
    if (PT_H264 == m_stVdecChnAttr.enType  || PT_MP4VIDEO == m_stVdecChnAttr.enType)
    {
        stVdecParam.s32StreamMode  = m_stVdecChnAttr.stVdecVideoAttr.enMode;
    }
    else
    {
        stVdecParam.s32StreamMode = VIDEO_MODE_FRAME;
    }

    Set_VdecAttr(stVdecParam);


    play();
    start();
    return HI_TRUE;
END4:
    m_pVdec->SAMPLE_COMM_VDEC_UnBindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);
    m_Vdec_Vio.SAMPLE_COMM_VO_UnBindVpss(VDEC_VO, m_pVpss->m_Grp_Tab[0], VPSS_CHN0);

END3:
    m_Vdec_Vio.SAMPLE_COMM_VO_StopChn(VDEC_VO);
END2:
    m_pVdec->SAMPLE_COMM_VDEC_Stop(1);

END1:
    m_pVpss->SAMPLE_COMM_VPSS_Stop();

    return HI_FALSE;
}

void VideoPlay::Stop_Vdec()
{
    qDebug()<<"stop vdec";
    m_Vdec_Run = false;
    wait();
    if(m_pVdec){
        m_pVdec->SAMPLE_COMM_VDEC_UnBindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);
        m_Vdec_Vio.SAMPLE_COMM_VO_UnBindVpss(VDEC_VO, m_pVpss->m_Grp_Tab[0], VPSS_CHN0);
        m_Vdec_Vio.SAMPLE_COMM_VO_StopChn(VDEC_VO);
        m_pVdec->SAMPLE_COMM_VDEC_Stop(1);
        m_pVpss->SAMPLE_COMM_VPSS_Stop();
//        mVideoFileList.clear();

        delete m_pVdec;
    }

    m_pVdec = nullptr;
    qDebug()<<"stop vdec end";

}

bool VideoPlay::getOneFrame(VDEC_STREAM_S *pstStream)
{
//    if(mCurFile.isOpen()){
//        if(mCurFileNode.isOpen()){

//        }else {

//        }
//    }else {
//        return false;
//    }

    return true;
}

bool VideoPlay::changeFile(int index)
{
//    MYVIDEO_HEAD videohead;
//    QString filename;
//    HI_U32 preFileTime;

//#ifdef USE_TAB
//                mCurFileNode.close();
//                filename = mVideoFileList.at(mCurrentFileIndex).absoluteFilePath();
//                mCurFileNode.setFileName(filename);
//                mCurFileNode.open(QIODevice::ReadOnly);
//                mCurFileNode.read((char *)&videohead,sizeof (MYVIDEO_HEAD));
//                lastFileTime = videohead.ctime;
////                mCurFileNode.seek(sizeof (FRAME_INDEX_HEAD));
//#endif

//                mCurFile.close();
//                filename = mVideoFileList.at(mCurrentFileIndex).path()+"/"+QString::number(videohead.fileindex)+".h264";
//                mCurFile.setFileName(filename);
////                fpStrm = fopen(filename.data(), "rb");
//                mCurFile.open(QIODevice::ReadOnly);
//                mCurFile.seek(videohead.stoffset);

//                mFileMutex.unlock();
//                if(!mCurFile.isOpen()){
//                    qDebug()<<"can not open file:"<<mVideoFileList.at(mCurrentFileIndex).fileName();
//                    break ;
//                }
//#ifdef USE_TAB
//                if(lastFileTime - preFileTime > 0){
//                    qDebug()<<"endofstream pertime:"<<preFileTime<<" lasttime:"<<lastFileTime;
//                    endofStream();
//                }
//                preFileTime = videohead.mtime;
//#endif
//                qDebug()<<"file end open next success";

    return true;
}

void VideoPlay::endofStream()
{
    VDEC_STREAM_S stStream;

    memset(&stStream, 0, sizeof(VDEC_STREAM_S) );
    stStream.bEndOfStream = HI_TRUE;
    HI_MPI_VDEC_SendStream(m_Thread_Attr.s32ChnId, &stStream, -1);

}

void VideoPlay::pause(){
    qDebug()<<"video pause";
    m_Thread_Attr.eCtrlSinal = VDEC_CTRL_PAUSE;
    emit stateChanged(QMediaPlayer::PausedState);
}

void VideoPlay::play()
{
    qDebug()<<"video play";
    m_Thread_Attr.eCtrlSinal = VDEC_CTRL_START;
    emit stateChanged(QMediaPlayer::PlayingState);
}

void VideoPlay::stop()
{
    qDebug()<<"video stop";
    m_Thread_Attr.eCtrlSinal = VDEC_CTRL_STOP;
    mVideoPlayList.setPosition(0);

    emit stateChanged(QMediaPlayer::StoppedState);
}

void VideoPlay::videoStart()
{
    m_Thread_Attr.eCtrlSinal = VDEC_CTRL_START;
    emit stateChanged(QMediaPlayer::PlayingState);
}

void VideoPlay::setRate(qreal rate)
{
    if(rate < FLT_EPSILON){
        return;
    }

    mRate = rate;
//    mPlayPts /=  rate;
    m_Thread_Attr.u64PtsIncrease  = (HI_U64)(mPlayPts /rate);
    qDebug()<<"u64PtsIncrease:"<<m_Thread_Attr.u64PtsIncrease;
}

void VideoPlay::onRateChanged(qreal rate)
{
    qDebug()<<"rate change";
    setRate(rate);
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
    HI_S32 s32Ret,  i,  start = 0;
    qint64 s32UsedBytes = 0, s32ReadLen = 0;
    HI_U64 u64pts = 0;
    char *buff;

    VDEC_CHN_STAT_S stStat;

    QString filename;
//    struct timeval stv;
//    struct timeval etv;
//    struct timezone tz;

    u64pts = m_Thread_Attr.u64PtsInit;
     m_Vdec_Run = true;
     qDebug()<<"vdec run";
    while (m_Vdec_Run)
    {
//        usleep(20000);
        if (m_Thread_Attr.eCtrlSinal == VDEC_CTRL_STOP || m_Thread_Attr.eCtrlSinal == VDEC_CTRL_PAUSE)
        {
//            qDebug()<<"pause";
            usleep(200000);
            continue;
        }

        mFileMutex.lock();
//        gettimeofday(&stv, &tz);
        HI_MPI_VDEC_Query(m_Thread_Attr.s32ChnId,&stStat);
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
        if(m_Thread_Attr.enType == PT_MJPEG)
        {
             stStream.u32Len  = s32ReadLen - start;
        }
        else
        {
//            fprintf(fpStrm,"len:%#x\n",s32ReadLen);/
            stStream.u32Len  = s32ReadLen;
//            qDebug()<<"len:"<<s32ReadLen<<" stStream.u32Len:"<<stStream.u32Len;
        }
        stStream.bEndOfFrame  = (m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME)? HI_TRUE: HI_FALSE;
        stStream.bEndOfStream = HI_FALSE;

        s32Ret=HI_MPI_VDEC_SendStream(m_Thread_Attr.s32ChnId, &stStream, m_Thread_Attr.s32MilliSec);
        m_Thread_Attr.cUserCmd = 0;
        if (HI_SUCCESS != s32Ret)
        {
            usleep(100);
            qDebug("send error s32Ret:%#x",s32Ret);
        }
        else
        {
            if(m_Thread_Attr.enType == PT_MJPEG)
            {
                s32UsedBytes = s32UsedBytes +s32ReadLen;
            }
            else
            {
                s32UsedBytes = s32UsedBytes +s32ReadLen + start;
            }
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
