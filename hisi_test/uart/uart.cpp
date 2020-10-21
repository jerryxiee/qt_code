#include "uart.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //文件控制定义
#include <termios.h>//终端控制定义
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define DEVICE "/dev/ttyAMA"

#define S_TIMEOUT 0

static int serial_fd = 0;

const char *Serial_Dev = "/dev/ttyAMA2";

//打开串口并初始化设置

int set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop)
{
    struct termios newtio,oldtio;
    if(tcgetattr(fd,&oldtio)!=0)
    {
        perror("error:SetupSerial 3\n");
        return -1;
    }
    bzero(&newtio,sizeof(newtio));
    //使能串口接收
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    newtio.c_lflag &=~ICANON;//原始模式

    //newtio.c_lflag |=ICANON; //标准模式

    //设置串口数据位
    switch(nBits)
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |=CS8;
            break;
    }
    //设置奇偶校验位
    switch(nEvent)

    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &=~PARENB;
            break;
    }
    //设置串口波特率
    switch(nSpeed)
    {
        case 2400:
            cfsetispeed(&newtio,B2400);
            cfsetospeed(&newtio,B2400);
            break;
        case 4800:
            cfsetispeed(&newtio,B4800);
            cfsetospeed(&newtio,B4800);
            break;
        case 9600:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;
        case 115200:
            cfsetispeed(&newtio,B115200);
            cfsetospeed(&newtio,B115200);
            break;
        case 460800:
            cfsetispeed(&newtio,B460800);
            cfsetospeed(&newtio,B460800);
            break;
        default:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;
    }
    //设置停止位
    if(nStop == 1)
        newtio.c_cflag &= ~CSTOPB;
    else if(nStop == 2)
        newtio.c_cflag |= CSTOPB;
    newtio.c_cc[VTIME] = 1;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);

    if(tcsetattr(fd,TCSANOW,&newtio)!=0)
    {
        perror("com set error\n");
        return -1;
    }
    return 0;
}

int serial_read(int fd,unsigned char data[],int num)
{
    int read_num = num;
    int count = 3;
    unsigned char *pdat = data;
    int ret;
    fd_set readfs;
    struct timeval Timeout;

    Timeout.tv_usec = 0;
    Timeout.tv_sec  = 1;
    FD_ZERO(&readfs);
    FD_SET(fd, &readfs);

    do
    {
        ret = select(fd+1, &readfs, NULL, NULL, &Timeout);
        if(ret == 0)
        {
            printf("time out!\n");
            //continue;
            return 0;


        }
        else if(ret < 0)
        {
            printf("select error!\n");

        }
        else
        {
            if (FD_ISSET(fd,&readfs))
            {

                ret = read( fd, pdat, read_num);
                read_num -= ret;
                pdat += ret;
                //printf("serial_read ret=%d\n",ret);
            }
        }
    }while((read_num > 0)/* && (ret > 0)*/);
    //printf("read_num=%d\n",num);

    return num;
}

int serial_sendx(int fd,unsigned char data[], int num)
{
    int send_num = num;
    unsigned char *pdat = data;
    int ret;

    do
    {
        ret = write( fd,pdat, send_num);
        send_num -= ret;
        pdat += ret;

    }while(send_num > 0);
    return 0 ;

}

int uart_init(int chn,int nSpeed,int nBits,char nEvent,int nStop)
{
    int fd;
    int ret;

    char buff[64];
    sprintf(buff,"%s%d",DEVICE,chn);

    fd = open( buff, O_RDWR | O_NOCTTY |O_NDELAY);
    if( fd < 0 )
    {

        printf("open uart error!\n");
        return -1;
    }

    ret = set_opt(fd,nSpeed,nBits,nEvent,nStop);  //设置串口
    if(ret < 0)
    {
        printf("set_opt failed!\n");
        return -1;
    }
    //printf("uart_init sucess\n");
    return fd;
}


int uart_main(int argc, char **argv)
{

    if ( (argc < 2))
    {

        return -1;
    }

    int chn = atoi(argv[1]);

    serial_fd = uart_init(chn,9600,8,'N',1);

    u_int8_t buf[]="hello world";
    u_int8_t buf1[11] ;
    memset(buf1,0,sizeof(char)*11);

    while(1)
    {
        serial_sendx(serial_fd, buf, 2);
//        printf("\n");
        sleep(1);
//        //if(uart_recv(serial_fd, buf1, 11) > 0)
        int n = serial_read(serial_fd, buf1, 2);
        if(n > 0)
            printf("receive: %X %x\n", buf1[0],buf1[0]);
        else {
            printf("read null\n");
        }
//        memset(buf1,0,sizeof(char)*11);
    }

    close(serial_fd);
    return 0;
}
