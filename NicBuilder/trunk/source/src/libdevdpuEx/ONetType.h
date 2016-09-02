#ifndef __NICSYS_DEV_ONETTYPE_H_
#define __NICSYS_DEV_ONETTYPE_H_
#include "ONetConst.h"
#include "convert_type.h"
//#include "RW_Thread_Mutex.h"

#include <pshpack1.h>

/** 通信协议中一个帧的数据包的格式 */
typedef struct FD_PACKET_st {
	__u8 sync_header;		/**< 同步头 */
	__u8 dest_addr;		/**< 目的 */
	__u8 source_addr;		/**< 源 */
	__u8 length_low;		/**< 长度低 */
	__u8 length_high;		/**< 长度高，作为协议版本号 */
	__u8 control_area_1;	/**< 控制域1 */
	__u8 control_area_2;	/**< 控制域2 */
	__u8 control_area_3;	/**< 控制域3 */
	__u8 protocol_type;	/**< 协议类型 */
	__u8 info_code;		/**< 信息码 */
	__u8 description_code;	/**< 描述码 */
	__u8 * data;			/**< 数据 */
	__u16 crc16;			/**< CRC16校验 */
}FD_PACKET_ONet;





/** 描述IED卡的静态属性信息 */
struct IED_CARD_ATTR {
	__u8 ioid;				/**< 物理地址，槽号 */
	__u8 ioaddr;			/**< IED板逻辑地址 */
	__u8 status;			/**< 初始态0x01; 配置态0x02; 运行态0x11; 死态0x4F; 错误态0x41; 通讯中断0x42; 配置错误0x44 */
	__u8 type;				/**< IED模板类型 */
	__u16 iotype;			/**< IED板卡类型，低字节->高字节 */
};

/** 描述IED的状态信息，应由DPBUS协议确定具体结构 */
struct IED_CARD_VDI {
	__u8 vdilen;		/**< 数据长度 */
	__u8 * vdidata;		/**< 数据区，长度为vdilen字节 */
};

/** 描述IED上送的数据和质量码，同时也存放着IED的数据类型描述 */
struct IED_CARD_VALUE {
	__u8 qlenHigh;		/**< 质量码长度，高位 */
	__u8 qlenLow;		/**< 质量码长度，低位 */
	__u8 * quality;		/**< 质量码数据区，长度为qlen字节 */
	__u8 vlenHigh;		/**< 数据区长度，高位 */
	__u8 vlenLow;		/**< 数据区长度，低位 */
	__u8 * vvalue;		/**< 数据区，长度为vlen字节 */
};


/** 描述VPT卡的静态属性信息 */
struct VPT_CARD_ATTR {
	__u8 ioid;				/**< 物理地址，槽号 */
	__u8 ioaddr;			/**< VPT板逻辑地址 */
	__u8 status;			/**< 初始态0x01; 配置态0x02; 运行态0x11; 死态0x4F; 错误态0x41; 通讯中断0x42; 配置错误0x44 */
	__u8 type;				/**< VPT模板类型 */
	__u8 linesHigh;			/**< VPT的点组数量(高位) */
	__u8 linesLow;			/**< VPT的点组数量(低位) */
};

/** 描述VPT的质量码长度，为"0x0000" */
struct VPT_CARD_QUALIFY {
	__u16 qualify;		/**< 质量码长度 */
};

/** 描述VPT上送的数据 */
struct VPT_CARD_VALUE {
	__u8 type;			/**< VPT点组类型 */
	__u8 vlenHigh;		/**< 数据区长度，高位 */
	__u8 vlenLow;		/**< 数据区长度，低位 */
	__u8 * vvalue;		/**< 数据区，长度为vlen字节 */
};


/** 描述IO卡的静态属性信息 */
struct IO_CARD_ATTR
{
	__u8 ioID;				/**< 物理地址，槽号 */
	__u8 ioAddr;			/**< IO板逻辑地址 */
	__u8 Status;			/**< 初始态0x01; 配置态0x02; 运行态0x11; 死态0x4F; 错误态0x41; 通讯中断0x42; 配置错误0x44 */
	__u8 Type;				/**< IO模板类型 */
	__u16 ioType;			/**< IO板卡类型，低字节->高字节 */
	__u8 ioSoftVer[7];		/**< IO板软件版本：软件版本号(低字节)->软件版本号(高字节)->软件写入日期:日->软件写入日期:月
								* ->软件写入日期:年(实际年与2000的差值)->CRC校验码低值->CRC校验码高值 */
	__u8 ctrlDpu;			/**<  */
};								

