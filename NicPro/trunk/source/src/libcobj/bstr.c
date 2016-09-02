#include "precomp.h"

LIBCOBJ_API char * c_alloc_str(int size)
{
	return __malloc__(size + 1);
}

LIBCOBJ_API void c_free_str(char * s)
{
	__free__(s);
}

LIBCOBJ_API char * c_strdup(const char * s)
{
	char * d;
	d = c_alloc_str(strlen(s));
	if( !d ){
		return 0;
	}
	strcpy(d, s);
	return d;
}

LIBCOBJ_API __bool bstr_from_stream(struct c_bstr_t * b, IF8Stream * s)
{
	if(__vcall__(s, get, (s, &b->count, sizeof(b->count))) != sizeof(b->count)){
		return __false;
	}
	b->buffer = c_alloc_str(b->count);
	if(!b->buffer){
		return __false;
	}
	if(__vcall__(s, get, (s, b->buffer, b->count + 1)) != b->count + 1){
		c_free_str(b->buffer);
		b->buffer = 0;
		return __false;
	}
	if(b->buffer[b->count] != '\0'){
		c_free_str(b->buffer);
		b->buffer = 0;
		return __false;
	}
	return __true;
}

LIBCOBJ_API __bool bstr_from_memstream(struct c_bstr_t * b, IMemStream *s)
{
	b->buffer = s->buffer + s->pointer;
	b->count = *((__u16*)b->buffer);
	if(s->length - s->pointer < b->count + sizeof(b->count) + 1){
		return __false;
	}
	b->buffer += sizeof(b->count);
	s->pointer += b->count + sizeof(b->count) + 1;
	if( b->buffer[b->count] ){
		return __false;
	}
	return __true;
}

LIBCOBJ_API void bstr_to_stream(struct c_bstr_t * b, IF8Stream *s)
{
	__vcall__(s, put, (s, &b->count, sizeof(b->count)));
	__vcall__(s, put, (s, b->buffer, b->count + 1));
}
