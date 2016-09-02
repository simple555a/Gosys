#include "precomp.h"
#include "ONetPctl.h"
#include "ONetPctlRecv.h"
#include "ONetPctlSend.h"


//包含构造函数，可以防止基类被实例化
CONetBase::CONetBase(__u8 downFlag_,__bool CRCCheck_,__bool mutilFlag_,ONet_DB *db):
_isDown(downFlag_),
_isCRCCheck(CRCCheck_),
_isMutilPacket (mutilFlag_) ,
_isMutilEnd(__false),
_isTestFrame(__false),
_packetLen(0),
_ONetdb(db)
{
	_packData.sync_header = SYNC_HEADER ;
	_packData.dest_addr = INVALID_ONET_CODE;
	_packData.source_addr = INVALID_ONET_CODE;
	_packData.control_area_1 = GENERAL_DATA_FRAME;
	_packData.control_area_2 = INVALID_ONET_CODE ;
	_packData.control_area_3 = INVALID_ONET_CODE;
	_packData.protocol_type  = ONet_Const::ONET_REAL_TRANSFER_PROTOCOL;
	_packData.length_low = 0x02;//长度默认为2字节 =信息码+信息描述
	_packData.length_high = INVALID_ONET_CODE | (PROTOCOL_VERSION << 4);
	_packData.info_code = INVALID_ONET_CODE;
	_packData.description_code = INVALID_ONET_CODE;
	_packData.data = NULL;
	_packData.crc16= CRCCheck_? 0x0000:NO_CRC_CHECK ;

	_oldCrc = 0;

};	/**< @brief 构造 */


CONetBase::~CONetBase()
{
	if ((_packData.data))
	{
		delete _packData.data;
		_packData.data = NULL;
	}
};/**< @brief 析构 */

//得到数据包头
__u8 CONetBase::get_pctl_header()
{
	return _packData.sync_header;
}
//设置数据包头
void CONetBase::set_pctl_header(__u8 sync_header_)
{
	_packData.sync_header = sync_header_;
}
__u8 CONetBase::get_pctl_dest_addr()
{
	return _packData.dest_addr;
}
void CONetBase::set_pctl_dest_addr(__u8 dest_addr_)
{
	_packData.dest_addr = dest_addr_;
}
__u8 CONetBase::get_pctl_source_addr()
{
	return _packData.source_addr;
}
void CONetBase::set_pctl_source_addr(__u8 source_addr_)
{
	_packData.source_addr = source_addr_;
}
__u8 CONetBase::get_protocol_type()
{
	return _packData.protocol_type;
}
void CONetBase::set_protocol_type(__u8 protocol_type_)
{
	_packData.protocol_type = protocol_type_;
}
__u8 CONetBase::get_control_area_1()
{
	return _packData.control_area_1;
}
void CONetBase::set_control_area_1(__u8 control_area_1_)
{
	_packData.control_area_1 = control_area_1_;
}
__u8 CONetBase::get_control_area_2()
{
	return _packData.control_area_2;
}
void CONetBase::set_control_area_2(__u8 control_area_2_)
{
	_packData.control_area_2= control_area_2_;
}
__u8 CONetBase::get_control_area_3()
{
	return _packData.control_area_3;
}
void CONetBase::set_control_area_3(__u8 control_area_3_)
{
	_packData.control_area_3 = control_area_3_;
}

//数据区长度低8位码
__u8 CONetBase::get_length_lowcode()
{
	return _packData.length_low;
}
void CONetBase::set_length_lowcode(__u8 length_low_)
{
	_packData.length_low = length_low_;
}

//数据区长度高8位码(长度高位码4位+4位版本码)
__u8 CONetBase::get_length_highcode()
{
	return _packData.length_high;
}
void CONetBase::set_length_highcode(__u8 length_high_)
{
	_packData.length_high = length_high_;
}

