#ifndef VIO_H
#define VIO_H

#include <QObject>
#include <QTimerEvent>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QList>
#include <QVector>
#include <QMap>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <QFileSystemWatcher>


#include "common/sample_common_sys.h"
#include <video/vdec.h>

class Vio :public QThread
{
    Q_OBJECT
public:
    explicit Vio(QObject *parent = nullptr);
    Vio(VI_CHN ViChnCnt,SAMPLE_VO_MODE_E enVoMode);
    ~Vio();
    HI_BOOL Vi_Start(VIDEO_NORM_E enNorm);
    HI_BOOL Vi_stop();
    HI_BOOL Vo_Start();
    HI_BOOL Vo_Start(VO_CHN Vo_Chn,RECT_S &pos);
    HI_BOOL Vo_Stop(VO_CHN Vo_Chn);
    HI_S32 Vo_SetMode(SAMPLE_VO_MODE_E enVoMode);
    void start_timer();

    void Set_VencAttr(PAYLOAD_TYPE_E enType,PIC_SIZE_E enSize,SAMPLE_RC_E enRcMode,HI_U32 u32Profile);
    HI_S32 Vi_Venc_Start();
    HI_BOOL Venc_CreatNewFile(VI_CHN ViChn);
    HI_BOOL Venc_Save_file_Stop(VI_CHN ViChn);
    HI_S32 Venc_exit();

private:
    HI_BOOL make_file();

protected:
    virtual void run();

public slots:
    //test
    void onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void onTimeHander();
    void onfinish();
    void onChangeStatus(VI_CHN ViChn);
    void onMakeNewFile(VI_CHN ViChn);
    void onDispChnToWin(QMap<VO_CHN, RECT_S> &);
    void onStopVoSlot();

signals:
    void VistatusChanged(VI_CHN ViChn);
    void MakeNewFile(VI_CHN ViChn);
public:
    const SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_720P;
//    const VIDEO_NORM_E enNorm = VIDEO_ENCODING_MODE_PAL;
    const VPSS_CHN m_VoBindVpss = VPSS_CHN2;
    const VPSS_CHN m_VencBindVpss = VPSS_CHN0;   //主码流绑定到通道0
    const VPSS_CHN m_ChildVencBindVpss = VPSS_CHN1;  //子码流绑定到通道1

    VI_CHN m_ViChnCnt;
    Sample_Common_Vio m_Vio;
    SAMPLE_VO_MODE_E m_enVoMode;
    Sample_Common_Vpss *m_pVpss;
    Sample_Common_Venc *m_pVenc[VENC_MAX_CHN_NUM];
    //Sample_Common_Vpss *m_pVpss[VPSS_MAX_GRP_NUM];

private:
    typedef struct{
        VI_CHN Vi_Chn;
        VENC_CHN Venc_Chn;
        HI_S32 VencFd;
        FILE *pFile;
    } Venc_Data;
    const HI_CHAR *VI_STATUS_FILE="/dev/vi_status";
    const HI_CHAR *VENC_PATH = "/mnt/sda1/venc";
    const HI_S32 TIMEOUT = 1000*1;
    const HI_U32 LUMCONST = 14745600;
    const HI_U32 MAXSIZE = 1024*1024*1024;

    VIDEO_NORM_E m_enNorm;
    PAYLOAD_TYPE_E m_enType;
    PIC_SIZE_E m_enSize;
    SAMPLE_RC_E m_enRcMode;
    HI_U32  m_u32Profile;
    HI_BOOL m_Venc_Run;
    QTimer *m_timer;
    QMutex m_file_mutex;
    QVector<FILE*> m_pFile;
    QList<Venc_Data> m_VencChnPara;
    QMap<QString,bool> m_ViStatus;
    HI_BOOL m_ViStatusChanged;
    HI_S32 m_maxfd;
    QFileSystemWatcher *m_ViDetect;
//    Vdec mVdec;
//    FILE* m_pFile[VENC_MAX_CHN_NUM];
//    array <FILE*,VENC_MAX_CHN_NUM> m_pFile;


};

#endif // VIO_H
