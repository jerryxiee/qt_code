#ifndef VIDEOLINK_H
#define VIDEOLINK_H

#include <QObject>
//#include "common/sample_common_vo.h"
#include "common/sample_common_sys.h"
#include <QtConcurrent/QtConcurrentRun>

//typedef void (*StreamProCallBack)(VENC_STREAM_S *pstStream,int index);

class VideoLink : public QObject
{
    Q_OBJECT
public:
    explicit VideoLink(QObject *parent = nullptr);

    VideoLink& operator=(const VideoLink & videolink);

    int Init(VO_DEV VoDev,VO_LAYER VoLayer,SAMPLE_VO_MODE_E enMode);
    int DeInit(VO_DEV VoDev,VO_LAYER VoLayer);

    int getVpssGrp() const;
    int getVpssChn() const;

    int setVideoFrameRate(uint framerate);

    int startChn(SAMPLE_VO_MODE_E enMode);
    int setChnFrameRate(int Chn,int framerate);

    int BindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
    int UnBindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
    int BindToVpss(VO_CHN VoChn,VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
    int UnBindFromVpss(VO_CHN VoChn,VPSS_GRP VpssGrp, VPSS_CHN VpssChn);
private:
    void savefram();
signals:

public slots:

private:
    Sample_Common_Vo mVo;
    Sample_Common_Vpss *mpVpss;
    QFuture<void> mProcess;

};

#endif // VIDEOLINK_H
