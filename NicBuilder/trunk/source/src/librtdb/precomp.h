#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable:4786)
#endif

#include <atlbase.h>

// #define RTK_USE_HASHMAP
// #define RTK_USE_SGI_STL

namespace std{
}

using namespace std;

#ifdef _WIN32
#include <process.h>
#endif
#include <string>
#include <ctype.h>

#include <utils.h>
#include <librtk.h>
#include <librtdb.h>

#include <object.h>

#include "tagtable.h"
#include "sysdb_object.h"

#define MAX_HANDLERS 8

extern PRTDB_DISPATCH_TABLE	g_Handlers[MAX_HANDLERS];
extern __int				g_iHandlers;

#define FIRE_EVENT(__field__, __parameters__)\
do{\
	for(int i=0; i<g_iHandlers; i++){\
		if(g_Handlers[i] -> __field__){\
			g_Handlers[i] -> __field__ __parameters__;\
		}\
	}\
}while(0)

extern __uint g_dwMaxTags;
extern __uint g_dwTags;
