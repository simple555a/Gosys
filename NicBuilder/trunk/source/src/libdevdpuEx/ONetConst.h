//= UPDATE RECORD
//= Update for    Version     Update by     Date         UpdateUpdate (function names modified)     
//= (22-1)        v1.0.023    Zander Sun    2014-10-13   modify MAX_SIZE_RECV_BUF

#ifndef __NICSYS_DEV_ONETCONST_H_
#define __NICSYS_DEV_ONETCONST_H_



namespace ONet_Const
{
	enum
	{
		CMD_DOWN_CALL = 0x03,				//一次性召唤命令
		CMD_DOWN_SUBSCRIBE = 0x02,			//开始订阅命令
		CMD_CANCEL_SUBSCRIBE = 0x01,		//取消订阅命令
	};

	/**
	* @name 协议类型
	* @{
	*/
	enum
	{
		HIGH_BYTE_ORDER = 0x80,
		ONET_REAL_TRANSFER_PROTOCOL = 0x08|HIGH_BYTE_ORDER,
		ONET_DATA_EXCHANGE_PROTOCOL = 0x09|HIGH_BYTE_ORDER,
		CNET_REAL_TRANSFER_PROTOCOL = 0x0A|HIGH_BYTE_ORDER,
		CNET_TRANSFER_CONTROL_PROTOCOL = 0x0B |HIGH_BYTE_ORDER,
	};
	/**
	* @name 链路建立帧类型
	* @{
	*/
	enum
	{
		FRAM_STYLE_GENERAL = 0x0,	//普通数据帧
		FRAM_STYLE_T1 = 0x01,		//T1时间帧
		FRAM_STYLE_T2 = 0x02,		//T2时间帧
		FRAM_STYLE_LINK= 0x03,		//链路建立帧
	};
	/**
	* @name 链路建立帧类型
	* @{
	*/
	enum
	{
		LINK_STYLE_NULL = 0,		//无效的链路帧	
		LINK_STYLE_TEST = 1,		//测试链路帧	
		LINK_STYLE_START = 2,		//建立链路帧
		LINK_STYLE_STOP = 3,		//停止链路帧
	};
	enum
	{
		LINK_NET_0 =0x01,	//1网注册	
		LINK_NET_1 =0x02,	//2网注册
	};
	enum IO_STATUS
	{
		IO_STATUS_INIT = 0x01,
		IO_STATUS_CONF = 0x02,
		IO_STATUS_RUN = 0x11,
		IO_STATUS_STOP = 0x4F,
		IO_STATUS_ERR = 0x41,
		IO_STATUS_NETERR = 0x42,
		IO_STATUS_CONFERR = 0x44,
	};
	/** 板卡类型 */
	enum IO_TYPE_T {
		IO_CARD,				/**< IO卡 */
		IED_CARD,				/**< IED卡 */
		VPT_CARD,				/**< VPT卡 */
	};


};
#define  DEFUALT_LINKTEST_TIME 2   //默认的链路测试时间
#define  INVALID_ONET_CODE	0x00 //无效的协议信息字节或者无效的地址
#define  MUTIL_PACKET_END_MARK	 0x80		//多帧的最后一帧掩码
#define  PROCOTOL_STYLE_MARK	 0x03		// 协议类型掩码
#define  DPU_MAX_SIZE   16		// DPU最大个数
#define  IOCARD_MAX_SIZE   256		// IO卡最大个数
#define  CHANNEL_MAX_SIZE   256		// IO卡的通道最大个数



