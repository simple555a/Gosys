//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update (function names modified)                            
//= (20-1)         1.0.0.20      Zander Sun      2013-10-10       parse_packet():�����˶�ÿһ֡��CRC��ıȶ��жϣ��Լ��������ݱ���CRCУ��������жϡ�
//= (21-1)         1.0.0.21      Zander Sun      2013-11-28       do_packet_to_onetdb():����io�߼���ַ�ļ�¼�����ж�isExistΪ1��IO���Ƿ�û�б������������ʶ��IO��������
//= (22-1)         v1.0.0.22     Zander Sun      2014-10-10       do_idvbuff_to_dpustatus
//= (23-1)         v1.0.0.23     Zander Sun      2014-10-13       char* to _u8*
//=                                                               parse_packet():delete realloc _packetBuf, use _recvPackBuf to store receiving packet

#include "precomp.h"
#include "ONetPctlRecv.h"
#include "CRCheck.h"

////VV
//FILE* g_fp = NULL;
//char g_buf[1024];


CONetPctl_20H:: CONetPctl_20H(ONet_DB *db):CONetBase( __false,__false,__false,db)
{
	_oldFrameNum =0;//WP
	
	//g_fp = fopen("D:\\nicsys\\target\\bin\\config\\packet.csv", "wt");

}/**< @brief ���� */
CONetPctl_20H:: ~CONetPctl_20H()
{
	//clear_packet_buff();
	_oldFrameNum =0;
	_packetLen = 0;
	//fclose(g_fp);
}/**< @brief ���� */


