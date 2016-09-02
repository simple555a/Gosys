
#include"precomp.h"


#include"SeiralScheduler.h"
#include"SerialComm.h"

CLock commlocker;


CRPNScheduler::CRPNScheduler(int pno,char *setting ,__uint refreshTime)
	: CSerialPort(pno,setting)
{
	m_reftime = refreshTime;
	memset(rcv_buff,0,rtk_serial_recv_len);
	memset(rpn_mid_buff,0,RPN_PTCL_LEN);
	memset(rpn_pow_buff,0,RPN_PTCL_LEN);
	memset(rpn_rng_buff,0,RPN_PTCL_LEN);
}

CRPNScheduler::~CRPNScheduler()
{

}

int CRPNScheduler::do_comm_recv()
{
	unsigned char temp[4] ={0};
	temp[0]= 0xAA;
	temp[1]= 0xAA;
	temp[2]= 0x55;
	temp[3]= 0x55;
	__uint retlen = 0;
	//此处会阻塞接收线程
	while(this->read(rcv_buff,RPN_HEADER_MAGIC_LEN,&retlen)&& !this->m_bStopPending)
	{
		//如果不是帧头，跳出继续寻找帧头
		if(memcmp(rcv_buff,temp,RPN_HEADER_MAGIC_LEN)!= 0)
			continue;
		//如果帧不对，直接放弃本次读
		if(read(rcv_buff+RPN_HEADER_MAGIC_LEN,RPN_PTCL_LEN-RPN_HEADER_MAGIC_LEN,&retlen)== false 
			|| retlen!=RPN_PTCL_LEN-RPN_HEADER_MAGIC_LEN )
			return -1;
		//这里还需要CRC32校验
		//todo it 

		commlocker.Lock();
		//判断机柜通道编码
		switch(rcv_buff[5])
		{
		case RPN_S_RANGE:
			memcpy(rpn_rng_buff,rcv_buff,RPN_PTCL_LEN);
			break;
		case RPN_M_RANGE:
			memcpy(rpn_mid_buff,rcv_buff,RPN_PTCL_LEN);
			break;
		case RPN_P_RANGE:
			memcpy(rpn_pow_buff,rcv_buff,RPN_PTCL_LEN);
			break;
		}
		commlocker.UnLock();
		return retlen;

	}
	return retlen;
}

__uint CRPNScheduler::run()
{	 


	
	notify(CRtkThread::EVT_INIT_COMPLETE);
	

	
	bool stopping=false;

	while(this->m_EvtStop.Wait(500) != WAIT_OBJECT_0)
	{ 
		check_and_open();
		//接收串口数据
		do_comm_recv();

		check_error();
	}
	
	close();

	utils_error(
		"Port %d scheduler stopped, %d sent(%d bytes), %d got(%d bytes), %d errors\n",
		m_iPortNo,
		m_iSent,
		m_iSentBytes,
		m_iGot,
		m_iGotBytes,
		m_iErrCount
		);

	return 1;
}
	__bool CRPNScheduler::check_and_open()
	{
		if(!opened()){
			open();
		}
		return opened();
	}