/*
	Trivial file-based F-Kernel Adapter
	by J.Hunter, 2003/9/18
*/
#include "precomp.h"
#pragma hdrstop

static f8_status IFileAdapter_online(IKAdapter * _this, const char * url, __uint timeout)
{
	_this->flags |= ADAPTER_F_ONLINE;
	return F8_SUCCESS;
}

/*
	stop scheduler
*/
static f8_status IFileAdapter_offline(IKAdapter * _this)
{
	_this->flags &= ~ADAPTER_F_ONLINE;
	return F8_SUCCESS;
}

__define_vtbl__(IFileAdapter)
__define_vtbl_end__(IFileAdapter)

__global_constructor__(IFileAdapter)
{
	__v_copy__(IFileAdapter, IKAdapter);
	__override__(IFileAdapter, IKAdapter, online);
	__override__(IFileAdapter, IKAdapter, offline);
}

__constructor__(IFileAdapter)
{
	IKAdapter *b;
	if(__this){
		__touch__();
		__create_base__(IFileAdapter, IKAdapter, __this);
		b = __ucast__(IFileAdapter, IKAdapter, __this);
		b->type = AT_FILE_ADAPTER;
	}
	return __this;
}

__destructor__(IFileAdapter)
{
	IKAdapter * ka;
	if(__this){
		ka = __ucast__(IFileAdapter, IKAdapter, __this);
		__destroy_base__(IFileAdapter, IKAdapter, __this);
	}
	return __this;
}
