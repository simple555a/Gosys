/*
  compatible code
*/
#include "precomp.h"

#ifdef __LINUX__
#include <sys/select.h>
#endif

#ifndef HAVE_STRUPR
char * strupr(char * s)
{
	 char *p;
	 p = s;
	 while(*p){
		  *p = toupper(*p);
		  p++;
	 }
	 return s;
}
#endif

LIBRTK_API void RTK_API rtk_sleep(__uint miliseconds)
{
#ifdef _WIN32
	Sleep(miliseconds);
#else
	 struct timeval tm;
	 tm.tv_sec = 0;
	 tm.tv_usec = miliseconds*1000;
	 select(0, NULL, NULL, NULL, &tm);
#endif
}

