


#ifndef __NICSYS_DEVDPU_ONETSEND_H_
#define __NICSYS_DEVDPU_ONETSEND_H_

#include "ONetPctl.h"


/**
* @class CONetPctl
* @brief 0X10H 0X11H 0X12H协议封装类  
* @author CNCS-0136
* @note 0X10H召唤HDPU中全部数据 
* 0X11H 召唤HDPU中全部数据,不包括DPU状态和IO状态 
* 0X12H召唤HDPU中的所有状态数据。包括DPU和IO状态数据
*/
class CONetPctl_10H_11H_12H:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:			
	explicit CONetPctl_10H_11H_12H(__u8 pro_code_);
	~CONetPctl_10H_11H_12H();

public:

	virtual __i32 create_frame();

	void set_call_style(__u8 substyle_);
	__u8 get_call_style();
	void set_call_periods( __u16 predtime_);
	__u16 get_call_periods();
private:
	__u8 _cmdStyle;// 0x01为取消订阅、0x02订购上传、0x03普通数据一次性召唤
	__u16 _subTime;		//订阅周期时间10---65535ms

};

/**
* @class CONetPctl
* @brief 0X13H 0X14H 0X16H协议 下行
* @author CNCS-0136
* @note 0X13H多个DPU的所有数据，包括状态数据和IO数据
* 0X14H召唤多个DPU的IO数据，不包括状态数据
* 0X16H召唤多个DPU的状态数据，不包括IO数据
*/
class CONetPctl_13H_14H_16H_18H:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:
	explicit CONetPctl_13H_14H_16H_18H(__u8 pro_code_);

	~CONetPctl_13H_14H_16H_18H();

public:

	virtual __i32 create_frame();
	void set_call_style(__u8 substyle_);
	__u8 get_call_style();
	void set_call_periods( __u16 predtime_);
	__u16 get_call_periods();
	__u32 add_dpu(__u8 dpuAddr);
	__u32 del_dpu(__u8 dpuAddr);

private:
	vector<__u8> _dpus;
	__u8 _cmdStyle;// 0x01为取消订阅、0x02订购上传、0x03普通数据一次性召唤
	__u16 _subTime;		//订阅周期时间10---65535ms

};

struct  DPU_IO_ADDR
{
	__u8 dpuAddr;
	__u8 ioCardAddr;
	inline int operator <(const DPU_IO_ADDR & temp_)
	{
		if (dpuAddr < temp_.dpuAddr)
		{
			return 1;
		}
		else if(dpuAddr == temp_.dpuAddr)
		{
			return ioCardAddr < temp_.ioCardAddr ? 1:0;

		}
		else
		{
			return 0;
		}	
	}
	inline bool operator == (const DPU_IO_ADDR & temp_ )
	{
		return ((dpuAddr == temp_.dpuAddr) && (ioCardAddr ==temp_.ioCardAddr))? true: false;
	}
};
/**
* @class CONetPctl
* @brief 协议类仓库
* @author CNCS-0136
* @note 
*/
class CONetPctl_17H_19H:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:
	explicit CONetPctl_17H_19H(__u8 pro_code_);

	~CONetPctl_17H_19H();


	typedef vector<DPU_IO_ADDR>  DPU_IO_LIST;
public:

	virtual __i32 create_frame();
	void set_call_style(__u8 substyle_);
	__u8 get_call_style();

	void set_call_periods( __u16 predtime_);
	__u16 get_call_periods();
	__u32 add_dpu_io(__u8 dpuAddr_,__u8 ioAddr_);
	__u32 del_dpu_io(__u8 dpuAddr_,__u8 ioAddr_);
private:
	DPU_IO_LIST _dpus_ios;
	__u8 _cmdStyle;// 0x01为取消订阅、0x02订购上传、0x03普通数据一次性召唤
	__u16 _subTime;		//订阅周期时间10---65535ms

};
//置AI、AO值协议
class CONetPctl_33H_34H_35H_36H_3A:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:
	explicit CONetPctl_33H_34H_35H_36H_3A(__u8 pro_code_);

	~CONetPctl_33H_34H_35H_36H_3A();

public:

	virtual __i32 create_frame();

	__i32 set_control_io(__u8 dpuAddr_,
		__u8 ioAddr_,
		__u8 groupID_,
		__u8 ptId_,
		__u8 valType_,
		pmc_value_t pVal_);

	__i32 set_control_vpt(__u8 dpuAddr_,
		__u8 ioAddr_,
		__u8 valType_,
		__u16 ioChannel_,
		__u8 ioSubChannel_,
		pmc_value_t pVal_);

private:
	__u8 _dpuAddr;
	__u8 _ioAddr;
	__u8 _groupID;
	__u8 _ptId;
	__u8 _valueType;
	__u8 _valLength;
	pmc_value_t _value;//下行要设置的值
	__u16 _ioChannel ;
	__u8 _ioSubChannel;

};

//***测试帧、建立链路、停止链路请求下行协议类，确认返回协议在上行类中
class CONetPctl_Link:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:
	explicit CONetPctl_Link(__u8 info_code);
	~CONetPctl_Link();
	/** @} */

	virtual __i32 create_frame();


	void set_link_time(__u8 settime_);
	__u8 get_link_time( );

	__i32 reset_frame();

	void set_link(
		__u8 linkMod_,
		__u8 linkTime_,
		__u8 linkNet_,
		__u8 linkAck_ = LINK_STYLE_NULL);

private:
	__u8 _linkModStyle; //帧类型:一般帧、T1帧、T2帧、请求类型
	__u8 _linkRqtStyle; // 请求类型：测试、连接、停止
	__u8 _linkNet; //1:1网注册，2:2网注册
	__u8 _tTime;	//连接时间
	__bool  _isHeartAck;//是否采用心跳线测试
};


//***对时协议
class CONetPctl_78H:public CONetBase
{
	/**
	* @name 构造
	* @{
	*/
public:
	explicit CONetPctl_78H(__u8 info_code);
	~CONetPctl_78H();
	/** @} */

	virtual __i32 create_frame();


private:
	ACE_Time_Value _curTime;
};
#endif