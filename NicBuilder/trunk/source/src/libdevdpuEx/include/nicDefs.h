//=========================================================================
//  Nicsys1000   版本:  2.0
//-------------------------------------------------------------------------
//  
//  实时库的通用变量、数据格式定义文件
//
//-------------------------------------------------------------------------
//  版权 (C) 2012 - 北京中核控制系统工程有限公司(CNCS)
//=========================================================================
// 
//  改动历史：
//  	1. 2012/02/27 : 	CNCS-0136 建立文件
//=========================================================================
#ifndef __Nicsys_DEFS_H__
#define __Nicsys_DEFS_H__

#include "nicPrecomp.h"



CDECL_BEGIN
#pragma pack( push ,NicDefs)
#pragma pack( 1 )

	//=====================================================================
	// 
	// 数据类型宏定义
	//
	// 客户应用程序应该以以下定义的数据类型来访问
	// 
	//=====================================================================
	typedef		void				nicVoid ;		                     
	typedef		float				nicFloat ;                         
	typedef		double				nicDouble ;                        
#ifndef _Linux_
	typedef		__int8				nicInt8 ;
	typedef		__int16				nicInt16 ;
	typedef		__int32				nicInt32 ;
	typedef		__int64				nicInt64 ;
	typedef		unsigned __int8		nicUInt8 ;
	typedef		unsigned __int16	nicUInt16 ;
	typedef		unsigned __int32	nicUInt32 ;
	typedef		unsigned __int64	nicUInt64 ;
#else
#include <sys/types.h>
	typedef     int8_t              nicInt8;
	typedef     int16_t             nicInt16;
	typedef     int32_t             nicInt32;
	typedef     int64_t             nicInt64;
	typedef     unsigned char       nicUInt8;
	typedef     unsigned short      nicUInt16;
	typedef     unsigned int        nicUInt32;
	typedef     unsigned long       nicUInt64;
#endif                       
	typedef		nicUInt8			nicBool ;                          
	typedef		nicUInt8			nicByte ;                          
	typedef		char				nicChar ;                          
	typedef		char				nicAChar ;                 
	typedef		nicUInt16			nicUChar ;                 
	typedef		nicAChar*			nicAStr ;                 
	typedef		nicUChar*			nicUStr ;                 
	typedef		nicChar*			nicStr ;                        
	typedef		nicUInt16			nicHandle ;                        
	typedef		nicInt32			nicAPIStatus ;// 函数返回错误码

	/// <summary> 
	///	常量定义
	/// </summary> 
