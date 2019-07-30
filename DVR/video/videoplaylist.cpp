#include "videoplaylist.h"

VideoPlayList::VideoPlayList(QObject *parent) : QObject(parent)
{

}

VideoPlayList::~VideoPlayList()
{
    if(mProcess.isRunning()){
        mCalcFram = false;
        mProcess.waitForFinished();
    }

    if(pFileTimeTab){
        HI_MPI_SYS_MmzFree(u32PhyAddr, pFileTimeTab);
    }
    if(mFileCache){
        HI_MPI_SYS_MmzFree(mFileCachePhyAddr, mFileCache);
        qDebug()<<"HI_MPI_SYS_MmzFree mFileCache";
    }

    qDebug()<<"exit ~VideoPlayList";
}


quint32 VideoPlayList::duration() const
{
    return mDuration;
}

quint32 VideoPlayList::position() const
{
    return mPosition;
}

void VideoPlayList::setPosition(quint32 position)
{
    mPosition = position;
    emit positionChanged(position);
}

int VideoPlayList::serach(quint32 *value, quint32 start, quint32 end, quint32 time)
{
    quint32 index = start + (end - start)/2;

    if(start == index){
        if(value[index] < time && index < mVideoFileList.count() - 1){
            if(time <= value[index + 1]){
                return index+1;
            }else {
                qDebug()<<"beyond current frame tab";
                return -1;
            }
        }else{
            return index;
        }
    }

    if(value[index] >= time){
        serach(value, start,index,time);
    }else {
        serach(value, index,end,time);
    }

    return -1;
}

int VideoPlayList::getFileIndex(quint32 time)
{
    if(mLoadFileNum == 0){
        return -1;
    }

    return serach(pFileTimeTab,0,mLoadFileNum-1,time);
}

bool VideoPlayList::changeCurFile()
{
    if(mCurSouFile.pos() >= mVideoFileList.at(mCurrentFileListIndex).getEndOffset()){
        mCurrentFileListIndex++;
        if(mCurrentFileListIndex >= mVideoFileList.count()){
            qDebug()<<"end of file";
            mCurrentFileListIndex = mVideoFileList.count();
            mCurSouFile.close();
            mCurFileIndex.close();
            return false;
        }
        MYVIDEO_HEAD videohead;

        if(mCurFileIndex.isOpen()){
            mCurFileIndex.close();
        }
        mCurFileIndex.setFileName(mVideoFileList.at(mCurrentFileListIndex).getFileName());
        if(!mCurFileIndex.open(QIODevice::ReadOnly)){
            qDebug()<<"open file error "<<mVideoFileList.at(mCurrentFileListIndex).getFileName();
        }

        mCurFileIndex.read((char *)&videohead,sizeof (MYVIDEO_HEAD));
        QString filename = mVideoFileList.at(mCurrentFileListIndex).path()+"/"+QString::number(videohead.fileindex)+".h264";

        if(mCurSouFile.fileName()!=filename){
            mCurSouFile.close();
            mCurSouFile.setFileName(filename);
            if(!mCurSouFile.open(QIODevice::ReadOnly)){
                qDebug()<<"can not open "+filename;
                return false;
            }
        }
        qDebug()<<"change file end";
        mCurSouFile.seek(mVideoFileList.at(mCurrentFileListIndex).getStartOffset());
        mCurFileOffset = mVideoFileList.at(mCurrentFileListIndex).getStartOffset();
    }

    return true;

}

void VideoPlayList::calcDuration()
{
    int i;
    HI_U32 *pTab = pFileTimeTab;

    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    qDebug()<<"start calc frame num";

    for(i = 0; i < mVideoFileList.count();i++){
        if(i != 0){
            pTab[i] = pTab[i-1] + mVideoFileList.at(i).getModTime() - mVideoFileList.at(i).getCreatTime();

        }else {
            pTab[i] = mVideoFileList.at(i).getModTime() - mVideoFileList.at(i).getCreatTime();
        }
        mLoadFileNum++;
        if(!mCalcFram){
            break;
        }
    }
    gettimeofday(&etv, &tz);

    qDebug()<<"search time(us):"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);
    qDebug()<<"i:"<<i<<"mTotalTime:"<<pTab[i - 1];

    mDuration = pTab[i - 1];
    emit durationChanged(mDuration);


}

//void VideoPlayList::setPlaylist(VideoFileList *playlist)
//{

//}

