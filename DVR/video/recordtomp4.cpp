#include "recordtomp4.h"

static SystemLog *Syslog = SystemLog::getSystemLog();

#define LOGW1(str) do{ Syslog->logwrite(1,QString(str));   \
    }while(0);

#define LOGWE(fmt,...) do{ Syslog->logwrite(1,fmt,__VA_ARGS__);   \
    }while(0);



RecordToMP4::RecordToMP4(Sample_Common_Vpss &Vpss, QObject *parent) : QThread(parent),mIDRFramBuf(nullptr),
    mVpss(Vpss),mMaxFd(0)
{
    char venc_path_name[VIDEO_FILENAME_SIZE];

    QDir dir(ALARM_FILE_PATH);
    if(!dir.exists()){
        if(!dir.mkpath(ALARM_FILE_PATH)){
            SAMPLE_PRT("mkpath %s failed\n",ALARM_FILE_PATH);
        }
    }

    for (int i = 0;i < VIDEO_MAX_NUM;i++) {
        sprintf(venc_path_name,"%s/channel%d/",VENC_PATH,i);
        QDir recorddir(venc_path_name);
        if(!recorddir.exists()){
            if(!recorddir.mkpath(venc_path_name)){
                SAMPLE_PRT("mkpath %s failed\n",venc_path_name);

            }
        }
    }

    mIDRFramBuf = (unsigned char *)malloc(BUFLEN);
    if(!mIDRFramBuf){
        printf("malloc error\n");
    }

    mVencSet = Settings::getVencIni();

    connect(this,SIGNAL(createNewFileSignal(int)),this,SLOT(onCreateNewFileSlot(int)));

}

RecordToMP4::~RecordToMP4()
{
    if(mIDRFramBuf)
        free(mIDRFramBuf);

}

void RecordToMP4::RecordExit()
{
    mRun = false;
    wait();
    for (int i = 0;i < mVencParam.count();i++) {
        deleteChnFromRecord(mVencParam[i].ViChn);
//        if(mVencParam[i].Mp4File.isOpen()){
//            mVencParam[i].Mp4File.closeMO4File();
//        }
    }
}

bool RecordToMP4::isRecordOpen(int Chn)
{
    if(mVencSet->m_Vdec_Param[0][Chn].mopen)
        return true;

    return false;
}

int RecordToMP4::checkRecordChn(int Chn)
{
    for(int i = 0;i < mVencParam.count();i++) {
        if(mVencParam[i].ViChn == Chn)
            return i;
    }

    return -1;
}

bool RecordToMP4::getAlarmFileName(int Chn,VIDEO_TYPE type,char *filename,int len)
{
    char file[VIDEO_FILENAME_SIZE];

    switch (type) {
        case VIDEO_MOVEDETECT:
        {
            sprintf(file,"%s/%s%d",ALARM_FILE_PATH,MOVED_FILE,Chn);
            break;
        }
        case VIDEO_IO0:
        case VIDEO_IO1:
        case VIDEO_IO2:
        case VIDEO_IO3:
        {
            sprintf(file,"%s/%s%d%d",ALARM_FILE_PATH,IO_FILE,type,Chn);
            break;
        }
    default:{
        return false;
    }
    }
    if(len < strlen(file)){
        return false;
    }
    strcpy(filename,file);

    return true;
}

int RecordToMP4::checkVideoAlarmList(int Chn, VIDEO_TYPE type)
{
    for (int i = 0;i < mVideoEventFileInfoList[Chn].count();i++) {
        if(mVideoEventFileInfoList[Chn].at(i).type == type){
            return i;
        }
    }

    return -1;

}

bool RecordToMP4::checkAlarmFile(char *filename)
{
    QFileInfo alarmFile(filename);
    if(!alarmFile.exists()){
        ALARM_VIDEO_HEAD AlarmFileHead;
        AlarmFileHead.cTime = QDateTime::currentDateTime().toTime_t();
        AlarmFileHead.num = 0;
        FILE *file = fopen(filename,"wb");
        if(!file){
            qDebug()<<"create alarm file error";
            return false;
        }
        fwrite((char *)&AlarmFileHead,sizeof (ALARM_VIDEO_HEAD),1,file);
        fclose(file);
    }

    return true;
}

