/*
	kernel procedures
	07/08/2005 panxh@actech-mc.com
*/
#include "precomp.h"
#pragma hdrstop

static void _init_user_program(struct kernel_t *kernel)
{
	printf("Initializing user program ...\n");

	RtkInitializeListHead(&kernel->m_1131_blocks);
	
	__create__(ICBlk, &kernel->_sections);
	kernel->sections = __ucast__(ICBlk, IBlk, &kernel->_sections);
	kernel->sections->uuid = BLK_ID_ROOT;
	kernel->sections->h.uuid = -1;

	ke_set_hash(
		kernel->blk_hash, 
		&kernel->sections->uuid, 
		kernel->sections
		);
}

/*
	per-kernel initialization
*/
void init_kernel(struct kernel_t * kernel)
{	
	#define kkk(s) printf("sizeof(" #s ")=%d\n", sizeof(struct s))
	// kkk(kern_conf_cmd);
	#undef kkk
	
	/*
		one-time initialization per kernel
	*/
	memset(kernel, 0, sizeof(struct kernel_t));
	kernel->lock = create_lock();
	kernel->blk_hash = ke_init_hash();
	_init_user_program(kernel);

	/* other initialization which can be done several times */
	ki_reset(kernel);

	if(kernel == &g_kernel){
		ki_init_var_table();
		ki_log(kernel, F8_EXEC_STARTED, 0, 0, 0);
	}
}

/*
	per-kernel un-initialization
*/
void uninit_kernel(struct kernel_t * kernel)
{
	ki_reset(kernel);

	if(kernel->lock){
		delete_lock(kernel->lock);
		kernel->lock = 0;
	}
	if(kernel->blk_hash){
		ke_uninit_hash(kernel->blk_hash);
		kernel->blk_hash = 0;
	}

	if(kernel == &g_kernel){
		ki_uninit_var_table();
		ki_log(kernel, F8_EXEC_STOPPED, 0, 0, 0);
	}
}

FKERN_API f8_u16 ke_get_type_size(f8_byte t)
{
	return _type_size(t);
}

FKERN_API IBlk * ke_get_blk(HF8_KERNEL _kernel, const f8_uuid * id)
{
	struct kernel_t *kernel = (struct kernel_t*)_kernel;
	if(!id){
		return kernel->sections;
	}
	if(*id == BLK_ID_ROOT){
		return kernel->sections;
	}
	return (IBlk*)ke_get_hash(kernel->blk_hash, id);
}

FKERN_API f8_u8 * ke_get_x_memory(HF8_KERNEL _kernel)
{
	struct kernel_t * kernel = (struct kernel_t*)_kernel;
	return kernel->x_memory;
}

FKERN_API f8_u8 * ke_get_section(
	HF8_KERNEL _kernel, 
	f8_u8 section
	)
{
	struct kernel_t * kernel = (struct kernel_t*)_kernel;
	if(section >= KERN_NUM_SECTIONS){
		return 0;
	}
	return kernel->x_mem_pointers[section];
}

FKERN_API void ke_get_mem_sizes(
	HF8_KERNEL _kernel, 
	f8_u32 sizes[KERN_NUM_SECTIONS]
	)
{
	struct kernel_t * kernel = (struct kernel_t*)_kernel;
	memcpy(
		sizes, 
		kernel->x_mem_sizes, 
		sizeof(kernel->x_mem_sizes)
		);
}

FKERN_API f8_uint ke_get_section_size(
	HF8_KERNEL _kernel, 
	f8_u8 section
	)
{
	struct kernel_t * kernel = (struct kernel_t*)_kernel;
	if(section >= KERN_NUM_SECTIONS){
		return (__uint)-1;
	}
	return kernel->x_mem_sizes[section];
}

/*
	API exported to simulator	
*/
FKERN_API f8_u8 * ke_get_io_memory(
	HF8_KERNEL _kernel, 
	f8_u16 device
	)
{
	struct kernel_t * kernel = (struct kernel_t*)_kernel;
	if(device >= IO_MAX_DEVICES){
		return 0;
	}
	return &g_kernel.io_memory[device][0];
}

