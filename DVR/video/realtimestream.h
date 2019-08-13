#ifndef REALTIMESTREAM_H
#define REALTIMESTREAM_H

#include <QThread>
#include <QMutex>
#include <QList>
#include "video/videostreampro.h"
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

class RealTimeStream : public QThread
{
    Q_OBJECT
public:
    explicit RealTimeStream(QObject *parent = nullptr);
    ~RealTimeStream();

protected:
    virtual void run();


signals:

public slots:

private:
    VideoStreamPro mStreamPro;
    bool mRun;
};

#endif // REALTIMESTREAM_H
