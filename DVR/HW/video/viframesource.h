#ifndef VIFRAMESOURCE_H
#define VIFRAMESOURCE_H

#include "hiframesource.h"

class ViFrameSource:public HiFrameSource
{

public:
    ViFrameSource *createNew(int DevId,int Chn);


protected:
    ViFrameSource(int &DevId,int &Chn,MOD_ID_E Mod = HI_ID_VIU);
    virtual ~ViFrameSource();

private:
    virtual bool isViSource() const;
};

#endif // VIFRAMESOURCE_H
