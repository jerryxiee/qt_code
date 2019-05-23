#include "vio.h"
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QByteArray>


Vio::Vio(QObject *parent):QThread(parent),
    m_ViChnCnt(8),m_enVoMode(VO_MODE_9MUX)
{
    m_enType = PT_H265;
    m_enSize = PIC_HD720;
    m_enRcMode = SAMPLE_RC_CBR;
    m_u32Profile = 0;
    m_pFile.resize(m_ViChnCnt);
    m_timer = nullptr;
    m_pVpss = nullptr;
    m_maxfd = 0;
//    m_ViDetect = new QFileSystemWatcher(this);
//    m_ViDetect->addPath(QString::fromLatin1(VI_STATUS_FILE));
//    connect(this,SIGNAL(VistatusChanged(VI_CHN)),this,SLOT(onChangeStatus(VI_CHN)));
    for(int i; i < m_ViChnCnt;i++){
        m_ViStatus.insert("channel"+QString::number(i),false);
    }
    m_ViStatusChanged = HI_FALSE;

    qDebug()<<"vio start";

}
Vio::Vio(VI_CHN ViChnCnt, SAMPLE_VO_MODE_E enVoMode):m_ViChnCnt(ViChnCnt),m_enVoMode(enVoMode)
{
    m_enType = PT_H265;
    m_enSize = PIC_HD720;
    m_enRcMode = SAMPLE_RC_CBR;
    m_u32Profile = 0;
    m_pFile.resize(m_ViChnCnt);
    m_timer = nullptr;
    m_pVpss = nullptr;
    m_maxfd = 0;
//    m_ViDetect = new QFileSystemWatcher(this);
//    m_ViDetect->addPath(QString::fromLatin1(VI_STATUS_FILE));
//    connect(this,SIGNAL(VistatusChanged(VI_CHN)),this,SLOT(onChangeStatus(VI_CHN)));
    for(int i; i < m_ViChnCnt;i++){
        m_ViStatus.insert("channel"+QString::number(i),false);
    }
    m_ViStatusChanged = HI_FALSE;


}

void Vio::start_timer()
{
    m_timer = new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimeHander()));
    m_timer->start(TIMEOUT);
}
Vio::~Vio()
{
    HI_S32 i;

    if(m_timer){
        m_timer->stop();
        delete m_timer;
    }
//    qDebug()<<"delete timer";

    m_Vio.SAMPLE_COMM_VI_UnBindVpss(enViMode,m_pVpss->m_Grp_Tab,m_ViChnCnt);
    for(i=0;i<m_ViChnCnt;i++){
        m_Vio.SAMPLE_COMM_VO_UnBindVpss(i,m_pVpss->m_Grp_Tab[i],m_VoBindVpss);
//        m_pVenc[i]->SAMPLE_COMM_VENC_Stop();
//        m_pVenc[i]->SAMPLE_COMM_VENC_UnBindVpss(m_pVpss->m_Grp_Tab[i], m_VencBindVpss);
    }

    m_pVpss->SAMPLE_COMM_VPSS_Stop();
    delete m_pVpss;
    m_Vio.SAMPLE_COMM_VO_StopChn();
    m_Vio.SAMPLE_COMM_VO_StopLayer();
    m_Vio.SAMPLE_COMM_VI_Stop(enViMode);
//    delete m_ViDetect;

    qDebug()<<"exit Vio";
}


HI_BOOL Vio::make_file()
{
    HI_S32 i;
    HI_S32 s32Ret;
    HI_CHAR szFilePostfix[10];
    HI_CHAR venc_path_name[64];
    HI_CHAR venc_file_name[64];

    qDebug()<<"create file";
    s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_GetFilePostfix(m_enType, szFilePostfix);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_COMM_VENC_GetFilePostfix failed with %#x!\n",s32Ret);
        return HI_FALSE;
    }

    for(i = 0; i < m_ViChnCnt; i++){
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy-MM-dd-hh-mm-ss");

        sprintf(venc_path_name,"%s/channel%d/",VENC_PATH,i);
        QDir dir(venc_path_name);
        if(!dir.exists()){
            if(!dir.mkpath(venc_path_name)){
                SAMPLE_PRT("mkpath %s failed\n",venc_path_name);
                return HI_FALSE;
            }
        }

        m_file_mutex.lock();
        if(m_pFile[i]){
            qDebug("close file mpFile[%d]\n",i);
            fclose(m_pFile[i]);
        }
        QByteArray file_name = current_date.toLatin1();
        sprintf(venc_file_name,"%s/%s%s",venc_path_name,file_name.data(),szFilePostfix);
        m_pFile[i] = fopen(venc_file_name, "wb");
        if (!m_pFile[i])
        {
            fclose(m_pFile[i]);
            m_file_mutex.unlock();
            SAMPLE_PRT("open file[%s] failed!\n",venc_file_name);
            return HI_FALSE;
        }
        SAMPLE_PRT("open file[%s] sucess!\n",venc_file_name);
        m_file_mutex.unlock();

    }
    qDebug()<<"create file sucess"<<endl;

    return HI_TRUE;
}

