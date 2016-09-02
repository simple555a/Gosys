/*
	f8 kernel data structures

	2004/8 jhunter

	Note there should be some documentation on the memory usage
	of the kernel, unfortunately not yet. here is a brief discussion:
	
	*) hardware memory (dev_mem) is at the very bottom. In a soft-plc
	installation, the hardware memory might be maintained by a specific io
	driver. while in a F8 controller installation, the hardware memory is
	synchronized with F8 districuted IO modules, using some sort of
	distributed IO protocol, for now, the F8IO protocol.

	*) io memory, as a buffer layer to the hardware memory, is synchronized
	to dev_mem on each cycle of kernel execution

	*) x_memory is synchronized against io memory by special blocks,
	e.g., the F8_DDO_161 block. note this is not done by the kernel. x_memory
	is discussed below.
	
	*) each bblk has its own local memory, including dpins/data section/bss 
	section, data section and bss section are private to the block, while the
	dpins are used to exchange information amonge blocks. the exchange
	is done through a common buffer, i.e., the x_memory. each time a block
	is to be executed, its dpins section will be synchronized with the x_memory.

	So we can see, there'll be many many memory copies when the kernel
	is executing, just hope they won't become the bottle-neck.
*/


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#if _MSC_VER > 1400
#pragma comment(lib, "Ws2_32.lib")
#endif
#else
//#define EM615_BOARD
//#define ETX_BOARD
#define ETX_2
#define LX800
#endif

#define HAVE_STDIO

#if defined(HAVE_STDIO)
#include <stdio.h>
#include <assert.h>
#else
#define printf
#define assert(condition)
#define DEBUG_PRINTF
#endif

#include <librtk.h>

#include <string.h>
#include <f8.h>

#if defined(__VXWORKS_SIMNT__)
/* hack for simnt, seems to be a bug in my warez version of vxworks */
#define setjmp stjmp
#endif
#include <setjmp.h>

#ifdef __cplusplus
extern "C"{
#endif

struct kernel_t;

extern int taskFlag;
extern f8_u16 agentPort;
extern f8_uint kthreadId;
extern f8_uint agentThreadId;

void init_kernel(struct kernel_t * kernel);
void uninit_kernel(struct kernel_t * kernel);

int RTK_API baseCycle(struct kernel_t * kernel);

void process_input(struct kernel_t * kernel);
void process_output(struct kernel_t * kernel);
f8_int init_io();
f8_int uninit_io();
void start_io();
void stop_io();
f8_status io_write_config(const struct io_device * d);
f8_status io_read_config(int s);
f8_status io_write_status();

void * create_lock();
void delete_lock(void * lock);
void lock_lock(void * lock);
void lock_unlock(void * lock);

extern struct cpmap_entry_t cpmap[MAX_KERN_COMMAND];

void init_config();
#ifdef _WIN32
__uint RTK_API f8Agent(__uint foo);
#else
int f8Agent();
#endif

/* 
	debug interface
*/
int kd_print(const char * fmt, ...);

#ifndef NDEBUG
	#define KD_PRINT(__x__) kd_print __x__
#else
	#define KD_PRINT(__x__)
#endif

void init_peer();
void uninit_peer();
void peer_sync();

/*
#define peer_flush() do{\
	char buf[PEER_RTU];\
	int cookie;\
	cookie = peer_lock();\
	while(__vcall__(g_kernel.peer, read, (g_kernel.peer, buf, sizeof(buf), 0)) > 0);\
	peer_unlock(cookie);\
}while(0)
*/

#define peer_write(buf, len) __vcall__(g_kernel.peer, write_zero_copy, (g_kernel.peer, buf, len, 0))

#define peer_write2(buf1, len1,buf2,len2) \
	__vcall__(g_kernel.peer, write_zero_copy, (g_kernel.peer, buf1, len1,buf2,len2,0))

#define peer_write3(buf1, len1,buf2,len2,buf3,len3) \
	__vcall__(g_kernel.peer, write_zero_copy, (g_kernel.peer, buf1, len1,buf2,len2,buf3,len3,0))

#define peer_read(buf, len, to) __vcall__(g_kernel.peer, read, (g_kernel.peer, buf, len, to))

#define peer_lock() __vcall__(g_kernel.peer, lock, (g_kernel.peer))

#define peer_unlock(cookie) __vcall__(g_kernel.peer, unlock, (g_kernel.peer, cookie))

/*
	source insight version 3.5 fails in parsing after __class__, but I
	found that with a foo class defined here, all will be ok.
*/
struct foo_make_source_insight_happy{
	int foo;
};

#include <private/kernel_internal.h>

/* runtime variables */
extern ktime_t current_time;
extern ktime_t kern_time_bias;
/*
	volatile memory
*/
extern f8_u8 peerData[F8_VOLATILE_MEM_SIZE];
extern f8_u8 peerDataZipped[F8_COMPRESSED_VOLATILE_SIZE];
extern struct kpeer_hdr_t peerHdr;
extern f8_u8 * peerPointer;
extern f8_u8 * peerGuardian;
extern f8_u32 ledAndKeyStatus;
/*
	2004/3/1 soycola
	this is the only one global variable per running executive
*/
extern struct kernel_t g_kernel;

#define kernelState (g_kernel.status.state)
#define kernelFlags (g_kernel.status.flags)
#define peerState (g_kernel.peer_status.state)
#define peerFlags (g_kernel.peer_status.flags)

BLK_SERVICE_PROC blk_services[256];
HF8_HEAP kern_heap;
f8_u16 type_size_map[256];

#ifdef __GNUC__
static __inline__ int _type_size(f8_byte t)
#elif defined(_WIN32)
static __inline int _type_size(f8_byte t)
#else
static int _type_size(f8_byte t)
#endif
{
	return type_size_map[t];
}

#ifndef KERN_HEAP_SIZE
#define KERN_HEAP_SIZE (4*1024*1024)
#endif

#define _set_current(h, b)\
do{\
	(h)->current_blk = b;\
	(h)->current_bblk = __dcast__(IBlk, IBBlk, b);\
}while(0)

#define _set_current_bb(h, b)\
do{\
	h->current_bblk = b;\
	h->current_blk = __ucast__(IBBlk, IBlk, b);\
}while(0)

#ifdef __cplusplus
}
#endif

#define PIN_OF_UNMOUNTED_BLOCK ((struct blk_pin_t*)1)

#include "irprocs.h"

#define TSK_RUNNING	    0
#define TSK_STOPPING    1
#define TSK_STOPPED     2

#ifdef _WIN32
#include <f8_softplc.h>
#endif

#if defined(_WIN32)
	#include <process.h>
	#include <winsock2.h>
	#define miliSleep(n)	Sleep(n)
	#undef ERROR
	#define ERROR -1	
#elif defined(__VXWORKS__)
	#include <taskLib.h>
	#include <timers.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
	#define closesocket close
	extern void miliSleep(unsigned int n);
#elif defined(__LINUX__)
	#include <netinet/in.h>
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
	#define ERROR -1
	#define closesocket close
	typedef int SOCKET;
	extern void miliSleep(unsigned int n);
#endif

extern void init_peer();
extern void uninit_peer();

#include "ki.h"

#ifndef LOCAL
#define LOCAL static
#endif

#define BUFFERED_TIMES 20

#ifdef __VXWORKS__
#include "f8SysLib.h"
#endif


