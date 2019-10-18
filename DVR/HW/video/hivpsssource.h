#ifndef HIVPSSSOURCE_H
#define HIVPSSSOURCE_H

#include "hiframesource.h"
#include "common/sample_common_vpss.h"

class HiVpssSource : public HiFrameSource
{
public:
    HiVpssSource *createNew(HiFrameSource & inSource);
    HiVpssSource *createNew(HiFrameSource & inSource,int DevId,int Chn);      //vpss通道已经创建


protected:
    HiVpssSource(HiFrameSource & inSource,int &DevId,int &Chn,MOD_ID_E Mod = HI_ID_VPSS);
    virtual ~HiVpssSource();

private:
    virtual bool isVpssSource() const;

private:
    HiFrameSource &mInSource;


};

#endif // HIVPSSSOURCE_H