bool RecordToMP4::checkAlarmFile(int Chn,VIDEO_TYPE type)
{
    char filename[VIDEO_FILENAME_SIZE];

    getAlarmFileName(Chn,type, filename,VIDEO_FILENAME_SIZE);
    QFileInfo alarmFile(filename);
    if(!alarmFile.exists()){
        ALARM_VIDEO_HEAD AlarmFileHead;
        AlarmFileHead.type = type;
        AlarmFileHead.cTime = QDateTime::currentDateTime().toTime_t();
        AlarmFileHead.num = 0;
        FILE *file = fopen(filename,"wb");
        if(!file){
            qDebug()<<"create alarm file error";
            return false;
        }
        fwrite((char *)&AlarmFileHead,sizeof (ALARM_VIDEO_HEAD),1,file);
        fclose(file);
    }

    return true;
}

bool RecordToMP4::initAlarmFile()
{

}

bool RecordToMP4::removeVideoAlarmEventFromlist(int Chn)
{
//    mEventFileMutex.lock();
    mVideoEventFileInfoList[Chn].clear();
//    mEventFileMutex.unlock();
    return true;
}

bool RecordToMP4::removeVideoAlarmEventFromlist(int Chn,VIDEO_TYPE type)
{
    int index;

    index = checkVideoAlarmList(Chn,type);
    if(index < 0){
        return false;
    }

//    mEventFileMutex.lock();
    mVideoEventFileInfoList[Chn].removeAt(index);
//    mEventFileMutex.unlock();

    return true;
}

bool RecordToMP4::addVideoAlarmEventFromlist(int Chn,VIDEO_TYPE type)
{
    ALARM_FILE alarmfile;
    int index;

    index = checkVideoAlarmList(Chn,type);
    if(index < 0){
        alarmfile.type = type;
        alarmfile.sTime = QDateTime::currentDateTime().toTime_t();
//        mEventFileMutex.lock();
        mVideoEventFileInfoList[Chn].append(alarmfile);
//        mEventFileMutex.unlock();
        qDebug()<<"add alarm to list";

        return true;
    }

    return false;

}

bool RecordToMP4::addVideoAlarmToFile(int Chn,VIDEO_TYPE type)
{
    char alarmfile[VIDEO_FILENAME_SIZE];
    FILE *pFile;

    int index = checkRecordChn(Chn);
    if(index < 0){
        qDebug()<<"can not add alarm file,record not on list";
        return false;
    }

    mFileMutex.lock();
    if(!mVencParam[index].Mp4File.isOpen()){
        qDebug()<<"addVideoAlarmToFile error";
        mFileMutex.unlock();
        return false;
    }

    int alarmindex = checkVideoAlarmList(Chn, type);
    if(alarmindex < 0){
        mFileMutex.unlock();
        qDebug()<<"alarm not in list";
        return false;
    }

    getAlarmFileName(Chn,type,alarmfile,VIDEO_FILENAME_SIZE);

    if(!checkAlarmFile(alarmfile)){
        mFileMutex.unlock();
        return false;
    }

    mVideoEventFileInfoList[Chn][alarmindex].eTime = QDateTime::currentDateTime().toTime_t();
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
//    mFileMutex.lock();
    strcpy(mVideoEventFileInfoList[Chn][alarmindex].filename,mVencParam[index].Mp4File.getMP4FileName());
    mFileMutex.unlock();
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    pFile = fopen(alarmfile,"rb+");
    if(!pFile){
        qDebug("open file[%s] error\n",alarmfile);
        return false;
    }
    ALARM_VIDEO_HEAD videoFileHead;
    fread((char *)&videoFileHead,sizeof (ALARM_VIDEO_HEAD),1,pFile);
    videoFileHead.type = mVideoEventFileInfoList[Chn][alarmindex].type;
    videoFileHead.num ++;
    videoFileHead.mtime = QDateTime::currentDateTime().toTime_t();
    videoFileHead.cTime = videoFileHead.cTime < mVideoEventFileInfoList[Chn][alarmindex].sTime?videoFileHead.cTime:mVideoEventFileInfoList[Chn][alarmindex].sTime;

    qDebug()<<"alarm file num:"<<videoFileHead.num;
    fseek(pFile,0,SEEK_END);
    fwrite((void *)&mVideoEventFileInfoList[Chn][alarmindex],sizeof (ALARM_FILE),1,pFile);

    fseek(pFile,0,SEEK_SET);
    fwrite((void *)&videoFileHead,sizeof (ALARM_VIDEO_HEAD),1,pFile);
    fclose(pFile);

    qDebug("write alarm file[%s] sucess!\n",alarmfile);

    return true;


}