#define		nicFALSE				0 			// 布尔量假
#define		nicTRUE					1 			// 布尔量真

	//=====================================================================
	// 
	// 通用数据类型结构定义
	// 
	//=====================================================================

	/// <summary> 
	///	标准时间
	/// </summary> 
	typedef struct 
	{
		nicUInt32		Second ;				// 秒
		nicUInt16		Millisec ;				// 豪秒
	} nicTime ,*nicTime_Ptr;	

	/// <summary> 
	///	带长度的Ansi字符串
	/// </summary> 
	typedef struct 
	{
		nicUInt32		Length ;				// 字符串长度
		nicAStr			Data ;					// 字符串内容
	} nicAnsiString ;

	/// <summary> 
	///	带长度的Unicode字符串
	/// </summary> 
	typedef struct
	{
		nicUInt32		Length ;				// 字符串长度
		nicUStr			Data ;					// 字符串内容
	} nicUnicodeString ;

	/// <summary> 
	///	带长度的字符串
	/// </summary> 
	typedef struct
	{
		nicUInt32		Length ;				// 字符串长度
		nicStr			Data ;					// 字符串内容
	} nicString ;

	/// <summary> 
	///	带长度的二进制串
	/// </summary> 
	typedef struct
	{
		nicUInt32		Length ;				// 二进制数据块的长度
		nicByte*		Data ;					// 二进制数据块的内容
	} nicBlob ;

	/// <summary> 
	///	支持的数据类型种类
	/// </summary> 
	typedef enum 
	{
		nicDataType_Empty = 0 ,					// 未定义
		nicDataType_Bool ,						// 开关量
		nicDataType_Int8 ,						// 一字节整数
		nicDataType_UInt8 ,						// 一字节无符号整数
		nicDataType_Int16 ,						// 二字节整数
		nicDataType_UInt16 ,						// 二字节无符号整数
		nicDataType_Int32 ,						// 四字节整数
		nicDataType_UInt32 ,						// 四字节无符号整数
		nicDataType_Int64 ,						// 八字节整数
		nicDataType_UInt64 ,						// 八字节无符号整数
		nicDataType_Float ,						// 单精度浮点数
		nicDataType_Double ,						// 双精度浮点数
		nicDataType_Time ,						// 时间
		nicDataType_AnsiString ,					// ANSI字符串
		nicDataType_UnicodeString ,				// Unicode字符串
		nicDataType_String ,						// 字符串
		nicDataType_Blob ,						// 二进制数据块
		nicDataType_Max ,						// 最大数据类型定义
	} nicDataTypeEnum ;	

	/// <summary> 
	///	通用数据类型
	/// </summary> 
	typedef struct 
	{
		nicUInt8			DataType ;			//数据类型（nicDataTypeEnum）
		union
		{
			nicBool			Bool ;				// 开关量
			nicInt8			Int8 ;				// 一字节整数
			nicUInt8		UInt8 ;				// 一字节无符号整数
			nicInt16		Int16 ;				// 二字节整数
			nicUInt16		UInt16 ;			// 二字节无符号整数
			nicInt32		Int32 ;				// 四字节整数
			nicUInt32		UInt32 ;			// 四字节无符号整数
			nicInt64		Int64 ;				// 八字节整数
			nicUInt64		UInt64 ;			// 八字节无符号整数
			nicFloat		Float ;				// 单精度浮点数
			nicDouble		Double ;			// 双精度浮点数
			nicTime			Time ;				// 时间
			nicAnsiString	AnsiString ;		// Ansi字符串
			nicUnicodeString	UnicodeString ;		// Unicode字符串
			nicString		String ;			// Unicode字符串
			nicBlob			Blob ;				// 二进制块
		}; 										// 不同数据类型的值定义
	} nicVariant, *nicVariant_Ptr;	 
	typedef const nicVariant*  nicVariant_CPtr;

	/// <summary> 
	///	字段串列表
	/// 注意:字符串列表不以nicstring作为序列。
	/// </summary> 
	typedef struct
	{
		nicUInt32			Count ;				// 个数
		nicStr*				List ;				// 字符串列表
	} nicStrList ;
	

	/// <summary> 
	///	 返回状态列表
	/// </summary> 
	typedef struct
	{
		nicUInt32			Count ;				// 返回状态个数
		nicAPIStatus*		List ;				// 返回状态列表
	} nicAPIStatusList ;

	//=====================================================================
	// 
	// 常用功能宏定义
	// 
	//=====================================================================
	
