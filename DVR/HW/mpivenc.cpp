#include "mpivenc.h"
#include "common/sample_common_sys.h"

int MpiVenc::sMinCanUseIndex = 0;
uint32_t MpiVenc::sVencMask[4] = {0};

MpiVenc::MpiVenc()
{

}


int MpiVenc::applyOncCoder()
{
    int index = sMinCanUseIndex;

    int tabindex = index/32;
    int bit = index%32;
    uint mask = 0x00000001;

    do{
        if(!(sVencMask[tabindex] & (mask<<bit))){
            sVencMask[tabindex] |= mask<<bit;       //标记通道被使用
            sMinCanUseIndex = index+1;              //标记下一个可以使用通道
            return index;
        }

        index++;
        tabindex = index/32;
        bit = index&32;

    }while (index < VENC_MAX_CHN_NUM);

    return -1;
}

bool MpiVenc::releaseOneCoder(int Chn)
{
    int tabindex = Chn/32;
    int bit = Chn%32;
    uint mask = 0x00000001;

    sVencMask[tabindex] &= ~(mask << bit);
    if(sMinCanUseIndex > Chn){
        sMinCanUseIndex = Chn;
    }

    return true;
}

bool MpiVenc::startCoderChn(int Chn,PAYLOAD_TYPE_E enType, VIDEO_NORM_E enNorm, PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, uint u32BitRate, HI_FR32 frmRate, uint  u32Profile,uint framInterval)
{
    HI_S32 s32Ret;
    VENC_CHN_ATTR_S stVencChnAttr;
    VENC_ATTR_H264_S stH264Attr;
    VENC_ATTR_H264_CBR_S    stH264Cbr;
    VENC_ATTR_H264_VBR_S    stH264Vbr;
    VENC_ATTR_H264_AVBR_S    stH264AVbr;
    VENC_ATTR_H264_FIXQP_S  stH264FixQp;
    VENC_ATTR_H265_S        stH265Attr;
    VENC_ATTR_H265_CBR_S    stH265Cbr;
    VENC_ATTR_H265_VBR_S    stH265Vbr;
    VENC_ATTR_H265_AVBR_S    stH265AVbr;
    VENC_ATTR_H265_FIXQP_S  stH265FixQp;
    VENC_ATTR_MJPEG_S stMjpegAttr;
    VENC_ATTR_MJPEG_FIXQP_S stMjpegeFixQp;
    VENC_ATTR_JPEG_S stJpegAttr;
    SIZE_S stPicSize;
    s32Ret = Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(enNorm, enSize, &stPicSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Get picture size failed!\n");
        return false;
    }


//    int index = getVencIndex();
//    if(index < 0){
//        return HI_FAILURE;
//    }
//    m_Venc_MaxTab[index] = 1;
//    m_Venc_Chn = index;
    /******************************************
     step 1:  Create Venc Channel
     ******************************************/
    stVencChnAttr.stVeAttr.enType = enType;
    switch (enType)
    {
        case PT_H264:
        {
            stH264Attr.u32MaxPicWidth = stPicSize.u32Width;
            stH264Attr.u32MaxPicHeight = stPicSize.u32Height;
            stH264Attr.u32PicWidth = stPicSize.u32Width;/*the picture width*/
            stH264Attr.u32PicHeight = stPicSize.u32Height;/*the picture height*/
            stH264Attr.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
            stH264Attr.u32Profile  = u32Profile;/*0: baseline; 1:MP; 2:HP;  3:svc_t */
            stH264Attr.bByFrame = HI_TRUE;/*get stream mode is slice mode or frame mode?*/
            //stH264Attr.u32BFrameNum = 0;/* 0: not support B frame; >=1: number of B frames */
            //stH264Attr.u32RefNum = 1;/* 0: default; number of refrence frame*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrH264e, &stH264Attr, sizeof(VENC_ATTR_H264_S));
            if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
                stH264Cbr.u32Gop            = framInterval > 0?framInterval:(VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Cbr.u32StatTime       = 1; /* stream rate statics time(s) */
                stH264Cbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* input (vi) frame rate */
                stH264Cbr.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30; /* target frame rate */
                if(u32BitRate == 0){
                    switch (enSize)
                    {
                        case PIC_QCIF:
                            stH264Cbr.u32BitRate = 256; /* average bit rate */
                            break;
                        case PIC_QVGA:    /* 320 * 240 */
                        case PIC_CIF:
                            stH264Cbr.u32BitRate = 512;
                            break;
                        case PIC_D1:
                        case PIC_VGA:	   /* 640 * 480 */
                            stH264Cbr.u32BitRate = 1024 * 2;
                            break;
                        case PIC_HD720:   /* 1280 * 720 */
                            stH264Cbr.u32BitRate = 1024 * 2;
                            break;
                        case PIC_HD1080:  /* 1920 * 1080 */
                            stH264Cbr.u32BitRate = 1024 * 4;
                            break;
                        case PIC_UHD4K:
                            stH264Cbr.u32BitRate = 1024 * 8;
                            break;
                        default :
                            stH264Cbr.u32BitRate = 1024 * 4;
                            break;
                    }
                }else{
                    stH264Cbr.u32BitRate = u32BitRate;
                }
                stH264Cbr.u32FluctuateLevel = 1; /* average bit rate */
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264Cbr, &stH264Cbr, sizeof(VENC_ATTR_H264_CBR_S));
            }
            else if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;
                stH264FixQp.u32Gop = framInterval > 0?framInterval:(VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264FixQp.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264FixQp.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30;
                stH264FixQp.u32IQp = 20;
                stH264FixQp.u32PQp = 23;
                stH264FixQp.u32BQp = 23;
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264FixQp, &stH264FixQp, sizeof(VENC_ATTR_H264_FIXQP_S));
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
                stH264Vbr.u32Gop = framInterval > 0?framInterval:(VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Vbr.u32StatTime = 1;
                stH264Vbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264Vbr.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30;
                stH264Vbr.u32MinQp = 10;
                stH264Vbr.u32MinIQp = 10;
                stH264Vbr.u32MaxQp = 40;
                if(u32BitRate == 0){
                    switch (enSize)
                    {
                        case PIC_QCIF:
                            stH264Vbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                            break;
                        case PIC_QVGA:    /* 320 * 240 */
                        case PIC_CIF:
                            stH264Vbr.u32MaxBitRate = 512 * 3;
                            break;
                        case PIC_D1:
                        case PIC_VGA:	   /* 640 * 480 */
                            stH264Vbr.u32MaxBitRate = 1024 * 2;
                            break;
                        case PIC_HD720:   /* 1280 * 720 */
                            stH264Vbr.u32MaxBitRate = 1024 * 3;
                            break;
                        case PIC_HD1080:  /* 1920 * 1080 */
                            stH264Vbr.u32MaxBitRate = 1024 * 4;
                            break;
                        case PIC_UHD4K:
                            stH264Vbr.u32MaxBitRate = 1024 * 8;
                            break;
                        default :
                            stH264Vbr.u32MaxBitRate = 1024 * 4;
                            break;
                    }
                }else{
                    stH264Vbr.u32MaxBitRate = u32BitRate;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264Vbr, &stH264Vbr, sizeof(VENC_ATTR_H264_VBR_S));
            }
            else if (SAMPLE_RC_AVBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264AVBR;
                stH264AVbr.u32Gop = framInterval > 0?framInterval:(VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264AVbr.u32StatTime = 1;
                stH264AVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH264AVbr.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30;
                if(u32BitRate == 0){
                    switch (enSize)
                    {
                        case PIC_QCIF:
                            stH264AVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                            break;
                        case PIC_QVGA:    /* 320 * 240 */
                        case PIC_CIF:
                            stH264AVbr.u32MaxBitRate = 512 * 3;
                            break;
                        case PIC_D1:
                        case PIC_VGA:	   /* 640 * 480 */
                            stH264AVbr.u32MaxBitRate = 1024 * 2;
                            break;
                        case PIC_HD720:   /* 1280 * 720 */
                            stH264AVbr.u32MaxBitRate = 1024 * 3;
                            break;
                        case PIC_HD1080:  /* 1920 * 1080 */
                            stH264AVbr.u32MaxBitRate = 1024 * 4;
                            break;
                        case PIC_UHD4K:
                            stH264AVbr.u32MaxBitRate = 1024 * 8;
                            break;
                        default :
                            stH264AVbr.u32MaxBitRate = 1024 * 4;
                            break;
                    }
                }else{
                    stH264AVbr.u32MaxBitRate = u32BitRate;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264AVbr, &stH264AVbr, sizeof(VENC_ATTR_H264_AVBR_S));
            }
            else
            {
                return false;
            }
        }
        break;
        case PT_MJPEG:
        {
            stMjpegAttr.u32MaxPicWidth = stPicSize.u32Width;
            stMjpegAttr.u32MaxPicHeight = stPicSize.u32Height;
            stMjpegAttr.u32PicWidth = stPicSize.u32Width;
            stMjpegAttr.u32PicHeight = stPicSize.u32Height;
            stMjpegAttr.u32BufSize = stPicSize.u32Width * stPicSize.u32Height * 3;
            stMjpegAttr.bByFrame = HI_TRUE;  /*get stream mode is field mode  or frame mode*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrMjpege, &stMjpegAttr, sizeof(VENC_ATTR_MJPEG_S));
            if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGFIXQP;
                stMjpegeFixQp.u32Qfactor        = 90;
                stMjpegeFixQp.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stMjpegeFixQp.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30;
                memcpy(&stVencChnAttr.stRcAttr.stAttrMjpegeFixQp, &stMjpegeFixQp,
                       sizeof(VENC_ATTR_MJPEG_FIXQP_S));
            }
            else if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32StatTime       = 1;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32FluctuateLevel = 1;
                if(u32BitRate == 0){
                    switch (enSize)
                    {
                        case PIC_QCIF:
                            stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 384 * 3; /* average bit rate */
                            break;
                        case PIC_QVGA:    /* 320 * 240 */
                        case PIC_CIF:
                            stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 768 * 3;
                            break;
                        case PIC_D1:
                        case PIC_VGA:	   /* 640 * 480 */
                            stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 3 * 3;
                            break;
                        case PIC_HD720:   /* 1280 * 720 */
                            stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 5 * 3;
                            break;
                        case PIC_HD1080:  /* 1920 * 1080 */
                            stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 10 * 3;
                            break;
                        default :
                            stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = 1024 * 10 * 3;
                            break;
                    }
                }else{
                    stVencChnAttr.stRcAttr.stAttrMjpegeCbr.u32BitRate = u32BitRate;
                }
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGVBR;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32StatTime = 1;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.fr32DstFrmRate = 5;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MinQfactor = 50;
                stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxQfactor = 95;
                if(u32BitRate == 0){
                    switch (enSize)
                    {
                        case PIC_QCIF:
                            stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                            break;
                        case PIC_QVGA:    /* 320 * 240 */
                        case PIC_CIF:
                            stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 512 * 3;
                            break;
                        case PIC_D1:
                        case PIC_VGA:	   /* 640 * 480 */
                            stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 2 * 3;
                            break;
                        case PIC_HD720:   /* 1280 * 720 */
                            stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 3 * 3;
                            break;
                        case PIC_HD1080:  /* 1920 * 1080 */
                            stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 6 * 3;
                            break;
                        default :
                            stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = 1024 * 4 * 3;
                            break;
                    }
                }else{
                    stVencChnAttr.stRcAttr.stAttrMjpegeVbr.u32MaxBitRate = u32BitRate;
                }
            }
            else
            {
                SAMPLE_PRT("cann't support other mode in this version!\n");
                return false;
            }
        }
        break;
        case PT_JPEG:
            stJpegAttr.u32PicWidth  = stPicSize.u32Width;
            stJpegAttr.u32PicHeight = stPicSize.u32Height;
            stJpegAttr.u32MaxPicWidth  = stPicSize.u32Width;
            stJpegAttr.u32MaxPicHeight = stPicSize.u32Height;
            stJpegAttr.u32BufSize   = stPicSize.u32Width * stPicSize.u32Height * 3;
            stJpegAttr.bByFrame     = HI_TRUE;/*get stream mode is field mode  or frame mode*/
            stJpegAttr.bSupportDCF  = HI_FALSE;
            memcpy(&stVencChnAttr.stVeAttr.stAttrJpege, &stJpegAttr, sizeof(VENC_ATTR_JPEG_S));
            break;
        case PT_H265:
        {
            stH265Attr.u32MaxPicWidth = stPicSize.u32Width;
            stH265Attr.u32MaxPicHeight = stPicSize.u32Height;
            stH265Attr.u32PicWidth = stPicSize.u32Width;/*the picture width*/
            stH265Attr.u32PicHeight = stPicSize.u32Height;/*the picture height*/
            stH265Attr.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
            if (enRcMode >= 1)
            {
                stH265Attr.u32Profile = 0;
            }/*0:MP; */
            else
            {
                stH265Attr.u32Profile  = enRcMode;
            }/*0:MP*/
            stH265Attr.bByFrame = HI_TRUE;/*get stream mode is slice mode or frame mode?*/
            memcpy(&stVencChnAttr.stVeAttr.stAttrH265e, &stH265Attr, sizeof(VENC_ATTR_H265_S));
            if (SAMPLE_RC_CBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
                stH265Cbr.u32Gop            = framInterval > 0?framInterval:(VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Cbr.u32StatTime       = 1; /* stream rate statics time(s) */
                stH265Cbr.u32SrcFrmRate      = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30; /* input (vi) frame rate */
                stH265Cbr.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30; /* target frame rate */
                if(u32BitRate == 0){
                    switch (enSize)
                    {
                        case PIC_QCIF:
                            stH265Cbr.u32BitRate = 256; /* average bit rate */
                            break;
                        case PIC_QVGA:    /* 320 * 240 */
                        case PIC_CIF:
                            stH265Cbr.u32BitRate = 512;
                            break;
                        case PIC_D1:
                        case PIC_VGA:	   /* 640 * 480 */
                            stH265Cbr.u32BitRate = 1024 * 2;
                            break;
                        case PIC_HD720:   /* 1280 * 720 */
                            stH265Cbr.u32BitRate = 1024 * 3;
                            break;
                        case PIC_HD1080:  /* 1920 * 1080 */
                            stH265Cbr.u32BitRate = 1024 * 4;
                            break;
                        default :
                            stH265Cbr.u32BitRate = 1024 * 4;
                            break;
                    }
                }else{
                    stH265Cbr.u32BitRate = u32BitRate;
                }
                stH265Cbr.u32FluctuateLevel = 1; /* average bit rate */
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265Cbr, &stH265Cbr, sizeof(VENC_ATTR_H265_CBR_S));
            }
            else if (SAMPLE_RC_FIXQP == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265FIXQP;
                stH265FixQp.u32Gop = framInterval > 0?framInterval:(VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265FixQp.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265FixQp.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30;
                stH265FixQp.u32IQp = 20;
                stH265FixQp.u32PQp = 23;
                stH265FixQp.u32BQp = 25;
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265FixQp, &stH265FixQp, sizeof(VENC_ATTR_H265_FIXQP_S));
            }
            else if (SAMPLE_RC_VBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
                stH265Vbr.u32Gop = framInterval > 0?framInterval:(VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Vbr.u32StatTime = 1;
                stH265Vbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265Vbr.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30;
                stH265Vbr.u32MinQp  = 10;
                stH265Vbr.u32MinIQp = 10;
                stH265Vbr.u32MaxQp  = 40;
                if(u32BitRate == 0){
                    switch (enSize)
                    {
                        case PIC_QCIF:
                            stH265Vbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                            break;
                        case PIC_QVGA:    /* 320 * 240 */
                        case PIC_CIF:
                            stH265Vbr.u32MaxBitRate = 512 * 3;
                            break;
                        case PIC_D1:
                        case PIC_VGA:	   /* 640 * 480 */
                            stH265Vbr.u32MaxBitRate = 1024 * 2;
                            break;
                        case PIC_HD720:   /* 1280 * 720 */
                            stH265Vbr.u32MaxBitRate = 1024 * 3;
                            break;
                        case PIC_HD1080:  /* 1920 * 1080 */
                            stH265Vbr.u32MaxBitRate = 1024 * 6;
                            break;
                        default :
                            stH265Vbr.u32MaxBitRate = 1024 * 4;
                            break;
                    }
                }else{
                    stH265Vbr.u32MaxBitRate = u32BitRate;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH265Vbr, &stH265Vbr, sizeof(VENC_ATTR_H265_VBR_S));
            }
            else if (SAMPLE_RC_AVBR == enRcMode)
            {
                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265AVBR;
                stH265AVbr.u32Gop = framInterval > 0?framInterval:(VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265AVbr.u32StatTime = 1;
                stH265AVbr.u32SrcFrmRate = (VIDEO_ENCODING_MODE_PAL == enNorm) ? 25 : 30;
                stH265AVbr.fr32DstFrmRate = frmRate;//(VIDEO_ENCODING_MODE_PAL == m_enNorm) ? 25 : 30;
                if(u32BitRate == 0){
                    switch (enSize)
                    {
                        case PIC_QCIF:
                            stH265AVbr.u32MaxBitRate = 256 * 3; /* average bit rate */
                            break;
                        case PIC_QVGA:    /* 320 * 240 */
                        case PIC_CIF:
                            stH265AVbr.u32MaxBitRate = 512 * 3;
                            break;
                        case PIC_D1:
                        case PIC_VGA:	   /* 640 * 480 */
                            stH265AVbr.u32MaxBitRate = 1024 * 2;
                            break;
                        case PIC_HD720:   /* 1280 * 720 */
                            stH265AVbr.u32MaxBitRate = 1024 * 3;
                            break;
                        case PIC_HD1080:  /* 1920 * 1080 */
                            stH265AVbr.u32MaxBitRate = 1024 * 4;
                            break;
                        case PIC_UHD4K:
                            stH265AVbr.u32MaxBitRate = 1024 * 8;
                            break;
                        default :
                            stH265AVbr.u32MaxBitRate = 1024 * 4;
                            break;
                    }
                }else{
                    stH265AVbr.u32MaxBitRate = u32BitRate;
                }
                memcpy(&stVencChnAttr.stRcAttr.stAttrH264AVbr, &stH265AVbr, sizeof(VENC_ATTR_H265_AVBR_S));
            }
            else
            {
                return false;
            }
        }
        break;
        default:
            return false;
    }
    stVencChnAttr.stGopAttr.enGopMode  = VENC_GOPMODE_NORMALP;
    stVencChnAttr.stGopAttr.stNormalP.s32IPQpDelta = 0;
    s32Ret = HI_MPI_VENC_CreateChn(Chn, &stVencChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_CreateChn [%d] faild with %#x! ===\n", \
                   Chn, s32Ret);
        return false;
    }
    s32Ret = HI_MPI_VENC_StartRecvPic(Chn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VENC_StartRecvPic faild with%#x! \n", s32Ret);
        return false;
    }

