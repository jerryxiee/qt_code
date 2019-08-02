#include "videoplaylist.h"
#include <QThread>

VideoPlayList::VideoPlayList(QObject *parent) : QObject(parent)
{

}

VideoPlayList::~VideoPlayList()
{
    if(mProcess.isRunning()){
        mCalcFram = false;
        qDebug()<<"kill process";
        mProcess.waitForFinished();
        qDebug()<<"process finished";
    }

    if(pFileTimeTab){
        HI_MPI_SYS_MmzFree(u32PhyAddr, pFileTimeTab);
    }
    if(mFileCache){
        HI_MPI_SYS_MmzFree(mFileCachePhyAddr, mFileCache);
        qDebug()<<"HI_MPI_SYS_MmzFree mFileCache";
    }

    pFileTimeTab = nullptr;
    mFileCache = nullptr;
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
    uint offset;
    qint64 fileoff;
    int fileindex = getFileIndex(position);
    if(fileindex < 0){
        qDebug()<<"setPosition error";
        return;
    }
    qDebug()<<"get fileindex:"<<fileindex;
    if(fileindex != 0){
        offset = position > pFileTimeTab[fileindex -1]?position - pFileTimeTab[fileindex -1]:0;

    }else {
        offset = position < pFileTimeTab[0]?position:pFileTimeTab[0]-1;
        position = offset;
    }

//    mCurSouFile.seek(mVideoFileList.at(fileindex).getStartOffset());
    fileoff = getOffset(fileindex,offset);
    if(fileoff < 0){
        qDebug()<<"getoffset error";
        return;
    }
    qDebug()<<"fileindex:"<<fileindex<<" pos:"<<offset<<" mCurFileOffset:"<<mCurFileOffset;


    mFileMutex.lock();

    if(mCurFileIndex.isOpen()){
        mCurFileIndex.close();
    }
    if(mCurSouFile.isOpen()){
        mCurSouFile.close();
    }

    mCurFileIndex.setFileName(mVideoFileList.at(fileindex).getFileName());
    if(!mCurFileIndex.open(QIODevice::ReadOnly)){
        qDebug()<<"open file error "<<mVideoFileList.at(fileindex).getFileName();
        mFileMutex.unlock();
        return;
    }

    MYVIDEO_HEAD videohead;
    mCurFileIndex.read((char *)&videohead,sizeof (MYVIDEO_HEAD));
    mCurFileIndex.close();
    QString filename = mVideoFileList.at(fileindex).path()+"/"+QString::number(videohead.fileindex)+".h264";

    mCurSouFile.setFileName(filename);
    if(!mCurSouFile.open(QIODevice::ReadOnly)){
        qDebug()<<"can not open "+filename;
        mFileMutex.unlock();
        return;
    }

    mCurrentFileListIndex = fileindex;
    mCurFileOffset = fileoff;

    mPosition = position;
    emit positionChanged(position);
    mFileMutex.unlock();
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

//    return -1;
}

int VideoPlayList::getOffset(int index,quint32 time)
{

    if(index >= mLoadFileNum){
        qDebug()<<"index out of range:"<<index;
        return -1;
    }

    QString filename = mVideoFileList.at(index).getFileName();
    int offset = sizeof(MYVIDEO_HEAD) + (mVideoFileList.at(index).getStartIndex()+time) *sizeof(FRAME_INDEX);
    FRAME_INDEX frameindex;
    qint64 readlen;

    QFile tabfile(filename);
    if(tabfile.exists()){
        tabfile.open(QIODevice::ReadOnly);
        if(!tabfile.isOpen()){
            qDebug()<<"can not open file "<<tabfile.fileName();
            return -1;
        }
        tabfile.seek(offset);
        readlen = tabfile.read((char *)&frameindex,sizeof (FRAME_INDEX));
        if(readlen <= 0){
            tabfile.close();
            return -1;
        }
        tabfile.close();
    }else {
        return -1;
    }

    return frameindex.offset;
}

int VideoPlayList::getFileIndex(quint32 time)
{
    if(mLoadFileNum == 0){
        qDebug()<<"loadfile null";
        return -1;
    }

    return serach(pFileTimeTab,0,mLoadFileNum-1,time);
}

