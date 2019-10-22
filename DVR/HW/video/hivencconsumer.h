#ifndef HIVENCCONSUMER_H
#define HIVENCCONSUMER_H

#include "hiframeconsumer.h"
#include "common/sample_common_venc.h"

class HiVencConsumer:public HiFrameConsumer
{
public:
    static HiVencConsumer *createNew(HiFrameSource *Source,
                              VIDEO_NORM_E enNorm,
                              PIC_SIZE_E enSize,
                              SAMPLE_RC_E enRcMode,
                              uint32_t u32BitRate,
                              uint32_t frmRate,
                              uint32_t  u32Profile,
                              PAYLOAD_TYPE_E enType);
    virtual ~HiVencConsumer();
    int getVencFd() const;
    PAYLOAD_TYPE_E getPlaylodType() const;
    bool isCreateVencSucess() const;

protected:
    HiVencConsumer(HiFrameSource *Source,
                   VIDEO_NORM_E enNorm,
                   PIC_SIZE_E enSize,
                   SAMPLE_RC_E enRcMode,
                   uint32_t u32BitRate,
                   uint32_t frmRate,
                   uint32_t  u32Profile,
                   PAYLOAD_TYPE_E enType,
                   int DevId = -1,
                   int Chn = -1,
                   MOD_ID_E Mod = HI_ID_VENC);

    virtual void doProcess(){}

private:
    virtual bool isVencConsumer() const;
    void closeVenc();

private:
    Sample_Common_Venc *mVenc;
    bool mCreateVencSucess;

};

#endif // HIVENCCONSUMER_H
