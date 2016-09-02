/* For some odd reason, Microsoft published a sample code that uses shlwapi.h 
(and shlwapi.lib)
to tinker file versions.

This header file could not be found anywhere !!!
Not in Visual C++ 4.2, Visual C++ 5.0 or MSDN versions up to July 97`.

So, I just took out the interesting structures from scraps I found 
and re-defined them here.
*/


// Remember: You must link version.lib to the project for this class to work !!
// 切记:为使该类正常工作,你必须将version.lib链接到你的项目中!!

#ifndef _DLL_VERSION_H_
#define _DLL_VERSION_H_


typedef struct tagLANGANDCODEPAGE {
  WORD wLanguage;
  WORD wCodePage;
} LANGANDCODEPAGE;


typedef enum 
{	WIN_DIR, 
	SYS_DIR, 
	CUR_DIR, 
	MOD_DIR,
	NO_DIR 
} FileLocationType; 


class CDLLVersion
{
	

public:
	CDLLVersion (LPCSTR szDLLFileName, FileLocationType FileLoc) : m_dwMajor (0), m_dwMinor (0), m_dwRelease(0), m_dwBuild (0)
	{
		m_FileLoc = FileLoc;
		m_bValid = GetDLLVersion (szDLLFileName, m_dwMajor, m_dwMinor, m_dwRelease, m_dwBuild);
	}
	
	CDLLVersion (LPCSTR szDLLFileName) : m_dwMajor (0), m_dwMinor (0), m_dwRelease(0), m_dwBuild (0)
	{
		m_bValid = GetDLLVersion (szDLLFileName, m_dwMajor, m_dwMinor, m_dwRelease, m_dwBuild);
	}

	virtual ~CDLLVersion () {};

	DWORD GetMajorVersion ()
	{
		return m_dwMajor;
	}

	DWORD GetMinorVersion ()
	{
		return m_dwMinor;
	}
	
	DWORD GetReleaseVersion()
	{
		return m_dwRelease;
	}
	DWORD GetBuildNumber ()
	{
		return m_dwBuild;
	}
	
	BOOL IsValid ()
	{
		return m_bValid;
	}

private:

	BOOL GetDLLVersion (LPCSTR szDLLFileName, 
			DWORD &dwMajor, DWORD &dwMinor, DWORD &dwRelease, DWORD &dwBuildNumber);
	
	
	BOOL CheckFileVersion (LPSTR szFileName,
		DWORD &dwMajor, DWORD &dwMinor, DWORD &dwRelease, DWORD &dwBuildNumber);

	BOOL ParseVersionString (LPSTR lpVersion, 
			DWORD &dwMajor, DWORD &dwMinor, DWORD &dwRelease, DWORD &dwBuildNumber, char *sztoken);

	BOOL FixFilePath (char * szFileName);

	DWORD m_dwMajor, // Major version number
		  m_dwMinor, // Minor version number
		  m_dwRelease, // Release version number
		  m_dwBuild; // Build number
	FileLocationType m_FileLoc;
	BOOL  m_bValid; // Is the DLL version information valid ?
};

#endif
