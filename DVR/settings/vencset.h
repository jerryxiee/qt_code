#ifndef VENCSET_H
#define VENCSET_H

#include <QObject>
#include "common/sample_comm.h"
#include <QList>

Q_DECLARE_METATYPE(VDEC_PARAM)
Q_DECLARE_METATYPE(STREAMTYPE)
Q_DECLARE_METATYPE(PIC_SIZE_E)
Q_DECLARE_METATYPE(SAMPLE_RC_E)
Q_DECLARE_METATYPE(HI_BOOL)

class VencSet : public QObject
{
    Q_OBJECT
public:
    explicit VencSet(QObject *parent = nullptr);

    HI_BOOL loadVideoConfig();
    QString readVencConfig(VI_CHN Chn,HI_U32 stream,QString str);
    HI_BOOL readVencConfig(VI_CHN Chn,HI_U32 stream,VDEC_PARAM &str);

    void setVencConfig(VI_CHN Chn,HI_U32 stream,QString str,QString value);
    void setVencConfig(VI_CHN Chn,HI_U32 stream,VDEC_PARAM &str);

    Q_INVOKABLE void setVencAttr(QString Chn,QString mainStream,QString streamType,QString vencSize,QString enRcMode,
                                 QString profile,QString dstFrmRate,QString bitRate);

private:
    QString vencToString(QString attr,QVariant &variant);
    QVariant stringToVenc(QString &str);

signals:
    void vencAttrChanged(VI_CHN Chn,HI_U32 stream);

public slots:


public:
    QList<VDEC_PARAM> m_Vdec_Param[2];

private:
#ifndef LUNUX_WIN
    const HI_CHAR * VIO_CONFIG_PATH = "/usr/dvr_config/video.ini";
#else
    const HI_CHAR * VIO_CONFIG_PATH = "/home/abhw/nfsroot/dvr_config/video.ini";
#endif

};

#endif // VENCSET_H
