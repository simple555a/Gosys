#if defined(_WIN32)
	#include <winsock2.h>
	#define taskDelay(n)	Sleep(n)
	#undef ERROR
	#define ERROR -1	
	#define IS_INVALID_SOCKET(s) (s == INVALID_SOCKET)
#elif defined(__VXWORKS__)
	#include <netinet/in.h>
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
	#define closesocket close
	typedef int SOCKET;
	#define IS_INVALID_SOCKET(s) (s<0)
#elif defined(__LINUX__)
	#include <netinet/in.h>
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
	#define ERROR -1
	#define closesocket close
	typedef int SOCKET;
	#define IS_INVALID_SOCKET(s) (s<0)
#endif

#define ACTECH_ETHER_TYPE 0xf810
#define ACTECH_IO_PROTOCOL 0
#define ACTECH_STANDBY_PROTOCOL 1
#define ACTECH_ETHER_ID "\x00\xac\x1e\xc1\xf8\x00"


