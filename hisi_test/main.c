#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "sample_comm.h"


#if 1
extern int vio_main(int argc, char *argv[]);
extern int hifb_main(int argc, char *argv[]);
extern int vdec_main(int argc, char *argv[]);
extern int venc_main(int argc, char *argv[]);
extern int vgs_main(int argc, char *argv[]);
extern int vda_main(int argc, char *argv[]);
extern int ivez_main(int argc, char *argv[]);
extern int region_main(int argc, char *argv[]);
extern int tde_main(int argc, char *argv[]);
extern int pciv_slave_main(int argc, char *argv[]);
extern int pciv_host_main(int argc, char *argv[]);
extern int cascade_master_main(int argc, char* argv[]);
extern int cascade_slave_main(int argc, char* argv[]);
extern int audio_main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    int cmd;
//    nvp6134_input_videofmt videofmt;

//    int fd = open(NVP6134_FILE, O_RDWR);

//    if (fd < 0)
//    {
//        printf("open nvp6134 (%s) fail\n", NVP6134_FILE);
//        return -1;
//    }

//    ioctl(fd, IOC_VDEC_GET_INPUT_VIDEO_FMT, &videofmt);

//    int i;
//    for (i = 0;i < 8;i++) {
//        printf("chn:%d %x:%x\n",i,videofmt.getvideofmt[i],videofmt.inputvideofmt[i]);
//    }


//    vio_main(argc, argv);

    printf("enter mode: \n");
    scanf("%d",&cmd);

    switch(cmd){
        case 1:hifb_main(argc,argv);break;
        case 2:venc_main(argc,argv);break;
        case 3:vdec_main(argc,argv);break;
        case 4:vio_main(argc,argv);break;
        case 5:vgs_main(argc,argv);break;
        case 6:region_main(argc,argv);break;
        case 7:audio_main(argc,argv);break;

    }
//#if 0
//    hifb_main(argc,argv);
//#else
//    venc_main(argc,argv);


//#endif
    return 0;
}

#else

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

#include "sample_comm.h"

#include <linux/fb.h>
#include "hifb.h"
#include "loadbmp.h"
#include "hi_tde_api.h"
#include "hi_tde_type.h"
#include "hi_tde_errcode.h"

int main(int argc, char *argv[])
{

    return 0;
}



#endif
