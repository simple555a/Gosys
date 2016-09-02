/*
* 文件名称：hisAPI.h
* 摘　　要：历史库对外提供的API接口
* 当前版本：1.0
* 作　　者：弓帅
* 完成日期：2011年7月28日
*
* 备注：
*
*       
*
* 修改记录：
* 日期　　　　　版本　　修改人　　修改内容
* 
*
*/
#ifndef _HISAPI_H_
#define _HISAPI_H_

#include "nicDefs.h"


#ifdef _WIN32
#if defined	LIB_HISD_EXPORTS
#define HISD_API	  __declspec(dllexport)
#else
#define HISD_API	  __declspec(dllimport)
#endif
#else
#define HISD_API
#endif

CDECL_BEGIN

/*
* 函数功能：释放历史库内核申请的历史数据内存，并设置指针为NULL
* 输入参数：需要释放的指针的地址
* 输出参数：无
* 返回值　：无
*/

NICSYSDLL HISD_API nicVoid  NICSYS_API Nichis_FreeHisdData(nicData **memblock);

/*
* 函数功能：创建历史库
* 输入参数：系统工作空间c_workspace
* 输出参数：无
* 返回值　：创建结果
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SysCreate(const nicAStr c_workspace, const nicInt8 c_sysmode);

/*
* 函数功能：初始化历史库
* 输入参数：无
* 输出参数：无
* 返回值　：初始化结果
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SysInit();

/*
 * 函数功能：启动历史库
 * 输入参数：无
 * 输出参数：无
 * 返回值　：启动结果
 */
 NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SysStart();

