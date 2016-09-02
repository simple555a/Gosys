/*
  Streamable support header file
  by Jackie, jhunter@tsinghua.org.cn
  Create : Mar. 7, 2003
*/

#ifndef __streamable_h__
#define __streamable_h__

#ifdef HAVE_STDIO
#include <stdio.h>
#endif
#include <c_object.h>
#include <pshpack1.h>
#include <f8_net.h>

CDECL_BEGIN

// IF8Stream object

__class__(IF8Stream)
__class_end__(IF8Stream)
	
__vtable__(IF8Stream)
	__vmethod__(__uint, get, (IF8Stream * s, void * buf, __uint bytes));
	__vmethod__(__uint, put, (IF8Stream * s, void * buf, __uint bytes));
	__vmethod__(__uint, tell, (IF8Stream *s));
	__vmethod__(__uint, seek, (IF8Stream * s, long offset, int whence));
	__vmethod__(__uint, align, (IF8Stream * s, int boundary));
	__vmethod__(__uint, left, (IF8Stream * s));
	__vmethod__(__bool, eos, (IF8Stream *s));
__vtable_end__(IF8Stream)

__class__(IMemStream)
	__inherit__(IF8Stream)
	char * buffer;
	__uint length;
	__uint pointer;
__class_end__(IMemStream)
	
__vtable__(IMemStream)
	__inherit__(IF8Stream_vtbl)
__vtable_end__(IMemStream)

__class__(IStreamable)
__class_end__(IStreamable)

__vtable__(IStreamable)
	__vmethod__(__bool, from_stream, (IStreamable * _this, IF8Stream * str));
	__vmethod__(__bool, to_stream, (IStreamable  * _this, IF8Stream * str));
__vtable_end__(IStreamable)

#ifdef INCLUDE_TCP_STREAM
__class__(ITcpStream)
	__inherit__(IF8Stream)
	struct sockaddr_in peer;
	SOCKET s;
__class_end__(ITcpStream)
	
__vtable__(ITcpStream)
	__inherit__(IF8Stream_vtbl)
__vtable_end__(ITcpStream)

LIBCOBJ_API ITcpStream * ITcpStream_build	(const char * peer, __u16 port);

#endif

#ifdef HAVE_STDIO

__class__(IFileStream)
	__inherit__(IF8Stream)
	FILE *fp;
__class_end__(IFileStream)

__vtable__(IFileStream)
	__inherit__(IF8Stream_vtbl)
__vtable_end__(IFileStream)

LIBCOBJ_API IFileStream * IFileStream_from_file(const char * file, const char * mode);

#ifdef _WIN32
/*
	a temporary stream objects, implemention based on temp file
	(file opened with flag : DELETE_ON_CLOSE )
*/
__class__(ITempStream)
	__inherit__(IF8Stream)
	void * handle;
__class_end__(ITempStream)

__vtable__(ITempStream)
	__inherit__(IF8Stream_vtbl)
__vtable_end__(ITempStream)

#else

#define ITempStream IFileStream

LIBCOBJ_API ITempStream * get_temp_stream();

#endif

#endif	// HAVE_STDIO

#ifndef SEEK_SET
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#endif

LIBCOBJ_API __uint IMemStream_bind(IMemStream * s, void * buf, __uint bytes);

#ifdef HAVE_STDIO

LIBCOBJ_API IMemStream * IMemStream_load_file(const char * file);

LIBCOBJ_API __bool IMemStream_insert_file(IMemStream * s, const char * file);

LIBCOBJ_API __bool IMemStream_to_file(IMemStream * s, const char * file);

// for delphi use
LIBCOBJ_API IF8Stream * create_mem_stream(__uint size);
LIBCOBJ_API void delete_mem_stream(IF8Stream *ms);
LIBCOBJ_API IF8Stream * create_temp_stream();
LIBCOBJ_API void delete_temp_stream(IF8Stream *ms);

#endif
	
#include <poppack.h>

CDECL_END

#endif	// dual-inclusion guard

