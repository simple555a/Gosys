#include <string.h>
#include <f8.h>

#ifdef __VXWORKS__
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