//��ͨ����ֻ֡�����info_code(����)��ʼ����Ϣ
__i32 CONetPctl_20H::parse_packet(__u8 *recvBuff_,__u32 recvLen_,__u8 curNetNum_)  
{
	
	// 
	//printf("go to parse_packet!!!\n");
	//�ж��������Ǳ�������������,0��IPС�Ŀ�����(ԭ��������)
	__u8 isMasterCPU_ = 0;
	//���������������ӳ��
	if (curNetNum_ == 0 || curNetNum_ == 2)
	{
		isMasterCPU_ = 0;		
	}
	else if(curNetNum_ == 1 || curNetNum_ == 3)
	{
		isMasterCPU_ = 1;	
	}
	else
	{
		ACE_ERROR((LM_ERROR, "(%P|%t) Net num :%d is not exist\n",curNetNum_));
		return ERROR_NODE_OVERLIMIT;
	}

	//__u16 crc = *((__u16*)(recvBuff_+(recvLen_ - 2))); // ȡ��CRCУ���� // Zander 2013-10-10 
	
	if(*recvBuff_ & MULTI_PACKET)
	{
		__u8 curFramNum_ = *(recvBuff_ + 2); // ȡ��֡���
		

		// Zander 2014-10-13  delete
		////��һ֡һ��Ҫ��ջ��壬��Ϊ�����Ƕ������������л���Ļ�����������
		//if(curFramNum_== 0x01)
		//{
		//	clear_packet_buff();
		//	_packetLen = 0;
		//	//_oldCrc = crc;// Zander 2013-10-10 
		//}

		_oldFrameNum ++;

		//�ж�֡�Ƿ�����
		if(_oldFrameNum != curFramNum_)
		{
			_oldFrameNum =0;//WP
			//clear_packet_buff();  // Zander 2014-10-13 delete
			_packetLen = 0;
			//_oldCrc = 0;// Zander 2013-10-10 
			//ACE_ERROR((LM_ERROR, "(%P|%t) current Frame num :%d ,but Frame is not sequence \n",curFramNum_));
			return RET_Fail;
		}

		//�����Ƕ�֡����һ֡���ݣ���Ҫ�ȿ�������������,�����һ֡���ݣ���ʵ_packetLen ��Ϊ0
		__u32 tempLen = _packetLen + (recvLen_- 4);  // ��ȥ֡β��2�ֽڵ�CRC��, Zander 2013-10-10
		//_packetbuff = (char*)ACE_OS::realloc(_packetbuff,tempLen);

		//
		//utils_debug("_packetLen = %d, templen = %d\n", _packetLen, tempLen);

		////����������Ϊ�յ�ʱ��
		//if (!(_packetbuff))
		//{
		//	_oldFrameNum = 0;
		//	clear_packet_buff();
		//	_packetLen = 0;
		//	//_oldCrc = 0;// Zander 2013-10-10 
		//	return RET_Fail;
		//}

		// Zander 2013-10-10 Start
		// �ж�ÿһ֡ĩβ��CRCУ�����Ƿ�һ��	
		/*if (crc != _oldCrc)
		{
			_oldFrameNum = 0;
			clear_packet_buff();
			_packetLen = 0;
			_oldCrc = 0;
			return RET_Fail;
		}*/
		// Zander 2013-10-10 End

        // Zander 2013-10-10 Start
		//֡�Ų����뿽������

		// Zander 2014-10-13
		//ACE_OS::memcpy(_packetbuff + _packetLen,recvBuff_ + 4,recvLen_- 4);  // ĩβ��16λCRCУ���벻���뿽������		
		ACE_OS::memcpy(_recvPacktBuf + _packetLen, recvBuff_ + 4, recvLen_ - 4);
		
		//Ϊ��ƴ�����ݣ�Ҫ�����󻺳�������
		_packetLen += recvLen_- 4; 
        // Zander 2013-10-10 End

		//��֡����£�����������ĩ֡���ſ�ʼ���д���
		if (*(recvBuff_ +3) & 0x80)
		{
			// Zander 2013-10-10 Start
			// �����������ݱ���CRCУ����
		/*	crc = CalSFCRC((__u8*)_packetbuff, _packetLen);
			if (crc != _oldCrc)
			{
				_oldFrameNum = 0;
				clear_packet_buff();
				_packetLen = 0;
				_oldCrc = 0;
				return RET_Fail;
			}*/
			// Zander 2013-10-10 End
			
			//��ʼ����֡
			do_packet_to_onetdb(_recvPacktBuf, _packetLen, (*(recvBuff_ + 1) & H21_TIME_MARK), isMasterCPU_);
			
			// �������Ҫ��ջ�����
			// clear_packet_buff();
			_oldFrameNum = 0;// Zander 2013-10-10hj
			_packetLen = 0;
			//_oldCrc = 0;// Zander 2013-10-10
		}
	}
	else 
	{
		
		// �����Ƕ�֡����һ֡���ݣ���Ҫ�ȿ�������������,�����һ֡���ݣ���ʵ_packetLen ��Ϊ0
		__u32 tempLen = _packetLen + (recvLen_- 2);  // // Zander 2013-10-10
		//_packetbuff = (char*)ACE_OS::realloc(_packetbuff,tempLen);

		//����������Ϊ�յ�ʱ��
		//if (!(_packetbuff))
		//{
		//	clear_packet_buff(); // Zander 2013-10-10
		//	_packetLen = 0;
		//	return RET_Fail;
		//}

		// Zander 2013-10-10 Start
		//��֡���ݲ���֡�ţ�����ֱ��ȥ����Ϣ�� + �����봦��
		ACE_OS::memcpy(_recvPacktBuf + _packetLen,recvBuff_ + 2,recvLen_- 2);
		_packetLen += recvLen_- 2;
		// Zander 2013-10-10 End

		// Zander 2013-10-10 Start
		//// CRCУ������ж�
		//_oldCrc = crc;
		//crc = CalSFCRC((__u8*)_packetbuff, _packetLen);
		//if (_oldCrc != crc)
		//{
		//	clear_packet_buff();
		//	_packetLen = 0;
		//	_oldCrc = 0;
		//	return RET_Fail;
		//}
		// Zander 2013-10-10 End

		//��ʼ����֡
		do_packet_to_onetdb(_recvPacktBuf, _packetLen, (*(recvBuff_ + 1) & H21_TIME_MARK), isMasterCPU_);

		//�������Ҫ��ջ�����
		//clear_packet_buff();
		// _oldFrameNum = 0;
		_packetLen = 0;
		_oldCrc = 0; // Zander 2013-10-10
	}
	return RET_OK;

}
__i32   CONetPctl_20H::do_packet_to_onetdb(__u8 * buff_, __u32 len_,__bool isHaveTime_,__u8 cpuNum_)
{	
	__u8 *ptr_ = buff_;
	__i32 nRet = RET_OK;

	if (isHaveTime_)
	{
		ACE_OS::memcpy(&time_mark_,buff_,H21_TIME_MARK_LENGTH);
		ptr_ += H21_TIME_MARK_LENGTH;	
	}
	//�˴δ���Ŀ���������������һ��������Ϊ�������������
	if (*ptr_ != 1 )
	{
		ACE_ERROR((LM_ERROR, "(%P|%t) DPU num is not 1\n"));
		return ERROR_NODE_OVERLIMIT;
	}
	ptr_++;	//ָ��ָ��̬���ԵĿ�ʼ
	//�������ĵľ�̬����
	//���ƿ������汾dpuVersion֮ǰ������
	ACE_OS::memcpy(&_ONetdb->_staticAttr[cpuNum_],ptr_,RTK_FIELD_OFFSET(DPU_ATTR,dpuVersion));

	//�˴�Ҫ����һ�¿�������״̬����Ϊ����������״̬Ҳ��ͨ������������ȡ�õ�
	//��λΪIP��ַС���Ǹ�������״̬
	__u16 dpustatus = _ONetdb->_staticAttr[cpuNum_].dpuStatus;

	//2015 GM 
	__u8 mDpustatus = __u8(_ONetdb->_staticAttr[cpuNum_].dpuStatus);
	__u8 cDpustatus = __u8(_ONetdb->_staticAttr[cpuNum_].dpuStatus >> 0x8);

	//__u8 cDpustatus = __u8(_ONetdb->_staticAttr[cpuNum_].dpuStatus);
	//__u8 mDpustatus = __u8(_ONetdb->_staticAttr[cpuNum_].dpuStatus >> 0x8);

	_ONetdb->_staticAttr[0].dpuStatus = mDpustatus;
	_ONetdb->_staticAttr[1].dpuStatus = cDpustatus;

	//printf("dpuStatus0 = %x,  dpuStatus1 = %x\n", _ONetdb->_staticAttr[0].dpuStatus, _ONetdb->_staticAttr[1].dpuStatus);

	ptr_ += RTK_FIELD_OFFSET(DPU_ATTR,dpuVersion);	//ָ��ָ��dpuVersion��
	__u8 verDPULen= *ptr_;
	ACE_OS::memcpy(_ONetdb->_staticAttr[cpuNum_].dpuVersion,ptr_+1,verDPULen);
	ptr_ += verDPULen +1;	//ָ��ָ��plcVersion��
	__u8 verPLCLen= *ptr_;
	ACE_OS::memcpy(_ONetdb->_staticAttr[cpuNum_].plcVersion,ptr_+1,verPLCLen);
	ptr_ += verPLCLen +1;	//ָ��ָ��oNetIpAddr1��
	_ONetdb->_staticAttr[cpuNum_].oNetIpAddr1 = *(__u32*)ptr_;
	ptr_ += 4;
	_ONetdb->_staticAttr[cpuNum_].oNetIpAddr2 = *(__u32*)ptr_;
	ptr_ += 4;
	_ONetdb->_staticAttr[cpuNum_].oNetIpAddr3 = *(__u32*)ptr_;
	ptr_ += 4;
	_ONetdb->_staticAttr[cpuNum_].oNetIpAddr4 = *(__u32*)ptr_;
	ptr_ += 4;
	_ONetdb->_staticAttr[cpuNum_].syncIpAddr = *(__u32*)ptr_;
	ptr_ += 4; //ָ��ָ��DPU_VDI��
	//DPU_VDI����
	__u8 vdiLen_ = *ptr_;
	ptr_ ++;	//ָ��ָ��DPU_STATUS

	do_idvbuff_to_dpustatus(ptr_,cpuNum_);

	ptr_ += vdiLen_;	//ָ��ָ��IO��num
	__u8 ioCount_ = *ptr_;
	_ONetdb->_IOCardsCount = (__u16)ioCount_;


	ptr_++;	////ָ��ָ��IO_CARD_ATTR
	//��Ϊ���ж�IO���Ƿ���ڣ�Ĭ���ǲ����ڣ���������Ľ��������д��ڣ��ͻᱻ��Ϊtrue
	// 2013-11-28ȥ������һ����
	//for (__u16 i = 0 ;i < IOCARD_MAX_SIZE;i++)
	//{
	//	_ONetdb->ioCards[i].isExist = __false;
	//}
	for (__u8 i = 0; i < ioCount_; i++)
	{
		//ȡ��IO�߼���ַ
		__u8 ioCardAddr_ = *(ptr_ +1);
		__u8 ioType_ = IO_CARD;		//�ϲ�ģ�����
		//IOModeת��
		if (*(__u8 *)(ptr_+3) == VPTGROUP || *(__u8 *)(ptr_+3) == RETVPTGROUP)
			ioType_ = VPT_CARD;
		else if (*(ptr_+3) == IEDMODEL)
			ioType_ = IED_CARD;
		else
			ioType_ = IO_CARD;
		//IO�������Ը�ֵ,ֻȡ�������õ���Ϣ�洢
		_ONetdb->ioCards[ioCardAddr_].isExist = __true;
		_ONetdb->ioCards[ioCardAddr_].dpuAddr = _ONetdb->_staticAttr[cpuNum_].dpuAddr;
		_ONetdb->ioCards[ioCardAddr_].ioId =  *ptr_;
		_ONetdb->ioCards[ioCardAddr_].ioAddr = *(ptr_+1);
		_ONetdb->ioCards[ioCardAddr_].ioStatus = *(ptr_+2);
		_ONetdb->ioCards[ioCardAddr_].ioModType =  *(ptr_+3);;
		_ONetdb->ioCards[ioCardAddr_].ioCardType = *(__u16*)(ptr_+4);
		iolist.push_back(ioCardAddr_); // Zander 2013-11-28

		//����Ҫ������ָ�������λ������Ҫ��ָ������
		nRet = do_iobuffer_to_channelvalue(ptr_,ioCardAddr_,ioType_);
		
		
		

		//printf("iolist size = %d\n", iolist.size());
		//�����з��ش���ʱ�����ݰ����쳣��ֱ�ӷ���
		if (nRet == RET_Fail)
		{
			//printf("")
			printf("nRet = %d\n", nRet);
			iolist.clear();// Zander 2013-11-28
			return RET_Fail;
		}	
	}

	//2015 GM �鿴�����IOͨ��ֵ
	//printf("DI=%d\n",_ONetdb->ioCards[26].channels[0].rData.value.Value.b);
	//printf("DI=%d\n",_ONetdb->ioCards[22].channels[0].rData.value.Value.b);
	//printf("D0-1=%d\n",_ONetdb->ioCards[23].channels[0].rData.value.Value.b);
	//printf("D0-2=%d\n",_ONetdb->ioCards[23].channels[1].rData.value.Value.b);
	//printf("+++++++++++++++++++\n");

	// Zander 2013-11-28
	for (__u16 i=0; i!=MAX_IO_CARD; i++)
	{
		if (_ONetdb->ioCards[i].isExist && (find(iolist.begin(), iolist.end(), i) == iolist.end()))
		{
			_ONetdb->ioCards[i].isExist = __false;
		}

	}
	iolist.clear();

	return RET_OK;
}

