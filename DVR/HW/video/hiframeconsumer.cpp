#include "hiframeconsumer.h"

HiFrameConsumer::HiFrameConsumer(MOD_ID_E Mod, int &DevId, int &Chn):HiVideoBase(Mod,DevId,Chn)
{

}

HiFrameConsumer::~HiFrameConsumer()
{

}

bool HiFrameConsumer::isVencConsumer() const
{
    return false;
}
bool HiFrameConsumer::isVpssConsumer() const
{
    return false;
}
bool HiFrameConsumer::isVoConsumer() const
{
    return false;
}
bool HiFrameConsumer::isVdecConsumer() const
{
    return false;
}
bool HiFrameConsumer::isVdaConsumer() const
{
    return false;
}

bool HiFrameConsumer::isConsumer() const
{
    return true;
}
