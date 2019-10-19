#ifndef HIVPSSSOURCE_H
#define HIVPSSSOURCE_H

#include "hiframesource.h"
#include "common/sample_common_vpss.h"

class HiVpssSource : public HiFrameSource
{
public:
    static HiVpssSource *createNew(int ChnNum,SIZE_S *pstSize,VPSS_GRP_ATTR_S *pstVpssGrpAttr);
    static HiVpssSource *createNew(int DevId, int Chn);      //vpss通道已经创建
    virtual ~HiVpssSource();

protected:
    HiVpssSource(Sample_Common_Vpss *vpss,int DevId,int Chn,MOD_ID_E Mod = HI_ID_VPSS);

private:

    virtual bool isVpssSource() const;

private:
    Sample_Common_Vpss *mVpss;


};

#endif // HIVPSSSOURCE_H
