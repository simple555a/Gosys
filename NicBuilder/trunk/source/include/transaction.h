/*
	PC based Monitoring System source file
	version 1.0
	by J.Hunter,jhunter@263.net, May 2001 
*/
#ifndef __rtkasync_h__
#define __rtkasync_h__
#include <list>

#include <utils.h>
#include <rtknet.h>
#include <object.h>

// use 4 bytes alignment
// to keep CTransaction::m_iRefCount at dword boundary
#include <pshpack4.h>	
class RTKNET_API CTransaction{
public:
	CTransaction();	
	virtual ~CTransaction();
	virtual void OnEvent(PCRTK_PACKET packet)=0;
	virtual void OnTimeout(){};	
	virtual void OnProgress(__uint total, __uint completed, PCRTK_PACKET p){};
public:
	// int		TimeOut; // in miliseconds
	RTK_GUID Guid;
	RTK_TIME birth;
	double	 life;
	int		 m_iRefcount;
	char     usrname[32];//mox20141017

	friend class CTransactionContainer;
	CTransactionContainer *m_Parent;
};
#include <poppack.h>

class RTKNET_API CWaitableTransaction : public CTransaction{
protected:
	CEvent	Event;	 //
	__bool	m_bWaitResult;
public:
	CWaitableTransaction();
	__bool Wait(__uint TimeOut, __bool ProcessMessage = __false);
	inline __bool WaitResult()
	{
		return m_bWaitResult;
	}
	void Cancel();
	void SetEvent();
	virtual void OnTimeout()
	{
		this->SetEvent();
	}
	virtual void OnCancel(){};
	virtual ~CWaitableTransaction();
};

RTKNET_API __bool PMC_API add_transaction(HVBUS, CTransaction *);

#endif
