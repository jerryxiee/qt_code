#ifndef HIVDACONSUMER_H
#define HIVDACONSUMER_H

#include "hiframeconsumer.h"

class HiVdaConsumer:public HiFrameConsumer
{
public:

    virtual void doProcess(){}

protected:
    HiVdaConsumer(HiFrameSource *Source, int &DevId, int& Chn,MOD_ID_E Mod = HI_ID_VDA);

private:
    virtual bool isVdaConsumer() const;
};

#endif // HIVDACONSUMER_H
