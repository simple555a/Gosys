//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update   
//= (21-1)         1.0.0.21      Zander Sun      2013-11-28      增加解析出来的IO列表

#ifndef __NICSYS_DEVDPU_ONETRECV_H_
#define __NICSYS_DEVDPU_ONETRECV_H_

#include "ONetPctl.h"
//上行20H协议： 系统中所有DPU数据（包括IO和状态）全部上送
class CONetPctl_20H:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:
	explicit CONetPctl_20H(ONet_DB *db);
	~CONetPctl_20H();



	//普通数据帧只处理从info_code(包含)开始的信息
	__i32 parse_packet(
		__u8 *recvBuff_,
		__u32 recvLen_,
		__u8 curNetNum_);
	
	__i32   do_packet_to_onetdb(
		__u8 * buff_, 
		__u32 len_,
		__bool isHaveTime_,
		__u8 cpuNum_);
	
	//处理DPU的IDV信息,根据协议中的内容只取一部分信息，具体要用时再扩展解析
	__i32  do_idvbuff_to_dpustatus(
		__u8 * buff_,
		__bool isDpuMaster_ );

	//处理IO的IDV信息,根据协议中的内容只取一部分信息，具体要用时再扩展解析
	__i32  do_idvbuff_to_iostatus(
		__u8 * buff_,
		__u8 ioCardAddr_,
		__u8 ioModType_ );

	__i32 do_databuff_to_iedvalue(
		__u8 ioAddr_,		//IO卡逻辑地址
		__u8 *qbuff_,		//质量戳指针
		__u8 qLen_,			//质量区长度
		__u8 *valbuff_,		//数据区指针
		__u8 vLen_,			//数据区长度
		__u8 qSize_);		//质量戳占有字节

	__i32 do_databuff_to_vptvalue(
		__u8 ioAddr_,		//IO逻辑地址
		__u8 *&valbuff_,		//VPT的数据指针
		__u16 vCount_ )	;	//VPT的VPT_VALUE的个数


	__i32 do_databuff_to_iovalue(
		__u8 ioAddr_,		//IO卡逻辑地址
		__u8 *qbuff_,		//质量戳指针
		__u8 qLen_,			//质量区长度
		__u8 *valbuff_,		//数据区指针
		__u8 vLen_,			//数据区长度
		IO_DATA_ATTR *datattr_,	//值属性数组
		__u8 qSize_);		//质量戳占有字节
	__i32   do_iobuffer_to_channelvalue(
		__u8 * & buff_,
		__u8 ioCardAddr_,
		__u8 ioModType_);
private:
	//__u8 _oldFrameNum; //上一次多帧的帧号
	__u32 time_mark_;
	__u8 *_tempData;//接收多帧包时的临时缓冲
	//__bool exioexist[MAX_IO_CARD];     // 上一时刻的IO存在状态
	vector<__u8>iolist;    // 这次解析出的IO列表  Zander 2013-11-28
};

//上行15H协议： 错误码上传
class CONetPctl_15H:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:
	explicit CONetPctl_15H(ONet_DB *db);
	~CONetPctl_15H();

	//普通数据帧只处理从info_code(包含)开始的信息
	__i32 parse_packet(
		__u8 *recvBuff_,
		__u32 recvLen_,
		__u8 curNetNum_);
};

//上行40H协议： 事件上传
class CONetPctl_40H:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:
	explicit CONetPctl_40H(ONet_DB *db);
	~CONetPctl_40H();

	//普通数据帧只处理从info_code(包含)开始的信息
	__i32 parse_packet(
		__u8 *recvBuff_,
		__u32 recvLen_,
		__u8 curNetNum_);
};

#endif