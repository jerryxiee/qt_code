#include "hivideobase.h"

HiVideoBase::HiVideoBase(MOD_ID_E Mod, int &DevId, int &Chn):mHiModId(Mod),mDevNo(DevId),mChn(Chn)
{

}

HiVideoBase::HiVideoBase(HiVideoBase *base)
{
    if(!base || base == this)
        return;

    mHiModId = base->getHiModId();
    mDevNo = base->getDevNo();
    mChn = base->getChn();

}

HiVideoBase::~HiVideoBase()
{

}

bool HiVideoBase::isSource() const
{
    return false;
}
bool HiVideoBase::isConsumer() const
{
    return false;
}

int HiVideoBase::HiSystemBind(MPP_CHN_S *pstSrcChn,MPP_CHN_S *pstDestChn)
{
    return HI_MPI_SYS_Bind(pstSrcChn,pstDestChn);
}

int HiVideoBase::HiSystemUnBind(MPP_CHN_S *pstSrcChn,MPP_CHN_S *pstDestChn)
{
    return HI_MPI_SYS_UnBind(pstSrcChn,pstDestChn);
}

MOD_ID_E HiVideoBase::getHiModId() const
{
    return mHiModId;
}

int HiVideoBase::getDevNo() const
{
    return mDevNo;
}

int HiVideoBase::getChn() const
{
    return mChn;
}

void HiVideoBase::setDevNo(int DevNo)
{
    mDevNo = DevNo;
}
void HiVideoBase::setChn(int Chn)
{
    mChn = Chn;
}
