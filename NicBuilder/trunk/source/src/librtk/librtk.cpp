#include "precomp.h"

#pragma comment(linker, "/base:0x76420000")

static char root[1024];
static char config[1024];
static char exedir[1024];

#ifdef UNDER_CE
#define GetEnvironmentVariable(a,b,c)	FALSE
#elif defined(__LINUX__)
static int GetEnvironmentVariable(char * name, char * buffer, int length)
{
	return false;
}
static int GetModuleFileName(int handle, char *buf, int len)
{
	// strncpy(buf, program_invocation_name, len);
	buf[len - 1] = '\0';
}
#define GetCurrentDirectory(size,buf) getcwd(buf,size)
#endif

LIBRTK_API char * RTK_API get_config_file()
{
	if(!config[0]){
		if( !GetEnvironmentVariable("pmc", config, sizeof(config)) ){
			GetModuleFileName(0, config, sizeof(config));
		}
		extract_file_path(config);		
		merge_path(config, CONFIG_FILE);
	}

	return config;
}

LIBRTK_API char * RTK_API get_exe_dir()
{
	if(!exedir[0]){
		GetModuleFileName(0, exedir, sizeof(exedir));
		extract_file_path(exedir);
		merge_path(exedir, "");
	}
	return exedir;
}

LIBRTK_API char * RTK_API get_working_dir()
{
	if(!root[0]){
		if( !GetEnvironmentVariable("pmc", root, sizeof(root)) ){
			GetModuleFileName(0, root, sizeof(root));
			extract_file_path(root);			
		}
		merge_path(root, "");
	}

	return root;
}

LIBRTK_API void RTK_API set_working_dir(const char * dir)
{
	if(dir){
		strncpy(root, dir, sizeof(root));
		root[sizeof(root) - 1] = 0;
	}else{
		GetCurrentDirectory(sizeof(root), root);
	}
	merge_path(root, "");
	strcpy(config, root);
	merge_path(config, CONFIG_FILE);
}

#if defined(_WIN32)
LIBRTK_API void RTK_API rtk_time_mark(PRTK_TIME t)
{
	GetSystemTimeAsFileTime((LPFILETIME)t);
}

/*
函数功能：计算时间差
参数说明：
          end    终止时间，100ns
		  start  开始时间，100ns
返回值： 时间差，s
*/
LIBRTK_API double RTK_API rtk_time_diff(PCRTK_TIME end, PCRTK_TIME start)
{
	double val = double((*((__i64*)end) - *((__i64*)start))) / (10*1e6);
	return val;
}
#else
LIBRTK_API void RTK_API rtk_time_mark(PRTK_TIME t)
{
	time_mark((UTILS_TIME*)t);
}

LIBRTK_API double RTK_API rtk_time_diff(PCRTK_TIME end, PCRTK_TIME start)
{
	return time_diff((UTILS_TIME*)end, (UTILS_TIME*)start);
}
#endif

/*
 *函数功能：将数组bufs中的信息装入一个片内存，并转化为void*类型
 *输入参数：数组bufs, 数组中有要处理的元素个数count
  返回值： 数组bufs各元素所组成的字符串指针，void*
 */
LIBRTK_API void * RTK_API alloc_buffer(int count, PCRTK_BUF bufs)
{
	int		i;
	void	*heap;
	int		heapsize;
	char	*p;

	for(i=0, heapsize=0; i<count; i++){
		heapsize += bufs[i].len;
	}
	heap=(void*)new char[heapsize];
	if(!heap){
		return 0;
	}
	for(i=0, p=(char*)heap; i<count; i++){
		memcpy(p, bufs[i].buf, bufs[i].len);
		p += bufs[i].len;
	}
	return heap;
}

LIBRTK_API void * RTK_API fill_buffer(int count, void *heap, PCRTK_BUF bufs)
{
	int	i;
	char	*p;

	for(i=0, p=(char*)heap; i<count; i++){
		memcpy(p, bufs[i].buf, bufs[i].len);
		p += bufs[i].len;
	}
	return heap;
}

/*
 *函数功能：统计有几组形参，并赋给数组bufs[],然后调用alloc_buffer(count, bufs);            
 *输入参数：数据包的内容有这些输入参数的buf组成。
  返回值： 所有形参buf的内容所组成的内存指针
*/
LIBRTK_API void * RTK_API alloc_buffer_v(const void *buf, int len, ...)
{
	va_list	args;
	int		count;
	RTK_BUF	bufs[128];

	if(!buf){
		return 0;
	}
	//“形参数可变的处理”
	va_start(args, len);
	bufs[0].buf = (char *)buf;
	bufs[0].len = len;
	count = 0;
	count++;
	bufs[count].buf = va_arg(args, char *);
	while(bufs[count].buf ){
		bufs[count].len = va_arg(args, int);
		count++;
		if(count == 128){
			break;
		}
		bufs[count].buf = va_arg(args, char *);
	}
	va_end(args);
	return alloc_buffer(count, bufs);
}

LIBRTK_API void * RTK_API fill_buffer_v(void *buf,  ...)
{
	va_list	args;
	int		count;
	RTK_BUF	bufs[128];

	va_start(args, buf);
	count = 0;
	bufs[count].buf = va_arg(args, char *);
	while(bufs[count].buf ){
		bufs[count].len = va_arg(args, int);
		count++;
		if(count == 128){
			break;
		}
		bufs[count].buf = va_arg(args, char *);
	}
	va_end(args);
	
	return fill_buffer(count, buf, bufs);
}

LIBRTK_API char * RTK_API rtk_alloc_string(const char * str)
{
	return (char *)alloc_buffer_v(str, strlen(str) + 1, 0);
}

LIBRTK_API void * RTK_API rtk_alloc_memory(__uint size)
{
	return new char[size];
}

LIBRTK_API void RTK_API free_buffer(void * buf)
{
	if(buf){
		delete (char *)buf;
	}
}

#if defined(_WIN32) && !defined(UNDER_CE)

LIBRTK_API void * RTK_API html_help(
    void * hwndCaller,
    const char * pszFile,
    __uint uCommand,
    __uint * dwData
    )
{
	return HtmlHelp((HWND)hwndCaller, pszFile, uCommand, (DWORD)dwData);
}

#endif
