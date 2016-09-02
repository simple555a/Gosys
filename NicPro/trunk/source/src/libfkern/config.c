/********************************************************************
	created:	24:3:2003   11:06
	filename:  D:\cvs\pmc2\ctrller\fkern\kern.c
	author: Jackie Hunter
	
	purpose:	F8 executive configuration stub
*********************************************************************/
#include "precomp.h"
#include "zlib.h"
#ifdef __VXWORKS__
#include "stat.h"
#endif

#if defined(_WIN32) && !defined(NDEBUG) && 0
void _trace_blk(IBlk * blk, const char * action)
{
	char path[F8_MAX_PATH];
	*path = 0;
	IBlk_path_by_blk(blk, path, sizeof(path));
	KD_PRINT(("%s block %s\n", action, path));
}
#else
#define _trace_blk(blk, action)
#endif

#define _err_exit(code)	return code

static __inline __bool _extract_uuid(IMemStream * s, f8_uuid *id)
{
	*id = *((f8_uuid*)(s->buffer + s->pointer));
	s->pointer += sizeof(f8_uuid);
	return s->pointer > s->length? __false : __true;
}

static __inline IBlk * _extract_blk(IMemStream * _s)
{
	IF8Stream * s;
	s = __ucast__(IMemStream, IF8Stream, _s);
	return IBlk_from_stream(s);
}

static __inline __bool _extract_u16(IMemStream * s, __u16 *i)
{
	if(s->pointer + sizeof(*i) > s->length){
		return __false;
	}
	*i = *((__u16*)(s->buffer + s->pointer));
	s->pointer += sizeof(*i);
	return __true;
}

static __inline IBlk * _extract_blk_by_uuid(
	struct kernel_t * kernel, 
	IMemStream * s
	)
{
	f8_uuid id;
	if(!_extract_uuid(s, &id)){
		return 0;
	}
	return ke_get_blk(kernel, &id);
}

static __inline struct blk_pin_t * _extract_pin(
	struct kernel_t * kernel, 
	IMemStream *s)
{
	IBlk * blk;
	__u16 pid;

	blk = _extract_blk_by_uuid(kernel, s);
	if(!blk){
		return 0;
	}
	if(!_extract_u16(s, &pid)){
		return 0;
	}

	return __vcall__(blk, pin_by_id, (blk, pid));
}

/*
		+--------------+
		|              |
		+--+        +--+
		   |        |
		+--+        +--+
	A---|bool      bool|--- C
		|              |
	B---|char    double|--- D
		+--------------+
*/
FKERN_API f8_bool is_address_valid(
	HF8_KERNEL _kernel,
	const struct blk_pin_t *p, 
	const struct mem_addr_t * addr
	)
{
	struct kernel_t * kernel = (struct kernel_t*)_kernel;

	if(!_is_valid_section(addr->section)){
		return __false;
	}
	
	if(p){
		switch(p->type){
		case PIN_T_BOOL:
			if(addr->section != MEM_SECTION_BIT){
				return __false;
			}
			break;
		case PIN_T_BYTE:
		case PIN_T_CHAR:
			if(addr->section != MEM_SECTION_BYTE){
				return __false;
			}
			break;
		case PIN_T_WORD:
		case PIN_T_SHORT:
			break;
			if(addr->section != MEM_SECTION_WORD){
				return __false;
			}
		case PIN_T_INTEGER:
		case PIN_T_DWORD:
		case PIN_T_FLOAT:
			if(addr->section != MEM_SECTION_R4){
				return __false;
			}
			break;
		case PIN_T_DOUBLE:
		case PIN_T_DATE:
			if(addr->section != MEM_SECTION_R8){
				return __false;
			}
			break;
		default:
			if(addr->section != MEM_SECTION_ANYSIZE){
				return __false;
			}
			break;
		}
	}

	if(addr->addr >= kernel->x_mem_sizes[addr->section]){
		return __false;
	}

	return __true;
}