//    SAMPLE_PRT("Chn(%d) VENC_Start Sucess! \n", m_Venc_Chn);
    return true;

}


//bool MpiVenc::SetChnAttr(int Chn,PIC_SIZE_E enSize, SAMPLE_RC_E enRcMode, HI_U32 u32BitRate, HI_U32  u32Profile, HI_FR32 frmRate)
//{
//    HI_S32 s32Ret;
//    VENC_CHN_ATTR_S stVencChnAttr;


//    SIZE_S stPicSize;

//    s32Ret = HI_MPI_VENC_StopRecvPic(Chn);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("HI_MPI_VENC_StopRecvPic vechn[%d] failed with %#x!\n", \
//                   Chn, s32Ret);
//        return HI_FAILURE;
//    }

//    s32Ret = HI_MPI_VENC_ResetChn(Chn);
//    if(s32Ret != HI_SUCCESS){
//        SAMPLE_PRT(" SAMPLE_COMM_VENC_StopRecv failed!\n");
//        goto END;
//    }

//    memset((void *)&stVencChnAttr,0x0,sizeof (VENC_CHN_ATTR_S));
//    s32Ret = HI_MPI_VENC_GetChnAttr(Chn, &stVencChnAttr);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT(" HI_MPI_VENC_GetChnAttr failed!\n");
//        return HI_FAILURE;
//    }
//    s32Ret = Sample_Common_Sys::SAMPLE_COMM_SYS_GetPicSize(enNorm, enSize, &stPicSize);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT("Get picture size failed!\n");
//        return HI_FAILURE;
//    }

