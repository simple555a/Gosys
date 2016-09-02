
#include "precomp.h"
#include "ONetPctlSend.h"
#include "ace/Date_Time.h"
//#include "pmc_value_t.h"

CONetPctl_10H_11H_12H::CONetPctl_10H_11H_12H(__u8 pro_code_):
		CONetBase( __true/**downFlag_*/,__false/**CRCCheck_*/,__false/**mutilFlag_*/,0),
		_cmdStyle(ONet_Const::CMD_DOWN_CALL),
		_subTime(DEFUALT_LINKTEST_TIME * 1000)
{
	set_info_code(pro_code_);
	set_description_code(_cmdStyle);
}/**< @brief 构造 */

CONetPctl_10H_11H_12H::~CONetPctl_10H_11H_12H()
{

}/**< @brief 析构 */
/** @} */


__i32 CONetPctl_10H_11H_12H::create_frame()
{
	set_data(0,0);
	return RET_OK;

}
//CMD_DOWN_SUBSCRIBE 
void CONetPctl_10H_11H_12H::set_call_style(__u8 substyle_)
{
	__u8 oldstyle =get_description_code();
	set_description_code(substyle_);
	_cmdStyle = substyle_;
}
__u8 CONetPctl_10H_11H_12H::get_call_style()
{
	return _cmdStyle;
}

void CONetPctl_10H_11H_12H::set_call_periods( __u16 predtime_)
{
	if (predtime_  < 10 || predtime_ > 0xFFFF)
	{
		predtime_ = DEFUALT_LINKTEST_TIME * 1000;
	}
	_subTime = predtime_;
	set_control_area_1(LOW_8_BIT(_subTime)& H11_TIMER_MASK);
	set_control_area_2(HIGH_8_BIT(_subTime));		
}
__u16 CONetPctl_10H_11H_12H::get_call_periods()
{
	return get_control_area_2() * 0x100 + get_control_area_1();
}



CONetPctl_13H_14H_16H_18H::CONetPctl_13H_14H_16H_18H(__u8 pro_code_):
	CONetBase( __true,__false,__false,0),
	_cmdStyle(ONet_Const::CMD_DOWN_CALL),
	_subTime(DEFUALT_LINKTEST_TIME * 1000)
{
	set_info_code(pro_code_);
}/**< @brief 构造 */

CONetPctl_13H_14H_16H_18H::~CONetPctl_13H_14H_16H_18H()
{

}/**< @brief 析构 */
/** @} */



__i32 CONetPctl_13H_14H_16H_18H::create_frame()
{
	//新建dpu数组空间 + 1字节的dpu个数
	__u8 dpsCount = _dpus.size();
	__u8 *buff = new __u8[dpsCount+ 1];

	ACE_OS::memcpy(buff,&dpsCount,1);

	for (__u8 i = 0 ; i < dpsCount; i++)
	{
		ACE_OS::memcpy(buff+1+i,&_dpus.at(i),1);
	}
	set_data(buff,dpsCount + 1);
	delete []buff;
	return RET_OK;

};
void CONetPctl_13H_14H_16H_18H::set_call_style(__u8 substyle_)
{
	__u8 oldstyle =get_protocol_type();
	_cmdStyle = substyle_& PROCOTOL_STYLE_MARK;
	set_protocol_type(_cmdStyle|(oldstyle &~PROCOTOL_STYLE_MARK));
}
__u8 CONetPctl_13H_14H_16H_18H::get_call_style()
{
	return _cmdStyle;
}