/*
	_redirect()

	redirect a pin's memory dynamic pin pointer.

	if address is not valid, then we continue processing,
	thus give the configurator a chance to correct it later.

	Note, the `mdp field is set to zero in IBLk_from_stream, and
	it can only be set to a non-zero value in here, so we can
	safely assume that the memory pointer is not bound if the `mdp'
	is zero.
*/
static __inline __bool _redirect(
	struct kernel_t * kernel, 
	struct blk_pin_t *p, 
	struct mem_addr_t * addr
	)
{
	f8_u8	* reg;
	struct blk_pin_t * peer;
	PTRIPLE_LIST_ENTRY lk;

	if(PIN_B_CONST == p->binding){
		p->mdp = 0;
		return __true;
	}
	
	if(p->mdp){
		/* already bound */
		return __true;
	}
	
	if(p->_class == PIN_CLASS_DI){
		/* 
			an input pin reuses the peer's address
			the addr parameter is ignored
		*/
		if(!RtkIsTripleListEmpty(&p->u1.link)){
			peer = IBlk_peer(p);
			assert(peer);
			/*
				the pin will have the value on the peer's memory
				register.
			*/			
			if(!peer->mdp){
				if(!_redirect(kernel, peer, &peer->u2.mem_addr)){
					return __false;
				}
			}
			p->mdp = peer->mdp;
			p->u2.mem_addr = peer->u2.mem_addr;			
			return __true;
		}else{
			if(!is_address_valid(kernel, p, addr)){
				return __false;
			}
		}
	}else{
		if(!is_address_valid(kernel, p, addr)){
			lk = p->u1.link.Flink;
			while(lk != &p->u1.link){
				peer = RTK_CONTAINING_RECORD(lk, struct blk_pin_t, u1.link);
				peer->mdp = 0;
				peer->u2.mem_addr = *addr;
				lk = lk->Flink;
			}
			return __false;
		}
	}
	
	reg = kernel->x_mem_pointers[addr->section];
	reg += addr->addr;
	
	p->mdp = (struct dyn_pin_t*)reg;
	if(addr != &p->u2.mem_addr){
		p->u2.mem_addr = *addr;
	}

	if(p->_class == PIN_CLASS_DO){
		lk = p->u1.link.Flink;
		while(lk != &p->u1.link){
			peer = RTK_CONTAINING_RECORD(lk, struct blk_pin_t, u1.link);
			peer->mdp = (struct dyn_pin_t*)reg;
			peer->u2.mem_addr = *addr;
			lk = lk->Flink;
		}
	}
		
	return __true;
}

static void _insert_1131_blk(struct kernel_t * kernel, IBBlk * b)
{
	IBBlk * bp;
#ifdef _1131_LST_DEBUG
	int po = -1;
#endif
	RTK_LIST_ENTRY * li;

	li = kernel->m_1131_blocks.Flink;
	while(li != &kernel->m_1131_blocks){
		bp = RTK_CONTAINING_RECORD(li, IBBlk, bh.x_link);
		if(bp->bh.x_order > b->bh.x_order){
#ifdef _1131_LST_DEBUG
			po = bp->bh.x_order;
#endif
			break;
		}
		li = li->Flink;
	}
	li = li->Blink;
#ifdef _1131_LST_DEBUG
	printf("Inserting %d before %d\n", b->bh.x_order, po);
#endif
	
	RtkInsertHeadList(li, &b->bh.x_link);
}

/*
	when a block is mounted into a existing section, some pointers
	must be set in the blk object. if it's a basic block, then
	check if some resources should be allocated(timers for now)
*/
static __bool _traverse_mount(ITreeNode *nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	IBBlk * bb;
	int i;
	struct blk_pin_t * p;
	struct kernel_t * kernel = (struct kernel_t*)context;

	b = __dcast__(ITreeNode, IBlk, nd);
	kernel->current_blk = b;
	_trace_blk(b, "Mounting");

	ke_set_hash(kernel->blk_hash, &b->uuid, b);

	if(b->h.magic != BBLK_MAGIC){
		return __true;
	}
	
	bb = __dcast__(IBlk, IBBlk, b);
	kernel->current_bblk = bb;
	
	/*
		do ELF relocating stuff
	*/
	*((BLK_SERVICE_PROC**)bb->bss_section - 1) = blk_services;

	for(i=0, p=bb->pins; i<b->h.n_pins; i++, p++){
		if(p->type == PIN_T_TIMER){
			/* insert a timer */
			if(p->_class == PIN_CLASS_EO){
				ki_sched(
					kernel,
					p->dp->u.e.interval,
					p,
					TIMER_T_PERIODIC
					);
				if(p->dp->u.e.tid < 0){
					ki_logblk(
						kernel,
						F8_EVENT(MODNUM_KERNEL, EVT_CLASS_FATAL, F8_NO_TIMER),
						b,
						0);
				}
			}else{
				ki_logblk(
					kernel,
					F8_EVENT(MODNUM_KERNEL, EVT_CLASS_FATAL, F8_GENERAL_CONFIG_ERR),
					b, 
					0
					);
			}			
		}
		if(p->_class == PIN_CLASS_DI || p->_class == PIN_CLASS_DO){
			_redirect(kernel, p, &p->u2.mem_addr);
		}
	}

	if(b->h.flags & BLK_F_AUTOSCAN){
		_insert_1131_blk(kernel, bb);
	}else{
		RtkInitializeListHead(&bb->bh.x_link);
	}

	if(KERN_S_RUNNING == kernel->status.state){
		_trace_blk(b, "Initing");
		ki_init_bblk(kernel, bb);
	}

	/* update status */
	kernel->status.n_blocks++;
	
	return __true;
}