HI_BOOL Vio::Venc_CreatNewFile(VI_CHN ViChn)
{
    Venc_Data VencD;
    HI_S32 s32Ret;
    HI_CHAR szFilePostfix[10];
    HI_CHAR venc_path_name[64];
    HI_CHAR venc_file_name[64];

    VencD.Vi_Chn   = ViChn;
    VencD.Venc_Chn = m_pVenc[ViChn]->m_Venc_Chn;
    VencD.VencFd   = HI_MPI_VENC_GetFd(VencD.Venc_Chn);
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

    sprintf(venc_path_name,"%s/channel%d/",VENC_PATH,ViChn);
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

    for(HI_S32 i = 0 ;i < m_VencChnPara.count();i++){
        if(m_VencChnPara[i].Venc_Chn == VencD.Venc_Chn){
            fclose(m_VencChnPara[i].pFile);
            m_VencChnPara.removeAt(i);
            qDebug("channel %d begin new file",ViChn);
        }
    }
    m_VencChnPara.append(VencD);

    m_file_mutex.unlock();


    return HI_TRUE;
}
HI_BOOL Vio::Venc_Save_file_Stop(VI_CHN ViChn)
{

    m_file_mutex.lock();

    for(HI_S32 i = 0 ;i < m_VencChnPara.count();i++){
        if(m_VencChnPara[i].Vi_Chn == ViChn){
            fclose(m_VencChnPara[i].pFile);
            m_VencChnPara.removeAt(i);
            qDebug("stop channel(%d) save file ",ViChn);
        }
    }
    m_file_mutex.unlock();
}

void Vio::onTimeHander()
{
    HI_S32 s32Ret;
    HI_S32 i;
    VI_CHN_LUM_S stLuma;
    bool status;

    for(i = 0; i < m_ViChnCnt;i++){
        s32Ret = m_Vio.SAMPLE_COMM_VI_GetChnLuma(enViMode,i, &stLuma);
        if (HI_SUCCESS != s32Ret){
            SAMPLE_PRT("HI_MPI_VI_GetChnLuma(%d) failed with %#x!\n", i,s32Ret);
        }
        status = m_ViStatus.value("channel"+QString::number(i));
        if(LUMCONST < stLuma.u32Lum){
            m_ViStatus["channel"+QString::number(i)] = true;
        }else{
            m_ViStatus["channel"+QString::number(i)] = false;
        }
        if(status != m_ViStatus.value("channel"+QString::number(i))){
            qDebug()<<"vi"<<i<<"status changed";
            emit VistatusChanged(i);
//            m_ViStatusChanged = HI_TRUE;
        }


//        qDebug("vi(%d)Luma =%d",i*4,stLuma.u32Lum);
    }
    for(i = 0;i < m_VencChnPara.count();i++){
        if(ftell(m_VencChnPara[i].pFile) > MAXSIZE){
            qDebug()<<"channel"<<m_VencChnPara[i].Vi_Chn<<"make new file";
            emit MakeNewFile(m_VencChnPara[i].Vi_Chn);

//        qDebug()<<ftell(m_VencChnPara[i].pFile)/1024/1024<<"MB";
        }
    }


//    if(m_ViStatusChanged)
//    {
//        m_ViStatusChanged = HI_FALSE;
//        QFile file(VI_STATUS_FILE);
//        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
//        {
//            qDebug() << "Open failed." << endl;
//            return ;
//        }
//        QTextStream txtOutput(&file);

//        for(i = 0;i < m_ViChnCnt;i++){
//            txtOutput<<"channel"<<i<<"="<<m_ViStatus.value("channel"+QString::number(i))<<endl;
//        }
//        file.close();


//    }


//    make_file();
}

