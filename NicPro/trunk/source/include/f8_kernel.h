/********************************************************************
	created:	2002:10:12   15:23
	author:	Jackie, jhunter@tsinghua.org.cn
	
	purpose:	F8 kernel

	Kernel is basically an dedicated operating system to
	user program written in LD/FBD/SFC/ST/IL, etc. 
*********************************************************************/
#ifndef __f8kern_h__
#define __f8kern_h__

#include <rtk_list_entry.h>
#include <f8_binary.h>

#if defined(_WIN32) && !defined(FCC_INVOKED)
#ifdef LIBFKERN_EXPORTS
#define FKERN_API __declspec(dllexport)
#else
#define FKERN_API __declspec(dllimport)
#endif
#else
#define FKERN_API
#endif

#include <f8_unistd.h>

#define F8_CFG_TCP_PORT		0xf8cf

/*
	kernel-generated events
*/
#define F8_MODE_SWITCH			1
#define F8_PROG_RESUMED		2
#define F8_PROG_DOWNLOADED	3

#define F8_EXEC_STARTED		4
#define F8_EXEC_STOPPED		5
#define F8_EXEC_DOWNLOADED	6
#define F8_PRIMARY_FAILURE        7
#define F8_IOBUS_COLLISION         8

#define F8_INVALID_TIMER_ID		-1
#define F8_NO_MEMORY			-2
#define F8_NO_TIMER				-3
#define F8_INCONSISTENCY		-4
#define F8_FIRE_NON_EVENT		-5
#define F8_TIMER_TYPE			-6
#define F8_QUEUE_OVERFLOW		-7
#define F8_INVALID_PIN_ID		-8
#define F8_INVALID_FUNC_ID		-9

#define BLK_ID_ANY		-1
#define BLK_ID_ROOT		1
#define BLK_ID_NONE		0

#define TIMER_T_ONESHOT	0
#define TIMER_T_PERIODIC	1

#define TIMER_H_FREE	('E'<<24 | 'E' << 16 | 'R' << 8 | 'F')
#define TIMER_H_BUSY	('Y'<<24 | 'S' << 16 | 'U' << 8 | 'B')

#include <pshpack1.h>

typedef f8_i64 ktime_t;

#define _traverse(root,proc,context)\
	ITreeNode_traverse\
		(\
			__ucast__(IBlk, ITreeNode, root),\
			proc,\
			__true,\
			ITreeNode_post_order,\
			(__uint)(context)\
		)\

/*
	executive status report(volatile information of the executive)
*/

#define KERN_S_STOPPED	0
#define KERN_S_RUNNING	1
#define KERN_S_HALTED	2
#define KERN_S_STANDBY	3
#define KERN_S_UNKNOWN 4
#define KERN_S_ARBITRATING 5

struct kstatus_t{
	f8_u16	length;			/* offset 0 */
	f8_u8	state;			/* 2 */
	f8_u8	padding;		/* 3 */
	f8_u32	flags;			/* 4 */
	ktime_t	curr_time;		/* 8 */
	f8_uuid	prog_id;		/* 10 */
	__u16	timer_q_len;	/* 18 */
	__u16	event_q_len;	/* 1a */
	__u16	n_blocks;	/*1c  number of basic blocks */
	__u16	n_memory;	/* 1e in kilobytes */
	__u16	n_cycleTime;	/* 20 in terms of 100 milliseconds */
	f8_u16   n_phaseTimes[5]; /* 22 */
	ktime_t  upTime; /* 2c system startup time */
	f8_r4     n_cycles; /* 34 */
	f8_uuid  varTableId; /* 38 */
	f8_uint  firmwareVersion; /* 0x40*/
	f8_u8    reserved[116]; /* 0x44 */
}PACKED;

#define LARGE_BUF_SIZE (10*1024*1024)

#define MEM_SECTION_BYTE		0
#define MEM_SECTION_WORD		1
#define MEM_SECTION_R4			2
#define MEM_SECTION_R8			3
#define MEM_SECTION_ANYSIZE		4
#define MEM_SECTION_BIT			5

#define KERN_NUM_SECTIONS	6

#define _is_valid_section(n) (((f8_u16)n)<KERN_NUM_SECTIONS)

#define MEM_SECTION_INVALID	((f8_byte)-1)
#define MEM_SECTION_VAR		((f8_byte)-2)

struct klog_ent_t{
	ktime_t	tm;
	int		c[4];
}PACKED;

#define KERN_LOG_BUF_SIZE	1024

struct klog_t{
	int	tail;
	struct klog_ent_t logs[KERN_LOG_BUF_SIZE];
}PACKED;

/*
	maximum size of kernel's working pool.
	exchange memory/io image/work memory are allocated
	on this pool, layout of this pool is to be changed by
	F8_SET_MEM_LAYOUT
*/
#define KERN_POOL_SIZE	(256*1024)

#define IO_POOL_SIZE			(IO_SIZE_PER_SLOT*IO_MAX_DEVICES)

