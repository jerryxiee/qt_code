#ifndef VENCSET_H
#define VENCSET_H

#include <QObject>
#include "common/sample_comm.h"
#include <QList>
#include <QMap>

Q_DECLARE_METATYPE(VDEC_PARAM)
Q_DECLARE_METATYPE(STREAMTYPE)
Q_DECLARE_METATYPE(PIC_SIZE_E)
Q_DECLARE_METATYPE(SAMPLE_RC_E)
Q_DECLARE_METATYPE(HI_BOOL)
Q_DECLARE_METATYPE(long)

class VencSet : public QObject
{
    Q_OBJECT
public:
    explicit VencSet(QObject *parent = nullptr);

    HI_BOOL loadVideoConfig();
    Q_INVOKABLE QString readVencConfig(int Chn,uint stream,QString str);
    HI_BOOL readVencConfig(int Chn,uint stream,VDEC_PARAM &str);

    Q_INVOKABLE void setVencConfig(int Chn,uint stream,QString str,QString value);
    void setVencConfig(int Chn,uint stream,VDEC_PARAM &str);
    void setFileInfo(int Chn, int index, HI_U32 offset);
    void getFileInfo(int Chn,int *index,HI_U32 *offset);

    Q_INVOKABLE void setVencAttr(QString Chn,QString mainStream,QString streamType,QString vencSize,QString enRcMode,
                                 QString profile,QString dstFrmRate,QString bitRate);
    Q_INVOKABLE void setVencOpen(int Chn,QString value);

private:
    QString vencToString(QString attr,QVariant &variant);
    QVariant stringToVenc(QString &str);

signals:
    void vencAttrChanged(VI_CHN Chn,HI_U32 stream);
    void vencStatusChanged(VI_CHN Chn,bool start);

public slots:


public:
    QList<VDEC_PARAM> m_Vdec_Param[2];
//    QMap<VI_CHN,bool> m_VencStatus;
    const QString Open = "open";
    const QString RecordTime = "RecordTime";


private:
#ifndef LUNUX_WIN
    const HI_CHAR * VIO_CONFIG_PATH = "/usr/dvr_config/.video.ini";
#else
    const HI_CHAR * VIO_CONFIG_PATH = "/home/abhw/nfsroot/dvr_config/.video.ini";
#endif

};

#endif // VENCSET_H