void Vio::onChangeStatus(VI_CHN ViChn)
{
    qDebug()<<"video "<<ViChn<<"changed";
    if(m_ViStatus.value("channel"+QString::number(ViChn)) == true){
        Venc_CreatNewFile(ViChn);
    }else{
        Venc_Save_file_Stop(ViChn);
    }

}
void Vio::onMakeNewFile(VI_CHN ViChn)
{
    Venc_CreatNewFile(ViChn);
}


HI_BOOL Vio::Vo_Start()
{
    HI_S32 s32Ret = HI_SUCCESS;
//    HI_U32 u32WndNum;
    HI_S32 i;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    memset(&(stLayerAttr), 0 , sizeof(VO_VIDEO_LAYER_ATTR_S));
    s32Ret = m_Vio.SAMPLE_COMM_VO_GetWH(Sample_Common_Sys::m_stPubAttr.enIntfSync, \
        &stLayerAttr.stImageSize.u32Width, \
        &stLayerAttr.stImageSize.u32Height, \
        &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_GetWH failed!\n");
        return HI_FALSE;
    }

    stLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    stLayerAttr.stDispRect.u32Width   = stLayerAttr.stImageSize.u32Width;
    stLayerAttr.stDispRect.u32Height  = stLayerAttr.stImageSize.u32Height;
    s32Ret = m_Vio.SAMPLE_COMM_VO_StartLayer(&stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartLayer failed!\n");
        goto END_1;
    }

    s32Ret = m_Vio.SAMPLE_COMM_VO_StartChn(m_enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_2;
    }

    for(i=0;i<m_ViChnCnt;i++)
    {
        s32Ret = m_Vio.SAMPLE_COMM_VO_BindVpss(i,m_pVpss->m_Grp_Tab[i],m_VoBindVpss);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_3;
        }
    }


    return HI_TRUE;

END_3:
    for(i=0;i<m_ViChnCnt;i++){
        m_Vio.SAMPLE_COMM_VO_UnBindVpss(i,m_pVpss->m_Grp_Tab[i],m_VoBindVpss);
    }
END_2:
    m_Vio.SAMPLE_COMM_VO_StopChn();
END_1:
    m_Vio.SAMPLE_COMM_VO_StopLayer();

    return HI_FALSE;
}

HI_BOOL Vio::Vo_Start(VO_CHN Vo_Chn, RECT_S &pos)
{

    if(m_Vio.SAMPLE_COMM_VO_StartChn(Vo_Chn,pos) < 0){
        return HI_FALSE;
    }
    return (m_Vio.SAMPLE_COMM_VO_BindVpss(Vo_Chn,m_pVpss->m_Grp_Tab[Vo_Chn],m_VoBindVpss)) < 0 ?HI_FALSE:HI_TRUE;
}

HI_BOOL Vio::Vo_Stop(VO_CHN Vo_Chn)
{
    HI_S32 i;

    if(Vo_Chn >= 0){
        m_Vio.SAMPLE_COMM_VO_UnBindVpss(Vo_Chn,m_pVpss->m_Grp_Tab[Vo_Chn],m_VoBindVpss);
        return m_Vio.SAMPLE_COMM_VO_StopChn(Vo_Chn) > 0 ? HI_TRUE:HI_TRUE;
    }
    for(i = 0; i < m_ViChnCnt; i++){
        m_Vio.SAMPLE_COMM_VO_UnBindVpss(i,m_pVpss->m_Grp_Tab[i],m_VoBindVpss);
    }

    m_Vio.SAMPLE_COMM_VO_StopChn();

//    m_Vio.SAMPLE_COMM_VO_StopLayer();

    return HI_TRUE;
}
HI_S32 Vio::Vo_SetMode(SAMPLE_VO_MODE_E enVoMode)
{
    return m_Vio.SAMPLE_COMM_VO_SetMode(enVoMode);
}

void Vio::onStopVoSlot()
{
    Vo_Stop(-1);
    qDebug()<<"stop vo";
}

