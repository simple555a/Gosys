//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update (function names modified)                            
//= (20-1)         1.0.0.20      Zander Sun      2013-10-10       parse_packet():增加了对每一帧的CRC码的比对判断，以及整个数据报的CRC校验计算与判断。
//= (21-1)         1.0.0.21      Zander Sun      2013-11-28       do_packet_to_onetdb():增加io逻辑地址的记录，并判断isExist为1的IO卡是否没有被解析，是则标识该IO卡不存在
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

}/**< @brief 构造 */
CONetPctl_20H:: ~CONetPctl_20H()
{
	//clear_packet_buff();
	_oldFrameNum =0;
	_packetLen = 0;
	//fclose(g_fp);
}/**< @brief 构造 */


//普通数据帧只处理从info_code(包含)开始的信息
__i32 CONetPctl_20H::parse_packet(__u8 *recvBuff_,__u32 recvLen_,__u8 curNetNum_)  
{
	
	// 
	//printf("go to parse_packet!!!\n");
	//判断是主还是备控制器的数据,0是IP小的控制器(原主控制器)
	__u8 isMasterCPU_ = 0;
	//网络编号与控制器的映射
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

	//__u16 crc = *((__u16*)(recvBuff_+(recvLen_ - 2))); // 取出CRC校验码 // Zander 2013-10-10 
	
	if(*recvBuff_ & MULTI_PACKET)
	{
		__u8 curFramNum_ = *(recvBuff_ + 2); // 取出帧序号
		

		// Zander 2014-10-13  delete
		////第一帧一定要清空缓冲，因为可能是断线重连或者切换后的缓冲区不完整
		//if(curFramNum_== 0x01)
		//{
		//	clear_packet_buff();
		//	_packetLen = 0;
		//	//_oldCrc = crc;// Zander 2013-10-10 
		//}

		_oldFrameNum ++;

		//判断帧是否连续
		if(_oldFrameNum != curFramNum_)
		{
			_oldFrameNum =0;//WP
			//clear_packet_buff();  // Zander 2014-10-13 delete
			_packetLen = 0;
			//_oldCrc = 0;// Zander 2013-10-10 
			//ACE_ERROR((LM_ERROR, "(%P|%t) current Frame num :%d ,but Frame is not sequence \n",curFramNum_));
			return RET_Fail;
		}

		//不管是多帧还是一帧数据，都要先拷贝到缓冲区中,如果是一帧数据，其实_packetLen 常为0
		__u32 tempLen = _packetLen + (recvLen_- 4);  // 除去帧尾的2字节的CRC码, Zander 2013-10-10
		//_packetbuff = (char*)ACE_OS::realloc(_packetbuff,tempLen);

		//
		//utils_debug("_packetLen = %d, templen = %d\n", _packetLen, tempLen);

		////可能有数据为空的时候
		//if (!(_packetbuff))
		//{
		//	_oldFrameNum = 0;
		//	clear_packet_buff();
		//	_packetLen = 0;
		//	//_oldCrc = 0;// Zander 2013-10-10 
		//	return RET_Fail;
		//}

		// Zander 2013-10-10 Start
		// 判断每一帧末尾的CRC校验码是否一致	
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
		//帧号不参与拷贝复制

		// Zander 2014-10-13
		//ACE_OS::memcpy(_packetbuff + _packetLen,recvBuff_ + 4,recvLen_- 4);  // 末尾的16位CRC校验码不参与拷贝复制		
		ACE_OS::memcpy(_recvPacktBuf + _packetLen, recvBuff_ + 4, recvLen_ - 4);
		
		//为了拼接数据，要将增大缓冲区长度
		_packetLen += recvLen_- 4; 
        // Zander 2013-10-10 End

		//多帧情况下，如果这是最后末帧，才开始进行处理
		if (*(recvBuff_ +3) & 0x80)
		{
			// Zander 2013-10-10 Start
			// 计算整个数据报的CRC校验码
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
			
			//开始处理帧
			do_packet_to_onetdb(_recvPacktBuf, _packetLen, (*(recvBuff_ + 1) & H21_TIME_MARK), isMasterCPU_);
			
			// 处理完成要清空缓冲区
			// clear_packet_buff();
			_oldFrameNum = 0;// Zander 2013-10-10hj
			_packetLen = 0;
			//_oldCrc = 0;// Zander 2013-10-10
		}
	}
	else 
	{
		
		// 不管是多帧还是一帧数据，都要先拷贝到缓冲区中,如果是一帧数据，其实_packetLen 常为0
		__u32 tempLen = _packetLen + (recvLen_- 2);  // // Zander 2013-10-10
		//_packetbuff = (char*)ACE_OS::realloc(_packetbuff,tempLen);

		//可能有数据为空的时候
		//if (!(_packetbuff))
		//{
		//	clear_packet_buff(); // Zander 2013-10-10
		//	_packetLen = 0;
		//	return RET_Fail;
		//}

		// Zander 2013-10-10 Start
		//单帧数据不含帧号，可以直接去掉信息码 + 描述码处理
		ACE_OS::memcpy(_recvPacktBuf + _packetLen,recvBuff_ + 2,recvLen_- 2);
		_packetLen += recvLen_- 2;
		// Zander 2013-10-10 End

		// Zander 2013-10-10 Start
		//// CRC校验计算判断
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

		//开始处理帧
		do_packet_to_onetdb(_recvPacktBuf, _packetLen, (*(recvBuff_ + 1) & H21_TIME_MARK), isMasterCPU_);

		//处理完成要清空缓冲区
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
	//此次处理的控制器个数必须是一个，不能为多个控制器数据
	if (*ptr_ != 1 )
	{
		ACE_ERROR((LM_ERROR, "(%P|%t) DPU num is not 1\n"));
		return ERROR_NODE_OVERLIMIT;
	}
	ptr_++;	//指针指向静态属性的开始
	//控制器的的静态属性
	//复制控制器版本dpuVersion之前的属性
	ACE_OS::memcpy(&_ONetdb->_staticAttr[cpuNum_],ptr_,RTK_FIELD_OFFSET(DPU_ATTR,dpuVersion));

	//此处要处理一下控制器的状态，因为备控制器的状态也是通过主控制器来取得的
	//低位为IP地址小的那个控制器状态
	__u16 dpustatus = _ONetdb->_staticAttr[cpuNum_].dpuStatus;

	//2015 GM 
	__u8 mDpustatus = __u8(_ONetdb->_staticAttr[cpuNum_].dpuStatus);
	__u8 cDpustatus = __u8(_ONetdb->_staticAttr[cpuNum_].dpuStatus >> 0x8);

	//__u8 cDpustatus = __u8(_ONetdb->_staticAttr[cpuNum_].dpuStatus);
	//__u8 mDpustatus = __u8(_ONetdb->_staticAttr[cpuNum_].dpuStatus >> 0x8);

	_ONetdb->_staticAttr[0].dpuStatus = mDpustatus;
	_ONetdb->_staticAttr[1].dpuStatus = cDpustatus;

	//printf("dpuStatus0 = %x,  dpuStatus1 = %x\n", _ONetdb->_staticAttr[0].dpuStatus, _ONetdb->_staticAttr[1].dpuStatus);

	ptr_ += RTK_FIELD_OFFSET(DPU_ATTR,dpuVersion);	//指针指向dpuVersion处
	__u8 verDPULen= *ptr_;
	ACE_OS::memcpy(_ONetdb->_staticAttr[cpuNum_].dpuVersion,ptr_+1,verDPULen);
	ptr_ += verDPULen +1;	//指针指向plcVersion处
	__u8 verPLCLen= *ptr_;
	ACE_OS::memcpy(_ONetdb->_staticAttr[cpuNum_].plcVersion,ptr_+1,verPLCLen);
	ptr_ += verPLCLen +1;	//指针指向oNetIpAddr1处
	_ONetdb->_staticAttr[cpuNum_].oNetIpAddr1 = *(__u32*)ptr_;
	ptr_ += 4;
	_ONetdb->_staticAttr[cpuNum_].oNetIpAddr2 = *(__u32*)ptr_;
	ptr_ += 4;
	_ONetdb->_staticAttr[cpuNum_].oNetIpAddr3 = *(__u32*)ptr_;
	ptr_ += 4;
	_ONetdb->_staticAttr[cpuNum_].oNetIpAddr4 = *(__u32*)ptr_;
	ptr_ += 4;
	_ONetdb->_staticAttr[cpuNum_].syncIpAddr = *(__u32*)ptr_;
	ptr_ += 4; //指针指向DPU_VDI处
	//DPU_VDI处理
	__u8 vdiLen_ = *ptr_;
	ptr_ ++;	//指针指向DPU_STATUS

	do_idvbuff_to_dpustatus(ptr_,cpuNum_);

	ptr_ += vdiLen_;	//指针指向IO的num
	__u8 ioCount_ = *ptr_;
	_ONetdb->_IOCardsCount = (__u16)ioCount_;


	ptr_++;	////指针指向IO_CARD_ATTR
	//人为的判断IO卡是否存在，默认是不存在，当在下面的解析过程中存在，就会被置为true
	// 2013-11-28去掉了这一部分
	//for (__u16 i = 0 ;i < IOCARD_MAX_SIZE;i++)
	//{
	//	_ONetdb->ioCards[i].isExist = __false;
	//}
	for (__u8 i = 0; i < ioCount_; i++)
	{
		//取得IO逻辑地址
		__u8 ioCardAddr_ = *(ptr_ +1);
		__u8 ioType_ = IO_CARD;		//上层模板归类
		//IOMode转换
		if (*(__u8 *)(ptr_+3) == VPTGROUP || *(__u8 *)(ptr_+3) == RETVPTGROUP)
			ioType_ = VPT_CARD;
		else if (*(ptr_+3) == IEDMODEL)
			ioType_ = IED_CARD;
		else
			ioType_ = IO_CARD;
		//IO卡的属性赋值,只取几个有用的信息存储
		_ONetdb->ioCards[ioCardAddr_].isExist = __true;
		_ONetdb->ioCards[ioCardAddr_].dpuAddr = _ONetdb->_staticAttr[cpuNum_].dpuAddr;
		_ONetdb->ioCards[ioCardAddr_].ioId =  *ptr_;
		_ONetdb->ioCards[ioCardAddr_].ioAddr = *(ptr_+1);
		_ONetdb->ioCards[ioCardAddr_].ioStatus = *(ptr_+2);
		_ONetdb->ioCards[ioCardAddr_].ioModType =  *(ptr_+3);;
		_ONetdb->ioCards[ioCardAddr_].ioCardType = *(__u16*)(ptr_+4);
		iolist.push_back(ioCardAddr_); // Zander 2013-11-28

		//由于要将处理指针继续移位，所以要用指针引用
		nRet = do_iobuffer_to_channelvalue(ptr_,ioCardAddr_,ioType_);
		
		
		

		//printf("iolist size = %d\n", iolist.size());
		//当其中返回错误时，数据包有异常，直接返回
		if (nRet == RET_Fail)
		{
			//printf("")
			printf("nRet = %d\n", nRet);
			iolist.clear();// Zander 2013-11-28
			return RET_Fail;
		}	
	}

	//2015 GM 查看具体的IO通道值
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

//处理IO信息
__i32   CONetPctl_20H::do_iobuffer_to_channelvalue(__u8 *& buff_,__u8 ioCardAddr_,__u8 ioType_)
{
	__u8 *&ptr_ = buff_;			//当前处理指针,初始指向IO_VDI
	__u8 *qPtr_ = buff_;			//质量区指针
	__u8 *dPtr_ = buff_;			//数据区指针
	IO_DATA_ATTR *dAttrPtr =NULL;		//数据属性区指针
	__u16 tempNum_ =0;			//IO上的通道编号
	__u8 qualityDataLen_ = 0;	//质量戳区长度
	__u8 valueDataLen_ = 0;		//数据区长度
	__u16 channelCount_ = 0;	//VPT卡上拥有有通道的个数
	__u8 qSize_ = 0;		//质量戳的长度
	__i32 ret = 0;
	//此处指针移动屏蔽了不同类型卡的属性信息
	switch ( ioType_)
	{
	case IO_CARD:
		ptr_ += sizeof(IO_CARD_ATTR);//指针指向IO_VDI
		//IO_VDI处理
		do_idvbuff_to_iostatus(ptr_ + 1,ioCardAddr_,ioType_);
		//数据的控制器中的定义类型

		ptr_ += (*ptr_) +1;	//当前指针指向IO的IO_VAL
		qPtr_ = ptr_ + 1;	//质量区指针
		qualityDataLen_ = (*ptr_);
		dPtr_ = ptr_ + 1 + qualityDataLen_ + 1;	//数据区指针
		valueDataLen_ = *(ptr_ + qualityDataLen_ + 1);
		dAttrPtr =(IO_DATA_ATTR *) (ptr_ + qualityDataLen_ + 1 +valueDataLen_  + 1 +2); //数据属性区指针
		//测点个数的低字节在前，高字节在后
		channelCount_ =*(__u8 *)(ptr_ + qualityDataLen_ + 1 +valueDataLen_  + 1) * 0x100 +(*(__u8 *)(ptr_ + qualityDataLen_ + 1 +valueDataLen_  + 1 +1)) ;

		//当IO通道的个数不正常时，返回错误，并且抛弃当前数据缓冲区的整个处理过程
		if ( channelCount_ <= 0 ||  channelCount_ >= 64)
		{
			return RET_Fail;
		}

		//临时设置通道个数
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
			else	//当初始化的时候_ONetdb->ioCards[ioAddr_].channels是NULL
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
	
		//没采用传递指针引用，记得要整体移动指针
		ptr_ += qualityDataLen_ +1 + valueDataLen_ + 1 + 2 + 4 * sizeof(IO_DATA_ATTR);//指针指向下一个IO卡
		break;
	case IED_CARD:
		qSize_ =1;		//IED质量码为1字节
		ptr_ += sizeof(IED_CARD_ATTR);//指针指向IED_VDI
		//IED卡的VDI暂不处理
		ptr_ += (*ptr_) +1;	//当前指针指向IO的IO_VAL
		qualityDataLen_ = (*ptr_) * 0x100 + *(ptr_+1);	//质量区长度
		qPtr_ = ptr_ + 2;	//质量区指针
		valueDataLen_ = *(__u8 *)(ptr_ + qualityDataLen_ + 2) * 0x100 + *(__u8 *)(ptr_ + qualityDataLen_ + 2 + 1);	//数据区长度
		dPtr_ = ptr_ + 2 + qualityDataLen_ + 2;	//数据区指针
		qSize_ = 1;
		ret = do_databuff_to_iedvalue(ioCardAddr_,qPtr_,qualityDataLen_,dPtr_,valueDataLen_,qSize_);
		//没采用传递指针引用，记得要整体移动指针
		ptr_ += qualityDataLen_ +2 + valueDataLen_ + 2;//指针指向下一个IO卡

		break;
	case VPT_CARD:
		channelCount_ = *(__u8*)(ptr_ + 4) * 0x100 + *(__u8*)(ptr_ + 5);	//数据区长度在静态属性中
		ptr_ += sizeof(VPT_CARD_ATTR);//指针指向VPT_ATTR DETAILS	sizeof = 6

		//采用指针引用，内部指针移位
		ret = do_databuff_to_vptvalue(ioCardAddr_,ptr_,channelCount_);
		if (ret == RET_Fail)
		{
			return RET_Fail;
		}
		break;
	}

	return RET_OK;
}

//处理DPU的IDV信息,根据协议中的内容只取一部分信息，具体要用时再扩展解析,任何情况下，只监视当前在运行的DPU，所以主备都是一样的状态
__i32  CONetPctl_20H::do_idvbuff_to_dpustatus(__u8 * buff_,__bool isDpuMaster_ )
{
	//注意此处的解析是根据控制器发送信息来具体解析，格式定义见控制器的相关说明文档
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
//处理IO的IDV信息,根据协议中的内容只取一部分信息，具体要用时再扩展解析
__i32  CONetPctl_20H::do_idvbuff_to_iostatus(__u8 * buff_,__u8 ioCardAddr_,__u8 ioModType_ )
{
	__u8 * ptr_ = buff_;
	//_ONetdb->ioCards[ioCardAddr_].ioStatus = (*(ptr_+0))& 0x01;


	return RET_OK;
}
__i32 CONetPctl_20H::do_databuff_to_iedvalue(
	__u8 ioAddr_,		//IO卡逻辑地址
	__u8 *qbuff_,		//质量戳指针
	__u8 qLen_,			//质量区长度
	__u8 *valbuff_,		//数据区指针
	__u8 vLen_,			//数据区长度
	__u8 qSize_)		//质量戳占有字节
{



	return RET_OK;


};
__i32 CONetPctl_20H::do_databuff_to_vptvalue(
	__u8 ioAddr_,		//IO逻辑地址
	__u8 * &valbuff_,		//VPT的数据指针
	__u16 vCount_ )		//VPT的VPT_VALUE的个数
{
	__u8 *&ptr_ = valbuff_;	//指针指向VPT ATTR DETAILS
	__u16 valLen_ = 0;
	__u32 sumChaCount_ = 0;//VPT中的数据总个数
	__u16 channelNum_ = 0;	//VPT中的通道计数器
	__u16 qData_ = 0;			//VPT共用的质量戳值
	__u16 j = 0,i = 0;
	__u8 tempVal[4] = {0};
	__u8 * types = NULL;			//每个VPT的类型
	__u16 * percounts = NULL;		//每个VPT的数据个数
	__u8 * vptStart_ptr = NULL;		//VPT中的数据区起始指针，可根据这个指针来计算数据点的偏移量
	if (vCount_ == 0)
	{
		//等于零时怎么处理
		return RET_Fail;
	}
	else
	{
		types = new __u8[vCount_];		//生成VPT类型数组
		percounts = new __u16[vCount_];	//生成VPT中数据个数数组
	}
	for ( i = 0; i < vCount_; i ++)
	{
		types[i] = *(__u8 *)(ptr_);
		percounts[i] =*(__u8 *)(ptr_+1) * 0x100 + *(__u8 *)(ptr_+2);
		ptr_ += (sizeof(__u8) +sizeof(__u16));	//每次指针增加3个字节
		sumChaCount_ += percounts[i];
	}
	//当VPT点的个数不正常时，返回错误，并且抛弃当前数据缓冲区的整个处理过程
	if (sumChaCount_ <= 0)
	{
		return RET_Fail;	
	}


	//临时设置通道个数
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
		else	//当初始化的时候_ONetdb->ioCards[ioAddr_].channels是NULL
		{
			_ONetdb->_data_mutex.acquire_write();
			_ONetdb->ioCards[ioAddr_].channels = (nicONetChannel *)ACE_OS::malloc(sumChaCount_ * sizeof(nicONetChannel));
			memset(_ONetdb->ioCards[ioAddr_].channels, 0, sumChaCount_*sizeof(nicONetChannel));//Zander 2012-7-24
			_ONetdb->_data_mutex.release();
		}
	}
	_ONetdb->ioCards[ioAddr_].ioCurNum = sumChaCount_;
	ACE_ASSERT(_ONetdb->ioCards[ioAddr_].channels != NULL);
	qData_ = *(__u16 *) ptr_;			//质量戳2字节
	ptr_ += 2; //指针指向数据长度
	valLen_ = *(__u8*)ptr_ * 0x100 + *(__u8*)(ptr_ + 1);
	ptr_ += 2; //指针指向数据区
	vptStart_ptr = ptr_;
	for ( i = 0; i < vCount_; i ++)
	{
		switch(types[i])
		{
		case BOOL_1_BIT:
			//BOOL型的一个字节有8个点信息
			for (j = 0; j < percounts[i]; j++)
			{	
				if(j!= 0)
				{
					ptr_ += (j%8)?0:1;		//逢8指针加1
				}
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = BOOL_1_BIT;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].offAddr = (__u16)(ptr_ - vptStart_ptr);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].subOffAddr = (__u8)(j%8) ;//偏移量计数从0 开始到 7
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = *(__u8 *)(&qData_); //把质量码放在实时值内，但这个质量定义暂时与nicDefs.h的定义不同
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData..DataType = DBDataType_Bool;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_bool);
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.b = ((*(__u8*)ptr_ )& (0x01 << (j%8)))?__true:__false;
				channelNum_++;
			}
			ptr_ ++;//指向下一种类型的数据区
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Float;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_float);
				//tempVal[0] = *(ptr_ + 3); Float低位自动填充为0
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality =  *(__u8 *)(&qData_);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Float;
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_float);
				//tempVal[0] = *(ptr_ + 3); 低位自动填充为0
				//tempVal[1] = *(ptr_ + 2); 自动填充为0
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = *(__u8 *)(&qData_);	//通道的状态就是质量码
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
	__u8 ioAddr_,		//IO卡逻辑地址
	__u8 *qbuff_,		//质量戳指针
	__u8 qLen_,			//质量区长度
	__u8 *valbuff_,		//数据区指针
	__u8 vLen_,			//数据区长度
	IO_DATA_ATTR *datattr_,	//值属性数组
	__u8 qSize_)		//质量戳占有字节
{
	__u8 i = 0;
	__u8 j = 0;
	__u8 datatype_ = 0;
	__u8 channelNum_ =0;
	__u8 tempVal[4];
	__u8 *Qptr_ = qbuff_;	//质量区的当前指针
	__u8 *Vptr_ = valbuff_;	//数据区的当前指针
	__u8 *Tptr_ =(__u8*)datattr_;	//属性区指针
	__u16 chCount_ = 0;		//总,在IO_DATA_ATTR取得

	//存在IO_DATA_ATTR为乱码的情况
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
					Vptr_ += (j%8)?0:1;		//逢8指针加1
				}
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dpuAddr = _ONetdb->ioCards[ioAddr_].dpuAddr;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioAddr = ioAddr_;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].dataType = BOOL_1_BIT;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].ioModType = _ONetdb->ioCards[ioAddr_].ioModType;
				//通道的质量码有16的话只采用低8位
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality = (qSize_==2)? *(__u8*)(qbuff_+channelNum_* 2 + 1):(__u8)qbuff_[channelNum_];
				set_value_type(_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Flags, dt_bool);
				//_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Value.DataType = DBDataType_Bool;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.value.Value.b = ((*(__u8*)Vptr_) & (0x01 << (j%8)))?__true:__false;
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
				channelNum_ ++;

			}
			Vptr_ ++;//指向下一种类型的数据区
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
				//tempVal[0] = *(Vptr_ + 3); Float低位自动填充为0
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
				//tempVal[0] = *(Vptr_ + 3); 低位自动填充为0
				//tempVal[1] = *(Vptr_ + 2); 自动填充为0
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
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
				_ONetdb->ioCards[ioAddr_].channels[channelNum_].status = _ONetdb->ioCards[ioAddr_].channels[channelNum_].rData.Quality;	//通道的状态就是质量码
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

