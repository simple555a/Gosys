#include "precomp.h"

static __uint IF8Stream_tell(IF8Stream * s)
{
	return __vcall__(s, seek, (s, 0, SEEK_CUR));
}

__constructor__(IF8Stream)
{
	if(__this){
		__touch__();
	}
	return __this;
}

__destructor__(IF8Stream)
{
	//
	return __this;
}

__global_constructor__(IF8Stream)
{
	__set_vfunc__(IF8Stream, tell);
}

__define_vtbl__(IF8Stream)
__define_vtbl_end__(IF8Stream)
