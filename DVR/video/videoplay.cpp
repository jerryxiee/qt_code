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

    mFileCache = nullptr;
    pFramTab = nullptr;
    m_pVdec = nullptr;
    mVideoFileList.clear();

}

VideoPlay::~VideoPlay()
{
    mVideoFileList.clear();
    qDebug("enter %s:%d",__FUNCTION__,__LINE__);
    if(m_pVdec){
        delete m_pVdec;
    }
    if(m_pVpss){
        delete m_pVpss;
    }
    if(pFramTab){
        HI_MPI_SYS_MmzFree(u32PhyAddr, pFramTab);
    }
    if(mFileCache){
        HI_MPI_SYS_MmzFree(mFileCachePhyAddr, mFileCache);
        qDebug()<<"HI_MPI_SYS_MmzFree mFileCache";
    }

}

int VideoPlay::serach(HI_U32 *value,int start,int end,int num)
{
    int index = start + (end - start)/2;

    if(start == index){
        if(value[index] < num && index < mVideoFileList.count() - 1){
            if(num <= value[index + 1]){
                return index+1;
            }else {
                qDebug()<<"beyond current frame tab";
                return -1;
            }
        }else{
            return index;
        }
    }

    if(value[index] >= num){
        serach(value, start,index,num);
    }else {
        serach(value, index,end,num);
    }

}

int VideoPlay::getFileIndex(HI_U32 framenum)
{
    if(mTotalFileNum == 0){
        return -1;
    }

    return serach(pFramTab,0,mTotalFileNum-1,framenum);
}

off_t VideoPlay::getFrameOffset(int fileindex,int frame)
{
    if(frame <= 0){
        return 0;
    }
    QString filename = mVideoFileList.at(fileindex).path()+"/."+mVideoFileList.at(fileindex).baseName();
    int offset = sizeof(FRAME_INDEX_HEAD) + (frame - 1) *sizeof(FRAME_INDEX);
    FRAME_INDEX frameindex;

    QFile tabfile(filename);
    if(tabfile.exists()){
        tabfile.open(QIODevice::ReadOnly);
        if(!tabfile.isOpen()){
            qDebug()<<"can not open file "<<tabfile.fileName();
            return -1;
        }
        tabfile.seek(offset);
        tabfile.read((char *)&frameindex,sizeof (FRAME_INDEX));
        tabfile.close();
    }else {
        return -1;
    }

    return frameindex.offset;
}

void VideoPlay::caclFramNum()
{
    int i;
    QFile file;
    QString filepath = mVideoFileList.at(0).path();
    FRAME_INDEX_HEAD framhead = {0,0};
    HI_U32 *pTab = pFramTab;

    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    qDebug()<<"start calc frame num";

    for(i = 0; i < mVideoFileList.count();i++){
        file.setFileName(mVideoFileList.at(i).path()+"/."+mVideoFileList.at(i).baseName());
        if(!file.open(QIODevice::ReadOnly)){
            file.close();
            qDebug()<<"open file failed "<<file.fileName();
            return;
        }
        file.read((char *)&framhead,sizeof (FRAME_INDEX_HEAD));
        if(i != 0){
            pTab[i] = pTab[i-1] + framhead.framenum;

        }else {
            pTab[i] = framhead.framenum;
        }
        file.close();
        mTotalFileNum++;
        if(!mCalcFram){
            break;
        }
    }
    gettimeofday(&etv, &tz);

    qDebug()<<"search time(us):"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);
    qDebug()<<"i:"<<i<<"mTotalFramNum:"<<pTab[i - 1];

    mTotalFramNum = pTab[i - 1];

}

void VideoPlay::setFileList(QFileInfoList &fileList)
{
    HI_S32 s32Ret = -1;

    if(mProcess.isStarted()){
        mCalcFram = false;
        mProcess.waitForFinished();
    }

    mVideoFileList = fileList;
    mTotalFramNum =   0xffffffff;
    mCurrentPrecent = 0;
    mCurrentFileIndex = 0;
    mCurFrameIndex = 0;
    mTotalFileNum = 0;

    s32Ret = HI_MPI_SYS_MmzAlloc(&u32PhyAddr, (void **)(&pFramTab),
                                 "frametab", nullptr, fileList.count() * sizeof(HI_U32));
    if (HI_SUCCESS != s32Ret)
    {
        qDebug("HI_MPI_SYS_MmzAlloc failed with %#x",s32Ret);
        HI_MPI_SYS_MmzFree(u32PhyAddr, pFramTab);
        pFramTab = nullptr;
        return;
//        return NULL;
    }
    qDebug("HI_MPI_SYS_MmzAlloc successed!size:%d",fileList.count() * sizeof(HI_U32));

    mCalcFram = true;
//    caclFramNum();

    mProcess = QtConcurrent::run(this,&VideoPlay::caclFramNum);
    qDebug()<<"run caclframe process";

}

