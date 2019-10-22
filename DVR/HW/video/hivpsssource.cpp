#include "hivpsssource.h"


HiVpssSource *HiVpssSource::createNew(int ChnNum, SIZE_S *pstSize, VPSS_GRP_ATTR_S *pstVpssGrpAttr)
{
    HiVpssSource *vpssSource = new HiVpssSource(-1,-1,ChnNum,pstSize,pstVpssGrpAttr);
    if(!vpssSource){
        return nullptr;
    }

    if(!vpssSource->isCreateSucess()){
        delete vpssSource;
        vpssSource = nullptr;
    }

    return vpssSource;

}
HiVpssSource *HiVpssSource::createNew(int DevId,int Chn)
{

    return new HiVpssSource(DevId,Chn);
}

HiVpssSource::HiVpssSource(int DevId, int Chn,int ChnNum,SIZE_S *pstSize,VPSS_GRP_ATTR_S *pstVpssGrpAttr, MOD_ID_E Mod):
    HiFrameSource (Mod,DevId,Chn),mVpss(nullptr),mCreateSucess(false)
{
    if(ChnNum > 0){
        mVpss = new Sample_Common_Vpss(1,ChnNum,pstSize,pstVpssGrpAttr);
        if(!mVpss){
            return;
        }
        if(mVpss->SAMPLE_COMM_VPSS_CreatIsSucess() != HI_TRUE){
            mVpss->SAMPLE_COMM_VPSS_Stop();
            delete mVpss;
            mVpss = nullptr;
            return;
        }
        static_cast<HiVideoBase *>(this)->setDevNo(0);
        static_cast<HiVideoBase *>(this)->setChn(mVpss->m_Grp_Tab[0]);
    }
    mCreateSucess = true;

}

HiVpssSource::~HiVpssSource()
{
    if(mVpss){
        mVpss->SAMPLE_COMM_VPSS_Stop();
        delete mVpss;
        mVpss = nullptr;
    }
}

bool HiVpssSource::isCreateSucess() const
{
    return mCreateSucess;
}

bool HiVpssSource::isVpssSource() const
{
    return true;
}
