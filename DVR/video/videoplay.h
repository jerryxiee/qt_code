#ifndef VDECPLAY_H
#define VDECPLAY_H

#include <QObject>
#include <QThread>
#include "common/sample_common_vo.h"
#include "common/sample_common_vdec.h"
#include <QFileInfoList>
#include <QMediaPlayer>
#include <QMutex>
#include <QtConcurrent/QtConcurrentRun>
#include "videofile.h"
#include "videoplaylist.h"

class VideoPlay : public QThread
{
    Q_OBJECT
public:
    explicit VideoPlay(QObject *parent = nullptr);
    ~VideoPlay();

    void Set_VdecAttr(VdecThreadParam &VdecAttr);
    HI_BOOL Start_Vdec(char *filename,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
    void Stop_Vdec();

    void setFileList(VideoFileList &fileList);
    void setCurrentposition(int percent);
    void videoStart();
    void setRate(qreal rate);

private:
    char* Get_FileType(char *filename);
    bool dealFileHead(char *filename);
    void setPosition(uint value);
    void caclFramNum();
    int serach(HI_U32 *value, int start, int end, int num);
    int getFileIndex(HI_U32 framenum);
    off_t getFrameOffset(int fileindex,int frame);
    bool getOneFrame(VDEC_STREAM_S *pstStream);
    void endofStream();
    bool changeFile(int index);
protected:
    virtual void run();

signals:
    void currentPrecentChanged(uint);
    void stateChanged(QMediaPlayer::State);

public slots:
    void onRateChanged(qreal rate);
    void pause();
    void play();
    void stop();



public:
    VDEC_CHN_ATTR_S m_stVdecChnAttr;

private:
    const VO_CHN VDEC_VO = 0;
    const int FRAMENUM = 10;
    const HI_U64 NORMALPTS = 40000;

    Sample_Common_Vo m_Vdec_Vio;
    Sample_Common_Vpss *m_pVpss = nullptr;
    Sample_Common_Vdec *m_pVdec = nullptr;
    VdecThreadParam m_Thread_Attr;
    bool m_Vdec_Run;
    int m_frameRate;
    PAYLOAD_TYPE_E m_enType;
    SIZE_S  m_PicSize;
    VideoFileList mVideoFileList;
    HI_U32 mTotalFileNum;
    HI_U32 mTotalFramNum;
    HI_U32 *pFramTab;
    HI_U32 u32PhyAddr;
    HI_U32 mCurrentPrecent ;
    HI_U32 mCurrentFileIndex;
//    FILE *mCurFileNode;
    QFile mCurFile;
    QFile mCurFileNode;
    char *mFileCache;
    HI_U32 mFileCachePhyAddr;
    HI_U32 mCurFrameIndex;
    QMutex mFileMutex;
    QFuture<void> mProcess;
    bool mCalcFram;
    int mPlayPts;
    qreal mRate;
    VideoPlayList mVideoPlayList;

};

#endif // VDECPLAY_H
