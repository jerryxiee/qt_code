#ifndef VIFRAMESOURCE_H
#define VIFRAMESOURCE_H

#include "hiframesource.h"

class ViFrameSource:public HiFrameSource
{

public:
    static ViFrameSource *createNew(int DevId,int Chn);
    virtual ~ViFrameSource();

protected:
    ViFrameSource(int &DevId,int &Chn,MOD_ID_E Mod = HI_ID_VIU);

private:
    virtual bool isViSource() const;
};

#endif // VIFRAMESOURCE_H
