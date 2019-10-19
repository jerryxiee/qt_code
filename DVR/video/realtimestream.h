#ifndef REALTIMESTREAM_H
#define REALTIMESTREAM_H

#include <QThread>
#include <QMutex>
#include <QList>
#include "encodetaskscheduler.h"


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

    bool mRun;
};

#endif // REALTIMESTREAM_H
