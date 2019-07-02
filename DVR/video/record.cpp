#include "record.h"
#include <QDateTime>
#include <QDir>
#include <QDebug>

Record::Record(QObject *parent) : QThread(parent)
{
    Init();
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

    for(HI_S32 i = 0 ;i < m_VencChnPara.count();i++){
        if(m_VencChnPara[i].Venc_Chn == VencChn){
            fclose(m_VencChnPara[i].pFile);
            m_VencChnPara.removeAt(i);
            qDebug("stop channel(%d) save file ",VencChn);
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
//    HI_S32 count;
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
                    /*******************************************************
                     step 2.5 : save frame to file
                    *******************************************************/

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
//    count = m_VencChnPara.count();
//    m_file_mutex.lock();
//    for (i = 0; i < count; i++)
//    {
//        m_pVenc[m_VencChnPara[i].Vi_Chn]->SAMPLE_COMM_VENC_Stop();
//        m_pVenc[m_VencChnPara[i].Vi_Chn]->SAMPLE_COMM_VENC_UnBindVpss(m_Vpss.m_Grp_Tab[m_VencChnPara[i].Vi_Chn], m_VencBindVpss);
//        delete m_pVenc[m_VencChnPara[i].Vi_Chn];
//        fclose(m_VencChnPara[i].pFile);
////        m_VencChnPara.removeAt(i);
////        qDebug()<<"stop venc"<<i<<"/"<<m_VencChnPara.count();

//    }
//    m_VencChnPara.clear();
//    m_file_mutex.unlock();
    return;

}
