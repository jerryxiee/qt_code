#include "videodetect.h"
#include <QDebug>
#include <QDateTime>

VideoDetect::VideoDetect(QObject *parent) : QThread(parent)
{

}

void VideoDetect::VideoDetectExit()
{
    mdetect_run = false;
    wait();

}

void VideoDetect::addVdaToList(VDACHNATTR &value)
{
    mVdaListMutex.lock();
    mVdaChnList.append(value);
    mVdaListMutex.unlock();

}

void VideoDetect::removeVdaFromList(int index)
{
    mVdaListMutex.lock();
    mVdaChnList.removeAt(index);
    mVdaListMutex.unlock();
}

int VideoDetect::checkVdaChn(VDA_CHN VdaChn)
{
    for (int i = 0;i < mVdaChnList.count();i++) {
        if(mVdaChnList.at(i).VdaChn == VdaChn){
            return i;
        }
    }

    return -1;
}

HI_S32 VideoDetect::Vda_CreateChn(VDA_CHN VdaChn,HI_U32 u32SadTh,HI_U32 u32VdaIntvl)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VDA_CHN_ATTR_S stVdaChnAttr;

    if(checkVdaChn(VdaChn) >= 0){
        qDebug("channel %d has been created",VdaChn);
        return HI_FAILURE;
    }

    stVdaChnAttr.enWorkMode = VDA_WORK_MODE_MD;
    stVdaChnAttr.u32Width   = 720;
    stVdaChnAttr.u32Height  = 576;

    stVdaChnAttr.unAttr.stMdAttr.enVdaAlg      = VDA_ALG_REF;
    stVdaChnAttr.unAttr.stMdAttr.enMbSize      = VDA_MB_16PIXEL;
    stVdaChnAttr.unAttr.stMdAttr.enMbSadBits   = VDA_MB_SAD_8BIT;
    stVdaChnAttr.unAttr.stMdAttr.enRefMode     = VDA_REF_MODE_DYNAMIC;
    stVdaChnAttr.unAttr.stMdAttr.u32MdBufNum   = 8;
    stVdaChnAttr.unAttr.stMdAttr.u32VdaIntvl   = u32VdaIntvl;
    stVdaChnAttr.unAttr.stMdAttr.u32BgUpSrcWgt = 128;
    stVdaChnAttr.unAttr.stMdAttr.u32SadTh      = u32SadTh;
    stVdaChnAttr.unAttr.stMdAttr.u32ObjNumMax  = 128;

    s32Ret = HI_MPI_VDA_CreateChn(VdaChn, &stVdaChnAttr);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VDA_CreateChn failed with %#X!\n",s32Ret);
        return s32Ret;
    }


    return s32Ret;
}

HI_S32 VideoDetect::createMoveDetect(VDA_CHN VdaChn, VI_CHN Chn, HI_U32 u32SadTh, HI_U32 u32VdaIntvl)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn, stDestChn;
    VDACHNATTR stVdaChn;

    s32Ret = Vda_CreateChn(VdaChn,u32SadTh,u32VdaIntvl);
    if(s32Ret != HI_SUCCESS){
        SAMPLE_PRT("Vda_CreateChn failed with %#X!\n",s32Ret);
        return s32Ret;
    }

    stSrcChn.enModId = HI_ID_VIU;
    stSrcChn.s32ChnId = Chn;
    stSrcChn.s32DevId = 0;

    stDestChn.enModId = HI_ID_VDA;
    stDestChn.s32ChnId = VdaChn;
    stDestChn.s32DevId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("err!\n");
        goto END1;
    }

//    s32Ret = HI_MPI_VDA_StartRecvPic(VdaChn);
//    if(s32Ret != HI_SUCCESS)
//    {
//        SAMPLE_PRT("err!\n");
//        goto END1;
//    }

    stVdaChn.stSrcChn = stSrcChn;
    stVdaChn.VdaChn = VdaChn;
    stVdaChn.enable = HI_FALSE;
    stVdaChn.VdaFd = HI_MPI_VDA_GetFd(VdaChn);
    if(stVdaChn.VdaFd > maxfd){
        maxfd = stVdaChn.VdaFd;
    }
    addVdaToList(stVdaChn);

