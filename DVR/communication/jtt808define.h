#ifndef JTT808DEFINE_H
#define JTT808DEFINE_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

//终端注册
typedef struct{
    short ProvinceID;          //省域ID
    short CountyID;            //市县域ID
    char ManufacturerID[11];   //制造商ID
    char DeviceModel[30];      //终端型号
    char DeviceID[30];         //终端ID
    char CarColor;             //车牌颜色
    char CarLicense[16];       //车牌
} RegisterMsg;

typedef struct{
    short reportNum;
    char result;
    char authNum[16];

} RegisterResult;

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
    char DeviceModel[30];   //终端型号
    char DeviceID[30];      //终端ID
    char ICCID[10];         //SIM卡ICCID号
    char HWVersionLen;      //终端硬件版本长度
    char HwVersion[DEFAULTLEN];     //硬件版本
    char HWVersionLen1;     //固件版本长度
    char HwVersion1[DEFAULTLEN];    //固件版本
    char GNSSAttr;          //GNSS模块属性
    char ComModuleAttr;     //通信模块属性
} DeviceAttrMsg;


//位置基本信息
typedef struct{
    int AlarmType;            //报警标志
    int Status;               //状态
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


//音视频传输请求
typedef struct{
    char PlayType;        //判断直播与回放
    char IpAddr[16];      //ip地址
    short TcpPort;        //tcp端口
    short UdpPort;        //udp端口
    char LogicChannel;    //逻辑通道
    char DateType;        //数据类型
    char StreamType;      //码流类型 0：主码流 1：子码流
    char StoreType;       //存储器类型
    char PlayBackType;    //回放模式
    char PlayRate;        //快进快推倍数
    char StartTime[6];    //开始时间
    char EndTime[6];      //结束时间
}StreamParam;

typedef struct{
    char LogicChannel;         //通道
    char OrderCtr;             //控制指令
    char CloseStreamType;      //关闭音视频类型
    char ChangeStream;         //切换码流类型

} ReamTimeStreamControl;

typedef struct{
    int LogicChannel;           //通道
    int OrderCtr;               //控制指令
    int PlayBackRate;           //快进快退速率
    unsigned PosTime;             //控制视频回放位置
} StreamControl;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // JTT808DEFINE_H
