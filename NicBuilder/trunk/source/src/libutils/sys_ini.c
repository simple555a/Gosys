/********************************************************************
	created:	2002/06/15
	created:	15:6:2002   20:47
	filename: 	D:\DFVS2\SRC\VC\PUBLIC\SRC\LIBUTILS\sys_ini.c
	file path:	D:\DFVS2\SRC\VC\PUBLIC\SRC\LIBUTILS
	file base:	sys_ini
	file ext:	c
	author:		Jackie Hunter, jhunter@tsinghua.org.cn
	
	purpose:	System.ini manipulation procedures

	Revision history :
	Jackie  2002/6/15  the fourth parameter of add/check/remove now become
	const.
*********************************************************************/
#pragma warning(disable:4996)
#include "precomp.h"
#pragma hdrstop

LIBUTILS_API int UTILS_API
add_ini_entryA(const char * sysdir,const char * section,const char * key, const char * _value)
{
	char large_buffer[32767];
	char new_buffer[32767];
	char * p,* q;
	int key_len=strlen(key);
	int key_added;
	
	/*
		由于下面要对这个缓冲区取小写，而传进来的参数可能是只读的（例如vc的字符串常数），so....
		Jackie, 2002/6/15
	*/
	char * value=strdup(_value);

	if( !value ){
		return 0;
	}

	DEBUG_PRINTF(("Adding ini entry : %s.[%s].%s=%s\n",sysdir,section,key,value));

	GetPrivateProfileSection(section,large_buffer,sizeof(large_buffer),sysdir);	

	// traverse the buffer, format them into a string list
	p=large_buffer;
	q=new_buffer;
	key_added=0;
	while(*p){
		if(strnicmp(p,key,key_len)){
			strcpy(q,p);
			q+=strlen(p)+1;			
		}else if('=' == p[key_len]){
			if(stricmp(p+key_len+1,value)){
				strcpy(q,p);			
				q+=strlen(p)+1;			
			}else{
				if(!key_added){
					// replace key
					q+=sprintf(q,"%s=%s",key,value)+1;
					key_added=TRUE;
				}else{
					// duplicated key found, delete it (just don't output it)
				}
			}
		}
		p+=strlen(p)+1;
	}
	if(!key_added){
		// replace key
		q+=sprintf(q,"%s=%s",key,value)+1;
	}
	q[0]=q[1]=0;
	
	WritePrivateProfileSection(section,(const char *)new_buffer,sysdir);
	free(value);
	return 1;
}

LIBUTILS_API int UTILS_API
check_ini_entryA(const char * sysdir,const char * section,const char * key, const char * _value)
{
	char large_buffer[32767];
	char new_buffer[32767];
	char * p,* q;
	int key_len=strlen(key);
	int entries=0;
	char * value;
	
	DEBUG_PRINTF(("Checking %s in %s.[%s]:", _value, sysdir, section));

	value = strdup(_value);
	if( !value ){
		DEBUG_PRINTF(("Oops, strdup failed in checkinientry.\n"));
		return 0;
	}

	GetPrivateProfileSection(section,large_buffer,sizeof(large_buffer),sysdir);	

	strlwr(value);
	// traverse the buffer, format them into a string list
	p=large_buffer;
	q=new_buffer;
	while(*p){
		
		// DEBUG_PRINTF(("%s\n", p));

		if(strnicmp(p,key,key_len)){			
			strcpy(q,p);	
			q+=strlen(p)+1;
		}else if('=' == p[key_len]){
			strlwr(p);
			if(strstr(p+key_len+1,value)){
				entries++;
			}
		}
		p+=strlen(p)+1;
	}
	if(!entries){
		DEBUG_PRINTF(("No entries found\n"));
	}else{
		DEBUG_PRINTF(("%d entries found\n",entries));
	}
	
	free(value);

	return entries;
}

LIBUTILS_API int UTILS_API
delete_ini_entryA(const char * sysdir,const char * section,const char * key, const char * _value)
{
	char large_buffer[32767];
	char new_buffer[32767];
	char * p,* q;
	int key_len=strlen(key);
	char * value;

	DEBUG_PRINTF(("Removing ini entry : %s.[%s].%s=%s\n", sysdir, section, key, _value));

	value = strdup(_value);
	if( !value ){
		DEBUG_PRINTF(("Oops, strdup failed in removeinientry.\n"));
		return 0;
	}

	GetPrivateProfileSection(section,large_buffer,sizeof(large_buffer),sysdir);	
	
	// traverse the buffer, format them into a string list
	p=large_buffer;
	q=new_buffer;
	strlwr(value);
	while(*p){		
		if(strnicmp(p,key,key_len)){
			strcpy(q,p);
			q+=strlen(p)+1;
		}else if('=' == p[key_len]){
			strlwr(p);
			if(strstr(p+key_len+1,value)){
				DEBUG_PRINTF(("Found %s\n",p));
				// key found, delete it (just don't output it)
			}else{
				strcpy(q,p);
				q+=strlen(p)+1;				
			}
		}
		p+=strlen(p)+1;
	}
	q[0]=q[1]=0;
	
	WritePrivateProfileSection(section,(const char *)new_buffer,sysdir);

	free(value);

	return 1;
}