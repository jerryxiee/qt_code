#include "videoplay.h"
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include <QDebug>
#include "common/sample_common_sys.h"

VideoPlay::VideoPlay(QObject *parent) : QThread(parent)
{
    VO_DEV VoDev;
    VO_LAYER VoLayer;

    Sample_Common_Sys::Get_Sys_VoDev(VoDev,VoLayer);
    m_Vdec_Vio.SAMPLE_COMM_VO_SetDev(VoDev,VoLayer);

    memset(&m_Thread_Attr,0x0,sizeof (VdecThreadParam));
    memset(&m_stVdecChnAttr,0x0,sizeof(VDEC_CHN_ATTR_S));

    mVideoFileList.clear();

}

VideoPlay::~VideoPlay()
{
    mVideoFileList.clear();
    qDebug("enter %s:%d",__FUNCTION__,__LINE__);
    if(m_pVdec){
        delete m_pVdec;
    }
    if(m_pVpss){
        delete m_pVpss;
    }


}

void VideoPlay::setFileList(QFileInfoList &fileList)
{
    mVideoFileList = fileList;
}

void VideoPlay::Set_VdecAttr(VdecThreadParam &VdecAttr)
{
    memcpy(&m_Thread_Attr,&VdecAttr,sizeof (VdecThreadParam));
    qDebug("Set_VdecAttr  m_Thread_Attr.s32MinBufSize=%d\n",m_Thread_Attr.s32MinBufSize);
}

char* VideoPlay::Get_FileType(char *filename)
{
    char *name = filename;
    char *filetype = nullptr;

    while (name) {
        name = strchr(name+1,'.');
        if(name){
            filetype = name;
        }
    }

    return filetype;
}

bool VideoPlay::dealFileHead(char *filename)
{

}

HI_BOOL VideoPlay::Start_Vdec(char *filename, VPSS_GRP VpssGrp, VPSS_CHN VpssChn)
{
    char* filetype = nullptr;
    HI_S32 s32Ret;
    SIZE_S  stSize;
    VB_CONF_S       stModVbConf;
    SIZE_S stRotateSize;
    PAYLOAD_TYPE_E enType;
    RECT_S pos;
    VdecThreadParam stVdecParam;

    memset(&stVdecParam,0x0,sizeof(VdecThreadParam));

    stSize.u32Width = WIDTH;
    stSize.u32Height = HEIGHT;

    if(mVideoFileList.count() == 0){
        qDebug()<<"no file";
        return HI_FALSE;
    }

    filetype = Get_FileType(filename);
    if(!filetype){
        SAMPLE_PRT("get file type failed!\n");
        return HI_FALSE;
    }
    if(!Sample_Common_Vdec::SAMPLE_COMM_VDEC_GetPayload_Type(filetype,&enType)){
        SAMPLE_PRT("get enType failed!\n");
        return HI_FALSE;
    }

    m_pVdec = new Sample_Common_Vdec(1,1,0);

    printf("init CommonPool\n");
    m_pVdec->SAMPLE_COMM_VDEC_ModCommPoolConf(&stModVbConf, enType, &stSize, 1, HI_FALSE);
    s32Ret = m_pVdec->SAMPLE_COMM_VDEC_InitModCommVb(&stModVbConf);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("init mod common vb fail for %#x!\n", s32Ret);
        return HI_FALSE;
    }

    m_pVdec->SAMPLE_COMM_VDEC_ChnAttr(1, &m_stVdecChnAttr, enType, &stSize);

    m_pVdec->SAMPLE_COMM_VDEC_SetChnAttr(0,&m_stVdecChnAttr);

    if(VpssGrp < 0){
        stRotateSize.u32Width = stRotateSize.u32Height = MAX2(m_stVdecChnAttr.u32PicWidth, m_stVdecChnAttr.u32PicHeight);
        m_pVpss= new Sample_Common_Vpss(1,1,&stRotateSize,nullptr);
        if(!m_pVpss){
            printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
            return HI_FALSE;
        }else if( HI_FALSE == m_pVpss->SAMPLE_COMM_VPSS_CreatIsSucess()){

            printf(">>>>>>>%s:%d\n",__FUNCTION__,__LINE__);
            goto END1;
        }
    }

    s32Ret = m_pVdec->SAMPLE_COMM_VDEC_Start(1);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("start VDEC fail for %#x!\n", s32Ret);
        goto END2;
    }

    if(VpssGrp < 0){
        pos.s32X = 0;
        pos.s32Y = 0;
        pos.u32Height = HEIGHT;
        pos.u32Width = WIDTH;
        s32Ret = m_Vdec_Vio.SAMPLE_COMM_VO_StartChn(VDEC_VO,pos);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("start vo fail for %#x!\n", s32Ret);
            goto END3;
        }

        m_pVdec->SAMPLE_COMM_VDEC_BindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);

        s32Ret = m_Vdec_Vio.SAMPLE_COMM_VO_BindVpss(VDEC_VO, m_pVpss->m_Grp_Tab[0], VPSS_CHN0);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("vpss bind vo fail for %#x!\n", s32Ret);
            goto END4;
        }
    }else{
        m_pVdec->SAMPLE_COMM_VDEC_BindVpss(m_pVdec->m_Vdec_Tab[0],VpssGrp,VpssChn);
    }

    sprintf(stVdecParam.cFileName, filename);
    stVdecParam.s32MilliSec     = -1;