static __bool _traverse_umount(ITreeNode *nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	IBBlk * bb;
	int i;
	struct blk_pin_t *p, **ep;
	struct kernel_t * kernel = (struct kernel_t*)context;

	b = __dcast__(ITreeNode, IBlk, nd);
	kernel->current_blk = b;

	_trace_blk(b, "Unmounting");

	if(b->h.magic == BBLK_MAGIC){		
		bb = __dcast__(IBlk, IBBlk, b);
		kernel->current_bblk = bb;
		for(i=0, p=bb->pins; i<b->h.n_pins; i++, p++){
			if(p->type == PIN_T_TIMER){
				/* insert a timer */
				assert(p->_class == PIN_CLASS_EO);
				if(p->dp->u.e.tid >= 0){
					ki_cancel(kernel, p->dp->u.e.tid, p);
				}
			}
		}

		if(KERN_S_RUNNING == kernel->status.state){
			_trace_blk(b, "Finalizing");
			ki_finalize_bblk(kernel, bb);
		}

		for(i=kernel->eq_head; i!=kernel->eq_end;){
			ep = kernel->event_q + i;
			if((*ep) != PIN_OF_UNMOUNTED_BLOCK){
				if((*ep)->blk == b){
					*ep = PIN_OF_UNMOUNTED_BLOCK;
				}
			}
			i++;
			i %= EVENT_Q_LENGTH;
		}
		if(b->h.flags & BLK_F_AUTOSCAN){
			RtkRemoveEntryList(&bb->bh.x_link);
		}
		/* update status */
		kernel->status.n_blocks--;
	}

	ke_del_hash(kernel->blk_hash, &b->uuid);

	return __true;
}

static f8_status F8_MOUNT_BLOCK_handler(struct kernel_t * kernel, struct kern_conf_cmd *cmd)
{
	IBlk		*peer, *blk;
	ICBlk		*cpeer;
	IMemStream	_s;
	f8_uuid		pid;

	cmd->osize = 0;

	if(OLD_ID(cmd) != kernel->sections->h.uuid){
		_err_exit(F8_VERSION_MISMATCH);
	}

	__create__(IMemStream, &_s);
	IMemStream_bind(&_s, cmd->ibuffer, cmd->isize);

	if(!_extract_uuid(&_s, &pid)){
		_err_exit(F8_INVALID_DATA);
	}
	
	peer = ke_get_blk(kernel, &pid);
	if(!peer){
		_err_exit(F8_PATH_NOT_FOUND);
	}

	if(peer->h.magic != CBLK_MAGIC){
		_err_exit(F8_NOT_A_CBLK);
	}
	cpeer = __dcast__(IBlk, ICBlk, peer);

	blk = _extract_blk(&_s);
	if(!blk){
		_err_exit(F8_INVALID_DATA);
	}

	if(!ICBlk_add_blk(cpeer, blk)){
		delete_blk(blk);
		_err_exit(F8_ADD_BLOCK_FAILED);
	}

	/* */
	_traverse(blk, _traverse_mount, kernel);

	_err_exit(F8_SUCCESS);
}

/*
	umount a block from tree
	if the block to be umounted is an section, then destroy 
	associated section.
*/
static f8_status F8_UMOUNT_BLOCK_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	f8_uuid	bid;
	IBlk * blk;
	IMemStream s;
	__uint i;

	cmd->osize = 0;
	
	if(OLD_ID(cmd) != kernel->sections->h.uuid){
		_err_exit(F8_VERSION_MISMATCH);
	}

	__create__(IMemStream, &s);
	IMemStream_bind(&s, cmd->ibuffer, cmd->isize);
	
	if(cmd->isize % sizeof(f8_uuid)){
		_err_exit(F8_INVALID_DATA);
	}

	for(i=0; i<cmd->isize; i+=sizeof(f8_uuid)){
		if(!_extract_uuid(&s, &bid)){
			_err_exit(F8_INVALID_DATA);
		}

		blk = ke_get_blk(kernel, &bid);
		if(!blk){
			_err_exit(F8_PATH_NOT_FOUND);
		}

		/* cannot umount '/' */
		if(blk == kernel->sections){
			_err_exit(F8_PERMISSION);
		}

		_traverse(blk, _traverse_umount, kernel);
		
		umount_blk(blk);
	}

	_err_exit(F8_SUCCESS);
}


static f8_status F8_GET_PROG_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	IMemStream ss;
	IF8Stream * s;

	__create__(IMemStream, &ss);
	IMemStream_bind(&ss, cmd->obuffer, cmd->osize);
	s = __ucast__(IMemStream, IF8Stream, &ss);

	if(!__vcall__(kernel->sections, save, (kernel->sections, s))){
		cmd->osize = 0;
		_err_exit(F8_BUFFER_TOO_SMALL);
	}
	cmd->osize = __vcall__(s, tell, (s));
	OLD_ID(cmd) = kernel->sections->h.uuid;
	NEW_ID(cmd) = BLK_ID_NONE;

	_err_exit( F8_SUCCESS );
}

/*
	umount all children of kernel->sections
	clear global timer database
	clear event queue
*/
static f8_status F8_RESET_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	ki_reset(kernel);
	_err_exit(F8_SUCCESS);
}

