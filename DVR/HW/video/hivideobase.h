#ifndef HIVIDEOBASE_H
#define HIVIDEOBASE_H

#include "common/sample_comm.h"

class HiVideoBase
{
public:
    virtual bool isSource() const;
    virtual bool isConsumer() const;

    MOD_ID_E getHiModId() const;
    int getDevNo() const;
    int getChn() const;
    void setDevNo(int DevNo);
    void setChn(int Chn);

protected:
    HiVideoBase(MOD_ID_E Mod,int &DevId,int& Chn);
    HiVideoBase(HiVideoBase *base);
    virtual ~HiVideoBase();

    virtual int HiSystemBind(MPP_CHN_S *pstSrcChn,MPP_CHN_S *pstDestChn);
    virtual int HiSystemUnBind(MPP_CHN_S *pstSrcChn,MPP_CHN_S *pstDestChn);

private:
    MOD_ID_E mHiModId;
    int mDevNo;
    int mChn;
};

#endif // HIVIDEOBASE_H
