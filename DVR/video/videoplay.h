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
    HI_S32 VideoBindVo(VO_DEV VoDev,VO_CHN Chn);
    HI_S32 VideoUnBindVo(VO_DEV VoDev,VO_CHN Chn);
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
    void onSetDuration(quint32 duration);
    void onPositionChanged(quint32 pos);
    void onFileFinished();
    void pause();
    void play();
    void stop();



public:
    VDEC_CHN_ATTR_S m_stVdecChnAttr;

private:
    const VO_CHN VDEC_VO = 0;
    const HI_U64 NORMALPTS = 40000;

    Sample_Common_Vo m_Vdec_Vio;
    Sample_Common_Vpss *m_pVpss = nullptr;
    Sample_Common_Vdec *m_pVdec = nullptr;
    VdecThreadParam m_Thread_Attr;
    bool m_Vdec_Run;
    PAYLOAD_TYPE_E m_enType;
    SIZE_S  m_PicSize;
    QMutex mFileMutex;
    int mPlayPts;
    qreal mRate;
    VideoPlayList mVideoPlayList;
//    QList<VideoPlayList> mVideoList;
    quint32 mDuration;

};

#endif // VDECPLAY_H