//得到协议类型，即协议信息码
__u8 CONetBase::get_info_code()
{
	if(_isTestFrame)
	{
		return 0;
	}
	return _packData.info_code & ~MULTI_PACKET;
};
//设置协议类型，即协议信息码
void CONetBase::set_info_code(__u8 info_code_)
{
	if(_isTestFrame)
	{
		return;
	}
	//一字节有两个作用的都必须保持原有另一个作用
	__u8 oldcode = _packData.info_code & MULTI_PACKET;

	_packData.info_code = (info_code_ & ~MULTI_PACKET)| oldcode ;
}
__u8 CONetBase::get_description_code()
{
	if(_isTestFrame)
	{
		return 0;
	}
	return _packData.description_code;
}
void CONetBase::set_description_code(__u8 description_code_)
{
	if(_isTestFrame)
	{
		return;
	}
	_packData.description_code = description_code_;
}
//得到数据区十进制的长度(不含校验位)
__u16 CONetBase::get_data_length()
{
	if(_isTestFrame)
	{
		return 0;
	}
	//如果是单帧数据，信息码+描述码+data
	if (_isMutilPacket == __false)
	{
		return get_buffer_length() - 2;
	}
	else 
	{
		//如果是多帧数据，信息码+描述码+帧号低+帧号高+data
		return get_buffer_length() - 4;
	}
}
//判断是否为多帧
__bool CONetBase::get_is_multi_packet()
{
	if(_isTestFrame)
	{
		return __false;
	}
	_isMutilPacket = (_packData.info_code & MULTI_PACKET) ? __true :__false;
	return _isMutilPacket;
}
//设置为多帧
void CONetBase::set_is_multi_packet()
{
	if(_isTestFrame)
	{
		return;
	}
	_packData.info_code |= MULTI_PACKET;
	_isMutilPacket = __true;
}
//判断是否为多帧最末帧
__bool CONetBase::get_is_multipacket_end()
{
	if(_isTestFrame)
	{
		return __false;
	}
	if ((_packData.data) && get_is_multi_packet())
	{
		_isMutilEnd =(*(_packData.data + 1)) & MUTIL_PACKET_END_MARK ? __true :__false;			
	}
	else
	{
		_isMutilEnd = __false;
	}
	return _isMutilEnd;
}
//设置为多帧的尾部 要在setdata之后才能执行有效
void CONetBase::set_is_multipacket_end()
{
	if(_isTestFrame)
	{
		return ;
	}
	if ((_packData.data) && get_is_multi_packet())
	{
		*(_packData.data + 1) |= MUTIL_PACKET_END_MARK;
		_isMutilEnd = __true;		
	}
	else
	{
		_isMutilEnd = __false;	
	}
}

//得到协议数据长度(含信息码以后)的十进制长度(不含校验位)

__u16 CONetBase::get_buffer_length()
{
	if(_isTestFrame)
	{
		return 0;
	}
	return (_packData.length_high & LENGTH_HIGH_MASK) * 0x100 +_packData.length_low;
}
__u32 CONetBase::get_packet_length()
{
	return _packetLen;
}

//void CONetBase::clear_packet_buff()
//{
//	if ((_packetbuff))
//	{
//		// Zander 2013-10-10
//		//delete [] _packetbuff;
//		ACE_OS::free(_packetbuff);
//		_packetbuff = NULL;
//	}
//		_oldFrameNum =0;//WP
//}
void CONetBase::clear_data()
{
	if ((_packData.data))
	{
		delete [] _packData.data;
		_packData.data = NULL;
	}
}
//取得数据缓冲区，多帧下的也包括帧号以及时间戳
__u8 * CONetBase::get_data()
{
	return _packData.data;
}
//上层分配数据内存
void  CONetBase::set_data(__u8 * buffer_,__u16 datalen)
{
	//测试链接帧没有信息码和描述码、数据码
	if (_isTestFrame)
	{	
		_packData.length_low  = 0;
		_packData.length_high =(PROTOCOL_VERSION << 4);

	}
	else
	{
		if ((!(buffer_))||datalen == 0)
		{
			_packData.length_low = LOW_8_BIT(datalen + 2);
			_packData.length_high = HIGH_8_BIT(datalen + 2) | (PROTOCOL_VERSION << 4);
			_packData.data = NULL;
			return;
		}
		if (!_isMutilPacket)
		{
			_packData.length_low = LOW_8_BIT(datalen + 2);
			_packData.length_high = HIGH_8_BIT(datalen + 2) | (PROTOCOL_VERSION << 4);
			clear_data();
			_packData.data = new __u8 [datalen];
			ACE_OS::memcpy(_packData.data,buffer_,datalen);
		}
		else
		{
			_packData.length_low = LOW_8_BIT(datalen + 4);
			_packData.length_high = HIGH_8_BIT(datalen + 4) | (PROTOCOL_VERSION << 4);
			clear_data();
			_packData.data = new __u8 [datalen + 2];
			//多帧要留两个字节给帧号
			ACE_OS::memcpy(_packData.data + 2,buffer_,datalen);
		}
	}
}
//得到十进制的帧号
__u16 CONetBase::get_frame_num()
{
	if(_isTestFrame)
	{
		return 0;
	}
	if ((_packData.data) && _isMutilPacket)
	{
		return (*(_packData.data + 1) & LENGTH_HIGH_MASK) *0x100 + (*(__u8*)_packData.data);
	}
	return 0;
}
//设置十进制的帧号,要在setdata之后才能执行有效
void CONetBase::set_frame_num(__u16 frame_num_)
{
	if(_isTestFrame)
	{
		return ;
	}
	//设置多帧标识
	set_is_multi_packet();

	if ((_packData.data) && _isMutilPacket)
	{
		*(_packData.data) = LOW_8_BIT(frame_num_);
		*(_packData.data + 1) = HIGH_8_BIT(frame_num_) & LENGTH_HIGH_MASK;

	}
}
//得到低位的帧号十六进制码
__u8 CONetBase::get_frame_num_lowcode()
{
	if(_isTestFrame)
	{
		return 0;
	}
	if ((_packData.data) && _isMutilPacket)
	{
		return *_packData.data ;

	}
	else
	{
		return 0;
	}
}
//得到高位的帧号十六进制码
__u8 CONetBase::get_frame_num_highcode()
{
	if(_isTestFrame)
	{
		return 0;
	}
	if ((_packData.data) && _isMutilPacket)
	{
		return * (_packData.data +1) ;

	}
	return 0;
}
__u16 CONetBase::get_crc16()
{
	return _packData.crc16;
}
void CONetBase::set_crc16(__u16 crc16_)
{
	_packData.crc16 = crc16_;
}

