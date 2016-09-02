#include "precomp.h"
#include "ONetPctl.h"
#include "ONetPctlRecv.h"
#include "ONetPctlSend.h"


//�������캯�������Է�ֹ���౻ʵ����
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
	_packData.length_low = 0x02;//����Ĭ��Ϊ2�ֽ� =��Ϣ��+��Ϣ����
	_packData.length_high = INVALID_ONET_CODE | (PROTOCOL_VERSION << 4);
	_packData.info_code = INVALID_ONET_CODE;
	_packData.description_code = INVALID_ONET_CODE;
	_packData.data = NULL;
	_packData.crc16= CRCCheck_? 0x0000:NO_CRC_CHECK ;

	_oldCrc = 0;

};	/**< @brief ���� */


CONetBase::~CONetBase()
{
	if ((_packData.data))
	{
		delete _packData.data;
		_packData.data = NULL;
	}
};/**< @brief ���� */

//�õ����ݰ�ͷ
__u8 CONetBase::get_pctl_header()
{
	return _packData.sync_header;
}
//�������ݰ�ͷ
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

//���������ȵ�8λ��
__u8 CONetBase::get_length_lowcode()
{
	return _packData.length_low;
}
void CONetBase::set_length_lowcode(__u8 length_low_)
{
	_packData.length_low = length_low_;
}

//���������ȸ�8λ��(���ȸ�λ��4λ+4λ�汾��)
__u8 CONetBase::get_length_highcode()
{
	return _packData.length_high;
}
void CONetBase::set_length_highcode(__u8 length_high_)
{
	_packData.length_high = length_high_;
}

//�õ�Э�����ͣ���Э����Ϣ��
__u8 CONetBase::get_info_code()
{
	if(_isTestFrame)
	{
		return 0;
	}
	return _packData.info_code & ~MULTI_PACKET;
};
//����Э�����ͣ���Э����Ϣ��
void CONetBase::set_info_code(__u8 info_code_)
{
	if(_isTestFrame)
	{
		return;
	}
	//һ�ֽ����������õĶ����뱣��ԭ����һ������
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
//�õ�������ʮ���Ƶĳ���(����У��λ)
__u16 CONetBase::get_data_length()
{
	if(_isTestFrame)
	{
		return 0;
	}
	//����ǵ�֡���ݣ���Ϣ��+������+data
	if (_isMutilPacket == __false)
	{
		return get_buffer_length() - 2;
	}
	else 
	{
		//����Ƕ�֡���ݣ���Ϣ��+������+֡�ŵ�+֡�Ÿ�+data
		return get_buffer_length() - 4;
	}
}
//�ж��Ƿ�Ϊ��֡
__bool CONetBase::get_is_multi_packet()
{
	if(_isTestFrame)
	{
		return __false;
	}
	_isMutilPacket = (_packData.info_code & MULTI_PACKET) ? __true :__false;
	return _isMutilPacket;
}
//����Ϊ��֡
void CONetBase::set_is_multi_packet()
{
	if(_isTestFrame)
	{
		return;
	}
	_packData.info_code |= MULTI_PACKET;
	_isMutilPacket = __true;
}
//�ж��Ƿ�Ϊ��֡��ĩ֡
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
//����Ϊ��֡��β�� Ҫ��setdata֮�����ִ����Ч
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

//�õ�Э�����ݳ���(����Ϣ���Ժ�)��ʮ���Ƴ���(����У��λ)

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
//ȡ�����ݻ���������֡�µ�Ҳ����֡���Լ�ʱ���
__u8 * CONetBase::get_data()
{
	return _packData.data;
}
//�ϲ���������ڴ�
void  CONetBase::set_data(__u8 * buffer_,__u16 datalen)
{
	//��������֡û����Ϣ��������롢������
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
			//��֡Ҫ�������ֽڸ�֡��
			ACE_OS::memcpy(_packData.data + 2,buffer_,datalen);
		}
	}
}
//�õ�ʮ���Ƶ�֡��
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
//����ʮ���Ƶ�֡��,Ҫ��setdata֮�����ִ����Ч
void CONetBase::set_frame_num(__u16 frame_num_)
{
	if(_isTestFrame)
	{
		return ;
	}
	//���ö�֡��ʶ
	set_is_multi_packet();

	if ((_packData.data) && _isMutilPacket)
	{
		*(_packData.data) = LOW_8_BIT(frame_num_);
		*(_packData.data + 1) = HIGH_8_BIT(frame_num_) & LENGTH_HIGH_MASK;

	}
}
//�õ���λ��֡��ʮ��������
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
//�õ���λ��֡��ʮ��������
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
//��������У������ɷ��Ͱ�
__i32 CONetBase::create_packet()
{
	//����Э����ô˽ӿ���Ч
	if(_isDown == __false)
	{
		return RET_Fail;
	}
	//����/����ֻ֡��֡ͷ
	if (_isTestFrame)
	{
		//ֻ�е�����������info_code��ֹ
		_packetLen = RTK_FIELD_OFFSET(FD_PACKET_ONet,info_code) ;
		ACE_OS::memcpy(_packetbuff, &_packData, _packetLen);
	}
	//һ������֡
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
//�γ�֡������Э�����
//���ղ�ͬ��ʼ���������ý����γ�֡
__i32 CONetBase::create_frame()
{
	return RET_OK;

}
//����֡������Э�����
//��socket���յ������ݣ����ղ�ͬЭ����н���
__i32 CONetBase::parse_packet(__u8 *recvBuff_,__u32 recvLen,__u8 curNum_) 
{
	return RET_OK;
}


/**
* @name ����
* @{
*/
CONetPctlDepository::CONetPctlDepository()
{
}/**< @brief ���� */
CONetPctlDepository::~CONetPctlDepository()
{

}/**< @brief ���� */
/** @} */


//��ʼ���Ѿ����ڵ�Э��
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
	CONetPctl_Link *pnetlink = new CONetPctl_Link(0xcd);//����֡��Ϊ��ȡһ��Э����
	add_pctl(0xcd,pnetlink);
	CONetPctl_78H *pnet78 = new CONetPctl_78H(0x78);
	add_pctl(0x78,pnet78);

	//����Э���ı����ݿ⣬������dbָ��
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
//���ӵ�Э�����Ҫ�ھ����ʹ�����������
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
//���Э��ֿ�֧�ֵ�Э������
void CONetPctlDepository::dubeg_pctls()
{
	PCTL_LIST::iterator iter_ = _pctls.begin();
	while (iter_ != _pctls.end())
	{
		ACE_DEBUG((LM_DEBUG, "ONetPctlDepository insist:0x%x\n",iter_->first));
		iter_ ++;
	}
}