void CONetPctl_13H_14H_16H_18H::set_call_periods( __u16 predtime_)
{
	if (predtime_  < 10 || predtime_ > 0xFFFF)
	{
		predtime_ = DEFUALT_LINKTEST_TIME * 1000;
	}
	_subTime = predtime_;
	set_control_area_1(LOW_8_BIT(_subTime));
	set_control_area_2(HIGH_8_BIT(_subTime));		
}
__u16 CONetPctl_13H_14H_16H_18H::get_call_periods()
{
	return get_control_area_2() * 0x100 + get_control_area_1();
}
__u32 CONetPctl_13H_14H_16H_18H::add_dpu(__u8 dpuAddr)
{
	if (_dpus.size() > DPU_MAX_SIZE)
	{
		return RET_Fail;

	}
	_dpus.push_back(dpuAddr);
	return RET_OK;
}
__u32 CONetPctl_13H_14H_16H_18H::del_dpu(__u8 dpuAddr)
{
	if (_dpus.empty())
	{
		return RET_Fail;

	}
	vector<__u8>::iterator itor = find(_dpus.begin(),_dpus.end(),dpuAddr);
	if(itor != _dpus.end())
	{
		_dpus.erase(itor);

	}
	return RET_OK;
}

 CONetPctl_17H_19H::CONetPctl_17H_19H(__u8 pro_code_):
	CONetBase( __true,__false,__false,0),
	_cmdStyle(ONet_Const::CMD_DOWN_CALL),
	_subTime(DEFUALT_LINKTEST_TIME * 1000)
{
	set_info_code(pro_code_);
}/**< @brief 构造 */

CONetPctl_17H_19H::~CONetPctl_17H_19H()
{

}/**< @brief 析构 */
/** @} */

__i32 CONetPctl_17H_19H::create_frame()
{
	__u8 io_count = _dpus_ios.size();
	__u8 buffSize = io_count*sizeof(DPU_IO_ADDR) + 1;
	__u8 * buffer = new __u8[buffSize];

	memcpy(buffer,&io_count,1);

	for (__u8 i = 0 ; i < io_count; i++)
	{
		ACE_OS::memcpy(buffer + 1 + i*sizeof(DPU_IO_ADDR),&_dpus_ios.at(i),sizeof(DPU_IO_ADDR));
	}
	set_data(buffer,buffSize);
	return RET_OK;

}
void CONetPctl_17H_19H::set_call_style(__u8 substyle_)
{
	__u8 oldstyle =get_protocol_type();
	_cmdStyle = substyle_& PROCOTOL_STYLE_MARK;
	set_protocol_type(_cmdStyle|(oldstyle &~PROCOTOL_STYLE_MARK));
}
__u8 CONetPctl_17H_19H::get_call_style()
{
	return _cmdStyle;
}

void CONetPctl_17H_19H::set_call_periods( __u16 predtime_)
{
	if (predtime_  < 10 || predtime_ > 0xFFFF)
	{
		predtime_ = DEFUALT_LINKTEST_TIME * 100;
	}
	_subTime = predtime_;
	set_control_area_1(LOW_8_BIT(_subTime)&H11_TIMER_MASK);
	set_control_area_2(HIGH_8_BIT(_subTime));		
}
__u16 CONetPctl_17H_19H::get_call_periods()
{
	return get_control_area_2() * 0x100 + get_control_area_1();
}
__u32 CONetPctl_17H_19H::add_dpu_io(__u8 dpuAddr_,__u8 ioAddr_)
{
	if (_dpus_ios.size() > DPU_MAX_SIZE)
	{
		return RET_Fail;
	}
	DPU_IO_ADDR temp;
	temp.dpuAddr = dpuAddr_;
	temp.ioCardAddr = ioAddr_;

	DPU_IO_LIST::iterator  iter = find(_dpus_ios.begin(),_dpus_ios.end(),temp);
	if (iter == _dpus_ios.end())
	{
		_dpus_ios.push_back(temp);
	}
	return RET_OK;
}
__u32 CONetPctl_17H_19H::del_dpu_io(__u8 dpuAddr_,__u8 ioAddr_)
{
	//查找是否已经存在
	DPU_IO_ADDR temp;
	temp.dpuAddr = dpuAddr_;
	temp.ioCardAddr = ioAddr_;
	DPU_IO_LIST::iterator &iter = find(_dpus_ios.begin(),_dpus_ios.end(),temp);
	//不存在则视为删除成功
	if (iter != _dpus_ios.end())
	{
		_dpus_ios.erase(iter);
	}
	return RET_OK;

}