static f8_status F8_PUT_PROG_handler(struct kernel_t * kernel, struct kern_conf_cmd *cmd)
{
	IMemStream ss;
	IF8Stream *s;

	ki_clear_user_program(kernel);
	
	cmd->osize = 0;

	__create__(IMemStream, &ss);
	IMemStream_bind(&ss, cmd->ibuffer, cmd->isize);
	s = __ucast__(IMemStream, IF8Stream, &ss);

	if(!__vcall__(kernel->sections, load, (kernel->sections, s))){
		_err_exit(F8_INVALID_DATA);
	}

	/* the low-memory condition can only be detected this way */
	if(!__vcall__(s, eos, (s))){
		_err_exit(F8_LOW_MEMORY);
	}

	/* build kernel->sections */
	_traverse(kernel->sections, _traverse_mount, kernel);

	ki_log(kernel, F8_PROG_DOWNLOADED, 0, 0, 0);
	// added by cj @ 2009-6-30
	ki_start_1();

	_err_exit(F8_SUCCESS);
}

static f8_status F8_LINK_handler(struct kernel_t * kernel, struct kern_conf_cmd *cmd)
{
	IMemStream ms;
	IF8Stream *str;
	struct blk_pin_t *s, *t, *tt;
	f8_uuid id;
	struct cblk_link_t * lk;

	cmd->osize = 0;

	if(OLD_ID(cmd) != kernel->sections->h.uuid){
		_err_exit(F8_VERSION_MISMATCH);
	}

	__create__(IMemStream, &ms);
	IMemStream_bind(&ms, cmd->ibuffer, cmd->isize);
	str = __ucast__(IMemStream, IF8Stream, &ms);

	if(!_extract_uuid(&ms, &id)){
		_err_exit(F8_INVALID_DATA);
	}
	s = _extract_pin(kernel, &ms);
	if(!s){
		_err_exit(F8_PATH_NOT_FOUND);
	}
	t = _extract_pin(kernel, &ms);
	if(!t){
		_err_exit(F8_PATH_NOT_FOUND);
	}
	if(!IBlk_is_output(s)){
		tt = s;
		s = t;
		t = tt;
	}
	lk = ICBlk_link(&id, s, t);
	if(!lk){
		_err_exit( F8_INVALID_DATA );
	}
	tt = IBlk_real_pin(s);
	if(tt->_class == PIN_CLASS_DO){
		tt = IBlk_real_pin(t);
		tt->mdp = 0;
		_redirect(kernel, tt, 0);
	}
	
	_err_exit( F8_SUCCESS );
}

static f8_status F8_UNLINK_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	IMemStream ms;
	IBlk * blk;
	ICBlk * cblk;
	f8_uuid id;
	struct cblk_link_t *lk;

	cmd->osize = 0;
	
	if(OLD_ID(cmd) != kernel->sections->h.uuid){
		_err_exit(F8_VERSION_MISMATCH);
	}

	__create__(IMemStream, &ms);
	IMemStream_bind (&ms, cmd->ibuffer, cmd->isize);

	blk = _extract_blk_by_uuid(kernel, &ms);
	if(!blk){
		_err_exit(F8_PATH_NOT_FOUND);
	}
	if(!_extract_uuid(&ms, &id)){
		_err_exit(F8_INVALID_DATA);
	}
	if(blk->h.magic != CBLK_MAGIC){
		_err_exit(F8_NOT_A_CBLK);
	}
	cblk = __dcast__(IBlk, ICBlk, blk);
	lk = ICBlk_get_link(cblk, &id);
	if(!lk){
		_err_exit(F8_OBJECT_NOT_FOUND);
	}
	if(!ICBlk_unlink(lk)){
		_err_exit(F8_INTERNAL_ERROR);
	}

	_err_exit( F8_SUCCESS );
}

/*
	syntax : expname, pinpath
*/
static f8_status F8_EXPORT_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	IMemStream ms;
	f8_uuid	id;
	struct blk_pin_t * p;

	cmd->osize = 0;
	if(OLD_ID(cmd) != kernel->sections->h.uuid){
		_err_exit(F8_VERSION_MISMATCH);
	}

	__create__(IMemStream, &ms);
	IMemStream_bind (&ms, cmd->ibuffer, cmd->isize);

	if(!_extract_uuid(&ms, &id)){
		_err_exit(F8_INVALID_DATA);
	}
	p = _extract_pin(kernel, &ms);
	if(!p){
		_err_exit(F8_PATH_NOT_FOUND);
	}
	if(!ICBlk_add_pin(&id, p)){
		_err_exit(F8_INVALID_OPERATION);
	}
	
	_err_exit(F8_SUCCESS);
}

static f8_status F8_UNEXPORT_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	IMemStream ms;
	struct blk_pin_t * p;

	cmd->osize = 0;
	if(OLD_ID(cmd) != kernel->sections->h.uuid){
		_err_exit(F8_VERSION_MISMATCH);
	}

	__create__(IMemStream, &ms);
	IMemStream_bind (&ms, cmd->ibuffer, cmd->isize);
	p = _extract_pin(kernel, &ms);
	if(!p){
		_err_exit(F8_PATH_NOT_FOUND);
	}
	if(!ICBlk_del_pin(p)){
		_err_exit(F8_INVALID_OPERATION);
	}
	
	_err_exit( F8_SUCCESS );
}