//    start();

    return s32Ret;

END1:
    HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
END:
    HI_MPI_VDA_DestroyChn(VdaChn);

    return s32Ret;
}

HI_S32 VideoDetect::createMoveDetect(VDA_CHN VdaChn,VPSS_GRP Grp,VPSS_CHN VpssChn,HI_U32 u32SadTh,HI_U32 u32VdaIntvl)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn, stDestChn;
    VDACHNATTR stVdaChn;

    s32Ret = Vda_CreateChn(VdaChn,u32SadTh,u32VdaIntvl);
    if(s32Ret != HI_SUCCESS){
        SAMPLE_PRT("Vda_CreateChn failed with %#X!\n",s32Ret);
        return s32Ret;
    }

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32ChnId = VpssChn;
    stSrcChn.s32DevId = Grp;

    stDestChn.enModId = HI_ID_VDA;
    stDestChn.s32ChnId = VdaChn;
    stDestChn.s32DevId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("err!\n");
        goto END1;
    }

//    s32Ret = HI_MPI_VDA_StartRecvPic(VdaChn);
//    if(s32Ret != HI_SUCCESS)
//    {
//        SAMPLE_PRT("err!\n");
//        goto END1;
//    }

    stVdaChn.stSrcChn = stSrcChn;
    stVdaChn.VdaChn = VdaChn;
    stVdaChn.enable = HI_FALSE;
    stVdaChn.VdaFd = HI_MPI_VDA_GetFd(VdaChn);
    if(stVdaChn.VdaFd > maxfd){
        maxfd = stVdaChn.VdaFd;
    }
    addVdaToList(stVdaChn);

//    start();

    return s32Ret;

END1:
    HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
END:
    HI_MPI_VDA_DestroyChn(VdaChn);

    return s32Ret;

}

HI_S32 VideoDetect::VdaStart(VDA_CHN VdaChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    int index;

    index = checkVdaChn(VdaChn);
    if(index < 0){
        qDebug("channel %d not in list!",VdaChn);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VDA_StartRecvPic(VdaChn);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VDA_StartRecvPic failed with %#X!\n",s32Ret);
        return s32Ret;
    }
    mVdaChnList[index].enable = HI_TRUE;
    start();

    return s32Ret;

}

HI_S32 VideoDetect::VdaStop(VDA_CHN VdaChn)
{
    int index;

    index = checkVdaChn(VdaChn);
    if(index < 0){
        qDebug("channel %d not in list!",VdaChn);
        return HI_FAILURE;
    }

    mVdaChnList[index].enable = HI_FALSE;
    return HI_MPI_VDA_StopRecvPic(VdaChn);

}

void VideoDetect::onViStatusChangedSlot(VI_CHN index ,HI_BOOL enable)
{
    qDebug()<<"vda status change"<<index<<" "<<enable;
    if(enable){
        VdaStart(mVdaChnList[index].VdaChn);
    }else {
        VdaStop(mVdaChnList[index].VdaChn);
    }
}

HI_S32 VideoDetect::distroyVdaChn(VDA_CHN VdaChn)
{
    int index;
    MPP_CHN_S stDestChn;

    index = checkVdaChn(VdaChn);
    if(index < 0){
        qDebug("channel %d not in list!",VdaChn);
        return HI_FAILURE;
    }

    HI_MPI_VDA_StopRecvPic(VdaChn);

    stDestChn.enModId = HI_ID_VDA;
    stDestChn.s32ChnId = VdaChn;
    stDestChn.s32DevId = 0;

    HI_MPI_SYS_UnBind(&mVdaChnList[index].stSrcChn, &stDestChn);

    HI_MPI_VDA_DestroyChn(VdaChn);

    mVdaListMutex.lock();
    mVdaChnList.removeAt(index);
    mVdaListMutex.unlock();

    return HI_SUCCESS;

}

