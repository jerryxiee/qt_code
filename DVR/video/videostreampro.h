#ifndef VIDEOSTREAMPRO_H
#define VIDEOSTREAMPRO_H

#include <QObject>
#include "common/sample_comm.h"
#include "common/sample_common_sys.h"
#include "settings/settings.h"
#include "video/videoplay.h"
#include "video/videolink.h"
#include "video/videocontrol.h"
#include "video/videosearch.h"
#include "video/mp4fileindex.h"
#include "video/mp4play.h"

class VideoStreamPro : public QObject
{
    Q_OBJECT
public:
    explicit VideoStreamPro(QObject *parent = nullptr);

    void setTime(QList<uint> &start, QList<uint> &end,VIDEO_TYPE type);      //设置播放本地视频时间段,type为报警文件或正常录像文件
    void setStreamType(SEND_STREAM_T type);
    int getVencFd() const;
    int getVencChn() const;
    bool startVenc(int Chn,SEND_STREAM_T type);
    bool setVencAttr(PIC_SIZE_E enSize, HI_U32 u32BitRate, SAMPLE_RC_E enRcMode = SAMPLE_RC_CBR,HI_U32  u32Profile = 0);
    bool stopVenc(int Chn);
    bool isStop() const {return !mStart;}

private:
    bool startLocalStream(VO_DEV VoDev,VO_LAYER VoLayer,int startChn);
    bool stopLocalStream(VO_DEV VoDev,VO_LAYER VoLayer);
//    bool startRealTime(VO_DEV VoDev,VO_LAYER VoLayer,int startindex);
//    bool virVoBindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
//    bool virVoUnBindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);


signals:

public slots:

private:
    const VPSS_CHN mVencBindVpss = VPSS_CHN1;   //子码流绑定到通道1
    static VencSet *mVencSet;

    SEND_STREAM_T mStreamType;

    Sample_Common_Venc *mpVenc;
    Sample_Common_Vpss *mpVpss;
    VideoLink mVideoLink;
    QList<MP4VideoPlay>mVideoPlayList;
    QList<uint>mStartTime;
    QList<uint>mEndTime;
    VIDEO_TYPE mVideoType;
//    VideoSearch mVideoSearch;
    bool mStart;



};

#endif // VIDEOSTREAMPRO_H