#define MAX_IO_CARD 256						/**< IO卡最大个数 */
#define NO_CRC_SEND							/**< 发送过程中不发送CRC信息 */
#define MAX_SIZE_BUF 4096					/**< 单个数据包最大长度 */
#define MAX_SIZE_RECV_BUF MAX_SIZE_BUF*4	/**< 接收缓冲区最大长度 */  // Zander 2014-10-13 OLD: *10  
#define SERV_PORT 2008						/**< 服务端DPU的端口号 */
#define DEFAULT_TIMEOUT 500*1000			/**< 接收数据的默认超时时间为30*1000us，即30ms */
#define DEFAULT_T1 3						/**< 默认T1心跳超时时间为3s */
#define DEFAULT_PERIOD_TIME 100				/**< 默认对控制器设定的定时发送全数据时间 */
#define MAX_DPU_NUM 100						/**< DPU逻辑地址上限，实际DPU逻辑地址为1~MAX_DPU_NUM-1 */

#define LOW_8_BIT(x) (char)(x & 0xFF)			/**< 获得一个数的低8bit位 */
#define HIGH_8_BIT(x) (char)((x & 0xFF00) >> 8)	/**< 获得一个16位数的高8bit位 */

/**
* @name 报头
* @{
*/
#define SYNC_HEADER 0x68				/**< 数据包头部第一个字节的内容 */
#define PROTOCOL_VERSION 0x00			/**< 为保持协议的兼容性，长度的高四位作为协议的版本号 */
#define LENGTH_HIGH_MASK 0x0F			/**< 长度的高四位的掩码 */
#define NO_CRC_CHECK 0x1616	/**< 如果不使用CRC码，则这两个字节为0x16, 0x16 */
/** @} */



/**
* @name IO板卡模板类型
* @{
*/
#define AI_8_1		0x01	/**< 8个通道，数据有效性采用AI_1型 */
#define AI_4_1		0x02	/**< 4个通道，数据有效性采用AI_1型 */
#define AI_10_2		0x08	/**< 6个通道，4个模态，数据有效性采用AI_2型 */
#define LAI_16_1	0x09	/**< 16个通道，数据有效性采用LIO型 */
#define AI_19_2		0x0A	/**< 8个通道（需上送实部和虚部），3个频率，数据有效性采用AI_3型 */
#define DI_16_1		0x10	/**< 16个通道，数据有效性采用DI_1型 */
#define PI_8_1		0x11	/**< 8个通道，数据有效性采用PI_1型 */
#define PI_12_1		0x12	/**< 2个通道，10个模态，数据有效性采用PI_1型 */
#define LDI_32_1	0x13	/**< 32个通道，数据有效性采用LIO型 */
#define LDI_64_1	0x14	/**< 64个通道，数据有效性采用LIO型 */
#define LPI_8_1		0x15	/**< 8个通道，数据有效性采用LIO型 */
#define AM_8_1		0x20	/**< 8个通道，1~4通道数据有效性采用AI_1型、5~8通道数据有效性采用AO_1型 */
#define AO_8_1		0x40	/**< 8个通道，数据有效性采用AO_1型 */
#define DO_8_1		0x50	/**< 8个通道，数据有效性采用DO_1型 */
#define DO_9_1		0x51	/**< 9个通道，数据有效性采用DO_1型 */
#define LDO_8_1		0x52	/**< 8个通道，数据有效性采用LIO型 */
#define DO_16_1		0x53	/**< 16个通道，数据有效性采用DO_1型 */
#define SD_1		0x21	/**< 12个通道，1～3通道数据有效性采用DO_1,4～9通道数据有效性采用DI_1，10～12通道数据有效性采用REV_1型 */
#define IEDMODEL	0xFD	/**< IED模型 */
#define VPTGROUP	0xFE	/**< VPT虚点组 */
#define RETVPTGROUP	0xFF	/**< VPT虚点组 */
/** @} */

