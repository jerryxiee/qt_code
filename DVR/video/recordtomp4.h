#ifndef RECORDTOMP4_H
#define RECORDTOMP4_H

#include <QThread>
#include <QTimer>
#include <QList>
#include <QMutex>
#include "common/sample_common_sys.h"
#include "settings/settings.h"
#include "common/systemlog.h"
#include "mp4file.h"
#include "common/systemlog.h"
#include "filetabfind.h"
#include "mp4fileindex.h"
#include "common/logtabdata.h"

#define BUFLEN 1024*1024

class RecordToMP4 : public QThread
{
    Q_OBJECT
public:
    explicit RecordToMP4(Sample_Common_Vpss &Vpss,QObject *parent = nullptr);
    void RecordExit();
    ~RecordToMP4();

    bool startRecordChn(int Chn,VIDEO_NORM_E enNorm);
    bool stopRecordChn(int Chn);

    bool addChnToRecord(int Chn);
    bool addRecordList(int Chn);
    bool deleteFromRecordList(int Chn);
    bool deleteChnFromRecord(int Chn);

protected:
    virtual void run();

private:
    bool isRecordOpen(int Chn);
    bool setRecordAttr(VI_CHN ViChnCnt,PIC_SIZE_E enSize,SAMPLE_RC_E enRcMode,HI_U32 u32BitRate,HI_FR32 frmRate,HI_U32 u32Profile);
    bool startRecordChn(VI_CHN ViChnCnt, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32 u32BitRate, HI_FR32 frmRate, HI_U32 u32Profile);
    int checkRecordChn(int Chn);
    bool createNewMp4File(int Chn);
    bool saveMp4File(int Chn);
    bool getAlarmFileName(int Chn,VIDEO_TYPE type,char *filename,int len);
    int checkVideoAlarmList(int Chn,VIDEO_TYPE type);
    bool initAlarmFile();
    bool checkAlarmFile(int Chn,VIDEO_TYPE type);   //检查报警文件是否存在，不存在则创建
    bool checkAlarmFile(char *filename);
    bool removeVideoAlarmEventFromlist(int Chn);
    bool removeVideoAlarmEventFromlist(int Chn,VIDEO_TYPE type);
    bool addVideoAlarmEventFromlist(int Chn,VIDEO_TYPE type);
    bool addVideoAlarmToFile(int Chn);
    bool addVideoAlarmToFile(int Chn,VIDEO_TYPE type);
    bool changeAlarmFile(int Chn);      //处理新建录像文件时的报警记录

signals:
    void createNewFileSignal(int Chn);

public slots:
    void onTimeHander();
    void onViStatusChangedSlot(VI_CHN Chn,HI_BOOL status);
    void onCreateNewFileSlot(int Chn);
    void onVideoAlarmEventChangedSlot(VI_CHN Chn,VIDEO_TYPE type,bool change);
    void onVencAttrChangedSlot(VI_CHN Chn,HI_U32 main);
    void onVencStatusChanged(VI_CHN Chn,bool open);

private:
    const HI_CHAR *ALARM_FILE_PATH = "/mnt/sda1/alarm";
    const HI_CHAR *IO_FILE = ".io_alarm";
    const HI_CHAR *MOVED_FILE = ".move_alarm";
    const HI_CHAR *VENC_PATH = "/mnt/sda1/venc";
    const VPSS_CHN m_VencBindVpss = VPSS_CHN0;   //主码流绑定到通道0
    const int64_t FILEDURATION = 60*5;

    typedef struct{
        int ViChn;
//        int VencChn;
        int VencFd;
        uint curFileIndex;
        MP4File Mp4File;
//        FILENODE filenode;
        MP4FileInfo fileinfo;
    } VENCPARAM;

    unsigned char *mIDRFramBuf;
    VencSet *mVencSet;
    Sample_Common_Venc *mPVenc[VIDEO_MAX_NUM];
    Sample_Common_Vpss &mVpss;
    QMutex mFileMutex;
    QList<VENCPARAM> mVencParam;
    FileTabFind *mPFileTabFind[VIDEO_MAX_NUM];
    MP4FileIndex *mPMP4FileIndex[VIDEO_MAX_NUM];

    bool mRun;
    int mMaxFd;

    QList<MP4FileInfo> mVideoEventFileInfoList[VIDEO_MAX_NUM];
    QMutex mEventFileMutex;

    LogTabData *mPLogDb;


};

#endif // RECORDTOMP4_H