/*
	modify inversion memory address/constant value
*/
static f8_status F8_MODIFY_PIN_handler(struct kernel_t * kernel, struct kern_conf_cmd *cmd)
{
	IMemStream ms;
	struct blk_pin_t *p, *rp;
	struct conf_mod_pin *mp;

	cmd->osize = 0;
	if(OLD_ID(cmd) != kernel->sections->h.uuid){
		_err_exit(F8_VERSION_MISMATCH);
	}
	__create__(IMemStream, &ms);	
	IMemStream_bind (&ms, cmd->ibuffer, cmd->isize);
	p = _extract_pin(kernel, &ms);
	if(!p){
		_err_exit(F8_PATH_NOT_FOUND);
	}
	rp = IBlk_real_pin(p);
	mp = (struct conf_mod_pin*)(ms.buffer + ms.pointer);
	p->binding = mp->binding;
	p->mdp = 0;
	if(PIN_B_CONST != mp->binding){		
		if(!_redirect(kernel, p, &mp->u.addr)){
			_err_exit(F8_INVALID_ADDRESS);
		}
	}else{
		if(p->_class != PIN_CLASS_DI){
			_err_exit(F8_INVALID_OPERATION);
		}
#define _trans(type)\
	*((type*)p->dp) = *((type*)&mp->u.dval);
		switch(p->type){
		case PIN_T_BOOL:
			((__bool*)p->dp)[4] = *((__bool*)&mp->u.dval);
			if(p->flags & PIN_F_INVERTED){
				p->dp->u.d.i8 = !((__bool*)p->dp)[4];
			}else{
				p->dp->u.d.i8 = ((__bool*)p->dp)[4];
			}
			break;
		case PIN_T_CHAR:
		case PIN_T_BYTE:
			_trans(__i8);
			break;
		case PIN_T_WORD:
		case PIN_T_SHORT:
			_trans(__i16);
			break;
		case PIN_T_DWORD:
		case PIN_T_INTEGER:
		case PIN_T_FLOAT:
			_trans(__i32);
			break;
		case PIN_T_DOUBLE:
		case PIN_T_DATE:
			_trans(__i64);
			break;
		}
#undef _trans
	}	
	_err_exit(F8_SUCCESS);
}

static f8_status F8_GET_VERSION_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	OLD_ID(cmd) = kernel->sections->h.uuid;
	cmd->osize = 0;
	_err_exit(F8_SUCCESS);
}

static f8_status F8_SET_INVERT_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	IMemStream ms;
	struct blk_pin_t *p, *rp;

	cmd->osize = 0;
	if(OLD_ID(cmd) != kernel->sections->h.uuid){
		_err_exit(F8_VERSION_MISMATCH);
	}
	__create__(IMemStream, &ms);
	IMemStream_bind (&ms, cmd->ibuffer, cmd->isize);
	p = _extract_pin(kernel, &ms);
	if(!p){
		_err_exit(F8_PATH_NOT_FOUND);
	}
	rp = IBlk_real_pin(p);
	if(ms.length != ms.pointer + 1){
		_err_exit(F8_INVALID_DATA);
	}
	if(p->type != PIN_T_BOOL){
		_err_exit(F8_TYPE_MISMATCH);
	}
	if(ms.buffer[ms.pointer]){
		p->flags |= PIN_F_INVERTED;
	}else{
		p->flags &= ~PIN_F_INVERTED;
	}
	if(p->dp == p->mdp){
		p->mdp->u.d.ui8 = !p->mdp->u.d.ui8;
	}
	_err_exit(F8_SUCCESS);
}

static f8_status F8_GET_SYS_CONFIG_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	struct ctrller_config * cfg;
	f8_u16 i;
	struct io_device * d, * dd;

	if(cmd->osize < sizeof(struct ctrller_config)){
		_err_exit(F8_BUFFER_TOO_SMALL);
	}
	
	cmd->osize = sizeof(struct ctrller_config);
	cfg = (struct ctrller_config*)cmd->obuffer;
	memset(cfg, 0, sizeof(*cfg));

	cfg->uuid = kernel->sections->h.uuid;
	cfg->cpuType = KERN_CPU_TYPE;
	cfg->poolSize = KERN_POOL_SIZE;
	memcpy(
		cfg->x_mem_sizes, 
		kernel->x_mem_sizes, 
		sizeof(kernel->x_mem_sizes)
		);
	d = cfg->devices;
	dd = kernel->devices;
	for(i=0; i<IO_MAX_DEVICES; i++, d++, dd++){
		*d = *dd;
		assert(d->addr == i);
		if(d->features & IO_DEVICE_PRESENT){
			cfg->deviceCount++;
		}
	}
	_err_exit(F8_SUCCESS);
}

