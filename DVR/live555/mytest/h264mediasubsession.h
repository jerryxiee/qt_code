#ifndef H264MEDIASUBSESSION_H
#define H264MEDIASUBSESSION_H

#include "OnDemandServerMediaSubsession.hh"


class H264MediaSubsession:public OnDemandServerMediaSubsession
{
public:
    static H264MediaSubsession * createNew(UsageEnvironment& env,int Chn,bool reuseFirstSource);

protected:
    H264MediaSubsession(UsageEnvironment& env,int Chn,bool reuseFirstSource);
    ~H264MediaSubsession();

protected:
    // virtual function  new source and rtplink
    FramedSource* createNewStreamSource(unsigned clientsessionId, unsigned& estBitrate);
    RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);

private:
    int mChn;
};

#endif // H264MEDIASUBSESSION_H