CONetPctl_33H_34H_35H_36H_3A::CONetPctl_33H_34H_35H_36H_3A(__u8 pro_code_):
	CONetBase( __true,__false,__false,0),
	_dpuAddr(0x00),
	_ioAddr(0x00),
	_groupID(0x00),
	_ptId(0x00),
	_valLength(0x01),
	_valueType(BOOL_1_BIT),
	_ioChannel(0x0000),
	_ioSubChannel(0x00)
{
	//_value.DataType = DBDataType_Empty;
	set_value_type(_value.Flags, dt_null);
	set_info_code(pro_code_);
}/**< @brief 构造 */

CONetPctl_33H_34H_35H_36H_3A::~CONetPctl_33H_34H_35H_36H_3A()
{

}/**< @brief 析构 */
/** @} */


__i32 CONetPctl_33H_34H_35H_36H_3A::create_frame()
{
	__u8 tempValBuffer[4] ={0};
	char *p = NULL;//临时指针
	pmc_value_t tempValue;
	switch(_valueType)
	{
	case BOOL_1_BIT:
		//VariantTypeCast(DBDataType_Bool,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_bool);
		pmc_type_cast(&_value, &tempValue);
		//tempValue = _value;
		if(get_info_code() == DOWN_CONTROL_VPT)
		{
			_valLength = 2;
			tempValBuffer[0] = 0x01<<_ioSubChannel;
			tempValBuffer[1] = tempValue.Value.b?0xFF:0x00;
		}
		else
		{
			_valLength = 1;
			//BOOL量非零都是0xFF值
			tempValBuffer[0] =  tempValue.Value.b?0xFF:0x00;
		}

		break;
		//先发送值的高位
	case FLOAT_4_BYTE:
		//VariantTypeCast(DBDataType_Float,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_float);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 4;
		p = (char *)&tempValue.Value.flt;
		tempValBuffer[0] = p[3];
		tempValBuffer[1] = p[2];
		tempValBuffer[2] = p[1];
		tempValBuffer[3] = p[0];
		break;
		//2、3 字节浮点型本质是将低2、1字节的尾数去掉传输，
		//在转换为4字节float的时候都填充为0
	case FLOAT_3_BYTE:
		//VariantTypeCast(DBDataType_Float,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_float);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 3;
		p = (char *)&tempValue.Value.flt;
		tempValBuffer[0] = p[3];
		tempValBuffer[1] = p[2];
		tempValBuffer[2] = p[1];
		break;
	case FLOAT_2_BYTE:
		//VariantTypeCast(DBDataType_Float,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_float);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 2;
		p = (char *)&tempValue.Value.flt;
		tempValBuffer[0] = p[3];
		tempValBuffer[1] = p[2];
		break;
	case UNSIGNED_INT_4_BYTE:
		//VariantTypeCast(DBDataType_UInt32,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_uint32);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 4;
		p = (char *)&tempValue.Value.u32;
		tempValBuffer[0] = p[3];
		tempValBuffer[1] = p[2];
		tempValBuffer[2] = p[1];
		tempValBuffer[3] = p[0];
		break;
	case UNSIGNED_INT_2_BYTE:
		//VariantTypeCast(DBDataType_UInt16,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_uint16);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 2;
		p = (char *)&tempValue.Value.u16;
		tempValBuffer[0] = p[1];
		tempValBuffer[1] = p[0];
		break;
	case UNSIGNED_INT_1_BYTE:
		//VariantTypeCast(DBDataType_UInt16,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_uint8);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 1;
		p = (char *)&tempValue.Value.u8;
		tempValBuffer[0] = p[0];
		break;
	case INT_4_BYTE:
		//VariantTypeCast(DBDataType_Int32,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_int32);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 4;
		p = (char *)&tempValue.Value.i32;
		tempValBuffer[0] = p[3];
		tempValBuffer[1] = p[2];
		tempValBuffer[2] = p[1];
		tempValBuffer[3] = p[0];
		break;
	case INT_2_BYTE:
		//VariantTypeCast(DBDataType_Int16,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_int16);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 3;
		p = (char *)&tempValue.Value.i16;
		tempValBuffer[0] = p[1];
		tempValBuffer[1] = p[0];
		break;
	case INT_1_BYTE:
		//VariantTypeCast(DBDataType_Int8,&_value,&tempValue);
		set_value_type(tempValue.Flags, dt_int8);
		pmc_type_cast(&_value,&tempValue);
		_valLength = 1;
		p = (char *)&tempValue.Value.i8;
		tempValBuffer[0] = p[0];
		break;
	default:
		set_data(0,0);
		return RET_Fail;
	}
	__u16 buffSize = _valLength + 5;
	__u8 *buffer = new __u8 [buffSize];
	ACE_OS::memcpy(buffer +0,&_dpuAddr,1);
	ACE_OS::memcpy(buffer +1,&_ioAddr,1);
	ACE_OS::memcpy(buffer +2,&_groupID,1);
	ACE_OS::memcpy(buffer +3,&_ptId,1);
	buffer[4] = _valueType;

	ACE_OS::memcpy(buffer +5,tempValBuffer,_valLength);
	set_data(buffer,buffSize);
	delete [] buffer;
	return RET_OK;

};
__i32 CONetPctl_33H_34H_35H_36H_3A::set_control_io(__u8 dpuAddr_,
	__u8 ioAddr_,
	__u8 groupID_,
	__u8 ptId_,
	__u8 valType_,
	pmc_value_t pVal_)
{
	if(get_info_code() == DOWN_CONTROL_VPT)
	{
		return RET_Fail;
	}
	_dpuAddr = dpuAddr_;
	_ioAddr = ioAddr_;
	_groupID = groupID_;
	_ptId = ptId_;
	_valueType = valType_;
	_value = pVal_;
	return RET_OK;
}

