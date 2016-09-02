/*
  compatible code
*/
#include "precomp.h"

#ifdef _WIN32
static __declspec(thread) int err = RTK_NO_ERROR;
#else
static int err = RTK_NO_ERROR;
#endif

LIBRTK_API int RTK_API rtk_set_last_error(int e)
{
	err = e;
	return err;
}

LIBRTK_API int RTK_API rtk_get_last_error()
{
	return err;
}