//    switch (stVencChnAttr.stVeAttr.enType)
//    {
//        case PT_H264:
//        {
//            printf("set venc(h264) attr\n");
//            stVencChnAttr.stVeAttr.stAttrH264e.u32PicWidth = stPicSize.u32Width;/*the picture width*/
//            stVencChnAttr.stVeAttr.stAttrH264e.u32PicHeight = stPicSize.u32Height;/*the picture height*/
////            stVencChnAttr.stVeAttr.stAttrH264e.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
//            stVencChnAttr.stVeAttr.stAttrH264e.u32Profile  = u32Profile;/*0: baseline; 1:MP; 2:HP;  3:svc_t */

//            if (SAMPLE_RC_CBR == enRcMode)
//            {
//                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
//                if(frmRate != 0){
//                    stVencChnAttr.stRcAttr.stAttrH264Cbr.fr32DstFrmRate = frmRate;
//                }
//                if(u32BitRate != 0){
//                    stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
//                    stVencChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate = u32BitRate;
//                }else {
//                    switch (enSize)
//                    {
//                        case PIC_QCIF:
//                            stVencChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate = 256; /* average bit rate */
//                            break;
//                        case PIC_QVGA:    /* 320 * 240 */
//                        case PIC_CIF:
//                            stVencChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate = 512;
//                            break;
//                        case PIC_D1:
//                        case PIC_VGA:	   /* 640 * 480 */
//                            stVencChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate = 1024 * 2;
//                            break;
//                        default:   /* 1280 * 720 */
//                            stVencChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate = 1024 * 2;
//                            break;

