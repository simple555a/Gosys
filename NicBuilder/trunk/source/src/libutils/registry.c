/********************************************************************
	created:	2001/10/23
	created:	23:10:2001   11:51
	filename: 	E:\eguard\libutils\registry.cpp
	file path:	E:\eguard\libutils
	file base:	registry
	file ext:	cpp
	author:		Jackie Hunter
	
	purpose:	注册表支持函数
	2002/7/24, Jackie, fixed a bug in delete_registry_key_value
*********************************************************************/
#pragma warning(disable:4996)
#include "precomp.h"
#pragma hdrstop 

#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#include <stdarg.h>

const static struct {
	char * name;
	HKEY key;		
}key_map[]={
	{"class_root",HKEY_CLASSES_ROOT},
	{"HKCR",HKEY_CLASSES_ROOT},
	{"HKEY_CLASSES_ROOT",HKEY_CLASSES_ROOT},

	{"local_machine",HKEY_LOCAL_MACHINE},
	{"HKLM",HKEY_LOCAL_MACHINE},
	{"HKEY_LOCAL_MACHINE",HKEY_LOCAL_MACHINE},

	{"current_config",HKEY_CURRENT_CONFIG},
	{"HKCC",HKEY_CURRENT_CONFIG},
	{"HKEY_CURRENT_CONFIG",HKEY_CURRENT_CONFIG},
	

	{"current_user",HKEY_CURRENT_USER},
	{"HKCU",HKEY_CURRENT_USER},
	{"HKEY_CURRENT_USER",HKEY_CURRENT_USER},	

	{"HKEY_USERS",HKEY_USERS},
	{"HKUS",HKEY_USERS},
	{"users",HKEY_USERS},	
};

HKEY find_root_key(const char * root)
{
	int i;

	for(i=0;i<sizeof(key_map)/sizeof(key_map[0]);i++){
		if(!strnicmp(key_map[i].name,root,strlen(key_map[i].name))){
			return key_map[i].key;
		};
	};
	return NULL;
}

static HKEY
find_direct_parent(const char * path)
{
	HKEY hRoot,hKey;
	CArgs args;
	LONG res;

	if(!path) {
		return FALSE;
	};

	parse_arg_exA((char*)path,&args,"\\");

	if(args.argc <= 2){
		return FALSE;
	}

	hRoot=find_root_key(args.argv[0]);
	if(!hRoot){
		utils_errorA("find_direct_parent: %s is unknown root key",args.argv[0]);
		return FALSE;
	}

	res=RegOpenKeyEx(hRoot,path+args.arg_idx[1],0,0,&hKey);
	if(ERROR_SUCCESS != res){
		wperrorA("RegOpenKeyEx");		
		return 0;
	}

	return hKey;
}
///////////////////////////////////////////////////////////////////
// create new registry key
// input;
//		path -->  full path of the registry entry, like
//				"class_root\\CLSID\\{00020820-0000-0000-C000-000000000046}\\LocalServer\\LocalServer"
//				"local_machine\\microsoft\\office\\office10\\common\\installroot"
//				 where the first field is a keyword, other keywords include:
//				"class_root"      -->      HKEY_CLASSES_ROOT
//				"local_machine"   -->      HKEY_LOCAL_MACHINE},
//				"current_config"  -->      HKEY_CURRENT_CONFIG},
//				"current_user"    -->      HKEY_CURRENT_USER},
//				"users"           -->      HKEY_USERS}
// returns:
//		int
///////////////////////////////////////////////////////////////////
LIBUTILS_API int UTILS_API create_registry_keyA(const char * path)
{
	HKEY hRoot,hSub;
	CArgs args;
	int i;
	DWORD disp;

	if(!path) {
		return FALSE;
	};

	parse_arg_exA((char*)path,&args,"\\");

	if(args.argc <= 2){
		return 0;
	}

	hRoot=find_root_key(args.argv[0]);
	if(!hRoot){
		return 0;
	}

	// recursively create the subkey
	for(i=1;i<args.argc;i++){
		// utils_trace("CreateKey %s ... \n",args.argv[i]);
		if( ERROR_SUCCESS != 
			RegCreateKeyEx(
				hRoot,
				args.argv[i],
				0,
				NULL,
				REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS,
				NULL,
				&hSub,
				&disp
				)
		){
			RegCloseKey(hRoot);
			return FALSE;
		}
		RegCloseKey(hRoot);
		hRoot=hSub;
	}
	
	RegCloseKey(hRoot);

	return TRUE;
}

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
LIBUTILS_API int UTILS_API get_registry_stringA(
	const char * path,const char * kname,char * buffer,int buflen)
{
	HKEY hRoot,hSub;
	DWORD type;
	CArgs args;
	int i;

	*buffer=0;

	if(!path || !buffer) {
		return 0;
	};

	parse_arg_exA((char*)path,&args,"\\");

	if(args.argc <= 1){
		return 0;
	}

	hRoot=find_root_key(args.argv[0]);

	if(!hRoot){
		return 0;
	}
	// recursively fetch the subkey
	for(i=1;i<args.argc;i++){
		if( ERROR_SUCCESS != RegOpenKeyEx(hRoot,args.argv[i],0,KEY_ALL_ACCESS,&hSub) ){
			RegCloseKey(hRoot);
			return 0;
		};
		RegCloseKey(hRoot);
		hRoot=hSub;
	}
	
	
	RegQueryValueEx(hRoot,kname,0,&type,(unsigned char*)buffer,(DWORD*)&buflen);

	RegCloseKey(hRoot);

	return strlen(buffer);
};

