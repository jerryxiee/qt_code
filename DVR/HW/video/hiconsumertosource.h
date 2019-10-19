#ifndef HICONSUMERTOSOURCE_H
#define HICONSUMERTOSOURCE_H

#include "hiframesource.h"

class HiConsumerToSource:public HiFrameSource
{
public:
    static HiConsumerToSource *createNew(HiVideoBase *base);

protected:
    HiConsumerToSource(HiVideoBase *base);
};

#endif // HICONSUMERTOSOURCE_H
