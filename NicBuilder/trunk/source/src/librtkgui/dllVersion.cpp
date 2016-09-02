#include "precomp.h"
#pragma hdrstop
#include "utils.h"
#include "DLLVersion.h"

/**********************************************************

Function: GetDLLVersion

Purpose: Retrieves DLL major version, minor version and build numbers

Input: DLL file name
Reference to Major number 
Reference to Minor number 
Reference to Build number 

Output: TRUE only if successful

Remarks: This function first tries to get the DLL version the nice way,
that is, call the DllGetVersion function in the DLL.

If this fails, it tries to located the DLL file in the file system,
read the file information block and retrieve the file version.

****************************************************************************/
BOOL CDLLVersion::GetDLLVersion (LPCSTR szDLLFileName, 
DWORD &dwMajor, DWORD &dwMinor, DWORD &dwRelease, DWORD &dwBuildNumber)
{
	
	char szFileName [MAX_PATH]; // Temp file name

	lstrcpy (szFileName, szDLLFileName); // Save a file name copy for the loading
	return CheckFileVersion (szFileName,  
				dwMajor, dwMinor, dwRelease, dwBuildNumber); // Try the ugly way
}

/***************************************************************************

Function: CheckFileVersion

Purpose: Check the version information of a given file

Input: File name
File location (Windows dir, System dir, Current dir or none)
Reference to Major number 
Reference to Minor number 
Reference to Build number 

Output: TRUE only if successful

Remarks: Trashes original file name

****************************************************************************/
BOOL CDLLVersion::CheckFileVersion (LPSTR szFileName, 
DWORD &dwMajor, DWORD &dwMinor, DWORD &dwRelease,
DWORD &dwBuildNumber)
{
	LPSTR lpVersion; // String pointer to 'version' text
	UINT uVersionLen;
	DWORD dwVerHnd=0; // An 'ignored' parameter, always '0'


	//FixFilePath (szFileName); // Add necessary path prefix to file name

	DWORD dwVerInfoSize = GetFileVersionInfoSize (szFileName, &dwVerHnd);
	if (!dwVerInfoSize) // Cannot reach the DLL file
		return FALSE;

	LPSTR lpstrVffInfo = (LPSTR) malloc (dwVerInfoSize); // Alloc memory for file info
	if (lpstrVffInfo == NULL)
		return FALSE; // Allocation failed

	// Try to get the info
	if (!GetFileVersionInfo(szFileName, dwVerHnd, dwVerInfoSize, lpstrVffInfo)) {
		free (lpstrVffInfo);
		return FALSE; // Cannot read the file information - 
		// wierd, since we could read the information size
	}

	LANGANDCODEPAGE *lpTranslate;
	char  szFileVersion[MAX_PATH];
	UINT cbTranslate;

	VerQueryValue(lpstrVffInfo, 
              TEXT("\\VarFileInfo\\Translation"),
              (LPVOID*)&lpTranslate,
              &cbTranslate);

	sprintf(szFileVersion, "\\StringFileInfo\\%04x%04x\\FileVersion", lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
	
	if (!VerQueryValue ( lpstrVffInfo, (LPSTR) (TEXT(szFileVersion)), 
			(LPVOID *)&lpVersion, (UINT *)&uVersionLen)) 
	{
		free (lpstrVffInfo);
		return FALSE; // Query was unsuccessful
	}
	
		
	// Now we have a string that looks like this :
	// "MajorVersion.MinorVersion.BuildNumber", so let's parse it

	BOOL bRes = ParseVersionString (lpVersion, dwMajor, dwMinor, dwRelease, dwBuildNumber, ",");
	if (!bRes)
	{
		bRes = ParseVersionString (lpVersion, dwMajor, dwMinor, dwRelease, dwBuildNumber, ".");
	}
	free (lpstrVffInfo);
	return bRes;
}

/***************************************************************************

Function: ParseVersionString

Purpose: Parse version information string into 3 different numbers

Input: The version string formatted as "MajorVersion.MinorVersion.BuildNumber"
Reference to Major number 
Reference to Minor number 
Reference to Build number 

Output: TRUE only if successful

Remarks: 

****************************************************************************/
BOOL CDLLVersion::ParseVersionString (LPSTR lpVersion, 
DWORD &dwMajor, DWORD &dwMinor, DWORD &dwRelease,
DWORD &dwBuildNumber, char *sztoken)
{
	// Get first token (Major version number)
	LPSTR token = strtok( lpVersion, sztoken); 
	if (token == NULL) // End of string
		return FALSE; // String ended prematurely
	dwMajor = atoi (token);

	token = strtok (NULL, sztoken); // Get second token (Minor version number)
	if (token == NULL) // End of string
		return FALSE; // String ended prematurely
	dwMinor = atoi (token);

	token = strtok (NULL, sztoken); // Get third token (Build number)
	if (token == NULL) // End of string
		return FALSE; // String ended prematurely
	dwRelease = atoi (token);
	
	token = strtok (NULL, sztoken); // Get third token (Build number)
	if (token == NULL) // End of string
		return FALSE; // String ended prematurely
	dwBuildNumber = atoi (token);

	return TRUE;
}




/***************************************************************************

Function: FixFilePath

Purpose: Adds the correct path string to a file name according 
to given file location 

Input: Original file name
File location (Windows dir, System dir, Current dir or none)

Output: TRUE only if successful

Remarks: Trashes original file name

****************************************************************************/
BOOL CDLLVersion::FixFilePath (char * szFileName)
{
	char szPathStr [MAX_PATH]; // Holds path prefix

	switch (this->m_FileLoc) {
		case WIN_DIR:
		// Get the name of the windows directory
		if (GetWindowsDirectory (szPathStr, MAX_PATH) == 0) 
			return FALSE; // Cannot get windows directory
		break;

		case SYS_DIR:
		// Get the name of the windows SYSTEM directory
		if (GetSystemDirectory (szPathStr, MAX_PATH) == 0) 
			return FALSE; // Cannot get system directory
		break;

		case CUR_DIR:
		// Get the name of the current directory
		if (GetCurrentDirectory (MAX_PATH, szPathStr) == 0) 
			return FALSE; // Cannot get current directory
		break;
		
		case MOD_DIR:
		if (FixModulePath(NULL, szPathStr) == 0)
			return FALSE;
		break;

		case NO_DIR:
			lstrcpy (szPathStr,"");
		break;

		default:
			return FALSE;
	}
	strcat (szPathStr, "\\");
	strcat (szPathStr, szFileName);
	strcpy (szFileName, szPathStr);
	return TRUE;
} 

/* 获得dll或者文件版本号和修改日期 */
RTKGUIAPI BOOL WINAPI RtkGuiGetFileVersion(RTKGUIFILEVERSION *lpvInfo, LPCSTR lpszFileName)
{
	if (!lpvInfo || !lpszFileName)
	{
		return FALSE;
	}
	
	CDLLVersion dllver(lpszFileName);

	if (!dllver.IsValid())
	{
		return FALSE;
	}
	
	lpvInfo->dwMajorVersion = dllver.GetMajorVersion();
	lpvInfo->dwMinorVersion = dllver.GetMinorVersion();
	lpvInfo->dwReleaseVersion = dllver.GetReleaseVersion();
	lpvInfo->dwBuildNumber  = dllver.GetBuildNumber();
	
	
	
	return TRUE;
}