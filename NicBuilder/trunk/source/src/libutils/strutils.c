#include "precomp.h"
#pragma hdrstop 

LIBUTILS_API int UTILS_API utils_rtrimA(char * str,char space)
{
	int i;
	if(!strlen(str)) return 0;
	i=strlen(str)-1;
	while(i>=0){
		if(space == str[i]) {
			str[i]=0;
			i--;
		}else{
			break;
		}		
	}
	return strlen(str);
}

LIBUTILS_API int UTILS_API utils_ltrimA(char * str,char space)
{
	char * p;
	int l;
	p = str;
	l = strlen(p);
	if(!space){
		return l;
	}
	while(*p == space){
		p++;
	}
	l -= p - str;
	if(p != str){
		memmove(str, p, l);
	}
	str[l] = 0;
	return l;
}

LIBUTILS_API int UTILS_API utils_trimA(char * str,char space)
{
	utils_ltrimA(str, space);
	return utils_rtrimA(str, space);
}

LIBUTILS_API int UTILS_API utils_rtrimW(wchar_t * str, wchar_t space)
{
	int i;
	if(!wcslen(str)) return 0;
	i=wcslen(str)-1;
	while(i>=0){
		if(space == str[i]) {
			str[i]=0;
			i--;
		}else{
			break;
		}
	}
	return wcslen(str);
}

LIBUTILS_API int UTILS_API utils_ltrimW(wchar_t * str, wchar_t space)
{
	wchar_t * p;
	int l;
	p = str;
	l = wcslen(p);
	if(!space){
		return l;
	}
	while(*p == space){
		p++;
	}
	l -= p - str;
	if(p != str){
		memmove(str, p, l*2);
	}
	str[l] = 0;
	return l;
}

LIBUTILS_API int UTILS_API utils_trimW(wchar_t * str, wchar_t space)
{
	utils_ltrimW(str, space);
	return utils_rtrimW(str, space);
}