//        stVdecParam.s32ChnId        = i;
    stVdecParam.s32IntervalTime = 1;
    stVdecParam.u64PtsInit      = 40000;
    stVdecParam.u64PtsIncrease  = 40000;
    stVdecParam.eCtrlSinal      = VDEC_CTRL_START;
    stVdecParam.bLoopSend       = HI_FALSE;
    stVdecParam.bManuSend       = HI_FALSE;
    stVdecParam.enType          = m_stVdecChnAttr.enType;
    stVdecParam.s32MinBufSize   = (m_stVdecChnAttr.u32PicWidth * m_stVdecChnAttr.u32PicHeight * 3)>>1;
    if (PT_H264 == m_stVdecChnAttr.enType  || PT_MP4VIDEO == m_stVdecChnAttr.enType)
    {
        stVdecParam.s32StreamMode  = m_stVdecChnAttr.stVdecVideoAttr.enMode;
    }
    else
    {
        stVdecParam.s32StreamMode = VIDEO_MODE_FRAME;
    }

    Set_VdecAttr(stVdecParam);


    start();
    return HI_TRUE;
END4:
    m_pVdec->SAMPLE_COMM_VDEC_UnBindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);
    m_Vdec_Vio.SAMPLE_COMM_VO_UnBindVpss(VDEC_VO, m_pVpss->m_Grp_Tab[0], VPSS_CHN0);

END3:
    m_Vdec_Vio.SAMPLE_COMM_VO_StopChn(VDEC_VO);
END2:
    m_pVdec->SAMPLE_COMM_VDEC_Stop(1);

END1:
    m_pVpss->SAMPLE_COMM_VPSS_Stop();

    return HI_FALSE;
}

