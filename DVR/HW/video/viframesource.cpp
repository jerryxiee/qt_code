#include "viframesource.h"

ViFrameSource *ViFrameSource::createNew(int DevId,int Chn)
{
    return new ViFrameSource(DevId,Chn);
}

ViFrameSource::ViFrameSource(int &DevId, int &Chn, MOD_ID_E Mod):HiFrameSource(Mod,DevId,Chn)
{

}

ViFrameSource::~ViFrameSource()
{

}

bool ViFrameSource::isViSource() const
{
    return true;
}

