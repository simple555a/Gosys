#ifndef __API_NICSYS_FILEANDDIR__
#define __API_NICSYS_FILEANDDIR__
#include "nicUtils.h"

#include "nicDefs.h"
#ifdef _WIN32
#define PATH_SEPERATOR '\\'
#define PATH_SEPERATOR_STR "\\"
#define PATH_SEPERATOR_STR_W L"\\"
#else
#define PATH_SEPERATOR '/'
#define PATH_SEPERATOR_STR "/"
#endif
CDECL_BEGIN

// ======================= 4) 文件相关 API ===========================

// dir /s support routines & types
typedef int (NICSYS_API * ENUMERATE_FILE_PROC)(
	const char * szBaseDir,
	const char * szFullPath,
	const char * szRelativePath,
	const void * pvSearchContext
	);

#define SEARCH_RECURSIVELY 0x00000001
#define SEARCH_DIRECTORY   0x00000002
#define SEARCH_HIDDEN      0x00000004
#define SEARCH_ALL_FILE    0x00ffffff

UTILS_API int NICSYS_API enumerate_filesA(
	const char * szBaseDir,
	const char * szWildcards,
	ENUMERATE_FILE_PROC eProc,
	int iSearchFlags,
	const void * pvSearchContext
	);

// shortcut for _filelength(_fileno(fopen(szFile,"rb")))
UTILS_API int NICSYS_API get_file_lengthA(const char * szFile);

// check if a specified file exists
UTILS_API int NICSYS_API file_existsA(const char * szFile);

// path must be full path
UTILS_API char * NICSYS_API extract_file_nameA(char * path);

// path must be full path, path will be truncated on return
UTILS_API char * NICSYS_API extract_file_pathA(char * path);

// don't know what's this? xfer to '^' operator in InstallShield
UTILS_API char * NICSYS_API merge_pathA(char * part1, char * part2);



// shortcut for _filelength(_fileno(fopen(szFile,"rb")))
UTILS_API int NICSYS_API get_file_lengthW(const wchar_t * szFile);

// check if a specified file exists
UTILS_API int NICSYS_API file_existsW(const wchar_t * szFile);

// path must be full path
UTILS_API wchar_t * NICSYS_API extract_file_nameW(wchar_t * path);

// path must be full path, path will be truncated on return
UTILS_API wchar_t * NICSYS_API extract_file_pathW(wchar_t * path);

// don't know what's this? xfer to '^' operator in InstallShield
UTILS_API wchar_t * NICSYS_API merge_pathW(wchar_t * part1, wchar_t * part2);

#ifdef _WIN32
UTILS_API FILE * NICSYS_API 
utils_fopenA (char * file_name, char * mode);

UTILS_API FILE * NICSYS_API 
utils_fopenW (wchar_t * file_name, wchar_t * mode);

UTILS_API int NICSYS_API utils_fclose(FILE *);

#else
#define utils_fopenA  fopen
#define utils_fopenW fopen
#define utils_fclose fclose
#endif



UTILS_API char * NICSYS_API get_config_file();
UTILS_API char * NICSYS_API get_working_dir();
UTILS_API char * NICSYS_API get_exe_dir();
UTILS_API void NICSYS_API set_working_dir(const char * dir);


CDECL_END

#if defined(UNICODE) || defined(_UNICODE)
#define  enumerate_files	enumerate_filesW
#define  get_file_length	get_file_lengthW
#define  file_exists		file_existsW
#define  extract_file_name  extract_file_nameW
#define  extract_file_path  extract_file_pathW
#define  merge_path			merge_pathW

#else
#define  enumerate_files	enumerate_filesA
#define  get_file_length	get_file_lengthA
#define  file_exists		file_existsA
#define  extract_file_name  extract_file_nameA
#define  extract_file_path  extract_file_pathA
#define  merge_path			merge_pathA

#endif

#endif