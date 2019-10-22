#ifndef VIFRAMESOURCE_H
#define VIFRAMESOURCE_H

#include "hiframesource.h"

class HiViFrameSource:public HiFrameSource
{

public:
    static HiViFrameSource *createNew(int DevId,int Chn);
    virtual ~HiViFrameSource();

protected:
    HiViFrameSource(int &DevId,int &Chn,MOD_ID_E Mod = HI_ID_VIU);

private:
    virtual bool isViSource() const;
};

#endif // VIFRAMESOURCE_H