bool RecordToMP4::changeAlarmFile(int Chn)
{
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    mEventFileMutex.lock();
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    addVideoAlarmToFile(Chn);
    LOGWE("%s:%d",__FUNCTION__,__LINE__);

    for (int i = 0;i < mVideoEventFileInfoList[Chn].count();i++) {
        mVideoEventFileInfoList[Chn][i].sTime = QDateTime::currentDateTime().toTime_t();
    }

    mEventFileMutex.unlock();
    return true;
}

bool RecordToMP4::addVideoAlarmToFile(int Chn)
{
    char alarmfile[VIDEO_FILENAME_SIZE];
    FILE *pFile;

    int index = checkRecordChn(Chn);
    if(index < 0){
        qDebug()<<"can not add alarm file,record not on list";
        return false;
    }
    mFileMutex.lock();
    if(!mVencParam[index].Mp4File.isOpen()){
        qDebug()<<"addVideoAlarmToFile error";
        mFileMutex.unlock();
        return false;
    }

    for(int i = 0;i < mVideoEventFileInfoList[Chn].count();i++){
        getAlarmFileName(Chn,mVideoEventFileInfoList[Chn][i].type,alarmfile,VIDEO_FILENAME_SIZE);
        if(!checkAlarmFile(alarmfile)){
            continue;
        }
        mVideoEventFileInfoList[Chn][i].eTime = QDateTime::currentDateTime().toTime_t();
//        LOGWE("%s:%d",__FUNCTION__,__LINE__);
//        mFileMutex.lock();
        strcpy(mVideoEventFileInfoList[Chn][i].filename,mVencParam[index].Mp4File.getMP4FileName());
//        mFileMutex.unlock();
//        LOGWE("%s:%d",__FUNCTION__,__LINE__);
        pFile = fopen(alarmfile,"rb+");
        if(!pFile){
            qDebug("open file[%s] error\n",alarmfile);
            continue;
        }
        ALARM_VIDEO_HEAD videoFileHead;
        fread((char *)&videoFileHead,sizeof (ALARM_VIDEO_HEAD),1,pFile);
        videoFileHead.type = mVideoEventFileInfoList[Chn][i].type;
        videoFileHead.num ++;
        videoFileHead.mtime = QDateTime::currentDateTime().toTime_t();

        fseek(pFile,0,SEEK_END);
        fwrite((void *)&mVideoEventFileInfoList[Chn][i],sizeof (ALARM_FILE),1,pFile);

        fseek(pFile,0,SEEK_SET);
        fwrite((void *)&videoFileHead,sizeof (ALARM_VIDEO_HEAD),1,pFile);
        fclose(pFile);

        qDebug("write alarm file[%s] sucess!alarm file num:%d\n",alarmfile,videoFileHead.num);
    }
    mFileMutex.unlock();

    return true;
}

bool RecordToMP4::createNewMp4File(int Chn)
{
    char venc_path_name[VIDEO_FILENAME_SIZE];
    char fileindex_name[VIDEO_FILENAME_SIZE];
    QSize size(1280,720);
    SIZE_S picsize;

    int index = checkRecordChn(Chn);
    if(index < 0){
        qDebug()<<"can not create new file";
        return false;
    }

    if(Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(VIDEO_NORM, mVencSet->m_Vdec_Param[0][Chn].mvencSize, &picsize) == HI_SUCCESS){
        size.setWidth(picsize.u32Width);
        size.setHeight(picsize.u32Height);
    }

    sprintf(venc_path_name,"%s/channel%d/",VENC_PATH,Chn);
    QDir dir(venc_path_name);
    if(!dir.exists()){
        if(!dir.mkpath(venc_path_name)){
            SAMPLE_PRT("mkpath %s failed\n",venc_path_name);
            return HI_FALSE;
        }
    }
//    QDateTime current_date_time =QDateTime::currentDateTime();
//    QString current_date =current_date_time.toString("yyyy-MM-dd-hh-mm-ss");
//    QByteArray file_name = current_date.toLatin1();

    MP4File &file = mVencParam[index].Mp4File;

    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    mFileMutex.lock();
    if(file.isOpen()){
        file.closeMO4File();
    }

    mVencParam[index].filenode.stTime = QDateTime::currentDateTime().toTime_t();

    mPFileTabFind[Chn]->createNewDayTab();

    sprintf(fileindex_name,"%sHI%d.mp4",venc_path_name,mVencParam[index].curFileIndex);
    if(!file.createMP4File(fileindex_name,25,size)){
        qDebug()<<"create new file error";
        return false;
    }

    mFileMutex.unlock();
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    qDebug("start record %s\n",fileindex_name);

    return true;

}

