#include "precomp.h"

HF8_HEAP g_hBlkHeap;

void init_fbin(void)
{
	g_hBlkHeap = mm_create_heap(HEAP_T_DATA, BLK_HEAP_SIZE);

	IBlk_global_create();
	IBBlk_global_create();
	ICBlk_global_create();
}

void uninit_fbin(void)
{
	mm_destroy_heap(g_hBlkHeap);
}

#ifdef _WIN32
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to the DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved   // reserved
)
{
	if(fdwReason == DLL_PROCESS_ATTACH){
		init_fbin();
	}else if(fdwReason == DLL_PROCESS_DETACH){
		uninit_fbin();
	}
	return TRUE;
}
#endif
