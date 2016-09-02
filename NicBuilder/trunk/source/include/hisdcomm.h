
// ***************************************************************
// hisd			版本:	2.0
// hisdcomm.h		版本:	1.0
// -------------------------------------------------------------
// 历史库公用头定义，供历史数据文件模块调用 
// -------------------------------------------------------------
// Copyright (C) 2011 - 北京中核东方控制系统有限公司
// ***************************************************************
//
// 创建:cncs-0136 郭欧杰
// mail:guooujie@cncs.bj.cn
// 
//
// ***************************************************************
//
// 改动历史记录
//
// ***************************************************************
//==============================================================================
// 
// 数据类型宏定义
//
// 客户应用程序应该以下定义的数据类型来访问hisd接口函数
// 开发人员最好只添加其中的宏或者数据结构定义，不要删除不是自己建立的数据结构，
// 若对别人的定义有意见先提出，再讨论修改
// 
//==============================================================================
#ifndef _HISDCOMM_H_
#define _HISDCOMM_H_

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "db_config.h"
#include "db_int.h"

//==============================================================================
// 
// 通用数据类型结构定义
// 
//==============================================================================

typedef		void				hisdVoid;		                     
typedef		float				hisdFloat;                         
typedef		double				hisdDouble;                        
typedef		__int8				hisdInt8;                          
typedef		__int16				hisdInt16;                         
typedef		__int32				hisdInt32;                         
typedef		__int64				hisdInt64;                         
typedef		unsigned __int8		hisdUInt8;                         
typedef		unsigned __int16	hisdUInt16;                        
typedef		unsigned __int32	hisdUInt32;                        
typedef		unsigned __int64	hisdUInt64;                        
typedef		hisdUInt8			hisdBool;                          
typedef		hisdUInt8			hisdByte;                          
typedef		char				hisdChar;                          
typedef		hisdChar*			hisdStr;                        
typedef		char*				hisdAStr;                 
typedef		hisdUInt16*			hisdUStr;                 
typedef		hisdUInt32			hisdHandle;                        
typedef		hisdInt32			hisdAPIStatus;		// 函数返回错误码

#define		hisdTRUE				1					// 布尔量真
#define		hisdFALSE				0					// 布尔量假

/// <summary> 
///	标准时间
/// </summary> 
typedef struct
{	
	hisdUInt32		Second;							// 秒
	hisdUInt16		Millisec;						// 豪秒
} hisdTime;	

/// <summary> 
///	带长度的Ansi字符串
/// </summary> 
typedef struct 
{
	hisdUInt32		Length;							// 字符串长度
	hisdAStr			Data;							// 字符串内容
} hisdAnsiString;

/// <summary> 
///	带长度的Unicode字符串
/// </summary> 
typedef struct
{
	hisdUInt32		Length;							// 字符串长度
	hisdUStr			Data;							// 字符串内容
} hisdUnicodeString;

/// <summary> 
///	带长度的二进制串
/// </summary> 
typedef struct
{
	hisdUInt32		Length;							// 二进制数据块的长度
	hisdByte*			Data;							// 二进制数据块的内容
} hisdBlob;

/// <summary> 
///	支持的数据类型种类
/// </summary> 
typedef enum 
{
	hisdDataType_Empty = 0,							// 未定义
		hisdDataType_Bool,								// 开关量
		hisdDataType_Int8,								// 一字节整数
		hisdDataType_UInt8,								// 一字节无符号整数
		hisdDataType_Int16,								// 二字节整数
		hisdDataType_UInt16,								// 二字节无符号整数
		hisdDataType_Int32,								// 四字节整数
		hisdDataType_UInt32,								// 四字节无符号整数
		hisdDataType_Int64,								// 八字节整数
		hisdDataType_UInt64,								// 八字节无符号整数
		hisdDataType_Float,								// 单精度浮点数
		hisdDataType_Double,								// 双精度浮点数
		hisdDataType_Time,								// 时间
		hisdDataType_AnsiString,							// ANSI字符串
		hisdDataType_UnicodeString,						// Unicode字符串
		hisdDataType_Blob,								// 二进制数据块
		hisdDataType_Max									// 最大数据类型?
}hisdDataType;	
/// <summary> 
///	通用数据类型
/// </summary> 
typedef struct 
{
	hisdDataType				DataType;					//数据类型（hisdDataType）
	union
	{
		hisdBool			Bool;						// 开关量
		hisdInt8			Int8;						// 一字节整数
		hisdUInt8			UInt8;						// 一字节无符号整数
		hisdInt16			Int16;						// 二字节整数
		hisdUInt16			UInt16;						// 二字节无符号整数
		hisdInt32			Int32;						// 四字节整数
		hisdUInt32			UInt32;						// 四字节无符号整数
		hisdInt64			Int64;						// 八字节整数
		hisdUInt64			UInt64;						// 八字节无符号整数
		hisdFloat			Float;						// 单精度浮点数
		hisdDouble			Double;						// 双精度浮点数
		hisdTime			Time;						// 时间
		hisdAnsiString		AnsiString;					// Ansi字符串
		hisdUnicodeString	UnicodeString;				// Unicode字符串
		hisdBlob			Blob;						// 二进制块
	};												// 不同数据类型的值定义
} hisdVariant;
	
