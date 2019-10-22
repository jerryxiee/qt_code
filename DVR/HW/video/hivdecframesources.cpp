#include "hivdecframesources.h"

HiVdecFrameSources::HiVdecFrameSources(int DevNo, int Chn, MOD_ID_E Mod):HiFrameSource (Mod,DevNo,Chn)
{

}

HiVdecFrameSources::~HiVdecFrameSources()
{

}

bool HiVdecFrameSources::isVdecSource() const
{
    return true;
}
