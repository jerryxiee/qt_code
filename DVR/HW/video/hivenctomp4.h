#ifndef HIVENCTOMP4_H
#define HIVENCTOMP4_H

#include "hivencconsumer.h"
#include "video/mp4file.h"
#include <QMutex>

#define BUFLEN 256*1024

class HiVencToMp4:public HiVencConsumer
{
public:
    static HiVencToMp4 *createNew(HiFrameSource *Source,
                              VIDEO_NORM_E enNorm,
                              PIC_SIZE_E enSize,
                              SAMPLE_RC_E enRcMode,
                              uint32_t u32BitRate,
                              uint32_t frmRate,
                              uint32_t  u32Profile,
                              PAYLOAD_TYPE_E enType,
                           const char *mp4FileName);

    virtual ~HiVencToMp4();
    bool saveFile(const char *newFileName,QSize size,uint32_t frmRate);

protected:
    HiVencToMp4(HiFrameSource *Source,
                MP4File *mp4file,
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

    MP4File *mMp4File;
    unsigned char *mIDRFramBuf;
    QMutex mMutex;

};

#endif // HIVENCTOMP4_H