#define nicSTR(str)		_TEXT(str)			// 将字符串常量自动转换为Unicode或Ansi



	//=====================================================================
	// 
	// 服务器（nicServer_*）相关函数的数据结构及宏定义
	// 
	//=====================================================================


	/// <summary> 
	///	 服务器的当前状态
	/// </summary> 
	typedef enum
	{
		nicServerStatus_Stop = 0 ,				// 服务器已停止，但并未退出
		nicServerStatus_Busy ,					// 服务器正忙，暂时不能响应客户端请求
		nicServerStatus_Run ,					// 服务器正常运行
		nicServerStatus_Error ,					// 服务器有故障
		nicServerStatus_NetError,				// 网络需要重连
		nicServerStatus_Max ,					// 最大服务器状态
	} nicServerStatusEnum ;



	//=====================================================================
	// 
	// 测点类型（nicTagType_*）相关函数的数据结构及宏定义
	// 
	//=====================================================================

	/// <summary> 
	///	常规属性ID定义
	/// 注意：
	/// 1. 整个系统用一套属性ID，即不同测点类型中，相同ID的属性具有相同的特性；
	/// 2. 属性有只读和可读写两类；
	/// 3. 属性在测点内部不一定需要一个特定的字段来描述
	/// </summary> 
	/// 
	typedef enum 
	{
		nicProp_Min = 0,
		nicProp_TagBase_Min = nicProp_Min,
		//名称属性
		nicProp_Name_Min = nicProp_TagBase_Min,
		nicProp_TagType = nicProp_Name_Min ,	
		nicProp_Name ,		
		nicProp_Description ,
		nicProp_Name_Max ,

		//内置结构属性
		nicProp_Inter_Min =nicProp_Name_Max,	
		nicProp_Id  =nicProp_Inter_Min,
		nicProp_ParentId ,	
		nicProp_FirstChildId ,
		nicProp_PrevId,		
		nicProp_NextId ,		
		nicProp_LastModifyTime ,
		nicProp_CreateTime ,
		nicProp_ExtendID,
		nicProp_RelateID,
		nicProp_Inter_Max,

		//工程属性
		nicProp_Proj_Min =nicProp_Inter_Max,
		nicProp_DataType = nicProp_Proj_Min ,	
		nicProp_InitVal,
		nicProp_OperateClass,
		nicProp_EnableWrite,
		nicProp_ProjectID,
		nicProp_ProjectSysName,
		nicProp_ProjectSysSign,
		nicProp_ProjectDevName,
		nicProp_ProjectDevSign,
		nicProp_CPAddr,
		nicProp_PhysicsMoAddr,
		nicProp_PhysicsMoType,
		nicProp_Proj_Max,

		//历史属性
		nicProp_His_Min = nicProp_Proj_Max,
		nicProp_His_IsSave =	nicProp_His_Min ,	  //是否保存开关
		nicProp_His_MaxSaveTime ,		              //最大保存时间
		nicProp_His_CompressRate ,		              //压缩率
		nicProp_His_CompressMode,		              //压缩模式
		nicProp_His_Max,		

		//报警属性
		nicProp_Alarm_Min = nicProp_His_Max,
		nicProp_EnableAlarm = nicProp_Alarm_Min,
		nicProp_AlarmClass,
		nicProp_AlarmClassify,
		nicProp_EnableQualityAlarm,
		nicProp_Alarm_Max,

		//驱动属性
		nicProp_IOSS_Min = nicProp_Alarm_Max,
		nicProp_IODriveDev = nicProp_IOSS_Min,
		nicProp_IODriveAddr,
		nicProp_RefreshPeriod,
		nicProp_IOSS_Max,

		nicProp_TagBase_Max =nicProp_IOSS_Max,
		


		////实时属性
		//nicPropFlag_Pv_Min = nicProp_TagBase_Max,
		//nicProp_PV = nicPropFlag_Pv_Min,
		//nicProp_LastPV,
		//nicProp_PV_Time,
		//nicProp_PV_Quality,
		//nicProp_LastPV_Time,
		//nicProp_LastPV_Quality,
		//nicPropFlag_Pv_Max,
		// 扩展属性最小值
		nicPropFlag_Extend_Min = nicProp_TagBase_Max,
		nicPropFlag_Extend_Digital_Min = nicPropFlag_Extend_Min,
		nicProp_OffMsg=nicPropFlag_Extend_Digital_Min,
		nicProp_OnMsg,
		nicProp_EnableOffMsg,
		nicProp_EnableOnMsg,
		nicProp_AlarmDelayTime,
		nicPropFlag_Extend_Digital_Max,

		//模拟点
		nicPropFlag_Extend_Analogy_Min = nicPropFlag_Extend_Digital_Max,
		nicProp_MinValue =nicPropFlag_Extend_Analogy_Min,//最小值
		nicProp_MaxValue,					//最大值
		nicProp_Unit,							//单位
		nicProp_EnableLLLL,			///是否支持L4报警
		nicProp_EnableLLL,			///是否支持L3报警
		nicProp_EnableLL,			///是否支持L2报警
		nicProp_EnableLO,		   ///是否支持低报警
		nicProp_EnableHI,		   ///是否支持高报警
		nicProp_EnableHH,		   ///是否支持高高报警
		nicProp_EnableHHH,			///是否支持高高高报警
		nicProp_EnableHHHH,			///是否支持高高高报警
		nicProp_EnableRate,		   ///变化率报警限值
		nicProp_LLLL,		///低低低
		nicProp_LLL,		///低低低
		nicProp_LL,		   ///低低限
		nicProp_LO,		   ///低限
		nicProp_HI,		   ///高限
		nicProp_HH,		   ///H2限
		nicProp_HHH,		///H3
		nicProp_HHHH,		///H4
		nicProp_Rate,		   ///变化率报警限值
		nicProp_DeadBand,     ///报警死区
		nicPropFlag_Extend_Analogy_Max,
		nicPropFlag_Extend_Max =nicPropFlag_Extend_Analogy_Max,

		nicPropFlag_Config_Max =nicPropFlag_Extend_Max, //静态属性最大编号


		nicPropFlag_SnapShot_Min = nicPropFlag_Config_Max,
		nicProp_CurValue=nicPropFlag_SnapShot_Min,
		nicProp_SQRT,
		nicProp_TOT10,
		nicProp_AVG10,
		nicProp_MIN10,
		nicProp_MAX10,
		nicProp_TOT30,                       
		nicProp_AVG30,
		nicProp_MIN30,
		nicProp_MAX30,
		nicProp_TOT60,
		nicProp_AVG60,
		nicProp_MIN60,            
		nicProp_MAX60,

		nicPropFlag_SnapShot_Max,

		nicProp_Max = nicPropFlag_SnapShot_Max,

	}nicTagPropEnum;
	/// <summary> 
	///	点类型常量定义
	/// </summary> 