__i32 CONetPctl_33H_34H_35H_36H_3A::set_control_vpt(__u8 dpuAddr_,
	__u8 ioAddr_,
	__u8 valType_,
	__u16 ioChannel_,
	__u8 ioSubChannel_,
	pmc_value_t pVal_)
{
	if(get_info_code() != DOWN_CONTROL_VPT)
	{
		return RET_Fail;
	}
	_dpuAddr = dpuAddr_;
	_ioAddr = ioAddr_;
	_ioChannel = ioChannel_;
	_ioSubChannel = ioSubChannel_;
	_groupID = HIGH_8_BIT(_ioChannel);//offset;
	_ptId = LOW_8_BIT(_ioChannel);//若6007  startchannel = 7d_;
	_valueType = valType_;
	_value = pVal_;
	return RET_OK;

}


CONetPctl_Link::CONetPctl_Link(__u8 info_code):CONetBase( __true,__false,__false,0),
_linkModStyle(T1_TIMEOUT_FRAME|FRAME_FORMAT_MASK),
_linkRqtStyle(LINK_STYLE_TEST),
_linkNet(LINK_NET_0),
_tTime(DEFUALT_LINKTEST_TIME),
_isHeartAck(__true)

{
	set_is_test_frame(__true);
}/**< @brief 构造 */

CONetPctl_Link::~CONetPctl_Link()
{

}/**< @brief 析构 */
/** @} */

