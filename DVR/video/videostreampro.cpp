#include "videostreampro.h"
#include <QDebug>

VencSet *VideoStreamPro::mVencSet = Settings::getVencIni();

VideoStreamPro::VideoStreamPro(QObject *parent) : QObject(parent),mStreamType(TYPENUM),
    mpVenc(nullptr),mVideoType(VIDEO_NORMAL),mStart(false)

{

}

void VideoStreamPro::setTime(QList<uint> &start, QList<uint> &end, VIDEO_TYPE type)
{
    mStartTime = start;
    mEndTime = end;
    mVideoType = type;

}

void VideoStreamPro::setStreamType(SEND_STREAM_T type)
{
    mStreamType = type;

}

int VideoStreamPro::getVencFd() const
{
    if(mpVenc){
        return HI_MPI_VENC_GetFd(mpVenc->m_Venc_Chn);
    }

    return -1;
}

int VideoStreamPro::getVencChn() const
{
    if(mpVenc){
        return mpVenc->m_Venc_Chn;
    }

    return -1;
}

//bool VideoStreamPro::virVoBindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn)
//{

//}

//bool VideoStreamPro::startRealTime(VO_DEV VoDev,VO_LAYER VoLayer,int startindex)
//{
//    int ret;
//    int i;



//    return false;
//}

bool VideoStreamPro::startLocalStream(VO_DEV VoDev,VO_LAYER VoLayer,int startChn)
{
    int ret;
    int i;
    QList<MP4FileInfo> filelist;
    SIZE_S stSize = {1280,720};

    ret = mVideoLink.Init(VoDev,VoLayer,VO_MODE_4MUX);
    if(ret < 0){

        goto END1;
    }

    for (i = 0;i < 4;i++) {
        MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(startChn+i,mVideoType);
        mp4fileindex->getFileList(filelist,mStartTime.at(i),mEndTime.at(i));
        delete  mp4fileindex;

        if(filelist.count() == 0){
            qDebug()<<"not file";
            continue;
        }

        mVideoPlayList.append(MP4VideoPlay());
        mVideoPlayList.last().setPlaylist(filelist);
        if(!mVideoPlayList.last().startPlay()){
            mVideoPlayList.last().stopPlay();
            continue;
        }
        mVideoPlayList.last().setVpssToUser(stSize);
//        mVideoPlayList.last().setDelay(15000);
//        if(mVideoLink.setChnFrameRate(i,20) < 0){
//            qDebug()<<"set chn framerate error";
//        }
        mVideoLink.BindVpss(i,mVideoPlayList.last().getVpssGrp(),mVideoPlayList.last().getVpssChn());

    }

    return true;

END1:
    mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT2,SAMPLE_VO_LAYER_VIRT2);
    return false;
}

bool VideoStreamPro::stopLocalStream(VO_DEV VoDev,VO_LAYER VoLayer)
{
    int i;

    mVideoLink.DeInit(VoDev,VoLayer);
    for (i = 0;i < mVideoPlayList.count();i++) {
        mVideoLink.UnBindVpss(i,mVideoPlayList.at(i).getVpssGrp(),mVideoPlayList.at(i).getVpssChn());
        mVideoPlayList[i].stopPlay();
    }
    return true;
}

bool VideoStreamPro::setVencAttr(PIC_SIZE_E enSize, HI_U32 u32BitRate, SAMPLE_RC_E enRcMode,HI_U32  u32Profile)
{
    if(!mpVenc){
        return false;
    }

    if(mpVenc->SAMPLE_COMM_VENC_SetChnAttr(enSize,enRcMode,u32BitRate,u32Profile) != HI_SUCCESS){
        qDebug()<<"set Venc Attr error!";
        return false;
    }

    return true;
}

