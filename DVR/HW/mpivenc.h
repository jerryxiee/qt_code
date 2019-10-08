#ifndef MPIVENC_H
#define MPIVENC_H

#include "common/sample_comm.h"

class MpiVenc
{
public:
    MpiVenc();

    static int applyOncCoder();
    static bool releaseOneCoder(int Chn);
    static bool startCoderChn(int Chn,PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, uint u32BitRate, HI_FR32 frmRate, uint  u32Profile=0,uint framInterval=0);
    static bool SetChnAttr(int Chn,PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode,HI_U32 u32BitRate,HI_U32  u32Profile, HI_FR32 frmRate = 0);


private:

    static int sMinCanUseIndex;
    static uint32_t sVencMask[4];
};

#endif // MPIVENC_H
