/********************************************************************
	created:	2002/07/17
	created:	17:7:2002   18:11
	filename: 	D:\DFVS2\SRC\VC\PUBLIC\SRC\LIBUTILS\files.cpp
	file path:	D:\DFVS2\SRC\VC\PUBLIC\SRC\LIBUTILS
	file base:	files
	file ext:	cpp
	author:		Jackie Hunter, jhunter@tsinghua.org.cn
	
	purpose:	处理文件的公用代码
	History:
*********************************************************************/
#pragma warning(disable:4996)

#include "precomp.h"
#pragma hdrstop

#ifdef _WIN32
#pragma comment (lib,"version.lib")
#endif

#ifdef _WIN32

#ifndef NDEBUG
static void my_heapchk()
{
	int heapstatus = _heapchk();
	assert(_HEAPOK == heapstatus);
}
#else
#define my_heapchk()
#endif

static 
// _declspec(thread) 
char g_szVersion[32]="";

LIBUTILS_API char * UTILS_API 
get_file_versionA(const char * szFile, LARGE_INTEGER * liVersion)
{	
	char * pVersion=0;
	DWORD dwVersionSize, dwHnd;
	LARGE_INTEGER _liVersion;
	WORD * s;
	VS_FIXEDFILEINFO * pBlock;
	UINT dwBytes;

	if(!liVersion){
		liVersion=&_liVersion;
	}
	
	memset(liVersion,0,sizeof(LARGE_INTEGER));

	while(TRUE){
		dwHnd = 0;
		dwVersionSize=GetFileVersionInfoSize((char *)szFile, &dwHnd);
		if(!dwVersionSize){
			return g_szVersion;
		}

		pVersion=(char *)malloc(dwVersionSize*4);	// *4: for some bugs
		if(!pVersion){
			break;
		}
		my_heapchk();
		if( !GetFileVersionInfo((char*)szFile, dwHnd, dwVersionSize, pVersion) ){
			break;
		};
		my_heapchk();
		// Structure used to store enumerated languages and code pages.

		pBlock=0;
		dwBytes = sizeof(pBlock);	
		// Retrieve file description for language and code page "i". 
		VerQueryValue(pVersion,"\\",(void**)&pBlock,&dwBytes);
		my_heapchk();
		if(pBlock){
#if 1
			liVersion->HighPart=pBlock->dwFileVersionMS;
			liVersion->LowPart=pBlock->dwFileVersionLS;
#else
			liVersion->HighPart=pBlock->dwProductVersionMS;
			liVersion->LowPart=pBlock->dwProductVersionLS;
#endif
		}
		break;
	};

#ifdef __profile__
	time_mark(&t2);
	utils_debug("get_file_version costs %.3f ms.\n", time_diff(&t2,&t1)*1000);
#endif

	s = (WORD*)liVersion;
	sprintf(g_szVersion, "%d.%d.%d.%d",s[3],s[2],s[1],s[0]);
	my_heapchk();

	if(pVersion){
		free(pVersion);
	}

	return g_szVersion;
}

