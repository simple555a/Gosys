#include "precomp.h"

// #define UUID_DEBUG

#ifdef UUID_DEBUG

static f8_uuid seq = 0x100;
static f8_uuid seqBreak = (f8_uuid)-1;

F8RES_API void create_f8_uuid(
	f8_uuid * id
	)
{
	seq++;
	if(seq == seqBreak){
		__asm{int 3};
	}
	*id = seq;
}
#else
F8RES_API void create_f8_uuid(
	f8_uuid * id
	)
{
	RTK_GUID guid;
	guid.create();
	int i;
	char * v=(char*)id;
	for(i=0; i<8; i++){
		v[i] = guid.data[i] + guid.data[i+8];
	}
}
#endif

F8RES_API void f8_uuid_to_string(
	const f8_uuid * id,
	char * buf,
	int length
	)
{
	__u16 * is;
	is = (__u16*)id;
	_snprintf(
		buf, 
		length, 
		"%04x-%04x-%04x-%04x", 
		is[0], is[1], is[2], is[3]
		);
}

F8RES_API void f8_uuid_from_string(
	f8_uuid * id,
	const char * buf
	)
{
	__u32 is[4];
	f8_u16 * _is = (f8_u16*)id;

	/* accept uuid enclosed by braces */
	if(*buf == '{'){
		buf++;
	}
	sscanf(buf, "%x%*c%x%*c%x%*c%x", is, is+1, is+2, is+3);
	_is[0] = is[0];
	_is[1] = is[1];
	_is[2] = is[2];
	_is[3] = is[3];
}

