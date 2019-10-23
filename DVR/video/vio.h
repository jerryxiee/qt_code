#ifndef VIO_H
#define VIO_H

#include <QObject>
#include <QTimerEvent>
#include <QTimer>
//#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QList>
#include <QVector>
#include <QMap>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <QFileSystemWatcher>
#include "settings/settings.h"


#include "common/sample_common_sys.h"
#include "common/sample_common_vi.h"
#include "regionctr.h"

class Vio :public QObject
{
    Q_OBJECT
public:
    explicit Vio(QObject *parent = nullptr);
    Vio(VI_CHN ViChnCnt,SAMPLE_VO_MODE_E enVoMode);
    ~Vio();

//    HI_BOOL Vi_Start(VIDEO_NORM_E enNorm);
    HI_BOOL Vi_Start(VIDEO_NORM_E enNorm, Sample_Common_Vpss *vpss);
    HI_BOOL Vi_stop();
    HI_BOOL Vo_Start();
    HI_BOOL Vo_Start(VO_CHN Vo_Chn,RECT_S &pos);
    HI_BOOL Vo_Stop(VO_CHN Vo_Chn);
    HI_S32 Vo_SetMode(SAMPLE_VO_MODE_E enVoMode);


private:
    void Init();
    HI_S32 OverlayInit();

protected:
//    virtual void run();

public slots:
    //test
    void onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void onTimeHander();
    void onChangeStatus(VI_CHN ViChn, HI_BOOL status);
//    void onMakeNewFile(VI_CHN ViChn);
    void onDispChnToWin(QMap<VO_CHN, RECT_S> &);
    void onStopVoSlot();
    void onOverlayTimeTypeChanged(QString type);
    void onOverlayNameChanged(int Chn,QString name);
    void onMoveTimePosChanged(int Chn,QPoint point);
    void onMoveNamePosChanged(int Chn,QPoint point);
    void onRgnOverlayShowSlot(int Chn);

signals:
    void VistatusChanged(VI_CHN,HI_BOOL);
//    void MakeNewFile(VI_CHN ViChn);
public:
    const SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_720P;
//    const VIDEO_NORM_E enNorm = VIDEO_ENCODING_MODE_PAL;
    const VPSS_CHN m_VoBindVpss = VPSS_CHN2;

    VI_CHN m_ViChnCnt;
    Sample_Common_Vi m_Vi;
    Sample_Common_Vo m_Vo;
    SAMPLE_VO_MODE_E m_enVoMode;
    Sample_Common_Vpss m_Vpss;

private:
    const HI_U32 LUMCONST = 14745600-10;
    const RGN_HANDLE TIMEHAND = 0;
    const RGN_HANDLE NAMEHAND = 1;

    VIDEO_NORM_E m_enNorm;
    QMap<QString,HI_BOOL> m_ViStatus;
    RegionCtr m_RegionCtr;
    PIXEL_FORMAT_E m_OverlayPixFmt;
    QString mOverlayTimeType = "ddd yyyy-MM-dd hh:mm:ss";
    QMutex mMutexTimeType;


};

#endif // VIO_H
