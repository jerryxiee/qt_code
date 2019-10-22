#ifndef HIFRAMESOURCE_H
#define HIFRAMESOURCE_H

#include "hivideobase.h"

class HiFrameSource:public HiVideoBase
{
public:
    virtual ~HiFrameSource();
    virtual bool isViSource() const;
    virtual bool isVpssSource() const;
    virtual bool isVoSource() const;
    virtual bool isVdecSource() const;
    virtual bool isFileSource() const;

    virtual MOD_ID_E getHiModId() const;
    virtual int getDevNo() const;
    virtual int getChn() const;
protected:
    HiFrameSource(MOD_ID_E Mod, int &DevId, int& Chn);
    HiFrameSource(HiVideoBase* base);




private:
    virtual bool isSource() const;


};

#endif // HIFRAMESOURCE_H
