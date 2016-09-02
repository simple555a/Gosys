#include "precomp.h"
#pragma hdrstop

#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <atlbase.h>

extern "C" HKEY find_root_key(const char * root);

// delete key, c.f., get_registry_string
// buggy code, 2002-3-29


LIBUTILS_API int UTILS_API delete_registry_key(const char * path)
{
	HKEY hRoot;
	CArgs args;
	LONG res;

	if(!path) {
		return FALSE;
	};

	parse_arg_ex((char*)path, &args, "\\");

	if(args.argc <= 2){
		return FALSE;
	}

	hRoot=find_root_key(args.argv[0]);
	if(!hRoot){
		utils_errorA("delete_registry_key : unknown root key");
		return FALSE;
	}
/*
	res=RegDeleteKey(hRoot, path+args.arg_idx[1]);
	if(ERROR_SUCCESS != res){
		wperror("RegDeleteKey");
	}
*/
	CRegKey key;
	key.Attach(hRoot);
	res = key.RecurseDeleteKey((path + args.arg_idx[1]));
	if(ERROR_SUCCESS != res){
		wperrorA("RegDeleteKey");
	}
	key.Detach();
	return ERROR_SUCCESS == res? TRUE : FALSE;
}
