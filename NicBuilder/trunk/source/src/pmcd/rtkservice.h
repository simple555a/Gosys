#ifndef __rtkservice_h__
#define __rtkservice_h__

#ifdef _WIN32
#include "ntservice.h"
class CRtkService : public CNTService 
{	  
	 
#elif defined(__LINUX__)
class CRtkService
{				
#endif	   
	CEvent m_hStop; 	
	int	m_dwStopReason;	// 0=stop, 1=restart
	int 	m_dwStartType;
	string versionString;
public:
	__bool	m_bPending;

	CRtkService();
#ifdef _WIN32	
	virtual void Run(DWORD, LPTSTR*);
#else
	virtual void Run(__uint, char **);
#endif
	virtual void	Stop();
	virtual void	Restart();

#ifdef __LINUX__
	void ReportStatus(__uint mode, __uint delay=0)
	{
	}
#define SERVICE_START_PENDING 0
#define SERVICE_STOP_PENDING  1
#define SERVICE_RUNNING 	  2
#define SERVICE_STOPPED 	  3
#define SERVICE_DEMAND_START  4
#endif
private:
	bool _init();
	bool _uninit();
	bool _restart();
};

extern CRtkService serv;

#endif
