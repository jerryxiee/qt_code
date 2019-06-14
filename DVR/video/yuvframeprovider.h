#ifndef YUVFRAMEPROVIDER_H
#define YUVFRAMEPROVIDER_H

#include <QObject>
#include <QTimer>

#include <QtMultimedia/QAbstractVideoSurface>
#include <QtMultimedia/QVideoSurfaceFormat>
#include <QtMultimedia/QVideoFrame>

class YuvFrameProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
//    Q_PROPERTY(bool start WRITE enable)
public:
    explicit YuvFrameProvider(QObject *parent = nullptr);

    void setVideoSurface(QAbstractVideoSurface *surface);
    void setFormat(int width, int heigth, QVideoFrame::PixelFormat value);

    QAbstractVideoSurface* videoSurface() const { return m_surface; }

    int width() const;
    void setWidth(int value);
    int height() const;
    void setHeight(int value);
//    void enable(bool value);
    Q_INVOKABLE void start();

//protected:
//    virtual void run();

signals:

public slots:
    void onNewVideoContentReceived(const QVideoFrame &frame);
    void onTimeHander();

public:

private:
    QAbstractVideoSurface *m_surface = nullptr;
    QVideoSurfaceFormat m_format;
    int m_width;
    int m_height;
    bool m_start;
    QTimer *m_timer;
    FILE *mfile;
    char *pdata;
};

#endif // YUVFRAMEPROVIDER_H