bool RecordToMP4::saveMp4File(int Chn)
{
    int index = checkRecordChn(Chn);
    if(index < 0){
        qDebug()<<"can not create new file";
        return false;
    }
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    mFileMutex.lock();
    if(!mVencParam[index].Mp4File.isOpen()){
        qDebug()<<"mp4file not open";
        return false;
    }
    mVencParam[index].filenode.fileindex = mVencParam[index].curFileIndex;
    mVencParam[index].filenode.duration = QDateTime::currentDateTime().toTime_t();

    mPFileTabFind[Chn]->addFileToTab(mVencParam[index].filenode);
    mVencParam[index].curFileIndex++;

    mVencParam[index].Mp4File.closeMO4File();

    mFileMutex.unlock();
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    return true;

}

bool RecordToMP4::addRecordList(int Chn)
{
    if(checkRecordChn(Chn) < 0){
       VENCPARAM param;
       param.ViChn = Chn;
       param.VencFd = HI_MPI_VENC_GetFd(mPVenc[Chn]->m_Venc_Chn);
       if(param.VencFd > mMaxFd){
           mMaxFd = param.VencFd;
       }
       param.curFileIndex = mPFileTabFind[Chn]->getFileIndex();
//       if(param.curFileIndex != 0){
//           param.curFileIndex++;
//       }
       qDebug()<<"current file index:"<<param.curFileIndex;
       param.filenode.stTime = QDateTime::currentDateTime().toTime_t();

       LOGWE("%s:%d",__FUNCTION__,__LINE__);
       qDebug()<<"add record Chn:"<<Chn;
       mFileMutex.lock();
       mVencParam.append(param);
//       mFileTab.append(filenode);
       qDebug()<<"add record Chn:"<<Chn<<" record num:"<<mVencParam.count();
       mFileMutex.unlock();
       LOGWE("%s:%d",__FUNCTION__,__LINE__);
       return true;
    }

    return false;
}

bool RecordToMP4::deleteFromRecordList(int Chn)
{
    int index = checkRecordChn(Chn);

    if(index < 0){
        qDebug()<<"not record:"<<Chn;
        return false;
    }

    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    mFileMutex.lock();
    mVencParam.removeAt(index);
    mFileMutex.unlock();
    LOGWE("%s:%d",__FUNCTION__,__LINE__);

    return true;

}

bool RecordToMP4::addChnToRecord(int Chn)
{
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    if(!mVencSet->m_Vdec_Param[0][Chn].mopen){
        qDebug()<<"Chn "<<Chn<<"record not open";
        deleteChnFromRecord(Chn);
        return false;
    }
    LOGWE("%s:%d",__FUNCTION__,__LINE__);

//    mFileTab.append()
    if(createNewMp4File(Chn)){
        start();
    }
    LOGWE("%s:%d",__FUNCTION__,__LINE__);

    return true;
}

bool RecordToMP4::deleteChnFromRecord(int Chn)
{
    mEventFileMutex.lock();
    addVideoAlarmToFile(Chn);
    removeVideoAlarmEventFromlist(Chn);
    mEventFileMutex.unlock();
    return saveMp4File(Chn);
}

void RecordToMP4::onCreateNewFileSlot(int Chn)
{
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    changeAlarmFile(Chn);
    LOGWE("%s:%d",__FUNCTION__,__LINE__);
    saveMp4File(Chn);
    createNewMp4File(Chn);

}

void RecordToMP4::onTimeHander()
{
    int i;
    uint time = QDateTime::currentDateTime().toTime_t();

    if(time % (3600/SPLITNUM) == 0){
        for (i = 0;i < mVencParam.count();i++) {
//            if(mVencParam[i].Mp4File.getDuration() >= FILEDURATION){
            qDebug()<<"create new file";
                emit createNewFileSignal(mVencParam[i].ViChn);
//            }
        }
    }


}

