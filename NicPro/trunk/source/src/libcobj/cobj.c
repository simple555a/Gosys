#include "precomp.h"
#include <stdlib.h>

#ifdef __GNUC__
void init_cobj(void) __attribute__((constructor));
#endif

#pragma comment (lib, "ws2_32.lib")

void init_cobj(void)
{
	IF8Stream_global_create();
	IMemStream_global_create();
#ifdef INCLUDE_TCP_STREAM
	ITcpStream_global_create();
#endif
#ifdef HAVE_STDIO
	IFileStream_global_create();
#ifdef _WIN32
	ITempStream_global_create();
#endif
#endif
	IStreamable_global_create();
	ITreeNode_global_create();
}

int LIBCOBJ_API _CObj_CheckPureVCall(void * f, char * fn, char * file, int line)
{
	static __bool Pure_Virtual_Function_Called = __true;
	if(!f){
#ifdef HAVE_STDIO
		fprintf(stderr, "Pure function %s called from %s, line %d.\n", 
			fn, file, line
			);
		assert(!Pure_Virtual_Function_Called);
#endif
	}
	return 0;
}

 EXTERN_C void *  __malloc__(__uint size)
{
	return (void*)malloc(size);
}

 EXTERN_C void __free__(void * buf)
{
	free(buf);
}

#ifdef _WIN32
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to the DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved   // reserved
)
{
	if(fdwReason == DLL_PROCESS_ATTACH){
		init_cobj();
	}
	return TRUE;
}
#endif