HI_BOOL Vio::Vi_Start()
{
    HI_S32 s32Ret = HI_SUCCESS;
    SIZE_S stSize;
    VPSS_GRP_ATTR_S stGrpAttr;


    /*** Start AD ***/
    s32Ret = m_Vio.SAMPLE_COMM_VI_ADStart(enViMode, enNorm);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("Start AD failed!\n");
        return HI_FALSE;
    }

    s32Ret = m_Vio.SAMPLE_COMM_VI_Start(enViMode, enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_VI_START;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(enNorm, PIC_HD720, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_VI_START;
    }

    memset(&stGrpAttr,0,sizeof(VPSS_GRP_ATTR_S));
    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    m_pVpss = new Sample_Common_Vpss(m_ViChnCnt,VPSS_MAX_CHN_NUM,&stSize,&stGrpAttr);
    if(!m_pVpss){
        goto END_VSPP;
    }
    if(HI_FALSE == m_pVpss->SAMPLE_COMM_VPSS_CreatIsSucess()){
        goto END_VPSS_START;
    }
    s32Ret = m_Vio.SAMPLE_COMM_VI_BindVpss(enViMode,m_pVpss->m_Grp_Tab,m_ViChnCnt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_BIND_VPSS;
    }

    return HI_TRUE;

END_BIND_VPSS:
    m_Vio.SAMPLE_COMM_VI_UnBindVpss(enViMode,m_pVpss->m_Grp_Tab,m_ViChnCnt);
END_VPSS_START:
    m_pVpss->SAMPLE_COMM_VPSS_Stop();
END_VSPP:
    delete m_pVpss;
END_VI_START:
    m_Vio.SAMPLE_COMM_VI_Stop(enViMode);

    return HI_FALSE;


}

void Vio::onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode)
{
    //qDebug()<<"get vo_mode:"<<enVoMode<<endl;
    Vo_SetMode(enVoMode);
}

void Vio::onDispChnToWin(QMap<VO_CHN,RECT_S> &ChnAttr)
{
    QMap<VO_CHN,RECT_S>::iterator i;

    Vo_Stop(-1);
    for(i = ChnAttr.begin();i != ChnAttr.end();i++){
        Vo_Start(i.key(),i.value());
        qDebug()<<"channel:"<<i.key();
    }


}

void Vio::Set_VencAttr(PAYLOAD_TYPE_E enType,PIC_SIZE_E enSize,SAMPLE_RC_E enRcMode,HI_U32 u32Profile)
{
    m_enType = enType;
    m_enSize = enSize;
    m_enRcMode = enRcMode;
    m_u32Profile = u32Profile;
}

HI_S32 Vio::Vi_Venc_Start()
{
    VI_CHN i;
    HI_S32 s32Ret;

    for(i = 0;i < m_ViChnCnt;i++){
        m_pVenc[i] = new Sample_Common_Venc();
        m_pVenc[i]->SAMPLE_COMM_VENC_SetAttr(m_enType,enNorm, m_enSize, m_enRcMode,0,m_u32Profile);
        s32Ret = m_pVenc[i]->SAMPLE_COMM_VENC_Start();
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_1;
        }

        s32Ret = m_pVenc[i]->SAMPLE_COMM_VENC_BindVpss(m_pVpss->m_Grp_Tab[i], m_VencBindVpss);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_2;
        }

    }

    start();
    return s32Ret;

END_1:
    m_pVenc[i]->SAMPLE_COMM_VENC_Stop();
    for(i--;i >= 0;i--){
        m_pVenc[i]->SAMPLE_COMM_VENC_Stop();
        m_pVenc[i]->SAMPLE_COMM_VENC_UnBindVpss(m_pVpss->m_Grp_Tab[i], m_VencBindVpss);
    }
