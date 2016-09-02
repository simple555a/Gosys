#ifndef __kserver_h__
#define __kserver_h__

#ifdef _WIN32
#ifdef LIBKSERVER_EXPORTS
#define KSERVER_API __declspec(dllexport)
#else
#define KSERVER_API __declspec(dllimport)
#endif
#else
#define KSERVER_API
#endif

CDECL_BEGIN

KSERVER_API int kserver_main2(int c, char **v);
KSERVER_API f8_status kserver_main(const char * cmdline);
KSERVER_API __bool kserver_trivial();
KSERVER_API __bool start_kserver();
KSERVER_API __bool stop_kserver();

CDECL_END

#endif

