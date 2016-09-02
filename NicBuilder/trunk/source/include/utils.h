/********************************************************************	
	created:	1:3:2000   16:48
	file base:	utils
	file ext:	h
	author:	Jackie Hunter,jHuNtEr@tsinghua.org.cn

	purpose:	common procedures used system wide
  
	1) 命令行参数处理
	2) TRACE API
	3) 高精度时间处理
	4) 文件相关
	5) 注册表相关
	6) 杂项

	History :
	Jackie, 2002/5/28, rename 'SEARCHALL' to 'SEARCHALL_FILE' so cl won't grudge
		on redefined macros.
	Jackie, 2002/7/18, add crc32 support
	Jackie, 2002/7/19, add exception displaying support
*********************************************************************/
#pragma warning(disable:4996)
#ifndef __utils_h__
#define __utils_h__

#include <stdio.h>
#include <errno.h>

#ifdef _WIN32
	#ifdef LIBUTILS_EXPORTS
		#define LIBUTILS_API __declspec(dllexport)
	#else
		#define LIBUTILS_API __declspec(dllimport)
	#endif
	#define UTILS_API _stdcall
#else
	#define LIBUTILS_API
	#define LIBUTILS_API
	#define UTILS_API
#endif

// ======================= 1) 命令行参数处理API ===========================

//以下的程序是用来处理命令行参数的，我的发明
//03/11/2000

/*
	Valid arg mask includes :
	"unsigned int",
	"int",
	"float",
	"double",
	"char *",
	"unsigned char *",
	"bool",
	"tbool"
*/
typedef struct __tag_CmdLineArgA
{
    char * flag;	// will appear on the command line, such as 'help','infile',etc.,
    void * value;	// address of the variable receiving the argument value
    char * mask;	// type of the variable
    char * hint;	// hint message to display when print_usage
}CCmdLineArgA;

typedef struct __tag_CmdLineArgW
{
	wchar_t * flag;	// will appear on the command line, such as 'help','infile',etc.,
	void * value;	// address of the variable receiving the argument value
	wchar_t * mask;	// type of the variable
	wchar_t * hint;	// hint message to display when print_usage
}CCmdLineArgW;

