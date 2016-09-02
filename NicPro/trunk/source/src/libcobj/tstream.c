#include "precomp.h"

#ifdef _WIN32

static ITempStream_get(IF8Stream * _s, void * buf, __uint bytes)
{
	__uint read = 0;
	ITempStream * s = __dcast__(IF8Stream, ITempStream, _s);
	ReadFile(s->handle, buf, bytes, &read, 0);
	return read;
}

static __uint ITempStream_put(IF8Stream * _s, void * buf, __uint bytes)
{
	__uint cb;
	ITempStream * s = __dcast__(IF8Stream, ITempStream, _s);
	WriteFile(s->handle, buf, bytes, &cb, 0);
	return cb;
}

static __uint ITempStream_seek(IF8Stream * _s, long offset, int whence)
{
	__uint m;
	ITempStream * s;

	switch(whence){
	case SEEK_SET:
		m = FILE_BEGIN;
	break;
	case SEEK_CUR:
		m = FILE_CURRENT;
	break;
	case SEEK_END:
		m = FILE_END;
	break;
	default:
		return -1;
	}
	s = __dcast__(IF8Stream, ITempStream, _s);
	return SetFilePointer(s->handle, offset, 0, m);
}

static __uint ITempStream_tell(IF8Stream * _s)
{
	ITempStream * s = __dcast__(IF8Stream, ITempStream, _s);
	return SetFilePointer(s->handle, 0, 0, FILE_CURRENT);
}

static __uint ITempStream_left(IF8Stream * _s)
{
	ITempStream * s = __dcast__(IF8Stream, ITempStream, _s);
	int cur ;
	int total;
	cur = ITempStream_tell(_s);
	total = GetFileSize(s->handle, 0);
	return total - cur;
}

static __bool ITempStream_eos(IF8Stream * _s)
{
	return ITempStream_left(_s) == 0? __true : __false;
}

static __uint ITempStream_align(IF8Stream * _s, int b)
{
	ITempStream * s;
	static char align[16];
	int i;
	__uint cb;

	s = __dcast__(IF8Stream, ITempStream, _s);
	i = ITempStream_tell(_s) % b;
	if(i){
		b = b - i;
	}else{
		b = 0;
	}
	// a number of b bytes should be written to the file
	while(b >= 16){
		WriteFile(s->handle, align, 16, &cb, 0);
		b -= 16;
	}
	WriteFile(s->handle, align, b, &cb, 0);
	return ITempStream_tell(_s);
}

__constructor__(ITempStream)
{
	char	tp[MAX_PATH];
	char	fn[MAX_PATH];

	GetTempPath(sizeof(tp), tp);
	GetTempFileName(tp, 0, 0, fn);
	if(__this){
		__touch__();
		__this->handle = CreateFile(
			fn,
			GENERIC_ALL,
			FILE_SHARE_READ,
			0,
			OPEN_ALWAYS,
			FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY,
			0
			);
		__create_base__(ITempStream, IF8Stream, __this);
	}
	return __this;
}

__destructor__(ITempStream)
{
	//
	if(__this){
		if(__this->handle){
			CloseHandle(__this->handle);
			__this->handle = 0;
			__destroy_base__(ITempStream, IF8Stream, __this);
		}
	}
	return __this;
}

__global_constructor__(ITempStream)
{
	__v_copy__(ITempStream, IF8Stream);
	__override__(ITempStream, IF8Stream, get);
	__override__(ITempStream, IF8Stream, put);
	__override__(ITempStream, IF8Stream, seek);
	__override__(ITempStream, IF8Stream, left);
	__override__(ITempStream, IF8Stream, align);
	__override__(ITempStream, IF8Stream, tell);
	__override__(ITempStream, IF8Stream, eos);
}

__define_vtbl__(ITempStream)
__define_vtbl_end__(ITempStream)


LIBCOBJ_API IF8Stream * create_temp_stream()
{
	ITempStream * ts;
	__new__(ITempStream, ts);
	return &ts->__inherited_IF8Stream;
}

LIBCOBJ_API void delete_temp_stream(IF8Stream * s)
{
	ITempStream * ts;
	ts = __dcast__(IF8Stream, ITempStream, s);
	__delete__(ts);
}

#else

LIBCOBJ_API ITempStream * get_temp_stream()
{
	ITempStream * fs;
	char * name;

	name = tempnam(NULL, NULL);
	fs = IFileStream_from_file(name, "wb+");
	
	return fs;
}

#endif

