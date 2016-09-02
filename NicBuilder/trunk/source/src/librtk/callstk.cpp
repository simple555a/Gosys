#include "precomp.h"
#pragma hdrstop

#include <callstk.h>

#define MAX_DEPTH 32

struct call_stack_t * get_call_stack()
{
	int regEbp;

	// get stack base and limit

	// get call depth
#ifdef _MSC_VER
	__asm{
		mov regEbp, ebp
	};
		
#else
#error "source incomplete, revise me"
#endif
	// allocate memory
	// copy stack frame information

	return 0;
}