typedef enum
{
	hisdDAQ_OPC_QUALITY_MASK				= 0x30,	/// Quality BITMASK
		
		// Quality(第4位和第5位)
		hisdDAQ_OPC_BAD					   	= 0x00,	// 坏数据	
		hisdDAQ_OPC_UNCERTAIN					= 0x10,	// 不可靠数据
		hisdDAQ_OPC_NA						= 0x20,	// N/A	
		hisdDAQ_OPC_GOOD						= 0x30,	// 好的数据	
		
		// Substatus for BAD Quality(第0位－第3位)
		hisdDAQ_OPC_CONFIG_ERROR				= 0x01,	// 配置错误
		hisdDAQ_OPC_NOT_CONNECTED				= 0x02,	// 没有连接设备
		hisdDAQ_OPC_DEVICE_FAILURE			= 0x03,	// 设备失败
		hisdDAQ_OPC_SENSOR_FAILURE			= 0x04,	// 传感器失败
		hisdDAQ_OPC_LAST_KNOWN				= 0x05,	// 上一次采集的值(通讯失败)
		hisdDAQ_OPC_COMM_FAILURE				= 0x06,	// 通讯失败(且无上一次采集值可用)
		hisdDAQ_OPC_OUT_OF_SERVICE			= 0x07,	// 设备停机
		hisdDAQ_OPC_WAITING_FOR_INITIAL_DATA	= 0x08,	// 尚未取得设备数据
		
		// Substatus for UNCERTAIN Quality(第0位－第3位)
		hisdDAQ_OPC_LAST_USABLE				= 0x11,	// 上一个可用值
		hisdDAQ_OPC_SENSOR_CAL				= 0x14,	// 传感器值不精确
		hisdDAQ_OPC_EGU_EXCEEDED				= 0x15,	// 超量程
		hisdDAQ_OPC_SUB_NORMAL				= 0x16,	// 值从多个数据源得到，但缺少足够多的好数据
		
		// Substatus for GOOD Quality(第0位－第3位)
		hisdDAQ_OPC_LOCAL_OVERRIDE			= 0x36,	// 值被覆盖(GOOD)
} hisdQuality; 


//==============================================================================
// 
// 历史库运行参数定义
// 
//==============================================================================
//系统启动时默认设置
#define NICHIS_SYSTEMDEFAULT_RUNMODEL hisdRML_INCRE//默认运行模式
#define NICHIS_SYSTEMDEFAULT_SAVEINTERVAL 60//默认保存时间间隔，秒为单位
#define NICHIS_SYSTEMDEFAULT_BAKPERIOD 120//默认备份周期，秒为单位

//查询模式定义
#define NICHIS_QUERYMODE_PRE 0//没有精确匹配时，取前一值
#define NICHIS_QUERYMODE_NEXT 1//没有精确匹配时，取后一值
#define NICHIS_DEFAULT_QUERYMODE NICHIS_QUERYMODE_PRE//系统默认查询模式

//工作空间定义
#define NICHIS_WORKSPACE_LOG "hisLog\\" //日志文件夹
#define NICHIS_WORKSPACE_DATA "hisData\\" //数据文件夹
#define NICHIS_WORKSPACE_SYSTEM "hisSys\\" //系统文件夹
#define NICHIS_WORKSPACE_BAK "hisBak\\" //备份文件夹
#define NICHIS_WORKSPACE_SUBDIRLEN 10

