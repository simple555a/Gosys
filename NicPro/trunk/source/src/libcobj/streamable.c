#include "precomp.h"

__define_vtbl__(IStreamable)
__define_vtbl_end__(IStreamable)

__constructor__(IStreamable)
{
	if(__this){
		__touch__();
	}
	return __this;
}

__destructor__(IStreamable)
{
	return __this;
}

__global_constructor__(IStreamable)
{
}
