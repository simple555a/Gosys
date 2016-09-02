
#ifndef  __PMC_RPN_SCHEDULE_H__
#define  __PMC_RPN_SCHEDULE_H__

#include"pmc.h"
#include "precomp.h"
#include "SerialComm.h"



class CRPNScheduler : 
	public CRtkThread, public CSerialPort
{
public:
	CRPNScheduler(int pno,char *setting = 0,__uint refreshTime = 500);
	virtual ~CRPNScheduler();
	virtual __uint run();
	__bool check_and_open();
	int do_comm_recv();
	char rpn_pow_buff[RPN_PTCL_LEN]; //功率量程
	char rpn_mid_buff[RPN_PTCL_LEN]; // 中间量程
	char rpn_rng_buff[RPN_PTCL_LEN]; // 满量程
protected:
	__uint m_reftime;
	char rcv_buff[rtk_serial_recv_len];

};
 


#endif