void RecordToMP4::onViStatusChangedSlot(VI_CHN Chn,HI_BOOL status)
{
    if(status){

        if(addRecordList(Chn)){
            LOGWE("%s:%d",__FUNCTION__,__LINE__);
            addChnToRecord(Chn);
            LOGWE("%s:%d",__FUNCTION__,__LINE__);
        }

    }else {
        deleteChnFromRecord(Chn);
        deleteFromRecordList(Chn);
    }

}

void RecordToMP4::onVideoAlarmEventChangedSlot(VI_CHN Chn,VIDEO_TYPE type,bool change)
{
    mEventFileMutex.lock();
    if(!isRecordOpen(Chn)){
        qDebug()<<"record not open";
        LOGWE("%s:%d",__FUNCTION__,__LINE__);
        addVideoAlarmToFile(Chn);
        removeVideoAlarmEventFromlist(Chn);
        mEventFileMutex.unlock();
        LOGWE("%s:%d",__FUNCTION__,__LINE__);
        return;
    }

    if(change){
        if(!addVideoAlarmEventFromlist(Chn,type)){
            qDebug()<<"addVideoAlarmEventFromlist falied";
        }
    }else {
        addVideoAlarmToFile(Chn,type);
        if(!removeVideoAlarmEventFromlist(Chn,type)){
            mEventFileMutex.unlock();
            return;
        }
    }
    mEventFileMutex.unlock();
}

bool RecordToMP4::startRecordChn(VI_CHN ViChnCnt, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32 u32BitRate, HI_FR32 frmRate, HI_U32 u32Profile)
{
    HI_S32 s32Ret;

    if(mVpss.m_Grp_Num == 0){
        SAMPLE_PRT("vpss not init!\n");
        return false;
    }

//    qDebug()<<"start venc chn "<<ViChnCnt;
    mPVenc[ViChnCnt] = new Sample_Common_Venc();
    mPVenc[ViChnCnt]->SAMPLE_COMM_VENC_SetAttr(PAYLOAD_TYPE,VIDEO_NORM, enSize, enRcMode,u32BitRate,frmRate,u32Profile);
    s32Ret = mPVenc[ViChnCnt]->SAMPLE_COMM_VENC_Start();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_1;
    }

    s32Ret = mPVenc[ViChnCnt]->SAMPLE_COMM_VENC_BindVpss(mVpss.m_Grp_Tab[ViChnCnt], m_VencBindVpss);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_2;
    }

    mPFileTabFind[ViChnCnt] = FileTabFind::createNewTab(ViChnCnt);
    mPFileTabFind[ViChnCnt]->createNewDayTab();

    return true;
END_2:
    mPVenc[ViChnCnt]->SAMPLE_COMM_VENC_UnBindVpss(mVpss.m_Grp_Tab[ViChnCnt], m_VencBindVpss);
END_1:
    mPVenc[ViChnCnt]->SAMPLE_COMM_VENC_Stop();
    return false;

}

bool RecordToMP4::startRecordChn(int Chn,VIDEO_NORM_E enNorm)
{
    if(Chn > mVencSet->m_Vdec_Param[0].count()){
        qDebug()<<"startRecordChn failed";
        return false;
    }

    return startRecordChn(Chn, mVencSet->m_Vdec_Param[0][Chn].mvencSize,mVencSet->m_Vdec_Param[0][Chn].menRcMode,
            mVencSet->m_Vdec_Param[0][Chn].mu32BitRate,mVencSet->m_Vdec_Param[0][Chn].mfr32DstFrmRate,mVencSet->m_Vdec_Param[0][Chn].mu32Profile);

}

bool RecordToMP4::stopRecordChn(int Chn)
{
    HI_S32 s32Ret;


    s32Ret = mPVenc[Chn]->SAMPLE_COMM_VENC_UnBindVpss(mVpss.m_Grp_Tab[Chn], m_VencBindVpss);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VENC_UnBindVpss failed with %#x!\n",s32Ret);
    }

    s32Ret = mPVenc[Chn]->SAMPLE_COMM_VENC_Stop();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VENC_Stop failed with %#x!\n",s32Ret);
    }
    mPFileTabFind[Chn]->close();

    delete mPFileTabFind[Chn];

    delete mPVenc[Chn];

    return true;

}