#define	nicTagType_All	0xFFFF 					// 所有点类型


	/// <summary> 
	///	属性ID列表
	/// </summary> 
	typedef struct
	{
		nicUInt16			PropCount ;			// 属性个数
		nicUInt16*			PropIdList ;		// 属性ID列表
	} nicPropIdList ;

	/// <summary> 
	///	单个属性的值
	/// </summary> 
	typedef struct
	{
		nicUInt16			PropId ;			// 属性ID
		nicVariant			Value ;				// 属性值
	} nicPropValue ;

	/// <summary> 
	///	 属性值列表
	/// </summary> 
	typedef struct
	{
		nicUInt16			PropCount ;			// 属性个数
		nicPropValue*		PropValueList ;		// 属性值列表
	} nicPropValueList ;

	//=====================================================================
	// 
	// 测点（nicTag_*）相关函数的数据结构及宏定义
	// 
	//=====================================================================

	/// <summary> 
	///	测点ID常量定义
	/// </summary> 
#define	nicTagId_Unused	0xFFFFFFFF				// 无效的测点ID
#define	nicTagId_Root	0x00000000				// 根测点ID



	/// <summary> 
	///	标签属性状态标志
	/// </summary> 
	typedef struct
	{
		nicUInt8 Deleted:1;
		nicUInt8 IsNode:1;
		nicUInt8 Changed:1;
		nicUInt8 Unused:5;
	} nicTagStatusFlag;


	/// <summary> 
	/// 标签点类型，可以扩展
	/// </summary> 
	typedef enum 
	{
		nicTagType_Null = 0,		//空类型
		nicTagType_Node,		//组节点
		nicTagType_Ana,		//模拟量点
		nicTagType_Dig,		//数字量点
		nicTagType_Compound,	//组合点
		nicTagType_Device,		//设备点
		nicTagType_Custom,		//自定义
	}nicTagTypeEnum;				



	/// <summary> 
	///	基本属性的链表
	/// </summary> 
	typedef struct 
	{
		nicUInt16		 PropCount;
		nicVariant**	 Value;
		nicBool			 HaveBaseProp;		
		nicBool			 HaveHisProp;
		nicBool			 HaveProjProp;
		nicBool			 HaveAlarmProp;
		nicBool			 HaveExtEndID_Prop;		
	}nicTagPropList;

	/// <summary> 
	///	测点ID列表
	/// </summary> 
	typedef struct
	{
		nicUInt32			TagCount ;			// 测点个数
		nicUInt32*			TagIdList ;			// 测点ID列表
	} nicTagIdList ;

	/// <summary> 
	///	ReconID列表
	/// </summary> 
	typedef struct
	{
		nicUInt32			RecnoCount ;		// 记录号的个数
		nicUInt32*			RecnoList ;			// 记录号列表
	} nicRecnoList ;


	/// <summary> 
	///	nicQueryArea.QueryLevel中的特殊值
	/// </summary> 