//普通数据帧只处理从info_code(包含)开始的信息
__i32 CONetPctl_15H::parse_packet(
					  __u8 *recvBuff_,
					  __u32 recvLen_,
					  __u8 curNetNum_)
{
	__u8 * ptr_ = recvBuff_;
	ACE_DEBUG((LM_DEBUG, "DPU返回错误信息: 0x%x----",*(ptr_ + 1)));
	switch (*(ptr_ + 1)) {
	case ERROR_UNKNOW_SFDCS: ACE_DEBUG((LM_DEBUG, "包头不是0x68H\n")); break;
	case ERROR_NOBUILDLINK: ACE_DEBUG((LM_DEBUG, "没有建立握手连接，就发出了请求或其他数据请求\n")); break;
	case ERROR_SUBSC_INVALID: ACE_DEBUG((LM_DEBUG, "申请订购定时器失败\n")); break;
	case ERROR_ALREADYHASTIMER: ACE_DEBUG((LM_DEBUG, "无用\n")); break;
	case ERROR_TIMEROVERFLOW: ACE_DEBUG((LM_DEBUG, "申请的定时器小于1\n")); break;
	case ERROR_HASNOTIMER: ACE_DEBUG((LM_DEBUG, "目前无用\n")); break;
	case ERROR_ALREADYLINK: ACE_DEBUG((LM_DEBUG, "已经建立了连接，又重新申请连接\n")); break;
	case ERROR_INVALIDPACKET: ACE_DEBUG((LM_DEBUG, "报头不是0x68H\n")); break;
	case ERROR_LENTOOSHORT: ACE_DEBUG((LM_DEBUG, "报头中长度小于等于0\n")); break;
	case ERROR_IODEAD: ACE_DEBUG((LM_DEBUG, "访问的IO已死\n")); break;
	case ERROR_IOADDR: ACE_DEBUG((LM_DEBUG, "IO地址错\n")); break;
	case ERROR_UNKNOWNDATATYPE: ACE_DEBUG((LM_DEBUG, "下行数据的数据类型，DPU没有定义\n")); break;
	case ERROR_MAKEFRAME: ACE_DEBUG((LM_DEBUG, "组祯出现了错误\n")); break;
	case ERROR_NOVALIDIO: ACE_DEBUG((LM_DEBUG, "请求数据中IO地址都找不到\n")); break;
	case ERROR_INVALID_TARGET: ACE_DEBUG((LM_DEBUG, "DPU地址不符合定义\n")); break;
	case ERROR_INVALID_SRC: ACE_DEBUG((LM_DEBUG, "HMI节点地址不符合定义\n")); break;
	case ERROR_REGISTER_DEV: ACE_DEBUG((LM_DEBUG, "注册通信设备地址出错\n")); break;
	case ERROR_TARGET_NOTCORRECT: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_DPU_STATUS_NORUN: ACE_DEBUG((LM_DEBUG, "DPU状态不是运行态\n")); break;
	case ERROR_DEVNO_INVALID: ACE_DEBUG((LM_DEBUG, "在节点中设备序号无效\n")); break;
	case ERROR_NODE_OVERLIMIT: ACE_DEBUG((LM_DEBUG, "请求DPU或IO的数目太多，超过了限制\n")); break;
	case ERROR_NO_PROTOCOL: ACE_DEBUG((LM_DEBUG, "协议没有初始化\n")); break;
	case ERROR_NODE_HASNOADDR: ACE_DEBUG((LM_DEBUG, "请求中出现了无效的DPU或IO地址\n")); break;
	case ERROR_NODE_INVALID: ACE_DEBUG((LM_DEBUG, "返回无效的地址\n")); break;
	case SYS_MEM_ALLOC_ERR: ACE_DEBUG((LM_DEBUG, "系统分配内存出错\n")); break;
	case SYS_QERR_QFULL: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_RIGHT_NOWRITE: ACE_DEBUG((LM_DEBUG, "没有写权限\n")); break;
	case ERROR_OPEN_FILENAME: ACE_DEBUG((LM_DEBUG, "打开文件错\n")); break;
	case ERROR_NOEVENT_ITEM: ACE_DEBUG((LM_DEBUG, "读事件缓冲区错（空）\n")); break;
	case ERROR_NOLOG_ITEM: ACE_DEBUG((LM_DEBUG, "读LOG缓冲区错（空）\n")); break;
	case ERROR_OVERLIMIT: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_NO_LBDATA: ACE_DEBUG((LM_DEBUG, "没有录波文件\n")); break;
	case ERROR_SETTING_INVALID: ACE_DEBUG((LM_DEBUG, "定值区无效（指读取NVRAM失败、定值区无效）\n")); break;
	case ERROR_SETTING_AREA: ACE_DEBUG((LM_DEBUG, "定值区号错\n")); break;
	case ERROR_SETTING_NUM: ACE_DEBUG((LM_DEBUG, "定值个数错\n")); break;
	case ERROR_SETTING_OVERFLOW: ACE_DEBUG((LM_DEBUG, "定值越限\n")); break;
	case ERROR_SETTING_CRC: ACE_DEBUG((LM_DEBUG, "定值crc校验错\n")); break;
	case ERROR_SETTING_TYPE: ACE_DEBUG((LM_DEBUG, "定值类型错\n")); break;
	case ERROR_SETTING_STATE: ACE_DEBUG((LM_DEBUG, "操作状态错\n")); break;
	case ERROR_SETTING_WFILE: ACE_DEBUG((LM_DEBUG, "往配置文件写定值区号错\n")); break;
	}


	return RET_OK;
}
CONetPctl_40H:: CONetPctl_40H(ONet_DB *db):CONetBase( __false,__false,__false,db)
{

}
CONetPctl_40H::~CONetPctl_40H()
{

}

//普通数据帧只处理从info_code(包含)开始的信息
__i32 CONetPctl_40H::parse_packet(
					  __u8 *recvBuff_,
					  __u32 recvLen_,
					  __u8 curNetNum_)
{
	return RET_OK;

}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                