//系统启动模式定义
#define NICHIS_STARTMODE_NEW 0 //创建历史库系统，清空工作空间下的系统文件
#define NICHIS_STARTMODE_CONTINUE 1 //按照工作空间下的系统配置继续运行

#define NICHIS_DATAFILE_PREFIXNAME "nichisfile_"//数据文件前缀名
#define NICHIS_INDEXFILE_PREFIXNAME "nichisfile_"//索引文件前缀名
#define NICHIS_DATAFILE_SUFFIXNAME "nhd"//数据文件后缀名
#define NICHIS_INDEXFILE_SUFFIXNAME "nhi"//索引文件后缀名

#ifdef _DEBUG
//	#define DEFAULT_DATAFILE_PAGESIZE 4096//默认的数据页大小
//	#define DEFAULT_DATAFILE_PAGENUMBER 15000//单个文件中数据页个数
//	#define DEFAULT_INDEXFILE_PAGESIZE 256//默认的索引页大小
//	#define DEFAULT_INDEXFILE_PAGENUMBER 1000//单个文件中索引页个数
//#else
	#define DEFAULT_DATAFILE_PAGESIZE 4096
	#define DEFAULT_DATAFILE_PAGENUMBER 500000
	#define DEFAULT_INDEXFILE_PAGESIZE 256//1:15
	#define DEFAULT_INDEXFILE_PAGENUMBER 50000
#endif // _DEBUG

#define DEFAULT_MPOOL_PAGESIZE DEFAULT_INDEXFILE_PAGESIZE//内存池页面大小

//注意：添加查询模式请在NICHIS_DATASUMMARY_COUNT之前添加，并保持值序列递增升序
#define NICHIS_DATASUMMARY_TOTAL 0//总和
#define NICHIS_DATASUMMARY_MAXIMUM 1//最大值
#define NICHIS_DATASUMMARY_MINIMUM 2//最小值
#define NICHIS_DATASUMMARY_AVERAGE 3//平均值
#define NICHIS_DATASUMMARY_STDEV 4//标准方差
#define NICHIS_DATASUMMARY_RANGE 5//最大值和最小值的差
#define NICHIS_DATASUMMARY_COUNT 6//计数

//系统运行模式
typedef enum
{
	hisdRML_INCRE = 0x01,//无限增长模式
	hisdRML_REPLACE,//循环替换模式
}HisdRunModel;

//系统令牌组：解决全局变量容量（存储文件和测点）自增时，地址变化而引起的游标无法寻址的问题
typedef struct
{
	hisdBool isOpen;       //开关
	hisdInt16 tokennum;    //令牌个数	

	hisdInt32 writefileID;//写游标操作的存储文件的ID
	hisdInt16 cursorcout;//访问writefileID的游标个数，包括读游标
} HisdTokens ;

//系统参数表
typedef struct
{
	hisdAStr nic_workspace;//历史库系统工作空间路径
	hisdUInt64 nic_initime;//历史库运行初始时间
	HisdRunModel nic_runmodel;//历史库运行模式，分为无限增长和循环替换两种模式
	hisdUInt64 nic_maxhisfilenum;//循环替换模型中，历史数据文件的最大个数
	hisdUInt32 nic_syssaveperiod;//历史库运行时，存储文件的存盘间隔，以秒为单位
	hisdUInt64 nic_syslastsavetime;//历史库运行时，最后一次存盘的时间
	hisdUInt32 nic_sysbakperiod;//系统运行参数文件备份周期，以秒为单位。备份内容包括测点管理文件和存储文件管理文件
	hisdUInt64 nic_syslastbaktime;//历史库运行时，最后一次备份参数文件的时间

	hisdInt64 nic_mpoolsize;//内存池大小
	DB_MPOOL_STAT *nic_mpoolstat;//内存池状态参数
	DB_MPOOL_FSTAT **nic_mpoolfilestat;//内存池文件状态参数

	hisdUInt32 nic_datafile_pagenumber;//数据文件页数
	hisdUInt32 nic_indexfile_pagenumber;//索引文件页数

	HisdTokens *nic_psystokens;//系统全局公共变量的令牌组

	hisdBool nic_ishisdataCheck;//是否进行写入的历史数据检查，包括数据类型是否一致，并按照时序写入。
}HisdSysPara;