static f8_status F8_SET_SYS_CONFIG_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	f8_u16 i;
	struct ctrller_config * cfg;
	struct io_device * d, * dd;
	
	if(cmd->isize < sizeof(struct ctrller_config)){
		_err_exit(F8_INVALID_DATA);
	}
	cmd->osize = 0;
	
	if(kernel->status.state != KERN_S_STOPPED){
		_err_exit(F8_ACCESS);
	}

	cfg = (struct ctrller_config*)cmd->ibuffer;

	cfg->poolSize = 0;
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		cfg->x_mem_sizes[i] = ((cfg->x_mem_sizes[i]+3)&(~3));
		cfg->poolSize += cfg->x_mem_sizes[i];
		if(!(cfg->x_mem_sizes[i] >=0 && cfg->x_mem_sizes[i]<64*1024)){
			_err_exit(F8_LOW_REGISTERS);
		}
	}
	
	if(cfg->poolSize >= KERN_POOL_SIZE){
		_err_exit(F8_LOW_REGISTERS);
	}
	
	/* set memory size */
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		kernel->x_mem_sizes[i] = cfg->x_mem_sizes[i];
	}
	ki_init_mem_pointers(kernel);

	/* set device config */
	d = cfg->devices;
	dd = kernel->devices;
	for(i=0; i<IO_MAX_DEVICES; i++, d++, dd++){
		*dd = *d;
		dd->addr = i;
		if(dd->n_ai || dd->n_di){
			dd->features |= IO_DEVICE_INPUT;
		}
		if(dd->n_ao || dd->n_do){
			dd->features |= IO_DEVICE_OUTPUT;
		}
	}

	_err_exit(F8_SUCCESS);
}

static f8_status F8_GET_X_MEMORY_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	int i;
	__uint msize;
	char *p;

	msize = 0;
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		msize += kernel->x_mem_sizes[i];
	}
	if(cmd->osize < msize){
		cmd->osize = 0;
		_err_exit(F8_BUFFER_TOO_SMALL);
	}
	p = cmd->obuffer;
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		if(kernel->x_mem_sizes[i] != 0){
			memcpy(p, kernel->x_mem_pointers[i], kernel->x_mem_sizes[i]);
			p += kernel->x_mem_sizes[i];
		}				
	}
	cmd->osize = msize;
	_err_exit(F8_SUCCESS);
}

static f8_status F8_GET_MEM_VALUE_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	_err_exit(F8_UNIMPLEMENTED);
}

// static f8_status F8_SET_MEM_VALUE_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
// {
// 	struct conf_mod_mem * mp;
// 	f8_byte * pp;
// 
// 	if(cmd->isize != sizeof(struct conf_mod_mem)){
// 		_err_exit(F8_INVALID_DATA);
// 	}
// 	if(OLD_ID(cmd) != kernel->sections->h.uuid){
// 		_err_exit(F8_VERSION_MISMATCH);
// 	}
// 	mp = (struct conf_mod_mem*)cmd->ibuffer;
// 	if(!is_address_valid(kernel, 0, &mp->addr)){
// 		_err_exit(F8_INVALID_DATA);
// 	}
// 	if(mp->len > 8){
// 		_err_exit(F8_INVALID_DATA);
// 	}
// 	pp = kernel->x_mem_pointers[mp->addr.section];
// 	pp += mp->addr.addr;
// 	memcpy(pp, &mp->value, mp->len);
// 	cmd->osize = 0;
// 	_err_exit(F8_SUCCESS);
// }

//Zander  2012-10-17 
//修改原因：增加多个标签值写入控制器的功能
 static f8_status F8_SET_MEM_VALUE_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
 {
 	struct conf_mod_mem * mp;
 	f8_byte * pp;
	f8_u32 num;
	f8_u32 i;
 
 	if ( (cmd->isize)%(sizeof(struct conf_mod_mem)) != 0 )
 	{
 		_err_exit(F8_INVALID_DATA);
 	}
 	if (OLD_ID(cmd) != kernel->sections->h.uuid)
 	{
 		_err_exit(F8_VERSION_MISMATCH);
 	}

	num = (f8_u32)(cmd->isize/sizeof(struct conf_mod_mem));
	mp = (struct conf_mod_mem*)cmd->ibuffer;
	for (i=0; i!=num; i++)
	{
		if(!is_address_valid(kernel, 0, &mp->addr)){
			_err_exit(F8_INVALID_DATA);
		}
		if(mp->len > 8){
			_err_exit(F8_INVALID_DATA);
		}
		pp = kernel->x_mem_pointers[mp->addr.section];
		pp += mp->addr.addr;
		memcpy(pp, &mp->value, mp->len);
	    cmd->osize = 0;
		mp++;
	}	
	_err_exit(F8_SUCCESS);	
 }
 

static f8_status F8_GET_LOG_BUF_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	if(cmd->osize < sizeof(struct klog_t)){
		cmd->osize = 0;
		_err_exit(F8_BUFFER_TOO_SMALL);
	}
	memcpy(cmd->obuffer, &kernel->klog, sizeof(struct klog_t));
	cmd->osize = sizeof(struct klog_t);
	_err_exit(F8_SUCCESS);
}

