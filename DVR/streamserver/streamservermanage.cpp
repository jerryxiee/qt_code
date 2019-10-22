#include "streamservermanage.h"
#include <QDebug>

StreamServerManage::StreamServerManage(QObject *parent) : QThread(parent)
{

}

StreamServerManage::~StreamServerManage()
{

}


int StreamServerManage::checkConnectExist(StreamInfo &info)
{
    for (int i = 0;i < mStreamInfo.count();i++) {
        if((mStreamInfo[i].IpAddr == info.IpAddr)
                &&(mStreamInfo[i].Port == info.Port
                &&mStreamInfo[i].Channel == info.Channel)){
            return i;

        }

    }

    return -1;
}


bool StreamServerManage::createNewConnect(StreamParam &param)
{
    int ret;
    QList<uint> sttime;
    QList<uint> endtime;
    StreamInfo streaminfo;


    streaminfo.Channel = param.logicChannel;
    ret = inet_pton(AF_INET, param.ipAddr, &streaminfo.IpAddr);
    if(ret < 0){
        qDebug()<<"ip error!";
        return false;
    }
    streaminfo.Port = param.tcpPort != 0?param.tcpPort:param.udpPort;

    //step1:检查是否已经创建
    ret = checkConnectExist(streaminfo);
    if(ret >= 0){
        qDebug()<<"this channel has exist";
        return false;
    }

    //step2:调用流媒体结构创建流媒体链接通道


#ifndef LUNUX_WIN
    VideoStreamPro sourcePor;
    //step3:创建码流通道
    if(!param.playType){ //直播通道
        sourcePor.setTime(sttime,endtime,VIDEO_NORMAL);
        if(!sourcePor.startVenc(param.logicChannel,REALTIME1)){
            return false;
        }

    }else {  //回放通道
        return false;
    }

    streaminfo.VencChn = sourcePor.getVencChn();
    streaminfo.VencFd = sourcePor.getVencFd();

    mStreamMutex.lock();
    mStreamInfo.append(streaminfo);
    mStreamMutex.unlock();

    mStreamProvider.insert(streaminfo.VencChn,sourcePor);

#endif

    return true;

}

void StreamServerManage::run()
{
    mRun = true;

    while (mRun) {

    }

}
