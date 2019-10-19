#include "hivpssconsumer.h"

HiVpssConsumer::HiVpssConsumer(HiFrameSource *Source, int &DevId, int &Chn, MOD_ID_E Mod):
    HiFrameConsumer (Source,Mod,DevId,Chn)
{

}
HiVpssConsumer::~HiVpssConsumer()
{

}

bool HiVpssConsumer::isVpssConsumer() const
{
    return true;
}
