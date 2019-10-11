#include "videolink.h"

VideoLink::VideoLink(QObject *parent) : QObject(parent),mpVpss(nullptr)
{

}

VideoLink& VideoLink::operator=(const VideoLink & videolink)
{
    mVo = videolink.mVo;
    mpVpss = videolink.mpVpss;
    mProcess = videolink.mProcess;

    return *this;
}

int VideoLink::Init(VO_DEV VoDev, VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode)
{
    int ret;
    VO_PUB_ATTR_S stPubAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
//    SIZE_S stSize = {1280,720};
    VPSS_GRP_ATTR_S stGrpAttr;

    stPubAttr.enIntfSync = VO_OUTPUT_720P50;
    stPubAttr.enIntfType = VO_INTF_BT1120;
    stPubAttr.u32BgColor = 0x0;

//    mVo.SAMPLE_COMM_VO_MemConfig("ddr1");

    mVo.SAMPLE_COMM_VO_SetDev(VoDev,VoLayer,enMode);

    ret = mVo.SAMPLE_COMM_VO_StartDev(&stPubAttr);
    if (HI_SUCCESS != ret)
    {
        SAMPLE_PRT("start vo dev failed with %d!\n", ret);
        goto SAMPLE_HIFB_NoneBufMode_1;
    }

    memset(&(stLayerAttr), 0 , sizeof(VO_VIDEO_LAYER_ATTR_S));
    ret = mVo.SAMPLE_COMM_VO_GetWH(stPubAttr.enIntfSync, \
        &stLayerAttr.stImageSize.u32Width, \
        &stLayerAttr.stImageSize.u32Height, \
        &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_GetWH failed!\n");
        goto SAMPLE_HIFB_NoneBufMode_1;
    }

    stLayerAttr.u32DispFrmRt = 25;
    stLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    stLayerAttr.stDispRect.u32Width   = stLayerAttr.stImageSize.u32Width;
    stLayerAttr.stDispRect.u32Height  = stLayerAttr.stImageSize.u32Height;
    ret = mVo.SAMPLE_COMM_VO_StartLayer(&stLayerAttr);
    if (HI_SUCCESS != ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartLayer failed!\n");
        goto END_1;
    }

    ret = mVo.SAMPLE_COMM_VO_StartChn(enMode);
    if (HI_SUCCESS != ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_2;
    }

    memset(&stGrpAttr,0,sizeof(VPSS_GRP_ATTR_S));
    stGrpAttr.u32MaxW = stLayerAttr.stDispRect.u32Width;
    stGrpAttr.u32MaxH = stLayerAttr.stDispRect.u32Height;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    mpVpss = new Sample_Common_Vpss(1,1,&stLayerAttr.stImageSize,&stGrpAttr);
    if(!mpVpss){
        qDebug()<<"create vpss error";
    }
    if(HI_FALSE == mpVpss->SAMPLE_COMM_VPSS_CreatIsSucess()){
        qDebug()<<"vpss error";
        goto END_3;
    }

    if(BindToVpss(0,mpVpss->m_Grp_Tab[0], VPSS_CHN0) < 0){
        qDebug()<<"vo bind vpss error";
        goto END_3;
    }

//    mProcess = QtConcurrent::run(this,&VideoLink::savefram);
//    priPreViewFile(Chn ,filetype);

    return 0;

END_3:
    mpVpss->SAMPLE_COMM_VPSS_Stop();

END_2:
    mVo.SAMPLE_COMM_VO_StopChn();

END_1:
    mVo.SAMPLE_COMM_VO_StopLayer();

SAMPLE_HIFB_NoneBufMode_1:
    mVo.SAMPLE_COMM_VO_StopDev();

    return -1;
}

int VideoLink::getVpssGrp() const
{
    if(!mpVpss){
        return -1;
    }

    return mpVpss->m_Grp_Tab[0];

}

int VideoLink::getVpssChn() const
{
    if(!mpVpss){
        return -1;
    }

    return VPSS_CHN0;
}

