#ifndef __evtsrv_h__
#define __evtsrv_h__
#include "mcudp.h"

class CEvtSocket : public CMulticastSocket{
public:
    CEvtSocket();
    virtual int OnData(void * buf, int length, int port, char * src_addr);
};

class CEvtSrv : public CNTService  
{
public:
	CEvtSrv()
		:CNTService(TEXT("evtd"), TEXT("PMC事件记录服务"))
	{
	};

	virtual void Run(DWORD, LPTSTR*);
	virtual void Stop();

private:
	static void __stdcall flush(struct HWND__ *,unsigned int,unsigned int,unsigned long);
	static void __stdcall record_soe(struct HWND__ *,unsigned int,unsigned int,unsigned long);
	static void __stdcall raise_alarms(struct HWND__ *,unsigned int,unsigned int,unsigned long);
	static int init_soe(string msg_file);

	DWORD dwMainThreadId;
	UINT idTimer;
	UINT idTimerSoe;
	UINT idTimerAlarm;
	CEvtSocket sck;
};

#endif
