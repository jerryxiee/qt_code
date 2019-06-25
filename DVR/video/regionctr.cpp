#include "regionctr.h"
#include "QDebug"

RegionCtr::RegionCtr(QObject *parent) : QObject(parent)
{

}

HI_S32 RegionCtr::SAMPLE_RGN_CreateRegion(RGN_HANDLE Handle,const RGN_ATTR_S *pstRegion)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(Handle > RGN_HANDLE_MAX){
        printf("Handle exceeds size limit\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_RGN_Create(Handle, pstRegion);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_RGN_Create failed! s32Ret: 0x%x.\n", s32Ret);

    }

    return s32Ret;
}

HI_S32 RegionCtr::SAMPLE_RGN_CreateRegionForVpss(VPSS_GRP VpssGrp, VPSS_CHN VpssChn, RGN_HANDLE Handle, RGN_ATTR_S *pstRgnAttrSet, RGN_CHN_ATTR_S *pstChnAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stChn;

    if(Handle > RGN_HANDLE_MAX){
        printf("Handle exceeds size limit\n");
        return HI_FAILURE;
    }

//    mpixformat = pstRgnAttrSet->unAttr.stOverlay.enPixelFmt;
    stChn.enModId  = HI_ID_VPSS;
    switch (pstRgnAttrSet->enType) {
        case OVERLAY_RGN:
        case MOSAIC_RGN:
        {
            stChn.s32DevId = VpssGrp;
            stChn.s32ChnId = 0;
            break;
        }
        case COVER_RGN:
        {
            if(VpssChn < 2){
                stChn.s32DevId = VpssGrp;
                stChn.s32ChnId = 0;
            }else {
                stChn.s32DevId = VpssGrp;
                stChn.s32ChnId = VpssChn;
            }

            break;

        }
        default:
        {
            stChn.s32DevId = VpssGrp;
            stChn.s32ChnId = VpssChn;
        }

    }

    s32Ret = HI_MPI_RGN_Create(Handle, pstRgnAttrSet);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_RGN_Create failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_RGN_AttachToChn(Handle, &stChn, pstChnAttr);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_RGN_AttachToChn failed! s32Ret: 0x%x.\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 RegionCtr::SAMPLE_RGN_AttachToChn(RGN_HANDLE Handle, MPP_CHN_S *pstChn, RGN_CHN_ATTR_S *pstChnAttr)
{
    return HI_MPI_RGN_AttachToChn(Handle,pstChn,pstChnAttr);
}

HI_S32 RegionCtr::SAMPLE_RGN_DetachFromChn(RGN_HANDLE Handle,MPP_CHN_S *pstChn)
{
    return HI_MPI_RGN_DetachFromChn(Handle, pstChn);

}

HI_S32 RegionCtr::SAMPLE_RGN_OverlayAttachToChn(RGN_HANDLE Handle,VPSS_GRP VpssGrp,RGN_CHN_ATTR_S *pstChnAttr)
{
    MPP_CHN_S stChn;

    stChn.enModId = HI_ID_VPSS;
    stChn.s32DevId = VpssGrp;
    stChn.s32ChnId = 0;

    return HI_MPI_RGN_AttachToChn(Handle,&stChn,pstChnAttr);

}

HI_S32 RegionCtr::SAMPLE_RGN_OverlayDetechFromChn(RGN_HANDLE Handle,VPSS_GRP VpssGrp)
{
    MPP_CHN_S stChn;

    stChn.enModId = HI_ID_VPSS;
    stChn.s32DevId = VpssGrp;
    stChn.s32ChnId = 0;

    return HI_MPI_RGN_DetachFromChn(Handle, &stChn);
}

HI_S32 RegionCtr::SAMPLE_RGN_UpdateCanvasUseBmpFile(RGN_HANDLE Handle,const char *filename)
{
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_ATTR_S stRgnAttrSet;
    RGN_CANVAS_INFO_S stCanvasInfo;
    BITMAP_S stBitmap;
    SIZE_S stSize;


    s32Ret = HI_MPI_RGN_GetAttr(Handle, &stRgnAttrSet);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_RGN_GetCanvasInfo(Handle, &stCanvasInfo);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetCanvasInfo failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    stBitmap.pData   = (HI_VOID *)stCanvasInfo.u32VirtAddr;
    stSize.u32Width  = stCanvasInfo.stSize.u32Width;
    stSize.u32Height = stCanvasInfo.stSize.u32Height;
    s32Ret = SAMPLE_RGN_UpdateCanvas(filename, &stBitmap, HI_FALSE, 0, &stSize, stCanvasInfo.u32Stride,
    stRgnAttrSet.unAttr.stOverlay.enPixelFmt);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_RGN_UpdateCanvas(Handle);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);

    }

    return s32Ret;
}