//                    }
//                }


//            }
//            else if (SAMPLE_RC_FIXQP == enRcMode)
//            {
//                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;

//            }
//            else if (SAMPLE_RC_VBR == enRcMode)
//            {
//                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;

//                if(frmRate != 0){
//                    stVencChnAttr.stRcAttr.stAttrH264Vbr.fr32DstFrmRate = frmRate;
//                }

//                if(u32BitRate != 0){
//                    stVencChnAttr.stRcAttr.stAttrH264Vbr.u32MaxBitRate = u32BitRate;
//                }else {
//                    switch (enSize)
//                    {
//                        case PIC_QCIF:
//                            stVencChnAttr.stRcAttr.stAttrH264Vbr.u32MaxBitRate = 256; /* average bit rate */
//                            break;
//                        case PIC_QVGA:    /* 320 * 240 */
//                        case PIC_CIF:
//                            stVencChnAttr.stRcAttr.stAttrH264Vbr.u32MaxBitRate = 512;
//                            break;
//                        case PIC_D1:
//                        case PIC_VGA:	   /* 640 * 480 */
//                            stVencChnAttr.stRcAttr.stAttrH264Vbr.u32MaxBitRate = 1024 * 2;
//                            break;
//                        default:   /* 1280 * 720 */
//                            stVencChnAttr.stRcAttr.stAttrH264Vbr.u32MaxBitRate = 1024 * 2;
//                            break;