void VideoPlayList::setPlaylist(VideoFileList &playlist)
{
    HI_S32 s32Ret = -1;

    mVideoFileList = playlist;
    if(mProcess.isRunning()){
        mCalcFram = false;
        mProcess.waitForFinished();
    }

    mCurrentFileListIndex = 0;
    mPosition = 0;
    mDuration = 0;

    if(pFileTimeTab){
        HI_MPI_SYS_MmzFree(u32PhyAddr, pFileTimeTab);
        qDebug()<<"HI_MPI_SYS_MmzFree pFileTimeTab";
        pFileTimeTab = nullptr;
    }
    if(mFileCache){
        HI_MPI_SYS_MmzFree(mFileCachePhyAddr, mFileCache);
        qDebug()<<"HI_MPI_SYS_MmzFree mFileCache";
        mFileCache = nullptr;
    }

    s32Ret = HI_MPI_SYS_MmzAlloc(&u32PhyAddr, (void **)(&pFileTimeTab),
                                 "FileTimeTab", nullptr, playlist.count() * sizeof(HI_U32));
    if (HI_SUCCESS != s32Ret)
    {
        qDebug("HI_MPI_SYS_MmzAlloc failed with %#x",s32Ret);
        HI_MPI_SYS_MmzFree(u32PhyAddr, pFileTimeTab);
        pFileTimeTab = nullptr;
        return;
    }
    qDebug("HI_MPI_SYS_MmzAlloc successed!size:%d",playlist.count() * sizeof(HI_U32));

    s32Ret = HI_MPI_SYS_MmzAlloc(&mFileCachePhyAddr, (void **)(&mFileCache),
                                 "FileCache", nullptr, MINBUFSIZE);

    if(s32Ret != HI_SUCCESS)
    {
        printf("SAMPLE_TEST:can't alloc file cache\n");
        HI_MPI_SYS_MmzFree(mFileCachePhyAddr, mFileCache);
        mFileCache = nullptr;
        return;
    }

    mCalcFram = true;

    mProcess = QtConcurrent::run(this,&VideoPlayList::calcDuration);
    qDebug()<<"run caclframe process";

    MYVIDEO_HEAD videohead;

    mCurFileIndex.setFileName(mVideoFileList.first().getFileName());
    if(!mCurFileIndex.open(QIODevice::ReadOnly)){
        qDebug()<<"open file error "<<mVideoFileList.first().getFileName();
    }

    mCurFileIndex.read((char *)&videohead,sizeof (MYVIDEO_HEAD));

    QString filename = mVideoFileList.first().path()+"/"+QString::number(videohead.fileindex)+".h264";
    mCurSouFile.setFileName(filename);
    mCurSouFile.open(QIODevice::ReadOnly);
    if(!mCurSouFile.isOpen()){
        qDebug()<<"open file error "<<mVideoFileList.first().getFileName();
    }
//    mCurSouFile.seek(mVideoFileList.first().getStartOffset());
    mCurFileOffset = mVideoFileList.first().getStartOffset();
    qDebug()<<"file size:"<<mCurSouFile.size();

}

int VideoPlayList::getFrame(char **buff, VIDEO_MODE_E mode)
{
    bool bFindStart;
    bool bFindEnd;
    int i;
//    if(len < )
    int readlen;
    if(!mCurSouFile.isOpen()){
        return 0;
    }
    mFileMutex.lock();
    if(!changeCurFile()){
        mFileMutex.unlock();
        qDebug()<<"change file error";
        return 0;
    }

    if(mVideoFileList.at(mCurrentFileListIndex).getEnType() == PT_H264 && mode == VIDEO_MODE_FRAME){
//        readlen = mCurSouFile.read(buff,len);

        bFindStart = false;
        bFindEnd   = false;
        mCurSouFile.seek(mCurFileOffset);
        readlen = mCurSouFile.read(mFileCache,MINBUFSIZE);
        if (readlen == 0)
        {
            qDebug()<<"file read len:"<<readlen;
            mCurSouFile.close();
            return 0;

        }

        for (i=0; i<readlen-8; i++)
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

        for (; i<readlen-8; i++)
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
        }

        if(i > 0) readlen = i;
        if (bFindStart == HI_FALSE)
        {
            printf("SAMPLE_TEST: can not find start code!s32ReadLen %d, s32UsedBytes %d. \n",
                                         readlen, mCurFileOffset);
        }
        else if (bFindEnd == HI_FALSE)
        {
            readlen = i+8;
        }

    }else {
        readlen = mCurSouFile.read(mFileCache,MINBUFSIZE);
    }

//    buff = mFileCache;
    *buff = mFileCache;

    mFileMutex.unlock();
//    memcpy(buff,mFileCache,readlen);

//    qDebug()<<"read len:"<<readlen;
    mCurFileOffset += readlen;
    return readlen;


}