//测点操作优化参数
typedef struct nicTagRWInfo
{
	hisdInt32 fileID;//数据文件ID
	hisdInt32 indexpagenumber;//索引页的位置
	hisdInt16 indexnumber;//索引项的位置
	hisdInt16 datanumber;//数据项的位置

	hisdBool isdatainsert;//是否有中间数据插入的情况
}TagRWInfo;

//历史数据格式
typedef struct 
{
	hisdTime				Time ;				// 时间戳
	hisdVariant				Value ;				// 值
	hisdUInt8				Quality ;			// 质量戳
} HisdData ;

//==============================================================================
// 
// 系统运行错误归纳
// 
//==============================================================================

#define NICHIS_SUCCESS 0

//通用错误
#define NICHIS_ERROR_UNKNOWN NICHIS_SUCCESS-1
#define NICHIS_ERROR_SYSTEMPARA_NULL  NICHIS_SUCCESS-2
#define NICHIS_ERROR_WORKSPACE_NULL  NICHIS_SUCCESS-3
#define NICHIS_ERROR_FILE_NOEXIST NICHIS_SUCCESS-4
#define NICHIS_ERROR_MALLOC_NULL NICHIS_SUCCESS-5
#define NICHIS_ERROR_FUNCTION_PARA NICHIS_SUCCESS-6
#define NICHIS_ERROR_INVALID_DATATYPE NICHIS_SUCCESS-7
#define NICHIS_ERROR_INVALID_SYSSTARTMODE NICHIS_SUCCESS-8
#define NICHIS_ERROR_INVALID_WORKSPACE NICHIS_SUCCESS-9

//日志和错误处理模块中的错误
#define NICHIS_ERROR_LOGFILE_OPEN  NICHIS_SUCCESS-200
#define NICHIS_ERROR_LOGFILE_CLOSE  NICHIS_SUCCESS-201
#define NICHIS_ERROR_LOGFILE_RENAME  NICHIS_SUCCESS-202

//磁盘文件操作模块
#define NICHIS_ERROR_PAGEHANDLE_CREATEENV  NICHIS_SUCCESS-250
#define NICHIS_ERROR_PAGEHANDLE_ENVNULL  NICHIS_SUCCESS-251
#define NICHIS_ERROR_PAGEHANDLE_MPOOLSET  NICHIS_SUCCESS-252
#define NICHIS_ERROR_PAGEHANDLE_DATADIRNULL  NICHIS_SUCCESS-253
#define NICHIS_ERROR_PAGEHANDLE_OPENENV   NICHIS_SUCCESS-254
#define NICHIS_ERROR_PAGEHANDLE_CLOSEENV   NICHIS_SUCCESS-255
#define NICHIS_ERROR_PAGEHANDLE_SYNENV   NICHIS_SUCCESS-256

#define NICHIS_ERROR_PAGEHANDLE_MPFNULL   NICHIS_SUCCESS-257
#define NICHIS_ERROR_PAGEHANDLE_CREATEMPF   NICHIS_SUCCESS-258
#define NICHIS_ERROR_PAGEHANDLE_OPENMPF   NICHIS_SUCCESS-259
#define NICHIS_ERROR_PAGEHANDLE_CLOSEMPF   NICHIS_SUCCESS-260
#define NICHIS_ERROR_PAGEHANDLE_SYNMPF   NICHIS_SUCCESS-261

#define NICHIS_ERROR_PAGEHANDLE_GETPAGE   NICHIS_SUCCESS-262
#define NICHIS_ERROR_PAGEHANDLE_SETPAGE   NICHIS_SUCCESS-263

#define NICHIS_ERROR_PAGEHANDLE_GETMPOOLSTAT   NICHIS_SUCCESS-264

