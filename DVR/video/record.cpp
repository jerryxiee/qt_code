#include "record.h"
#include <QDateTime>
#include <QDir>
#include <QDebug>

SystemLog *Syslog = SystemLog::getSystemLog();

#define LOGW1(str) do{ Syslog->logwrite(1,QString(str));   \
    }while(0);

#define LOGWE(fmt,...) do{ Syslog->logwrite(1,fmt,__VA_ARGS__);   \
    }while(0);


Record::Record(QObject *parent) : QThread(parent)
{
    Init();
}

void Record::RecordExit()
{
    m_Venc_Run = HI_FALSE;
    wait();
}

void Record::Init()
{
    m_enType = PT_H264;
    m_enSize = PIC_HD720;

    QDir dir(ALARM_FILE_PATH);
    if(!dir.exists()){
        if(!dir.mkpath(ALARM_FILE_PATH)){
            SAMPLE_PRT("mkpath %s failed\n",ALARM_FILE_PATH);
        }
    }

    m_VencSet = Settings::getVencIni();

    for (int i = 0;i < m_VencSet->m_Vdec_Param[0].count();i++) {
        m_VencStatus.insert("channel"+QString::number(i),m_VencSet->m_Vdec_Param[0][i].mopen);
        m_VencSet->getFileInfo(i,&mfileindex[i],&mfileoffset[i]);
    }

    connect(m_VencSet,SIGNAL(vencAttrChanged(VI_CHN,HI_U32)),this,SLOT(onVencAttrChangedSlot(VI_CHN,HI_U32)));
    connect(m_VencSet,SIGNAL(vencStatusChanged(VI_CHN,bool)),this,SLOT(onVencStatusChanged(VI_CHN,bool)));

    connect(this,SIGNAL(createNewFileSignal(VI_CHN)),this,SLOT(onCreatNewFileSlot(VI_CHN)));

}

void Record::onTimeHander()
{
    checkFileSize();
}

HI_S32 Record::checkRecordChn(VI_CHN Chn)
{
    for(HI_S32 i = 0 ;i < m_VencChnPara.count();i++){
        if(m_VencChnPara[i].ViChn == Chn && m_VencStatus["channel"+QString::number(Chn)]){
            qDebug("Video(%d) active ",Chn);
            return i;
        }
    }

    return HI_FAILURE;
}
void Record::setRecordSrc(Sample_Common_Vpss *vpss)
{
    m_Vpss = vpss;
}

void Record::setRecordSrc(Sample_Common_Vpss &vpss)
{
    m_Vpss = vpss;
}

HI_S32 Record::startRecordChn(VI_CHN Chn,VIDEO_NORM_E enNorm)
{
    m_enNorm = enNorm;

    if(Chn > m_VencSet->m_Vdec_Param[0].count()){
        qDebug()<<"startRecordChn failed";
        return HI_FAILURE;
    }

    return startRecordChn(Chn, m_VencSet->m_Vdec_Param[0][Chn].mvencSize,m_VencSet->m_Vdec_Param[0][Chn].menRcMode,
            m_VencSet->m_Vdec_Param[0][Chn].mu32BitRate,m_VencSet->m_Vdec_Param[0][Chn].mfr32DstFrmRate,m_VencSet->m_Vdec_Param[0][Chn].mu32Profile);
}

HI_S32 Record::stopRecordChn(VI_CHN Chn)
{
    HI_S32 s32Ret;


    s32Ret = m_pVenc[Chn]->SAMPLE_COMM_VENC_UnBindVpss(m_Vpss.m_Grp_Tab[Chn], m_VencBindVpss);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VENC_UnBindVpss failed!\n");
    }

    s32Ret = m_pVenc[Chn]->SAMPLE_COMM_VENC_Stop();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VENC_Stop failed!\n");
    }

    delete m_pVenc[Chn];


    return s32Ret;

}

