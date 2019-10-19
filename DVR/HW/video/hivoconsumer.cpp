#include "hivoconsumer.h"

HiVoConsumer::HiVoConsumer(HiFrameSource *Source, int &DevId, int& Chn, MOD_ID_E Mod):
    HiFrameConsumer (Source,Mod,DevId,Chn)
{

}

bool HiVoConsumer::isVoConsumer() const
{
    return true;
}
