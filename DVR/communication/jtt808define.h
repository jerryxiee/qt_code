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
//typedef struct{
//    uint32_t AlarmType;            //报警标志
//    uint32_t Status;               //状态
//    uint32_t Latitude;             //纬度
//    uint32_t Longitude;            //经度
//    uint16_t Altitude;           //海拔
//    uint16_t Speed;              //速度
//    uint16_t Direction;          //方向
//    char time[6];             //时间

//} PositionBaseMsg;


// 报警标志位
typedef   union
{
  struct Bit
  {
    uint32_t sos:1;  // 紧急报瞥触动报警开关后触发
    uint32_t overspeed:1;  // 超速报警
    uint32_t fatigue:1;  // 疲劳驾驶
    uint32_t earlywarning:1;  // 预警
    uint32_t gnssfault:1;  // GNSS模块发生故障
    uint32_t gnssantennacut:1;  // GNSS天线未接或被剪断
    uint32_t gnssantennashortcircuit:1;  // GNSS天线短路
    uint32_t powerlow:1;  // 终端主电源欠压

    uint32_t powercut:1;  // 终端主电源掉电
    uint32_t lcdfault:1;  // 终端LCD或显示器故障
    uint32_t ttsfault:1;  // TTS模块故障
    uint32_t camerafault:1;  // 摄像头故障
    uint32_t obddtc:1;  // OBD故障码
    uint32_t retain1:5;  // 保留

    uint32_t daydriveovertime:1;  // 当天累计驾驶超时
    uint32_t stopdrivingovertime:1;  // 超时停车
    uint32_t inoutarea:1;  // 进出区域
    uint32_t inoutroad:1;  // 进出路线
    uint32_t roaddrivetime:1;  // 路段行驶时间不足/过长
    uint32_t roaddeviate:1;   // 路线偏离报警
    uint32_t vssfault:1;  // 车辆VSS故障
    uint32_t oilfault:1;  // 车辆油量异常
    uint32_t caralarm:1;  // 车辆被盗(通过车辆防盗器)
    uint32_t caraccalarm:1;  // 车辆非法点火
    uint32_t carmove:1;  // 车辆非法位移
    uint32_t collision:1;  // 碰撞侧翻报警
    uint32_t opendoor:1;  // 非法开门
    uint32_t retain2:1;  // 保留
  }bit;
  uint32_t value;
} AlarmFlagBit;

// 状态位
typedef   union
{
  struct Bit
  {
    uint32_t acc:1;  // ACC 0:ACC关; 1:ACC开
    uint32_t location:1;  // 定位 0:未定位; 1:定位
    uint32_t snlatitude:1;  // 0:北纬: 1:南纬
    uint32_t ewlongitude:1;  // 0:东经; 1:西经
    uint32_t operation:1;  // 0:运营状态; 1:停运状态
    uint32_t gpsencrypt:1;  // 0:经纬度未经保密插件加密;
                            // 1:经纬度已经保密插件加密
    uint32_t retain1:2;      // 保留
    uint32_t trip_stat:2;  // 00: 空车; 01: 半载; 10: 保留; 11: 满载;
    uint32_t oilcut:1;  // 0:车辆油路正常; 1:车辆油路断开
    uint32_t circuitcut:1;  // 0:车辆电路正常; 1:车辆电路断开
    uint32_t doorlock:1;  // 0:车门解锁; 1: 车门加锁
    uint32_t door1status:1;  // 0:门 1 关; 1: 门 1 开; (前门)
    uint32_t door2status:1;  // 0:门 2 关; 1: 门 2 开; (中门)
    uint32_t door3status:1;  // 0:门 3 关; 1: 门 3 开; (后门)
    uint32_t door4status:1;  // 0:门 4 关; 1: 门 4 开; (驾驶席门)
    uint32_t door5status:1;  // 0:门 5 关; 1: 门 5 开; (自定义)
    uint32_t gpsen:1;  // 0: 未使用 GPS 卫星进行定位;
                       // 1: 使用 GPS 卫星进行定位;
    uint32_t beidouen:1;  // 0: 未使用北斗卫星进行定位;
                          // 1: 使用北斗卫星进行定位;
    uint32_t glonassen:1;  // 0: 未使用 GLONASS 卫星进行定位;
                           // 1: 使用 GLONASS 卫星进行定位;
    uint32_t galileoen:1;  // 0: 未使用 Galileo 卫星进行定位;
                           // 1: 使用 Galileo 卫星进行定位;
    uint32_t retain2:10;  // 保留
  }bit;
  uint32_t value;
} StatusFlagBit;

// 位置基本信息数据
typedef   struct
{
  uint32_t alarm;		//报警标志
  uint32_t status;      //状态标志
  uint32_t latitude;    // 纬度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度)
  uint32_t longitude;   // 经度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度)
  uint16_t atitude;     // 海拔高度, 单位为米(m)
  uint16_t speed;       // 速度 1/10km/h
  uint16_t bearing;     // 方向 0-359,正北为0, 顺时针
  uint8_t time[6];      // 时间 BCD[6] YY-MM-DD-hh-mm-ss(GMT+8时间, 本标准之后涉及的时间均采用此时区)
} PositionBaseInfo;

// 位置附加信息项
typedef   struct
{
  uint8_t item_id;
  uint8_t item_len;
  uint8_t item_value[8];
} PositionExtensionInfo;

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
    char  startTime[6];                //开始时间
    char  endTime[6];                  //结束时间
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
    char  startTime[6];                  //开始时间
    char  endTime[6];                    //结束时间
    uint32_t topSpeed;                    //最高速度
    uint32_t overSpeedDuration;           //超速持续时间
} RectangleRegionAttr;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // JTT808DEFINE_H
