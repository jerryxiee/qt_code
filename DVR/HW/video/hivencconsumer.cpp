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

    Sample_Common_Venc *Venc = new Sample_Common_Venc();
    if(Venc){
        Venc->SAMPLE_COMM_VENC_SetAttr(enType,enNorm,enSize,enRcMode,u32BitRate,frmRate,u32Profile);
        if(Venc->SAMPLE_COMM_VENC_Start() != HI_SUCCESS){
           goto END;
        }
    }

    consumer = new HiVencConsumer(Source,Venc,0,Venc->m_Venc_Chn);
    if(!consumer){
        goto END;
    }
    if(!consumer->isSucessbindSource()){

        goto END1;
    }
    return consumer;

END1:
    delete consumer;
END:
    if(Venc){
        Venc->SAMPLE_COMM_VENC_Stop();
        delete Venc;
        Venc = nullptr;
    }
    return nullptr;
}

HiVencConsumer::HiVencConsumer(HiFrameSource *Source, Sample_Common_Venc *Venc, int DevId, int Chn, MOD_ID_E Mod):
    HiFrameConsumer (Source,Mod,DevId,Chn),mVenc(Venc)
{

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
