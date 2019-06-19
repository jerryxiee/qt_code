#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QList>

#include "settings/settings.h"
#include "common/sample_comm.h"
#ifndef LUNUX_WIN
#include "video/vio.h"
#include <video/videoplay.h>
#endif

class VideoControl : public QObject
{

    Q_OBJECT
public:
    explicit VideoControl(QObject *parent = nullptr);
    ~VideoControl();

    HI_BOOL videoStart();



signals:
    void vencAttrChanged(VI_CHN Chn,HI_U32 stream);

public slots:
    void onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void onDispChnToWin(QMap<VO_CHN, RECT_S> &);
    void onStopVoSlot();
    void onVencAttrChangedSlot(VI_CHN Chn,HI_U32 stream);
    void onVencStatusChanged(VI_CHN Chn,bool start);


public:


private:

#ifndef LUNUX_WIN
    VIDEO_NORM_E m_enNorm;
    Vio vio;
    Sample_Common_Vpss *m_pVpss;
//    Vdec mVdec;
#endif
    VencSet *m_VencSet;

};

#endif // VIDEOCONTROL_H
