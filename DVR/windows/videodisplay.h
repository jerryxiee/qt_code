#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>
#include <QPushButton>
#include <QFileInfoList>
#include <QSlider>
#include <QLabel>
#include "playcontrols.h"
#include "video/videofile.h"
#ifndef LUNUX_WIN
#include <video/videoplay.h>
#endif

class VideoDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDisplay(QWidget *parent = nullptr);
    ~VideoDisplay();

    void paintEvent(QPaintEvent *event);

signals:
    void exitClicked();

public slots:
    void onVideoExitClickSlot();
    void onVideoDispSignalSlot(QString path);
    void onVideoDispListSlot(VideoFileList & filelist);
    void onPositionChanged(uint value);

private slots:
    void seek(int value);

private:
    QPushButton *mVideoExit;
#ifndef LUNUX_WIN
    VideoPlay mVdec;
#endif
    QSlider *slider;
    QLabel *labelDuration;
    PlayControls *controls;
};

#endif // VIDEODISPLAY_H