static f8_status F8_GET_STATUS_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	if(cmd->osize < sizeof(struct kstatus_t)){
		cmd->osize = 0;
		_err_exit(F8_BUFFER_TOO_SMALL);
	}
	memcpy(cmd->obuffer, &kernel->status, sizeof(struct kstatus_t));
	cmd->osize = sizeof(struct kstatus_t);
	_err_exit(F8_SUCCESS);
}

static f8_status F8_SET_STATUS_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	cmd->osize = 0;
	_err_exit(F8_UNIMPLEMENTED);
}

static f8_status F8_CLEAR_LOG_BUF_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	cmd->osize = 0;
	memset(&kernel->klog, 0, sizeof(struct klog_t));
	_err_exit(F8_SUCCESS);
}

static f8_status F8_SET_DEVICE_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	f8_u16 count;
	char * p;
	struct io_device * d;
	f8_u16 i;

	cmd->osize = 0;

	if(cmd->isize < sizeof(f8_u16)){
		_err_exit(F8_INVALID_DATA);
	}
	p = cmd->ibuffer;
	count = *(f8_u16*)p;
	p += sizeof(count);
	if(cmd->isize != sizeof(count) + count * sizeof(struct io_device)){
		_err_exit(F8_INVALID_DATA);
	}
	d = (struct io_device*)p;

	if(count >= IO_MAX_DEVICES){
		_err_exit(F8_INVALID_DATA);
	}

	for(i=0; i<count; i++, d++){
		if(d->addr >= IO_MAX_DEVICES){
			_err_exit(F8_INVALID_DATA);
		}
	}
	
	d = (struct io_device*)p;
	for(i=0; i<count; i++, d++){
		kernel->devices[d->addr] = *d;
		io_write_config(d);
	}
	
	_err_exit(F8_SUCCESS);
}

static f8_status F8_START_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	cmd->osize = 0;
	if(kernel==&g_kernel)
		ke_set_flag(FKERN_LED_SOFT_STOP,0);
	if(&g_kernel == kernel){
		if(g_kernel.status.flags & KERN_F_ENABLE_STANDBY){
			return ki_switch_to(KERN_S_ARBITRATING);
		}
	}
	return ki_switch_to(KERN_S_RUNNING);
}

/*
	reset a section to ready state
*/
static f8_status F8_STOP_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{	
	cmd->osize = 0;
	if(kernel==&g_kernel)
		ke_set_flag(FKERN_LED_SOFT_STOP,1);
	return ki_switch_to(KERN_S_STOPPED);
}

static f8_status F8_BREAK_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	return F8_INVALID_OPERATION;
}

static f8_status F8_CONTINUE_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	return F8_INVALID_OPERATION;
}

static f8_status F8_SUSPEND_STATE_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	cmd->osize = 0;
	return ki_suspend_to_nvram(kernel);
}

static f8_status F8_RESUME_STATE_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	cmd->osize = 0;
	return ki_resume_from_nvram(kernel);
}

static f8_status F8_GET_DEV_STATUS_handler(struct kernel_t * kernel, struct kern_conf_cmd * cmd)
{
	if(cmd->osize < sizeof(struct module_status_t)*(IO_MAX_DEVICES+IO_MAX_STATIONS)){
		cmd->osize=0;
		return F8_BUFFER_TOO_SMALL;
	}
	memcpy(cmd->obuffer,g_kernel.stationStatus,sizeof(struct station_status_t)*IO_MAX_STATIONS);
	memcpy(
		(char*)cmd->obuffer+sizeof(struct station_status_t)*IO_MAX_STATIONS,
		g_kernel.moduleStatus,
		sizeof(struct module_status_t)*IO_MAX_DEVICES
		);
	cmd->osize=sizeof(struct module_status_t)*(IO_MAX_DEVICES+IO_MAX_STATIONS);
	return F8_SUCCESS;
}


static f8_status F8_TARGET_EXEC_handler(struct kernel_t * kernel,struct kern_conf_cmd * cmd)
{
	cmd->osize=0;
#ifdef __VXWORKS__
{
	char cmdBuf[1024];
	char resultFileName[32];
	int fd;
	unsigned short randomName=pentiumTscGet32();
	
	// cmd->ibuffer[cmd->isize]=0;
	strncpy(cmdBuf,cmd->ibuffer,sizeof(cmdBuf));
	sprintf(resultFileName,"/ram/%04d.txt",randomName);
	strncat(cmdBuf," > ",sizeof(cmdBuf));
	strncat(cmdBuf,resultFileName,sizeof(cmdBuf));
	printf("Execute:`%s'\n",cmdBuf);
	execute(cmdBuf);
	fd=open(resultFileName);
	if(fd != ERROR){
		struct stat st;
		fstat(fd,&st);
		if(st.st_size > cmd->osize)
			cmd->retcode=F8_BUFFER_TOO_SMALL;
		else{
			read(fd,cmd->obuffer,st.st_size);
			cmd->retcode=F8_SUCCESS;
			cmd->osize=st.st_size;
		}
		close(fd);
		//unlink(resultFileName);
	}else{
		cmd->retcode=F8_FILE_NOT_FOUND;
	}
}
#else
	cmd->retcode=F8_UNIMPLEMENTED;
#endif
	
	return cmd->retcode;
}


