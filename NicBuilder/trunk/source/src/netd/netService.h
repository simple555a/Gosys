//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPSSERVICE_H__60DEB7A7_A202_4EFF_9561_ECD0F7E76616__INCLUDED_)
#define AFX_GPSSERVICE_H__60DEB7A7_A202_4EFF_9561_ECD0F7E76616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NTService.h"

class CNetService : public CNTService  
{
public:
	virtual void Stop();
	virtual void Run(DWORD, LPTSTR *);
	CNetService();
	virtual ~CNetService();

private:
	bool Init();
	HANDLE hStop;
};

#endif // !defined(AFX_GPSSERVICE_H__60DEB7A7_A202_4EFF_9561_ECD0F7E76616__INCLUDED_)