#define nicQueryLevel_ALL	0xFFFF				// 查询结点下的所有内容，用于nicQueryArea

	/// <summary> 
	///	 查询范围
	/// </summary> 

	typedef struct
	{
		nicUInt32			RootTagId ;			// 查询的起始结点（包括它本身）
		nicUInt16			QueryLevel ;		// 查询的树深度，0查本身，nicQueryLevel_ALL查所有，1、2、3...查相关深度
		nicBool				QuerySelf ;			// 当查询的深度超过0时，是否查起始结点本身
	} nicQueryArea ;

	/// <summary> 
	///	 值列表
	/// </summary> 
	typedef struct 
	{
		nicUInt32			ValueCount;			// 值个数
		nicVariant*			ValueList;			// 值列表
	} nicValueList;



	/// <summary> 
	///	测点改变的类型
	/// </summary> 
	/// 
	typedef enum
	{
		nicTagChangeType_Create = 0 ,			// 测点建立
		nicTagChangeType_Change ,				// 测点属性改动
		nicTagChangeType_Delete ,				// 测点被删除
		nicTagChangeType_Init ,					// 系统初始化，所有测点被删除
		nicTagChangeType_Max ,					// 
	} nicTagChangeTypeEnum ;

	//=====================================================================
	// 
	// 实时数据处理（nicReal_*）相关函数的数据结构及宏定义
	// 
	//=====================================================================

	typedef enum
	{
		nicDAQ_OPC_QUALITY_MASK				= 0x30,	/// Quality BITMASK

		// Quality(第4位和第5位)
		nicDAQ_OPC_BAD					   	= 0x00,	// 坏数据	
		nicDAQ_OPC_UNCERTAIN					= 0x10,	// 不可靠数据
		nicDAQ_OPC_NA						= 0x20,	// N/A	
		nicDAQ_OPC_GOOD						= 0x30,	// 好的数据	

		// Substatus for BAD Quality(第0位－第3位)
		nicDAQ_OPC_CONFIG_ERROR				= 0x01,	// 配置错误
		nicDAQ_OPC_NOT_CONNECTED				= 0x02,	// 没有连接设备
		nicDAQ_OPC_DEVICE_FAILURE			= 0x03,	// 设备失败
		nicDAQ_OPC_SENSOR_FAILURE			= 0x04,	// 传感器失败
		nicDAQ_OPC_LAST_KNOWN				= 0x05,	// 上一次采集的值(通讯失败)
		nicDAQ_OPC_COMM_FAILURE				= 0x06,	// 通讯失败(且无上一次采集值可用)
		nicDAQ_OPC_OUT_OF_SERVICE			= 0x07,	// 设备停机
		nicDAQ_OPC_WAITING_FOR_INITIAL_DATA	= 0x08,	// 尚未取得设备数据

		// Substatus for UNCERTAIN Quality(第0位－第3位)
		nicDAQ_OPC_LAST_USABLE				= 0x11,	// 上一个可用值
		nicDAQ_OPC_SENSOR_CAL				= 0x14,	// 传感器值不精确
		nicDAQ_OPC_EGU_EXCEEDED				= 0x15,	// 超量程
		nicDAQ_OPC_SUB_NORMAL				= 0x16,	// 值从多个数据源得到，但缺少足够多的好数据

		// Substatus for GOOD Quality(第0位－第3位)
		nicDAQ_OPC_LOCAL_OVERRIDE			= 0x36,	// 值被覆盖(GOOD)
	} nicQualityEnum; 

