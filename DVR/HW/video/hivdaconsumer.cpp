#include "hivdaframeconsumer.h"

HiVdaConsumer::HiVdaConsumer(HiFrameSource *Source, int &DevId, int &Chn, MOD_ID_E Mod):
    HiFrameConsumer (Source,Mod,DevId,Chn)
{

}

bool HiVdaConsumer::isVdaConsumer() const
{
    return true;
}
