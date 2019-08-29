#ifndef VIDEOPLAYTEST_H
#define VIDEOPLAYTEST_H

#include <QWidget>
#include <QPushButton>
#include "video/videofile.h"
#ifndef LUNUX_WIN
#include <video/videoplay.h>
#include <video/mp4play.h>
//#include "common/sample_common_sys.h"
#endif

class videoplaytest : public QWidget
{
    Q_OBJECT
public:
    explicit videoplaytest(QWidget *parent = nullptr);
    ~videoplaytest();

    void paintEvent(QPaintEvent *event);


signals:
    void exitClicked();

public slots:
    void onSetWinNum(int num);
    void onVideoExitClickSlot();
    void onVideoDispListSlot(QList<MP4FileInfo> & filelist);
//    void onVideoDispListSlot(QList<MP4FileInfo> & filelist);

private:
    QPushButton *mVideoExit;
#ifndef LUNUX_WIN
    Sample_Common_Vo mVideoVo;
    QList<MP4VideoPlay>mVideoPlayList;
#endif
    int mWinNum;
};

#endif // VIDEOPLAYTEST_H
