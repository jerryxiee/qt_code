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
    Sample_Common_Vo(VO_DEV VoDev,VO_LAYER VoLayer,SAMPLE_VO_MODE_E enVoMode = VO_MODE_9MUX);
    Sample_Common_Vo(const Sample_Common_Vo &);
    Sample_Common_Vo & operator = (const Sample_Common_Vo & Sample_Vo);

    HI_S32 SAMPLE_COMM_VO_SetDev(VO_DEV VoDev,VO_LAYER VoLayer,SAMPLE_VO_MODE_E enVoMode = VO_MODE_9MUX);
    HI_S32 SAMPLE_COMM_VO_StartDev(VO_PUB_ATTR_S *pstPubAttr);
    HI_S32 SAMPLE_COMM_VO_StopDev();
    HI_S32 SAMPLE_COMM_VO_SetChnFrameRate(int Chn, int rate);
    HI_S32 SAMPLE_COMM_VO_GetLayerAttr(VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);
    HI_S32 SAMPLE_COMM_VO_StartLayer(const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);
    HI_S32 SAMPLE_COMM_VO_StopLayer();
    HI_S32 SAMPLE_COMM_VO_StartChn(SAMPLE_VO_MODE_E enMode);
    HI_S32 SAMPLE_COMM_VO_StartChn(VO_CHN VoChn,RECT_S &pos);
//    HI_S32 SAMPLE_COMM_VO_StartChn(SAMPLE_VO_MODE_E enMode,VO_CHN StartChn);
    HI_S32 SAMPLE_COMM_VO_SetMode(SAMPLE_VO_MODE_E enMode);
    HI_S32 SAMPLE_COMM_VO_SetChnDispPos(VO_CHN VoChn, const POINT_S *pstDispPos);
    HI_S32 SAMPLE_COMM_VO_StopChn();
    HI_S32 SAMPLE_COMM_VO_StopChn(VO_CHN VoChn);
   // HI_S32 SAMPLE_COMM_Vpss_BindVpss(VPSS_GRP VpssDestGrp,VO_CHN VoChn,VPSS_GRP VpssSrcGrp,VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VO_BindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VO_UnBindVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VO_BindToVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VO_UnBindFromVpss(VO_CHN VoChn,VPSS_GRP VpssGrp,VPSS_CHN VpssChn);
    HI_S32 SAMPLE_COMM_VO_BindVi(VO_CHN VoChn, VI_CHN ViChn);
    HI_S32 SAMPLE_COMM_VO_UnBindVi(VO_CHN VoChn);
    HI_S32 SAMPLE_COMM_VO_HdmiStart(VO_INTF_SYNC_E enIntfSync);
    HI_S32 SAMPLE_COMM_VO_HdmiStop(HI_VOID);
    HI_S32 SAMPLE_COMM_VO_GetWH(VO_INTF_SYNC_E enIntfSync, HI_U32 *pu32W,HI_U32 *pu32H, HI_U32 *pu32Frm);
    HI_S32 SAMPLE_COMM_VO_MemConfig(HI_CHAR *pcMmzName);
    VO_DEV getVoDev() const ;
    VO_LAYER getVoLayer() const;
    SAMPLE_VO_MODE_E getVoMode() const;


private:
    HI_VOID SAMPLE_COMM_VO_HdmiConvertSync(VO_INTF_SYNC_E enIntfSync,HI_HDMI_VIDEO_FMT_E *penVideoFmt);

    static HI_VOID HDMI_HotPlug_Proc(HI_VOID *pPrivateData);
    static HI_VOID HDMI_UnPlug_Proc(HI_VOID *pPrivateData);
    static HI_VOID HDMI_EventCallBack(HI_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData);

private:
    VO_DEV m_VoDev;
    VO_LAYER m_VoLayer;
    SAMPLE_VO_MODE_E m_enVoMode;
    static HI_HDMI_CALLBACK_FUNC_S stCallbackFunc;
    static HDMI_ARGS_S      stHdmiArgs;


public:

};

#endif // SAMPLE_COMMON_VO_H