//����IO��Ϣ
__i32   CONetPctl_20H::do_iobuffer_to_channelvalue(__u8 *& buff_,__u8 ioCardAddr_,__u8 ioType_)
{
	__u8 *&ptr_ = buff_;			//��ǰ����ָ��,��ʼָ��IO_VDI
	__u8 *qPtr_ = buff_;			//������ָ��
	__u8 *dPtr_ = buff_;			//������ָ��
	IO_DATA_ATTR *dAttrPtr =NULL;		//����������ָ��
	__u16 tempNum_ =0;			//IO�ϵ�ͨ�����
	__u8 qualityDataLen_ = 0;	//������������
	__u8 valueDataLen_ = 0;		//����������
	__u16 channelCount_ = 0;	//VPT����ӵ����ͨ���ĸ���
	__u8 qSize_ = 0;		//�������ĳ���
	__i32 ret = 0;
	//�˴�ָ���ƶ������˲�ͬ���Ϳ���������Ϣ
	switch ( ioType_)
	{
	case IO_CARD:
		ptr_ += sizeof(IO_CARD_ATTR);//ָ��ָ��IO_VDI
		//IO_VDI����
		do_idvbuff_to_iostatus(ptr_ + 1,ioCardAddr_,ioType_);
		//���ݵĿ������еĶ�������

		ptr_ += (*ptr_) +1;	//��ǰָ��ָ��IO��IO_VAL
		qPtr_ = ptr_ + 1;	//������ָ��
		qualityDataLen_ = (*ptr_);
		dPtr_ = ptr_ + 1 + qualityDataLen_ + 1;	//������ָ��
		valueDataLen_ = *(ptr_ + qualityDataLen_ + 1);
		dAttrPtr =(IO_DATA_ATTR *) (ptr_ + qualityDataLen_ + 1 +valueDataLen_  + 1 +2); //����������ָ��
		//�������ĵ��ֽ���ǰ�����ֽ��ں�
		channelCount_ =*(__u8 *)(ptr_ + qualityDataLen_ + 1 +valueDataLen_  + 1) * 0x100 +(*(__u8 *)(ptr_ + qualityDataLen_ + 1 +valueDataLen_  + 1 +1)) ;

		//��IOͨ���ĸ���������ʱ�����ش��󣬲���������ǰ���ݻ������������������
		if ( channelCount_ <= 0 ||  channelCount_ >= 64)
		{
			return RET_Fail;
		}

		//��ʱ����ͨ������
		if(_ONetdb->ioCards[ioCardAddr_].ioCurNum != channelCount_)
		{
			if ((_ONetdb->ioCards[ioCardAddr_].channels))
			{
				_ONetdb->_data_mutex.acquire_write();
				ACE_OS::free(_ONetdb->ioCards[ioCardAddr_].channels);
				_ONetdb->ioCards[ioCardAddr_].channels = (nicONetChannel *)ACE_OS::malloc(channelCount_ * sizeof(nicONetChannel));
				memset(_ONetdb->ioCards[ioCardAddr_].channels, 0, channelCount_*sizeof(nicONetChannel));//Zander 2012-7-24
				_ONetdb->_data_mutex.release();
			}
			else	//����ʼ����ʱ��_ONetdb->ioCards[ioAddr_].channels��NULL
			{
				_ONetdb->_data_mutex.acquire_write();
				_ONetdb->ioCards[ioCardAddr_].channels = (nicONetChannel *)ACE_OS::malloc(channelCount_ * sizeof(nicONetChannel));
				memset(_ONetdb->ioCards[ioCardAddr_].channels, 0, channelCount_*sizeof(nicONetChannel));//Zander 2012-7-24
				_ONetdb->_data_mutex.release();
			}
		}
		_ONetdb->ioCards[ioCardAddr_].ioCurNum = channelCount_;
		ACE_ASSERT(_ONetdb->ioCards[ioCardAddr_].channels != NULL);
		
		
	
		if (_ONetdb->ioCards[ioCardAddr_].ioModType  == AI_8_1 ||
			_ONetdb->ioCards[ioCardAddr_].ioModType  == AI_4_1 ||
			_ONetdb->ioCards[ioCardAddr_].ioModType  == AI_10_2 ||
			_ONetdb->ioCards[ioCardAddr_].ioModType  == LAI_16_1)
		{
			qSize_ =2;
		}
		else
		{
			qSize_ = 1;
		}

		ret = do_databuff_to_iovalue(ioCardAddr_,qPtr_,qualityDataLen_,dPtr_,valueDataLen_,dAttrPtr,qSize_);

		if ( (ret == RET_Fail) || (ret != channelCount_))
		{
			return RET_Fail;
		}
	
		//û���ô���ָ�����ã��ǵ�Ҫ�����ƶ�ָ��
		ptr_ += qualityDataLen_ +1 + valueDataLen_ + 1 + 2 + 4 * sizeof(IO_DATA_ATTR);//ָ��ָ����һ��IO��
		break;
	case IED_CARD:
		qSize_ =1;		//IED������Ϊ1�ֽ�
		ptr_ += sizeof(IED_CARD_ATTR);//ָ��ָ��IED_VDI
		//IED����VDI�ݲ�����
		ptr_ += (*ptr_) +1;	//��ǰָ��ָ��IO��IO_VAL
		qualityDataLen_ = (*ptr_) * 0x100 + *(ptr_+1);	//����������
		qPtr_ = ptr_ + 2;	//������ָ��
		valueDataLen_ = *(__u8 *)(ptr_ + qualityDataLen_ + 2) * 0x100 + *(__u8 *)(ptr_ + qualityDataLen_ + 2 + 1);	//����������
		dPtr_ = ptr_ + 2 + qualityDataLen_ + 2;	//������ָ��
		qSize_ = 1;
		ret = do_databuff_to_iedvalue(ioCardAddr_,qPtr_,qualityDataLen_,dPtr_,valueDataLen_,qSize_);
		//û���ô���ָ�����ã��ǵ�Ҫ�����ƶ�ָ��
		ptr_ += qualityDataLen_ +2 + valueDataLen_ + 2;//ָ��ָ����һ��IO��

		break;
	case VPT_CARD:
		channelCount_ = *(__u8*)(ptr_ + 4) * 0x100 + *(__u8*)(ptr_ + 5);	//�����������ھ�̬������
		ptr_ += sizeof(VPT_CARD_ATTR);//ָ��ָ��VPT_ATTR DETAILS	sizeof = 6

		//����ָ�����ã��ڲ�ָ����λ
		ret = do_databuff_to_vptvalue(ioCardAddr_,ptr_,channelCount_);
		if (ret == RET_Fail)
		{
			return RET_Fail;
		}
		break;
	}

	return RET_OK;
}

