#include "hivencconsumer.h"
#include <QDebug>


HiVencConsumer *HiVencConsumer::createNew(HiFrameSource *Source,
                          VIDEO_NORM_E enNorm,
                          PIC_SIZE_E enSize,
                          SAMPLE_RC_E enRcMode,
                          uint32_t u32BitRate,
                          uint32_t frmRate,
                          uint32_t  u32Profile,
                          PAYLOAD_TYPE_E enType)
{
    HiVencConsumer *consumer = nullptr;

    consumer = new HiVencConsumer(Source,enNorm,enSize,enRcMode,u32BitRate,frmRate,u32Profile,enType);
    if(!consumer){
        if(Source)
            delete Source;
        return nullptr;
    }
    if(!consumer->isCreateVencSucess()||!consumer->isSucessbindSource()){
        delete consumer;
        return nullptr;
    }
    return consumer;

}

HiVencConsumer::HiVencConsumer(HiFrameSource *Source,
                               VIDEO_NORM_E enNorm,
                               PIC_SIZE_E enSize,
                               SAMPLE_RC_E enRcMode,
                               uint32_t u32BitRate,
                               uint32_t frmRate,
                               uint32_t  u32Profile,
                               PAYLOAD_TYPE_E enType,
                               int DevId,
                               int Chn,
                               MOD_ID_E Mod):
    HiFrameConsumer (Source,Mod,DevId,Chn),mVenc(nullptr),mCreateVencSucess(false)
{
    mVenc = new Sample_Common_Venc();
    if(mVenc){
        mVenc->SAMPLE_COMM_VENC_SetAttr(enType,enNorm,enSize,enRcMode,u32BitRate,frmRate,u32Profile);
        if(mVenc->SAMPLE_COMM_VENC_Start() != HI_SUCCESS){
            mVenc->SAMPLE_COMM_VENC_Stop();
            delete mVenc;
            mVenc = nullptr;
            return;
        }
        mCreateVencSucess = true;
        //重新设置目的通道，重新绑定源
        static_cast<HiVideoBase *>(this)->setDevNo(0);
        static_cast<HiVideoBase *>(this)->setChn(mVenc->m_Venc_Chn);
        static_cast<HiFrameConsumer *>(this)->bindSource();

    }

}

HiVencConsumer::~HiVencConsumer()
{
    closeVenc();
}

int HiVencConsumer::getVencFd() const
{
    if(mVenc){
        return mVenc->SAMPLE_COMM_VENC_GetFd();
    }

    return -1;
}

PAYLOAD_TYPE_E HiVencConsumer::getPlaylodType() const
{
    if(mVenc){
        return mVenc->m_enType;
    }

    return PT_BUTT;
}

bool HiVencConsumer::isCreateVencSucess() const
{
    return mCreateVencSucess;
}

bool HiVencConsumer::isVencConsumer() const
{
    return true;
}

void HiVencConsumer::closeVenc()
{
    qDebug()<<"enter closeVenc";
    if(mVenc){
        mVenc->SAMPLE_COMM_VENC_Stop();
        delete mVenc;
        mVenc = nullptr;
    }
    qDebug()<<"exit closeVenc";
}

int HiVencConsumer::stopStreamRecv() const
{
    if(!mVenc){
        return -1;
    }

    return mVenc->SAMPLE_COMM_VENC_StopRecv();
}

int HiVencConsumer::startStreamRecv() const
{
    if(!mVenc){
        return -1;
    }

    return mVenc->SAMPLE_COMM_VENC_StartRecv();
}

int HiVencConsumer::setVencAttr( PIC_SIZE_E enSize,
                SAMPLE_RC_E enRcMode,
                HI_U32 u32BitRate,
                HI_FR32 frmRate,
                HI_U32 u32Profile)
{
    if(!mVenc){
        return -1;
    }

    return mVenc->SAMPLE_COMM_VENC_SetChnAttr(enSize,enRcMode,u32BitRate,u32Profile,frmRate);

}
