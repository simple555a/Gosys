#include "precomp.h"

#ifdef __GNUC__
void init_kadapter(void) __attribute__((constructor));
#endif

#pragma comment (lib, "librtk.lib")
#pragma comment (lib, "libutils.lib")
#pragma comment (lib, "libcobj.lib")
#pragma comment (lib, "libzlib_shared.lib")
#pragma comment (lib, "libfkern.lib")
#pragma comment (lib, "ws2_32.lib")

void init_kadapter(void)
{
	IKAdapter_global_create();
	IFileAdapter_global_create();
	ITcpAdapter_global_create();
}

#ifdef _WIN32
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to the DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved   // reserved
)
{
	if(fdwReason == DLL_PROCESS_ATTACH){
		init_kadapter();
	}
	return TRUE;
}
#endif
