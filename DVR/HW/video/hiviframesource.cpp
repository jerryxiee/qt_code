#include "hiviframesource.h"

HiViFrameSource *HiViFrameSource::createNew(int DevId,int Chn)
{
    return new HiViFrameSource(DevId,Chn);
}

HiViFrameSource::HiViFrameSource(int &DevId, int &Chn, MOD_ID_E Mod):HiFrameSource(Mod,DevId,Chn)
{

}

HiViFrameSource::~HiViFrameSource()
{

}

bool HiViFrameSource::isViSource() const
{
    return true;
}

