#ifndef SAMPLE_COMMON_VO_H
#define SAMPLE_COMMON_VO_H

#include "sample_comm.h"

#define CHECK_POINTER_NO_RET(p) \
do{                      \
    if(HI_NULL == p){\
        printf("The pointer is null\n");       \
        return; \
    } \
}while(0)


#define CHECK_RET_SUCCESS_NO_RET(val) \
do{                      \
    if(HI_SUCCESS != val){\
        printf("return value is not HI_SUCCESS, 0x%x!\n", val);       \
        return ; \
    } \
}while(0)


typedef struct hiHDMI_ARGS_S
{
    HI_HDMI_ID_E  enHdmi;
}HDMI_ARGS_S;

class Sample_Common_Vo
{
public:
    Sample_Common_Vo();
    Sample_Common_Vo(Sample_Common_Vo &);
    Sample_Common_Vo & operator = (const Sample_Common_Vo & Sample_Vo);


    HI_S32 SAMPLE_COMM_VO_StartDev(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr);
    HI_S32 SAMPLE_COMM_VO_StopDev(VO_DEV VoDev);
    HI_S32 SAMPLE_COMM_VO_StartLayer(VO_LAYER VoLayer,const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);
    HI_S32 SAMPLE_COMM_VO_StopLayer(VO_LAYER VoLayer);
    HI_S32 SAMPLE_COMM_VO_StartChn(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode);
    HI_S32 SAMPLE_COMM_VO_StopChn(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode);
   // HI_S32 SAMPLE_COMM_Vpss_BindVpss(VPSS_GRP VpssDestGrp,VO_CHN VoChn,VPSS_GRP VpssSrcGrp,VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VO_BindVpss(VO_LAYER VoLayer,VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VO_UnBindVpss(VO_LAYER VoLayer,VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VO_BindVi(VO_LAYER VoLayer, VO_CHN VoChn, VI_CHN ViChn);
    HI_S32 SAMPLE_COMM_VO_UnBindVi(VO_LAYER VoLayer, VO_CHN VoChn);
    HI_S32 SAMPLE_COMM_VO_HdmiStart(VO_INTF_SYNC_E enIntfSync);
    HI_S32 SAMPLE_COMM_VO_HdmiStop(HI_VOID);
    HI_S32 SAMPLE_COMM_VO_GetWH(VO_INTF_SYNC_E enIntfSync, HI_U32 *pu32W,HI_U32 *pu32H, HI_U32 *pu32Frm);
    HI_S32 SAMPLE_COMM_VO_MemConfig(VO_DEV VoDev, HI_CHAR *pcMmzName);



private:
    HI_VOID SAMPLE_COMM_VO_HdmiConvertSync(VO_INTF_SYNC_E enIntfSync,HI_HDMI_VIDEO_FMT_E *penVideoFmt);

    static HI_VOID HDMI_HotPlug_Proc(HI_VOID *pPrivateData);
    static HI_VOID HDMI_UnPlug_Proc(HI_VOID *pPrivateData);
    static HI_VOID HDMI_EventCallBack(HI_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData);

private:
    static HI_HDMI_CALLBACK_FUNC_S stCallbackFunc;
    static HDMI_ARGS_S      stHdmiArgs;


public:
//    HI_U32 m_u32PicWidth;
//    HI_U32 m_u32PicHeight;
//    VO_DEV m_VoDev;
//    VO_CHN m_VoChn;
//    VPSS_GRP m_VpssDestGrp;
//    VPSS_CHN m_VpssChn;
//    VO_LAYER m_VoLayer;
//    VO_PUB_ATTR_S m_stPubAttr;
//    VO_VIDEO_LAYER_ATTR_S m_stLayerAttr;

};

#endif // SAMPLE_COMMON_VO_H
