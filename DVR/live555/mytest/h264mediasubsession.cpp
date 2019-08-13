#include "h264mediasubsession.h"
#include "h264videosource.h"
#include "H264VideoStreamFramer.hh"
#include "H264VideoRTPSink.hh"


H264MediaSubsession * H264MediaSubsession::createNew(UsageEnvironment& env,int Chn,bool reuseFirstSource)
{
    return new H264MediaSubsession(env,Chn,reuseFirstSource);
}

H264MediaSubsession::H264MediaSubsession(UsageEnvironment& env, int Chn, bool reuseFirstSource)
    :OnDemandServerMediaSubsession(env, reuseFirstSource),mChn(Chn)
{

}

H264MediaSubsession::~H264MediaSubsession()
{

}

FramedSource* H264MediaSubsession::createNewStreamSource(unsigned clientsessionId, unsigned& estBitrate)
{
    estBitrate = 1024;

    H264VideoSource* fileSource
        = H264VideoSource::createNew(envir(), mChn);
      if (fileSource == nullptr) {
        envir() << "create video stream error \""
             << "\" as a byte-stream file source\n";

      }

      return H264VideoStreamFramer::createNew(envir(), fileSource);
}

RTPSink* H264MediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
{
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}
