#ifndef __API_NICSYS_ARGS__
#define __API_NICSYS_ARGS__

#include "nicUtils.h"

// ======================= 1) 命令行参数处理API ===========================

//以下的程序是用来处理命令行参数
/*
Valid arg mask includes :
"nicUInt8",
"nicInt8",
"nicUInt16",
"nicInt16",
"nicUInt32",
"nicInt32",
"nicFloat",
"nicDouble",
"nicChar *",
"nicBool"
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
struct UTILS_API CArgsExA{
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

struct UTILS_API CArgsExW{
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

CDECL_BEGIN

	/*
	Parse the command line according to specified pattern
	returns :
	0 -- OK
	-1 -- invalid parameter
	-2 -- /? -? /help or -help specified
	*/
	UTILS_API int NICSYS_API 
		process_argsA(
		int __Argc,
		char **__Argv,
		CCmdLineArgA * __CmdLineArgValues
		);

	UTILS_API int NICSYS_API 
		process_argsW(
		int __Argc,
		wchar_t **__Argv,
		CCmdLineArgW * __CmdLineArgValues
		);

#ifdef __cplusplus
	UTILS_API  int NICSYS_API 
		process_args_exA(
		int c, 
		char **v, 
		CCmdLineArgA * CCmdLineArgValues, 
		int startArgOffset = 1, 
		int reserved = 0
		);
	UTILS_API  int NICSYS_API 
		process_args_exW(
		int c, 
		wchar_t **v, 
		CCmdLineArgW * CCmdLineArgValues, 
		int startArgOffset = 1, 
		int reserved = 0
		);
#else
	UTILS_API  int NICSYS_API 
		process_args_exA(
		int c, 
		char **v, 
		CCmdLineArg * CCmdLineArgValues, 
		int startArgOffset, 
		int reserved
		);
	UTILS_API  int NICSYS_API 
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
	UTILS_API int NICSYS_API 
		parse_arg_exA(
		const char * arg_str,
		CArgsA *,
		char * delimiter=nicNULL
		);
	UTILS_API int NICSYS_API 
		parse_arg_exW(
		const wchar_t * arg_str,
		CArgsW *,
		wchar_t * delimiter=nicNULL
		);
#else
	UTILS_API int NICSYS_API 
		parse_arg_exA(
		const char * arg_str,
		CArgsA *,
		char * delimiter
		);
	UTILS_API int NICSYS_API 
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
	UTILS_API int NICSYS_API  
		parse_arg_eyA(
		const char * arg_str,
		CArgsA * args,
		char * delimiter
		);

	UTILS_API int NICSYS_API  
		parse_arg_eyW(
		const wchar_t * arg_str,
		CArgsW * args,
		wchar_t * delimiter
		);
CDECL_END

#if defined(UNICODE) || defined(_UNICODE)
#define CArgs		CArgsW
#define CArgsEx		CArgsExW
#define process_args	process_argsW
#define process_args_ex	process_args_exW
#define parse_arg_ex	parse_arg_exW
#define parse_arg_ey	parse_arg_eyW
#else
#define CArgs		CArgsA
#define CArgsEx		CArgsExA
#define process_args	process_argsA
#define process_args_ex	process_args_exA
#define parse_arg_ex	parse_arg_exA
#define parse_arg_ey	parse_arg_eyA
#endif

#define process_args_1(c,v) process_args(c,v,__CmdLineArgValues)
#define process_args_0(c,v) process_args_ex(c,v,__CmdLineArgValues,0,0)
#define parse_arg_1(arg,deli) CArgs args;parse_arg_ex(arg,&args,deli);

#endif