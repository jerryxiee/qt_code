#ifndef REGIONCTR_H
#define REGIONCTR_H

#include <QObject>
#include "common/sample_comm.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class RegionCtr : public QObject
{
    Q_OBJECT
public:
    explicit RegionCtr(QObject *parent = nullptr);

    HI_S32 SAMPLE_RGN_CreateRegion(RGN_HANDLE Handle,const RGN_ATTR_S *pstRegion);
    HI_S32 SAMPLE_RGN_CreateRegionForVpss(VPSS_GRP VpssGrp,VPSS_CHN VpssChn, RGN_HANDLE Handle, RGN_ATTR_S *pstRgnAttrSet, RGN_CHN_ATTR_S *pstChnAttr);
    HI_S32 SAMPLE_RGN_OverlayAttachToChn(RGN_HANDLE Handle,VPSS_GRP VpssGrp,RGN_CHN_ATTR_S *pstChnAttr);
    HI_S32 SAMPLE_RGN_OverlayDetechFromChn(RGN_HANDLE Handle,VPSS_GRP VpssGrp);
    HI_S32 SAMPLE_RGN_UpdateCanvasUseBmpFile(RGN_HANDLE Handle,const char *filename);
    HI_S32 SAMPLE_RGN_DispTextToOverlay(RGN_HANDLE Handle,const char *dispstr);
    HI_S32 SAMPLE_RGN_DispTextInit(const char *fontname,int ptsize,SDL_Color &color,PIXEL_FORMAT_E pixformat);
    void SAMPLE_RGN_DispTextDeInit();
    SIZE_S &SAMPLE_RGN_GetFontSize(const char *dispstr, SIZE_S &size) const;
    void SAMPLE_RGN_SetTextColor(SDL_Color &color);
    HI_S32 SAMPLE_RGN_ShowOverlay(RGN_HANDLE Handle,VPSS_GRP VpssGrp,HI_BOOL enable);
    HI_S32 SAMPLE_RGN_SetOverlayPosToVpss(RGN_HANDLE Handle, VPSS_GRP VpssGrp, POINT_S &Point);
    HI_S32 SAMPLE_RGN_SetOverlayMovePos(RGN_HANDLE Handle, VPSS_GRP VpssGrp, POINT_S &Point);

private:
    HI_S32 SAMPLE_RGN_AttachToChn(RGN_HANDLE Handle,MPP_CHN_S *pstChn,RGN_CHN_ATTR_S *pstChnAttr);
    HI_S32 SAMPLE_RGN_DetachFromChn(RGN_HANDLE Handle,MPP_CHN_S *pstChn);
    HI_S32 SAMPLE_RGN_UpdateCanvas(const char *filename, BITMAP_S *pstBitmap, HI_BOOL bFil,
        HI_U32 u16FilColor, SIZE_S *pstSize, HI_U32 u32Stride, PIXEL_FORMAT_E enPixelFmt);

//protected:
//    void run();

signals:

public slots:

private:
    PIXEL_FORMAT_E mpixformat;
    TTF_Font *mfont = nullptr;
    SDL_PixelFormat *msdl_pixfmt = nullptr;
    SDL_Color mforecol;
};

#endif // REGIONCTR_H