///////////////////////////////////////////////////////////////////
// read dword key value from the registry
// input;
//		path -->  full path of the registry entry, like
//				"class_root\\CLSID\\{00020820-0000-0000-C000-000000000046}\\LocalServer\\LocalServer"
//				"local_machine\\microsoft\\office\\office10\\common\\installroot"
//				 where the first field is a keyword, other keywords include:
//				"class_root"      -->      HKEY_CLASSES_ROOT
//				"local_machine"   -->      HKEY_LOCAL_MACHINE},
//				"current_config"  -->      HKEY_CURRENT_CONFIG},
//				"current_user"    -->      HKEY_CURRENT_USER},
//				"users"           -->      HKEY_USERS}
//		kname --> key to read
//		value --> receiving buffer
// returns:
//		int
///////////////////////////////////////////////////////////////////
LIBUTILS_API int UTILS_API get_registry_dwordA(
	const char * path,const char * kname,unsigned int * value)
{
	HKEY hRoot,hSub;
	DWORD type;
	CArgs args;
	int i;
	DWORD buflen=sizeof(DWORD);
	LONG res;

	if(!path || !value) {
		return FALSE;
	};

	parse_arg_exA((char*)path,&args,"\\");

	if(args.argc <= 1){
		return FALSE;
	}

	hRoot=find_root_key(args.argv[0]);
	if(!hRoot){
		return 0;
	}

	// recursively fetch the subkey
	for(i=1;i<args.argc;i++){
		if( ERROR_SUCCESS != RegOpenKeyEx(hRoot,args.argv[i],0,KEY_ALL_ACCESS,&hSub) ){
			RegCloseKey(hRoot);
			return 0;
		};
		RegCloseKey(hRoot);
		hRoot=hSub;
	}
	
	res = RegQueryValueEx(hRoot,kname,0,&type,(unsigned char*)value,(DWORD*)&buflen);
	if(ERROR_SUCCESS != res)
	{
		wperrorA("get_registry_dword -- RegQueryValueEx");
	}

	RegCloseKey(hRoot);

	return res==ERROR_SUCCESS? TRUE : FALSE;
};

