#ifndef VENC_H
#define VENC_H

#include "vedc/vio.h"
#include "common/sample_common_venc.h"

class venc
{
public:
    venc();
    venc(VENC_CHN VencChnCnt);
    venc(Vio &vi);


};

#endif // VENC_H