//                    }
//                }

//            }
//            else if (SAMPLE_RC_AVBR == enRcMode)
//            {
//                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264AVBR;

//                if(frmRate != 0){
//                    stVencChnAttr.stRcAttr.stAttrH264AVbr.fr32DstFrmRate = frmRate;
//                }

//                if(u32BitRate != 0){
//                    stVencChnAttr.stRcAttr.stAttrH264AVbr.u32MaxBitRate = u32BitRate;
//                }else {
//                    switch (enSize)
//                    {
//                        case PIC_QCIF:
//                            stVencChnAttr.stRcAttr.stAttrH264AVbr.u32MaxBitRate = 256; /* average bit rate */
//                            break;
//                        case PIC_QVGA:    /* 320 * 240 */
//                        case PIC_CIF:
//                            stVencChnAttr.stRcAttr.stAttrH264AVbr.u32MaxBitRate = 512;
//                            break;
//                        case PIC_D1:
//                        case PIC_VGA:	   /* 640 * 480 */
//                            stVencChnAttr.stRcAttr.stAttrH264AVbr.u32MaxBitRate = 1024 * 2;
//                            break;
//                        default:   /* 1280 * 720 */
//                            stVencChnAttr.stRcAttr.stAttrH264AVbr.u32MaxBitRate = 1024 * 2;
//                            break;

