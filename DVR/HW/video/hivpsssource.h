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
    HiVpssSource(int DevId, int Chn,int ChnNum = 0,SIZE_S *pstSize = nullptr,VPSS_GRP_ATTR_S *pstVpssGrpAttr = nullptr, MOD_ID_E Mod = HI_ID_VPSS);

private:

    bool isCreateSucess() const;
    virtual bool isVpssSource() const;

private:
    Sample_Common_Vpss *mVpss;
    bool mCreateSucess;


};

#endif // HIVPSSSOURCE_H