/**
* @name 链路建立帧格式(控制域1)
* @{
*/
#define FRAME_FORMAT_MASK		0x03		/**< 后两位的掩码 */
#define GENERAL_DATA_FRAME		0x00		/**< 一般的数据帧 */
#define T1_TIMEOUT_FRAME		0x01		/**< T1连接，高4位为超时时间 */
#define T2_TIMEOUT_FRAME		0x02		/**< T2连接，高4位为超时时间 */
#define LINK_FRAME				0x03		/**< 测试帧、建立链路、停止链路 */
#define T1_REQUEST_MASK			0x04		/**< T1连接请求位的掩码 */
#define T1_ACK_MASK				0x08		/**< T1连接确认位的掩码 */
#define T2_REQUEST_MASK			0x04		/**< T2连接请求位的掩码 */
#define T2_ACK_MASK				0x08		/**< T2连接确认位的掩码 */
#define CHECK_HEARTBEAT_MASK	0xC0		/**< CHECK HEARTBEAT中间两位的掩码 */
#define LINK_START_REQUEST		0x10		/**< 建立链路请求掩码 */
#define LINK_START_ACK			0x20		/**< 建立链路确认掩码 */
#define LINK_STOP_REQUEST		0x04		/**< 停止链路请求掩码 */
#define LINK_STOP_ACK			0x08		/**< 停止链路确认掩码 */
#define LINK_TEST_REQUEST		0x40		/**< 测试链路请求掩码 */
#define LINK_TEST_ACK			0x80		/**< 测试链路确认掩码 */

#define MULTI_PACKET			0x80		/**< 多帧数据，信息码的标志 */


/** @} */


