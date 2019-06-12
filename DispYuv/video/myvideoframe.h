#ifndef MYVIDEOFRAME_H
#define MYVIDEOFRAME_H

#include <QObject>
#include <QtMultimedia/QVideoFrame>
#include <QTimer>
#include <stdio.h>

class MyvideoFrame : public QObject
{
    Q_OBJECT
public:
    explicit MyvideoFrame(QObject *parent = nullptr);
    ~MyvideoFrame();

signals:
    void newFrameAvailable(const QVideoFrame &frame);

private slots:
    void onTimeHander();

public:

    int mwidth;
    int mheigh;
    QVideoFrame::PixelFormat mformat ;//(following QVideoFrame::PixelFormat)

private:
    QTimer *mtimer;
    FILE *mfile;
    char *pdata;
};

#endif // MYVIDEOFRAME_H