/** 描述IO卡的动态属性信息 */
struct IO_DYNA_ATTR
{
	__u8 ioId;				/**< 物理地址，槽号 */
	__u8 ioAddr;			/**< IO板逻辑地址 */
	__u8 status;			/**< 初始态0x01; 配置态0x02; 运行态0x11; 死态0x4F; 错误态0x41; 通讯中断0x42; 配置错误0x44 */
	__u8 Type;				/**< IO模板类型 */
};

/** 描述IO的状态信息，应由DPBUS协议确定具体结构 */
struct IO_VDI {
	__u8 vdilen;				/**< 数据长度 */
	__u8 vdidata[14];	/**< 数据区，长度为vdilen字节 */
};

/** 描述IO卡的数据类型和访问描述 */
struct IO_DATA_ATTR
{
	__u8 Type;					/**< 点类型：BIT位0x1F; 四字节符点0x24; 三字节符点0x23; 二字节符点0x22; 四字节无符号整型0x34;
									* 三字节无符号整型0x32; 二字节无符号整型0x31; 四字节符号整型0x44; 三字节符号整型0x42; 二字节符号整型0x41 */
	__u8 Mode;					/**< r=01;w=02; 读写权限 */
	__u16 num;					/**< 点数目 */
};

/** 描述IO上送的数据和质量码，同时也存放着IO的数据类型描述 */
struct IO_VALUE
{
	__u8 qLen;					/**< 质量码长度 */
	__u8 *quaData;					/**< 质量码数据区，长度为qlen字节 */
	__u8 vLen;					/**< 数据区长度 */
	__u8 *valData;				/**< 数据区，长度为vlen字节 */
	__u16 ptNum;				/**< 测点数目 */
	IO_DATA_ATTR attr[4];			/**< 数据类型描述，最多支持4种类型 */
};

struct IO_DYNA_VALUE
{
	__u8 qLen;					/**< 质量码长度 */
	__u8 *quaData;					/**< 质量码数据区，长度为qlen字节 */
	__u8 vLen;					/**< 数据区长度 */
	__u8 *valData;				/**< 数据区，长度为vlen字节 */
};									

struct DPU_ATTR
{
	__u8 dpuId;			//ID =255
	__u8 dpuAddr;		//逻辑地址 1-99
	__u16 dpuStatus;	// Byte0:默认主CM的DPU状态; Bit0:主机; Bit1:备机; Bit2:错误态; Bit3:冗余使用\n
										// Byte1:默认备CM的DPU状态; Bit0:主机; Bit1:备机; Bit2:错误态; Bit3:冗余使用 */
	__u8 dpuType;		//类型
	__u8 dpuIOCap;		//最大IO数目
	__u8 dpuIOSize;		//最大实IO数目
	__u8 dpuVirIOSize;	//最大虚IO数目	
	__u8 dpuRunIONum;	//运行实IO数目
	__u8 dpuRunVirIONum;//运行虚IO数目
	char dpuVersion[32];	//DPU软件版本号
	char	plcVersion[32];	//PLC软件版本号
	__u32 oNetIpAddr1;	//IP地址1
	__u32 oNetIpAddr2;	//IP地址2
	__u32 oNetIpAddr3;	//IP地址3
	__u32 oNetIpAddr4;	//IP地址4
	__u32 syncIpAddr;	//同步IP地址
};
struct DPU_STATUS
{
	__u8 temp;			//温度
	__u8 cpu;			//CPU利用率（%）
	__u8 mem;			//内存使用[MByte]
	__u8 hardware;		//硬件状态
	__u8 dpBus1;			//DpBus1状态
	__u8 dpBus2;			//DpBus2状态
	__u8 comm;			//comm状态
	__u8 heartbeat;		//同步心跳线状态	
	__u8 dpbus_state;	//DP主站状态
	__u8 baud_ratestatus;//dpbus主站波特率
	__u32 HostSendCount;							//主站发送总次数
	__u32 HostRecvCount;							//主站接收总次数
	__u32 HostErrorRecvCount;						//主站接收的无效或未响应帧次数统计
	__u32 PrimaryOverRun_Count;						//主通道接收中的OverRun错误次数统计
	__u32 SecondaryOverRun_Count;					//备通道接收中的OverRun错误次数统计
	__u32 PrimaryParity_Count;						//主通道接收中的Parity错误次数统计
	__u32 SecondaryParity_Count;					//备通道接收中的Parity错误次数统计
	__u32 PrimaryFrameClosed_Count;					//主通道接收中的Frame Closed错误次数统计
	__u32 SecondaryFrameClosed_Count;				//备通道接收中的Frame Closed错误次数统计
	__u16 Send_MaxTimeSpend;						//CPU发送一帧所耗最大时间长度
	__u16 Recv_MaxTimeSpend;						//CPU接收一帧所耗最大时间长度
};

