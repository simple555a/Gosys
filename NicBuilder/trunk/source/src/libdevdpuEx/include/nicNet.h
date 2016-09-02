#ifndef Nicsys_NET_H_
#define Nicsys_NET_H_

#include "nicDefs.h"
#include "nicRefACE.h"

//多播目标地址的标志位，最多可以有32个目标
typedef enum
{
	Cast_Net = (1<< 0),			//发送给所有客户端标识
	Cast_DB = (1<< 1),			//发送给内存数据库
	Cast_Alarm = (1<< 2),			//发送给报警模块
	Cast_IOSS = (1<< 3),			//发送给IOSS模块
	Cast_Tran = (1<< 4),			//多发送给数据传送模块
	Cast_Config = (1<<5),		//发送给数据库管理模块
	Cast_Hisd = (1<< 6),			//发送给历史数据存储	
	Cast_Log = (1<< 7),			//发送给Log模块
	Cast_SOE = (1<< 8),			//发送给SOE模块
	Cast_Event =(1<< 9),		//发送给事件存储模块
	Cast_AlarmHisd = (1<< 10),	//发送给报警存储	
	Cast_Backup = (1<< 11),		//发送给主备模块
	Cast_Swtich = (1<< 12),		//发送给网络交换机模块
}CastTagertFlag;

//在目标地址标志位基础上总结出的所有多播可能产生的消息类型
typedef enum
{
	MsgStyle_DB_CastAll = Cast_Net
						|Cast_Alarm
						|Cast_IOSS
						|Cast_Tran
						|Cast_Config,
	MsgStyle_DB_CastLocal = Cast_Alarm
							|Cast_IOSS
							|Cast_Tran
							|Cast_Config,
	MsgStyle_CastNet = Cast_Net,			//报警和内存数据库都有网网络客户端多播数据的消息包
	MsgStyle_DB_CastAlarm = Cast_Alarm,
	MsgStyle_DB_CastIOSS = Cast_IOSS,
	MsgStyle_DB_CastTran = Cast_Tran,
	MsgStyle_DB_CastConfig = Cast_Config,
	MsgStyle_Alarm_CastAll= Cast_Tran
							|Cast_Config
							|Cast_Net,
	MsgStyle_IOSS_CastLocal= Cast_DB
							|Cast_Alarm
							|Cast_Tran
							|Cast_Config,

}MsgStyleFlag;

//消息头结构定义
//包头特征分析
//1.客户端应用程序--->服务器端某一个进程：不为返回包，不为多播包，
//         目标掩码高16位为服务器通信句柄，下班部分为服务端处理进程句柄
//		   源掩码上班部分为客户端通信句柄，低16位为客户端应用程序进程句柄
//2.服务器端某一个进程--->客户端应用程序:为返回包，不为多播包
//         源掩码高16位为服务器通信句柄，低16位为服务端处理进程句柄
//		   目标掩码上班部分为客户端通信句柄，低16位为客户端应用程序进程句柄  
//3.服务器端进程的多播:为返回包，为多播包
//			源掩码高16位为服务器通信句柄，下班部分为服务端处理进程句柄
//			目标掩码为32位进程标识SwitchTagertFlag，一个进程占一位。目标为所有的客户端模式占一位。
//       XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
//       
typedef struct 
{
	nicUInt16 flag;   							  //包头标示
	nicUInt8  msgPriority;					//数据包的处理优先级
	nicUInt32 msgLength;					  //数据包总长度，包括包尾
	nicUInt32 serialNo;						 //序列号
	nicUInt16 functionNo;						  //函数编号
	nicHandle localHandle;                    //本地句柄
	nicUInt32 sourceMask;						//数据包源目标掩码
	nicUInt32 targetMask;						//数据包目标掩码
	nicUInt8 hasLocalHandle : 1;                  //是否有本地句柄,肯定会有，将字段放在包头，此字段暂留
	nicUInt8 hasRemoteHandle : 1;                 //是否有远程句柄
	nicBool  isMultCast : 1;                      //是否是多播
	nicUInt8 hasSequnence : 1;					  //是否有后继包，暂留。
	nicUInt8 hasCompressed : 1;                   //是否压缩
	nicUInt8 hasVerify : 1;                       //是否校验
	nicUInt8 waitReturn :1; 					  //是否等待返回
	nicBool isReturnPack : 1;                   //是否返回包
}msgHeader;

#define get_msg_souceNetid(head) ((head->sourceMask >> 16))
#define get_msg_souceAppid(head) ((head->sourceMask & 0x0000ffff))
#define get_msg_targetNetid(head) ((head->targetMask >> 16))
#define get_msg_targetAppid(head) ((head->targetMask & 0x0000ffff))

#define set_msg_souceNetid(head,id) \
do \
{   \
	head->sourceMask = (head->sourceMask & 0x0000FFFF) |(id << 16);  \
} while (0)

#define set_msg_souceAppid(head,id) \
do \
{   \
	head->sourceMask = (head->sourceMask &0xFFFF0000) |id;   \
} while (0)

#define set_msg_targetNetid(head,id) \
do \
{   \
	head->targetMask = (head->targetMask & 0x0000FFFF) |(id << 16);  \
} while (0);

#define set_msg_targetAppid(head,id)\
do \
{   \
	head->targetMask = (head->targetMask &0xFFFF0000) |id;   \
} while (0)