#define BEGIN_ARGS_A  \
	CCmdLineArgA __CmdLineArgValues[]={
#define BEGIN_ARGS_W  \
	CCmdLineArgW __CmdLineArgValues[]={

#define ARG(flag,arg,mask,hint) {flag,&(arg),mask,hint},
#define END_ARGS {0,0,0,0}};

#ifdef UNICODE
#define CCmdLineArg CCmdLineArgW
#define BEGIN_ARGS BEGIN_ARGS_W
#else
#define CCmdLineArg CCmdLineArgA
#define BEGIN_ARGS BEGIN_ARGS_A
#endif

#define MAX_ARGC 128
#define MAX_LINE_LENGTH 2048

typedef struct __tag_CArgsA
{
	int argc;
	char * argv[MAX_ARGC];
	int arg_idx[MAX_ARGC];
	char args[MAX_LINE_LENGTH];
}CArgsA;

typedef struct __tag_CArgsW
{
	int argc;
	wchar_t * argv[MAX_ARGC];
	int arg_idx[MAX_ARGC];
	wchar_t args[MAX_LINE_LENGTH];
}CArgsW;

#ifdef UNICODE
#define CArgs CArgsW
#else
#define CArgs CArgsA
#endif

#ifdef __cplusplus
struct LIBUTILS_API CArgsExA{
	int		argc;
	char	**argv;
	int		*arg_idx;
	char	*args;

	CArgsExA();
	CArgsExA(const char * src, 
		const char * delimiter=",", 
		int maxArgc=MAX_ARGC,
		int maxlen=MAX_LINE_LENGTH,
		bool ignore_empty_fields = true
		);
	virtual ~CArgsExA();
	
	int parse(
		const char * src, 
		const char * delimiter=",", 
		int maxArgc=MAX_ARGC,
		int maxlen=MAX_LINE_LENGTH,
		bool ignore_empty_fields = true
		);
	inline const char * operator[](int i)
	{
		return argv[i];
	}

	int		arg_len;
	int		len;
	void destroy();
	bool create(int arglen, int len);
};

struct LIBUTILS_API CArgsExW{
	int		argc;
	wchar_t	**argv;
	int		*arg_idx;
	wchar_t	*args;

	CArgsExW();
	CArgsExW(
		const wchar_t * src, 
		const wchar_t * delimiter=L",", 
		int maxArgc=MAX_ARGC,
		int maxlen=MAX_LINE_LENGTH,
		bool ignore_empty_fields = true
		);
	virtual ~CArgsExW();

	int parse(
		const wchar_t * src, 
		const wchar_t * delimiter=L",", 
		int maxArgc=MAX_ARGC,
		int maxlen=MAX_LINE_LENGTH,
		bool ignore_empty_fields = true
		);
	inline const wchar_t * operator[](int i)
	{
		return argv[i];
	}
protected:
	int		arg_len;
	int		len;
	void destroy();
	bool create(int arglen, int len);
};
#endif

#ifdef UNICODE
#define CArgsEx CArgsExW
#else
#define CArgsEx CArgsExA
#endif

#define UTILS_INVALID_PATTERN -1
#define UTILS_HELP_NEEDED     -2
#define UTILS_SUCCESS         0

#ifdef __cplusplus
extern "C"{
#endif
	
	/*
	Parse the command line according to specified pattern
	returns :
	0 -- OK
	-1 -- invalid parameter
	-2 -- /? -? /help or -help specified
	*/
	LIBUTILS_API int UTILS_API 
	process_argsA(
		int __Argc,
		char **__Argv,
		CCmdLineArgA * __CmdLineArgValues
	);

	LIBUTILS_API int UTILS_API 
	process_argsW(
		int __Argc,
		wchar_t **__Argv,
		CCmdLineArgW * __CmdLineArgValues
		);

#ifdef __cplusplus
	LIBUTILS_API  int UTILS_API 
	process_args_exA(
		int c, 
		char **v, 
		CCmdLineArgA * CCmdLineArgValues, 
		int startArgOffset = 1, 
		int reserved = 0
		);
	LIBUTILS_API  int UTILS_API 
		process_args_exW(
		int c, 
		wchar_t **v, 
		CCmdLineArgW * CCmdLineArgValues, 
		int startArgOffset = 1, 
		int reserved = 0
		);
#else
	LIBUTILS_API  int UTILS_API 
	process_args_exA(
		int c, 
		char **v, 
		CCmdLineArg * CCmdLineArgValues, 
		int startArgOffset, 
		int reserved
		);
	LIBUTILS_API  int UTILS_API 
		process_args_exW(
		int c, 
		wchar_t **v, 
		CCmdLineArgW * CCmdLineArgValues, 
		int startArgOffset, 
		int reserved
		);
#endif

	/*
	这里定义的函数是为了解决原先parse_arg的问题
	原来的parse_arg使用了全局变量(stupid)，这样在函数重入时就会带来问题
	现在修改后的函数就不会有这个问题了。
	为了避免修改原来大量的对parse_arg的调用，补充定义了几个宏(见下面)
	注意arg的最大长度为MAX_LINE_LENGTH(当前版本2048)字节
	*/
#ifdef __cplusplus
	LIBUTILS_API int UTILS_API 
	parse_arg_exA(
		const char * arg_str,
		CArgsA *,
		char * delimiter=NULL
	);
	LIBUTILS_API int UTILS_API 
		parse_arg_exW(
		const wchar_t * arg_str,
		CArgsW *,
		wchar_t * delimiter=NULL
		);
#else
	LIBUTILS_API int UTILS_API 
	parse_arg_exA(
		const char * arg_str,
		CArgsA *,
		char * delimiter
	);
	LIBUTILS_API int UTILS_API 
		parse_arg_exW(
		const wchar_t * arg_str,
		CArgsW *,
		wchar_t * delimiter
		);
#endif
	/*
    parse_arg_ey 和 parse_arg_ex 不同，在于它将连续出现的分割符号的处理方式不同
    举例来说：
    parse_arg_ex(",,,,,",&args,",");
    得到零个参数，因为strtok将所有的‘,’都忽略了
    而parse_arg_ey(",,,,,",&args,",");
    则得到5个参数，每个参数都是空字符串
	*/
	LIBUTILS_API int UTILS_API  
	parse_arg_eyA(
		const char * arg_str,
		CArgsA * args,
		char * delimiter
	);

	LIBUTILS_API int UTILS_API  
		parse_arg_eyW(
		const wchar_t * arg_str,
		CArgsW * args,
		wchar_t * delimiter
		);

	LIBUTILS_API void UTILS_API 
	print_usageA(
		const CCmdLineArgA * __CmdLineArgValues
	);

	LIBUTILS_API void UTILS_API 
	print_usageW(
		const CCmdLineArgW * __CmdLineArgValues
	);

	LIBUTILS_API void UTILS_API 
	print_argumentsA(
		const CCmdLineArgA * __CmdLineArgValues
	);

	LIBUTILS_API void UTILS_API 
		print_argumentsW(
		const CCmdLineArgW * __CmdLineArgValues
		);

	/*
	为了向前兼容的宏定义
	*/
#define parse_arg(arg) CArgs args;parse_arg_ex(arg,&args);
#define utils_argc (args.argc)
#define utils_argv (args.argv)
	/*
	LIBUTILS_API int UTILS_API utilsArgc;
	LIBUTILS_API char * utilsArgv[MAX_ARGC];
	*/
	
	// ======================= 2) 调试输出TRACE API ===========================
	
	typedef enum TAG_MSG_LEVEL{
		msg_debug=0,
		msg_error=1,
		msg_trace=2
	}MSG_LEVEL;
	
	/*
    these constants control what to output
    the higher the level is, the more message will be
    output
    This mechanism is newer to the old 3-level(debug,error,trace) output
	*/
	
	/* no message will be output */
#define dbg_level_none      0
	/* output fatal error */
#define dbg_level_fatal     1
	/* output non-fatal msg */
#define dbg_level_error     2
	/* output tracing msg */
#define dbg_level_trace     3
	/* everything */
#define dbg_level_verbose   5
	
extern LIBUTILS_API  int (*utils_errorA)(char *, ...);
extern LIBUTILS_API  int (*utils_debugA)(char *, ...);
extern LIBUTILS_API  int (*utils_traceA)(char *, ...);

extern LIBUTILS_API  int (*utils_errorW)(wchar_t *, ...);
extern LIBUTILS_API  int (*utils_debugW)(wchar_t *, ...);
extern LIBUTILS_API  int (*utils_traceW)(wchar_t *, ...);

LIBUTILS_API void UTILS_API set_debug_stream(FILE * stream);
LIBUTILS_API void UTILS_API set_error_stream(FILE * stream);
LIBUTILS_API  void UTILS_API set_trace_stream(FILE * stream);

extern LIBUTILS_API int (UTILS_API *utils_getc)(void);

extern LIBUTILS_API int (UTILS_API *utils_putsA)(MSG_LEVEL level, char *);
extern LIBUTILS_API char * (UTILS_API *utils_getsA)(char *, int max_len);
extern LIBUTILS_API int (UTILS_API *utils_putsW)(MSG_LEVEL level, wchar_t *);
extern LIBUTILS_API wchar_t * (UTILS_API *utils_getsW)(wchar_t *, int max_len);

extern LIBUTILS_API int debug_level;

//  NOTE !!!! file pointers cannot be exchanged
//  between DLL and its clients.
//  See Microsoft Knowledge for more information
//  by Jackie Hunter, March 28, 2001
extern LIBUTILS_API  FILE * fp_trace;
extern LIBUTILS_API  FILE * fp_error;
extern LIBUTILS_API  FILE * fp_debug;

extern LIBUTILS_API int (*utils_logA)(char * fmt,...);
extern LIBUTILS_API int (*utils_logW)(wchar_t * fmt,...);
extern LIBUTILS_API int (UTILS_API *utils_log_putsA)(char *s, int level);
extern LIBUTILS_API int (UTILS_API *utils_log_putsW)(wchar_t *s, int level);

	// ======================= 3) 高精度时间 API ===========================
	
	typedef struct TAG_UTILS_TIME{
		unsigned int lo;
		unsigned int hi;
	}UTILS_TIME;
	
	extern LIBUTILS_API  double cpu_speed;
	/* returns CPU speed, in MHz */
	LIBUTILS_API  double UTILS_API cpu_calibrate(void);
	/*
    returns time as CPU ticks, requires support of high-performance counter
    in i586 or above, the built-in timer
	*/
	LIBUTILS_API  void   UTILS_API time_mark(UTILS_TIME * t);
	/* returns time as CPU ticks since last reset of RDT */
	LIBUTILS_API  double UTILS_API time_mark_d(void);
	/* return time differences, in seconds */
	LIBUTILS_API  double UTILS_API time_diff(const UTILS_TIME * _t1, const UTILS_TIME * t2);
	
	/* ---------------------------------------------------------------- */
	/* for Win-GUI application, pops up a console window to display debug info. */
	LIBUTILS_API int UTILS_API setup_console_for_log();
	/* ---------------------------------------------------------------- */
	
	// ======================= 4) 文件相关 API ===========================
	
	// dir /s support routines & types
	typedef int (UTILS_API * ENUMERATE_FILE_PROC)(
		const char * szBaseDir,
		const char * szFullPath,
		const char * szRelativePath,
		const void * pvSearchContext
		);
	
#define SEARCH_RECURSIVELY 0x00000001
#define SEARCH_DIRECTORY   0x00000002
#define SEARCH_HIDDEN      0x00000004
#define SEARCH_ALL_FILE    0x00ffffff
	
	LIBUTILS_API int UTILS_API enumerate_filesA(
		const char * szBaseDir,
		const char * szWildcards,
		ENUMERATE_FILE_PROC eProc,
		int iSearchFlags,
		const void * pvSearchContext
		);

	// shortcut for _filelength(_fileno(fopen(szFile,"rb")))
	LIBUTILS_API int UTILS_API get_file_lengthA(const char * szFile);
	// check if a specified file exists
	LIBUTILS_API int UTILS_API file_existsA(const char * szFile);
	// path must be full path
	LIBUTILS_API char * UTILS_API extract_file_nameA(char * path);
	// path must be full path, path will be truncated on return
	LIBUTILS_API char * UTILS_API extract_file_pathA(char * path);
	// don't know what's this? xfer to '^' operator in InstallShield
	LIBUTILS_API char * UTILS_API merge_pathA(char * part1, char * part2);

	// shortcut for _filelength(_fileno(fopen(szFile,"rb")))
	LIBUTILS_API int UTILS_API get_file_lengthW(const wchar_t * szFile);
	// check if a specified file exists
	LIBUTILS_API int UTILS_API file_existsW(const wchar_t * szFile);
	// path must be full path
	LIBUTILS_API wchar_t * UTILS_API extract_file_nameW(wchar_t * path);
	// path must be full path, path will be truncated on return
	LIBUTILS_API wchar_t * UTILS_API extract_file_pathW(wchar_t * path);
	// don't know what's this? xfer to '^' operator in InstallShield
	LIBUTILS_API wchar_t * UTILS_API merge_pathW(wchar_t * part1, wchar_t * part2);

#ifdef _WIN32
    LIBUTILS_API FILE * UTILS_API 
	utils_fopenA (char * file_name, char * mode);
	LIBUTILS_API FILE * UTILS_API 
	utils_fopenW (wchar_t * file_name, wchar_t * mode);
    LIBUTILS_API int UTILS_API utils_fclose(FILE *);
#else
	#define utils_fopenA  fopen
	#define utils_fopenW fopen
	#define utils_fclose fclose
#endif
	
	// ======================= 5) 注册表 API ===========================
	
	///////////////////////////////////////////////////////////////////
	// read string key value from the registry
	// input;
	//		path -->  full path of the registry entry, like
	//				"class_root\\CLSID\\{00020820-0000-0000-C000-000000000046}\\LocalServer\\LocalServer"
	//				"local_machine\\microsoft\\office\\office10\\common\\installroot"
	//			the first field is a keyword, other keywords include:
	//				"class_root"      -->      HKEY_CLASSES_ROOT
	//				"local_machine"   -->      HKEY_LOCAL_MACHINE},
	//				"current_config"  -->      HKEY_CURRENT_CONFIG},
	//				"current_user"    -->      HKEY_CURRENT_USER},
	//				"users"           -->      HKEY_USERS}
	//
	//		kname  --> key to read
	//		NOTE : you must set kname to NULL if you want to read the 'default' key value
	//
	//		buffer --> receiving buffer
	//
	//		buflen --> max size of buffer
	// returns:
	//		size of characters copied
	///////////////////////////////////////////////////////////////////
	LIBUTILS_API int UTILS_API 
	get_registry_stringA(
		const char * path,
		const char * kname,
		char * buffer,
		int buflen
	);
	// read  dword value from system registry, c.f., get_registry_string
	LIBUTILS_API int UTILS_API 
	get_registry_dwordA(
		const char * path,
		const char * kname,
		unsigned int * value
	);
	// write string to system registry, c.f., get_registry_string
	LIBUTILS_API int UTILS_API 
	set_registry_stringA(
		const char * path,
		const char * kname,
		const char * buffer
	);
	// write dword value to system registry, c.f., get_registry_string
	LIBUTILS_API int UTILS_API 
	set_registry_dwordA(
		const char * path,
		const char * kname,
		const unsigned int value
	);
	// create new key, c.f., get_registry_string
	LIBUTILS_API int UTILS_API 
	create_registry_keyA(const char * path);
	// delete key, c.f., get_registry_string, not implemented
	LIBUTILS_API int UTILS_API 
	delete_registry_keyA(const char * path);
	// delete key value, c.f., get_registry_string, not implemented
	LIBUTILS_API int UTILS_API 
	delete_registry_key_valueA(const char * path_name);

	LIBUTILS_API int UTILS_API 
	get_registry_stringW(
		const wchar_t * path,
		const wchar_t * kname,
		wchar_t * buffer,
		int buflen
		);
	// read  dword value from system registry, c.f., get_registry_string
	LIBUTILS_API int UTILS_API 
	get_registry_dwordW(
		const wchar_t * path,
		const wchar_t * kname,
		unsigned int * value
		);
	// write string to system registry, c.f., get_registry_string
	LIBUTILS_API int UTILS_API 
	set_registry_stringW(
		const wchar_t * path,
		const wchar_t * kname,
		const wchar_t * buffer
		);
	// write dword value to system registry, c.f., get_registry_string
	LIBUTILS_API int UTILS_API 
	set_registry_dwordW(
		const wchar_t * path,
		const wchar_t * kname,
		const unsigned int value
		);
	// create new key, c.f., get_registry_string
	LIBUTILS_API int UTILS_API 
	create_registry_keyW(const wchar_t * path);
	// delete key, c.f., get_registry_string, not implemented
	LIBUTILS_API int UTILS_API 
	delete_registry_keyW(const wchar_t * path);
	// delete key value, c.f., get_registry_string, not implemented
	LIBUTILS_API int UTILS_API 
	delete_registry_key_valueW(const wchar_t * path_name);


	// ====================== 6) INI API =============================
	LIBUTILS_API int UTILS_API 
	add_ini_entryA(
		const char * sysdir,
		const char * section,
		const char * key,
		const char * value
	);
	LIBUTILS_API int UTILS_API 
	delete_ini_entryA(
		const char * sysdir,
		const char * section,
		const char * key,
		const char * value
	);
	LIBUTILS_API int UTILS_API 
	check_ini_entryA(
		const char * sysdir,
		const char * section,
		const char * key,
		const char * value
	);
	
	LIBUTILS_API int UTILS_API 
		add_ini_entryW(
		const wchar_t * sysdir,
		const wchar_t * section,
		const wchar_t * key,
		const wchar_t * value
		);
	LIBUTILS_API int UTILS_API 
		remove_ini_entryW(
		const wchar_t * sysdir,
		const wchar_t * section,
		const wchar_t * key,
		const wchar_t * value
		);
	LIBUTILS_API int UTILS_API 
		check_ini_entryW(
		const wchar_t * sysdir,
		const wchar_t * section,
		const wchar_t * key,
		const wchar_t * value
		);

	LIBUTILS_API int UTILS_API utils_crc32(const char * buffer, int length);
	
	// ======================= 99) 未归类的 API ===========================
	
	LIBUTILS_API int UTILS_API rng_rand(int min,int max);

#ifdef _WIN32
	LIBUTILS_API char * UTILS_API 
	get_file_descriptionA(
		const char * szFile, 
		char * szDescription, 
		int len
	);
	LIBUTILS_API char * UTILS_API 
	get_file_vendorA(
		const char * szFile, 
		char * szDescription, 
		int len
	);

	LIBUTILS_API wchar_t * UTILS_API 
		get_file_versionW(
		const wchar_t * szFile,
		/* LARGE_INTEGER */ void * liVersion
		);
	LIBUTILS_API wchar_t * UTILS_API 
		get_file_descriptionW(
		const wchar_t * szFile, 
		wchar_t * szDescription, 
		int len
		);
	LIBUTILS_API wchar_t * UTILS_API 
		get_file_vendorW(
		const wchar_t * szFile, 
		wchar_t * szDescription, 
		int len
		);
/*
	// this function was removed to reduce memory overhead of shell32.dll	
	// Jackie, 2002/7/22
	LIBUTILS_API char * UTILS_API 
	browse_for_folder(
		HWND hParent,
		const char * szTitle
	);
*/	
	LIBUTILS_API int UTILS_API 
	iconizeWindow(void * hwnd, char *szTip, void * hIcon);
	
	LIBUTILS_API int UTILS_API 
	exename_by_pid(unsigned int pid, char buf[], int buf_len);
	/*------------------------------------------------------------------
	| Name: running_as_administrator
	| Desc: checks if user has administrator privileges
	| Notes: This function returns TRUE if the user identifier associated with 
	|   this process is a member of the the Administrators group.
	------------------------------------------------------------------*/
	LIBUTILS_API int UTILS_API running_as_administrator (void);
	
#ifdef _WINNT_ /* (下面这些函数用到的数据类型需要包含 windows.h ) */
	// get a file's version string, the file must be PE format.
	LIBUTILS_API char * UTILS_API 
		get_file_versionA(
		const char * szFile,
		/* LARGE_INTEGER */ void * liVersion
		);
	LIBUTILS_API int UTILS_API dump_context(PCONTEXT context);
	LIBUTILS_API int UTILS_API dump_exception(LPEXCEPTION_POINTERS info);
	/*
	在win32环境下，如果代码发生意外，并且你不知道如何处理（当你第一次看到这个例外时多半
	是这种情况），可以考虑只是把例外的相关信息dump出来，使用上面的两个函数和下面的这个宏
	可以搞定。
	例子：
		CHAR ×p = 0;
		__try{
			*p=5;
		}__except(utils_exception_filter()){
		}
	注意utils_execption_filter缺省的处理结果是execute_handler，如果你不想这么干
	在后面用逗号表达式追加其他处理常数就可以了。
	*/
	#define utils_exception_filter() \
	dump_exception(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER

	LIBUTILS_API BOOL UTILS_API is_winnt();
	LIBUTILS_API BOOL UTILS_API is_win98_fe();
	
#endif	// _WINNT_
#endif // _WIN32
	
	LIBUTILS_API int UTILS_API 
	check_running_instanceA(const char * szProgramName);
	
	LIBUTILS_API int UTILS_API 
	check_runningA(const char * szProgramName);

	LIBUTILS_API int UTILS_API set_runningA(const char * szProgramName);

	LIBUTILS_API int UTILS_API 
	check_running_instanceW(const wchar_t * szProgramName);

	LIBUTILS_API int UTILS_API 
	check_runningW(const wchar_t * szProgramName);

	LIBUTILS_API int UTILS_API set_runningW(const wchar_t * szProgramName);

#ifdef _WIN32
	// perror, windows version
	LIBUTILS_API int UTILS_API wperrorA(char * error_string);
	LIBUTILS_API int UTILS_API wperrorW(wchar_t * error_string);
#else
	#define wperrorA(e) perror(e)
	#define wperrorW(e) perror(e)
	#define GetLastError() errno
#endif

	// convert text strings like "3345d876" to byte array
	LIBUTILS_API int UTILS_API 
	byte_array_by_hex_string(
		const char * szArray,
		unsigned char * pbArray,
		int * length
	);

	LIBUTILS_API int UTILS_API 
	pattern_matchA(const char * pattern, const char * expression);

	LIBUTILS_API int UTILS_API 
	pattern_matchW(const wchar_t * pattern, const wchar_t * expression);

	// Debug tracing API
	LIBUTILS_API int debug_printfA(const char * s, ...);
	LIBUTILS_API int debug_printfW(const wchar_t * s, ...);

	LIBUTILS_API int UTILS_API utils_rtrimA(char * str, char space);
	LIBUTILS_API int UTILS_API utils_ltrimA(char * str, char space);
	LIBUTILS_API int UTILS_API utils_trimA(char * str, char space);

	LIBUTILS_API int UTILS_API utils_rtrimW(wchar_t * str, wchar_t space);
	LIBUTILS_API int UTILS_API utils_ltrimW(wchar_t * str, wchar_t space);
	LIBUTILS_API int UTILS_API utils_trimW(wchar_t * str, wchar_t space);

#ifndef _WIN32
#define ZeroMemory(chunk, size) memset(chunk, 0, size)
#endif
	
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
inline void utils_swap(int & a,int & b)
{
    a ^= b,b ^=a,a ^= b;
}

inline void utils_swap(double & a,double & b)
{
    double c=b;
    b=a;a=c;
}
#endif	// __cplusplus

#if defined(UNICODE) || defined(_UNICODE)
	#define  add_ini_entry  add_ini_entryW
	#define  check_ini_entry  check_ini_entryW
	#define  create_registry_key  create_registry_keyW
	#define  debug_printf  debug_printfW
	#define  delete_ini_entry  delete_ini_entryW
	#define  delete_registry_key_value  delete_registry_key_valueW
	#define  enumerate_files  enumerate_filesW
	#define  extract_file_name  extract_file_nameW
	#define  extract_file_path  extract_file_pathW
	#define  file_exists  file_existsW
	#define  get_file_description  get_file_descriptionW
	#define  get_file_length  get_file_lengthW
	#define  get_file_vendor  get_file_vendorW
	#define  get_file_vendorW  get_file_vendorWW
	#define  get_file_version  get_file_versionW
	#define  get_registry_dword  get_registry_dwordW
	#define  get_registry_string  get_registry_stringW
	#define  merge_path  merge_pathW
	#define  parse_arg_ex  parse_arg_exW
	#define  parse_arg_ey  parse_arg_eyW
	#define  pattern_match  pattern_matchW
	#define  print_arguments  print_argumentsW
	#define  print_usage  print_usageW
	#define  process_args  process_argsW
	#define  process_args_ex  process_args_exW
	#define  set_registry_dword  set_registry_dwordW
	#define  set_registry_string  set_registry_stringW
	#define  utils_debug  utils_debugW
	#define  utils_error  utils_errorW
	#define  utils_fopen  utils_fopenW
	#define  utils_gets  utils_getsW
	#define  utils_log  utils_logW
	#define  utils_log_puts  utils_log_putsW
	#define  utils_ltrim  utils_ltrimW
	#define  utils_puts  utils_putsW
	#define  utils_rtrim  utils_rtrimW
	#define  utils_trace  utils_traceW
	#define  utils_trim  utils_trimW
	#define  wperror  wperrorW
	#define check_running check_runningW
	#define check_running_instance check_running_instanceW
	#define set_running set_runningW
#else
	#define  add_ini_entry  add_ini_entryA
	#define  check_ini_entry  check_ini_entryA
	#define  create_registry_key  create_registry_keyA
	#define  debug_printf  debug_printfA
	#define  delete_ini_entry  delete_ini_entryA
	#define  delete_registry_key_value  delete_registry_key_valueA
	#define  enumerate_files  enumerate_filesA
	#define  extract_file_name  extract_file_nameA
	#define  extract_file_path  extract_file_pathA
	#define  file_exists  file_existsA
	#define  get_file_description  get_file_descriptionA
	#define  get_file_length  get_file_lengthA
	#define  get_file_vendor  get_file_vendorA
	#define  get_file_version  get_file_versionA
	#define  get_registry_dword  get_registry_dwordA
	#define  get_registry_string  get_registry_stringA
	#define  merge_path  merge_pathA
	#define  parse_arg_ex  parse_arg_exA
	#define  parse_arg_ey  parse_arg_eyA
	#define  pattern_match  pattern_matchA
	#define  print_arguments  print_argumentsA
	#define  print_usage  print_usageA
	#define  process_args  process_argsA
	#define  process_args_ex  process_args_exA
	#define  set_registry_dword  set_registry_dwordA
	#define  set_registry_string  set_registry_stringA
	#define  utils_debug  utils_debugA
	#define  utils_error  utils_errorA
	#define  utils_fopen  utils_fopenA
	#define  utils_gets  utils_getsA
	#define  utils_log  utils_logA
	#define  utils_log_puts  utils_log_putsA
	#define  utils_ltrim  utils_ltrimA
	#define  utils_puts  utils_putsA
	#define  utils_rtrim  utils_rtrimA
	#define  utils_trace  utils_traceA
	#define  utils_trim  utils_trimA
	#define  wperror  wperrorA
	#define check_running check_runningA
	#define check_running_instance check_running_instanceA
	#define set_running set_runningA
#endif

#ifndef NDEBUG
#define DEBUG_PRINTF(__x__) debug_printf __x__
#define DEBUG_PRINTFW(__x__) debug_printfW __x__
#define DEBUG_PRINTFA(__x__) debug_printfA __x__
#else
#define DEBUG_PRINTF(__x__)
#define DEBUG_PRINTFW(__x__)
#define DEBUG_PRINTFA(__x__)
#endif

#define process_args_1(c,v) process_args(c,v,__CmdLineArgValues)
#define process_args_0(c,v) process_args_ex(c,v,__CmdLineArgValues,0,0)
#define print_usage_1() print_usage(__CmdLineArgValues)
#define print_arguments_1() print_arguments(__CmdLineArgValues)
#define parse_arg_1(arg,deli) CArgs args;parse_arg_ex(arg,&args,deli);	

#endif	// __utils_h__

