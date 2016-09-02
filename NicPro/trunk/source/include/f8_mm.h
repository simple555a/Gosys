#ifndef __fmm_h__
#define __fmm_h__

#include <f8_vm.h>

#if defined(_WIN32) && !defined(FCC_INVOKED)
#ifdef LIBCPMM_EXPORTS
#define F8MM_API __declspec(dllexport)
#else
#define F8MM_API __declspec(dllimport)
#endif
#else
#define F8MM_API
#endif

CDECL_BEGIN

typedef void * HF8_HEAP;

#define HEAP_T_DATA		1
#define HEAP_T_CODE		2
#define HEAP_T_SYSTEM	3
#define HEAP_T_STRING	4

F8MM_API HF8_HEAP mm_create_heap(__uint type, __uint size);
F8MM_API void	mm_destroy_heap(HF8_HEAP heap);
F8MM_API void	* mm_alloc(HF8_HEAP heap, __uint size, __uint flags);
F8MM_API void	mm_free(HF8_HEAP heap, void * mem);

CDECL_END

#endif	// dual inclusion guard