HI_S32 VideoDetect::SAMPLE_COMM_VDA_MdPrtObj(FILE *fp, VDA_DATA_S *pstVdaData)
{
    VDA_OBJ_S *pstVdaObj;
    HI_S32 i;

    fprintf(fp, "===== %s =====\n", __FUNCTION__);

    if (HI_TRUE != pstVdaData->unData.stMdData.bObjValid)
    {
        fprintf(fp, "bMbObjValid = FALSE.\n");
        return HI_SUCCESS;
    }

    fprintf(fp, "ObjNum=%d, IndexOfMaxObj=%d, SizeOfMaxObj=%d, SizeOfTotalObj=%d\n", \
                   pstVdaData->unData.stMdData.stObjData.u32ObjNum, \
             pstVdaData->unData.stMdData.stObjData.u32IndexOfMaxObj, \
             pstVdaData->unData.stMdData.stObjData.u32SizeOfMaxObj,\
             pstVdaData->unData.stMdData.stObjData.u32SizeOfTotalObj);
    for (i=0; i<pstVdaData->unData.stMdData.stObjData.u32ObjNum; i++)
    {
        pstVdaObj = pstVdaData->unData.stMdData.stObjData.pstAddr + i;
        fprintf(fp, "[%d]\t left=%d, top=%d, right=%d, bottom=%d\n", i, \
              pstVdaObj->u16Left, pstVdaObj->u16Top, \
              pstVdaObj->u16Right, pstVdaObj->u16Bottom);
    }
    fflush(fp);
    return HI_SUCCESS;
}

HI_S32 VideoDetect::SAMPLE_COMM_VDA_MdPrtSad(FILE *fp, VDA_DATA_S *pstVdaData)
{
    HI_S32 i, j;
    HI_VOID *pAddr;

    fprintf(fp, "===== %s =====\n", __FUNCTION__);
    if (HI_TRUE != pstVdaData->unData.stMdData.bMbSadValid)
    {
        fprintf(fp, "bMbSadValid = FALSE.\n");
        return HI_SUCCESS;
    }

    for(i=0; i<pstVdaData->u32MbHeight; i++)
    {
        pAddr = (HI_VOID *)((HI_U32)pstVdaData->unData.stMdData.stMbSadData.pAddr
                + i * pstVdaData->unData.stMdData.stMbSadData.u32Stride);

        for(j=0; j<pstVdaData->u32MbWidth; j++)
        {
            HI_U8  *pu8Addr;
            HI_U16 *pu16Addr;

            if(VDA_MB_SAD_8BIT == pstVdaData->unData.stMdData.stMbSadData.enMbSadBits)
            {
                pu8Addr = (HI_U8 *)pAddr + j;

                fprintf(fp, "%-4d ",*pu8Addr);

            }
            else
            {
                pu16Addr = (HI_U16 *)pAddr + j;

                fprintf(fp, "%-6d ",*pu16Addr);
            }
        }

//        printf("\n");
    }

    fflush(fp);
    return HI_SUCCESS;
}

bool VideoDetect::VideoMoveDetectProcess(VDA_DATA_S *pstVdaData)
{

    VDA_OBJ_S *pstVdaObj;
    HI_S32 i;

    if (HI_TRUE != pstVdaData->unData.stMdData.bObjValid)
    {
        return false;
    }

    if(pstVdaData->unData.stMdData.stObjData.u32ObjNum > 0
            && pstVdaData->unData.stMdData.stObjData.u32SizeOfMaxObj > 5000)
        return true;

//    pstVdaObj = pstVdaData->unData.stMdData.stObjData.pstAddr + \
//            pstVdaData->unData.stMdData.stObjData.u32IndexOfMaxObj;

    return false;
}

