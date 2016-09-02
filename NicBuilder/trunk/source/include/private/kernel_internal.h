#ifndef __kernel_internal_h__
#define __kernel_internal_h__

#include <pshpack1.h>

struct blk_timer_t{
	RTK_LIST_ENTRY link;
	int			hint;
	struct blk_pin_t * pin;
	f8_u8		type;
	f8_u8		pad[3];
	ktime_t		duetime;
	ktime_t		period;
}PACKED;

#ifndef EVENT_Q_LENGTH
#define EVENT_Q_LENGTH	4096		// max events at a time
#endif
#ifndef TIMER_Q_LENGTH 
#define TIMER_Q_LENGTH	4096		// max timers per task
#endif
#ifndef F8_VOLATILE_MEM_SIZE
#define F8_VOLATILE_MEM_SIZE (2*1024*1024)
#endif
#ifndef F8_COMPRESSED_VOLATILE_SIZE
#define F8_COMPRESSED_VOLATILE_SIZE (2*1024*1024)
#endif

struct marshalled_event_t{
	f8_uuid	blkid;
	f8_u16	pid;
}PACKED;

struct marshalled_timer_t{	
	f8_uuid	blkid;
	f8_u16	pid;
	f8_u8	type;
	f8_u8	pad;
	ktime_t	duetime;
	ktime_t	period;
}PACKED;

struct kpeer_hdr_t{
	f8_uuid progId;
	f8_uint timer_q_len;
	f8_uint event_q_len;
	f8_uint i_mem_size;
	f8_uint x_mem_sizes[KERN_NUM_SECTIONS];
	f8_uint zipped_data_len;
};

/*
	check every two hours to see if configuration should be 
	saved to non-volatile memory
	Note : changes are also saved whenever client communication
	is shutdown.
*/
#define SAVE_CONFIG_WDOG_COUNTER	7200

#define HSE_PEER_RX 1
#define HSE_PEER_TX 2
#define HSE_PEER_LINEDOWN 3
#define HSE_PEER_COLLISION 4
#define HSE_PEER_STATUS_BROADCAST 5

/*
	IPeer is a class that manages communication with the peer
	controller.
*/
__class__(IPeer)
/* physical address of peer, e.g., the MAC address in ethernet peer 
or IP:port in UDP peer
*/
	__u8 peerAddress[14];
	__u8 localAddress[14];
/* timestamp of last message from peer */
	ktime_t lastMsgTime;
	void (*eventProc)(int code, void *e1,int e2,int e3,int e4);
__class_end__(IPeer)

__vtable__(IPeer)
	__vmethod__(
		int,
		write, 
		(IPeer * _this, const void * buf, int len)
		);
	__vmethod__(
		int,
		read, 
		(IPeer * _this, void * buf, int len, int microseconds)
		);
	__vmethod__(
		int,
		write_zero_copy, 
		(IPeer * _this, /* const void * buf, int len */ ...)
		);
	__vmethod__(int,lock,(IPeer*_this));
	__vmethod__(void,unlock,(IPeer*_this, int cookie));
__vtable_end__(IPeer)

struct make_source_insight_happy{
	int foo;
};
/*
	for executive, there should be only one running kernel, so
	the following variable ``current'' is used for quick access
	to the running kernel.

	2004/3/1 soycola
	Stupid design, though it's true for executive, the global
	variable approach is definitely not appropriate for client
	side, e.g., the PMC driver interface to F8 executive.
*/

/*
	2004/3/1 soycola
	a structure representing a kernel executive to clients, 
	one instance per client connection to a kernel executive.
*/
struct kernel_t{
	/* block tree representation */
	IBlk  * sections;
	ICBlk _sections;
	void	*lock;
	RTK_LIST_ENTRY *blk_hash;
	IBlk	*current_blk;
	IBBlk	*current_bblk;
	RTK_LIST_ENTRY m_1131_blocks;
	struct blk_pin_t * current_pin;
	f8_bool bModified;
	f8_byte padding_1[3];

	/* hot standby support */
	f8_byte * standby_x_buffer;
	struct kstatus_t peer_status;
	IPeer * peer;
	
	/* event queue and timer list */
	int eq_head, eq_end;
	RTK_LIST_ENTRY	tmr_busy, tmr_free;
	
	/* exception support */
	jmp_buf g_jmpbuf;

	/* status */
	struct kstatus_t status;

/* memory consuming stuff begins here */

	/* event queue and timer list */
	struct blk_pin_t * event_q[EVENT_Q_LENGTH];
	struct blk_timer_t	timers[TIMER_Q_LENGTH];
	
	/* exchange memory */
	f8_u8  x_memory[KERN_POOL_SIZE];
	f8_u8 * x_mem_pointers[KERN_NUM_SECTIONS];
	f8_uint x_mem_sizes[KERN_NUM_SECTIONS];
	f8_uint total_x_mem_size;

	/* io memory */
	f8_u8 io_memory[IO_MAX_DEVICES][IO_SIZE_PER_SLOT];

	/* device configuration list */
	struct io_device devices[IO_MAX_DEVICES];
	struct module_status_t moduleStatus[IO_MAX_DEVICES];
	struct station_status_t stationStatus[IO_MAX_STATIONS];
	
	/* log support */
	struct klog_t klog;
}PACKED;

#include <poppack.h>

#endif