END_2:
    for(;i >= 0;i--){
        m_pVenc[i]->SAMPLE_COMM_VENC_Stop();
        m_pVenc[i]->SAMPLE_COMM_VENC_UnBindVpss(m_pVpss->m_Grp_Tab[i], m_VencBindVpss);
    }

    return s32Ret;
}
void Vio::run()
{
    HI_S32 i = 0;
//    HI_S32 s32Cnt = 0;
//    VENC_CHN_ATTR_S stVencChnAttr;
//    SAMPLE_VENC_GETSTREAM_PARA_S* pstPara;
//    HI_S32 maxfd = 0;
    struct timeval TimeoutVal;
    fd_set read_fds;
//    HI_S32 VencFd[VENC_MAX_CHN_NUM];
//    HI_CHAR aszFileName[VENC_MAX_CHN_NUM][64];
//    HI_CHAR venc_path_name[64];

    VENC_CHN_STAT_S stStat;
    VENC_STREAM_S stStream;
    HI_S32 s32Ret;
//    VI_CHN_LUM_S stLuma;
//    VENC_CHN VencChn;
//    PAYLOAD_TYPE_E enPayLoadType[VENC_MAX_CHN_NUM];
//    VENC_STREAM_BUF_INFO_S stStreamBufInfo[VENC_MAX_CHN_NUM];

    /******************************************
     step 1:  check & prepare save-file & venc-fd
    ******************************************/
    if (m_ViChnCnt >= VENC_MAX_CHN_NUM)
    {
        SAMPLE_PRT("input count invaild\n");
        return ;
    }
//    for (i = 0; i < m_ViChnCnt; i++)
//    {
        /* decide the stream file name, and open file to save stream */
//        s32Ret = HI_MPI_VENC_GetChnAttr(m_pVenc[i]->m_Venc_Chn, &stVencChnAttr);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_PRT("HI_MPI_VENC_GetChnAttr chn[%d] failed with %#x!\n", \
//                       m_pVenc[i]->m_Venc_Chn, s32Ret);
//            return;
//        }
//        enPayLoadType[i] = stVencChnAttr.stVeAttr.enType;
//        s32Ret = Sample_Common_Venc::SAMPLE_COMM_VENC_GetFilePostfix(enPayLoadType[i], szFilePostfix);
//        if (s32Ret != HI_SUCCESS)
//        {
//            SAMPLE_PRT("SAMPLE_COMM_VENC_GetFilePostfix [%d] failed with %#x!\n", \
//                       stVencChnAttr.stVeAttr.enType, s32Ret);
//            return ;
//        }
//        sprintf(venc_path_name,"%s/channel%d",venc_path,i);
//        QDir dir(venc_path_name);
//        if(!dir.exists()){
//            if(!dir.mkpath(venc_path_name)){
//                SAMPLE_PRT("mkpath %s failed\n",venc_path_name);
//                return;
//            }
//        }
//        snprintf(aszFileName[i], FILE_NAME_LEN, "%s/stream%d%s", venc_path_name , i , szFilePostfix);
//        m_pFile[i] = fopen(aszFileName[i], "wb");
//        if (!m_pFile[i])
//        {
//            SAMPLE_PRT("open file[%s] failed!\n",
//                       aszFileName[i]);
//            return ;
//        }        /* Set Venc Fd. */
//        VencFd[i] = HI_MPI_VENC_GetFd(m_pVenc[i]->m_Venc_Chn);
//        if (VencFd[i] < 0)
//        {
//            SAMPLE_PRT("HI_MPI_VENC_GetFd failed with %#x!\n",
//                       VencFd[i]);
//            return ;
//        }
//        if (m_maxfd <= VencFd[i])
//        {
//            m_maxfd = VencFd[i];
//        }
//        s32Ret = HI_MPI_VENC_GetStreamBufInfo (m_pVenc[i]->m_Venc_Chn, &stStreamBufInfo[i]);
//        if (HI_SUCCESS != s32Ret)
//        {
//            SAMPLE_PRT("HI_MPI_VENC_GetStreamBufInfo failed with %#x!\n", s32Ret);
//            return ;
//        }
//    }

//    if(!make_file()){
//        return ;
//    }
//    Venc_CreatNewFile(0);
//    for(int i = 0; i < m_ViChnCnt;i++){
//        s32Ret = HI_MPI_VI_GetChnLuma(i,&stLuma);
//        if (HI_SUCCESS != s32Ret){
//            SAMPLE_PRT("HI_MPI_VI_GetChnLuma(%d) failed with %#x!\n", i,s32Ret);
//        }
//        qDebug("vi(%d)Luma =%d",i,stLuma.u32Lum);
//    }


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
            continue;
        }


        for(i = 0; i < m_VencChnPara.count(); i++){
            FD_SET(m_VencChnPara[i].VencFd, &read_fds);
        }
        TimeoutVal.tv_sec  = 2;
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
    }
    qDebug()<<"exit venc thread";
    /*******************************************************
     step 3 : close save-file
    *******************************************************/
    m_file_mutex.lock();
    for (i = 0; i < m_VencChnPara.count(); i++)
    {  
        m_pVenc[i]->SAMPLE_COMM_VENC_Stop();
        m_pVenc[i]->SAMPLE_COMM_VENC_UnBindVpss(m_pVpss->m_Grp_Tab[i], m_VencBindVpss);
        delete m_pVenc[i];
        fclose(m_VencChnPara[i].pFile);
        m_VencChnPara.removeAt(i);

    }
    m_file_mutex.unlock();
    return;
}

HI_S32 Vio::Venc_exit()
{
    m_Venc_Run = HI_FALSE;
}

void Vio::onfinish()
{
    Venc_exit();
}
