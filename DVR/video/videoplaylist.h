#ifndef VIDEOPLAYLIST_H
#define VIDEOPLAYLIST_H

#include <QObject>
#include "videofile.h"
#include <QMediaPlayer>
#include <QFileInfo>
#include <QtConcurrent/QtConcurrentRun>
#include <QMutex>
//#include <common/sample_common_vdec.h>

class VideoPlayList : public QObject
{
    Q_OBJECT
public:
    explicit VideoPlayList(QObject *parent = nullptr);
    ~VideoPlayList();

    quint32 duration() const;   //返回播放持续时间
    quint32 position() const;   //返回当前位置
    int getFrame(char **buff, VIDEO_MODE_E mode);    //获取一帧视频数据

signals:

    void durationChanged(quint32 duration);
    void positionChanged(quint32 position);
    void filefinished();
//    void stateChanged(QMediaPlayer::State);

public slots:
    void setPosition(quint32 position);
//    void setPlaylist(VideoFileList *playlist);
    void setPlaylist(VideoFileList &playlist);
//    void onRateChanged(qreal rate);
//    void pause();
//    void play();
//    void stop();

private:
    void calcDuration();
    int serach(quint32 *value, quint32 start, quint32 end, quint32 time);
    int getFileIndex(quint32 time);
    int getOffset(int index,quint32 time);
    bool changeCurFile();

private:
    const qint64 MINBUFSIZE = 102400;
//    Sample_Common_Vdec *m_pVdec = nullptr;
//    int mVedcVo;   //vdec输出到vo设备号
//    int mVoChn;    //绑定到vo通道号
//    VdecThreadParam mVdecParam;        //解码控制参数
    quint32 mDuration;     //播放文件持续时间
    quint32 mPosition;     //播放文件当前位置
    quint32 mCurrentFileListIndex;   //当前播放文件在列表中的索引
    QFile mCurSouFile;              //视频文件节点
    QFile mCurFileIndex;           //索引文件节点
    VideoFileList mVideoFileList;   //播放文件列表
    QFuture<void> mProcess;         //线程处理
    QMutex mFileMutex;             //文件操作锁
//    qreal mRate;                   //视频播放帧率
//    int mPlayPts;                  //控制帧率时间戳
    qint64 mCurFileOffset;          //当前打开文件的偏移
    quint32 *pFileTimeTab = nullptr;
    quint32 u32PhyAddr;
    char *mFileCache = nullptr;
    HI_U32 mFileCachePhyAddr;
    bool mCalcFram;
    quint32 mLoadFileNum;

};

#endif // VIDEOPLAYLIST_H