//存储文件管理模块
#define NICHIS_ERROR_FILEMGR_MGRNULL   NICHIS_SUCCESS-300
#define NICHIS_ERROR_HISFILE_NULL   NICHIS_SUCCESS-302
#define NICHIS_ERROR_FILEDATAID_OVERFLOW   NICHIS_SUCCESS-303
#define NICHIS_ERROR_OPENFILE_FILEID  NICHIS_SUCCESS-304
#define NICHIS_ERROR_HISFILE_NONEXITE  NICHIS_SUCCESS-305
#define NICHIS_ERROR_HISFILE_OPENED  NICHIS_SUCCESS-306
#define NICHIS_ERROR_HISFILE_FILEID  NICHIS_SUCCESS-307
#define NICHIS_ERROR_FILEMGR_NOFILE   NICHIS_SUCCESS-308
#define NICHIS_ERROR_FILEMGR_OPENED   NICHIS_SUCCESS-309
#define NICHIS_ERROR_HISFILE_FULL   NICHIS_SUCCESS-310
#define NICHIS_ERROR_HEADPAGE_LOADED   NICHIS_SUCCESS-311
#define NICHIS_ERROR_HISFILE_NAME   NICHIS_SUCCESS-312
#define NICHIS_ERROR_HISFILE_PATH   NICHIS_SUCCESS-313
#define NICHIS_ERROR_DATAFILE_UNFULL   NICHIS_SUCCESS-314

//测点管理模块
#define NICHIS_ERROR_TAGMGR_MGRNULL   NICHIS_SUCCESS-350
#define NICHIS_ERROR_TAGMGR_TAGTYPE   NICHIS_SUCCESS-351
#define NICHIS_ERROR_TAGMGR_TAGID   NICHIS_SUCCESS-352
#define NICHIS_ERROR_TAGMGR_TAGADDED   NICHIS_SUCCESS-353
#define NICHIS_ERROR_TAGMGR_ADDDELETED   NICHIS_SUCCESS-354
#define NICHIS_ERROR_TAGMGR_NOFILE   NICHIS_SUCCESS-355

//压缩模块
#define NICHIS_ERROR_COMPRESS_UNPOINTK   NICHIS_SUCCESS-400
#define NICHIS_ERROR_COMPRESS_UNPARALLE  NICHIS_SUCCESS-401
#define NICHIS_ERROR_COMPRESS_PCOMPNULL  NICHIS_SUCCESS-402
#define NICHIS_ERROR_COMPRESS_UNDATATYPE  NICHIS_SUCCESS-403
#define NICHIS_ERROR_COMPRESS_UNCOMPTYPE  NICHIS_SUCCESS-404

/***************************
	页数据结构模块
***************************/
//文件头页结构
#define NICHIS_ERROR_HEADPAGE_NULL  NICHIS_SUCCESS-450
//首索引页结构
#define NICHIS_ERROR_FIRSTINDEXPAGE_NULL  NICHIS_SUCCESS-500
#define NICHIS_ERROR_FIRSTINDEX_PUTTED  NICHIS_SUCCESS-501
//索引页结构
#define NICHIS_ERROR_INDEXPAGE_NULL  NICHIS_SUCCESS-550
#define NICHIS_ERROR_INDEXPAGE_INDEXNUMBER  NICHIS_SUCCESS-551
#define NICHIS_ERROR_INDEXPAGE_ITEMDATA  NICHIS_SUCCESS-552
#define NICHIS_FIND_INDEXPAGE_TOFRONT  NICHIS_SUCCESS-553
#define NICHIS_FIND_INDEXPAGE_TOBEHIND  NICHIS_SUCCESS-554
//数据页结构
#define NICHIS_ERROR_DATAPAGE_NULL  NICHIS_SUCCESS-600
#define NICHIS_ERROR_DATATYPE_DIFF  NICHIS_SUCCESS-601
#define NICHIS_FIND_DATAPAGE_TOFRONT  NICHIS_SUCCESS-602
#define NICHIS_FIND_DATAPAGE_TOBEHIND  NICHIS_SUCCESS-603

