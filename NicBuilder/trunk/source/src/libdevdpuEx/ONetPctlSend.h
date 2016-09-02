


#ifndef __NICSYS_DEVDPU_ONETSEND_H_
#define __NICSYS_DEVDPU_ONETSEND_H_

#include "ONetPctl.h"


/**
* @class CONetPctl
* @brief 0X10H 0X11H 0X12HЭ���װ��  
* @author CNCS-0136
* @note 0X10H�ٻ�HDPU��ȫ������ 
* 0X11H �ٻ�HDPU��ȫ������,������DPU״̬��IO״̬ 
* 0X12H�ٻ�HDPU�е�����״̬���ݡ�����DPU��IO״̬����
*/
class CONetPctl_10H_11H_12H:public CONetBase
{
	/**
	* @name ����
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
	__u8 _cmdStyle;// 0x01Ϊȡ�����ġ�0x02�����ϴ���0x03��ͨ����һ�����ٻ�
	__u16 _subTime;		//��������ʱ��10---65535ms

};

/**
* @class CONetPctl
* @brief 0X13H 0X14H 0X16HЭ�� ����
* @author CNCS-0136
* @note 0X13H���DPU���������ݣ�����״̬���ݺ�IO����
* 0X14H�ٻ����DPU��IO���ݣ�������״̬����
* 0X16H�ٻ����DPU��״̬���ݣ�������IO����
*/
class CONetPctl_13H_14H_16H_18H:public CONetBase
{
	/**
	* @name ����
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
	__u8 _cmdStyle;// 0x01Ϊȡ�����ġ�0x02�����ϴ���0x03��ͨ����һ�����ٻ�
	__u16 _subTime;		//��������ʱ��10---65535ms

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
* @brief Э����ֿ�
* @author CNCS-0136
* @note 
*/
class CONetPctl_17H_19H:public CONetBase
{
	/**
	* @name ����
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
	__u8 _cmdStyle;// 0x01Ϊȡ�����ġ�0x02�����ϴ���0x03��ͨ����һ�����ٻ�
	__u16 _subTime;		//��������ʱ��10---65535ms

};
//��AI��AOֵЭ��
class CONetPctl_33H_34H_35H_36H_3A:public CONetBase
{
	/**
	* @name ����
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
	pmc_value_t _value;//����Ҫ���õ�ֵ
	__u16 _ioChannel ;
	__u8 _ioSubChannel;

};

//***����֡��������·��ֹͣ��·��������Э���࣬ȷ�Ϸ���Э������������
class CONetPctl_Link:public CONetBase
{
	/**
	* @name ����
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
	__u8 _linkModStyle; //֡����:һ��֡��T1֡��T2֡����������
	__u8 _linkRqtStyle; // �������ͣ����ԡ����ӡ�ֹͣ
	__u8 _linkNet; //1:1��ע�ᣬ2:2��ע��
	__u8 _tTime;	//����ʱ��
	__bool  _isHeartAck;//�Ƿ���������߲���
};


//***��ʱЭ��
class CONetPctl_78H:public CONetBase
{
	/**
	* @name ����
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