#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>
#include <QPushButton>
#ifndef LUNUX_WIN
#include <video/videoplay.h>
#endif

class VideoDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDisplay(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);

signals:
    void exitClicked();

public slots:
    void onVideoExitClickSlot();
    void onVideoDispSignalSlot(QString path);

private:
    QPushButton *mVideoExit;
#ifndef LUNUX_WIN
    VideoPlay mVdec;
#endif
};

#endif // VIDEODISPLAY_H