HI_S32 Record::startRecordChn(VI_CHN ViChnCnt, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32 u32BitRate, HI_FR32 frmRate, HI_U32 u32Profile)
{

    HI_S32 s32Ret;

    if(m_Vpss.m_Grp_Num == 0){
        SAMPLE_PRT("vpss not init!\n");
        return HI_FAILURE;
    }

//    qDebug()<<"start venc chn "<<ViChnCnt;
    m_pVenc[ViChnCnt] = new Sample_Common_Venc();
    m_pVenc[ViChnCnt]->SAMPLE_COMM_VENC_SetAttr(m_enType,m_enNorm, enSize, enRcMode,u32BitRate,frmRate,u32Profile);
    s32Ret = m_pVenc[ViChnCnt]->SAMPLE_COMM_VENC_Start();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_1;
    }

    s32Ret = m_pVenc[ViChnCnt]->SAMPLE_COMM_VENC_BindVpss(m_Vpss.m_Grp_Tab[ViChnCnt], m_VencBindVpss);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_2;
    }

    return s32Ret;
END_2:
    m_pVenc[ViChnCnt]->SAMPLE_COMM_VENC_UnBindVpss(m_Vpss.m_Grp_Tab[ViChnCnt], m_VencBindVpss);
END_1:
    m_pVenc[ViChnCnt]->SAMPLE_COMM_VENC_Stop();
    return s32Ret;
}

HI_S32 Record::setRecordAttr(VI_CHN ViChnCnt,PIC_SIZE_E enSize,SAMPLE_RC_E enRcMode,HI_U32 u32BitRate,HI_FR32 frmRate,HI_U32 u32Profile)
{

    VPSS_CHN_MODE_S stVpssMode;
    SIZE_S stPicSize;
    HI_S32 s32Ret;
    HI_BOOL deleteRecord = HI_TRUE;

//    Venc_Save_file_Stop(ViChnCnt);
    deleteRecord = deleteChnFromRecord(ViChnCnt);

    s32Ret = stopRecordChn(ViChnCnt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("GetChnMod failed!\n");
        goto END;
    }

    s32Ret = m_Vpss.SAMPLE_COMM_VPSS_GetChnMod(ViChnCnt,m_VencBindVpss,&stVpssMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("GetChnMod failed!\n");
        return HI_FAILURE;
    }

    s32Ret = Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(m_enNorm, enSize, &stPicSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Get picture size failed!\n");
        return HI_FAILURE;
    }
    //根据编码图像大小设置VPSS输出大小
    stVpssMode.u32Width = stPicSize.u32Width;
    stVpssMode.u32Height = stPicSize.u32Height;

    s32Ret = m_Vpss.SAMPLE_COMM_VPSS_SetChnMod(ViChnCnt,m_VencBindVpss,&stVpssMode,0);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SetChnMod failed!\n");
        return HI_FAILURE;
    }

    s32Ret = startRecordChn(ViChnCnt,enSize,enRcMode,u32BitRate,frmRate,u32Profile);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi_Venc_Start failed!\n");
        return HI_FAILURE;
    }

END:
    if(deleteRecord == HI_TRUE){
//        Venc_CreatNewFile(ViChnCnt);
        addChnToRecord(ViChnCnt);
    }

    return s32Ret;
}

void Record::setRecordStatus(VI_CHN ViChn,bool start)
{
    if(m_VencStatus.value("channel"+QString::number(ViChn)) !=  start){
        m_VencStatus["channel"+QString::number(ViChn)] = start;
        addChnToRecord(ViChn);

    }
}

void Record::onVencStatusChanged(VI_CHN Chn,bool start)
{
    qDebug("%s:%d\n",__FUNCTION__,__LINE__);
    setRecordStatus(Chn,start);
}

void Record::onViStatusChangedSlot(VI_CHN Chn,HI_BOOL status)
{
    qDebug("%s:%d\n",__FUNCTION__,__LINE__);
    if(status){
        addRecordList(Chn);
        addChnToRecord(Chn);
    }else {
        deleteChnFromRecord(Chn);
        deleteFromRecordList(Chn);
        removeVideoAlarmEventFromlist(Chn);
    }
}


void Record::onVencAttrChangedSlot(VI_CHN Chn,HI_U32 stream)
{
    qDebug("%s:%d",__FUNCTION__,__LINE__);
    if(stream == 0){
        setRecordAttr(Chn,m_VencSet->m_Vdec_Param[stream][Chn].mvencSize,m_VencSet->m_Vdec_Param[stream][Chn].menRcMode,
                      m_VencSet->m_Vdec_Param[stream][Chn].mu32BitRate,m_VencSet->m_Vdec_Param[stream][Chn].mfr32DstFrmRate,
                          m_VencSet->m_Vdec_Param[stream][Chn].mu32Profile);
    }

}

