#ifndef HIFRAMESOURCE_H
#define HIFRAMESOURCE_H

#include "hivideobase.h"

class HiFrameSource:public HiVideoBase
{
public:

    virtual bool isViSource() const;
    virtual bool isVpssSource() const;
    virtual bool isVoSource() const;
    virtual bool isVdecSource() const;


protected:
    HiFrameSource(MOD_ID_E Mod, int &DevId, int& Chn);
    virtual ~HiFrameSource();



private:
    virtual bool isSource() const;

};

#endif // HIFRAMESOURCE_H
