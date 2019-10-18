#ifndef HIFRAMECONSUMER_H
#define HIFRAMECONSUMER_H

#include "hivideobase.h"

class HiFrameConsumer:public HiVideoBase
{
public:

    virtual bool isVencConsumer() const;
    virtual bool isVpssConsumer() const;
    virtual bool isVoConsumer() const;
    virtual bool isVdecConsumer() const;
    virtual bool isVdaConsumer() const;

protected:
    HiFrameConsumer(MOD_ID_E Mod, int &DevId, int& Chn);
    virtual ~HiFrameConsumer();


private:
    virtual bool isConsumer() const;
};

#endif // HIFRAMECONSUMER_H