///////////////////////////////////////////////////////////////////
// write string key value to the registry
// input;
//		path -->  full path of the registry entry, like
//				"class_root\\CLSID\\{00020820-0000-0000-C000-000000000046}\\LocalServer\\LocalServer"
//				"local_machine\\microsoft\\office\\office10\\common\\installroot"
//				 where the first field is a keyword, other keywords include:
//				"class_root"      -->      HKEY_CLASSES_ROOT
//				"local_machine"   -->      HKEY_LOCAL_MACHINE},
//				"current_config"  -->      HKEY_CURRENT_CONFIG},
//				"current_user"    -->      HKEY_CURRENT_USER},
//				"users"           -->      HKEY_USERS}
//		kname  --> key to read
//		buffer --> string to write
// returns:
//		int
///////////////////////////////////////////////////////////////////
LIBUTILS_API int UTILS_API set_registry_stringA(
	const char * path,const char * kname,const char * buffer)
{
	HKEY hRoot,hSub;
	CArgs args;
	int buflen;
	LONG res;

	if(!path || !buffer) {
		return FALSE;
	};

	parse_arg_exA((char*)path,&args,"\\");

	if(args.argc <= 2){
		return FALSE;
	}

	hRoot=find_root_key(args.argv[0]);
	if(!hRoot){
		utils_errorA("set_registry_string : unknown root key");
		return FALSE;
	}

	// utils_errorA("Openkey %s \n",path+args.arg_idx[1]);
	if( ERROR_SUCCESS != RegOpenKeyEx(hRoot,path+args.arg_idx[1],0,KEY_ALL_ACCESS,&hSub) ){
		utils_errorA("Openkey %s error\n",path+args.arg_idx[1]);
		wperrorA("RegOpenKeyEx");
		return FALSE;
	};
	
	RegCloseKey(hRoot);

	buflen=strlen(buffer)+1;
	res=RegSetValueEx(hSub,kname,0,REG_SZ,(unsigned char*)buffer,buflen+1);
	if(ERROR_SUCCESS != res)
	{
		wperrorA("set_registry_string -- RegSetValueEx");
	}
	RegCloseKey(hSub);	

	return res==ERROR_SUCCESS? TRUE : FALSE;
}

///////////////////////////////////////////////////////////////////
// write dword key value to the registry
// input;
//		path -->  full path of the registry entry, like
//				"class_root\\CLSID\\{00020820-0000-0000-C000-000000000046}\\LocalServer\\LocalServer"
//				"local_machine\\microsoft\\office\\office10\\common\\installroot"
//				 where the first field is a keyword, other keywords include:
//				"class_root"      -->      HKEY_CLASSES_ROOT
//				"local_machine"   -->      HKEY_LOCAL_MACHINE},
//				"current_config"  -->      HKEY_CURRENT_CONFIG},
//				"current_user"    -->      HKEY_CURRENT_USER},
//				"users"           -->      HKEY_USERS}
//		kname --> key to read
//		value --> key value 
// returns:
//		int
///////////////////////////////////////////////////////////////////
LIBUTILS_API int UTILS_API set_registry_dwordA(
	const char * path,const char * kname,const unsigned int value)
{
	HKEY hRoot,hSub;
	CArgs args;
	LONG res;

	if(!path || !value) {
		return FALSE;
	};

	parse_arg_exA((char*)path,&args,"\\");

	if(args.argc <= 2){
		return FALSE;
	}

	hRoot=find_root_key(args.argv[0]);
	if(!hRoot){
		utils_errorA("set_registry_dword : unknown root key");
		return FALSE;
	}

	// utils_errorA("Openkey %s \n",path+args.arg_idx[1]);
	if( ERROR_SUCCESS != RegOpenKeyEx(hRoot,path+args.arg_idx[1],0,KEY_ALL_ACCESS,&hSub) ){
		utils_errorA("Openkey %s error\n",path+args.arg_idx[1]);		
		return FALSE;
	};
	
	RegCloseKey(hRoot);

	res=RegSetValueEx(hSub,kname,0,REG_DWORD,(unsigned char*)&value,sizeof(DWORD));
	RegCloseKey(hRoot);	

	return res==ERROR_SUCCESS? TRUE : FALSE;
}


// delete key value, c.f., get_registry_string, not implemented
// buggy code, Jackie 2002-3-29
LIBUTILS_API int UTILS_API delete_registry_key_valueA(const char * path)
{
	HKEY hRoot, hKey;
	CArgs args;
	LONG res;
	int i;

	if(!path) {
		return FALSE;
	};

	parse_arg_exA((char*)path,&args,"\\");

	if(args.argc <= 2){
		return FALSE;
	}

	hRoot=find_root_key(args.argv[0]);
	if(!hRoot){
		utils_errorA("delete_registry_key : %s is unknown root key",args.argv[0]);
		return FALSE;
	}

	hKey=hRoot;
	// find the nearest parent key
	for(i=1; i<args.argc-1; i++){
		res = RegOpenKey(hKey, args.argv[i], &hKey);
		if( ERROR_SUCCESS != res){
			wperrorA("RegOpenKey");
			return FALSE;
		}
	}
	res=RegDeleteValue(hKey,path+args.arg_idx[i]);
	if(ERROR_SUCCESS != res){
		wperrorA("RegDeleteValue");
	}
	if(hRoot != hKey){
		RegCloseKey( hKey );
	}
	return ERROR_SUCCESS == res? TRUE : FALSE;
}