#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>
#include <QPushButton>
#include <QFileInfoList>
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
    void onVideoDispListSlot(QFileInfoList & filelist);

private:
    QPushButton *mVideoExit;
#ifndef LUNUX_WIN
    VideoPlay mVdec;
#endif
};

#endif // VIDEODISPLAY_H