void RecordToMP4::run()
{
    int i = 0;
    int n;
    struct timeval TimeoutVal;
    fd_set read_fds;
    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    int s32Ret;
    unsigned char* pPackVirtAddr = nullptr;
    unsigned int u32PackLen = 0;
    bool isIDR = false;

    mRun = true;
    qDebug()<<"start record thread";

    while (mRun) {

        FD_ZERO(&read_fds);
        mFileMutex.lock();
        if(mVencParam.count() == 0){
            mFileMutex.unlock();
            break;
        }

        for(i = 0; i < mVencParam.count(); i++){
            if(!mVencParam[i].Mp4File.isOpen()){
                continue;
            }
            FD_SET(mVencParam[i].VencFd, &read_fds);
        }

        TimeoutVal.tv_sec  = 1;
        TimeoutVal.tv_usec = 0;
        s32Ret = select(mMaxFd + 1, &read_fds, nullptr, nullptr, &TimeoutVal);
        if (s32Ret < 0)
        {
            mFileMutex.unlock();
            SAMPLE_PRT("select failed!\n");
            break;
        }
        else if (s32Ret == 0)
        {
            SAMPLE_PRT("get venc stream time out, exit thread %d\n",mVencParam.count());
//            continue;
        }
        else
        {

            for (i = 0; i < mVencParam.count(); i++)
            {
                if (FD_ISSET(mVencParam[i].VencFd, &read_fds))
                {
                    /*******************************************************
                    step 2.1 : query how many packs in one-frame stream.
                    *******************************************************/
                    memset(&stStream, 0, sizeof(stStream));
                    s32Ret = HI_MPI_VENC_Query(mPVenc[mVencParam[i].ViChn]->m_Venc_Chn, &stStat);
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
                    s32Ret = HI_MPI_VENC_GetStream(mPVenc[mVencParam[i].ViChn]->m_Venc_Chn, &stStream, HI_TRUE);
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


//                    s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_SaveStream(m_enType, m_VencChnPara[i].pFile, &stStream);
//                    if (HI_SUCCESS != s32Ret)
//                    {
//                        free(stStream.pstPack);
//                        stStream.pstPack = nullptr;
//                        SAMPLE_PRT("save stream failed!\n");
//                        break;
//                    }
//                    if(stStream.stH264Info.enRefType == BASE_IDRSLICE){
//                        framesize = 0;
//                        for(uint n = 0; n < stStream.u32PackCount ;n++){
//                            framesize += stStream.pstPack[n].u32Len - stStream.pstPack[n].u32Offset;
//                        }
//                        m_VencChnPara[i].IInterval = stStream.pstPack->u64PTS/1000 - interval[m_VencChnPara[i].ViChn];
//                        interval[m_VencChnPara[i].ViChn] = m_VencChnPara[i].IInterval;
//                        framindex.size = framesize;
//                        framindex.frame = m_VencChnPara[i].frame;
////                        qDebug()<<"IFrame PTS(us):"<<stStream.pstPack->u64PTS;
//                        fwrite((void *)&framindex,sizeof (FRAME_INDEX),1,m_VencChnPara[i].pFile_index);
//                        fflush(m_VencChnPara[i].pFile_index);
//                        m_VencChnPara[i].frame++;
//                    }

                    //处理关键帧,关键帧有多个包
                    u32PackLen = 0;
                    if(stStream.u32PackCount == 1){
                        pPackVirtAddr = stStream.pstPack[0].pu8Addr + stStream.pstPack[0].u32Offset;
                        u32PackLen = stStream.pstPack[0].u32Len - stStream.pstPack[0].u32Offset;
//                        u32PackOffset = stStream.pstPack[0].u32Offset;
                        isIDR = Sample_Common_Venc::isIDRFrame(&stStream,PAYLOAD_TYPE);

                    }else {
                        for (n = 0 ; n < stStream.u32PackCount; n++)
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
                    mVencParam[i].Mp4File.writeFrame(pPackVirtAddr,u32PackLen,isIDR);


                    /*******************************************************
                     step 2.6 : release stream
                     *******************************************************/
                    s32Ret = HI_MPI_VENC_ReleaseStream(mPVenc[mVencParam[i].ViChn]->m_Venc_Chn, &stStream);
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
        mFileMutex.unlock();
        usleep(20000);

    }
    qDebug()<<"exit recordmp4 thread";


}
