#include "h264videosource.h"


H264VideoSource *H264VideoSource::createNew(UsageEnvironment& env,int Chn)
{

    return new H264VideoSource(env,Chn);

}

H264VideoSource::H264VideoSource(UsageEnvironment &env, int Chn):FramedSource(env)
  ,mChn(Chn),mCache(nullptr),mCacheLen(0),mPos(0)
{

}

H264VideoSource::~H264VideoSource()
{
    printf("exit %s:%d\n",__FUNCTION__,__LINE__);
//    mVideoStreamPro.stopVenc(mChn);
}

void H264VideoSource::doGetNextFrame()
{
    int ret;
    uint n;
    uint CurPackPos;
    struct timeval TimeoutVal;

    if(!mHaveStartedReading){
        QList<uint>start;
        QList<uint>end;
        start.append(0);
        end.append(QDateTime::currentDateTime().toTime_t());
        mVideoStreamPro.setTime(start,end,VIDEO_NORMAL);
        if(!mVideoStreamPro.startVenc(mChn,REALTIME1)){
            qDebug()<<"start venc error";
            return;
        }
        mCache = (char *)malloc(CACHELEN);
        if(!mCache){
            qDebug()<<"malloc failed";
        }
        qDebug()<<"malloc sucess";
        mFd = mVideoStreamPro.getVencFd();
        mVencChn = mVideoStreamPro.getVencChn();
        mHaveStartedReading = true;
        fFrameSize = 0;

//        if(!mVideoStreamPro.setVencAttr(PIC_CIF,0)){
//            qDebug()<<"mVideoStreamPro.setVencAttr error";
//        }

    }

    if (!isCurrentlyAwaitingData()) {
        printf("isCurrentlyAwaitingData\n");
      return ;
    }

    if(mCacheLen != 0){
//        qDebug()<<"mCacheLen:"<<mCacheLen<<" fMaxSize:"<<fMaxSize;
        if(mCacheLen <= fMaxSize){
            fFrameSize = mCacheLen;
        }else {
            fFrameSize = fMaxSize;
        }
        mCacheLen -= fFrameSize;
        memmove(fTo, &mCache[mPos], fFrameSize);
        mPos +=  fFrameSize;

    }else {
        FD_ZERO(&mRead_fds);
        FD_SET(mFd, &mRead_fds);

        TimeoutVal.tv_sec  = 0;
        TimeoutVal.tv_usec = 200000;

        ret = select(mFd + 1, &mRead_fds, nullptr, nullptr, &TimeoutVal);
        if(ret < 0){
            printf("selsect error\n");
            fFrameSize = 0;
        }else if(ret ==0){
            printf("selsect timeout\n");
            fFrameSize = 0;
        }else {
            memset(&mStStream, 0, sizeof(mStStream));
            ret = HI_MPI_VENC_Query(mVencChn, &mStStat);
            if (HI_SUCCESS != ret)
            {
                SAMPLE_PRT("HI_MPI_VENC_Query chn[%d] failed with %#x!\n", mVencChn, ret);
                return;

            }
            if (0 == mStStat.u32CurPacks)
            {
                SAMPLE_PRT("NOTE: Current  frame is NULL!\n");
                return;
            }

            mStStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * mStStat.u32CurPacks);
            if (nullptr == mStStream.pstPack)
            {

                SAMPLE_PRT("malloc stream pack failed!\n");
                return;

            }
            /*******************************************************
             step 2.4 : call mpi to get one-frame stream
            *******************************************************/
            mStStream.u32PackCount = mStStat.u32CurPacks;
            ret = HI_MPI_VENC_GetStream(mVencChn, &mStStream, 200000);
            if (HI_SUCCESS != ret)
            {
                free(mStStream.pstPack);
                mStStream.pstPack = nullptr;
                SAMPLE_PRT("HI_MPI_VENC_GetStream failed with %#x!\n", \
                           ret);
                return;

            }

            mCurFrameSize = 0;
            for(n = 0; n < mStStream.u32PackCount ;n++){
                mCurFrameSize += mStStream.pstPack[n].u32Len - mStStream.pstPack[n].u32Offset;
            }

            if(mCurFrameSize <= fMaxSize){
                CurPackPos = 0;
                for(n = 0; n < mStStream.u32PackCount ;n++){
                    memmove(fTo/*[CurPackPos]*/, mStStream.pstPack[n].pu8Addr + mStStream.pstPack[n].u32Offset, mStStream.pstPack[n].u32Len - mStStream.pstPack[n].u32Offset);
                    CurPackPos += mStStream.pstPack[n].u32Len - mStStream.pstPack[n].u32Offset;
                }

                fFrameSize = mCurFrameSize;
//                qDebug()<<"frameSize:"<<fFrameSize<<" fMaxSize:"<<fMaxSize;
            }else {
                if(mCurFrameSize < CACHELEN){
                    fFrameSize = fMaxSize;
                    CurPackPos = 0;
                    for(n = 0; n < mStStream.u32PackCount ;n++){
                        memmove(mCache+CurPackPos, mStStream.pstPack[n].pu8Addr + mStStream.pstPack[n].u32Offset, mStStream.pstPack[n].u32Len - mStStream.pstPack[n].u32Offset);
                        CurPackPos += mStStream.pstPack[n].u32Len - mStStream.pstPack[n].u32Offset;
                    }
                    mCacheLen = mCurFrameSize;

                    memmove(fTo,mCache,fFrameSize);
                    mCacheLen -= fFrameSize;

                    mPos = 0;
                    memmove(mCache,mCache+fFrameSize,mCacheLen);
                }

            }

            ret = HI_MPI_VENC_ReleaseStream(mVencChn, &mStStream);
            if (HI_SUCCESS != ret)
            {
                free(mStStream.pstPack);
                mStStream.pstPack = nullptr;
                return;
            }

            free(mStStream.pstPack);
            mStStream.pstPack = nullptr;

        }
    }

    if (fFrameSize == 0) {
//        doStopGettingFrames();
//      handleClosure();
      return;
    }

    gettimeofday(&fPresentationTime, NULL);

    FramedSource::afterGetting(this);

    return;

}

void H264VideoSource::doStopGettingFrames()
{
    printf("stop get frame\n");
    if(!mVideoStreamPro.isStop()){
        mVideoStreamPro.stopVenc(mChn);
        mHaveStartedReading = false;
        free(mCache);
    }

    envir().taskScheduler().unscheduleDelayedTask(nextTask());
}