#define IO_DEVICE_INPUT	0x1
#define IO_DEVICE_OUTPUT 0x2
#define IO_DEVICE_TYPE_CONFLICT 0x4
#define IO_DEVICE_PRESENT	0x10

#define DEV_DATA_LEN 24
#define DEV_CONFIG_LEN 24

struct io_device{
	f8_uuid	clsid;
	f8_u16	addr;
	f8_u16	features;
	f8_u16	revision;
	f8_u8	n_di;
	f8_u8	n_do;
	f8_u8	n_ai;
	f8_u8	ai_width;	// in bytes
	f8_u8	n_ao;
	f8_u8	ao_width;	// in bytes
	f8_u8	configData[DEV_CONFIG_LEN];
}PACKED;

struct dev_status_t{
	f8_uuid id;
	f8_u8 data[DEV_DATA_LEN];
	f8_u8 configData[DEV_CONFIG_LEN];
}PACKED;

struct module_status_t{
	struct dev_status_t dev;
	f8_u8 ext[64];
}PACKED;

struct station_status_t{
	struct dev_status_t dev;

	f8_u8 route; /* for CM... modules only */
	f8_u8 padding[3];
	f8_r4 readCount;
	f8_r4 readFailCount;
	f8_r4 writeCount;
	f8_r4 writeFailCount;
	
	f8_r4 readTime;
	f8_r4 writeTime;
	f8_r4 readMaxTime;
	f8_r4 writeMaxTime;
	f8_r4 readMinTime;
	f8_r4 writeMinTime;

	f8_u8 reserved[20];
}PACKED;

struct ctrller_config{
	f8_uuid	uuid;
	f8_u16	size;
	f8_u16	deviceCount;
	f8_u32	cpuType;
	f8_u32	poolSize;

	f8_u32	x_mem_sizes[KERN_NUM_SECTIONS];
	struct io_device devices[IO_MAX_DEVICES];
}PACKED;

#include <poppack.h>

CDECL_BEGIN

extern FKERN_API int f8FirmwareVersion;

FKERN_API void ex_init();
FKERN_API void ex_uninit();

/*
	handle to a running kernel, each call into f8 kernel
	should supply a handle of this type.
*/
typedef void * HF8_KERNEL;

FKERN_API f8_bool is_address_valid(HF8_KERNEL _exec,const struct blk_pin_t *p, const struct mem_addr_t * addr);

struct cpmap_entry_t{
	char * name;
	int accessMode;
	int routing;
	/* internal use only, don't reference them outside of libfkern */
	int foo[4];
};

FKERN_API struct cpmap_entry_t * ke_get_config_map(void);
FKERN_API void ke_config(HF8_KERNEL _exec, struct kern_conf_cmd *cmd,__bool isConfigurator);
FKERN_API f8_bool ke_lock(HF8_KERNEL _exec, int writeMode);
FKERN_API void ke_unlock(HF8_KERNEL _exec, int writeMode);
FKERN_API IBlk * ke_get_blk(HF8_KERNEL _exec, const f8_uuid * id);
FKERN_API HF8_KERNEL ke_alloc_kernel();
FKERN_API void ke_free_kernel(HF8_KERNEL _kernel);

/*
	the default kernel is only used by the simulator.
*/
FKERN_API HF8_KERNEL ke_get_default_kernel();

/*
	kernel flags constants
*/
#define KERN_F_ENABLE_INPUT			(0x1 << 0)
#define KERN_F_ENABLE_OUTPUT		(0x1 << 1)
#define KERN_F_ENABLE_IO			(KERN_F_ENABLE_INPUT | KERN_F_ENABLE_OUTPUT)
#define KERN_F_ENABLE_STANDBY		(0x1 << 2)
#define KERN_F_ENABLE_SCHEDULER		(0x1 << 3)
#define KERN_F_ENABLE_AGENT			(0x1 << 4)
#define KERN_F_SERIALIZED_IO (0x1<<5)
#define KERN_F_ENABLE_WEB (0x1<<6)
#define KERN_F_ENABLE_WATCHDOG (0x1<<7)
#define KERN_FEATURES_MASK 0xff

/* normal startup flag, four-phase basic processor cycle, 0x1f */
#define KERN_F_NORMAL (KERN_F_ENABLE_IO | KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_AGENT \
		| KERN_F_ENABLE_STANDBY | KERN_F_ENABLE_WATCHDOG | KERN_F_SERIALIZED_IO)
/* w/o IO phase, 0x1c */
#define KERN_F_HEADLESS (KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_AGENT | \
		KERN_F_ENABLE_STANDBY | KERN_F_ENABLE_WATCHDOG)
/* w/o standby phase, 0x1b */
#define KERN_F_SINGLE (KERN_F_ENABLE_IO | KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_AGENT | \
		KERN_F_ENABLE_WATCHDOG)

/* 
* PANEL LED indication and key switch position 
* bit offset in kernel.flags
**/

