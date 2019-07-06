#ifndef VIDEODETECT_H
#define VIDEODETECT_H

#include <QThread>
#include <QList>
#include <QMutex>
#include "common/sample_comm.h"

class VideoDetect : public QThread
{
    Q_OBJECT
public:
    typedef struct{
        MPP_CHN_S stSrcChn;
        VDA_CHN VdaChn;
        HI_S32 VdaFd;
        HI_BOOL enable;
    } VDACHNATTR;
    explicit VideoDetect(QObject *parent = nullptr);

    HI_S32 createMoveDetect(VDA_CHN VdaChn,VI_CHN Chn,HI_U32 u32SadTh,HI_U32 u32VdaIntvl);
    HI_S32 createMoveDetect(VDA_CHN VdaChn,VPSS_GRP Grp,VPSS_CHN VpssChn,HI_U32 u32SadTh,HI_U32 u32VdaIntvl);
    HI_S32 distroyVdaChn(VDA_CHN VdaChn);
    HI_S32 VdaStart(VDA_CHN VdaChn);
    HI_S32 VdaStop(VDA_CHN VdaChn);
    void VideoDetectExit();
    void addVdaToList(VDACHNATTR &value);
    void removeVdaFromList(int index);


protected:
    virtual void run();


private:
    int checkVdaChn(VDA_CHN VdaChn);
    HI_S32 Vda_CreateChn(VDA_CHN VdaChn,HI_U32 u32SadTh,HI_U32 u32VdaIntvl);
    HI_S32 SAMPLE_COMM_VDA_MdPrtSad(FILE *fp, VDA_DATA_S *pstVdaData);
    HI_S32 SAMPLE_COMM_VDA_MdPrtObj(FILE *fp, VDA_DATA_S *pstVdaData);
    bool VideoMoveDetectProcess(VDA_DATA_S *pstVdaData);


signals:
    void videoMoveDetectChangeSignal(VI_CHN Chn,VIDEO_TYPE type,bool change);

public slots:
    void onViStatusChangedSlot(VI_CHN index ,HI_BOOL enable);

private:

    QList<VDACHNATTR> mVdaChnList;
    QMutex mVdaListMutex;

    bool mdetect_run;
    HI_S32 maxfd = 0;
    int mDetectDelay = 1;
};

#endif // VIDEODETECT_H