bool VideoStreamPro::startVenc(int Chn,SEND_STREAM_T type)
{
    int ret;
    int i;
    int VpssGrp;
    int VpssChn;
    QList<MP4FileInfo> filelist;

    if(type == TYPENUM || Chn >= VIDEO_MAX_NUM){
        qDebug()<<"not set stream type";
        return false;
    }
    mStreamType = type;

    mpVenc = new Sample_Common_Venc();
    mpVenc->SAMPLE_COMM_VENC_SetAttr(PAYLOAD_TYPE,VIDEO_NORM, mVencSet->m_Vdec_Param[1][Chn].mvencSize,mVencSet->m_Vdec_Param[1][Chn].menRcMode,
            mVencSet->m_Vdec_Param[1][Chn].mu32BitRate,mVencSet->m_Vdec_Param[1][Chn].mfr32DstFrmRate,mVencSet->m_Vdec_Param[1][Chn].mu32Profile);
    ret = mpVenc->SAMPLE_COMM_VENC_Start();
    if (HI_SUCCESS != ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_1;
    }

    switch (type) {
    case REALTIME1:{
        ret = mpVenc->SAMPLE_COMM_VENC_BindVpss(VideoControl::getVpssGrp(Chn), mVencBindVpss);
        if (HI_SUCCESS != ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            mpVenc->SAMPLE_COMM_VENC_UnBindVpss(VideoControl::getVpssGrp(Chn), mVencBindVpss);
            goto END_1;
        }
        break;
    }

    case REALTIME2:{

        ret = mVideoLink.Init(SAMPLE_VO_DEV_VIRT0,SAMPLE_VO_LAYER_VIRT0,VO_MODE_4MUX);
        if(ret < 0){
            mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT0,SAMPLE_VO_LAYER_VIRT0);
            goto END_1;
        }
        VpssGrp = mVideoLink.getVpssGrp();
        VpssChn = mVideoLink.getVpssChn();

        if(VpssGrp < 0 || VpssChn < 0){
            mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT0,SAMPLE_VO_LAYER_VIRT0);
            goto END_1;
        }

        for (i = 0;i < 4;i++) {
            mVideoLink.BindVpss(i,VideoControl::getVpssGrp(i), mVencBindVpss);
        }
//        ret = mpVenc->SAMPLE_COMM_VENC_BindVo(SAMPLE_VO_DEV_VIRT0,SAMPLE_VO_LAYER_VIRT0);
        ret = mpVenc->SAMPLE_COMM_VENC_BindVpss(VpssGrp,VpssChn);
        if(ret < 0){
            mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT0,SAMPLE_VO_LAYER_VIRT0);
            for (i = 0;i < 4;i++) {
                mVideoLink.UnBindVpss(i,VideoControl::getVpssGrp(i), mVencBindVpss);
            }
            goto END_1;
        }

        break;
    }
    case REALTIME3:{
        ret = mVideoLink.Init(SAMPLE_VO_DEV_VIRT1,SAMPLE_VO_LAYER_VIRT1,VO_MODE_4MUX);
        if(ret < 0){
            mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT1,SAMPLE_VO_LAYER_VIRT1);
            goto END_1;
        }
        VpssGrp = mVideoLink.getVpssGrp();
        VpssChn = mVideoLink.getVpssChn();

        if(VpssGrp < 0 || VpssChn < 0){
            mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT1,SAMPLE_VO_LAYER_VIRT1);
            goto END_1;
        }

        for (i = 0;i < 4;i++) {
            mVideoLink.BindVpss(i,VideoControl::getVpssGrp(i+4), mVencBindVpss);
        }
