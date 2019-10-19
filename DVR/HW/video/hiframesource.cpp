#include "hiframesource.h"

HiFrameSource::HiFrameSource(MOD_ID_E Mod,int &DevId, int &Chn):HiVideoBase(Mod,DevId,Chn)
{

}

HiFrameSource::HiFrameSource(HiVideoBase* base):HiVideoBase (base)
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

MOD_ID_E HiFrameSource::getHiModId() const
{
    return HiVideoBase::getHiModId();
}
int HiFrameSource::getDevNo() const
{
    return HiVideoBase::getDevNo();
}
int HiFrameSource::getChn() const
{
    return HiVideoBase::getChn();
}




