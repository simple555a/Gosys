#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <stdlib.h>
#include <f8.h>

#ifdef _WIN32
F8MM_API HF8_HEAP mm_create_heap(__uint type, __uint size)
{
	// return HeapCreate(0, size, size);
	return 0;
}

F8MM_API void	mm_destroy_heap(HF8_HEAP heap)
{
	// HeapDestroy(heap);
}

F8MM_API void	* mm_alloc(HF8_HEAP heap, __uint size, __uint flags)
{
	// return HeapAlloc(heap, 0, size);
	return malloc(size);
}

F8MM_API void	mm_free(HF8_HEAP heap, void * mem)
{
	// HeapFree(heap, 0, mem);
	free(mem);
}

F8MM_API HF8_HEAP mm_create_x_heap(__uint type, __uint size)
{
	return 0;
}

F8MM_API void mm_destroy_x_heap(HF8_HEAP heap)
{
}

#else
F8MM_API HF8_HEAP mm_create_heap(__uint type, __uint size)
{
	return (HF8_HEAP)1;
}

F8MM_API void	mm_destroy_heap(HF8_HEAP heap)
{
}

F8MM_API void	* mm_alloc(HF8_HEAP heap, __uint size, __uint flags)
{
	return malloc(size);
}

F8MM_API void	mm_free(HF8_HEAP heap, void * mem)
{
	free(mem);
}

F8MM_API HF8_HEAP mm_create_x_heap(__uint type, __uint size)
{
	return 0;
}

F8MM_API void mm_destroy_x_heap(HF8_HEAP heap)
{
}
#endif

