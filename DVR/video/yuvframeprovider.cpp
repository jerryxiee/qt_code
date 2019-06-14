#include "yuvframeprovider.h"
#include <QDebug>
#include "common/sample_common_vpss.h"

YuvFrameProvider::YuvFrameProvider(QObject *parent) : QObject(parent)
{
    m_width = 1280;
    m_height = 720;

}

void YuvFrameProvider::setVideoSurface(QAbstractVideoSurface *surface)
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

void YuvFrameProvider::setFormat(int width, int heigth, QVideoFrame::PixelFormat value)
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

void YuvFrameProvider::onNewVideoContentReceived(const QVideoFrame &frame)
{

    if (m_surface)
        m_surface->present(frame);
}

int YuvFrameProvider::width() const
{
    return m_width;
}

void YuvFrameProvider::setWidth(int value)
{
    m_width = value;
}

int YuvFrameProvider::height() const
{
    return m_height;
}

void YuvFrameProvider::setHeight(int value)
{
    m_height = value;
}

//void FameProvider::enable(bool value)
//{
//    m_start = value;
//    if(value){
//        start();
//    }
//}


void YuvFrameProvider::onTimeHander()
{
#ifndef LUNUX_WIN
    HI_S32 s32Ret = HI_SUCCESS;

    QVideoFrame f(m_width * m_height *3/2, QSize(m_width, m_height), m_width, QVideoFrame::Format_NV21);
    if (f.map(QAbstractVideoBuffer::WriteOnly)) {
        s32Ret = Sample_Common_Vpss::SAMPLE_COMM_VPSS_GetChnFrame(0,0,f.bits(),m_width * m_height *3/2);
        if(s32Ret != HI_SUCCESS){
            SAMPLE_PRT("SAMPLE_COMM_VPSS_GetChnFrame failed!%#x\n",s32Ret);
            return ;
        }
        f.setStartTime(0);
        f.unmap();
        onNewVideoContentReceived(f);
    }


#else

//    qDebug()<<"timer interrupt";
    if(mfile){
        if(!feof(mfile)){
            fread(pdata,m_width * m_height *3/2,1,mfile);
            QVideoFrame f(m_width * m_height *3/2, QSize(m_width, m_height), m_width, QVideoFrame::Format_NV21);
            if (f.map(QAbstractVideoBuffer::WriteOnly)) {
                memcpy(f.bits(), pdata, m_width * m_height *3/2);
                f.setStartTime(0);
                f.unmap();
                qDebug()<<"emit newFrameAvailable";
                onNewVideoContentReceived(f);
            }
        }else {
            qDebug()<<"time stop";
            m_timer->stop();
        }
    }
#endif

}

void YuvFrameProvider::start()
{

#ifndef LUNUX_WIN
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_CHN_MODE_S stVpssMode;

    stVpssMode.enChnMode = VPSS_CHN_MODE_USER;
    stVpssMode.enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssMode.u32Width = 1280;
    stVpssMode.u32Height = 720;
    s32Ret = Sample_Common_Vpss::SAMPLE_COMM_VPSS_SetChnMode(0, 0, &stVpssMode, 8);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VPSS_SetChnMod failed!%#x\n",s32Ret);
        return;

    }

#else
    pdata = new char[m_width * m_height *3/2];
    mfile = fopen("/home/abhw/nfsroot/test.yuv","r");

#endif

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimeHander()));

    setFormat(m_width, m_height, QVideoFrame::Format_NV21);

    m_timer->start(30);


}
