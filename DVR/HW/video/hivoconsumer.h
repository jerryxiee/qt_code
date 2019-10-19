#ifndef HIVOCONSUMER_H
#define HIVOCONSUMER_H

#include "hiframeconsumer.h"

class HiVoConsumer:public HiFrameConsumer
{
public:

protected:
    HiVoConsumer(HiFrameSource *Source, int &DevId, int& Chn,MOD_ID_E Mod = HI_ID_VOU);
    virtual void doProcess(){}

private:
    virtual bool isVoConsumer() const;
};

#endif // HIVOCONSUMER_H