#define IsDataBad( dataQuality )		(((dataQuality) & nicDAQ_OPC_QUALITY_MASK) == nicDAQ_OPC_BAD )
#define IsDataUncertain( dataQuality )	(((dataQuality) & nicDAQ_OPC_QUALITY_MASK) == nicDAQ_OPC_UNCERTAIN )
#define IsDataGood( dataQuality )		(((dataQuality) & nicDAQ_OPC_QUALITY_MASK) == nicDAQ_OPC_GOOD )

	/// <summary> 
	///  数据记录定义
	/// </summary> 
	typedef struct 
	{
		nicTime				Time ;				// 时间戳
		nicVariant			Value ;				// 值
		nicUInt8				Quality ;			// 质量戳
	} nicData ;							


	/// <summary> 
	///	 数据记录列表
	/// </summary> 
	typedef struct
	{
		nicUInt32			DataCount ;			// 数据记录个数
		nicData*				DataList ;			// 数据记录列表
	} nicDataList ;

	/// <summary> 
	///	 测点实时值
	/// </summary> 

	typedef struct
	{
		nicUInt32			TagId ;				// 测点ID
		nicData				RealData ;			// 测点实时数据
	} nicRealData ;


	/// <summary> 
	///	 多个测点值列表
	/// </summary> 
	typedef struct
	{
		nicUInt32			TagCount ;			// 测点个数
		nicRealData*		TagListRealData ;	// 测点实时数据列表
	} nicRealDataList ;

	//=====================================================================
	// 
	// 实时历史数据处理（nicHis_*）相关函数的数据结构及宏定义
	// 
	//=====================================================================

	/// <summary> 
	///  测点历史数据定义
	/// </summary> 
	typedef struct 
	{
		nicUInt32			TagId ; 			// 测点ID
		nicUInt32			DataCount ;			// 数据个数
		nicData*				DataList ; 			// 数据列表
	} nicTagHisData ;
	//系统启动模式定义
	#define NICHIS_STARTMODE_NEW 0 //创建历史库系统，清空工作空间下的系统文件
	#define NICHIS_STARTMODE_CONTINUE 1 //按照工作空间下的系统配置继续运行
	//系统运行模式
	typedef enum
	{
		nicRML_INCRE = 0x01,//无限增长模式
		nicRML_REPLACE,//循环替换模式
	}nicHisdRunModel;
	/// <summary> 
	///  多个测点历史数据定义
	/// </summary> 
	typedef struct 
	{
		nicUInt32			TagCount ;			// 测点个数
		nicTagHisData*		TagListHisData ;	// 测点历史数据列表，每条记录保存一个测点的历史数据列表
	} nicTagListHisData ;						

	/// <summary> 
	/// 查询历史数据时，取历史数据的方式
	/// </summary> 
	/// 
	typedef enum 
	{
		nicQueryMode_Undefined = 0 ,				// 未定义
		nicQueryMode_Interpolated ,				// 等时间间隔，每个时间点的数据取二分插值后计算的值
		nicQueryMode_RawByTime ,					// 取压缩后的数据，开始和结束点不进行二分插值计算
		nicQueryMode_RawByNumber ,				// 取压缩后的数据，不考虑时间段，取够足够数量的数据
		nicQueryMode_Stepped ,					// 按布近方式取数据，主要用于不可进行线性插值的数据类型的插值计算
		nicQueryMode_Statistic ,					// 取按时间分段的统计量
		nicQueryMode_Max ,						// 最大取历史数据方式
	} nicQueryModeEnum ;

	/// <summary> 
	/// 数据查询时的过滤方法
	/// </summary> 
	/// 
	typedef enum 
	{
		nicFilterMode_Undefined = 0 ,			// 未定义
		nicFilterMode_ExactTime ,				// 取满足过滤条件对应的时间段
		nicFilterMode_BeforeTime ,				// 取满足过滤条件之前的时间段
		nicFilterMode_AfterTime ,				// 取满足过滤条件之后的时间段
		nicFilterMode_BeforeAndAfterTime ,		// 取满足过滤条件之前和之后的时间段
		nicFilterMode_Max ,						// 最大过滤方法
	} nicFilterModeEnum ;	

	/// <summary> 
	/// 查询历史数据时，取点的方向（按时间大小）
	/// </summary> 
	/// 
	typedef enum 
	{
		nicDirection_Forward = 0 ,				// 向后取点
		nicDirection_Backward ,					// 向前取点
		nicDirection_Max ,						// 
	} nicDirectionEnum ;								

	/// <summary> 
	/// 统计方法
	/// </summary> 
	/// 
	typedef enum 
	{
		nicStatisticMode_Undefined = 0 ,			// 未知的计算方式
		nicStatisticMode_Count ,					// 计数(每个采样区间的原始值的个数，数据质量不论)
		nicStatisticMode_Average ,				// 原始平均值
		nicStatisticMode_Total ,					// 原始值求和
		nicStatisticMode_Stdev ,					// 标准方差
		nicStatisticMode_Time_Total ,			// 时间合成求和
		nicStatisticMode_Time_Average ,			// 时间合成平均值
		nicStatisticMode_Minimum ,				// 最小值(每个采样区间内的最小值，数据质量是好的原始值)
		nicStatisticMode_Maximum ,				// 最大值(每个采样区间内的最大值，数据质量是好的原始值)
		nicStatisticMode_Minimum_Time ,			// 最小值对应的时间
		nicStatisticMode_Maximum_Time ,			// 最大值对应的时间
		nicStatisticMode_Duration_Good ,			// 好数据的时间总长(ms)
		nicStatisticMode_Duration_Bad ,			// 坏数据的时间总长(ms)
		nicStatisticMode_Start ,					// 区间内的第一个原始数据（质量戳可能是好的，也可能是坏的）
		nicStatisticMode_End ,					// 区间内的最后一个原始数据（质量戳可能是好的，也可能是坏的）
		nicStatisticMode_Delta ,					// 区间内的第一个和最后一个好的原始数据之差
		nicStatisticMode_Range ,					// 区间内最大/最小值的绝对差值（只计算好数据） 
		nicStatisticMode_Percent_Good ,			// 好数据总长/区间长度
		nicStatisticMode_Percent_Bad ,			// 坏数据总长/区间长度
		nicStatisticMode_Max ,					// 
	} nicStatisticModeEnum ;	

	/// <summary> 
	/// 数据查询时的时间间隔种类
	/// </summary> 
	/// 
	typedef enum 
	{
		nicIntervalMode_Undefined = 0 ,			// 未定义
		nicIntervalMode_MilliSecond ,			// 豪秒
		nicIntervalMode_Second ,					// 秒
		nicIntervalMode_Minute ,					// 分
		nicIntervalMode_Hour ,					// 时
		nicIntervalMode_Day ,					// 日
		nicIntervalMode_Month ,					// 月
		nicIntervalMode_Year ,					// 年
		nicIntervalMode_Week ,					// 周
		nicIntervalMode_Max ,					// 最大间隔种类
	} nicIntervalModeEnum ;

	/// <summary> 
	///  历史数据查询过滤器
	/// </summary> 
	typedef struct 
	{
		nicUInt32			TagCount ;			// 测点个数
		nicUInt32*			TagIdList;			// 测点ID列表
		nicQueryModeEnum		QueryMode ;			// 查询类型
		nicTime				StartTime ;			// 开始时间
		nicTime				EndTime ;			// 结束时间
		nicDirectionEnum		Direction ;			// 取样方向
		nicUInt32			SamplingCount ;		// 取样点总数
		nicIntervalModeEnum	IntervalMode ;		// 取样间隔类型
		nicUInt32			IntervalTime ;		// 取样间隔时间
		nicStatisticModeEnum	StatisticMode ;		// 统计方式 
		nicFilterModeEnum	FilterMode ;		// 过滤方式 	
		nicStr				FilterExpression ;	// 过滤表达式
	} nicHisDataFilter ;	

	/// <summary> 
	///  数据删除条件定义
	/// </summary> 

	typedef struct 
	{
		nicUInt32			TagCount ;			// 测点个数
		nicUInt32*			TagIdList ;			// 测点ID列表
		nicTime				StartTime ;			// 开始时间
		nicTime				EndTime ;			// 结束时间
	} nicDeleteDataFilter ;

	//=====================================================================
	// 
	// 历史数据文件处理（nicArchive_*）相关函数的数据结构及宏定义
	// 
	//=====================================================================

	/// <summary> 
	///	 历史数据文件信息
	/// </summary> 

	typedef struct 
	{
		nicUInt16			FileId ;			// 文件标识号
		nicTime				StartTime ;			// 文件记录数据的开始时间
		nicTime				EndTime ;			// 文件记录数据的结束时间
		nicStr				FileName ;			// 文件名
	} nicDataFileInfo ;

	/// <summary> 
	///	 历史数据文件列表
	/// </summary> 

	typedef struct
	{
		nicUInt16			DataFileCount ;		// 历史数据文件个数
		nicDataFileInfo*		DataFileList ;		// 历史数据文件列表
	} nicDataFileList ;




	/// <summary> 
	///	进程运行状态
	/// </summary> 
	typedef enum
	{
		nicProcess_run = 0 ,			// 进程运行
		nicProcess_stop ,				// 进程停止
		nicProcess_noBuffer ,			// 进程主缓冲不足
		nicProcess_Max,
	} nicProcessStatus ;

	/// <summary> 
	///	网络连接状态
	/// </summary> 
	typedef enum
	{
		Connections_run = 0 ,			// 网络连接运行正常
		Connections_stop ,              // 网络连接线程停止
		Connections_noBuffer ,			// 网络连接缓冲不足
		Connections_waitReConnect,		// 网络连接等待重连
		Connections_notexist ,          // 网络连接已不存在
		Connections_clientInterrupt,	// 客户端连接已断开
	} nicConnectionStatus;

	/// <summary> 
	///	 交换机消息处理的优先级
	/// </summary> 

	typedef enum
	{
		MSGQ_Priority_High = 1,
		MSGQ_Priority_Normal = 2,
		MSGQ_Priority_Low = 4,
	}MsgQueuePriority;


	/// <summary> 
	///	网络服务器端过滤模式
	/// </summary> 
	/// 
	typedef enum
	{
		IP_DISABLE_FILTER = 1,
		IP_IS_FILTER = 2,
		IP_IS_ACCESS = 4,
	}IPFilterMode;


	///IPC进程通信的标识

	typedef enum
	{
		IPC_ID_Max = 0xFFFF,
		IPC_ID_Switch = IPC_ID_Max -1,
		//实时库上的进程ID号
		IPC_ID_Alarm =IPC_ID_Max - 2,
		IPC_ID_IOSS = IPC_ID_Max - 3,
		IPC_ID_Tran = IPC_ID_Max - 4,
		IPC_ID_Db  = IPC_ID_Max - 5,
		IPC_ID_Config  = IPC_ID_Max - 6,
		IPC_ID_User1  = IPC_ID_Max - 7,
		IPC_ID_User2  = IPC_ID_Max - 8,
		IPC_ID_User4  = IPC_ID_Max - 9,
		//历史库的
		IPC_ID_Hisd  = IPC_ID_Max - 10,	
		IPC_ID_AalrmHisd  = IPC_ID_Max - 11,	
		IPC_ID_Event  = IPC_ID_Max - 12,	
		IPC_ID_Log  = IPC_ID_Max - 13,	
		IPC_ID_SOE = IPC_ID_Max - 14,

		IPC_ID_Min = IPC_ID_Max - 32,
	}Nicsys_IPC_ID;

	///网络交换机处理模块状态
	typedef enum
	{
		SwitchCore_Stop = 0,
		SwitchCore_Puase,
		SwitchCore_Idle,
		SwitchCore_Normal,
		SwitchCore_Bussy,
		SwitchCore_Run,
	}nicSwitchTaskStatus;

	///网络交换机计数器结构体

	typedef struct
	{
		nicUInt32  Switch_P2P_up;			//单播上行
		nicUInt32  Switch_P2P_down;			//单播下行
		nicUInt32  Switch_DB_CastAll;		//内存数据库多播全部
		nicUInt32  Switch_DB_CastLocal;		//内存数据库多播本机其他进程
		nicUInt32  Switch_CastNet;			//DB和报警模块多播至所有网络客户端
		nicUInt32  Switch_DB_CastAlarm;		//DB多播至报警模块
		nicUInt32  Switch_DB_CastIOSS;		//DB多播至IOSS模块
		nicUInt32  Switch_DB_CastTran;		//DB多播至数据发送模块
		nicUInt32  Switch_DB_CastConfig;	//DB多播至数据库管理模块
		nicUInt32  Switch_Alarm_CastAll;	//报警模块多播至所有模块
		nicUInt32  Switch_IOSS_CastLocal;	//IOSS模块多播至本服务器所有子模块
		nicUInt32  Switch_Client_Write;		//客户端写操作
		nicUInt32  Switch_Other;			//其他类型的数据
	}nicSwitchStatistic;