struct DPU_DYNA_ATTR
{
	__u8 dpuAddr;			//逻辑地址
	__u8 dpuStatus;			//状态
	__u8 dpuType;			//类型
	__u8 Real_io_num;		//实际运行的IO个数
	__u8 vir_io_num;		//实际运行的虚拟IO个数
};

/** 描述上送的事件，一共52个字节 */
struct IO_DYNA_EVENT {
	__u8 dpuaddr;			/**< DPU地址 */
	__u8 ioaddr;			/**< io地址，若是全装置，为FFH */
	__u8 time56[7];			/**< 7字节时标 */
	__u8 dputype;			/**< DPU类型 */
	__u8 eventtype;		/**< 事件类型，分为告警、动作、运行三种，见"事件类型编码" */
	__u8 eventcode;		/**< 事件编码，见“详细事件编码”，保留 */
	__u8 eventdesp[40];		/**< 40个字节的文本描述事件，若参数不足40个将剩余空间置0，内容描述如EVENT_DESP */
};

/**
* @name 通道结构体
* @{
*/
//模拟量的实际地址可以根据startAddr+subCHnum *sizeof(Type)得出，
//开关量的实际地址可以根据startAddr|subCHnum得出
typedef struct NICSYS_CHANNEL_st
{
	//静态属性
	__u8 dpuAddr;		//DPU地址
	__u8 ioAddr;		//IO卡逻辑地址
	__u8 status;		//通道的状态
	__u16 offAddr;		//VPT类型时为内部内存中的偏移地址,IO类型时为IO卡的通道号,以0起计数
	__u8  subOffAddr;	//开关量字节内偏移通道号，从0 开始到 7，模拟量常为0
	__u8 ioModType;		//IO类型板卡的具体类型
	__u8  dataType;		//DPU实际定义的数据类型
	//动态属性

	DBData   rData;		//实时值，此中包含的数据类型为二层定义的数据类型
}nicONetChannel;



/**
* @name IO卡结构体
* @{
*/
typedef struct NICSYS_IOCARD_st
{
	//静态属性
	//动态属性
	__u8 dpuAddr;			//DPU逻辑地址
	__u8 ioId;				//IO的插槽号
	__u8 ioAddr;			//IO的逻辑地址
	__u8 ioStatus;			//IO状态	
	__bool isExist;			//判断卡是否存在
	__u8 ioModType;				/**< IO模板类型，物理类型 AI_8 DI_16等 */
	__u16 ioCardType;			/**< IO类型，VPT IED IOCard低字节->高字节 */
	__i32 ioCurNum;				//当前IO卡的通道点数
	nicONetChannel *channels ;	//由于VPT类型的IO卡通道数量不定，只能设置为不定长
}nicONetIOCard;


/**
* @name ONet协议缓冲区类
* @{
*/
//全局唯一的数据缓冲
class ONet_DB
{
public:
	ONet_DB()
	{
		ACE_OS::memset(_staticAttr,0,sizeof(DPU_ATTR) *2);
		//初始化dpu状态是运行的，这可以在初次连接的做逻辑判断
		_staticAttr[0].dpuStatus = 0x0001;
		_staticAttr[1].dpuStatus = 0x0001;
		ACE_OS::memset(_dynaAttr,0,sizeof(DPU_DYNA_ATTR) *2);
		ACE_OS::memset(_status,0,sizeof(DPU_STATUS) *2);
		_IOCardsCount =0;
		ACE_OS::memset(ioCards,0,sizeof(nicONetIOCard) *IOCARD_MAX_SIZE);
		for (__u16 i = 0 ;i < IOCARD_MAX_SIZE;i++)
		{
			ioCards[i].isExist = __false;
		}
	}

	//静态属性
	DPU_ATTR   _staticAttr[2];
	//动态属性
	DPU_DYNA_ATTR _dynaAttr[2];

	//状态
	DPU_STATUS  _status[2];
	__u16 _IOCardsCount;
	__u8   _netStatus0;//主CP，A网状态
	__u8   _netStatus1;//主CP，B网状态
	__u8   _netStatus2;//备CP，A网状态
	__u8   _netStatus3;//备CP，B网状态
	ACE_RW_Thread_Mutex _data_mutex;	/**< 一个控制器分配一个数据区的读写锁 */
	nicONetIOCard ioCards[IOCARD_MAX_SIZE];//全局有两个控制器，但只有一套IO卡
};

#include <poppack.h>


#endif