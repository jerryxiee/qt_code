#ifndef JTT808DEFINE_H
#define JTT808DEFINE_H

#include "common/sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define CUSTOMCMD     0xAA00
#define SERVERINFOMSGTYPE  CUSTOMCMD|0x01
typedef struct{
    uint32_t registerType;       //注册方式 0：主服务器注册，1：同时注册
    uint32_t mainIpLen;              //地址长度
    char mainServerIp[16];        //主服务器ip
    uint16_t mainTcpPort;            //主服务器tcp端口
    uint16_t mainUdpPort;            //主服务器udp端口
    uint32_t backupIpLen;            //地址长度
    char backupServerIp[16];       //备份服务器ip
    uint16_t backupTcpPort;          //备份服务器tcp端口
    uint16_t backupUdpPort;          //备份服务器udp端口
    uint32_t mainServerUseTcp;       //主服务器注册协议 1：tcp 其他：udp
    uint32_t backupServerUseTcp;     //备份服务器注册协议 1：tcp 其他：udp
    char telephoneNum[12];        //电话号码
    uint16_t tcpTimeout;             //tcp超时时间
    uint16_t udpTimeout;             //udp超时时间
    uint16_t tcpRepeatTimes;         //tcp重传次数
    uint16_t udpRepeatTimes;         //udp重传次数
    uint16_t smsTimeout;             //sms超时时间
    uint16_t smsRepeatTimes;         //sms重传次数
    uint32_t heratBeatTime;          //心跳时间
    uint32_t reserve[8];             //
}ServerInfo;

//通用应答
typedef struct{
    uint16_t responseNum;       //应答流水号
    uint16_t msgId;             //应答ID
    char result;    //结果
} CommonResponse;

//终端注册
typedef struct{
    uint16_t provinceID;          //省域ID
    uint16_t countyID;            //市县域ID
    char manufacturerID[5];   //制造商ID
    char deviceModel[20];      //终端型号
    char deviceID[7];         //终端ID
    char carColor;             //车牌颜色
    char carLicense[10];       //车牌
} RegisterMsg;

typedef struct{
    uint16_t reportNum;
    char result;
    char authNum[16];

} RegisterResponse;

//终端鉴权
typedef struct{
    char len;               //鉴权码长度
    char authID[16];        //鉴权码
    char deviceIMEI[15];    //
    char softWareV[20];     //软件版本
}AuthMsg;

#define DEFAULTLEN 60
//终端属性应答
typedef struct{
    uint16_t deviceType;       //终端类型
    char manufacturerID[5]; //制造商ID
    char deviceModel[20];   //终端型号
    char deviceID[7];      //终端ID
    char simID[10];         //SIM卡ICCID号
    char hardVersionLen;      //终端硬件版本长度
    char hardVersion[DEFAULTLEN];     //硬件版本
    char hardVersionLen1;     //固件版本长度
    char hardVersion1[DEFAULTLEN];    //固件版本
    char gnssAttr;          //GNSS模块属性
    char moduleAttr;     //通信模块属性
} DeviceAttrMsg;

typedef struct{
    uint16_t id;
    char len;
    char value[DEFAULTLEN];
} DeviceParamInfo;

//位置基本信息
typedef struct{
    uint32_t AlarmType;            //报警标志
    uint32_t Status;               //状态
    uint32_t Latitude;             //纬度
    uint32_t Longitude;            //经度
    uint16_t Altitude;           //海拔
    uint16_t Speed;              //速度
    uint16_t Direction;          //方向
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
    char ipLen;
    char ipAddr[16];
    uint16_t tcpPort;
    uint16_t udpPort;
    char channel;
    char dataType;
    char streamType;

} RealTimeStreamPack;

//回放音视频请求指令包
typedef struct{
    char ipLen;
    char ipAddr[16];
    uint16_t tcpPort;
    uint16_t udpPort;
    char channel;
    char dataType;
    char streamType;
    char playType;
    char speedCtr1;
    char speedCtr2;
    char startTime[6];
    char endTime[6];

} PlayBackStreamPack;


//音视频传输请求
typedef struct{               //字段            //直播           //回放
    char playType;        //判断直播与回放           0               1
    char ipLen;
    char ipAddr[16];      //ip地址                Y               Y
    uint16_t tcpPort;        //tcp端口               Y               Y
    uint16_t udpPort;        //udp端口               Y               Y
    char logicChannel;    //逻辑通道               Y               Y
    char dateType;        //数据类型               Y            音视频类型Y
    char streamType;      //码流类型               Y               Y
    char storeType;       //存储器类型                              Y
    char playBackType;    //回放模式                                Y
    char playRate;        //快进快推倍数                            Y
    char startTime[6];    //开始时间                                Y
    char endTime[6];      //结束时间                                Y
}StreamParam;

//查询文件报文
typedef struct{
    char logicChn;         //通道
    char startTime[6];     //开始时间
    char endTime[6];       //结束时间
    char alarmFlag[8];     //报警标志
    char fileType;         //文件类型
    char streamType;       //码流类型
    char storeType;        //存储器类型
} RecordFileMsg;

//查询文件应答
typedef struct{
    RecordFileMsg file;
    uint32_t fileSize;

} RecordFileInfo;

typedef struct{
    char logicChannel;         //通道
    char orderCtr;             //控制指令
    char closeStreamType;      //关闭音视频类型
    char changeStream;         //切换码流类型

} ReamTimeStreamControl;

typedef struct{                      //字段                 直播          回放
    char ipLen;
    char ipAddr[16];
    uint16_t port;
    char streamType;             //                0            1
    char logicChannel;           //通道              Y           Y
    char orderCtr;               //控制指令           Y           Y
    char cmd1;           //                   关闭音视频类型Y    快进快退倍数Y
    char cmd2;             //                  切换码流类型Y
    char posTime[6];             //                            控制视频回放位置Y
} StreamControl;

//圆形区域属性
typedef struct{
    uint32_t regionId;                  //区域ID
    uint32_t regionAttr;                //区域属性
    uint32_t centerLatitude;            //中心点纬度
    uint32_t centerLongitude;           //中心点经度
    uint32_t regionRadius;              //半径
    uchar  startTime[6];                //开始时间
    uchar  endTime[6];                  //结束时间
    uint32_t topSpeed;                  //最高速度
    uint32_t overSpeedDuration;         //超速持续时间
} CicularRegionAttr;

//矩形区域属性
typedef struct{
    uint32_t regionId;                    //区域ID
    uint32_t regionAttr;                  //区域属性
    uint32_t leftTopLatitude;             //左上角纬度
    uint32_t leftTopLongitude;            //左上角经度
    uint32_t rightLowLatitude;            //右下角纬度
    uint32_t rightLowLongitude;           //右下角经度
    uchar  startTime[6];                  //开始时间
    uchar  endTime[6];                    //结束时间
    uint32_t topSpeed;                    //最高速度
    uint32_t overSpeedDuration;           //超速持续时间
} RectangleRegionAttr;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // JTT808DEFINE_H