void CONetBase::debug_procotol_data()
{
	ACE_DEBUG((LM_DEBUG, "Packet content :\n"));
	ACE_DEBUG((LM_DEBUG, "header: 0x%x\n",_packData.sync_header));
	ACE_DEBUG((LM_DEBUG, "dest_addr: 0x%x\n",_packData.dest_addr));
	ACE_DEBUG((LM_DEBUG, "source_addr: 0x%x\n",_packData.source_addr));
	ACE_DEBUG((LM_DEBUG, "length_low: 0x%x\n",_packData.length_low));
	ACE_DEBUG((LM_DEBUG, "length_high: 0x%x\n",_packData.length_high));
	ACE_DEBUG((LM_DEBUG, "control_area_1: 0x%x\n",_packData.control_area_1));
	ACE_DEBUG((LM_DEBUG, "control_area_2: 0x%x\n",_packData.control_area_2));
	ACE_DEBUG((LM_DEBUG, "control_area_3: 0x%x\n",_packData.control_area_3));
	ACE_DEBUG((LM_DEBUG, "procotol_type: 0x%x\n",_packData.protocol_type));
	ACE_DEBUG((LM_DEBUG, "info_code: 0x%x\n",_packData.info_code));
	ACE_DEBUG((LM_DEBUG, "description_code: 0x%x\n",_packData.description_code));
	__u8 * p = get_data();
	if ( p != NULL)
	{
		if (_isMutilPacket)
		{
			ACE_DEBUG((LM_DEBUG, "frame num:%d ,frame end is %d\n", get_frame_num(),_isMutilEnd));

			for (__u16 i = 0; i < get_data_length() ; i++) 
			{
				ACE_DEBUG((LM_DEBUG, "0x%x\n ", p[i]));
			}
		}
		else
		{
			for (__u16 i = 0; i < get_data_length() ; i++) 
			{
				ACE_DEBUG((LM_DEBUG, "0x%x\n ", p[i]));
			}
		}

	}
	ACE_DEBUG((LM_DEBUG, "Packet data length %d\n",get_data_length()));
	ACE_DEBUG((LM_DEBUG, "Packet  multipack is: %d\n",get_is_multi_packet()));
	ACE_DEBUG((LM_DEBUG, "\n"));
}

__bool CONetBase::get_is_test_frame()
{
	return _isTestFrame;
}
void CONetBase::set_is_test_frame(__bool islink_)
{
	_isTestFrame = islink_;
}
//如果是下行，则生成发送包
__i32 CONetBase::create_packet()
{
	//上行协议调用此接口无效
	if(_isDown == __false)
	{
		return RET_Fail;
	}
	//测试/连接帧只有帧头
	if (_isTestFrame)
	{
		//只有到（不包含）info_code截止
		_packetLen = RTK_FIELD_OFFSET(FD_PACKET_ONet,info_code) ;
		ACE_OS::memcpy(_packetbuff, &_packData, _packetLen);
	}
	//一般数据帧
	else
	{
		__u32 headerLen_ = RTK_FIELD_OFFSET(FD_PACKET_ONet,data);
		__u32 dataLen_ = get_data_length();
		_packetLen = headerLen_ + dataLen_;
		ACE_OS::memcpy(_packetbuff, &_packData, headerLen_);
		ACE_OS::memcpy(_packetbuff + headerLen_, get_data(), dataLen_);
	}
	return RET_OK;
}
//形成帧，下行协议才用
//按照不同初始化参数设置进行形成帧
__i32 CONetBase::create_frame()
{
	return RET_OK;

}
//解析帧，上行协议才用
//从socket接收到的数据，按照不同协议进行解析
__i32 CONetBase::parse_packet(__u8 *recvBuff_,__u32 recvLen,__u8 curNum_) 
{
	return RET_OK;
}


