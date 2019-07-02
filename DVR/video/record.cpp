#include "record.h"
#include <QDateTime>
#include <QDir>
#include <QDebug>

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

    connect(this,SIGNAL(createNewFile(VENC_CHN)),this,SLOT(onCreatNewFileSlot(VENC_CHN)));

}

HI_BOOL Record::addChnToRecord(VENC_CHN VencChn)
{
    HI_BOOL result = HI_FALSE;

    result = onCreatNewFileSlot(VencChn);
    if(result){
        start();
    }

    return result;
}

HI_BOOL Record::deleteChnFromRecord(VENC_CHN VencChn)
{
    HI_BOOL result = HI_TRUE;

    m_file_mutex.lock();
    result = onSaveFileStopSlot(VencChn);
    m_file_mutex.unlock();

    return result;
}


HI_BOOL Record::onCreatNewFileSlot(VENC_CHN VencChn)
{
    Venc_Data VencD;
    HI_S32 s32Ret;
    HI_CHAR szFilePostfix[10];
    HI_CHAR venc_path_name[64];
    HI_CHAR venc_file_name[64];

    VencD.Venc_Chn = VencChn;
    VencD.VencFd   = HI_MPI_VENC_GetFd(VencChn);
    VencD.framnum  = 0;
    if (VencD.VencFd < 0)
    {
        SAMPLE_PRT("HI_MPI_VENC_GetFd failed with %#x!\n",VencD.VencFd);
        return HI_FALSE;
    }
    if(m_maxfd < VencD.VencFd)
        m_maxfd = VencD.VencFd;

    s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_GetFilePostfix(m_enType, szFilePostfix);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_COMM_VENC_GetFilePostfix failed with %#x!\n",s32Ret);
        return HI_FALSE;
    }

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd-hh-mm-ss");

    sprintf(venc_path_name,"%s/channel%d/",VENC_PATH,VencChn);
    QDir dir(venc_path_name);
    if(!dir.exists()){
        if(!dir.mkpath(venc_path_name)){
            SAMPLE_PRT("mkpath %s failed\n",venc_path_name);
            return HI_FALSE;
        }
    }

    QByteArray file_name = current_date.toLatin1();
    sprintf(venc_file_name,"%s%s%s",venc_path_name,file_name.data(),szFilePostfix);
    VencD.pFile = fopen(venc_file_name, "wb");
    if (!VencD.pFile)
    {
        fclose(VencD.pFile);
        SAMPLE_PRT("open file[%s] failed!\n",venc_file_name);
        return HI_FALSE;
    }
    if(fseek(VencD.pFile,sizeof (MYVIDEO_HEAD),SEEK_SET)){
        fclose(VencD.pFile);
        return HI_FALSE;
    }
    SAMPLE_PRT("open file[%s] sucess!\n",venc_file_name);

    m_file_mutex.lock();

//    for(HI_S32 i = 0 ;i < m_VencChnPara.count();i++){
//        if(m_VencChnPara[i].Venc_Chn == VencD.Venc_Chn){
//            fclose(m_VencChnPara[i].pFile);
//            m_VencChnPara.removeAt(i);
//            qDebug("channel %d begin new file",VencChn);
//        }
//    }
    onSaveFileStopSlot(VencChn);
    m_VencChnPara.append(VencD);

    m_file_mutex.unlock();


    return HI_TRUE;
}
HI_BOOL Record::onSaveFileStopSlot(VENC_CHN VencChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MYVIDEO_HEAD videohead;
//    VENC_FRAME_RATE_S frameRate;
    VENC_CHN_ATTR_S vencAttr;

    s32Ret = HI_MPI_VENC_GetChnAttr(VencChn, &vencAttr);
    if(s32Ret != HI_SUCCESS){
        qDebug("HI_MPI_VENC_GetChnAttr failed with %#x!",s32Ret);
    }else {
        videohead.payload_type =  vencAttr.stVeAttr.enType;
        switch (vencAttr.stVeAttr.enType) {
            case PT_H264:
            {
                videohead.width  = vencAttr.stVeAttr.stAttrH264e.u32PicWidth;
                videohead.height = vencAttr.stVeAttr.stAttrH264e.u32PicHeight;
                if(vencAttr.stRcAttr.enRcMode == VENC_RC_MODE_H264CBR){
                    videohead.framerate = vencAttr.stRcAttr.stAttrH264Cbr.fr32DstFrmRate;
                }else if(vencAttr.stRcAttr.enRcMode == VENC_RC_MODE_H264VBR) {
                    videohead.framerate = vencAttr.stRcAttr.stAttrH264Vbr.fr32DstFrmRate;
                }else if(vencAttr.stRcAttr.enRcMode == VENC_RC_MODE_H264FIXQP){
                    videohead.framerate = vencAttr.stRcAttr.stAttrH264FixQp.fr32DstFrmRate;
                }
                break;
            }
            case PT_H265:
            {
                videohead.width  = vencAttr.stVeAttr.stAttrH265e.u32PicWidth;
                videohead.height = vencAttr.stVeAttr.stAttrH265e.u32PicHeight;
                if(vencAttr.stRcAttr.enRcMode == VENC_RC_MODE_H265CBR){
                    videohead.framerate = vencAttr.stRcAttr.stAttrH265Cbr.fr32DstFrmRate;
                }else if(vencAttr.stRcAttr.enRcMode == VENC_RC_MODE_H265VBR) {
                    videohead.framerate = vencAttr.stRcAttr.stAttrH265Vbr.fr32DstFrmRate;
                }else if(vencAttr.stRcAttr.enRcMode == VENC_RC_MODE_H265FIXQP){
                    videohead.framerate = vencAttr.stRcAttr.stAttrH265FixQp.fr32DstFrmRate;
                }
                break;
            }
        }

    }


//    s32Ret = HI_MPI_VENC_GetFrameRate(VencChn, &frameRate);
//    if(s32Ret != HI_SUCCESS){
//        qDebug("HI_MPI_VENC_GetFrameRate failed with %#x!",s32Ret);
//    }else {
//        videohead.framerate = frameRate.s32DstFrmRate;
//    }


    for(HI_S32 i = 0 ;i < m_VencChnPara.count();i++){
        if(m_VencChnPara[i].Venc_Chn == VencChn){
            videohead.framnum = m_VencChnPara[i].framnum;
            fseek(m_VencChnPara[i].pFile,0,SEEK_SET);
            fwrite((void *)&videohead,sizeof (videohead),1,m_VencChnPara[i].pFile);

            fclose(m_VencChnPara[i].pFile);
            m_VencChnPara.removeAt(i);
            qDebug("stop channel(%d) save file ",VencChn);
            break;
        }
    }

    return HI_TRUE;
}

