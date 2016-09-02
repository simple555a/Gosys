#include "precomp.h"

static __uint IMemStream_get(IF8Stream * _s, void * buf, __uint bytes)
{
	IMemStream * s = __dcast__(IF8Stream, IMemStream, _s);
	if(s->pointer + bytes > s->length){
		return 0;
	}
	memcpy(buf, s->buffer + s->pointer, bytes);
	s->pointer += bytes;
	return bytes;
}

static __uint IMemStream_put(IF8Stream * _s, void * buf, __uint bytes)
{
	IMemStream * s = __dcast__(IF8Stream, IMemStream, _s);
	if(s->pointer + bytes > s->length){
		return 0;
	}
	memcpy(s->buffer + s->pointer, buf, bytes);
	s->pointer += bytes;
	return bytes;
}

static __uint IMemStream_seek(IF8Stream * _s, long offset, int whence)
{
	IMemStream * s = __dcast__(IF8Stream, IMemStream, _s);
	__uint new_pointer = s->pointer;

	switch(whence){
	case SEEK_SET:
		new_pointer = offset;
		break;
	case SEEK_CUR:
		new_pointer = (__uint)(s->pointer + offset);
		break;
	case SEEK_END:
		new_pointer = (__uint)(s->length + offset);
		break;
	}
	if(new_pointer >=0 && new_pointer <= s->length){
		s->pointer = new_pointer;
	}
	return s->pointer;
}

static __uint IMemStream_tell(IF8Stream *_s)
{
	IMemStream * s = __dcast__(IF8Stream, IMemStream, _s);
	return s->pointer;
}

static __uint IMemStream_left(IF8Stream * _s)
{
	IMemStream * s = __dcast__(IF8Stream, IMemStream, _s);
	return s->length - s->pointer;
}

static __bool IMemStream_eos(IF8Stream * _s)
{
	IMemStream * s = __dcast__(IF8Stream, IMemStream, _s);
	return s->length - s->pointer == 0? __true : __false;
}

__uint IMemStream_bind(IMemStream * __this, void * buf, __uint bytes)
{
	__this->buffer = (char *)buf;
	__this->length = bytes;
	__this->pointer = __this->pointer = 0;
	return bytes;
}

static __uint IMemStream_align(IF8Stream * _s, int b)
{
	IMemStream * s;
	__uint p;
	s = __dcast__(IF8Stream, IMemStream, _s);
	p = s->pointer;
	p = ((p + b - 1) /b) * b;
	if( p < s->length ){
		s->pointer = p;
	}
	return s->pointer;
}

#ifdef HAVE_STDIO

/*
	allocates a memory stream object from heap, whose contents is 
	initialized with a file.
*/
IMemStream * IMemStream_load_file(const char * file)
{
	void *buf;
	unsigned int len;
	IMemStream * s;
	FILE * fp;

	fp = fopen(file, "rb");
	if( !fp ){
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = __malloc__(sizeof(IMemStream) + len);
	if( !buf ){
		fclose(fp);
		return 0;
	}
	if( len != fread((char *)buf + sizeof(IMemStream), 1, len, fp) ){
		fclose(fp);
		return 0;
	}
	s = (IMemStream*)buf;
	__create__(IMemStream, s);
	IMemStream_bind(s, &s[1], len);
	fclose(fp);
	return s;
}

/*
	insert contents of a file at current position
*/
__bool IMemStream_insert_file(IMemStream * s, const char * file)
{
	unsigned int len;
	FILE * fp;
	char buf[10240];
	IF8Stream * _s;

	fp = fopen(file, "rb");
	if( !fp ){
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(s->length - s->pointer < len){
		fclose(fp);
		return __false;
	}

	_s = __ucast__(IMemStream, IF8Stream, s);
	len = fread(buf, 1, sizeof(buf), fp);
	while(len){
		__vcall__(_s, put, (_s, buf, len));
		len = fread(buf, 1, sizeof(buf), fp);
	}

	fclose(fp);
	
	return __true;
}

/*
	write remaining part of a memory stream to a file.
*/
__bool IMemStream_to_file(IMemStream * s, const char * file)
{
	FILE *fp;
	__bool ret;
	fp = fopen(file, "wb");
	if(!fp){
		return __false;
	}
	ret = fwrite(
		s->buffer + s->pointer, 
		1, 
		s->length - s->pointer, 
		fp) == s->length - s->pointer ? __true : __false;
	fclose(fp);
	return ret;
}

#endif

__constructor__(IMemStream)
{
	if(__this){
		__touch__();
		__create_base__(IMemStream, IF8Stream, __this);
	}
	return __this;
}

__destructor__(IMemStream)
{
	//
	__destroy_base__(IMemStream, IF8Stream, __this);
	return __this;
}

__global_constructor__(IMemStream)
{
	__v_copy__(IMemStream, IF8Stream);
	__override__(IMemStream, IF8Stream, get);
	__override__(IMemStream, IF8Stream, put);
	__override__(IMemStream, IF8Stream, seek);
	__override__(IMemStream, IF8Stream, left);
	__override__(IMemStream, IF8Stream, align);
	__override__(IMemStream, IF8Stream, tell);
	__override__(IMemStream, IF8Stream, eos);
}

__define_vtbl__(IMemStream)
__define_vtbl_end__(IMemStream)

#if defined(_WIN32)

/* 
	allocates a memory stream object with specified size 
*/
LIBCOBJ_API IF8Stream * create_mem_stream(__uint size)
{
	char * buf;
	IMemStream * ms;
	buf = __malloc__(size + sizeof(IMemStream));
	if(!buf){
		return 0;
	}
	ms = (IMemStream*)buf;
	__create__(IMemStream, ms);
	IMemStream_bind(ms, &ms[1], size);
	return &ms->__inherited_IF8Stream;
}

LIBCOBJ_API void delete_mem_stream(IF8Stream * s)
{
	IMemStream * ms = __dcast__(IF8Stream, IMemStream, s);
	__delete__(ms);
}

#endif