void VideoPlay::setCurrentposition(int percent)
{
    HI_U32 framenum = percent * mTotalFramNum /100;
    int fileindex;
    off_t frameoff = 0;
    int frameindex = 0;

    fileindex = getFileIndex(framenum);
    if(fileindex < 0){
        fileindex = mTotalFileNum > 0 ? mTotalFileNum -1 : 0;
        frameindex = 0;
        frameoff = 0;
        mCurFrameIndex = mTotalFileNum > 0 ? pFramTab[mTotalFileNum -1]:0;
        goto END;
    }
    endofStream();
    pause();
    if(fileindex == 0){
        frameindex = framenum - FRAMENUM > 0 ? framenum - FRAMENUM:0;
    }else{
        frameindex = framenum - pFramTab[fileindex -1] - FRAMENUM > 0?framenum - pFramTab[fileindex -1] - FRAMENUM:0;
    }
    frameoff = getFrameOffset(fileindex,frameindex);
    if(frameoff < 0){
        qDebug()<<"get frame offset failed";
        return;
    }

    mCurFrameIndex = framenum > FRAMENUM ? framenum - FRAMENUM : 0;

END:
    qDebug()<<"frameoff:"<<frameoff;

    mFileMutex.lock();
    if(mCurFile.isOpen()){
        if(mCurrentFileIndex != fileindex){
            mCurFile.close();
            QString filename = mVideoFileList.at(fileindex).absoluteFilePath();

            mCurFile.setFileName(filename);
            mCurFile.open(QIODevice::ReadOnly);

#ifdef USE_TAB
            mCurFileNode.close();
            filename = mVideoFileList.at(fileindex).path()+"/."+mVideoFileList.at(fileindex).baseName();
            mCurFileNode.setFileName(filename);
            mCurFileNode.open(QIODevice::ReadOnly);

#endif
            mCurrentFileIndex = fileindex;
        }
        mCurFile.seek(frameoff);
#ifdef USE_TAB
        if(frameoff == 0){
            mCurFileNode.seek(sizeof (FRAME_INDEX_HEAD));
        }else {
            mCurFileNode.seek(sizeof (FRAME_INDEX_HEAD) + (frameindex-1)*sizeof(FRAME_INDEX));
        }
#endif
    }

    setPosition(mCurFrameIndex * 100 / mTotalFramNum);
    mFileMutex.unlock();
    play();
    qDebug()<<"framenum:"<<framenum<<" fileindex"<<fileindex<<" mCurFrameIndex:"<<mCurFrameIndex;
}

void VideoPlay::setPosition(uint value)
{
    mCurrentPrecent = value;

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

    if(mVideoFileList.count() == 0){
        qDebug()<<"no file";
        return HI_FALSE;
    }

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

    s32Ret = HI_MPI_SYS_MmzAlloc(&mFileCachePhyAddr, (void **)(&mFileCache),
                                 "FileCache", nullptr, stVdecParam.s32MinBufSize);

    if(s32Ret != HI_SUCCESS)
    {
        printf("SAMPLE_TEST:can't alloc %d in send stream thread:%d\n", m_Thread_Attr.s32MinBufSize, m_Thread_Attr.s32ChnId);
        HI_MPI_SYS_MmzFree(mFileCachePhyAddr, mFileCache);
        mFileCache = nullptr;
        goto END4;
    }
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
    if(m_pVdec){
        m_pVdec->SAMPLE_COMM_VDEC_UnBindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);
        m_Vdec_Vio.SAMPLE_COMM_VO_UnBindVpss(VDEC_VO, m_pVpss->m_Grp_Tab[0], VPSS_CHN0);
        m_Vdec_Vio.SAMPLE_COMM_VO_StopChn(VDEC_VO);
        m_pVdec->SAMPLE_COMM_VDEC_Stop(1);
        m_pVpss->SAMPLE_COMM_VPSS_Stop();
        mVideoFileList.clear();

        delete m_pVdec;
    }

    if(mProcess.isRunning()){
        qDebug()<<"process kill";
        mCalcFram = false;
        mProcess.waitForFinished();
        qDebug()<<"process finished";
    }

    if(pFramTab){
        HI_MPI_SYS_MmzFree(u32PhyAddr, pFramTab);
        qDebug()<<"HI_MPI_SYS_MmzFree";
    }
    if(mFileCache){
        HI_MPI_SYS_MmzFree(mFileCachePhyAddr, mFileCache);
        qDebug()<<"HI_MPI_SYS_MmzFree mFileCache";
    }

    mFileCache = nullptr;
    pFramTab = nullptr;
    m_pVdec = nullptr;
    qDebug()<<"stop vdec end";

}

