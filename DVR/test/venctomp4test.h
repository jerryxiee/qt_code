#ifndef VENCTOMP4TEST_H
#define VENCTOMP4TEST_H

#include <QThread>
#include <QMutex>
#include <QList>
#include "video/videostreampro.h"

extern "C"
{
#include "libavformat/avformat.h"
}


class VencToMp4Test : public QThread
{
    Q_OBJECT
public:
    explicit VencToMp4Test(QObject *parent = nullptr);
    ~VencToMp4Test();

    int HI_PDT_CreateMp4(char *mp4name);
    void HI_PDT_CloseMp4(void);
    void HI_PDT_WriteVideo(VENC_STREAM_S *pstStream,PAYLOAD_TYPE_E	enType);
    int HI_PDT_Add_Stream(AVFormatContext *poutFmtCtx);

private:
    int HI_PDT_CheckKeyFrame_H264(H264E_NALU_TYPE_E enH264EType);
    int HI_PDT_CheckKeyFrame_H265(H265E_NALU_TYPE_E enH265EType);


protected:
    virtual void run();

signals:

public slots:

private:
    VideoStreamPro mStreamPro;
    bool mRun;

    const int STREAM_FRAME_RATE = 25;
    AVFormatContext* mOutFmt_Ctx;
    bool mFirstIDRFind;
    int mPtsInc;
    int mVi;
    unsigned char *mIDRFramBuf;
};

#endif // VENCTOMP4TEST_H