//                    }
//                }


//            }
//            else
//            {
//                return HI_FAILURE;
//            }
//            m_enRcMode = enRcMode;
//        }
//        break;
//        case PT_H265:
//        {
//            stVencChnAttr.stVeAttr.stAttrH265e.u32PicWidth = stPicSize.u32Width;/*the picture width*/
//            stVencChnAttr.stVeAttr.stAttrH265e.u32PicHeight = stPicSize.u32Height;/*the picture height*/
////            stVencChnAttr.stVeAttr.stAttrH265e.u32BufSize  = stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
//            stVencChnAttr.stVeAttr.stAttrH265e.u32Profile = 0;


//            if (SAMPLE_RC_CBR == enRcMode)
//            {
//                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;

//                if(u32BitRate != 0){
//                    stVencChnAttr.stRcAttr.stAttrH265Cbr.u32BitRate = u32BitRate;
//                }

//            }
//            else if (SAMPLE_RC_FIXQP == enRcMode)
//            {
//                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265FIXQP;

//            }
//            else if (SAMPLE_RC_VBR == enRcMode)
//            {
//                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
//                if(u32BitRate != 0){
//                    stVencChnAttr.stRcAttr.stAttrH265Vbr.u32MaxBitRate = u32BitRate;
//                }
//            }
//            else if (SAMPLE_RC_AVBR == enRcMode)
//            {
//                stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265AVBR;

//                if(u32BitRate != 0){
//                    stVencChnAttr.stRcAttr.stAttrH264AVbr.u32MaxBitRate = u32BitRate;
//                }
//            }
//            else
//            {
//                return HI_FAILURE;
//            }
//        }
//        break;
//        default:
//            return HI_ERR_VENC_NOT_SUPPORT;
//    }

//    s32Ret = HI_MPI_VENC_SetChnAttr(m_Venc_Chn, &stVencChnAttr);
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT(" HI_MPI_VENC_GetChnAttr failed with(%#x)!\n",s32Ret);
//        return HI_FAILURE;
//    }

//END:
//    s32Ret = SAMPLE_COMM_VENC_StartRecv();
//    if (HI_SUCCESS != s32Ret)
//    {
//        SAMPLE_PRT(" SAMPLE_COMM_VENC_StartRecv failed with(%#x)!\n",s32Ret);
//        return HI_FAILURE;
//    }


//    return HI_SUCCESS;
//}
