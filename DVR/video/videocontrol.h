#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QList>
#include <QTimer>

#include "settings/settings.h"
#include "common/sample_comm.h"
#ifndef LUNUX_WIN
#include "video/vio.h"
#include <video/videoplay.h>
#include "record.h"
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
        void timePosChanged(int,QPoint);
        void namePosChanged(int,QPoint);

public slots:
    void onSet_VoMode(SAMPLE_VO_MODE_E &enVoMode);
    void onDispChnToWin(QMap<VO_CHN, RECT_S> &);
    void onStopVoSlot();
    void onOverlayTimePosChanged(int Chn,QPoint point);
    void onOverlayNamePosChanged(int Chn,QPoint point);
    void onRgnOverlayShowSlot(int Chn,bool enable);


public:


private:
    const HI_S32 TIMEOUT = 1000*1;

    QTimer *mTimer;

#ifndef LUNUX_WIN
    VIDEO_NORM_E m_enNorm;
    Vio vio;
    Sample_Common_Vpss *m_pVpss;
    Record m_Record;
#endif

};

#endif // VIDEOCONTROL_H