/**
* @name 信息码表
* @{
*/
#define CALL_FDCS_WHOLE_SYSTEM_DATA											0x10		/**< 召唤FDCS全系统数据 */
#define CALL_FDCS_OBSERVATION_POINT_DATA									0x11		/**< 召唤FDCS测点数据 */
#define CALL_FDCS_STATUS_DATA												0x12		/**< 召唤FDCS状态数据 */
#define CALL_FDCS_SOME_DPU_ALL_DATA											0x13		/**< 召唤FDCS中多个DPU全数据，最多16个 */
#define CALL_FDCS_SOME_DPU_OBSERVATION_POINT_DATA							0x14		/**< 召唤FDCS中多个DPU测点数据，最多16个 */
#define RETURN_OPERATION_FAILED_MESSAGE										0x15		/**< 返回操作失败报文 */
#define CALL_FDCS_SOME_DPU_STATUS_DATA										0x16		/**< 召唤FDCS中多个DPU状态数据，最多16个 */
#define CALL_SOME_IO_BOARD_DATA												0x17		/**< 召唤多个IO板数据,最多16个 */
#define CALL_SOME_DPU_STATUS_DATA											0x18		/**< 召唤多个DPU的状态数据，最多16个 */
#define CALL_SOME_IO_BOARD_STATUS_DATA										0x19		/**< 召唤多个IO板状态数据，最多16个 */
#define CALL_DATA															0x1A		/**< 召唤定值、录波、历史事件 */
#define CALL_DPNET_DATA														0x1B		/**< 召唤DPNET数据 */
#define CALL_DEST_DPU_ALL_DATA												0x1C		/**< 召唤目标节点DPU中所有数据，PHENIX370使用(0x1D同样也使用) */
#define ANSWER_FDCS_WHOLE_SYSTEM_DATA										0x20		/**< 应答FDCS全系统数据 */
#define ANSWER_FDCS_WHOLE_SYSTEM_OBSERVATION_POINT_DATA					    0x21		/**< 应答FDCS全系统测点数据 */
#define ANSWER_FDCS_WHOLE_SYSTEM_STATUS_DATA								0x22		/**< 应答FDCS全系统状态数据 */
#define ANSWER_FDCS_SOME_DPU_DATA											0x23		/**< 应答FDCS系统中多个DPU数据 */
#define ANSWER_FDCS_SOME_DPU_OBSERVATION_POINT_DATA						    0x24		/**< 应答FDCS系统多个DPU测点数据 */
#define ANSWER_FDCS_SOME_DPU_STATUS_DATA									0x25		/**< 应答FDCS系统多个DPU状态数据 */
#define RETURN_OPERATION_SUCCEED_MESSAGE									0x26		/**< 返回操作成功信息 */
#define ANSWER_FDCS_SOME_IO_BOARD_DATA										0x27		/**< 应答FDCS系统多个IO板数据 */
#define RETURN_FDCS_SOME_DPU_STATUS_DATA_NO_IO_STATUS					    0x28		/**< 返回FDCS中多个DPU状态数据(不包括其中的IO状态数据) */
#define RETURN_FDCS_SOME_IO_STATUS_DATA										0x29		/**< 返回FDCS中多个IO状态数据 */
#define ANSWER_DEST_DPU_ALL_DATA											0x2C		/**< 应答目标节点DPU中所有数据，PHENIX370使用(0x2D同样也使用) */
#define DI_POINT_CHANGE_UPLOAD												0x31		/**< DI点变化上送 */
#define ADJUST_OR_CONTROL_RETURN											0x32		/**< 调节或控制返回 */
#define DOWN_CONTROL_AO														0x33		/**< 下行控制AO */
#define DOWN_CONTROL_DO														0x34		/**< 下行控制DO */
#define DOWN_CONTROL_AI														0x35		/**< 下行控制AI */
#define DOWN_CONTROL_DI														0x36		/**< 下行控制DI */
#define DATA_FILE_REQUEST_DOWNLOAD											0x3C		/**< 数据文件请求下载 */
#define ALLOW_DATA_DOWNLOAD													0x3D		/**< 可以进行数据下载 */
#define DATA_DOWNLOAD_TRANSFER_COMPLETED									0x3E		/**< 数据下载传输完毕 */
#define PHENIX_370_DOWN_MESSAGE												0x3F		/**< Phenix370下行报文 */
#define EVENT_UP_MESSAGE												    0x40		/**< 事件上送报文 */
#define SOE_ACTIVELY_UPLOAD													0x4D		/**< SOE主动上送 */
#define HMI_IO_TRANSPARENT_DATA_DOWN										0x52		/**< HMI-IO透明数据下行 */
#define HMI_IO_TRANSPARENT_DATA_UP											0x57		/**< HMI-IO透明数据上行 */
#define SET_DPU_LOGICAL_ADDRESS												0x60		/**< 设置DPU逻辑地址 */
#define SET_DPU_TYPE															0x61		/**< 设置DPU类型 */
#define SET_DPU_IP_ADDRESS														0x62		/**< 设置DPU IP地址 */
#define SET_DPU_ALL_PROPERTIES												0x63		/**< 设置DPU所有属性 */
#define REQUEST_GET_DPU_PROPERTY_INFO										0x64		/**< 请求得到DPU属性信息 */
#define UPLOAD_DPU_PROPERTY_INFO												0x65		/**< 上送DPU属性信息 */
#define REQUEST_IO_PROPERTY_DATA												0x66		/**< 请求IO属性数据 */
#define UPLOAD_IO_PROPERTY_DATA												0x67		/**< 上送IO属性数据 */
#define ONLINE_ADD_IO_TYPE														0x68		/**< 在线增加IO类型 */
#define ONLINE_SET_IO_ADDRESS_TYPE											0x69		/**< 在线设置IO地址、类型 */
#define SEND_CHECK_TIME_COMMAND												0x71		/**< 发出对时命令 */
#define UPLOAD_WAVE_FILE_LIST													0x72		/**< 上送录波文件列表 */
#define UPLOAD_WAVE_FILE														0x73		/**< 上送录波文件 */
#define FIXED_VALUE_DOWNLOAD													0x74		/**< 定值下传(保留) */
#define UPLOAD_FIXED_VALUE_DATA												0x75		/**< 上送定值数据(保留) */
#define FIXED_VALUE_OPERATION													0x76		/**< 定值操作(保留) */
#define UPLOAD_CUR_FIEXED_AREAID												0x77		/**< 定值操作(保留) */
#define SET_SYS_TIME															0x78		/**< 定值操作(保留) */
/** @} */
#define DOWN_CONTROL_VPT				0x3A		/**< 下行控制VPT */

