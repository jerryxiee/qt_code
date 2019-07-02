#ifndef RECORD_H
#define RECORD_H

#include <QThread>
#include <QTimer>
#include <QList>
#include <QMutex>
#include "common/sample_common_sys.h"
#include "settings/settings.h"

class Record : public QThread
{
    Q_OBJECT
public:
    explicit Record(QObject *parent = nullptr);

    HI_BOOL addChnToRecord(VENC_CHN VencChn);
    HI_BOOL deleteChnFromRecord(VENC_CHN VencChn);
//    HI_BOOL CreatNewFile(VENC_CHN VencChn);
//    HI_BOOL Save_file_Stop(VENC_CHN VencChn);
    HI_S32 getFileSize(VENC_CHN VencChn);
    void checkFileSize();

protected:
    virtual void run();

private:
    void Init();

signals:
    void createNewFile(VENC_CHN VencChn);

public slots:
    HI_BOOL onCreatNewFileSlot(VENC_CHN VencChn);
    HI_BOOL onSaveFileStopSlot(VENC_CHN VencChn);


private:
    const HI_CHAR *VENC_PATH = "/mnt/sda1/venc";
    const HI_U32 MAXSIZE = 1024*1024*20;

    typedef struct{
        VENC_CHN Venc_Chn;
        HI_S32 VencFd;
        FILE *pFile;
    } Venc_Data;

    PAYLOAD_TYPE_E m_enType;
    PIC_SIZE_E m_enSize;
    HI_BOOL m_Venc_Run;
    QTimer *m_timer;
    QList<Venc_Data> m_VencChnPara;
    HI_S32 m_maxfd;
    QMutex m_file_mutex;
};

#endif // RECORD_H