bool VideoPlay::getOneFrame(VDEC_STREAM_S *pstStream)
{
    if(mCurFile.isOpen()){
        if(mCurFileNode.isOpen()){

        }else {

        }
    }else {
        return false;
    }

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
    if(mCurFile.isOpen()){
        endofStream();
        setPosition(0);
        mCurrentFileIndex = 0;
        mCurFrameIndex = 0;
        mFileMutex.lock();
        mCurFile.close();
        QString filename = mVideoFileList.first().absoluteFilePath();

        mCurFile.setFileName(filename);
        mCurFile.open(QIODevice::ReadOnly);

#ifdef USE_TAB
        mCurFileNode.close();
        filename = mVideoFileList.first().path()+"/."+mVideoFileList.first().baseName();
        mCurFileNode.setFileName(filename);
        mCurFileNode.open(QIODevice::ReadOnly);
        mCurFileNode.seek(sizeof (FRAME_INDEX_HEAD));
#endif
        mFileMutex.unlock();
        if(!mCurFile.isOpen()){
            qDebug()<<"can not open file:"<<mVideoFileList.at(mCurrentFileIndex).fileName();
            return ;
        }
    }

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
    m_Thread_Attr.u64PtsIncrease  = (HI_U64)(NORMALPTS /rate);
    qDebug()<<"u64PtsIncrease:"<<m_Thread_Attr.u64PtsIncrease;
}

void VideoPlay::onRateChanged(qreal rate)
{
    qDebug()<<"rate change";
    setRate(rate);
}