/// <summary> 
///	IPC，NET通信回调函数序号定义，此定义包含IPC和NET两种通信方式，也是服务器端与客户端的共同网络事务定义
/// 跟Connection相关的定义是指具体的某个连接，而其他的则是指服务端或者客户端的状态，一个服务端或者客户端可以有多个connection
/// </summary> 

#define  callBackFuncsCount						5
#define  idGetMsgBlcokDataCallBack				0		//连接中断
#define  idGetCSProccessStatusCallBack			1		//接收到新信息
#define  idGetConnectionStatusCallBack			2		//Client & Server进程的运行状态
#define  idNewConnectionCallBack				3		//连接状态回调
#define  idConnectionInteruptCallBack			4		//得到新连接

/// <summary> 
///		从客户端收到数据，回调转发给调度程序
/// </summary> 
/// <param name="mBlock">
///		待转发数据。
/// </param>
/// <param name="timeOut">
///		数据发送超时时间，超时则丢弃
/// </param>
/// <param name="CLientHandle">
///		客户端连接到服务器的物理连接ID
/// </param>
/// <returns>
///		不返回。
/// </returns>

typedef nicVoid (nicAPI *getMsgBlcokDataCallBack) (
	nicUInt16 nNetId,
	ACE_Message_Block * mBlock);

/// <summary> 
///		服务器网络端状态回调通知
/// </summary> 
/// <param name="status">
///		服务器运行状态
/// </param>
/// <param name="CLientHandle">
///		客户端连接到服务器的物理连接ID
/// </param>
/// <returns>
///		不返回。
/// </returns>
typedef nicVoid (nicAPI *getCSProccessStatusCallBack) (
	nicProcessStatus  status);

/// <summary> 
///		服务器网络端连接状态回调通知
/// </summary> 
/// <param name="pServer">
///		网络连接句柄
/// </param>
/// <param name="status">
///		句柄对应的网络连接目前状态
/// </param>
/// <param name="CLientHandle">
///		客户端连接到服务器的物理连接ID
/// </param>
/// <returns>
///		不返回。
/// </returns>
typedef nicVoid (nicAPI *getConnectionStatusCallBack) (
	nicUInt16 ClientHandle,
	nicConnectionStatus  status);

/// <summary> 
///		服务器网络端新连接接入调通知
/// </summary> 
/// <param name="pServer">
///		网络连接句柄
/// </param>
/// <param name="status">
///		句柄对应的网络连接目前状态
/// </param>
/// <param name="CLientHandle">
///		客户端连接到服务器的物理连接ID
/// </param>
/// <returns>
///		不返回。
/// </returns>
typedef nicVoid (nicAPI *newConnectionCallBack) (
	nicUInt16 ClientHandle,
	nicVoid *  args);

/// <summary> 
///		服务器网络端连接断开回调通知
/// </summary> 
/// <param name="status">
///		句柄对应的网络连接目前状态
/// </param>
/// <param name="CLientHandle">
///		客户端连接到服务器的物理连接ID
/// </param>
/// <returns>s
///		不返回。
/// </returns>
typedef nicVoid (nicAPI * connectionInteruptCallBack) (
	nicHandle ClientHandle,
	nicVoid *  args);	

#define MSG_HEADER_SIZE                            28

//网络默认端口号
const nicUInt32 DEFUALT_NET_PPORT =	8889;
//IPC默认端口号
const nicUInt32 DEFUALT_IPC_PPORT =	9090;
//消息报包尾标志
const nicUInt16 BEGFLAG  =	0x5a5a;

const nicUInt16 ENDFLAG	 =	0x6a6a;	

//默认可以链接的服务器最多数量
const nicUInt16 DEFAULT_SERVER_COUNT = 16;

//序列化缓冲区最大默认长度
const nicUInt32 PSMAX_BUFFER_SIZE = 1024*1024;

//缓冲区初始默认长度
const nicUInt32 DEFAULT_BUFFER_SIZE = 8192;

//序列号最大值
const nicUInt32 nicMAX_SER_NO =  2147483648u;

//分配给回调函数的序列号基数
const nicUInt32 nicCALLBACK_BASE = 2147483548u;

//系统超时时间设置
const nicUInt32 nicMax_TIME_OUT = 10;

//设置对Message_queue操作的统一操作超时时间
const nicUInt32 nicMQ_OP_TIMEOUT = 2;

//断线重连次数
const nicUInt32 nicReconnect_Times = 5;

//x线程沉睡时间，便于调用下次容器操作
const nicInt32 PSPACECONT_SLEEP_TIME = 2;

//默认消息包大小
const size_t DEFAULT_MB_SIZE = 1024;

//默认的支持最大连接数
const nicUInt32 MAX_CONNECT_COUNT = 128;

//定义网络断开标志（）
const nicInt32 NONEEDRECONNECT = -1;

//定义网络需要重连标志
const nicInt32 NEEDCONNECT = 1;

//定义网络不需要重连标志
const nicInt32 NONEEDCONNECT = 0;

//定义IP地址的最大长度
const nicInt32 MAX_IP_SIZE = 16;

const nicInt32 MAX_NODE_SIZE = MAX_IP_SIZE;

const nicStr  DEFUALT_NODENAME =("NODE");


#endif/**/