//游标模块
#define NICHIS_ERROR_CURSOR_INVALID_TAGID  NICHIS_SUCCESS-650
#define NICHIS_ERROR_CURSOR_INVALID_FILEID  NICHIS_SUCCESS-651
#define NICHIS_ERROR_CURSOR_TAGNODATA  NICHIS_SUCCESS-652
#define NICHIS_ERROR_CURSOR_UNINIT  NICHIS_SUCCESS-653 
#define NICHIS_ERROR_CURSOR_CREATEFAIL  NICHIS_SUCCESS-654
#define NICHIS_ERROR_CURSOR_HISFILECLOSE  NICHIS_SUCCESS-655
#define NICHIS_ERROR_CURSOR_NOINDEXPAGE  NICHIS_SUCCESS-656
#define NICHIS_ERROR_CURSOR_INVALID_INDEXNUM  NICHIS_SUCCESS-657
#define NICHIS_ERROR_CURSOR_INVALID_CURSORMODE  NICHIS_SUCCESS-658
#define NICHIS_ERROR_CURSOR_NONEXTHISFILE  NICHIS_SUCCESS-659
#define NICHIS_ERROR_CURSOR_DEAD  NICHIS_SUCCESS-660
#define NICHIS_ERROR_CURSOR_INVALID_TIME  NICHIS_SUCCESS-661
#define NICHIS_FIND_HISFILE_TOFRONT  NICHIS_SUCCESS-662
#define NICHIS_FIND_HISFILE_TOBEHIND  NICHIS_SUCCESS-663
#define NICHIS_ERROR_CURSOR_INVALID_HISFILE  NICHIS_SUCCESS-664
#define NICHIS_ERROR_CURSOR_FINDDATA  NICHIS_SUCCESS-665
#define NICHIS_ERROR_CURSOR_INVALID_QUERYMODE  NICHIS_SUCCESS-666
#define NICHIS_ERROR_CURSOR_ADDHISFILE  NICHIS_SUCCESS-667

//API接口
#define NICHIS_ERROR_API_NODATA   NICHIS_SUCCESS-700
#define NICHIS_ERROR_API_RECEIVEDATA_NOTALL   NICHIS_SUCCESS-701
#define NICHIS_ERROR_API_INVALID_SUMMARYMODE   NICHIS_SUCCESS-702
#define NICHIS_ERROR_API_INVALID_HISDATA_DATATYPE   NICHIS_SUCCESS-703
#define NICHIS_ERROR_API_INVALID_HISDATA_DATATIME   NICHIS_SUCCESS-704
#define NICHIS_ERROR_API_FAILURE_BAKPARAFILE   NICHIS_SUCCESS-705

/*
 * 系统错误归纳结束
 */

//*******************************************************************************
// 系统公共变量和常量声明 弓帅添加于 2011-6-30
//*******************************************************************************

extern HisdSysPara * g_NicHis_syspara;//系统运行时的全局参数变量

//*******************************************************************************
// 系统基础函数 弓帅添加于 2011-6-30
//*******************************************************************************

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * 函数功能：为文件名补充文件路径，需要回收返回的指针
 * 输入参数：文件名称c_filename，文件所在的子目录c_foldername
 * 输出参数：无
 * 返回值　：文件完整路径
 */
hisdAStr __GetFilepath(const hisdAStr c_foldername, const hisdAStr c_filename);

/***********************************
*         时间操作函数             *
***********************************/

/*
* 函数功能：hisdTime时间大比较
* 输入参数：时间参数1：pfirsttime，时间参数2：psecondtime
* 输出参数：无
* 返回值　：比较结果，如果1>2返回true，否则返回false
*/
hisdBool __HisdTimeMoreThan(const hisdTime *pfirsttime, const hisdTime *psecondtime);

/*
* 函数功能：hisdTime时间小比较
* 输入参数：时间参数1：pfirsttime，时间参数2：psecondtime
* 输出参数：无
* 返回值　：比较结果，如果1<2返回true，否则返回false
*/
hisdBool __HisdTimeLessThan(const hisdTime *pfirsttime, const hisdTime *psecondtime);

/*
* 函数功能：hisdTime时间相等比较
* 输入参数：时间参数1：pfirsttime，时间参数2：psecondtime
* 输出参数：无
* 返回值　：比较结果，如果1=2返回true，否则返回false
*/
hisdBool __HisdTimeEqual(const hisdTime *pfirsttime, const hisdTime *psecondtime);

/*
* 函数功能：检查hisdTime时间值是否为0
* 输入参数：时间参数ptime
* 输出参数：无
* 返回值　：检查结果
*/
hisdBool __IsHisdTimeZero(const hisdTime *ptime);

/***********************************
*      历史数据结构操作函数        *
***********************************/