int Record::checkVideoAlarmList(VI_CHN Chn,VIDEO_TYPE type)
{
    for (int i = 0;i < m_VideoEventFileInfoList[Chn].count();i++) {
        if(m_VideoEventFileInfoList[Chn].at(i).type == type){
            return i;
        }
    }

    return -1;

}

bool Record::removeVideoAlarmEventFromlist(VI_CHN Chn)
{
    m_EventFileMutex.lock();
    m_VideoEventFileInfoList[Chn].clear();
    m_EventFileMutex.unlock();
    return true;
}

bool Record::removeVideoAlarmEventFromlist(VI_CHN Chn,VIDEO_TYPE type)
{
    int index;

    index = checkVideoAlarmList(Chn,type);
    if(index < 0){
        return false;
    }

    m_EventFileMutex.lock();
    m_VideoEventFileInfoList[Chn].removeAt(index);
    m_EventFileMutex.unlock();
    return true;


}

bool Record::addVideoAlarmEventFromlist(VI_CHN Chn,VIDEO_TYPE type)
{
    ALARM_FILE_INFO fileinfo;
    int index;

    index = checkVideoAlarmList(Chn,type);
    if(index < 0){
        fileinfo.type = type;
        m_EventFileMutex.lock();
        m_VideoEventFileInfoList[Chn].append(fileinfo);
        m_EventFileMutex.unlock();
        qDebug()<<"add alarm to list";

        return true;
    }

    return false;
}

void Record::onVideoAlarmEventChangedSlot(VI_CHN Chn,VIDEO_TYPE type,bool change)
{
    qDebug()<<"onVideoAlarmEventChangedSlot";
    if(m_VideoEventFileInfoList[Chn].count()){
        saveAlarmFile(Chn);
    }
    if(change){
        if(!addVideoAlarmEventFromlist(Chn,type)){
            return;
        }
        createAlarmFile(Chn);
    }else {

        if(!removeVideoAlarmEventFromlist(Chn,type)){
            return;
        }
    }

//    addChnToRecord(Chn);

}

HI_BOOL Record::addRecordList(VI_CHN Chn)
{
    Venc_Data VencD;
    HI_S32 index;

    index = checkRecordChn(Chn);
    if(index >= 0){
        qDebug()<<"alread in list video "<<Chn;
        return HI_FALSE;
    }

    VencD.ViChn   = Chn;
    VencD.pFile   = nullptr;
    VencD.pFile_index = nullptr;
    VencD.Venc_Chn = m_pVenc[Chn]->m_Venc_Chn;
    VencD.VencFd   = HI_MPI_VENC_GetFd(VencD.Venc_Chn);
    VencD.frame = 0;
    if (VencD.VencFd < 0)
    {
        SAMPLE_PRT("HI_MPI_VENC_GetFd failed with %#x!\n",VencD.VencFd);
        return HI_FALSE;
    }
    if(m_maxfd < VencD.VencFd)
        m_maxfd = VencD.VencFd;

    m_file_mutex.lock();
    m_VencChnPara.append(VencD);
    m_file_mutex.unlock();

    return HI_TRUE;


}

HI_BOOL Record::deleteFromRecordList(VI_CHN Chn)
{
    HI_S32 index;

    index = checkRecordChn(Chn);
    if(index < 0){
        qDebug()<<"Record list not video "<<Chn;
        return HI_FALSE;
    }
    m_file_mutex.lock();
    m_VencChnPara.removeAt(index);
    m_file_mutex.unlock();
    return HI_TRUE;

}


HI_BOOL Record::addChnToRecord(VI_CHN Chn)
{
    HI_BOOL result = HI_FALSE;

    if(!m_VencStatus.value("channel"+QString::number(Chn))){
        qDebug()<<"Chn "<<Chn<<"record not open";
        deleteChnFromRecord(Chn);
        return HI_FALSE;
    }

    result = createNewFile(Chn);
    if(result){
        start();
    }

    return result;
}