/**
* @name 构造
* @{
*/
CONetPctlDepository::CONetPctlDepository()
{
}/**< @brief 构造 */
CONetPctlDepository::~CONetPctlDepository()
{

}/**< @brief 析构 */
/** @} */


//初始化已经存在的协议
void CONetPctlDepository::init_pctls(ONet_DB *db)
{
	CONetPctl_10H_11H_12H *pnet10 = new CONetPctl_10H_11H_12H(0x10);
	add_pctl(0x10,pnet10);
	CONetPctl_10H_11H_12H *pnet11 = new CONetPctl_10H_11H_12H(0x11);
	add_pctl(0x11,pnet11);
	CONetPctl_10H_11H_12H *pnet12 = new CONetPctl_10H_11H_12H(0x12);
	add_pctl(0x12,pnet12);
	CONetPctl_13H_14H_16H_18H *pnet13 = new CONetPctl_13H_14H_16H_18H(0x13);
	add_pctl(0x13,pnet13);
	CONetPctl_13H_14H_16H_18H *pnet14 = new CONetPctl_13H_14H_16H_18H(0x14);
	add_pctl(0x14,pnet14);
	CONetPctl_13H_14H_16H_18H *pnet16 = new CONetPctl_13H_14H_16H_18H(0x16);
	add_pctl(0x16,pnet16);
	CONetPctl_13H_14H_16H_18H *pnet18 = new CONetPctl_13H_14H_16H_18H(0x18);
	add_pctl(0x18,pnet18);
	CONetPctl_17H_19H *pnet17 = new CONetPctl_17H_19H(0x17);
	add_pctl(0x17,pnet17);
	CONetPctl_17H_19H *pnet19 = new CONetPctl_17H_19H(0x19);
	add_pctl(0x19,pnet19);
	CONetPctl_33H_34H_35H_36H_3A *pnet33 = new CONetPctl_33H_34H_35H_36H_3A(0x33);
	add_pctl(0x33,pnet33);
	CONetPctl_33H_34H_35H_36H_3A *pnet34 = new CONetPctl_33H_34H_35H_36H_3A(0x34);
	add_pctl(0x34,pnet34);
	CONetPctl_33H_34H_35H_36H_3A *pnet35 = new CONetPctl_33H_34H_35H_36H_3A(0x35);
	add_pctl(0x35,pnet35);
	CONetPctl_33H_34H_35H_36H_3A *pnet36 = new CONetPctl_33H_34H_35H_36H_3A(0x36);
	add_pctl(0x36,pnet36);
	CONetPctl_33H_34H_35H_36H_3A *pnet3A = new CONetPctl_33H_34H_35H_36H_3A(0x3A);
	add_pctl(0x3A,pnet3A);
	CONetPctl_Link *pnetlink = new CONetPctl_Link(0xcd);//连接帧认为的取一个协议编号
	add_pctl(0xcd,pnetlink);
	CONetPctl_78H *pnet78 = new CONetPctl_78H(0x78);
	add_pctl(0x78,pnet78);

	//上行协议会改变数据库，所以有db指针
	CONetPctl_20H *pnet20 = new CONetPctl_20H(db);
	add_pctl(0x20,pnet20);
	CONetPctl_15H *pnet15 = new CONetPctl_15H(db);
	add_pctl(0x15,pnet15);
}
CONetBase *CONetPctlDepository::get_pctl(__u8 pctlnum_)
{
	PCTL_LIST::iterator iter_ = _pctls.find(pctlnum_);
	if (iter_ != _pctls.end())
	{
		return iter_->second;
	}
	else
	{
		return NULL;
	}

}
//增加的协议参数要在具体的使用中填充内容
__i32 CONetPctlDepository::add_pctl(__u8 pctlnum_,CONetBase *  pctl_)
{
	_pctls.insert(PCTL_LIST::value_type(pctlnum_,pctl_));
	return RET_OK;
}
__i32 CONetPctlDepository::del_pctl(__u8 pctlnum_)
{
	PCTL_LIST::iterator iter_ = _pctls.find(pctlnum_);
	if (iter_ != _pctls.end())
	{
		delete iter_->second;
		_pctls.erase(iter_);
	}
	return RET_OK;
}
__i32 CONetPctlDepository::clear_all_pctl()
{
	PCTL_LIST::iterator iter_ = _pctls.begin();
	while (iter_ != _pctls.end())
	{
		delete iter_->second;
		_pctls.erase(iter_++);
	}
	return RET_OK;
}
//输出协议仓库支持的协议类型
void CONetPctlDepository::dubeg_pctls()
{
	PCTL_LIST::iterator iter_ = _pctls.begin();
	while (iter_ != _pctls.end())
	{
		ACE_DEBUG((LM_DEBUG, "ONetPctlDepository insist:0x%x\n",iter_->first));
		iter_ ++;
	}
}