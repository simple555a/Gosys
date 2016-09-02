#pragma warning(disable:4996)
#include "precomp.h"

/*
	function : is_win98_fe
	returns TRUE if current OS is windows 98 first version.
*/
LIBUTILS_API BOOL UTILS_API is_win98_fe()
{
	OSVERSIONINFO osi;
	
	ZeroMemory(&osi,sizeof(osi));
	osi.dwOSVersionInfoSize=sizeof(osi);

	if( !GetVersionEx(&osi) ){
		wperrorA("GetVersionEx");
	}

	// seems to be MS's bug, dwBuildNumber is a DWORd, but only the
	// lower 16 bits are used.
	osi.dwBuildNumber &= 0xffff;

	utils_debugA("Windows version:%d.%d, build %d\n",
		osi.dwMajorVersion,
		osi.dwMinorVersion,
		osi.dwBuildNumber
	);
	utils_debugA("CSD : %s\n",osi.szCSDVersion?osi.szCSDVersion : "");
	
	if(osi.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS){
		return FALSE;
	}
	if(osi.dwMajorVersion != 4){
		return FALSE;
	}
	if(osi.dwMinorVersion != 10){
		// is it windows 98?
		return FALSE;
	}
	// if(2184 == osi.dwBuildNumber || 2222 == osi.dwBuildNumber){
	if(2184 <= osi.dwBuildNumber){	
		// is it windows 98 second version?
		return FALSE;
	}
	if(!stricmp(osi.szCSDVersion,"A")){
		// is it windows 98 second version?
		return FALSE;
	}
	if(osi.szCSDVersion){
		if(!stricmp(osi.szCSDVersion,"µÚ¶þ°æ")){
			return FALSE;
		}
	}
	return TRUE;
}