/*
* 函数功能：退出历史库
* 输入参数：无
* 输出参数：无
* 返回值　：退出结果
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SysExit();

/*
* 函数功能：磁盘同步系统文件，包括当前内存池、存储文件管理和测点管理
* 输入参数：无
* 输出参数：无
* 返回值　：同步结果
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SynHisSystem();

/*
* 函数功能：备份系统参数文件
* 输入参数：无
* 输出参数：无
* 返回值　：备份结果
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_BakSysParaFiles();

/*
* 函数功能：设定系统运行模式，如果是循环替换模式，需要输入历史数据文件最大数
* 输入参数：枚举型运行模式参数c_runmodel，历史数据文件最大数c_maxhisfilenum
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetRunModel(const nicHisdRunModel c_runmodel, const nicUInt64 c_maxhisfilenum);

/*
* 函数功能：设定存储文件存盘时间间隔，以秒为单位。如果不需要自动保存，设置<0的值
* 输入参数：时间间隔
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetSavePeriod(const nicUInt32 c_saveinterval);

/*
* 函数功能：设定系统运行参数文件备份周期，以秒为单位。如果不需要备份，设置<0的值
* 输入参数：备份周期
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetBakPeriod(const nicUInt32 c_bakperiod);

/*
* 函数功能：设定系统内存池大小，以字节为单位
* 输入参数：内存池大小
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetMPoolSize(const nicInt64 c_mpoolsize);

/*
* 函数功能：设定测点的压缩间隔
* 输入参数：测点ID：tagID，压缩间隔compinterval
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetCompIntervalOfTag(const nicInt32 tagID, nicUInt32 compinterval);

/*
* 函数功能：设定测点压缩偏移量，目前只旋转门算法有效
* 输入参数：测点ID，压缩偏移量，压缩偏移类型
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetCompOffsetOfTag(const nicInt32 tagID, nicDouble compoffset, nicInt8 compoffset_type);

/*
* 函数功能：设定系统数据文件参数，包括页面数量。
* 输入参数：数据文件页面数量datafile_pagenumber。设定为负值时，会自动设定为默认设置
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetDatafilePara(nicUInt32 datafile_pagenumber);

/*
* 函数功能：设定系统索引文件参数，包括页面数量。建议设定为datafile_pagenumber的1/15以上
* 输入参数：索引文件页面数量indexfile_pagenumber。设定为负值时，会自动设定为默认设置
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetIndexfilePara(nicUInt32 indexfile_pagenumber);

/*
* 函数功能：设定系统是否进行写入历史数据检查，检查内容包括写入的历史数据是否类型一致，并且按照时序写入的
* 输入参数：是否进行检查，默认为不检查
* 输出参数：无
* 返回值　：无
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetHisdataCheck(nicBool isCheck);

/*
* 函数功能：写tagID测点的历史数据
* 输入参数：测点ID：tagID，待写入的历史数据pdataitems，待写入历史数据的个数datacount
* 输出参数：无
* 返回值　：写入结果
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_WriteData(const nicInt32 tagID, nicData *pdataitems, nicInt32 datacount);

/*
* 函数功能：读取tagID测点压缩后的历史数据
* 输入参数：测点ID：tagID，读数关键时间点ptimes
* 输出参数：读出的历史数据out_pdataitems，读出历史数据的个数out_datacount
* 返回值　：读取结果
* 备    注：ptimes数组要最少初始化5个大小，ptimes[0]表示读取时间段的起始时刻，ptimes[1]表示时间段的终止时刻。
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_ReadCompressDataItem(const nicInt32 tagID, nicTime *ptimes, nicData **out_pdataitems, nicInt32 *out_datacount);

/*
* 函数功能：读取tagID测点某一个时刻的历史数据，数据值由插值计算获得
* 输入参数：测点ID：tagID，时刻点ptime
* 输出参数：读出的历史数据out_pdataitem
* 返回值　：读取结果
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_ReadOneDataItem(const nicInt32 tagID, nicTime *ptime, nicData **out_pdataitem);

/*
* 函数功能：按照个数，读取tagID测点的数据。读出的时刻点是根据起至时间和目标个数平均分布的，值由插值计算获得
* 输入参数：测点ID：tagID，关键时间点ptimes，数据的个数datacount
* 输出参数：读出的压缩数据out_pdataitems
* 返回值　：读取结果
* 备    注：ptimes数组要最少初始化5个大小，ptimes[0]表示读取时间段的起始时刻，ptimes[1]表示时间段的终止时刻，函数在内部会使用ptimes[2]
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_ReadDataItemsByCount(const nicInt32 tagID, nicTime *ptimes, nicInt32 datacount, nicData **out_pdataitems);

/*
* 函数功能：按照特定的时间间隔，读取tagID测点的数据。数据值由插值计算获得
* 输入参数：测点ID：tagID，关键时间点ptimes
* 输出参数：读出的压缩数据out_pdataitems，返回数据的个数out_datacount
* 返回值　：读取结果
* 备    注：ptimes数组要最少初始化5个大小，ptimes[0]表示读取时间段的起始时刻，ptimes[1]表示时间段的终止时刻，ptimes[2]表示特定的时间间隔。
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_ReadDataItemsByInterval(const nicInt32 tagID, nicTime *ptimes, nicData **out_pdataitems, nicInt32 *out_datacount);

/*
* 函数功能：根据mode的设置，统计计算tagID测点某时间段内的数据特点
* 输入参数：测点ID：tagID，关键时间点ptimes，统计计算类型summarymode
* 输出参数：返回统计计算结果out_datasummary，好数据所占百分比out_pctgooddata（最大为1）
* 返回值　：统计计算结果
* 备    注：1、在任何模式下，ptimes数组要最少初始化5个大小，ptimes[0]表示读取时间段的起始时刻，ptimes[1]表示时间段的终止时刻，其余用于返回值使用
*           2、NICHIS_DATASUMMARY_MAXIMUM时，ptimes[3]返回最大值对应的时刻
*           3、NICHIS_DATASUMMARY_MAXIMUM时，ptimes[3]返回最小值对应的时刻
*           4、NICHIS_DATASUMMARY_RANGE时，ptimes[3]返回最小值对应的时刻，ptimes[4]返回最大值对应的时刻
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_HisdataSummary(const nicInt32 tagID, nicTime *ptimes, nicInt8 summarymode, nicDouble *out_pctgooddata, nicDouble *out_datasummary);

CDECL_END

#endif