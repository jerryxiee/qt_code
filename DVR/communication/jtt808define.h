#ifndef JTT808DEFINE_H
#define JTT808DEFINE_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define CUSTOMCMD     0xAA00
#define SERVERINFOMSGTYPE  CUSTOMCMD|0x01
typedef struct{
    int registerType;                      //注册方式 0：主服务器注册，1：同时注册
    unsigned mainServerIp;                 //主服务器ip
    unsigned short mainTcpPort;            //主服务器tcp端口
    unsigned short mainUdpPort;            //主服务器udp端口
    unsigned backupServerIp;               //备份服务器ip
    unsigned short backupTcpPort;          //备份服务器tcp端口
    unsigned short backupUdpPort;          //备份服务器udp端口
    unsigned mainServerUseTcp;             //主服务器注册协议 1：tcp 其他：udp
    unsigned backupServerUseTcp;           //备份服务器注册协议 1：tcp 其他：udp
    char telephoneNum[12];                 //电话号码
    unsigned short tcpTimeout;             //tcp超时时间
    unsigned short udpTimeout;             //udp超时时间
    unsigned short tcpRepeatTimes;           //tcp重传次数
    unsigned short udpRepeatTimes;           //udp重传次数
    unsigned short smsTimeout;             //sms超时时间
    unsigned short smsRepeatTimes;           //sms重传次数
    unsigned heratBeatTime;                //心跳时间
    unsigned reserve[8];                   //
}ServerInfo;

//通用应答
typedef struct{
    short responseNum;       //应答流水号
    short msgId;             //应答ID
    unsigned char result;    //结果
} CommonResponse;

//终端注册
typedef struct{
    short ProvinceID;          //省域ID
    short CountyID;            //市县域ID
    char ManufacturerID[5];   //制造商ID
    char DeviceModel[20];      //终端型号
    char DeviceID[7];         //终端ID
    char CarColor;             //车牌颜色
    char CarLicense[16];       //车牌
} RegisterMsg;

typedef struct{
    short reportNum;
    char result;
    char authNum[16];

} RegisterResponse;

//终端鉴权
typedef struct{
    char Len;               //鉴权码长度
    char AuthID[16];        //鉴权码
    char DeviceIMEI[15];    //
    char SoftWareV[20];     //软件版本
}AuthMsg;

#define DEFAULTLEN 20
//终端属性应答
typedef struct{
    short DeviceType;       //终端类型
    char ManufacturerID[5]; //制造商ID
    char DeviceModel[20];   //终端型号
    char DeviceID[7];      //终端ID
    char ICCID[10];         //SIM卡ICCID号
    char HWVersionLen;      //终端硬件版本长度
    char HWVersion[DEFAULTLEN];     //硬件版本
    char HWVersionLen1;     //固件版本长度
    char HWVersion1[DEFAULTLEN];    //固件版本
    char GNSSAttr;          //GNSS模块属性
    char ComModuleAttr;     //通信模块属性
} DeviceAttrMsg;


//位置基本信息
typedef struct{
    unsigned AlarmType;            //报警标志
    unsigned Status;               //状态
    int Latitude;             //纬度
    int Longitude;            //经度
    short Altitude;           //海拔
    short Speed;              //速度
    short Direction;          //方向
    char time[6];             //时间

} PositionBaseMsg;

//驾驶员身份信息
typedef struct{
    char Status;                                //状态
    char Time[6];                               //时间
    char BIcRead;                               //读IC卡是否成功
    char NameLen;                               //姓名长度
    char Name[DEFAULTLEN];                      //姓名
    char DriverNum[20];                         //资格证编号
    char CardauthNameLen;                       //发证机构名长度
    char CardauthName[DEFAULTLEN];              //发证机构名
    char CertificateValidityPeriod[4];          //证件有效期
    char DriverIDNum[20];                       //驾驶员身份证号
} DriverInfoMsg;


//实时音视频请求指令包
typedef struct{
    unsigned char ipLen;
    char ipAddr[16];
    short tcpPort;
    short udpPort;
    unsigned char channel;
    unsigned char dataType;
    unsigned char streamType;

} RealTimeStreamPack;

//回放音视频请求指令包
typedef struct{
    unsigned char ipLen;
    unsigned char ipAddr[16];
    short tcpPort;
    short udpPort;
    unsigned char channel;
    unsigned char dataType;
    unsigned char streamType;
    unsigned char playType;
    unsigned char speedCtr1;
    unsigned char speedCtr2;
    unsigned char startTime[6];
    unsigned char endTime[6];

} PlayBackStreamPack;


//音视频传输请求
typedef struct{               //字段            //直播           //回放
    char playType;        //判断直播与回放           0               1
    char ipAddr[16];      //ip地址                Y               Y
    short tcpPort;        //tcp端口               Y               Y
    short udpPort;        //udp端口               Y               Y
    char logicChannel;    //逻辑通道               Y               Y
    char dateType;        //数据类型               Y            音视频类型Y
    char streamType;      //码流类型               Y               Y
    char storeType;       //存储器类型                              Y
    char playBackType;    //回放模式                                Y
    char playRate;        //快进快推倍数                            Y
    char startTime[6];    //开始时间                                Y
    char endTime[6];      //结束时间                                Y
}StreamParam;

typedef struct{
    char logicChn;
    char startTime[6];
    char endTime[6];
    char alarmFlag[8];
    char fileType;
    char streamType;
    char storeType;
} RecordFileMsg;

typedef struct{
    RecordFileMsg file;
    unsigned fileSize;

} RecordFileInfo;

typedef struct{
    unsigned char logicChannel;         //通道
    unsigned char orderCtr;             //控制指令
    unsigned char closeStreamType;      //关闭音视频类型
    unsigned char changeStream;         //切换码流类型

} ReamTimeStreamControl;

typedef struct{                      //字段                 直播          回放
    char ipAddr[16];
    short port;
    unsigned char streamType;             //                0            1
    unsigned char logicChannel;           //通道              Y           Y
    unsigned char orderCtr;               //控制指令           Y           Y
    unsigned char cmd1;           //                   关闭音视频类型Y    快进快退倍数Y
    unsigned char cmd2;             //                  切换码流类型Y
    unsigned char posTime[6];             //                            控制视频回放位置Y
} StreamControl;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // JTT808DEFINE_H