/**
* @name 信息码/描述码指定参数
* @{
*/
#define H11_DATA_CALL					0x03		/**< 子站收到该请求后，将数据发送给HMI主站，一次会话结束 */
#define H11_DATA_SUBSCRIPTION			0x02		/**< 子站将数据上送的同时，为该命令开启定时器，将按照设定的节拍主动上送数据 */
#define H11_CANCAL_DATA_SUBSCRIPTION	0x01		/**< 子站将数据上送的同时，取消数据订购 */
#define H11_TIMER_MASK					0xFC		/**< 低位的后两位是链路建立的标志 */
#define H21_TIME_MARK					0x02		/**< 带时标 */
#define H21_TIME_MARK_LENGTH			4			/**< 时标长度，单位是字节 */
#define H21_QUALIFY_DESCRIPTION			0x01		/**< 带品质描述 */
#define H52_DATA_DOWN_TO_SLAVE			0x0F		/**< 数据流向，写入IO板卡 */
#define H52_PRIORITY_EMERGENCY			0x01		/**< 紧急数据，优先级最高 */
#define H52_CMD_WRITE					0x51		/**< 主命令码，写入数据 */
#define H52_HMI_ADDR1					0x50		/**< 源地址(HMI、IoSmart的地址范围(0x50，0x60) */
/** @} */

/**
* @name 错误代码表
* @{
*/
#define NIC_SUCCESS					0		/**< 成功 */
#define ERROR_UNKNOW_SFDCS			1		/**< 包头不是0x68H */
#define ERROR_NOBUILDLINK			2		/**< 没有建立握手连接，就发出了请求或其他数据请求 */
#define ERROR_SUBSC_INVALID			3		/**< 申请订购定时器失败 */
#define ERROR_ALREADYHASTIMER		4		/**< 无用 */
#define ERROR_TIMEROVERFLOW			5		/**< 申请的定时器小于1 */
#define ERROR_HASNOTIMER			6		/**< 目前无用 */
#define ERROR_ALREADYLINK			7		/**< 已经建立了连接，又重新申请连接 */
#define ERROR_INVALIDPACKET			8		/**< 报头不是0x68H */
#define ERROR_LENTOOSHORT			9		/**< 报头中长度小于等于0 */
#define ERROR_IODEAD				10		/**< 访问的IO已死 */
#define ERROR_IOADDR				11		/**< IO地址错 */
#define ERROR_UNKNOWNDATATYPE		12		/**< 下行数据的数据类型，DPU没有定义 */
#define ERROR_MAKEFRAME				13		/**< 组祯出现了错误 */
#define ERROR_NOVALIDIO				14		/**< 请求数据中IO地址都找不到 */
#define ERROR_INVALID_TARGET		15		/**< DPU地址不符合定义 */
#define ERROR_INVALID_SRC			16		/**< HMI节点地址不符合定义 */
#define ERROR_REGISTER_DEV			17		/**< 注册通信设备地址出错 */
#define ERROR_TARGET_NOTCORRECT		18		/**<  */
#define ERROR_DPU_STATUS_NORUN		19		/**< DPU状态不是运行态 */
#define ERROR_DEVNO_INVALID			20		/**< 在节点中设备序号无效 */
#define ERROR_NODE_OVERLIMIT		21		/**< 请求DPU或IO的数目太多，超过了限制 */
#define ERROR_NO_PROTOCOL			22		/**< 协议没有初始化 */
#define ERROR_NODE_HASNOADDR		23		/**< 请求中出现了无效的DPU或IO地址 */
#define ERROR_NODE_INVALID			24		/**< 返回无效的地址 */
#define SYS_MEM_ALLOC_ERR			25		/**< 系统分配内存出错 */
#define SYS_QERR_QFULL				26		/**<  */
#define ERROR_RIGHT_NOWRITE			27		/**< 没有写权限 */
#define ERROR_OPEN_FILENAME			28		/**< 打开文件错 */
#define ERROR_NOEVENT_ITEM			29		/**< 读事件缓冲区错（空） */
#define ERROR_NOLOG_ITEM			30		/**< 读LOG缓冲区错（空） */
#define ERROR_OVERLIMIT				31		/**<  */
#define ERROR_NO_LBDATA				32		/**< 没有录波文件 */
#define ERROR_SETTING_INVALID		33		/**< 定值区无效（指读取NVRAM失败、定值区无效） */
#define ERROR_SETTING_AREA			34		/**< 定值区号错 */
#define ERROR_SETTING_NUM			35		/**< 定值个数错 */
#define ERROR_SETTING_OVERFLOW		36		/**< 定值越限 */
#define ERROR_SETTING_CRC			37		/**< 定值crc校验错 */
#define ERROR_SETTING_TYPE			38		/**< 定值类型错 */
#define ERROR_SETTING_STATE			39		/**< 操作状态错 */
#define ERROR_SETTING_WFILE			40		/**< 往配置文件写定值区号错 */
#define ERROR_SEND_FAILED			41		/**< 数据包发送失败 */
#define ERROR_RECV_TIMEOUT			42		/**< 数据包接收超时 */
#define ERROR_CONNECT_FAILED		43		/**< 连接失败 */
#define ERROR_CREATE_THREAD			44		/**< 建立后台线程失败 */
#define ERROR_STOP_THREAD			45		/**< 停止线程失败 */
#define ERROR_H11_TIMER_OUTRANGE	47		/**< 数据总召唤=10H，订购时间超过范围 */
#define ERROR_CREATE_LINK			48		/**< 建立链路连接失败 */
#define ERROR_IS_ENABLED			49		/**< 设备没有被启用 */
#define ERROR_DPU_OUT_RANGE			50		/**< DPU序号超出范围 */
#define ERROR_IO_OUT_RANGE			51		/**< IO板卡地址超出范围 */
#define ERROR_CHANNEL_OUT_RANGE		52		/**< IO通道序号超出范围 */
/** @} */

