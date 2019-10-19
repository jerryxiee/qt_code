#include "hiframeconsumer.h"
#include <QDebug>

HiFrameConsumer::HiFrameConsumer(HiFrameSource *inSource, MOD_ID_E Mod, int &DevId, int &Chn):
    HiVideoBase(Mod,DevId,Chn),mBindSource(false),mFramSource(inSource)
{
    if(DevId == -1 || Chn == -1){
        return;
    }
    if(mFramSource){
        MPP_CHN_S srcChn,destChn;

        srcChn.enModId = mFramSource->getHiModId();
        srcChn.s32DevId = mFramSource->getDevNo();
        srcChn.s32ChnId = mFramSource->getChn();

        destChn.enModId = getHiModId();
        destChn.s32DevId = getDevNo();
        destChn.s32ChnId = getChn();

        int ret = HiSystemBind(&srcChn,&destChn);
        if(ret >= 0){
            mBindSource = true;
        }
    }
}

HiFrameConsumer::~HiFrameConsumer()
{
    if(mFramSource){

        if(getDevNo() == -1 || getChn() == -1){
            delete mFramSource;
            return;
        }

        MPP_CHN_S srcChn,destChn;

        srcChn.enModId = mFramSource->getHiModId();
        srcChn.s32DevId = mFramSource->getDevNo();
        srcChn.s32ChnId = mFramSource->getChn();

        destChn.enModId = getHiModId();
        destChn.s32DevId = getDevNo();
        destChn.s32ChnId = getChn();

        int ret = HiSystemUnBind(&srcChn,&destChn);
        if(ret >= 0){
            mBindSource = false;
        }

        delete mFramSource;
    }
}

bool HiFrameConsumer::isVencConsumer() const
{
    return false;
}
bool HiFrameConsumer::isVpssConsumer() const
{
    return false;
}
bool HiFrameConsumer::isVoConsumer() const
{
    return false;
}
bool HiFrameConsumer::isVdecConsumer() const
{
    return false;
}
bool HiFrameConsumer::isVdaConsumer() const
{
    return false;
}

bool HiFrameConsumer::isConsumer() const
{
    return true;
}

bool HiFrameConsumer::isSucessbindSource() const
{
    return mBindSource;
}

void HiFrameConsumer::doProcess(void *consumer, int value)
{
    Q_UNUSED(value)

    ((HiFrameConsumer *)consumer)->doProcess();
}