void VideoPlay::Stop_Vdec()
{
    m_Vdec_Run = false;
    if(m_pVdec){
        m_pVdec->SAMPLE_COMM_VDEC_UnBindVpss(m_pVdec->m_Vdec_Tab[0],m_pVpss->m_Grp_Tab[0],0);
        m_Vdec_Vio.SAMPLE_COMM_VO_UnBindVpss(VDEC_VO, m_pVpss->m_Grp_Tab[0], VPSS_CHN0);
        m_Vdec_Vio.SAMPLE_COMM_VO_StopChn(VDEC_VO);
        m_pVdec->SAMPLE_COMM_VDEC_Stop(1);
        m_pVpss->SAMPLE_COMM_VPSS_Stop();
        mVideoFileList.clear();
    }
    m_pVdec = nullptr;


}
void VideoPlay::run()
{
//    VdecThreadParam *pstVdecThreadParam =(VdecThreadParam *)pArgs;
    FILE *fpStrm=nullptr;
    HI_U8 *pu8Buf = nullptr;
    VDEC_STREAM_S stStream;
    HI_BOOL bFindStart, bFindEnd;
    HI_S32 s32Ret,  i,  start = 0;
    HI_S32 s32UsedBytes = 0, s32ReadLen = 0;
    HI_U64 u64pts = 0;
    HI_S32 len;
    HI_BOOL sHasReadStream = HI_FALSE;
    int index = 0;
    VDEC_CHN_STAT_S stStat;

//    prctl(PR_SET_NAME, "hi_SendStream", 0, 0, 0);

//    if(m_Thread_Attr.cFileName != 0)
//    {
//        fpStrm = fopen(m_Thread_Attr.cFileName, "rb");
//        if(fpStrm == nullptr)
//        {
//            printf("SAMPLE_TEST:can't open file %s in send stream thread:%d\n",m_Thread_Attr.cFileName, m_Thread_Attr.s32ChnId);
//            return ;
//        }
//    }
    QByteArray filename = mVideoFileList.first().absoluteFilePath().toLocal8Bit();

    fpStrm = fopen(filename.data(), "rb");
    if(fpStrm == nullptr){
        qDebug()<<"can not open file:"<<mVideoFileList.first().fileName();
        return ;
    }
    //printf("SAMPLE_TEST:chn %d, stream file:%s, bufsize: %d\n",
    //m_Thread_Attr.s32ChnId, m_Thread_Attr.cFileName, m_Thread_Attr.s32MinBufSize);

    pu8Buf = (HI_U8 *)malloc(m_Thread_Attr.s32MinBufSize);
    if(pu8Buf == nullptr)
    {
        printf("SAMPLE_TEST:can't alloc %d in send stream thread:%d\n", m_Thread_Attr.s32MinBufSize, m_Thread_Attr.s32ChnId);
//        fclose(fpStrm);
        return;
    }
    fflush(stdout);

//    printf("start vdec filename=%s\n",m_Thread_Attr.cFileName);
//    s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
//    printf("read data(%d) = %d\n",m_Thread_Attr.s32MinBufSize,s32ReadLen);
//    fseek(fpStrm, 0, SEEK_END);
//    printf("file size=%d\n",ftell(fpStrm));

//    fseek(fpStrm, 0, SEEK_SET);

    u64pts = m_Thread_Attr.u64PtsInit;
     m_Vdec_Run = true;
    while (m_Vdec_Run)
    {
        if(feof(fpStrm)){
            memset(&stStream, 0, sizeof(VDEC_STREAM_S) );
            stStream.bEndOfStream = HI_TRUE;
            HI_MPI_VDEC_SendStream(m_Thread_Attr.s32ChnId, &stStream, -1);

            index ++;
            if(index < mVideoFileList.count()){
                fclose(fpStrm);
                s32UsedBytes = 0;
                filename = mVideoFileList.at(index).absoluteFilePath().toLocal8Bit();
                fpStrm = fopen(filename.data(), "rb");
                if(fpStrm == nullptr){
                    qDebug()<<"can not open file:"<<mVideoFileList.at(index).fileName();
                    break ;
                }
                qDebug()<<"file end open next success";
            }else {
                qDebug()<<"file list play over";

                break;
            }

        }
//        usleep(20000);
        if (m_Thread_Attr.eCtrlSinal == VDEC_CTRL_STOP)
        {
//            break;
        }
        else if (m_Thread_Attr.eCtrlSinal == VDEC_CTRL_PAUSE)
        {
            sleep(MIN2(m_Thread_Attr.s32IntervalTime,1000));
            continue;
        }

        if ( (m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME) && (m_Thread_Attr.enType == PT_MP4VIDEO) )
        {
            bFindStart = HI_FALSE;
            bFindEnd   = HI_FALSE;
//            fseek(fpStrm, s32UsedBytes, SEEK_SET);
            s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
            if (s32ReadLen == 0)
            {
                if (m_Thread_Attr.bLoopSend)
                {
                    s32UsedBytes = 0;
                    fseek(fpStrm, 0, SEEK_SET);
                    s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
                }
                else
                {
                    continue;
                }
            }

            for (i=0; i<s32ReadLen-4; i++)
            {
                if (pu8Buf[i] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1 && pu8Buf[i+3] == 0xB6)
                {
                    bFindStart = HI_TRUE;
                    i += 4;
                    break;
                }
            }

            for (; i<s32ReadLen-4; i++)
            {
                if (pu8Buf[i  ] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1 && pu8Buf[i+3] == 0xB6)
                {
                    bFindEnd = HI_TRUE;
                    break;
                }
            }

            s32ReadLen = i;
            if (bFindStart == HI_FALSE)
            {
                printf("SAMPLE_TEST: chn %d can not find start code! s32ReadLen %d, s32UsedBytes %d. \n",
                                            m_Thread_Attr.s32ChnId, s32ReadLen, s32UsedBytes);
            }
            else if (bFindEnd == HI_FALSE)
            {
                s32ReadLen = i+4;
            }

        }
        else if ( (m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME) && (m_Thread_Attr.enType == PT_H264) )
        {
            bFindStart = HI_FALSE;
            bFindEnd   = HI_FALSE;
//            fseek(fpStrm, s32UsedBytes, SEEK_SET);
            HI_MPI_VDEC_Query(m_Thread_Attr.s32ChnId,&stStat);
            if(stStat.u32LeftStreamFrames > 0){
                continue;
            }
            s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
            if (s32ReadLen == 0)
            {
                if (m_Thread_Attr.bLoopSend)
                {
                    s32UsedBytes = 0;
                    fseek(fpStrm, 0, SEEK_SET);
                    s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
                }
                else
                {
                    continue;
                }
            }

            for (i=0; i<s32ReadLen-8; i++)
            {
                int tmp = pu8Buf[i+3] & 0x1F;
                if (  pu8Buf[i] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1 &&
                       (
                           ((tmp == 5 || tmp == 1) && ((pu8Buf[i+4]&0x80) == 0x80)) ||
                           (tmp == 20 && (pu8Buf[i+7]&0x80) == 0x80)
                        )
                   )
                {
                    bFindStart = HI_TRUE;
                    i += 8;
                    break;
                }
            }

            for (; i<s32ReadLen-8; i++)
            {
                int tmp = pu8Buf[i+3] & 0x1F;
                if (  pu8Buf[i  ] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1 &&
                            (
                                  tmp == 15 || tmp == 7 || tmp == 8 || tmp == 6 ||
                                  ((tmp == 5 || tmp == 1) && ((pu8Buf[i+4]&0x80) == 0x80)) ||
                                  (tmp == 20 && (pu8Buf[i+7]&0x80) == 0x80)
                              )
                   )
                {
                    bFindEnd = HI_TRUE;
                    break;
                }
            }

            if(i > 0) s32ReadLen = i;
            if (bFindStart == HI_FALSE)
            {
                printf("SAMPLE_TEST: chn %d can not find start code!s32ReadLen %d, s32UsedBytes %d. \n",
                                            m_Thread_Attr.s32ChnId, s32ReadLen, s32UsedBytes);
            }
            else if (bFindEnd == HI_FALSE)
            {
                s32ReadLen = i+8;
            }

        }


        else if (m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME && m_Thread_Attr.enType == PT_H265)
        {
            HI_BOOL  bNewPic = HI_FALSE;
            bFindStart = HI_FALSE;
            bFindEnd   = HI_FALSE;
//            fseek(fpStrm, s32UsedBytes, SEEK_SET);
            s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);

            if (s32ReadLen == 0)
            {
                if (m_Thread_Attr.bLoopSend == HI_TRUE)
                {
                    s32UsedBytes = 0;
                    fseek(fpStrm, 0, SEEK_SET);
                    s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
                }
                else
                {
                    continue;
                }
            }

            for (i=0; i<s32ReadLen-6; i++)
            {
                bNewPic = (HI_BOOL)(pu8Buf[i+0] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1
                    &&(((pu8Buf[i+3]&0x7D) >= 0x0 && (pu8Buf[i+3]&0x7D) <= 0x2A) || (pu8Buf[i+3]&0x1F) == 0x1)
                    &&((pu8Buf[i+5]&0x80) == 0x80));//first slice segment

                if (bNewPic)
                {
                    bFindStart = HI_TRUE;
                    i += 4;
                    break;
                }
            }

            for (; i<s32ReadLen-6; i++)
            {
                bNewPic = (HI_BOOL)(pu8Buf[i+0] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1
                    &&(((pu8Buf[i+3]&0x7D) >= 0x0 && (pu8Buf[i+3]&0x7D) <= 0x2A) || (pu8Buf[i+3]&0x1F) == 0x1)
                    &&((pu8Buf[i+5]&0x80) == 0x80));//first slice segment

                if (  pu8Buf[i  ] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1
                    &&(((pu8Buf[i+3]&0x7D) == 0x40) || ((pu8Buf[i+3]&0x7D) == 0x42) || ((pu8Buf[i+3]&0x7D) == 0x44)|| bNewPic)
                   )
                {
                    bFindEnd = HI_TRUE;
                    break;
                }
            }

            s32ReadLen = i;

            if (bFindStart == HI_FALSE)
            {
                printf("hevc can not find start code! %d s32ReadLen 0x%x +++++++++++++\n", m_Thread_Attr.s32ChnId,s32ReadLen);
            }
            else if (bFindEnd == HI_FALSE)
            {
                s32ReadLen = i+6;
            }

        }
        else if (m_Thread_Attr.enType == PT_MJPEG)
        {
            bFindStart = HI_FALSE;
            bFindEnd   = HI_FALSE;
//            fseek(fpStrm, s32UsedBytes, SEEK_SET);
            s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
            if (s32ReadLen == 0)
            {
                if (m_Thread_Attr.bLoopSend)
                {
                    s32UsedBytes = 0;
                    fseek(fpStrm, 0, SEEK_SET);
                    s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
                }
                else
                {
                    continue;
                }
            }


            for (i=0; i<s32ReadLen-2; i++)
            {
                if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD8)
                {
                    start = i;
                    bFindStart = HI_TRUE;
                    i = i + 2;
                    break;
                }
            }

            for (; i<s32ReadLen-4; i++)
            {
                if ( (pu8Buf[i] == 0xFF) && (pu8Buf[i+1]& 0xF0) == 0xE0 )
                {
                     len = (pu8Buf[i+2]<<8) + pu8Buf[i+3];
                     i += 1 + len;
                }
                else
                {
                    break;
                }
            }

            for (; i<s32ReadLen-2; i++)
            {
                if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD8)
                {
                    bFindEnd = HI_TRUE;
                    break;
                }
            }
            s32ReadLen = i;
            if (bFindStart == HI_FALSE)
            {
                printf("SAMPLE_TEST: chn %d can not find start code! s32ReadLen %d, s32UsedBytes %d. \n",
                                            m_Thread_Attr.s32ChnId, s32ReadLen, s32UsedBytes);
            }
            else if (bFindEnd == HI_FALSE)
            {
                s32ReadLen = i+2;
            }
        }
         else if ((m_Thread_Attr.enType == PT_JPEG) )
        {
            if (HI_TRUE != sHasReadStream)
            {

                bFindStart = HI_FALSE;
                bFindEnd   = HI_FALSE;

                fseek(fpStrm, s32UsedBytes, SEEK_SET);
                s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
                if (s32ReadLen == 0)
                {
                    if (m_Thread_Attr.bLoopSend)
                    {
                        s32UsedBytes = 0;
                        fseek(fpStrm, 0, SEEK_SET);
                        s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
                    }
                    else
                    {
                        continue;
                    }
                }


                for (i=0; i<s32ReadLen-2; i++)
                {
                    if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD8)
                    {
                        start = i;
                        bFindStart = HI_TRUE;
                        i = i + 2;
                        break;
                    }
                }

                for (; i<s32ReadLen-4; i++)
                {
                    if ( (pu8Buf[i] == 0xFF) && (pu8Buf[i+1]& 0xF0) == 0xE0 )
                    {
                         len = (pu8Buf[i+2]<<8) + pu8Buf[i+3];
                         i += 1 + len;
                    }
                    else
                    {
                        break;
                    }
                }

                for (; i<s32ReadLen-2; i++)
                {
                    if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD8)
                    {
                        bFindEnd = HI_TRUE;
                        break;
                    }
                }
                s32ReadLen = i;
                if (bFindStart == HI_FALSE)
                {
                    printf("SAMPLE_TEST: chn %d can not find start code! s32ReadLen %d, s32UsedBytes %d. \n",
                                                m_Thread_Attr.s32ChnId, s32ReadLen, s32UsedBytes);
                }
                else if (bFindEnd == HI_FALSE)
                {
                    s32ReadLen = i+2;
                }
                 sHasReadStream = HI_TRUE;
            }
        }
        else
        {
//            fseek(fpStrm, s32UsedBytes, SEEK_SET);
            s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
            if (s32ReadLen == 0)
            {
                if (m_Thread_Attr.bLoopSend)
                {
                    s32UsedBytes = 0;
                    fseek(fpStrm, 0, SEEK_SET);
                    s32ReadLen = fread(pu8Buf, 1, m_Thread_Attr.s32MinBufSize, fpStrm);
                }
                else
                {
                    continue;
                }
            }
        }

        stStream.u64PTS  = u64pts;
        stStream.pu8Addr = pu8Buf + start;
        if(m_Thread_Attr.enType == PT_MJPEG)
        {
             stStream.u32Len  = s32ReadLen - start;
        }
        else
        {
            stStream.u32Len  = s32ReadLen;
        }
        stStream.bEndOfFrame  = (m_Thread_Attr.s32StreamMode==VIDEO_MODE_FRAME)? HI_TRUE: HI_FALSE;
        stStream.bEndOfStream = HI_FALSE;


        //printf("Send One Frame");
        //fflush(stdout);

        s32Ret=HI_MPI_VDEC_SendStream(m_Thread_Attr.s32ChnId, &stStream, m_Thread_Attr.s32MilliSec);
        m_Thread_Attr.cUserCmd = 0;
        if (HI_SUCCESS != s32Ret)
        {
            usleep(100);
        }
        else
        {
            if(m_Thread_Attr.enType == PT_MJPEG)
            {
                s32UsedBytes = s32UsedBytes +s32ReadLen;
            }
            else
            {
                s32UsedBytes = s32UsedBytes +s32ReadLen + start;
            }
            u64pts += m_Thread_Attr.u64PtsIncrease;
        }
        fseek(fpStrm, s32UsedBytes, SEEK_SET);
       usleep(1000);
    }

    /* send the flag of stream end */
    memset(&stStream, 0, sizeof(VDEC_STREAM_S) );
    stStream.bEndOfStream = HI_TRUE;
    HI_MPI_VDEC_SendStream(m_Thread_Attr.s32ChnId, &stStream, -1);

    //printf("SAMPLE_TEST:send steam thread %d return ...\n", m_Thread_Attr.s32ChnId);
    fflush(stdout);
    if (pu8Buf != nullptr)
    {
        free(pu8Buf);
    }
    fclose(fpStrm);
    mVideoFileList.clear();
    qDebug()<<"exit paly thread";
}