__i32 CONetPctl_Link::create_frame()
{
	//此类帧不是普通帧
	if (_linkModStyle == FRAM_STYLE_GENERAL)
	{
		return RET_Fail;
	}
	//T1帧，高4位为设置时间，3、4位为请求和回应确认，此处为下行请求，1、2位为帧类型
	else if(_linkModStyle == FRAM_STYLE_T1 && _isHeartAck == __true)
	{
		set_control_area_1(_tTime << 4|T1_REQUEST_MASK|FRAM_STYLE_T1);
	}
	//T2帧，高4位为设置时间，3、4位为请求和回应确认，此处为下行请求，1、2位为帧类型
	else if(_linkModStyle == FRAM_STYLE_T2)
	{
		set_control_area_1(_tTime << 4|T2_REQUEST_MASK|FRAM_STYLE_T2);
	}
	//测试连接帧，1、2位为帧类型，3、4位 为stop的请求、确认，5、6为start的请求确认，7、8位为test的请求确认
	else if(_linkModStyle == FRAM_STYLE_LINK)
	{
		if (_linkRqtStyle == LINK_STYLE_TEST)
		{
			set_control_area_1(LINK_TEST_REQUEST|FRAM_STYLE_LINK);
		}
		else if (_linkRqtStyle == LINK_STYLE_START)
		{
			set_control_area_2(_linkNet);
			set_control_area_1(LINK_START_REQUEST|FRAM_STYLE_LINK);
		}
		else if (_linkRqtStyle == LINK_STYLE_STOP)
		{
			set_control_area_2(_linkNet);
			set_control_area_1(LINK_STOP_REQUEST|FRAM_STYLE_LINK);
		}
		else if (_linkRqtStyle == LINK_STYLE_TEST)
		{
			set_control_area_1(LINK_STOP_REQUEST|FRAM_STYLE_LINK);
		}
		else
		{
			set_data(0,0);
			return RET_Fail;
		}
	}
	else
	{
		set_data(0,0);
		return RET_Fail;
	}

	set_data(0,0);
	return RET_OK;
}


void CONetPctl_Link::set_link_time(__u8 settime_)
{
	if (_linkModStyle == FRAM_STYLE_T1 || _linkModStyle == FRAM_STYLE_T2)
	{
		_tTime = (settime_ >= 2 && settime_ <= 16)? settime_: DEFUALT_LINKTEST_TIME;

	}
}
__u8 CONetPctl_Link::get_link_time( )
{
	return _tTime ;
}
//由于此帧可以用作不同用途，在每种用途之前都要将packet内容清空
__i32 CONetPctl_Link::reset_frame()
{
	clear_data();
	memset(&_packData,0,sizeof(FD_PACKET_ONet));
	return RET_OK;

}
void CONetPctl_Link::set_link(
				 __u8 linkMod_,
				 __u8 linkTime_,
				 __u8 linkNet_,
				 __u8 linkAck_)
{
	_linkModStyle = linkMod_;
	_tTime = linkTime_;
	_linkNet = linkNet_;
	//T1帧中设置时间为0，则不采用心跳判断。
	if (_linkModStyle == FRAM_STYLE_T1 && _tTime == 0)
	{
		_isHeartAck = __false;
	}
	if (_linkModStyle == FRAM_STYLE_LINK)
	{
		_linkRqtStyle = linkAck_;
	}

}

////下行对时协议
CONetPctl_78H::CONetPctl_78H(__u8 info_code):
			CONetBase( __true,__false,__false,0)
{
	set_info_code(info_code);

}
CONetPctl_78H::~CONetPctl_78H()
{

}
/** @} */

__i32 CONetPctl_78H::create_frame()
{
	__u8 buff[6] = {0};
	ACE_Date_Time dt;
	dt.update();
	buff[0] = dt.second();
	buff[1] = dt.minute();
	buff[2] = dt.hour();
	buff[3] = dt.day();
	buff[4] = dt.month();
	buff[5] = dt.year() - 2000;

	set_data(buff,6);

	return RET_OK;

}