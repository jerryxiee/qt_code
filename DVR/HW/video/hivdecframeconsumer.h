#ifndef HIVDECFRAMECONSUMER_H
#define HIVDECFRAMECONSUMER_H

#include "hiframeconsumer.h"
#include "common/sample_common_vdec.h"

class HiVdecFrameConsumer:public HiFrameConsumer
{
public:
    static HiVdecFrameConsumer *createNew(HiFrameSource *source,PAYLOAD_TYPE_E codeType,int h,int w);
    virtual ~HiVdecFrameConsumer();
    bool isCreateSucess() const;

protected:
    HiVdecFrameConsumer(HiFrameSource *source,PAYLOAD_TYPE_E codeType,int h,int w);

private:
    virtual bool isVdecConsumer() const;
    virtual void doProcess();

private:
    Sample_Common_Vdec *mVdec;
    bool mCreateSucess;
};

#endif // HIVDECFRAMECONSUMER_H
