#include "hiconsumertosource.h"


HiConsumerToSource *HiConsumerToSource::createNew(HiVideoBase *base)
{
    return new HiConsumerToSource(base);
}

HiConsumerToSource::HiConsumerToSource(HiVideoBase *base):HiFrameSource(base)
{

}
