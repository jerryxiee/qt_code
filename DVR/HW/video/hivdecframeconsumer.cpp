#include "hivdecframeconsumer.h"

HiVdecFrameConsumer *HiVdecFrameConsumer::createNew(HiFrameSource *source,PAYLOAD_TYPE_E codeType,int h,int w)
{

    HiVdecFrameConsumer *consumer = nullptr;

    consumer = new HiVdecFrameConsumer(source,codeType,h,w);
    if(!consumer){
        return nullptr;
    }

    if(!consumer->isCreateSucess()||!consumer->isSucessbindSource()){
        delete consumer;
        return nullptr;
    }

    return consumer;

}


HiVdecFrameConsumer::HiVdecFrameConsumer(HiFrameSource *source, PAYLOAD_TYPE_E codeType, int h, int w):
    HiFrameConsumer(source,HI_ID_VDEC,-1,-1),mVdec(nullptr),mCreateSucess(false)
{
    VDEC_CHN_ATTR_S stVdecChnAttr;

    stVdecChnAttr.enType       = codeType;
    stVdecChnAttr.u32BufSize   = 3 * w * h;
    stVdecChnAttr.u32Priority  = 5;
    stVdecChnAttr.u32PicWidth  = w;
    stVdecChnAttr.u32PicHeight = h;
    if (PT_H264 == codeType || PT_MP4VIDEO == codeType)
    {
        stVdecChnAttr.stVdecVideoAttr.enMode=VIDEO_MODE_FRAME;
        stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 2;
        stVdecChnAttr.stVdecVideoAttr.bTemporalMvpEnable = HI_FALSE;
    }
    else if (PT_JPEG == codeType || PT_MJPEG == codeType)
    {
        stVdecChnAttr.stVdecJpegAttr.enMode = VIDEO_MODE_FRAME;
        stVdecChnAttr.stVdecJpegAttr.enJpegFormat = JPG_COLOR_FMT_YCBCR420;
    }
    else if(PT_H265 == codeType)
    {
        stVdecChnAttr.stVdecVideoAttr.enMode=VIDEO_MODE_STREAM;
        stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 2;
        stVdecChnAttr.stVdecVideoAttr.bTemporalMvpEnable = HI_TRUE;
    }

    mVdec = new Sample_Common_Vdec(1,1,1);
    mVdec->SAMPLE_COMM_VDEC_SetChnAttr(0,&stVdecChnAttr);
    int s32Ret = mVdec->SAMPLE_COMM_VDEC_Start(1);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VDEC fail for %#x!\n", s32Ret);
        goto END;
    }

    mCreateSucess = true;
    //重新设置目的通道，重新绑定源
    static_cast<HiVideoBase *>(this)->setDevNo(0);
    static_cast<HiVideoBase *>(this)->setChn(mVdec->m_Vdec_Tab[0]);
    static_cast<HiFrameConsumer *>(this)->bindSource();

    return;

END:
    mVdec->SAMPLE_COMM_VDEC_Stop(1);
    delete mVdec;
    mVdec = nullptr;
    return ;


}

HiVdecFrameConsumer::~HiVdecFrameConsumer()
{

}

bool HiVdecFrameConsumer::isVdecConsumer() const
{
    return true;
}

bool HiVdecFrameConsumer::isCreateSucess() const
{
    return mCreateSucess;
}

void HiVdecFrameConsumer::doProcess()
{

}