void VideoDetect::run()
{
    int i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    fd_set read_fds;
    struct timeval TimeoutVal;
    VDA_DATA_S stVdaData;
    bool ignore_flag[8] = {false};
    bool premovestart[8] = {false};
    bool movestart[8] = {false};
    FILE *fp = fopen("moveobj","wb");
    FILE *fp1 = fopen("movesad","wb");
    HI_U32 movestarttime,moveendtime;

    qDebug()<<"maxfd:"<<maxfd;
    mdetect_run = true;
#if 1
    while (mdetect_run) {

        FD_ZERO(&read_fds);
        mVdaListMutex.lock();
        if(mVdaChnList.count() == 0){
            mVdaListMutex.unlock();
            usleep(10000);
            continue;
        }

        for(i = 0; i < mVdaChnList.count(); i++){
            if(mVdaChnList[i].enable)
                FD_SET(mVdaChnList[i].VdaFd, &read_fds);
        }
        TimeoutVal.tv_sec  = 1;
        TimeoutVal.tv_usec = 0;

        s32Ret = select(maxfd + 1, &read_fds, nullptr, nullptr, &TimeoutVal);
        if (s32Ret < 0)
        {
            SAMPLE_PRT("select failed!\n");
            mVdaListMutex.unlock();
            break;
        }
        else if (s32Ret == 0)
        {
            SAMPLE_PRT("get vda result time out, exit thread\n");
//            break;
        }
        else
        {
            for (i = 0; i < mVdaChnList.count(); i++){
                if (FD_ISSET(mVdaChnList[i].VdaFd, &read_fds))
                {
                    /*******************************************************
                    step 2.3 : call mpi to get one-frame stream
                    *******************************************************/
                    s32Ret = HI_MPI_VDA_GetData(mVdaChnList[i].VdaChn, &stVdaData, -1);
                    if(s32Ret != HI_SUCCESS)
                    {
                        SAMPLE_PRT("HI_MPI_VDA_GetData failed with %#x!\n", s32Ret);
                        break ;
                    }

//                    qDebug()<<"chn "<<mVdaChnList[i].VdaChn <<"move detect alarm cnt:"<<stVdaData.unData.stMdData.u32AlarmPixCnt;
//                    printf("ObjNum=%d, IndexOfMaxObj=%d, SizeOfMaxObj=%d, SizeOfTotalObj=%d\n", \
//                                   stVdaData.unData.stMdData.stObjData.u32ObjNum, \
//                             stVdaData.unData.stMdData.stObjData.u32IndexOfMaxObj, \
//                             stVdaData.unData.stMdData.stObjData.u32SizeOfMaxObj,\
//                             stVdaData.unData.stMdData.stObjData.u32SizeOfTotalObj);
                    /*******************************************************
                    *step 2.4 : save frame to file
                    *******************************************************/
//                    printf("\033[0;0H");/*move cursor*/
//                    SAMPLE_COMM_VDA_MdPrtSad(fp1, &stVdaData);
//                    SAMPLE_COMM_VDA_MdPrtObj(fp, &stVdaData);
//                    SAMPLE_COMM_VDA_MdPrtAp(fp, &stVdaData);

                    movestart[i] = VideoMoveDetectProcess(&stVdaData);
                    if(!ignore_flag[i]){

                        ignore_flag[i] = true;
                        movestarttime = QDateTime::currentDateTime().toTime_t();
                        if(movestart[i] && !premovestart[i]){
                            emit videoMoveDetectChangeSignal(i,VIDEO_MOVEDETECT,true);
                            qDebug()<<"move detect enable signal "<<i;
                        }else if (premovestart[i] && !movestart[i]) {
                            emit videoMoveDetectChangeSignal(i,VIDEO_MOVEDETECT,false);
                            qDebug()<<"move detect disenable signal "<<i;
                        }
                        premovestart[i] = movestart[i];

                    }else {
                        moveendtime = QDateTime::currentDateTime().toTime_t();
                        if(moveendtime - movestarttime >= mDetectDelay){
                            ignore_flag[i] = false;
                        }
                    }
                    /*******************************************************
                    *step 2.5 : release stream
                    *******************************************************/
                    s32Ret = HI_MPI_VDA_ReleaseData(mVdaChnList[i].VdaChn,&stVdaData);
                    if(s32Ret != HI_SUCCESS)
                    {
                        SAMPLE_PRT("HI_MPI_VDA_ReleaseData failed with %#x!\n", s32Ret);
                        break;
                    }
                }
            }
        }
        mVdaListMutex.unlock();

        usleep(100000);

    }
#endif
    fclose(fp);
    fclose(fp1);
    for (i = 0;i < mVdaChnList.count();i++) {
        VdaStop(mVdaChnList[i].VdaChn);
    }
}
