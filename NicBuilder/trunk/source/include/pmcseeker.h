#ifndef __pmc_seeker_h
#define __pmc_seeker_h

#include <librtdb.h>

#ifdef __cplusplus
extern "C" {
#endif

__bool PMC_API Discover();
__bool PMC_API GetFirstTag(PRTK_TAG tag);
__bool PMC_API GetNextTag(PRTK_TAG tag);
void PMC_API LoadSeeker(HWND hWnd);
void PMC_API UnloadSeeker();

#ifdef __cplusplus
}
#endif

#endif
