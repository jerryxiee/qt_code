#ifndef HIVPSSCONSUMER_H
#define HIVPSSCONSUMER_H

#include "hiframeconsumer.h"

class HiVpssConsumer:public HiFrameConsumer
{
public:
    virtual ~HiVpssConsumer();
protected:
    HiVpssConsumer(HiFrameSource *Source, int &DevId, int& Chn,MOD_ID_E Mod = HI_ID_VPSS);
    virtual void doProcess(){}

private:
    virtual bool isVpssConsumer() const;
};

#endif // HIVPSSCONSUMER_H