void VideoPlay::run()
{
//    VdecThreadParam *pstVdecThreadParam =(VdecThreadParam *)pArgs;
//    FILE *fpStrm=fopen("frameinfo","wb");
//    HI_U8 *pu8Buf = nullptr;
    VDEC_STREAM_S stStream;
    HI_BOOL bFindStart, bFindEnd;
    HI_S32 s32Ret,  i,  start = 0;
    qint64 s32UsedBytes = 0, s32ReadLen = 0;
    HI_U64 u64pts = 0;
    FRAME_INDEX fram;
    FRAME_INDEX_HEAD framehead;
//    HI_S32 len;
//    HI_BOOL sHasReadStream = HI_FALSE;
//    int index = 0;
//    HI_U32 framenum = 0;
    VDEC_CHN_STAT_S stStat;
    HI_U32 preFileTime,lastFileTime;

    QString filename = mVideoFileList.first().absoluteFilePath();
    qDebug()<<"set curfile name";
    mCurFile.setFileName(filename);
    mCurFile.open(QIODevice::ReadOnly);
    if(!mCurFile.isOpen()){
        qDebug()<<"can not open file:"<<mVideoFileList.at(mCurrentFileIndex).fileName();
        return ;
    }
    qDebug()<<"curfile open success";

//    QFileInfo fileinfo(filename);
//    preFileTime = fileinfo.lastModified().toTime_t();
//    qDebug()<<"create time:"<<fileinfo.created().toString("yyyy-MM-dd-hh-mm-ss")<<"lastmod time:"<<fileinfo.lastModified().toString("yyyy-MM-dd-hh-mm-ss");

#ifdef USE_TAB
    qDebug()<<"curfilenode set name";
    filename = mVideoFileList.first().path()+"/."+mVideoFileList.first().baseName();
    mCurFileNode.setFileName(filename);
    mCurFileNode.open(QIODevice::ReadOnly);
    mCurFileNode.read((char *)&framehead,sizeof (FRAME_INDEX_HEAD));
    preFileTime = framehead.mtime;
    qDebug()<<"curfilenode open success";
#endif
    fflush(stdout);

    u64pts = m_Thread_Attr.u64PtsInit;
     m_Vdec_Run = true;
     qDebug()<<"vdec run";
    while (m_Vdec_Run)
    {
//        usleep(20000);
        if (m_Thread_Attr.eCtrlSinal == VDEC_CTRL_STOP || m_Thread_Attr.eCtrlSinal == VDEC_CTRL_PAUSE)
        {
            usleep(200000);
            continue;
        }
//        else if (m_Thread_Attr.eCtrlSinal == VDEC_CTRL_PAUSE)
//        {
//            usleep(200000);
//            continue;
//        }

        if(mCurFrameIndex >= mTotalFramNum){
            usleep(100000);
            endofStream();
            pause();
            qDebug()<<"pause";
            continue;
        }
        if(mCurFile.atEnd()){
//            memset(&stStream, 0, sizeof(VDEC_STREAM_S) );
//            stStream.bEndOfStream = HI_TRUE;
//            HI_MPI_VDEC_SendStream(m_Thread_Attr.s32ChnId, &stStream, -1);

            mCurrentFileIndex ++;
            if(mCurrentFileIndex < mVideoFileList.count()){
//                fclose(fpStrm);
                mFileMutex.lock();
                mCurFile.close();
                s32UsedBytes = 0;
                filename = mVideoFileList.at(mCurrentFileIndex).absoluteFilePath();
                mCurFile.setFileName(filename);
//                fpStrm = fopen(filename.data(), "rb");
                mCurFile.open(QIODevice::ReadOnly);
#ifdef USE_TAB
                mCurFileNode.close();
                filename = mVideoFileList.at(mCurrentFileIndex).path()+"/."+mVideoFileList.at(mCurrentFileIndex).baseName();
                mCurFileNode.setFileName(filename);
                mCurFileNode.open(QIODevice::ReadOnly);
                mCurFileNode.read((char *)&framehead,sizeof (FRAME_INDEX_HEAD));
                lastFileTime = framehead.ctime;
//                mCurFileNode.seek(sizeof (FRAME_INDEX_HEAD));
#endif

                mFileMutex.unlock();
                if(!mCurFile.isOpen()){
                    qDebug()<<"can not open file:"<<mVideoFileList.at(mCurrentFileIndex).fileName();
                    break ;
                }
#ifdef USE_TAB
                if(lastFileTime - preFileTime > 0){
                    qDebug()<<"endofstream pertime:"<<preFileTime<<" lasttime:"<<lastFileTime;
                    endofStream();
                }
                preFileTime = framehead.mtime;
#endif
                qDebug()<<"file end open next success";
            }else {
                qDebug()<<"file list play over";
                endofStream();
                pause();
                usleep(100000);
                continue;
            }

        }

        mFileMutex.lock();
        if ( (m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME) && (m_Thread_Attr.enType == PT_H264) )
        {
#ifdef USE_TAB
            HI_MPI_VDEC_Query(m_Thread_Attr.s32ChnId,&stStat);
            if(stStat.u32LeftStreamFrames > 0){
                mFileMutex.unlock();
                continue;
            }
//            s32UsedBytes = mCurFile.pos();
            mCurFileNode.read((char *)&fram,sizeof (FRAME_INDEX));
            s32ReadLen = mCurFile.read(mFileCache,fram.size);

#else

            bFindStart = HI_FALSE;
            bFindEnd   = HI_FALSE;
//            fseek(fpStrm, s32UsedBytes, SEEK_SET);
            s32UsedBytes = mCurFile.pos();
//            qDebug()<<"offset:"<<s32UsedBytes;
            HI_MPI_VDEC_Query(m_Thread_Attr.s32ChnId,&stStat);
            if(stStat.u32LeftStreamFrames > 0){
                mFileMutex.unlock();
                continue;
            }
//            s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);

            s32ReadLen = mCurFile.read(mFileCache,(qint64)m_Thread_Attr.s32MinBufSize);
            if (s32ReadLen == 0)
            {
                if (m_Thread_Attr.bLoopSend)
                {
                    s32UsedBytes = 0;
//                    fseek(fpStrm, 0, SEEK_SET);
                    mCurFile.seek(0);
//                    s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
                    s32ReadLen = mCurFile.read(mFileCache,(qint64)m_Thread_Attr.s32MinBufSize);
                }
                else
                {
                    mFileMutex.unlock();
                    continue;
                }
            }
            fprintf(fpStrm,"mCurFrameIndex:%#x",mCurFrameIndex);
            fprintf(fpStrm," offset:%#x",s32UsedBytes);

            for (i=0; i<s32ReadLen-8; i++)
            {
                int tmp = mFileCache[i+3] & 0x1F;
                if (  mFileCache[i] == 0 && mFileCache[i+1] == 0 && mFileCache[i+2] == 1 &&
                       (
                           ((tmp == 5 || tmp == 1) && ((mFileCache[i+4]&0x80) == 0x80)) ||
                           (tmp == 20 && (mFileCache[i+7]&0x80) == 0x80)
                        )
                   )
                {
                    bFindStart = HI_TRUE;
                    i += 8;
                    break;
                }
            }

            for (; i<s32ReadLen-8; i++)
            {
                int tmp = mFileCache[i+3] & 0x1F;
                if (  mFileCache[i  ] == 0 && mFileCache[i+1] == 0 && mFileCache[i+2] == 1 &&
                            (
                                  tmp == 15 || tmp == 7 || tmp == 8 || tmp == 6 ||
                                  ((tmp == 5 || tmp == 1) && ((mFileCache[i+4]&0x80) == 0x80)) ||
                                  (tmp == 20 && (mFileCache[i+7]&0x80) == 0x80)
                              )
                   )
                {
                    bFindEnd = HI_TRUE;
                    break;
                }
#endif
            }
#ifndef USE_TAB
            if(i > 0) s32ReadLen = i;
            if (bFindStart == HI_FALSE)
            {
                printf("SAMPLE_TEST: chn %d can not find start code!s32ReadLen %d, s32UsedBytes %d. \n",
                                            m_Thread_Attr.s32ChnId, s32ReadLen, s32UsedBytes);
            }
            else if (bFindEnd == HI_FALSE)
            {
                qDebug()<<"can not found end s32ReadLen:"<<s32ReadLen;
                s32ReadLen = i+8;
            }

        }
#endif

        stStream.u64PTS  = u64pts;
        stStream.pu8Addr = (HI_U8 *)mFileCache + start;
        if(m_Thread_Attr.enType == PT_MJPEG)
        {
             stStream.u32Len  = s32ReadLen - start;
        }
        else
        {
//            qDebug()<<"len:"<<s32ReadLen;
//            fprintf(fpStrm,"len:%#x\n",s32ReadLen);
            stStream.u32Len  = s32ReadLen;
        }
        stStream.bEndOfFrame  = (m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME)? HI_TRUE: HI_FALSE;
        stStream.bEndOfStream = HI_FALSE;
//        fprintf(fpStrm,"frame:%#x offset:%#x len:%#x\n",mCurFrameIndex,s32UsedBytes,s32ReadLen);

//        printf("Send One Frame");
//        fflush(stdout);

        s32Ret=HI_MPI_VDEC_SendStream(m_Thread_Attr.s32ChnId, &stStream, m_Thread_Attr.s32MilliSec);
        m_Thread_Attr.cUserCmd = 0;
        if (HI_SUCCESS != s32Ret)
        {
            usleep(100);
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
            u64pts += m_Thread_Attr.u64PtsIncrease;

            mCurFrameIndex++;
            if(mTotalFramNum != 0){
                if((mCurFrameIndex * 100 /mTotalFramNum) != mCurrentPrecent){
                    setPosition(mCurFrameIndex * 100 /mTotalFramNum);
                }
            }

        }
//        fseek(fpStrm, s32UsedBytes, SEEK_SET);
#ifndef USE_TAB
        mCurFile.seek(s32UsedBytes);
#endif
        mFileMutex.unlock();
       usleep(10000);
    }

    endofStream();
    fflush(stdout);

    mCurFile.close();
    mCurFileNode.close();
    qDebug()<<"exit paly thread";
}
