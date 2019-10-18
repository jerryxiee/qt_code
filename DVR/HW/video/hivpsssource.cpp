#include "hivpsssource.h"

HiVpssSource::HiVpssSource(HiFrameSource &inSource, int &DevId, int &Chn, MOD_ID_E Mod):HiFrameSource (Mod,DevId,Chn),mInSource(inSource)
{

}

HiVpssSource::~HiVpssSource()
{

}

bool HiVpssSource::isVpssSource() const
{
    return true;
}