HI_BOOL Record::deleteChnFromRecord(VI_CHN Chn)
{
    HI_BOOL result = HI_TRUE;

//    saveAlarmFile(Chn);
    m_file_mutex.lock();
    result = onSaveFileStopSlot(Chn);
    m_file_mutex.unlock();

    return result;
}


HI_S32 Record::getAlarmFileName(VI_CHN Chn, VIDEO_TYPE type, char *filename, int len)
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
        return HI_FAILURE;
    }
    }
    if(len < strlen(file)){
        return HI_FAILURE;
    }
    strcpy(filename,file);

    return HI_SUCCESS;
}

HI_BOOL Record::createNewFile(VI_CHN Chn)
{
//    Venc_Data VencD;
    FILE *VencFile;
    HI_S32 index = HI_FAILURE;
    HI_S32 s32Ret;
    HI_CHAR szFilePostfix[10];
    HI_CHAR venc_path_name[VIDEO_FILENAME_SIZE];
    HI_CHAR venc_file_name[VIDEO_FILENAME_SIZE];

    index = checkRecordChn(Chn);
    if(index < 0){
        qDebug()<<"recordlist not video "<<Chn;
        return HI_FALSE;
    }

    if(mfileoffset[Chn] > MAXSIZE){
       mfileindex[Chn]++;
    }

    s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_GetFilePostfix(m_enType, szFilePostfix);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_COMM_VENC_GetFilePostfix failed with %#x!\n",s32Ret);
        return HI_FALSE;
    }



    sprintf(venc_path_name,"%s/channel%d/",VENC_PATH,Chn);
    QDir dir(venc_path_name);
    if(!dir.exists()){
        if(!dir.mkpath(venc_path_name)){
            SAMPLE_PRT("mkpath %s failed\n",venc_path_name);
            return HI_FALSE;
        }
    }

    sprintf(venc_file_name,"%s%d%s",venc_path_name,mfileindex[Chn],szFilePostfix);
    VencFile = fopen(venc_file_name, "ab");
    if (!VencFile)
    {
//        fclose(VencFile);
        SAMPLE_PRT("open file[%s] failed!\n",venc_file_name);
        return HI_FALSE;
    }

    LOGWE("open file[%s] sucess!\n",venc_file_name);

    m_file_mutex.lock();
    saveFileNode(index);

    if(m_VencChnPara[index].pFile){
        mfileoffset[Chn] = ftell(m_VencChnPara[index].pFile);
        fclose(m_VencChnPara[index].pFile);
        m_VencSet->setFileInfo(Chn,mfileindex[Chn],mfileoffset[Chn]);
    }

    m_VencChnPara[index].pFile = VencFile;
    m_VencChnPara[index].frame = 0;

    createFileNode(index);
    m_file_mutex.unlock();

    qDebug()<<"creat new file end";

    return HI_TRUE;
}

HI_BOOL Record::createFileNode(int index)
{
    HI_CHAR venc_fileindex_name[VIDEO_FILENAME_SIZE];
    FILE *VencFileIndex;
    MYVIDEO_HEAD videohead;
    SIZE_S size;

    if(!m_VencChnPara[index].pFile){
        qDebug()<<"can not record";
    }
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd-hh-mm-ss");
    QByteArray file_name = current_date.toLatin1();

    sprintf(venc_fileindex_name,"%s/channel%d/%s.index",VENC_PATH,m_VencChnPara[index].ViChn,file_name.data());
    VencFileIndex = fopen(venc_fileindex_name, "wb+");
    if (!VencFileIndex)
    {
//        fclose(VencFileIndex);
        SAMPLE_PRT("open file[%s] failed!\n",venc_fileindex_name);
        return HI_FALSE;
    }
    LOGWE("open file[%s] sucess!\n",venc_fileindex_name);

    Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(m_pVenc[m_VencChnPara[index].ViChn]->m_enNorm,m_enSize,&size);

    videohead.fileindex = mfileindex[m_VencChnPara[index].ViChn];
    videohead.ctime = current_date_time.toTime_t();
    videohead.enType = m_enType;
//    videohead.framerate = (m_pVenc[m_VencChnPara[index].ViChn]->m_enNorm == VIDEO_ENCODING_MODE_PAL)?25:30;
    videohead.framerate = m_pVenc[m_VencChnPara[index].ViChn]->m_DstFrmRate;
    videohead.mtime = 0;
    videohead.num = 0;
    videohead.width = size.u32Width;
    videohead.height = size.u32Height;
    videohead.stoffset = ftell(m_VencChnPara[index].pFile);
    videohead.endoffset = 0;
    videohead.IIntervalTime = 0;
    m_VencChnPara[index].pFile_index = VencFileIndex;

    fwrite((char *)&videohead,sizeof (MYVIDEO_HEAD),1,VencFileIndex);
    fflush(VencFileIndex);

    strcpy(curFileIndexName[m_VencChnPara[index].ViChn],venc_fileindex_name);

    createAlarmFile(m_VencChnPara[index].ViChn);

    VencFileIndex=nullptr;


    return HI_TRUE;


}

