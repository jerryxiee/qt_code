#include "hivpsssource.h"


HiVpssSource *HiVpssSource::createNew(int ChnNum, SIZE_S *pstSize, VPSS_GRP_ATTR_S *pstVpssGrpAttr)
{
    Sample_Common_Vpss *Vpss = new Sample_Common_Vpss(1,ChnNum,pstSize,pstVpssGrpAttr);
    if(!Vpss){
        return nullptr;
    }
    if(Vpss->SAMPLE_COMM_VPSS_CreatIsSucess() != HI_TRUE){
        Vpss->SAMPLE_COMM_VPSS_Stop();
        delete Vpss;
        Vpss = nullptr;
    }

    return new HiVpssSource(Vpss,Vpss->m_Grp_Tab[0],ChnNum);

}
HiVpssSource *HiVpssSource::createNew(int DevId,int Chn)
{

    return new HiVpssSource(nullptr,DevId,Chn);
}

HiVpssSource::HiVpssSource(Sample_Common_Vpss *vpss, int DevId, int Chn, MOD_ID_E Mod):
    HiFrameSource (Mod,DevId,Chn),mVpss(vpss)
{

}

HiVpssSource::~HiVpssSource()
{
    if(mVpss){
        mVpss->SAMPLE_COMM_VPSS_Stop();
        delete mVpss;
        mVpss = nullptr;
    }
}


bool HiVpssSource::isVpssSource() const
{
    return true;
}