//����DPU��IDV��Ϣ,����Э���е�����ֻȡһ������Ϣ������Ҫ��ʱ����չ����,�κ�����£�ֻ���ӵ�ǰ�����е�DPU��������������һ����״̬
__i32  CONetPctl_20H::do_idvbuff_to_dpustatus(__u8 * buff_,__bool isDpuMaster_ )
{
	//ע��˴��Ľ����Ǹ��ݿ�����������Ϣ�������������ʽ����������������˵���ĵ�
	__u8 * ptr_ = buff_;
	_ONetdb->_status[0].temp = *(ptr_ + 2);
	_ONetdb->_status[0].cpu = *(ptr_ + 3);
	_ONetdb->_status[0].mem = *(ptr_ + 6);

	// Zander 2014-10-10
	_ONetdb->_status[0].dpBus1 = (*(ptr_+7)>> 4) & 0x01;//OLD: >> 5  
	_ONetdb->_status[0].dpBus2 = (*(ptr_+7)>> 5) & 0x01;//OLD: >> 6

	_ONetdb->_status[1].temp = *(ptr_ + 2);
	_ONetdb->_status[1].cpu = *(ptr_ + 3);
	_ONetdb->_status[1].mem = *(ptr_ + 6);

	// Zander 2014-10-10
	_ONetdb->_status[1].dpBus1 = (*(ptr_+7)>> 4) & 0x01;//OLD: >> 5
	_ONetdb->_status[1].dpBus2 = (*(ptr_+7)>> 5) & 0x01;//OLD: >> 6

	return RET_OK;
}
//����IO��IDV��Ϣ,����Э���е�����ֻȡһ������Ϣ������Ҫ��ʱ����չ����
__i32  CONetPctl_20H::do_idvbuff_to_iostatus(__u8 * buff_,__u8 ioCardAddr_,__u8 ioModType_ )
{
	__u8 * ptr_ = buff_;
	//_ONetdb->ioCards[ioCardAddr_].ioStatus = (*(ptr_+0))& 0x01;


	return RET_OK;
}
__i32 CONetPctl_20H::do_databuff_to_iedvalue(
	__u8 ioAddr_,		//IO���߼���ַ
	__u8 *qbuff_,		//������ָ��
	__u8 qLen_,			//����������
	__u8 *valbuff_,		//������ָ��
	__u8 vLen_,			//����������
	__u8 qSize_)		//������ռ���ֽ�
{



	return RET_OK;


};
__i32 CONetPctl_20H::do_databuff_to_vptvalue(
	__u8 ioAddr_,		//IO�߼���ַ
	__u8 * &valbuff_,		//VPT������ָ��
	__u16 vCount_ )		//VPT��VPT_VALUE�ĸ���
{
	__u8 *&ptr_ = valbuff_;	//ָ��ָ��VPT ATTR DETAILS
	__u16 valLen_ = 0;
	__u32 sumChaCount_ = 0;//VPT�е������ܸ���
	__u16 channelNum_ = 0;	//VPT�е�ͨ��������
	__u16 qData_ = 0;			//VPT���õ�������ֵ
	__u16 j = 0,i = 0;
	__u8 tempVal[4] = {0};
	__u8 * types = NULL;			//ÿ��VPT������
	__u16 * percounts = NULL;		//ÿ��VPT�����ݸ���
	__u8 * vptStart_ptr = NULL;		//VPT�е���������ʼָ�룬�ɸ������ָ�����������ݵ��ƫ����
	if (vCount_ == 0)
	{
		//������ʱ��ô����
		return RET_Fail;
	}
	else
	{
		types = new __u8[vCount_];		//����VPT��������
		percounts = new __u16[vCount_];	//����VPT�����ݸ�������
	}
	for ( i = 0; i < vCount_; i ++)
	{
		types[i] = *(__u8 *)(ptr_);
		percounts[i] =*(__u8 *)(ptr_+1) * 0x100 + *(__u8 *)(ptr_+2);
		ptr_ += (sizeof(__u8) +sizeof(__u16));	//ÿ��ָ������3���ֽ�
		sumChaCount_ += percounts[i];
	}
	//��VPT��ĸ���������ʱ�����ش��󣬲���������ǰ���ݻ������������������
	if (sumChaCount_ <= 0)
	{
		return RET_Fail;	
	}


	//��ʱ����ͨ������
	if(_ONetdb->ioCards[ioAddr_].ioCurNum != sumChaCount_)
	{
		if ((_ONetdb->ioCards[ioAddr_].channels))
		{
			_ONetdb->_data_mutex.acquire_write();
			ACE_OS::free(_ONetdb->ioCards[ioAddr_].channels);
			_ONetdb->ioCards[ioAddr_].channels = (nicONetChannel *)ACE_OS::malloc(sumChaCount_ * sizeof(nicONetChannel));
			memset(_ONetdb->ioCards[ioAddr_].channels, 0, sumChaCount_*sizeof(nicONetChannel));//Zander 2012-7-24
			_ONetdb->_data_mutex.release();
		}
		else	//����ʼ����ʱ��_ONetdb->ioCards[ioAddr_].channels��NULL
		{
			_ONetdb->_data_mutex.acquire_write();
			_ONetdb->ioCards[ioAddr_].channels = (nicONetChannel *)ACE_OS::malloc(sumChaCount_ * sizeof(nicONetChannel));
			memset(_ONetdb->ioCards[ioAddr_].channels, 0, sumChaCount_*sizeof(nicONetChannel));//Zander 2012-7-24
			_ONetdb->_data_mutex.release();
		}
	}
	_ONetdb->ioCards[ioAddr_].ioCurNum = sumChaCount_;
	ACE_ASSERT(_ONetdb->ioCards[ioAddr_].channels != NULL);
	qData_ = *(__u16 *) ptr_;			//������2�ֽ�
	ptr_ += 2; //ָ��ָ�����ݳ���
	valLen_ = *(__u8*)ptr_ * 0x100 + *(__u8*)(ptr_ + 1);
	ptr_ += 2; //ָ��ָ��������
	vptStart_ptr = ptr_;
	for ( i = 0; i < vCount_; i ++)
	{
		switch(types[i])
		{
		case BOOL_1_BIT:
			//BOOL�͵�һ���ֽ���8������Ϣ
			for (j = 0; j < percounts[i]; j++)
			{	
				if(j!= 0)
				{
					ptr_ += (j%8)?0:1;		//��8ָ���1
				}
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = BOOL_1_BIT;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_ - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = (__u8)(j%8) ;//ƫ����������0 ��ʼ�� 7
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = *(__u8 *)(&qData_); //�����������ʵʱֵ�ڣ����������������ʱ��nicDefs.h�Ķ��岻ͬ
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData..DataType = DBDataType_Bool;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_bool);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.b = ((*(__u8*)ptr_ )& (0x01 << (j%8)))?__true:__false;
				channelNum_++;
			}
			ptr_ ++;//ָ����һ�����͵�������
			break;
		case FLOAT_4_BYTE:
			for (j = 0; j < percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = FLOAT_4_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr =(__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =   *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Float;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_float);
				tempVal[0] = *(ptr_ + 3);
				tempVal[1] = *(ptr_ + 2);
				tempVal[2] = *(ptr_ + 1);
				tempVal[3] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.flt = *(__r4*)tempVal;
				channelNum_ ++;
				ptr_ += 4;
			}
			break;
		case FLOAT_3_BYTE:	
			for (j = 0; j <percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = FLOAT_3_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Float;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_float);
				//tempVal[0] = *(ptr_ + 3); Float��λ�Զ����Ϊ0
				tempVal[1] = *(ptr_ + 2);
				tempVal[2] = *(ptr_ + 1);
				tempVal[3] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.flt = *(__r4*)tempVal;
				channelNum_ ++;
				ptr_ += 3;
			}
			break;
		case FLOAT_2_BYTE:
			for (j = 0; j <percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = FLOAT_2_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr =(__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Float;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_float);
				//tempVal[0] = *(ptr_ + 3); ��λ�Զ����Ϊ0
				//tempVal[1] = *(ptr_ + 2); �Զ����Ϊ0
				tempVal[2] = *(ptr_ + 1);
				tempVal[3] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.flt = *(__r4*)tempVal;
				channelNum_ ++;
				ptr_ += 2;
			}
			break;
		case UNSIGNED_INT_4_BYTE:
			for (j = 0; j < percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_4_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_UInt32;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_uint32);
				tempVal[0] = *(ptr_ + 3);
				tempVal[1] = *(ptr_ + 2);
				tempVal[2] = *(ptr_ + 1);
				tempVal[3] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.u32 = *(__u32*)tempVal;
				channelNum_ ++;
				ptr_ += 4;
			}
			break;
		case UNSIGNED_INT_2_BYTE:
			for (j = 0; j < percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_2_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_UInt16;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_uint16);
				tempVal[0] = *(ptr_ + 1);
				tempVal[1] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.u16 = *(__u16*)tempVal;
				channelNum_ ++;
				ptr_ += 2;
			}
			break;
		case UNSIGNED_INT_1_BYTE:
			for (j = 0; j < percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_1_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_UInt8;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_uint8);
				tempVal[0] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.u8 = *(__u8*)tempVal;
				channelNum_ ++;
				ptr_ += 1;
			}
			break;
		case INT_4_BYTE:
			for (j = 0; j < percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = INT_4_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Int32;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_int32);
				tempVal[0] = *(ptr_ + 3);
				tempVal[1] = *(ptr_ + 2);
				tempVal[2] = *(ptr_ + 1);
				tempVal[3] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.i32 = *(__i32*)tempVal;
				channelNum_ ++;
				ptr_ += 4;
			}
			break;
		case INT_2_BYTE:
			for (j = 0; j < percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = INT_2_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Int16;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_int16);
				tempVal[0] = *(ptr_ + 1);
				tempVal[1] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.u16 = *(__u16*)tempVal;
				channelNum_ ++;
				ptr_ += 2;
			}
			break;
		case INT_1_BYTE:
			for (j = 0; j < percounts[i]; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = INT_1_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_  - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = 0;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//ͨ����״̬����������
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Int8;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_int8);
				tempVal[0] = *(ptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.i8 = *(__i8*)tempVal;
				channelNum_ ++;
				ptr_ += 1;//Zander 2013-5-27
			}
			break;
		default:
			ACE_DEBUG((LM_DEBUG,"VPT unclude datatype :0x%x\n ",types[i]));
		}
	}
	ACE_ASSERT(channelNum_ == sumChaCount_);
	delete[] types;
	delete[] percounts;

	return RET_OK;


};

