#include "precomp.h"
#pragma hdrstop

void CWaitable::Duplicate(const CWaitable *item)
{
#ifdef _WIN32
	DuplicateHandle(
		GetCurrentProcess(),
		item->LockData,
		GetCurrentProcess(),
		&LockData,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS
		);
#endif
}