/*
* 函数功能：根据时间段的起至时间，模拟量插值计算其中某一个时刻的值，开关量取前一时刻值
* 输入参数：起始时间pstartdata，结束时间penddata
* 输出参数：计算得出的插值数据pdataitem
* 返回值　：计算结果
*/
hisdInt16 __HisdDataInterpolate(HisdData *pstartdata, HisdData *penddata, HisdData *pdataitem);

/*
 * 函数功能：获取hisdVariant数据结构中当前数据类型的大小
 * 输入参数：目标变量的指针pvariant
 * 输出参数：无
 * 返回值　：对应数据类型的数据大小
 */
hisdInt16 __SizeofVariant(const hisdVariant *pvariant);

/*
 * 函数功能：针对hisdVariant结构的数值拷贝。
 *           若pdst为NULL，则pvariant为目标对象；若psrc为NULL，则pvariant为源对象；若pdst和psrc均不为NULL，报错
 * 输入参数：目标地址pdst，源地址psrc，待操作hisdVariant结构指针pvariant
 * 输出参数：无
 * 返回值　：拷贝结果
 */
hisdInt16 __MemcpyofVariant(hisdVoid *pdst, hisdVoid *psrc, hisdVariant *pvariant);

/*
 * 函数功能：将pvariant的数值转换成double型
 * 输入参数：待操作hisdVariant结构指针pvariant
 * 输出参数：无
 * 返回值　：转换后的double值
 */
hisdDouble __VariantToDouble(const hisdVariant *pvariant);


/***********************************
*       令牌组操作函数             *
***********************************/

/*
* 函数功能：创建一个令牌组，并初始化
* 输入参数：目标令牌组ptokens
* 输出参数：无
* 返回值　：创建结果
*/
hisdInt16 __CreateHisdTokens(HisdTokens **pptokens);

/*
* 函数功能：销毁一个令牌组
* 输入参数：目标令牌组ptokens
* 输出参数：无
* 返回值　：无
*/
hisdVoid __DestroyHisdTokens(HisdTokens **pptokens);

/*
* 函数功能：打开一个令牌组
* 输入参数：目标令牌组ptokens
* 输出参数：无
* 返回值　：无
*/
hisdVoid __OpenHisdTokens(HisdTokens *ptokens);

/*
* 函数功能：当令牌组的令牌数等于0时，关闭一个令牌组，否则一直等待
* 输入参数：目标令牌组ptokens
* 输出参数：无
* 返回值　：无
*/
hisdVoid __CloseHisdTokens(HisdTokens *ptokens);

/*
* 函数功能：向某一令牌组申请拿到一个令牌，当令牌组开关打开且个数大于1时可以拿到令牌，否则一直处于等待状态
* 输入参数：目标令牌组ptokens
* 输出参数：无
* 返回值　：无
*/
hisdVoid __GetHisdToken(HisdTokens *ptokens);

/*
* 函数功能：放回令牌组的令牌
* 输入参数：目标令牌组ptokens
* 输出参数：无
* 返回值　：无
*/
hisdVoid __PutHisdToken(HisdTokens *ptokens);

/*
* 函数功能：设置令牌组的写文件
* 输入参数：令牌组，写文件的ID
* 输出参数：无
* 返回值　：无
*/
hisdVoid __SetHisdTokenFile(HisdTokens *ptokens, const hisdInt32 hisfileID);

/*
* 函数功能：获取令牌组的写文件ID
* 输入参数：令牌组
* 输出参数：无
* 返回值　：写文件的ID
*/
const hisdInt32 __GetHisdTokenFile(HisdTokens *ptokens);

/*
* 函数功能：清除令牌组的写文件
* 输入参数：令牌组
* 输出参数：无
* 返回值　：无
*/
hisdVoid __ClearHisdTokenFile(HisdTokens *ptokens);

/*
* 函数功能：写文件游标访问数加1
* 输入参数：令牌组
* 输出参数：无
* 返回值　：添加后的访问游标个数
*/
hisdInt16 __AddTokenFileCount(HisdTokens *ptokens);

/*
* 函数功能：写文件游标访问数减1
* 输入参数：令牌组
* 输出参数：无
* 返回值　：减少后的访问游标个数
*/
hisdInt16 __MinusTokenFileCount(HisdTokens *ptokens);

#ifdef __cplusplus
};
#endif

#endif //#ifndef _HISDCOMM_H_