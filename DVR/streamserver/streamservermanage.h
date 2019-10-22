#ifndef STREAMSERVERMANAGE_H
#define STREAMSERVERMANAGE_H

#include <QThread>
#include <QMutex>
#include <QList>
#include "communication/jtt808define.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <QMap>
#ifndef LUNUX_WIN
#include "video/videostreampro.h"
#endif
#include "video/encodetaskscheduler.h"

typedef struct{
    int Channel;     //录像通道
    int VencChn;     //编码通道
    int VencFd;      //编码通道句柄
    uint32_t IpAddr;      //ip地址
    int Port;        //端口号
}StreamInfo;

class StreamServerManage : public QThread
{
    Q_OBJECT
public:
    explicit StreamServerManage(QObject *parent = nullptr);
    ~StreamServerManage();
    bool createNewConnect(StreamParam &param);
    bool disConnect(StreamInfo &info);
    bool streamCtr(StreamControl &streamctr);

protected:
    virtual void run();

private:
    int checkConnectExist(StreamInfo &info);

signals:

public slots:

private:
    bool mRun;
    QMutex mStreamMutex;
    QList<StreamInfo> mStreamInfo;

#ifndef LUNUX_WIN
    QMap<int,VideoStreamPro> mStreamProvider;
#endif
};

#endif // STREAMSERVERMANAGE_H