__i32 CONetPctl_20H::do_databuff_to_iovalue(
	__u8 ioAddr_,		//IO���߼���ַ
	__u8 *qbuff_,		//������ָ��
	__u8 qLen_,			//����������
	__u8 *valbuff_,		//������ָ��
	__u8 vLen_,			//����������
	IO_DATA_ATTR *datattr_,	//ֵ��������
	__u8 qSize_)		//������ռ���ֽ�
{
	__u8 i = 0;
	__u8 j = 0;
	__u8 datatype_ = 0;
	__u8 channelNum_ =0;
	__u8 tempVal[4];
	__u8 *Qptr_ = qbuff_;	//�������ĵ�ǰָ��
	__u8 *Vptr_ = valbuff_;	//�������ĵ�ǰָ��
	__u8 *Tptr_ =(__u8*)datattr_;	//������ָ��
	__u16 chCount_ = 0;		//��,��IO_DATA_ATTRȡ��

	//����IO_DATA_ATTRΪ��������
	for (i = 0; i < 4; i++)
	{
		Tptr_ = (__u8*)(datattr_ +i);
		datatype_ = *Tptr_;
		chCount_ = *(__u8*)(Tptr_ + 2) *0x100 + *(__u8*)(Tptr_ + 3);

		switch(datatype_)
		{
		case BOOL_1_BIT:
			for (j = 0; j < chCount_; j++)
			{	
				if(j!= 0)
				{
					Vptr_ += (j%8)?0:1;		//��8ָ���1
				}
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = BOOL_1_BIT;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				//ͨ������������16�Ļ�ֻ���õ�8λ
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = (qSize_==2)? *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_bool);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Bool;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.b = ((*(__u8*)Vptr_) & (0x01 << (j%8)))?__true:__false;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				channelNum_ ++;

			}
			Vptr_ ++;//ָ����һ�����͵�������
			break;
		case FLOAT_4_BYTE:
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = FLOAT_4_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  (qSize_==2)? *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Float;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_float);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				tempVal[0] = *(Vptr_ + 3);
				tempVal[1] = *(Vptr_ + 2);
				tempVal[2] = *(Vptr_ + 1);
				tempVal[3] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.flt = *(__r4*)tempVal;
				channelNum_ ++;
				Vptr_ += 4;
			}
			break;
		case FLOAT_3_BYTE:	
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = FLOAT_3_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = (qSize_==2)?  *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Float;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_float);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				//tempVal[0] = *(Vptr_ + 3); Float��λ�Զ����Ϊ0
				tempVal[1] = *(Vptr_ + 2);
				tempVal[2] = *(Vptr_ + 1);
				tempVal[3] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.flt = *(__r4*)tempVal;
				channelNum_ ++;
				Vptr_ += 3;
			}
			break;
		case FLOAT_2_BYTE:
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = FLOAT_2_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = (qSize_==2)? *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Float;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_float);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				//tempVal[0] = *(Vptr_ + 3); ��λ�Զ����Ϊ0
				//tempVal[1] = *(Vptr_ + 2); �Զ����Ϊ0
				tempVal[2] = *(Vptr_ + 1);
				tempVal[3] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.flt = *(__r4*)tempVal;
				channelNum_ ++;
				Vptr_ += 2;
			}
			break;
		case UNSIGNED_INT_4_BYTE:
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_4_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = (qSize_==2)?  *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_UInt32;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_uint32);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				tempVal[0] = *(Vptr_ + 3);
				tempVal[1] = *(Vptr_ + 2);
				tempVal[2] = *(Vptr_ + 1);
				tempVal[3] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.u32 = *(__u32*)tempVal;
				channelNum_ ++;
				Vptr_ += 4;
			}
			break;
		case UNSIGNED_INT_2_BYTE:
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_2_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  (qSize_==2)?  *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_UInt16;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_uint16);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				tempVal[0] = *(Vptr_ + 1);
				tempVal[1] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.u16 = *(__u16*)tempVal;
				channelNum_ ++;
				Vptr_ += 2;
			}
			break;
		case UNSIGNED_INT_1_BYTE:
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_2_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  (qSize_==2)?  *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_UInt8;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_uint8);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				tempVal[0] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.u8 = *(__u8*)tempVal;
				channelNum_ ++;
				Vptr_ += 1;
			}
			break;
		case INT_4_BYTE:
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_4_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  (qSize_==2)?  *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Int32;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_int32);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				tempVal[0] = *(Vptr_ + 3);
				tempVal[1] = *(Vptr_ + 2);
				tempVal[2] = *(Vptr_ + 1);
				tempVal[3] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.i32 = *(__i32*)tempVal;
				channelNum_ ++;
				Vptr_ += 4;
			}
			break;
		case INT_2_BYTE:
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_4_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  (qSize_==2)?  *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Int16;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_int16);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				tempVal[0] = *(Vptr_ + 1);
				tempVal[1] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.u16 = *(__u16*)tempVal;
				channelNum_ ++;
				Vptr_ += 2;
			}
			break;
		case INT_1_BYTE:
			for (j = 0; j < chCount_; j++)
			{	
				ACE_OS::memset(tempVal,0,4);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = UNSIGNED_INT_2_BYTE;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  (qSize_==2)?  *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Int8;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_int8);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//ͨ����״̬����������
				tempVal[0] = *(Vptr_ + 0);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.i8 = *(__i8*)tempVal;
				channelNum_ ++;
				Vptr_ += 1; // Zander, 2013-8-28
			}
			break;
		default:
			break;
		}
	}
	return channelNum_;
}

