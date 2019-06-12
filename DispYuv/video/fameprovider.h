#ifndef FAMEPROVIDER_H
#define FAMEPROVIDER_H

#include <QObject>

#include <QtMultimedia/QAbstractVideoSurface>
#include <QtMultimedia/QVideoSurfaceFormat>
#include <QtMultimedia/QVideoFrame>

class FameProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface)
public:
    explicit FameProvider(QObject *parent = nullptr);

    void setVideoSurface(QAbstractVideoSurface *surface);
    void setFormat(int width, int heigth, QVideoFrame::PixelFormat value);

    QAbstractVideoSurface* videoSurface() const { return m_surface; }


signals:

public slots:
    void onNewVideoContentReceived(const QVideoFrame &frame);

private:
    QAbstractVideoSurface *m_surface = nullptr;
    QVideoSurfaceFormat m_format;
};

#endif // FAMEPROVIDER_H