HI_BOOL Record::saveFileHead(int index)
{
    if(m_VencChnPara[index].pFile_index){
        MYVIDEO_HEAD videohead;
//            framhead.framenum = m_VencChnPara[index].frame;
        fseek(m_VencChnPara[index].pFile_index,0,SEEK_SET);
        fread((char *)&videohead,sizeof (MYVIDEO_HEAD),1,m_VencChnPara[index].pFile_index);

        videohead.mtime = QDateTime::currentDateTime().toTime_t();
        videohead.num = m_VencChnPara[index].frame;
        videohead.endoffset = ftell(m_VencChnPara[index].pFile);
        videohead.IIntervalTime = m_VencChnPara[index].IInterval;
        fseek(m_VencChnPara[index].pFile_index,0,SEEK_SET);
        fwrite((char *)&videohead,sizeof (MYVIDEO_HEAD),1,m_VencChnPara[index].pFile_index);

        fseek(m_VencChnPara[index].pFile_index,0,SEEK_END);

        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_BOOL Record::saveFileNode(int index)
{
    if(saveFileHead(index) == HI_TRUE){
        fclose(m_VencChnPara.at(index).pFile_index);
        saveAlarmFile(m_VencChnPara[index].ViChn);
    }

    m_VencChnPara[index].pFile_index = nullptr;
    m_VencChnPara[index].frame = 0;

}

HI_S32 Record::createAlarmFile(VI_CHN Chn)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_CHAR alarm_path[VIDEO_FILENAME_SIZE];

    HI_S32 index = HI_FAILURE;

    index = checkRecordChn(Chn);

    if(!m_VencChnPara[index].pFile_index){
        qDebug()<<"not record can not create alarm file";
        return HI_FAILURE;
    }
    m_EventFileMutex.lock();
    if(index >= 0){
        for (int i = 0;i < m_VideoEventFileInfoList[Chn].count();i++) {
            s32Ret = getAlarmFileName(Chn,m_VideoEventFileInfoList[Chn].at(i).type, alarm_path,VIDEO_FILENAME_SIZE);
            if(s32Ret != HI_SUCCESS){
                qDebug()<<"getAlarmFileName failed";
                m_VideoEventFileInfoList[Chn].removeAt(i);
                continue;
            }
            strcpy(m_VideoEventFileInfoList[Chn][i].filename,curFileIndexName[Chn]);
            m_VideoEventFileInfoList[Chn][i].ctime = QDateTime::currentDateTime().toTime_t();
            m_VideoEventFileInfoList[Chn][i].mtime = 0;
            m_VideoEventFileInfoList[Chn][i].stframe = m_VencChnPara[index].frame > 2?m_VencChnPara[index].frame-2:0;
            m_VideoEventFileInfoList[Chn][i].endframe = 0;
            qDebug("create alarm file[%s]",alarm_path);

            QFileInfo alarmFile(alarm_path);
            if(!alarmFile.exists()){
                ALARM_VIDEO_HEAD videoFileHead;
                videoFileHead.type = m_VideoEventFileInfoList[Chn].at(i).type;
                videoFileHead.cTime = QDateTime::currentDateTime().toTime_t();
                FILE *file = fopen(alarm_path,"wb");
                fwrite((char *)&videoFileHead,sizeof (ALARM_VIDEO_HEAD),1,file);
                fclose(file);

            }
        }
    }

    m_EventFileMutex.unlock();


    return s32Ret;
}

HI_S32 Record::saveAlarmFile(VI_CHN Chn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR alarm_path[VIDEO_FILENAME_SIZE];
    int index = -1;

    index = checkRecordChn(Chn);
    if(index < 0){
        qDebug("video %d not active",Chn);
        return HI_FAILURE;
    }

    m_EventFileMutex.lock();
    for(int i = 0;i < m_VideoEventFileInfoList[Chn].count();i++) {
        s32Ret = getAlarmFileName(Chn,m_VideoEventFileInfoList[Chn].at(i).type, alarm_path,VIDEO_FILENAME_SIZE);
        if(s32Ret != HI_SUCCESS){
            qDebug()<<"getAlarmFileName failed";
            continue;
        }
        qDebug("write file[%s]",alarm_path);
        FILE *alarmFile = fopen(alarm_path,"rb+");
        if(alarmFile){
            ALARM_VIDEO_HEAD videoFileHead;
            fread((char *)&videoFileHead,sizeof (ALARM_VIDEO_HEAD),1,alarmFile);
            videoFileHead.mtime = QDateTime::currentDateTime().toTime_t();
            videoFileHead.num++;

            fseek(alarmFile,0,SEEK_END);
            m_VideoEventFileInfoList[Chn][i].mtime = videoFileHead.mtime;
            m_VideoEventFileInfoList[Chn][i].frame = m_VencChnPara[index].frame - m_VideoEventFileInfoList[Chn][i].stframe;
            m_VideoEventFileInfoList[Chn][i].endframe = m_VencChnPara[index].frame > 0?m_VencChnPara[index].frame-1:0;
            fwrite((char *)(&m_VideoEventFileInfoList[Chn].at(i)),sizeof (ALARM_FILE_INFO),1,alarmFile);

            fseek(alarmFile,0,SEEK_SET);
            fwrite((char *)&videoFileHead,sizeof (ALARM_VIDEO_HEAD),1,alarmFile);

            fclose(alarmFile);

            qDebug("write file[%s] sucess",alarm_path);
        }
    }
    m_EventFileMutex.unlock();

    return s32Ret;
}

HI_BOOL Record::onCreatNewFileSlot(VI_CHN Chn)
{
//    saveAlarmFile(Chn);
    return createNewFile(Chn);
}

HI_BOOL Record::onSaveFileStopSlot(VI_CHN Chn)
{

    HI_S32 index = HI_FAILURE;

    index = checkRecordChn(Chn);
    if(index >= 0){
        if(m_VencChnPara[index].pFile){
            saveFileNode(index);
            mfileoffset[Chn] = ftell(m_VencChnPara[index].pFile);
            fclose(m_VencChnPara[index].pFile);
            m_VencSet->setFileInfo(Chn,mfileindex[Chn],mfileoffset[Chn]);
        }

        m_VencChnPara[index].pFile = nullptr;
        qDebug("stop channel(%d) save file ",Chn);
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 Record::getFileSize(VI_CHN Chn)
{
    for(int i = 0; i < m_VencChnPara.count();i++){
        if(m_VencChnPara[i].ViChn == Chn && m_VencChnPara[i].pFile)
            return ftell(m_VencChnPara[i].pFile);
    }

    return 0;
}

void Record::checkFileSize()
{
    for(int i = 0;i < m_VencChnPara.count();i++){
        if(m_VencChnPara[i].pFile && ftell(m_VencChnPara[i].pFile) > MAXSIZE){
            qDebug()<<"channel"<<m_VencChnPara[i].Venc_Chn<<"make new file";
            emit createNewFileSignal(m_VencChnPara[i].ViChn);
        }
        m_file_mutex.lock();
        saveFileHead(i);
        m_file_mutex.unlock();
    }
    LOGW1("check file size!");

}

void Record::run()
{
    HI_S32 i = 0;
    struct timeval TimeoutVal;
    fd_set read_fds;
    HI_U32 interval[VIDEO_MAX_NUM]={0};

    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    HI_S32 s32Ret;
    FRAME_INDEX framindex;
    HI_U32 framesize = 0;

//    FILE *frameinfo = fopen("saveinfo","wb");
    printf("start venc count:%d\n",m_VencChnPara.count());

    m_Venc_Run = HI_TRUE;
    /******************************************
     step 2:  Start to get streams of each channel.
    ******************************************/
    while (m_Venc_Run)
    {
        FD_ZERO(&read_fds);
        m_file_mutex.lock();
        if(m_VencChnPara.count() == 0){
            m_file_mutex.unlock();
            break;
        }

        for(i = 0; i < m_VencChnPara.count(); i++){
            if(!m_VencChnPara[i].pFile|| !m_VencStatus["channel"+QString::number(m_VencChnPara[i].ViChn)]){
                continue;
            }
            FD_SET(m_VencChnPara[i].VencFd, &read_fds);
        }
        TimeoutVal.tv_sec  = 1;
        TimeoutVal.tv_usec = 0;
        s32Ret = select(m_maxfd + 1, &read_fds, nullptr, nullptr, &TimeoutVal);
        if (s32Ret < 0)
        {
            m_file_mutex.unlock();
            SAMPLE_PRT("select failed!\n");
            break;
        }
        else if (s32Ret == 0)
        {
            SAMPLE_PRT("get venc stream time out, exit thread %d\n",m_VencChnPara.count());
//            continue;
        }
        else
        {

            for (i = 0; i < m_VencChnPara.count(); i++)
            {
                if (FD_ISSET(m_VencChnPara[i].VencFd, &read_fds))
                {
                    /*******************************************************
                    step 2.1 : query how many packs in one-frame stream.
                    *******************************************************/
                    memset(&stStream, 0, sizeof(stStream));
                    s32Ret = HI_MPI_VENC_Query(m_VencChnPara[i].Venc_Chn, &stStat);
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
                    s32Ret = HI_MPI_VENC_GetStream(m_VencChnPara[i].Venc_Chn, &stStream, HI_TRUE);
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

                    framindex.offset = ftell(m_VencChnPara[i].pFile);
//                    fprintf(frameinfo,"frame:%#x offset:%#x len:%#x\n",m_VencChnPara[i].frame,framindex.offset,framindex.size);


                    s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_SaveStream(m_enType, m_VencChnPara[i].pFile, &stStream);
                    if (HI_SUCCESS != s32Ret)
                    {
                        free(stStream.pstPack);
                        stStream.pstPack = nullptr;
                        SAMPLE_PRT("save stream failed!\n");
                        break;
                    }
                    if(stStream.stH264Info.enRefType == BASE_IDRSLICE){
                        framesize = 0;
                        for(uint n = 0; n < stStream.u32PackCount ;n++){
                            framesize += stStream.pstPack[n].u32Len - stStream.pstPack[n].u32Offset;
                        }
                        m_VencChnPara[i].IInterval = stStream.pstPack->u64PTS/1000 - interval[m_VencChnPara[i].ViChn];
                        interval[m_VencChnPara[i].ViChn] = m_VencChnPara[i].IInterval;
                        framindex.size = framesize;
                        framindex.frame = m_VencChnPara[i].frame;
//                        qDebug()<<"IFrame PTS(us):"<<stStream.pstPack->u64PTS;
                        fwrite((void *)&framindex,sizeof (FRAME_INDEX),1,m_VencChnPara[i].pFile_index);
                        fflush(m_VencChnPara[i].pFile_index);
                        m_VencChnPara[i].frame++;
                    }



                    /*******************************************************
                     step 2.6 : release stream
                     *******************************************************/
                    s32Ret = HI_MPI_VENC_ReleaseStream(m_VencChnPara[i].Venc_Chn, &stStream);
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
        m_file_mutex.unlock();
        usleep(20000);
    }
    /*******************************************************
     step 3 : close save-file
    *******************************************************/

    for (int i = 0;i < m_VencChnPara.count();i++) {
        deleteChnFromRecord(m_VencChnPara[i].ViChn);
    }

    qDebug()<<"exit venc thread";
//    fclose(frameinfo);
    return;

}
