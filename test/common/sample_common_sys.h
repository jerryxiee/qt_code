#ifndef SAMPLE_COMMON_SYS_H
#define SAMPLE_COMMON_SYS_H

#include <QObject>
#include "sample_comm.h"
#include "common/sample_common_vio.h"
#include "common/sample_common_vdec.h"
#include "sample_common_venc.h"
#include "hifb.h"

class Sample_Common_Sys : public QObject
{
    Q_OBJECT
public:
    explicit Sample_Common_Sys(QObject *parent = nullptr);
    ~Sample_Common_Sys();

    HI_S32 SAMPLE_SYS_SetReg(HI_U32 u32Addr, HI_U32 u32Value);
    HI_S32 SAMPLE_SYS_GetReg(HI_U32 u32Addr, HI_U32 *pu32Value);

    HI_S32 SAMPLE_COMM_SYS_MemConfig(HI_VOID);
    HI_S32 SAMPLE_COMM_SYS_Init(VB_CONF_S *pstVbConf);
    HI_S32 SAMPLE_COMM_SYS_Payload2FilePostfix(PAYLOAD_TYPE_E enPayload, HI_CHAR* szFilePostfix);
    static HI_VOID SAMPLE_COMM_SYS_Exit(void);
//    HI_BOOL SYS_INIT_ISSUCESS();

    static HI_S32 SAMPLE_COMM_SYS_GetPicSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstSize);
    static HI_U32 SAMPLE_COMM_SYS_CalcHistVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, SIZE_S *pstHistBlkSize, HI_U32 u32AlignWidth);
    static HI_U32 SAMPLE_COMM_SYS_CalcPicVbBlkSize(VIDEO_NORM_E enNorm, PIC_SIZE_E enPicSize, PIXEL_FORMAT_E enPixFmt, HI_U32 u32AlignWidth,COMPRESS_MODE_E enCompFmt);
    static HI_S32 SAMPLE_COMM_BindVpss(MPP_CHN_S *pstSrcChn, MPP_CHN_S *pstDestChn);

    //test
//    HI_S32 VdecTest();
//    HI_S32 Vio_8_1080P_Test();
//    HI_S32 Vio_Venc_Test();
//    Q_INVOKABLE void test(){qDebug("test\n");}
//    Q_INVOKABLE void Enable_Alpha();
//    Q_INVOKABLE void DisEnable_Alpha();
//    Q_INVOKABLE void Set_ColorKey();

//    void m_exit();
//    HI_S32 SAMPLE_VDEC_H264(HI_VOID);


public:
//    HI_U32 m_u32PicWidth;
//    HI_U32 m_u32PicHeight;
//    VO_CHN m_FbVoChn;
//    VPSS_GRP m_VpssDestGrp;
//    VPSS_CHN m_VpssChn;
    //VO_LAYER m_FbVoLayer;
//    VO_PUB_ATTR_S m_stFbPubAttr;
    static VO_PUB_ATTR_S m_stPubAttr;
//    VO_VIDEO_LAYER_ATTR_S m_stFbLayerAttr;
    HI_S32 Sys_init_Sucess;

private:
    const struct fb_bitfield s_a32 = {24,8,0};
    const struct fb_bitfield s_r32 = {16,8,0};
    const struct fb_bitfield s_g32 = {8,8,0};
    const struct fb_bitfield s_b32 = {0,8,0};

    static VO_DEV m_FbVoDev;
    static VO_LAYER m_FbVoLayer;
    HI_S32 m_sys_hifb_fd;
    Sample_Common_Vio m_Sys_Vo;
//    Sample_Common_Vdec *m_Sys_Vdec;
//    Sample_Common_Venc *m_Sys_Venc[VENC_MAX_CHN_NUM];
//    Sample_Common_Vpss *m_pVpss[VPSS_MAX_GRP_NUM];


signals:

public slots:
};

#endif // SAMPLE_COMMON_SYS_H