//        ret = mpVenc->SAMPLE_COMM_VENC_BindVo(SAMPLE_VO_DEV_VIRT0,SAMPLE_VO_LAYER_VIRT0);
        ret = mpVenc->SAMPLE_COMM_VENC_BindVpss(VpssGrp,VpssChn);
        if(ret < 0){
            mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT1,SAMPLE_VO_LAYER_VIRT1);
            for (i = 0;i < 4;i++) {
                mVideoLink.UnBindVpss(i,VideoControl::getVpssGrp(i+4), mVencBindVpss);
            }
            goto END_1;
        }

        break;
    }
    case LOCALMODE1:{
        if(mStartTime.count()<1 || mEndTime.count()<1){
            qDebug()<<"not set time";
            goto END_1;
        }
        MP4FileIndex *mp4fileindex = MP4FileIndex::openFileIndex(Chn,mVideoType);
        mp4fileindex->getFileList(filelist,mStartTime.first(),mEndTime.first());
        delete  mp4fileindex;

        if(filelist.count() == 0){
            qDebug()<<"not file";
            goto END_1;
        }
        mVideoPlayList.append(MP4VideoPlay());
        mVideoPlayList.first().setPlaylist(filelist);
        if(!mVideoPlayList.first().startPlay()){
            mVideoPlayList.first().stopPlay();
            goto END_1;
        }

        ret = mpVenc->SAMPLE_COMM_VENC_BindVpss(mVideoPlayList.first().getVpssGrp(), mVideoPlayList.first().getVpssChn());
        if (HI_SUCCESS != ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            mpVenc->SAMPLE_COMM_VENC_UnBindVpss(mVideoPlayList.first().getVpssGrp(), mVideoPlayList.first().getVpssChn());
            mVideoPlayList.first().stopPlay();
            goto END_1;
        }

        break;
    }
    case LOCALMODE2:{
        if(!startLocalStream(SAMPLE_VO_DEV_VIRT2,SAMPLE_VO_LAYER_VIRT2,0)){
            stopLocalStream(SAMPLE_VO_DEV_VIRT2,SAMPLE_VO_LAYER_VIRT2);
            goto END_1;
        }

        VpssGrp = mVideoLink.getVpssGrp();
        VpssChn = mVideoLink.getVpssChn();
        if(VpssGrp < 0 || VpssChn < 0){
            stopLocalStream(SAMPLE_VO_DEV_VIRT2,SAMPLE_VO_LAYER_VIRT2);
            goto END_1;
        }

        ret = mpVenc->SAMPLE_COMM_VENC_BindVpss(VpssGrp,VpssChn);
        if(ret < 0){
            stopLocalStream(SAMPLE_VO_DEV_VIRT2,SAMPLE_VO_LAYER_VIRT2);
            goto END_1;
        }

        break;
    }
    case LOCALMODE3:{
        if(!startLocalStream(SAMPLE_VO_DEV_VIRT3,SAMPLE_VO_LAYER_VIRT3,4)){
            stopLocalStream(SAMPLE_VO_DEV_VIRT3,SAMPLE_VO_LAYER_VIRT3);
            goto END_1;
        }

        VpssGrp = mVideoLink.getVpssGrp();
        VpssChn = mVideoLink.getVpssChn();

        if(VpssGrp < 0 || VpssChn < 0){
            stopLocalStream(SAMPLE_VO_DEV_VIRT3,SAMPLE_VO_LAYER_VIRT3);
            goto END_1;
        }

        ret = mpVenc->SAMPLE_COMM_VENC_BindVpss(VpssGrp,VpssChn);
        if(ret < 0){
            stopLocalStream(SAMPLE_VO_DEV_VIRT3,SAMPLE_VO_LAYER_VIRT3);
            goto END_1;
        }

        break;
    }
    default:
    {
        goto END_1;
    }


    }

    mStart = true;
    return true;

END_1:
    mpVenc->SAMPLE_COMM_VENC_Stop();
    delete mpVenc;
    mpVenc = nullptr;
    return false;

}

bool VideoStreamPro::stopVenc(int Chn)
{
    int i;

    if(!mStart){
        qDebug()<<"have been stop";
        return true;
    }

    mpVenc->SAMPLE_COMM_VENC_Stop();

    switch (mStreamType) {
    case REALTIME1:{

        mpVenc->SAMPLE_COMM_VENC_UnBindVpss(VideoControl::getVpssGrp(Chn), mVencBindVpss);
        break;
    }

    case REALTIME2:{
            mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT0,SAMPLE_VO_LAYER_VIRT0);
            for (i = 0;i < 4;i++) {
                mVideoLink.UnBindVpss(i,VideoControl::getVpssGrp(i), mVencBindVpss);
            }
            mpVenc->SAMPLE_COMM_VENC_UnBindVpss(mVideoLink.getVpssGrp(),mVideoLink.getVpssChn());

        break;
    }
    case REALTIME3:{
            mVideoLink.DeInit(SAMPLE_VO_DEV_VIRT1,SAMPLE_VO_LAYER_VIRT1);
            for (i = 0;i < 4;i++) {
                mVideoLink.UnBindVpss(i,VideoControl::getVpssGrp(i+4), mVencBindVpss);
            }
            mpVenc->SAMPLE_COMM_VENC_UnBindVpss(mVideoLink.getVpssGrp(),mVideoLink.getVpssChn());

        break;
    }
    case LOCALMODE1:{
        mpVenc->SAMPLE_COMM_VENC_UnBindVpss(mVideoPlayList.first().getVpssGrp(), mVideoPlayList.first().getVpssChn());
        mVideoPlayList.first().stopPlay();

        break;
    }
    case LOCALMODE2:{
        stopLocalStream(SAMPLE_VO_DEV_VIRT2,SAMPLE_VO_LAYER_VIRT2);
        break;
    }
    case LOCALMODE3:{
        stopLocalStream(SAMPLE_VO_DEV_VIRT3,SAMPLE_VO_LAYER_VIRT3);
        break;
    }
    default:
    {

    }

    }

    if(mpVenc)
        delete mpVenc;
    mpVenc = nullptr;
    mStart = false;
    return true;
}
