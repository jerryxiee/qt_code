#include "hiframesource.h"

HiFrameSource::HiFrameSource(MOD_ID_E Mod,int &DevId, int &Chn):HiVideoBase(Mod,DevId,Chn)
{

}

HiFrameSource::~HiFrameSource()
{

}
bool HiFrameSource::isViSource() const
{
    return false;
}
bool HiFrameSource::isVpssSource() const
{
    return false;
}

bool HiFrameSource::isVoSource() const
{
    return false;
}
bool HiFrameSource::isVdecSource() const
{
    return false;
}


bool HiFrameSource::isSource() const
{
    return true;
}



