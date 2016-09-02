#ifdef _WIN32
#define _WIN32_WINNT 0x0400
#define WINVER 0x0400
#define WIN32_LEAN_AND_MEAN

// Zander 2014-1-7
#pragma warning(disable:4005)

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef __LINUX__
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <net/if.h>
#endif

#if defined(_WIN32) && !defined(UNDER_CE)
#include <process.h>
#endif

#include <stdio.h>
#include <assert.h>

#pragma warning(disable : 4786)
#include <map>
#include <list>
#include <string>

// #define UTILS_NODEBUG

#include <pmc_config.h>
#include <rtkdefs.h>
#include <utils.h>

#include <powermgr.h>

#include <rtknet.h>
#include <transaction.h>
#include <rtksocket.h>
#include <object.h>
#include <vbus.h>
#define RTK_NO_PACKET_STRUCTS
#include <rtk_packet.h>

#include "netversion.h"

#ifdef _WIN32
typedef int socklen_t;
typedef char * _SOCKBUF;
#define IsInvalidSocket(s) (s == INVALID_SOCKET)
#else
#define closesocket(s) close(s)
typedef void * _SOCKBUF;
#define IsInvalidSocket(s) (s < 0)
#define GetCurrentProcessId() getpid()
#endif

extern double g_TransactionLife;
