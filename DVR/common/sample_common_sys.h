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


public:
    static VO_PUB_ATTR_S m_stPubAttr;
    HI_S32 Sys_init_Sucess;

private:
    const struct fb_bitfield s_a32 = {24,8,0};
    const struct fb_bitfield s_r32 = {16,8,0};
    const struct fb_bitfield s_g32 = {8,8,0};
    const struct fb_bitfield s_b32 = {0,8,0};
    const HI_CHAR *CONFIG_PATH = "/usr/dvr_config/";

    static VO_DEV m_FbVoDev;
    static VO_LAYER m_FbVoLayer;
    HI_S32 m_sys_hifb_fd;
    Sample_Common_Vio m_Sys_Vo;

signals:

public slots:
};

#endif // SAMPLE_COMMON_SYS_H
