#ifndef __f8_cc_h__
#define __f8_cc_h__

#ifdef _WIN32
#ifdef LIBFCC_EXPORTS
#define FCC_API __declspec(dllexport)
#else
#define FCC_API __declspec(dllimport)
#endif
#else
#define FCC_API
#endif

#include <f8_binary.h>

CDECL_BEGIN

FCC_API int fcc_main_v(int c, char **v);
FCC_API int fcc_main(const char * cmdline);
FCC_API int blk_disassembly(IBlk * blk);
FCC_API int blk_disassembly_b(IBlk * blk, char * buf, int len);
FCC_API int blk_disassembly_ex(
	IBlk * blk,
	f8_bool bRecursively
	);

CDECL_END

#endif

