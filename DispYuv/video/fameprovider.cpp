#include "fameprovider.h"

FameProvider::FameProvider(QObject *parent) : QObject(parent)
{

}

void FameProvider::setVideoSurface(QAbstractVideoSurface *surface)
    {
        if (m_surface && m_surface != surface  && m_surface->isActive()) {
            m_surface->stop();
        }

        m_surface = surface;

        if (m_surface && m_format.isValid())
        {
            m_format = m_surface->nearestFormat(m_format);
            m_surface->start(m_format);

        }
    }

void FameProvider::setFormat(int width, int heigth, QVideoFrame::PixelFormat value)
{
    QSize size(width, heigth);
    QVideoSurfaceFormat format(size, value);
    m_format = format;

    if (m_surface)
    {
        if (m_surface->isActive())
        {
            m_surface->stop();
        }
        m_format = m_surface->nearestFormat(m_format);
        m_surface->start(m_format);
    }
}

void FameProvider::onNewVideoContentReceived(const QVideoFrame &frame)
    {

        if (m_surface)
            m_surface->present(frame);
    }