/**
* @name 数据类型码表
* @{
*/
#define BOOL_1_BIT			0x1F		/**< 1Bit的BOOL */
#define FLOAT_4_BYTE		0x24		/**< 4字节浮点 */
#define FLOAT_3_BYTE		0x23		/**< 3字节浮点 */
#define FLOAT_2_BYTE		0x22		/**< 2字节浮点 */
#define UNSIGNED_INT_4_BYTE	0x34		/**< 4字节无符号整数 */
#define UNSIGNED_INT_2_BYTE	0x32		/**< 2字节无符号整数 */
#define UNSIGNED_INT_1_BYTE	0x31		/**< 1字节无符号整数 */
#define INT_4_BYTE			0x44		/**< 4字节整数 */
#define INT_2_BYTE			0x42		/**< 2字节整数 */
#define INT_1_BYTE			0x41		/**< 1字节整数 */
/** @} */


// Zander
#define LINK_ERROR_COUNT 20          // 连接控制器的4个IP地址失败的上限次数，达到该次数则将缓冲中的IO卡的通道数全部置0  2013-7-24
#define SET_TIME_OK      60          // svn循环10次发送一次校时  2013-8-21
#define DPU_EEROR_IO_NUM -1          // 在控制器所有网口连接失败时，当前的IO卡置为-1
#define PING_FAILED_NUM  3           // 连续ping了3次通，则close与该DPU网口的连接
#define PING_INTERVAL    3           // 循环检查几次ping一次。
#define SMALL_IP_SWITCH_DELAY_NUM 6  // 小IP控制器由备切到主后，连续几次为主才把状态信息真正切换为主。



#pragma pack(push, 1)


#pragma pack(pop)




#endif