bool VideoPlayList::changeCurFile()
{
    if(mCurSouFile.pos() > mVideoFileList.at(mCurrentFileListIndex).getEndOffset()||mCurSouFile.atEnd()){
        mCurrentFileListIndex++;
        if(mCurrentFileListIndex >= mVideoFileList.count()){
            qDebug()<<"end of file";
            mCurrentFileListIndex = mVideoFileList.count();
            mCurSouFile.close();
//            mCurFileIndex.close();
            emit filefinished();
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
        mCurFileIndex.close();
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
//        qint64 fileoff = getOffset(mCurrentFileListIndex,0);
//        if(fileoff < 0){
            mCurFileOffset = mVideoFileList.at(mCurrentFileListIndex).getStartOffset();
//        }else {
//            mCurFileOffset = fileoff;
//        }

        mCurSouFile.seek(mCurFileOffset);
//        mCurFileOffset = mVideoFileList.at(mCurrentFileListIndex).getStartOffset();
    }

    return true;

}

void VideoPlayList::calcDuration()
{
    int i;
//    HI_U32 *pTab = pFileTimeTab;

    struct timeval stv;
    struct timeval etv;
    struct timezone tz;
    gettimeofday(&stv, &tz);
    qDebug()<<"start calc frame num";

//    QThread::sleep(1);
    for(i = 0; i < mVideoFileList.count();i++){
        if(i != 0){
            pFileTimeTab[i] = pFileTimeTab[i-1] + mVideoFileList.at(i).getEndIndex() - mVideoFileList.at(i).getStartIndex() + 1;

        }else {
            pFileTimeTab[i] = mVideoFileList.at(i).getEndIndex() - mVideoFileList.at(i).getStartIndex() + 1;
        }
        mDuration = pFileTimeTab[i];
        mLoadFileNum++;
        if(!mCalcFram){
            break;
        }
    }
    gettimeofday(&etv, &tz);

    qDebug()<<"search time(us):"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);
    qDebug()<<"i:"<<i<<"mTotalTime:"<<pFileTimeTab[i - 1];

//    mDuration = pFileTimeTab[i - 1];
    if(mDuration != 0)
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
    mLoadFileNum = 0;

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
    memset(pFileTimeTab,0x0,playlist.count() * sizeof(HI_U32));
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
    calcDuration();

//    mProcess = QtConcurrent::run(this,&VideoPlayList::calcDuration);
//    qDebug()<<"run caclframe process";

    MYVIDEO_HEAD videohead;

    if(mCurFileIndex.isOpen()){
        mCurFileIndex.close();
    }
    mCurFileIndex.setFileName(mVideoFileList.first().getFileName());
    if(!mCurFileIndex.open(QIODevice::ReadOnly)){
        qDebug()<<"open file error "<<mVideoFileList.first().getFileName();
    }

    mCurFileIndex.read((char *)&videohead,sizeof (MYVIDEO_HEAD));
    mCurFileIndex.close();

    if(mCurSouFile.isOpen()){
        mCurSouFile.close();
    }
    QString filename = mVideoFileList.first().path()+"/"+QString::number(videohead.fileindex)+".h264";
    mCurSouFile.setFileName(filename);
    mCurSouFile.open(QIODevice::ReadOnly);
    if(!mCurSouFile.isOpen()){
        qDebug()<<"open file error "<<mVideoFileList.first().getFileName();
    }
//    mCurSouFile.seek(mVideoFileList.first().getStartOffset());
    mCurFileOffset = mVideoFileList.first().getStartOffset();
//    qint64 fileoff = getOffset(0,0);
//    if(fileoff < 0){
//        mCurFileOffset = mVideoFileList.at(0).getStartOffset();
//    }else {
//        mCurFileOffset = fileoff;
//    }
    qDebug()<<"file size:"<<mCurSouFile.size();

}

int VideoPlayList::getFrame(char **buff, VIDEO_MODE_E mode)
{
//    struct timeval stv;
//    struct timeval etv;
//    struct timezone tz;

    bool bFindStart;
    bool bFindEnd;
    int i;
//    if(len < )
    int readlen;
    if(!mCurSouFile.isOpen()||mCurrentFileListIndex>mVideoFileList.count()){
        emit filefinished();
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
//gettimeofday(&stv, &tz);
        bFindStart = false;
        bFindEnd   = false;
        mCurSouFile.seek(mCurFileOffset);
        readlen = mCurSouFile.read(mFileCache,MINBUFSIZE);
        if (readlen == 0)
        {
            mFileMutex.unlock();
            qDebug()<<"file read len:"<<readlen;
//            mCurSouFile.close();
            return 0;

        }
//        gettimeofday(&etv, &tz);

//        qDebug()<<"read fram usec:"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);

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
                if(tmp == 5){
                    mPosition++;
                    emit positionChanged(mPosition);
                }
//                qDebug()<<"start:"<<start;
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

//        gettimeofday(&etv, &tz);

//        qDebug()<<"deal fram usec:"<<etv.tv_sec*1000000 + etv.tv_usec - (stv.tv_sec * 1000000 + stv.tv_usec);
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