typedef f8_status (*CONFIG_PROC)(struct kernel_t * kernel, struct kern_conf_cmd *);
struct cpmap_entry_t cpmap[MAX_KERN_COMMAND];

void init_config()
{
	assert(sizeof(struct module_status_t)==sizeof(struct station_status_t));
	
	printf("Initializing kernel configuration map...\n");

	/* initialize the command map */
#define define_cmd(nm,v,exclusive,route,foo1,foo2,foo3)\
	cpmap[nm].accessMode = F8CMD_attr_##exclusive;\
	cpmap[nm].name=#nm;\
	cpmap[nm].routing=F8CMD_route_##route;

#include "f8_cmd.h"

	
#define _(code) cpmap[code].foo[0] = (int)code##_handler;

	_(F8_MOUNT_BLOCK);
	_(F8_UMOUNT_BLOCK);
	_(F8_START);
	_(F8_STOP);
	_(F8_BREAK);
	_(F8_CONTINUE);
	_(F8_GET_PROG);
	_(F8_PUT_PROG);
	_(F8_LINK);
	_(F8_UNLINK);
	_(F8_RESET);
	_(F8_EXPORT);
	_(F8_UNEXPORT);
	_(F8_GET_VERSION);
	_(F8_MODIFY_PIN);
	_(F8_SET_INVERT);
	_(F8_GET_SYS_CONFIG);
	_(F8_SET_SYS_CONFIG);
	_(F8_SET_MEM_VALUE);
	_(F8_GET_MEM_VALUE);
	_(F8_GET_X_MEMORY);
	_(F8_GET_LOG_BUF);
	_(F8_CLEAR_LOG_BUF);
	_(F8_GET_STATUS);
	_(F8_SET_STATUS);
	_(F8_SET_DEVICE);
	_(F8_SUSPEND_STATE);
	_(F8_RESUME_STATE);
	_(F8_GET_DEV_STATUS);
	_(F8_TARGET_EXEC);
}

FKERN_API void ke_config(HF8_KERNEL _kernel, struct kern_conf_cmd *cmd, __bool isConfigurator)
{
	struct cpmap_entry_t * ce;
	struct kernel_t * kernel = (struct kernel_t *)_kernel;

	cmd->retcode = F8_UNIMPLEMENTED;

	if((unsigned)cmd->cmd >= MAX_KERN_COMMAND){
		cmd->retcode = F8_UNIMPLEMENTED;
		return;
	}
	ce = &cpmap[cmd->cmd];
	if(!ce->foo[0]){
		cmd->retcode = F8_UNIMPLEMENTED;
		cmd->osize=0;
		return;
	}
	if(ce->accessMode == F8CMD_attr_exclusive && !isConfigurator){
		cmd->retcode = F8_DATABASE_LOCKED;
		cmd->osize=0;
		return;
	}

	if(ce->accessMode != F8CMD_attr_nolock){
		if(!ke_lock(_kernel, ce->accessMode)){
			cmd->retcode=F8_DATABASE_LOCKED;
			cmd->osize=0;
			return;
		}
	}
	
	cmd->retcode = ((CONFIG_PROC)ce->foo[0])(kernel, cmd);

	if(cmd->retcode == F8_SUCCESS && NEW_ID(cmd) != BLK_ID_NONE){
		kernel->sections->h.uuid = NEW_ID(cmd);
		kernel->status.prog_id = NEW_ID(cmd);
		kernel->bModified = f8_true;
	}

	if(ce->accessMode != F8CMD_attr_nolock){
		ke_unlock(_kernel, ce->accessMode);
	}

	return;
}

FKERN_API f8_byte ke_section_by_type(f8_byte type)
{
	switch(type){
	case PIN_T_BOOL:
		return MEM_SECTION_BIT;

	case PIN_T_CHAR:
		return MEM_SECTION_BYTE;
		
	case PIN_T_SHORT:
		return MEM_SECTION_WORD;
		
	case PIN_T_INTEGER:
		return MEM_SECTION_R4;
		
	case PIN_T_BYTE:
		return MEM_SECTION_BYTE;
		
	case PIN_T_WORD:
		return MEM_SECTION_WORD;
		
	case PIN_T_DWORD:
		return MEM_SECTION_R4;
		
	case PIN_T_FLOAT:
		return MEM_SECTION_R4;
		
	case PIN_T_DOUBLE:
		return MEM_SECTION_R8;
		
	case PIN_T_DATE:
		return MEM_SECTION_R8;
	}

	return MEM_SECTION_INVALID;
}

FKERN_API struct cpmap_entry_t * ke_get_config_map(void)
{
	return cpmap;
}