LIBUTILS_API char * UTILS_API 
get_file_vendorA(const char * szFile, char * szDescription,int len)
{	
	char * pVersion=0;
	DWORD dwVersionSize;
	struct LANGANDCODEPAGE {
	  WORD wLanguage;
	  WORD wCodePage;
	} *lpTranslate;
	char SubBlock[64];
	void * lpBuffer;
	UINT dwBytes;
	UINT cbTranslate;
	UINT i;
	
	*szDescription=0;
	

	dwVersionSize=GetFileVersionInfoSize((char *)szFile,0);
	if(!dwVersionSize){
		return szDescription;
	}
	pVersion=(char *)malloc(dwVersionSize);
	if(!pVersion){
		return "";
	}
	if( !GetFileVersionInfo((char*)szFile,0,dwVersionSize,pVersion) ){
		free(pVersion);
		return "";
	};
	// Read the list of languages and code pages.

	VerQueryValue(pVersion, 
				  TEXT("\\VarFileInfo\\Translation"),
				  (LPVOID*)&lpTranslate,
				  &cbTranslate);
	if(!cbTranslate){
		free(pVersion);
		return "";
	}
	// Structure used to store enumerated languages and code pages.
	lpBuffer=0;
	
	for(i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ ){
		wsprintf( SubBlock, 
				TEXT("\\StringFileInfo\\%04x%04x\\CompanyName"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage);

		if(0x0804 == lpTranslate[i].wLanguage){
			// Retrieve file description for language and code page "i". 
			VerQueryValue(pVersion, 
						SubBlock, 
						&lpBuffer, 
						&dwBytes
			);
			strncpy(szDescription,(char*)lpBuffer,len);
			szDescription[len-1]=0;
			free(pVersion);
			return szDescription;
		}			
	};		
	wsprintf( SubBlock, 
			TEXT("\\StringFileInfo\\%04x%04x\\CompanyName"),
			lpTranslate[0].wLanguage,
			lpTranslate[0].wCodePage);
	// Retrieve file description for language and code page "i". 
	VerQueryValue(pVersion, 
				SubBlock, 
				&lpBuffer, 
				&dwBytes
	);
	if(lpBuffer) {
		strncpy(szDescription,(char*)lpBuffer,len);
		szDescription[len-1]=0;
	};
	free(pVersion);
	return szDescription;
}

LIBUTILS_API char * UTILS_API 
get_file_descriptionA(const char * szFile, char * szDescription, int len)
{	
	char * pVersion=0;
	DWORD dwVersionSize;
	struct LANGANDCODEPAGE {
	  WORD wLanguage;
	  WORD wCodePage;
	} *lpTranslate;
	char SubBlock[64];
	void * lpBuffer;
	UINT dwBytes;
	UINT cbTranslate;
	UINT i;
	
	*szDescription=0;
	

	dwVersionSize=GetFileVersionInfoSize((char *)szFile,0);
	if(!dwVersionSize){
		return szDescription;
	}
	pVersion=(char *)malloc(dwVersionSize);
	if(!pVersion){
		return "";
	}
	if( !GetFileVersionInfo((char*)szFile,0,dwVersionSize,pVersion) ){
		free(pVersion);
		return "";
	};
	// Read the list of languages and code pages.

	VerQueryValue(pVersion, 
				  TEXT("\\VarFileInfo\\Translation"),
				  (LPVOID*)&lpTranslate,
				  &cbTranslate);
	if(!cbTranslate){
		free(pVersion);
		return "";
	}
	// Structure used to store enumerated languages and code pages.
	lpBuffer=0;
	
	for(i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ ){
		wsprintf( SubBlock, 
				TEXT("\\StringFileInfo\\%04x%04x\\FileDescription"),
				lpTranslate[i].wLanguage,
				lpTranslate[i].wCodePage);

		if(0x0804 == lpTranslate[i].wLanguage){
			// Retrieve file description for language and code page "i". 
			VerQueryValue(pVersion, 
						SubBlock, 
						&lpBuffer, 
						&dwBytes
			);
			strncpy(szDescription,(char*)lpBuffer,len);
			szDescription[len-1]=0;
			free(pVersion);
			return szDescription;
		}			
	};		
	wsprintf( SubBlock, 
			TEXT("\\StringFileInfo\\%04x%04x\\FileDescription"),
			lpTranslate[0].wLanguage,
			lpTranslate[0].wCodePage);
	// Retrieve file description for language and code page "i". 
	VerQueryValue(pVersion, 
				SubBlock, 
				&lpBuffer, 
				&dwBytes
	);
	if(lpBuffer) {
		strncpy(szDescription,(char*)lpBuffer,len);
		szDescription[len-1]=0;
	};
	free(pVersion);
	return szDescription;
}

int _declspec(dllexport) __stdcall destest(int a, int b, int c, int d)
{
	char buf[256];	
	char * vs;
	setup_console_for_log();
	GetModuleFileName(0,buf,sizeof(buf));	
	get_file_descriptionA("c:/winnt/system32/telnet.exe",buf,sizeof(buf));
	utils_debugA(buf);
	utils_debugA("\n");
	vs=get_file_versionA("c:/winnt/system32/telnet.exe", NULL);
	utils_debugA("Version = %s\n", vs);
	utils_getc();
	return 1;
}
#endif // defined(_WIN32)

LIBUTILS_API char * UTILS_API extract_file_nameA(char * path)
{
	char * p;
	
	p=path+strlen(path)-1;
	while(p != path){
#ifdef _WIN32
		if(*p == '\\' || *p=='/')	break;
#else
		if(*p == '/') break;		
#endif		
		p--;
	}
	return p+1;
}

LIBUTILS_API char * UTILS_API extract_file_pathA(char * path)
{
	char * p;
	p=extract_file_name(path);
	*(p-1)=0;
	return path;
}

/****************************************************************
	不懂这锅函数是作啥子的？
	看看Install Shield脚本语言里头的^操作符就知道了hoho～～
*****************************************************************/
LIBUTILS_API char * UTILS_API merge_pathA(char * part1,char * part2)
{
	switch(part1[strlen(part1)-1])
	{
	case '/':
#ifdef _WIN32
	case '\\':
#endif
		strcat(part1,part2);		
		break;
	default:
#ifdef _WIN32
		strcat(part1,"\\");
#else
		strcat(part1,"/");
#endif
		strcat(part1,part2);
	}
	return part1;
}

