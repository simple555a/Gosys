#include "precomp.h"
#pragma hdrstop
#include <time.h>

#ifdef INCLUDE_TCP_STREAM
static int recvPacket(
	ITcpStream * str,
	char * buf, 
	int left, 
	int timeout
	)
{
	int bytes;
	fd_set sset;
	struct timeval tout;
	clock_t t1, t2;
	int totalBytes = left;
	SOCKET s = str->s;

	if(IS_INVALID_SOCKET(s)){
		return 0;
	}
	
	t1 = clock();
	while(left /* && timeout > 0 */){
		FD_ZERO(&sset);
		FD_SET(s, &sset);
		tout.tv_sec = 0;
		tout.tv_usec = timeout * 1000;
		select(s + 1, &sset, 0, 0, &tout);
		if(!FD_ISSET(s, &sset)){
			break;
		}
		bytes = recv(s, buf, left, 0);
		if(bytes <= 0){
			/* error or connection closed by peer */
			closesocket(str->s);
			str->s = INVALID_SOCKET;
			break;
		}
		left -= bytes;
		buf += bytes;
		t2 = clock();
		timeout -= (t2 - t1) * 1000 / CLOCKS_PER_SEC;
	}
	return totalBytes - left;
}

static int sendPacket(
	ITcpStream * str,
	const char * buf, 
	int left, 
	int timeout
	)
{
	int bytes;
	fd_set sset;
	struct timeval tout;
	clock_t t1, t2;
	SOCKET s = str->s;
	int totalBytes = left;
	
	if(IS_INVALID_SOCKET(s)){
		return 0;
	}
	t1 = clock();
	while(left /* && timeout > 0 */){
		FD_ZERO(&sset);
		FD_SET(s, &sset);
		tout.tv_sec = 0;
		tout.tv_usec = timeout * 1000;
		select(s + 1, 0, &sset, 0, &tout);
		if(!FD_ISSET(s, &sset)){
			break;
		}
		bytes = send(s, buf, left, 0);
		if(bytes <= 0){
			/* error or connection closed by peer */
			closesocket(str->s);
			str->s = INVALID_SOCKET;
			break;
		}
		left -= bytes;
		buf += bytes;
		t2 = clock();
		timeout -= (t2 - t1) * 1000 / CLOCKS_PER_SEC;
	}
	return totalBytes - left;
}

static __uint ITcpStream_get(IF8Stream * _s, void * buf, __uint bytes)
{
	ITcpStream * s = __dcast__(IF8Stream, ITcpStream, _s);
	return recvPacket(s, buf, bytes, 1000*1000);
}

static __uint ITcpStream_put(IF8Stream * _s, void * buf, __uint bytes)
{
	ITcpStream * s = __dcast__(IF8Stream, ITcpStream, _s);
	return sendPacket(s, buf, bytes, 1000);
}

static __uint ITcpStream_seek(IF8Stream * _s, long offset, int whence)
{
	char smallBuf[256];
	ITcpStream * s;
	__uint bytes;
	
	if(whence != SEEK_CUR){
		return (__uint)-1;
	}
	if(offset < 0){
		return (__uint)-1;
	}
	s = __dcast__(IF8Stream, ITcpStream, _s);
	while(offset && !IS_INVALID_SOCKET(s->s)){
		if(offset >= sizeof(smallBuf)){
			bytes = sizeof(smallBuf);
		}else{
			bytes = offset;
		}
		bytes = recvPacket(s, smallBuf, bytes, 1000*1000);
		offset -= bytes;
	}
	return 0;
}

static __uint ITcpStream_tell(IF8Stream * _s)
{
	return 0;
}

static __uint ITcpStream_left(IF8Stream * _s)
{
	return 0;
}

static __bool ITcpStream_eos(IF8Stream * _s)
{
	ITcpStream * s;
	s = __dcast__(IF8Stream, ITcpStream, _s);
	if(IS_INVALID_SOCKET(s->s)){
		return __true;
	}
	return __false;
}

static __uint ITcpStream_align(IF8Stream * _s, int b)
{
	return 0;
}

ITcpStream * ITcpStream_build(const char * peer, __u16 port)
{
	int ret;
	ITcpStream * s;
	__new__(ITcpStream, s);
	if( !s ){
		return 0;
	}
	s->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(IS_INVALID_SOCKET(s->s)){
		__delete__(s);
		return 0;
	}
	memset(&s->peer, 0, sizeof(s->peer));
	s->peer.sin_family = AF_INET;
	s->peer.sin_addr.s_addr = inet_addr(peer);
	s->peer.sin_port = htons(port);
	ret = connect(s->s, (struct sockaddr*)&s->peer, sizeof(s->peer));
	if(ret < 0){
		__delete__(s);
		return 0;
	}
	return s;
}

__constructor__(ITcpStream)
{
	if(__this){
		__touch__();
		__this->s = INVALID_SOCKET;
		__create_base__(ITcpStream, IF8Stream, __this);
	}
	return __this;
}

__destructor__(ITcpStream)
{
	//
	if(__this){
		if(!IS_INVALID_SOCKET(__this->s)){
			closesocket(__this->s);
			__this->s = INVALID_SOCKET;
		}
		__destroy_base__(ITcpStream, IF8Stream, __this);
	}
	return __this;
}

__global_constructor__(ITcpStream)
{
	__v_copy__(ITcpStream, IF8Stream);
	__override__(ITcpStream, IF8Stream, get);
	__override__(ITcpStream, IF8Stream, put);
	__override__(ITcpStream, IF8Stream, seek);
	__override__(ITcpStream, IF8Stream, left);
	__override__(ITcpStream, IF8Stream, align);
	__override__(ITcpStream, IF8Stream, tell);
	__override__(ITcpStream, IF8Stream, eos);
}

__define_vtbl__(ITcpStream)
__define_vtbl_end__(ITcpStream)


#endif