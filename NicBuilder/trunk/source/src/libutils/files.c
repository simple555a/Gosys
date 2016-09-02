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
#define PATH_SEPERATOR '\\'
#define PATH_SEPERATOR_STR "\\"
#define PATH_SEPERATOR_STR_W L"\\"
#else
#define PATH_SEPERATOR '/'
#define PATH_SEPERATOR_STR "/"
#endif

#ifdef _WIN32
#pragma comment (lib,"version.lib")
#endif

#ifdef _WIN32
static int InternalEnumerateFiles(
								  const char * szTopDir,
								  const char * szFullTopDir,
								  const char * szBaseDir,
								  const char * szWildcards,
								  ENUMERATE_FILE_PROC eProc,
								  int iSearchFlags,
								  const void * pvSearchContext,
								  BOOL * bContinueSearch
								  )
{
	DWORD iHitCount=0;
	char szFiles[_MAX_PATH*2];
	WIN32_FIND_DATA ffFindFileData;
	HANDLE hFindFile;
	char szFullPath[_MAX_PATH];
	char * szRelativePath;
	char szSubDir[_MAX_PATH*2];

	/* first of all we find the files directly under the base directory */

	DEBUG_PRINTF(("%s--%s--%s\n", szTopDir, szFullTopDir, szBaseDir));

	strcpy(szFiles,szBaseDir);
	merge_path(szFiles,(char*)szWildcards);
	hFindFile=FindFirstFile(szFiles,&ffFindFileData);
	if(INVALID_HANDLE_VALUE != hFindFile)
	{
		do
		{
			if(ffFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(!(SEARCH_DIRECTORY & iSearchFlags))
				{
					continue;
				}
				if( !strcmp(ffFindFileData.cFileName,".") ||
					!strcmp(ffFindFileData.cFileName,"..")){
						continue;
					}
			}

			strcpy(szFullPath,szBaseDir);
			merge_path(szFullPath,ffFindFileData.cFileName);
			szRelativePath = szFullPath+strlen(szFullTopDir)+1;			
			iHitCount++;
			if( !eProc(szTopDir,szFullPath,szRelativePath,pvSearchContext) ){
				*bContinueSearch=FALSE;
				return iHitCount;
			}
		}while(FindNextFile(hFindFile,&ffFindFileData));

		FindClose(hFindFile);
	};

	// find sub directories
	if(SEARCH_RECURSIVELY & iSearchFlags){
		/* Ok, now recursely find in the sub-directories */

		strcpy(szFiles,szBaseDir);
		merge_path(szFiles,"*.*");
		hFindFile=FindFirstFile(szFiles,&ffFindFileData);
		if(INVALID_HANDLE_VALUE == hFindFile)
		{
			return iHitCount;
		}	
		do
		{
			if(!(ffFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				continue;
			}
			if( !strcmp(ffFindFileData.cFileName,".") ||
				!strcmp(ffFindFileData.cFileName,"..")){
					continue;
				}
				strcpy(szSubDir,szBaseDir);
				merge_path(szSubDir,ffFindFileData.cFileName);
				iHitCount += InternalEnumerateFiles(
					szTopDir,
					szFullTopDir,
					szSubDir,
					szWildcards,
					eProc,
					iSearchFlags,
					pvSearchContext,
					bContinueSearch);
				if(! *bContinueSearch){
					break;
				}
		}while(FindNextFile(hFindFile,&ffFindFileData));

		FindClose(hFindFile);
	};

	return iHitCount;
};

/******************************************************************
在指定的目录中递归的查找文件名匹配指定模式的文件。
输入：
输出：
返回：
1/0规则
******************************************************************/
LIBUTILS_API int UTILS_API 
enumerate_filesA(
				const char * szBaseDir,
				const char * szWildcards,
				ENUMERATE_FILE_PROC eProc,
				int iSearchFlags,
				const void * pvSearchContext)
{
	char szFullTopDir[_MAX_PATH];
	char * szFilePart;
	BOOL bContinueSearch=TRUE;
	int l;

	if(!szBaseDir || !szWildcards || !eProc){
		SetLastError(87);
		return 0;
	};

	GetFullPathName(szBaseDir,sizeof(szFullTopDir),szFullTopDir,&szFilePart);
	l = strlen(szFullTopDir);
	if ('\\' == szFullTopDir[l-1] || '/' == szFullTopDir[l-1]){
		szFullTopDir[l-1] = 0;
	}

	return InternalEnumerateFiles(
		szBaseDir,
		szFullTopDir,
		szFullTopDir,
		szWildcards,
		eProc,
		iSearchFlags,
		pvSearchContext,
		&bContinueSearch
		);
}
#endif  // defined(_WIN32)

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
wchar_t g_szVersion[32]=L"";

LIBUTILS_API wchar_t * UTILS_API 
get_file_versionW(const wchar_t * szFile, LARGE_INTEGER * liVersion)
{	
	return NULL;
}

LIBUTILS_API wchar_t * UTILS_API 
get_file_vendorW(const wchar_t * szFile, wchar_t * szDescription,int len)
{	
	return NULL;
}

LIBUTILS_API wchar_t * UTILS_API 
get_file_descriptionW(const wchar_t * szFile, wchar_t * szDescription, int len)
{	
	return NULL;
}

#endif // defined(_WIN32)

LIBUTILS_API wchar_t * UTILS_API extract_file_nameW(wchar_t * path)
{
	wchar_t * p;
	
	p=path+wcslen(path)-1;
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

LIBUTILS_API wchar_t * UTILS_API extract_file_pathW(wchar_t * path)
{
	wchar_t * p;
	p=extract_file_nameW(path);
	*(p-1)=0;
	return path;
}

/****************************************************************
	不懂这锅函数是作啥子的？
	看看Install Shield脚本语言里头的^操作符就知道了hoho～～
*****************************************************************/
LIBUTILS_API wchar_t * UTILS_API merge_pathW(wchar_t * part1,wchar_t * part2)
{
	switch(part1[wcslen(part1)-1])
	{
	case '/':
#ifdef _WIN32
	case '\\':
#endif
		wcscat(part1,part2);		
		break;
	default:
#ifdef _WIN32
		wcscat(part1, L"\\");
#else
		wcscat(part1, L"/");
#endif
		wcscat(part1, part2);
	}
	return part1;
}

LIBUTILS_API int UTILS_API get_file_lengthA(const char * szFile)
{	
	struct stat s;
	if( stat(szFile, &s) ){
		 return 0;
	}
	return s.st_size;
}

LIBUTILS_API int UTILS_API file_existsA(const char * szFile)
{
	 struct stat s;
	 return !stat(szFile, &s);
}

#ifdef _WIN32
LIBUTILS_API int UTILS_API get_file_lengthW(const wchar_t * szFile)
{	
	struct _stat s;
	if( _wstat(szFile, &s) ){
		 return 0;
	}
	return s.st_size;
}

LIBUTILS_API int UTILS_API file_existsW(const wchar_t * szFile)
{
	 struct _stat s;
	 return !_wstat(szFile, &s);
}
#endif

