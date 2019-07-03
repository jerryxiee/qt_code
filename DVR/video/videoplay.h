#ifndef VDECPLAY_H
#define VDECPLAY_H

#include <QObject>
#include <QThread>
#include "common/sample_common_vo.h"
#include "common/sample_common_vdec.h"

class VideoPlay : public QThread
{
    Q_OBJECT
public:
    explicit VideoPlay(QObject *parent = nullptr);
    ~VideoPlay();

    void Set_VdecAttr(VdecThreadParam &VdecAttr);
    HI_BOOL Start_Vdec(char *filename,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
    void Stop_Vdec();

private:
    char* Get_FileType(char *filename);
    bool dealFileHead(char *filename);
protected:
    virtual void run();

signals:

public slots:

public:
    VDEC_CHN_ATTR_S m_stVdecChnAttr;

private:
    const VO_CHN VDEC_VO = 0;

    Sample_Common_Vo m_Vdec_Vio;
    Sample_Common_Vpss *m_pVpss = nullptr;
    Sample_Common_Vdec *m_pVdec = nullptr;
    VdecThreadParam m_Thread_Attr;
    bool m_Vdec_Run;
    int m_frameRate;
    PAYLOAD_TYPE_E m_enType;
    SIZE_S  m_PicSize;
};

#endif // VDECPLAY_H
