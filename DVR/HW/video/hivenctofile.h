#ifndef HIVENCTOFILE_H
#define HIVENCTOFILE_H
#include "hivencconsumer.h"

class HiVencToFile:public HiVencConsumer
{
public:
    static HiVencToFile *createNew(HiFrameSource *Source,
                              VIDEO_NORM_E enNorm,
                              PIC_SIZE_E enSize,
                              SAMPLE_RC_E enRcMode,
                              uint32_t u32BitRate,
                              uint32_t frmRate,
                              uint32_t  u32Profile,
                              PAYLOAD_TYPE_E enType,
                           const char *fileName);

    virtual ~HiVencToFile();
protected:
    HiVencToFile(HiFrameSource *Source,
                 FILE *fp,
                 VIDEO_NORM_E enNorm,
                 PIC_SIZE_E enSize,
                 SAMPLE_RC_E enRcMode,
                 uint32_t u32BitRate,
                 uint32_t frmRate,
                 uint32_t  u32Profile,
                 PAYLOAD_TYPE_E enType);

private:
    virtual void doProcess();


private:
    FILE *mFile;


};

#endif // HIVENCTOFILE_H
