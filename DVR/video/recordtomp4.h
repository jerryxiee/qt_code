#ifndef RECORDTOMP4_H
#define RECORDTOMP4_H

#include <QThread>
#include <QTimer>
#include <QList>
#include <QMutex>
#include "common/sample_common_sys.h"
#include "settings/settings.h"
#include "common/systemlog.h"

class RecordToMP4 : public QThread
{
    Q_OBJECT
public:
    explicit RecordToMP4(Sample_Common_Vpss &Vpss,QObject *parent = nullptr);

protected:
    virtual void run();

private:

signals:

public slots:

private:
    VencSet *m_VencSet;
    Sample_Common_Venc *mPVenc[VIDEO_MAX_NUM];
    Sample_Common_Vpss &mVpss;
    QMutex mFileMutex;


};

#endif // RECORDTOMP4_H
