#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "common/sample_comm.h"

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);
    Q_INVOKABLE void setMoveDetect(int Chn, bool value);
    Q_INVOKABLE void setIOAlarm(int Chn,int io,bool value);

signals:
    void videoAlarmEventChangedSignal(VI_CHN Chn,VIDEO_TYPE type,bool change);

public slots:
};

#endif // TEST_H
