#ifndef LimitSingleInstance_H
#define LimitSingleInstance_H

#include <windows.h> 

//this code is from Q243953 in case you lose the article and wonder
//where this code came from...
class CLimitSingleInstance
{
private:
	HANDLE m_hMutex;
	TCHAR m_mutexName[64];
	
public:
	CLimitSingleInstance(TCHAR *strMutexName)
	{
		memset( m_mutexName, 0, sizeof( m_mutexName ) );
		strncpy( m_mutexName, strMutexName, sizeof( m_mutexName ) - 1 );
	}
	
	~CLimitSingleInstance() 
	{
		if (m_hMutex)  //don't forget to close handles...
		{
			CloseHandle(m_hMutex); //do as late as possible
			m_hMutex = NULL; //good habit to be in
		}
	}
	
	BOOL IsAnotherInstanceRunning() 
	{
		//be sure to use a name that is unique for this application otherwise
		//two apps may think they are the same if they are using same name for
		//3rd parm to CreateMutex
		
		SECURITY_DESCRIPTOR    SD, *pSD;
		SECURITY_ATTRIBUTES    sa;
		
		pSD = &SD;
		
		if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
		{
			return FALSE;
		}
		
		// Add a NULL DACL to the security descriptor..
		
		if (!SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE))
		{
			return FALSE;
		}
		
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = TRUE;
		
		m_hMutex = CreateMutex( &sa, FALSE, m_mutexName );
		DWORD e;
		e = GetLastError();
		return (ERROR_ALREADY_EXISTS == e);
	}
};
#endif 