#define FKERN_LED_RUNNING 8
#define FKERN_LED_STOP 9
#define FKERN_LED_STANDBY 10
#define FKERN_LED_BUSA 11
#define FKERN_LED_BUSB 12
#define FKERN_LED_SOFT_LOCK 13

#define FKERN_LED_DBG1 14
#define FKERN_LED_DBG2 15

#define FKERN_KEY_A_B  16
#define FKERN_KEY_RUN 17
#define FKERN_KEY_STOP 18
#define FKERN_KEY_LOCK 19

#define FKERN_LED_EX_RUNNING 20
#define FKERN_LED_SOFT_STOP 21
#define FKERN_LED_SYNCHRONIZED 22

FKERN_API f8_status ex_start(int flags, int agtPort);
FKERN_API f8_status ex_stop();
FKERN_API f8_u16 ke_get_type_size(f8_byte t);

#include <pshpack1.h>

#define VAR_S_LOCAL 		0
#define VAR_S_GLOBAL		1
#define VAR_S_UNSPECIFIED	2

#define BLK_VAR_MAGIC	'VA'

struct blk_var_t{
	f8_u16	magic;
	f8_u16	size;

	union{
		IBlk	* blk;
		f8_uuid blk_uuid;
	};	
	
	f8_byte type;
	f8_byte padding[3];

	/* name is stored right here */
	char name[F8_MAX_VAR_NAME_LEN];
	f8_uuid uuid;	
	f8_int ref_count;
	
	struct mem_addr_t addr; 
	/* comment is stored in the registry */
	const char * comment;
	/* reserved fields */
	struct dyn_pin_t initVal;
	f8_u8 reserved[12];
}PACKED;

#define is_var(p)\
	(((blk_var_t*)p)->magic == BLK_VAR_MAGIC? __true : __false)

#include <poppack.h>

/*
	API exported to configuration proxy
	the global variables are also exported, these APIs are
	only used by those which cannot access exported variables.
*/
FKERN_API f8_u8 * ke_get_x_memory(HF8_KERNEL _exec);
FKERN_API void ke_get_mem_sizes(HF8_KERNEL _exec, f8_u32 sizes[KERN_NUM_SECTIONS]);
FKERN_API f8_u8 * ke_get_section(HF8_KERNEL _kernel, f8_u8 section);
FKERN_API f8_uint ke_get_section_size(HF8_KERNEL _kernel, f8_u8 section);
FKERN_API f8_byte ke_section_by_type(f8_byte type);

/*
	API exported to simulator	
*/
FKERN_API f8_u8 * ke_get_io_memory(HF8_KERNEL _kernel, f8_u16 device);
FKERN_API struct io_device * ke_get_device_list(HF8_KERNEL _kernel);

/*
	hash support
*/
typedef f8_uuid hash_key_t;
typedef void * hash_val_t;

FKERN_API RTK_LIST_ENTRY * ke_init_hash();

FKERN_API void ke_uninit_hash(RTK_LIST_ENTRY * buckets);
FKERN_API __bool ke_set_hash(RTK_LIST_ENTRY * h, const hash_key_t * t, hash_val_t v);
FKERN_API hash_val_t ke_get_hash(RTK_LIST_ENTRY * h, const hash_key_t *t);
FKERN_API __bool ke_del_hash(RTK_LIST_ENTRY * h, const hash_key_t *t);
FKERN_API void ke_clear_hash(RTK_LIST_ENTRY * buckets);
FKERN_API f8_uint ke_enum_hash(
	RTK_LIST_ENTRY * buckets, 
	__bool (*visitor)(f8_uint context, const hash_key_t * k, const hash_val_t v),
	f8_uint context
	);

FKERN_API f8_bool ke_get_flag(int idx);
FKERN_API void ke_set_flag(int idx, f8_bool bit);
FKERN_API void ke_toggle_flag(int idx);
FKERN_API f8_bool ke_get_peer_flag(int idx);
FKERN_API f8_u32 ke_get_flags();
FKERN_API f8_u32 ke_get_peer_flags();
FKERN_API ktime_t ke_get_time();

#ifndef _WIN32
typedef struct _SYSTEMTIME { 
  f8_u16 wYear; 
  f8_u16 wMonth; 
  f8_u16 wDayOfWeek; 
  f8_u16 wDay; 
  f8_u16 wHour; 
  f8_u16 wMinute; 
  f8_u16 wSecond; 
  f8_u16 wMilliseconds; 
} SYSTEMTIME;
#endif

FKERN_API void ke_time_to_systime(ktime_t * ftime,SYSTEMTIME * st);
FKERN_API int ke_time_to_string(ktime_t t, char * buf, int len);
FKERN_API int ke_reltime_to_string(ktime_t t, char * buf, int len);
FKERN_API ktime_t ke_iotime_to_ktime(double iotime);
FKERN_API double ke_ktime_to_iotime(ktime_t t);

FKERN_API const char * ke_get_mode_name(f8_u8 s);

CDECL_END

#endif	/**/