HI_S32 RegionCtr::SAMPLE_RGN_UpdateCanvas(const char *filename, BITMAP_S *pstBitmap, HI_BOOL bFil,
    HI_U32 u16FilColor, SIZE_S *pstSize, HI_U32 u32Stride, PIXEL_FORMAT_E enPixelFmt)
{
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    if(GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0)
    {
        printf("GetBmpInfo err!\n");
        return HI_FAILURE;
    }

    if (PIXEL_FORMAT_RGB_1555 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;
    }
    else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB4444;
    }
    else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB8888;
    }
    else
    {
        printf("Pixel format is not support!\n");
        return HI_FAILURE;
    }

    if(NULL == pstBitmap->pData)
    {
        printf("malloc osd memroy err!\n");
        return HI_FAILURE;
    }
    CreateSurfaceByCanvas(filename, &Surface, (HI_U8*)(pstBitmap->pData), pstSize->u32Width, pstSize->u32Height, u32Stride);

    pstBitmap->u32Width  = Surface.u16Width;
    pstBitmap->u32Height = Surface.u16Height;

    if (PIXEL_FORMAT_RGB_1555 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;
    }
    else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_4444;
    }
    else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_8888;
    }

    int i,j;
    HI_U16 *pu16Temp;
    pu16Temp = (HI_U16*)pstBitmap->pData;

    if (bFil)
    {
        for (i=0; i<pstBitmap->u32Height; i++)
        {
            for (j=0; j<pstBitmap->u32Width; j++)
            {
                if (u16FilColor == *pu16Temp)
                {
                    *pu16Temp &= 0x7FFF;
                }
                pu16Temp++;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 RegionCtr::SAMPLE_RGN_DispTextInit(const char *fontname, int ptsize, SDL_Color &color, PIXEL_FORMAT_E pixformat)
{

    if (TTF_Init() < 0 )
    {
        fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());

        goto ERR_INIT;
    }

    mfont = TTF_OpenFont(fontname, ptsize);
    if ( mfont == nullptr )
    {
        fprintf(stderr, "Couldn't load %d pt font from %s: %s\n",ptsize,fontname, SDL_GetError());
        goto ERR_INIT;
    }

    msdl_pixfmt = new SDL_PixelFormat;
    if(!msdl_pixfmt){
        qDebug()<<"create SDL_PixelFormat failed!";
        goto ERR_INIT1;
    }
    memset(msdl_pixfmt,0x0,sizeof (SDL_PixelFormat));

    switch (pixformat) {
        case PIXEL_FORMAT_RGB_1555:
        {
            msdl_pixfmt->format = SDL_PIXELFORMAT_ARGB1555;
            msdl_pixfmt->BitsPerPixel = 16;
            msdl_pixfmt->BytesPerPixel = 2;
            msdl_pixfmt->Amask = 0x8000;
            msdl_pixfmt->Rmask = 0x7C00;
            msdl_pixfmt->Gmask = 0x03E0;
            msdl_pixfmt->Bmask = 0x001F;

            break;
        }
        case PIXEL_FORMAT_RGB_8888:
        {
            msdl_pixfmt->format = SDL_PIXELFORMAT_ARGB8888;
            msdl_pixfmt->BitsPerPixel = 32;
            msdl_pixfmt->BytesPerPixel = 4;
            msdl_pixfmt->Amask = 0xFF000000;
            msdl_pixfmt->Rmask = 0x00FF0000;
            msdl_pixfmt->Gmask = 0x0000FF00;
            msdl_pixfmt->Bmask = 0x000000FF;
            break;
        }
        default:{
            qDebug("format %#x not support!\n",mpixformat);
            goto ERR_INIT1;
        }

    }

    mforecol = color;
    mpixformat = pixformat;

    return HI_SUCCESS;

ERR_INIT1:
    TTF_CloseFont(mfont);
ERR_INIT:

    SDL_Quit();

    return HI_FAILURE;
}

SIZE_S &RegionCtr::SAMPLE_RGN_GetFontSize(const char *dispstr, SIZE_S &size) const
{
//    SIZE_S size = {0,0};
    SDL_Surface *text = nullptr;

    if(!mfont){
        qDebug()<<"font not Init!";
        return size;
    }

    text = TTF_RenderUTF8_Solid(mfont, dispstr, mforecol);
    if(text){
        size.u32Width = text->w;
        size.u32Height = text->h;
    }

    SDL_FreeSurface(text);

    return size;
}
void RegionCtr::SAMPLE_RGN_DispTextDeInit()
{

    if(msdl_pixfmt)
        delete msdl_pixfmt;

    TTF_CloseFont(mfont);
    SDL_Quit();

}

void RegionCtr::SAMPLE_RGN_SetTextColor(SDL_Color &color)
{
    mforecol = color;
}


HI_S32 RegionCtr::SAMPLE_RGN_DispTextToOverlay(RGN_HANDLE Handle,const char *dispstr)
{
    SDL_Surface *text = nullptr;
    SDL_Surface *temp = nullptr;
    HI_S32 s32Ret = HI_FAILURE;
    BITMAP_S stBitmap;

    if(!mfont){
        qDebug()<<"font not init!";
        goto END;
    }

    text = TTF_RenderUTF8_Solid(mfont, dispstr, mforecol);
    if(!text){
        qDebug()<<"TTF_RenderUTF8_Solid failed!";
        goto END;
    }

    temp = SDL_ConvertSurface(text,msdl_pixfmt,0);
    if(!temp){
        qDebug()<<"SDL_ConvertSurface failed!";
        goto END1;
    }

    stBitmap.u32Width = temp->w;
    stBitmap.u32Height = temp->h;
    stBitmap.enPixelFormat = mpixformat;
    stBitmap.pData = temp->pixels;

    s32Ret = HI_MPI_RGN_SetBitMap(Handle,&stBitmap);
    if(s32Ret != HI_SUCCESS){
        qDebug("HI_MPI_RGN_SetBitMap failed with %#x!\n",s32Ret);
    }
END1:
    SDL_FreeSurface(temp);
END:
    SDL_FreeSurface(text);


    return s32Ret;
}

HI_S32 RegionCtr::SAMPLE_RGN_SetOverlayPosToVpss(RGN_HANDLE Handle,VPSS_GRP VpssGrp,POINT_S &Point)
{
    MPP_CHN_S stChn;
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_CHN_ATTR_S stChnAttr;

    stChn.enModId = HI_ID_VPSS;
    stChn.s32DevId = VpssGrp;
    stChn.s32ChnId = 0;

    s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle,&stChn,&stChnAttr);
    if(s32Ret != HI_SUCCESS){
        qDebug("HI_MPI_RGN_GetDisplayAttr failed with %#x!",s32Ret);
        return s32Ret;
    }
//    qDebug()<<"back setRgnPoint:"<<Point.s32X<<":"<<Point.s32Y;

    if(stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X < 0)
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
    if(stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y < 0)
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;

    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = ALIGN_UP(Point.s32X,2);
    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = ALIGN_UP(Point.s32Y,2);

//    qDebug()<<"back setRgnPoint:"<<stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X<<":"<<stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y;

    s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle,&stChn,&stChnAttr);
    if(s32Ret != HI_SUCCESS){
        qDebug("HI_MPI_RGN_SetDisplayAttr failed with %#x!",s32Ret);

    }

    return s32Ret;

}