CONetPctl_15H::CONetPctl_15H(ONet_DB *db):CONetBase( __false,__false,__false,db)
{

}
CONetPctl_15H::~CONetPctl_15H()
{

}

//��ͨ����ֻ֡�����info_code(����)��ʼ����Ϣ
__i32 CONetPctl_15H::parse_packet(
					  __u8 *recvBuff_,
					  __u32 recvLen_,
					  __u8 curNetNum_)
{
	__u8 * ptr_ = recvBuff_;
	ACE_DEBUG((LM_DEBUG, "DPU���ش�����Ϣ: 0x%x----",*(ptr_ + 1)));
	switch (*(ptr_ + 1)) {
	case ERROR_UNKNOW_SFDCS: ACE_DEBUG((LM_DEBUG, "��ͷ����0x68H\n")); break;
	case ERROR_NOBUILDLINK: ACE_DEBUG((LM_DEBUG, "û�н����������ӣ��ͷ����������������������\n")); break;
	case ERROR_SUBSC_INVALID: ACE_DEBUG((LM_DEBUG, "���붩����ʱ��ʧ��\n")); break;
	case ERROR_ALREADYHASTIMER: ACE_DEBUG((LM_DEBUG, "����\n")); break;
	case ERROR_TIMEROVERFLOW: ACE_DEBUG((LM_DEBUG, "����Ķ�ʱ��С��1\n")); break;
	case ERROR_HASNOTIMER: ACE_DEBUG((LM_DEBUG, "Ŀǰ����\n")); break;
	case ERROR_ALREADYLINK: ACE_DEBUG((LM_DEBUG, "�Ѿ����������ӣ���������������\n")); break;
	case ERROR_INVALIDPACKET: ACE_DEBUG((LM_DEBUG, "��ͷ����0x68H\n")); break;
	case ERROR_LENTOOSHORT: ACE_DEBUG((LM_DEBUG, "��ͷ�г���С�ڵ���0\n")); break;
	case ERROR_IODEAD: ACE_DEBUG((LM_DEBUG, "���ʵ�IO����\n")); break;
	case ERROR_IOADDR: ACE_DEBUG((LM_DEBUG, "IO��ַ��\n")); break;
	case ERROR_UNKNOWNDATATYPE: ACE_DEBUG((LM_DEBUG, "�������ݵ��������ͣ�DPUû�ж���\n")); break;
	case ERROR_MAKEFRAME: ACE_DEBUG((LM_DEBUG, "���������˴���\n")); break;
	case ERROR_NOVALIDIO: ACE_DEBUG((LM_DEBUG, "����������IO��ַ���Ҳ���\n")); break;
	case ERROR_INVALID_TARGET: ACE_DEBUG((LM_DEBUG, "DPU��ַ�����϶���\n")); break;
	case ERROR_INVALID_SRC: ACE_DEBUG((LM_DEBUG, "HMI�ڵ��ַ�����϶���\n")); break;
	case ERROR_REGISTER_DEV: ACE_DEBUG((LM_DEBUG, "ע��ͨ���豸��ַ����\n")); break;
	case ERROR_TARGET_NOTCORRECT: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_DPU_STATUS_NORUN: ACE_DEBUG((LM_DEBUG, "DPU״̬��������̬\n")); break;
	case ERROR_DEVNO_INVALID: ACE_DEBUG((LM_DEBUG, "�ڽڵ����豸�����Ч\n")); break;
	case ERROR_NODE_OVERLIMIT: ACE_DEBUG((LM_DEBUG, "����DPU��IO����Ŀ̫�࣬����������\n")); break;
	case ERROR_NO_PROTOCOL: ACE_DEBUG((LM_DEBUG, "Э��û�г�ʼ��\n")); break;
	case ERROR_NODE_HASNOADDR: ACE_DEBUG((LM_DEBUG, "�����г�������Ч��DPU��IO��ַ\n")); break;
	case ERROR_NODE_INVALID: ACE_DEBUG((LM_DEBUG, "������Ч�ĵ�ַ\n")); break;
	case SYS_MEM_ALLOC_ERR: ACE_DEBUG((LM_DEBUG, "ϵͳ�����ڴ����\n")); break;
	case SYS_QERR_QFULL: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_RIGHT_NOWRITE: ACE_DEBUG((LM_DEBUG, "û��дȨ��\n")); break;
	case ERROR_OPEN_FILENAME: ACE_DEBUG((LM_DEBUG, "���ļ���\n")); break;
	case ERROR_NOEVENT_ITEM: ACE_DEBUG((LM_DEBUG, "���¼����������գ�\n")); break;
	case ERROR_NOLOG_ITEM: ACE_DEBUG((LM_DEBUG, "��LOG���������գ�\n")); break;
	case ERROR_OVERLIMIT: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_NO_LBDATA: ACE_DEBUG((LM_DEBUG, "û��¼���ļ�\n")); break;
	case ERROR_SETTING_INVALID: ACE_DEBUG((LM_DEBUG, "��ֵ����Ч��ָ��ȡNVRAMʧ�ܡ���ֵ����Ч��\n")); break;
	case ERROR_SETTING_AREA: ACE_DEBUG((LM_DEBUG, "��ֵ���Ŵ�\n")); break;
	case ERROR_SETTING_NUM: ACE_DEBUG((LM_DEBUG, "��ֵ������\n")); break;
	case ERROR_SETTING_OVERFLOW: ACE_DEBUG((LM_DEBUG, "��ֵԽ��\n")); break;
	case ERROR_SETTING_CRC: ACE_DEBUG((LM_DEBUG, "��ֵcrcУ���\n")); break;
	case ERROR_SETTING_TYPE: ACE_DEBUG((LM_DEBUG, "��ֵ���ʹ�\n")); break;
	case ERROR_SETTING_STATE: ACE_DEBUG((LM_DEBUG, "����״̬��\n")); break;
	case ERROR_SETTING_WFILE: ACE_DEBUG((LM_DEBUG, "�������ļ�д��ֵ���Ŵ�\n")); break;
	}


	return RET_OK;
}
CONetPctl_40H:: CONetPctl_40H(ONet_DB *db):CONetBase( __false,__false,__false,db)
{

}
CONetPctl_40H::~CONetPctl_40H()
{

}

//��ͨ����ֻ֡�����info_code(����)��ʼ����Ϣ
__i32 CONetPctl_40H::parse_packet(
					  __u8 *recvBuff_,
					  __u32 recvLen_,
					  __u8 curNetNum_)
{
	return RET_OK;

}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                