//////////////////////////////////////////////////////////////////////////
	//测点的报警状态级别
	//分为八级
	typedef struct
	{
		nicBool		EnableHHHH:1;				//是否支持H4报警
		nicBool		EnableHHH:1;				//是否支持HHH报警
		nicBool		EnableHH:1;				//是否支持HH报警
		nicBool		EnableHI:1;				//是否支持H报警
		nicBool		EnableLO:1;				//是否支持L报警
		nicBool		EnableLL:1;				//是否支持LL报警
		nicBool		EnableLLL:1;				//是否支持LLL报警
		nicBool		EnableLLLL:1;				//是否支持L4报警
		nicBool		EnableRate:1;				//是否支持RATE报警
		nicBool		UnUsed:7;						//占位
	}nicAnaAlarmMask;

////全局函数定义
//内部遍历使用的

	typedef  nicAPIStatus  (*TagCBFunc) (nicVoid*,nicUInt32 unTagId) ;


	typedef  nicAPIStatus  (*RealDataNotifyCBFunc) (nicVoid *pList) ;
	typedef  nicAPIStatus  (*AddTagNotifyCBFunc) (nicUInt32 unTagId,nicPropValueList *valList) ;
	typedef  nicAPIStatus  (*DelTagNotifyCBFunc) (nicUInt32 unTagId) ;
	typedef  nicAPIStatus  (*ModifyTagNotifyCBFunc) (nicUInt32 unTagId, nicPropValueList *propValueList) ;


	typedef struct
	{
		RealDataNotifyCBFunc func_notify;
		AddTagNotifyCBFunc   func_addtag;
		DelTagNotifyCBFunc   func_deltag;
		ModifyTagNotifyCBFunc func_modtag;
	}nicOnlineConfigProc;
//////////////////////////////////////////////////////////////////////////

CDECL_END
#pragma pack( pop  , NicDefs )
#endif ///  __Nicsys_DEFS_H__
