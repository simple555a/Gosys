#include "precomp.h"


inline char* _extract_file_name(char * path)
{
	char * p;
	
	p=path+strlen(path)-1;
	while(p != path){
		if(*p == '\\' || *p=='/')	break;
		p--;
	}
	return p+1;
}

inline char* _extract_file_path(char * path)
{
	char * p;
	p=_extract_file_name(path);
	*(p-1)=0;
	return path;
}

BOOL FixModulePath(HMODULE hModule, LPSTR lpFilePath)
{
	char szFilePath[MAX_PATH];

	
	if (!GetModuleFileName(hModule, szFilePath, MAX_PATH))
	{
		return FALSE;
	}

	_extract_file_path(szFilePath);

	strcpy(lpFilePath, szFilePath);
	
	return TRUE;
}