FKERN_API HF8_KERNEL ke_alloc_kernel()
{
	struct kernel_t * kernel;
	kernel = (struct kernel_t *)mm_alloc(
		kern_heap, 
		sizeof(struct kernel_t), 
		0
		);
	if(kernel){
		memset(kernel, 0, sizeof(struct kernel_t));
		init_kernel(kernel);
	}
	return kernel;
}

FKERN_API void ke_free_kernel(HF8_KERNEL _kernel)
{
	struct kernel_t * kernel = (struct kernel_t*)_kernel;
	uninit_kernel(_kernel);
	mm_free(kern_heap, _kernel);
}

/*
	The default kernel is a global instance of kernel_t.
	Only F8 executive is permited to use this kernel. Other
	components, such as IKAdapter/IKproxy should work on
	their private kernel objects.
*/
FKERN_API HF8_KERNEL ke_get_default_kernel()
{
	return &g_kernel;
}

FKERN_API struct io_device * ke_get_device_list(
	HF8_KERNEL _kernel
	)
{
	struct kernel_t * kernel = (struct kernel_t*)_kernel;
	return kernel->devices;
}

FKERN_API f8_bool ke_get_flag(int idx)
{
	return (g_kernel.status.flags& (1<<idx))? 1 :0;
}

FKERN_API void ke_set_flag(int idx, f8_bool bit)
{
	if(bit){
		g_kernel.status.flags |= (1<<idx);
	}else{
		g_kernel.status.flags &= ~(1<<idx);
	}
}

FKERN_API f8_u32 ke_get_flags()
{
	return g_kernel.status.flags;
}

FKERN_API f8_u32 ke_get_peer_flags()
{
	return g_kernel.peer_status.flags;
}

FKERN_API void ke_toggle_flag(int idx)
{
	ke_set_flag(idx, !ke_get_flag(idx));
}

FKERN_API f8_bool ke_get_peer_flag(int idx)
{
	return (g_kernel.peer_status.flags & (1<<idx))? 1 :0;
}

FKERN_API const char * ke_get_mode_name(f8_u8 s)
{
#define _(n) case KERN_S_##n:return #n
	switch(s){
	_(STOPPED);
	_(RUNNING);
	_(HALTED);
	_(STANDBY);
	_(ARBITRATING);
	default:
		assert(0);
		return "unknown";
	};
#undef _
}

FKERN_API int ke_time_to_string(ktime_t t, char * buf, int len)
{
	SYSTEMTIME st;
	ke_time_to_systime(&t, &st);
#ifdef _WIN32
	return _snprintf(buf,len,"%04d-%02d-%02d %02d:%02d:%02d %03d", 
		st.wYear, st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
#else
	return sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d %03d", 
		st.wYear, st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
#endif
}

FKERN_API int ke_reltime_to_string(ktime_t t, char * buf, int len)
{
	SYSTEMTIME st;

	t/=10*1000*1000;
	st.wSecond=(f8_u16)(t % 60);
	t=t/60; // in seconds
	
	st.wMinute=(f8_u16)(t %60);
	t=t/60; // in minutes
	
	st.wHour=(f8_u16)(t % 24);

#ifdef _WIN32
	return _snprintf(buf,len,"%d days, %d hours %d minutes %d seconds", 
		(f8_u32)(t/24),st.wHour,st.wMinute,st.wSecond);
#else
	return sprintf(buf,"%d days, %d hours %d minutes %d seconds", 
		(f8_u32)(t/24),st.wHour,st.wMinute,st.wSecond);
#endif
}

FKERN_API ktime_t ke_iotime_to_ktime(double iotime)
{
	return (ktime_t)(iotime*1e7);
}

FKERN_API double ke_ktime_to_iotime(ktime_t t)
{
	return (double)t / 1e7;
}

