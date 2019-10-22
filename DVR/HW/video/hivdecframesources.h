#ifndef HIVDECFRAMESOURCES_H
#define HIVDECFRAMESOURCES_H

#include "hiframesource.h"

class HiVdecFrameSources:public HiFrameSource
{
public:

    virtual ~HiVdecFrameSources();
protected:
    HiVdecFrameSources(int DevNo,int Chn,MOD_ID_E Mod = HI_ID_VDEC);


private:
    virtual bool isVdecSource() const;

};

#endif // HIVDECFRAMESOURCES_H
