#pragma warning(disable:4996)
#include "precomp.h"

LIBUTILS_API int debug_printfA(const char * s,...)
{
	char ss[MAX_LINE_LENGTH];
	int  len;
	va_list arg;
	va_start(arg,s);
	// len=wvsprintfA(ss,s,arg);
	len = _vsnprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
#ifdef _WIN32
	OutputDebugStringA(ss);
#else
	utils_debug("%s", ss);
#endif
	return len;
}

#ifdef _WIN32

LIBUTILS_API int debug_printfW(const wchar_t * s,...)
{
	wchar_t ss[MAX_LINE_LENGTH];
	int  len;
	va_list arg;
	va_start(arg,s);
	len=wvsprintfW(ss,s,arg);
	va_end(arg);
	OutputDebugStringW(ss);
	return len;
}

#endif