HI_S32 Record::getFileSize(VENC_CHN VencChn)
{
    for(int i = 0; i < m_VencChnPara.count();i++){
        if(m_VencChnPara[i].Venc_Chn == VencChn)
            return ftell(m_VencChnPara[i].pFile);
    }

    return 0;
}

void Record::checkFileSize()
{
    for(int i = 0;i < m_VencChnPara.count();i++){
        if(ftell(m_VencChnPara[i].pFile) > MAXSIZE){
            qDebug()<<"channel"<<m_VencChnPara[i].Venc_Chn<<"make new file";
            emit createNewFile(m_VencChnPara[i].Venc_Chn);
        }
    }

}

void Record::run()
{
    HI_S32 i = 0;
    HI_S32 j = 0;
    HI_S32 framesize = 0;
    HI_S32 count;
    struct timeval TimeoutVal;
    fd_set read_fds;

    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    HI_S32 s32Ret;


    printf("start venc count:%d\n",m_VencChnPara.count());

    m_Venc_Run = HI_TRUE;
    /******************************************
     step 2:  Start to get streams of each channel.
    ******************************************/
    while (m_Venc_Run)
    {
        FD_ZERO(&read_fds);
//        for (i = 0; i < m_ViChnCnt; i++)
//        {
//            FD_SET(VencFd[i], &read_fds);
//        }
        m_file_mutex.lock();
        if(m_VencChnPara.count() == 0){
            m_file_mutex.unlock();
            break;
        }


        for(i = 0; i < m_VencChnPara.count(); i++){
            FD_SET(m_VencChnPara[i].VencFd, &read_fds);
        }
        TimeoutVal.tv_sec  = 0;
        TimeoutVal.tv_usec = 200000;
        s32Ret = select(m_maxfd + 1, &read_fds, nullptr, nullptr, &TimeoutVal);
        if (s32Ret < 0)
        {
            m_file_mutex.unlock();
            SAMPLE_PRT("select failed!\n");
            break;
        }
        else if (s32Ret == 0)
        {
            SAMPLE_PRT("get venc stream time out, exit thread\n");
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
                    m_VencChnPara[i].framnum ++;
                    /*******************************************************
                     step 2.5 : save frame to file
                    *******************************************************/
                    framesize = 0;
                    for(j = 0;j < stStream.u32PackCount;j++){
                        framesize += stStream.pstPack[j].u32Len - stStream.pstPack[j].u32Offset;
                    }
                    fwrite((void *)&framesize,sizeof (framesize),1,m_VencChnPara[i].pFile);

                    s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_SaveStream(m_enType, m_VencChnPara[i].pFile, &stStream);
                    if (HI_SUCCESS != s32Ret)
                    {
                        free(stStream.pstPack);
                        stStream.pstPack = nullptr;
                        SAMPLE_PRT("save stream failed!\n");
                        break;
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
    qDebug()<<"exit venc thread";
    /*******************************************************
     step 3 : close save-file
    *******************************************************/

    m_file_mutex.lock();
    while (m_VencChnPara.count()) {
        onSaveFileStopSlot(m_VencChnPara[0].Venc_Chn);
    }
    m_file_mutex.unlock();
    return;

}
