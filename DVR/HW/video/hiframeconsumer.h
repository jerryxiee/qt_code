#ifndef HIFRAMECONSUMER_H
#define HIFRAMECONSUMER_H

#include "hivideobase.h"
#include "hiframesource.h"

class HiFrameConsumer:public HiVideoBase
{
public:

    virtual bool isVencConsumer() const;
    virtual bool isVpssConsumer() const;
    virtual bool isVoConsumer() const;
    virtual bool isVdecConsumer() const;
    virtual bool isVdaConsumer() const;
    virtual bool isSucessbindSource();
    virtual void doProcess() = 0;
    bool bindSource();

    static void doProcess(void *consumer,int value);

protected:
    HiFrameConsumer(HiFrameSource *, MOD_ID_E Mod, int DevId, int Chn);
    virtual ~HiFrameConsumer();

private:



private:
    virtual bool isConsumer() const;
    bool mBindSource;
    HiFrameSource *mFramSource;

};

#endif // HIFRAMECONSUMER_H
