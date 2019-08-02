#ifndef RECORD_H
#define RECORD_H

#include <QThread>
#include <QTimer>
#include <QList>
#include <QMutex>
#include "common/sample_common_sys.h"
#include "settings/settings.h"
#include "common/systemlog.h"

class Record : public QThread
{
    Q_OBJECT
public:
    explicit Record(QObject *parent = nullptr);
    void RecordExit();

    void setRecordSrc(Sample_Common_Vpss *vpss);
    void setRecordSrc(Sample_Common_Vpss &vpss);
    void setRecordStatus(VI_CHN ViChn,bool start);
    HI_S32 startRecordChn(VI_CHN Chn,VIDEO_NORM_E enNorm);
    HI_S32 stopRecordChn(VI_CHN Chn);

    HI_BOOL addChnToRecord(VI_CHN Chn);
    HI_BOOL addRecordList(VI_CHN Chn);
    HI_BOOL deleteFromRecordList(VI_CHN Chn);
    HI_BOOL deleteChnFromRecord(VI_CHN Chn);
//    HI_BOOL CreatNewFile(VENC_CHN VencChn);
//    HI_BOOL Save_file_Stop(VENC_CHN VencChn);
    HI_S32 getFileSize(VI_CHN Chn);
    void checkFileSize();

protected:
    virtual void run();

private:
    void Init();
    HI_S32 setRecordAttr(VI_CHN ViChnCnt,PIC_SIZE_E enSize,SAMPLE_RC_E enRcMode,HI_U32 u32BitRate,HI_FR32 frmRate,HI_U32 u32Profile);
    HI_S32 startRecordChn(VI_CHN ViChnCnt, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32 u32BitRate, HI_FR32 frmRate, HI_U32 u32Profile);
    HI_S32 getAlarmFileName(VI_CHN Chn,VIDEO_TYPE type,char *filename,int len);
    HI_S32 createAlarmFile(VI_CHN Chn);
    HI_S32 saveAlarmFile(VI_CHN Chn);
    HI_BOOL createNewFile(VI_CHN Chn);
    HI_BOOL createFileNode(int index);
    HI_BOOL saveFileNode(int index);
    HI_BOOL saveFileHead(int index);
    HI_S32 checkRecordChn(VI_CHN Chn);
    int checkVideoAlarmList(VI_CHN Chn,VIDEO_TYPE type);
    bool removeVideoAlarmEventFromlist(VI_CHN Chn);
    bool addVideoAlarmEventFromlist(VI_CHN Chn,VIDEO_TYPE type);
    bool removeVideoAlarmEventFromlist(VI_CHN Chn,VIDEO_TYPE type);

signals:
    void createNewFileSignal(VI_CHN Chn);

public slots:
    HI_BOOL onCreatNewFileSlot(VI_CHN Chn);
    HI_BOOL onSaveFileStopSlot(VI_CHN Chn);
    void onTimeHander();
    void onVencAttrChangedSlot(VI_CHN Chn,HI_U32 stream);
    void onVencStatusChanged(VI_CHN Chn,bool start);
    void onViStatusChangedSlot(VI_CHN Chn,HI_BOOL status);
    void onVideoAlarmEventChangedSlot(VI_CHN Chn,VIDEO_TYPE type,bool change);



private:
    const HI_CHAR *ALARM_FILE_PATH = "/mnt/sda1/alarm";
    const HI_CHAR *IO_FILE = ".io_alarm";
    const HI_CHAR *MOVED_FILE = ".move_alarm";
    const HI_CHAR *VENC_PATH = "/mnt/sda1/venc";
    const HI_U32 MAXSIZE = 1024*1024*20;
    const VPSS_CHN m_VencBindVpss = VPSS_CHN0;   //主码流绑定到通道0

    typedef struct{
        VI_CHN ViChn;
        VENC_CHN Venc_Chn;
        HI_S32 VencFd;
        FILE *pFile;
        FILE *pFile_index;
        HI_U32 frame;
        HI_U32 IInterval;
        HI_U32 ctime;
    } Venc_Data;

    VencSet *m_VencSet;
    VIDEO_NORM_E m_enNorm;
    PAYLOAD_TYPE_E m_enType;
    PIC_SIZE_E m_enSize;
    HI_BOOL m_Venc_Run;
    QList<Venc_Data> m_VencChnPara;
    HI_S32 m_maxfd;
    QMutex m_file_mutex;
    Sample_Common_Vpss m_Vpss;
    Sample_Common_Venc *m_pVenc[VIDEO_MAX_NUM];
    QMap<QString,bool> m_VencStatus;
    QList<ALARM_FILE_INFO> m_VideoEventFileInfoList[VIDEO_MAX_NUM];
    QMutex m_EventFileMutex;
    int mfileindex[VIDEO_MAX_NUM];
    HI_U32 mfileoffset[VIDEO_MAX_NUM];
    HI_CHAR curFileIndexName[VIDEO_MAX_NUM][VIDEO_FILENAME_SIZE];
};

#endif // RECORD_H