HI_S32 RegionCtr::SAMPLE_RGN_SetOverlayMovePos(RGN_HANDLE Handle, VPSS_GRP VpssGrp, POINT_S &Point)
{
    MPP_CHN_S stChn;
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_CHN_ATTR_S stChnAttr;

    stChn.enModId = HI_ID_VPSS;
    stChn.s32DevId = VpssGrp;
    stChn.s32ChnId = 0;

    s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle,&stChn,&stChnAttr);
    if(s32Ret != HI_SUCCESS){
        qDebug("HI_MPI_RGN_GetDisplayAttr failed with %#x!",s32Ret);
        return s32Ret;
    }

    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = ALIGN_UP(stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X+Point.s32X+Point.s32X,2);
    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = ALIGN_UP(stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y+Point.s32Y+Point.s32Y,2);

    if(stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X < 0)
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
    if(stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y < 0)
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;

    s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle,&stChn,&stChnAttr);
    if(s32Ret != HI_SUCCESS){
        qDebug("HI_MPI_RGN_SetDisplayAttr failed with %#x!",s32Ret);

    }

    return s32Ret;

}

HI_S32 RegionCtr::SAMPLE_RGN_ShowOverlay(RGN_HANDLE Handle, VPSS_GRP VpssGrp, HI_BOOL enable)
{
    MPP_CHN_S stChn;
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_CHN_ATTR_S stChnAttr;

    stChn.enModId = HI_ID_VPSS;
    stChn.s32DevId = VpssGrp;
    stChn.s32ChnId = 0;

    s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle,&stChn,&stChnAttr);
    if(s32Ret != HI_SUCCESS){
        qDebug("HI_MPI_RGN_GetDisplayAttr failed with %#x!",s32Ret);
        return s32Ret;
    }

    stChnAttr.bShow  = enable;
    s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle,&stChn,&stChnAttr);
    if(s32Ret != HI_SUCCESS){
        qDebug("HI_MPI_RGN_GetDisplayAttr failed with %#x!",s32Ret);

    }

    return s32Ret;

}