int VideoLink::setVideoFrameRate(uint framerate)
{
    int ret;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    ret = mVo.SAMPLE_COMM_VO_GetLayerAttr(&stLayerAttr);
    if(ret < 0){
        qDebug()<<"getlayerattr error";
        return -1;
    }
    if(mVo.SAMPLE_COMM_VO_StopLayer() < 0){
        return -1;
    }

    stLayerAttr.u32DispFrmRt = framerate;

    return mVo.SAMPLE_COMM_VO_StartLayer(&stLayerAttr);
}

int VideoLink::setChnFrameRate(int Chn, int framerate)
{
    return mVo.SAMPLE_COMM_VO_SetChnFrameRate(Chn,framerate);
}

int VideoLink::DeInit(VO_DEV VoDev, VO_LAYER VoLayer)
{
    Q_UNUSED(VoDev)
    Q_UNUSED(VoLayer)

    if(mpVpss){
        UnBindFromVpss(0,mpVpss->m_Grp_Tab[0], VPSS_CHN0);
        mpVpss->SAMPLE_COMM_VPSS_Stop();
        delete mpVpss;
    }
    mVo.SAMPLE_COMM_VO_StopChn();
    mVo.SAMPLE_COMM_VO_StopLayer();
    mVo.SAMPLE_COMM_VO_StopDev();

    mpVpss = nullptr;
}

int VideoLink::BindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    return mVo.SAMPLE_COMM_VO_BindVpss(VoChn,VpssGrp,VpssChn);
}
int VideoLink::UnBindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    return mVo.SAMPLE_COMM_VO_UnBindVpss(VoChn,VpssGrp,VpssChn);
}

int VideoLink::BindToVpss(VO_CHN VoChn,VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    return mVo.SAMPLE_COMM_VO_BindToVpss(VoChn,VpssGrp,VpssChn);
}

int VideoLink::UnBindFromVpss(VO_CHN VoChn,VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    return mVo.SAMPLE_COMM_VO_UnBindFromVpss(VoChn,VpssGrp,VpssChn);
}


void VideoLink::savefram()
{
    HI_S32 s32Ret = HI_SUCCESS;
    VIDEO_FRAME_INFO_S stVideoFrame;
    HI_VOID* yPtr;

    FILE *file = fopen("test.yuv","wb");
    int len;
    int i = 100;

    qDebug()<<"start save frame";

    while(i > 0)
    {
        s32Ret = HI_MPI_VO_GetScreenFrame(mVo.getVoLayer(),&stVideoFrame, -1);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("HI_MPI_VPSS_GetChnFrame failed with %d!\n", s32Ret);
            break;

        }

        len = stVideoFrame.stVFrame.u32Width * stVideoFrame.stVFrame.u32Height*3/2;

        yPtr= HI_MPI_SYS_Mmap(stVideoFrame.stVFrame.u32PhyAddr[0], len);
        fwrite(yPtr,len,1,file);
        HI_MPI_SYS_Munmap(yPtr, len);

//         yPtr= HI_MPI_SYS_Mmap(stVideoFrame.stVFrame.u32PhyAddr[1], len/4);
//         fwrite(yPtr,len/4,1,file);
//         HI_MPI_SYS_Munmap(yPtr, len/4);

//         yPtr= HI_MPI_SYS_Mmap(stVideoFrame.stVFrame.u32PhyAddr[2], len/4);
//         fwrite(yPtr,len/4,1,file);
//         HI_MPI_SYS_Munmap(yPtr, len/4);




        s32Ret = HI_MPI_VO_ReleaseScreenFrame(mVo.getVoLayer(),&stVideoFrame);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("HI_MPI_VPSS_GetChnFrame failed with %d!\n", s32Ret);


        }
        i--;
    }
    qDebug()<<"height:"<<stVideoFrame.stVFrame.u32Height<<" width:"<<stVideoFrame.stVFrame.u32Width;
    qDebug()<<"recard over";
    fclose(file);
}
