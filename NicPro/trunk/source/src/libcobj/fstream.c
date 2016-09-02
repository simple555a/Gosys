#include "precomp.h"

#ifdef HAVE_STDIO

static __uint IFileStream_get(IF8Stream * _s, void * buf, __uint bytes)
{
	IFileStream * s = __dcast__(IF8Stream, IFileStream, _s);
	return fread(buf, 1, bytes, s->fp);
}

static __uint IFileStream_put(IF8Stream * _s, void * buf, __uint bytes)
{
	IFileStream * s = __dcast__(IF8Stream, IFileStream, _s);
	return fwrite(buf, 1, bytes, s->fp);
}

static __uint IFileStream_seek(IF8Stream * _s, long offset, int whence)
{
	IFileStream * s = __dcast__(IF8Stream, IFileStream, _s);
	fseek(s->fp, offset, whence);
	return ftell(s->fp);
}

static __uint IFileStream_tell(IF8Stream * _s)
{
	IFileStream * s = __dcast__(IF8Stream, IFileStream, _s);
	return ftell(s->fp);
}

static __uint IFileStream_left(IF8Stream * _s)
{
	IFileStream * s = __dcast__(IF8Stream, IFileStream, _s);
	int cur ;
	int total;
	cur = ftell(s->fp);
	if( fseek(s->fp, 0, SEEK_END) ){
		return 0;
	}
	total = ftell(s->fp);
	if( fseek(s->fp, cur, SEEK_SET) ){
		return 0;
	}
	return total - cur;
}

static __bool IFileStream_eos(IF8Stream * _s)
{
	IFileStream * s = __dcast__(IF8Stream, IFileStream, _s);
	return feof(s->fp);
}

static __uint IFileStream_align(IF8Stream * _s, int b)
{
	IFileStream * s;
	static char align[16];
	int i;

	s = __dcast__(IF8Stream, IFileStream, _s);
	i = ftell(s->fp) % b;
	if(i){
		b = b - i;
	}else{
		b = 0;
	}
	// a number of b bytes should be written to the file
	while(b >= 16){
		fwrite(align, 1, 16, s->fp);
		b -= 16;
	}
	fwrite(align, 1, b, s->fp);
	return ftell(s->fp);
}

IFileStream * IFileStream_from_file(const char * file, const char * mode)
{
	IFileStream * s;
	__new__(IFileStream, s);
	if( !s ){
		return 0;
	}
	s->fp = fopen(file, mode);
	if( !s->fp ){
		__delete__(s);
		return 0;
	}
	return s;
}

__constructor__(IFileStream)
{
	if(__this){
		__touch__();
		__this->fp = 0;
		__create_base__(IFileStream, IF8Stream, __this);
	}
	return __this;
}

__destructor__(IFileStream)
{
	//
	if(__this){
		if(__this->fp){
			fclose(__this->fp);
		}
		__destroy_base__(IFileStream, IF8Stream, __this);
	}
	return __this;
}

__global_constructor__(IFileStream)
{
	__v_copy__(IFileStream, IF8Stream);
	__override__(IFileStream, IF8Stream, get);
	__override__(IFileStream, IF8Stream, put);
	__override__(IFileStream, IF8Stream, seek);
	__override__(IFileStream, IF8Stream, left);
	__override__(IFileStream, IF8Stream, align);
	__override__(IFileStream, IF8Stream, tell);
	__override__(IFileStream, IF8Stream, eos);
}

__define_vtbl__(IFileStream)
__define_vtbl_end__(IFileStream)

#endif
