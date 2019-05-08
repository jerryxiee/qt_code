#ifndef VDEC_H
#define VDEC_H

#include <QObject>
#include <QThread>
#include "common/sample_common_vio.h"
#include "common/sample_common_vdec.h"

class Vdec : public QThread
{
    Q_OBJECT
public:
    explicit Vdec(QObject *parent = nullptr);
    ~Vdec();

    void Set_VdecAttr(VdecThreadParam &VdecAttr);
    HI_BOOL Start_Vdec(char *filename);
    void Stop_Vdec();

private:
    char* Get_FileType(char *filename);
protected:
    virtual void run();

signals:

public slots:

public:
    VDEC_CHN_ATTR_S m_stVdecChnAttr;

private:
    const VO_CHN VDEC_VO = 0;

    Sample_Common_Vio m_Vdec_Vio;
    Sample_Common_Vpss *m_pVpss;
    Sample_Common_Vdec *m_pVdec;
    VdecThreadParam m_Thread_Attr;
    bool m_Vdec_Run;
};

#endif // VDEC_H
