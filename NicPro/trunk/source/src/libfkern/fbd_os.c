/*
	function block diagram interpreter.
	
	2003-8 panxiaohui@actech-mc.com
*/
#include "precomp.h"

typedef void (*F8_FUNCTION)(void);

/*
	Notes, _sample_in() and _sample_out() might be crucial
	to overall performance, maybe I should rewrite them in 
	assembly	language someday.

	N.B. it looks loke _sample_in and _sample_out forms a 'call by 
	value/result' programming scheme.
*/
static void _sample_in(IBBlk *b)
{
	int i;
	IBlk * blk;
	struct blk_pin_t *p;

	blk = __ucast__(IBBlk, IBlk, b);

	p = b->pins;
	for(i=0; i<blk->h.n_pins; i++, p++){		
		if(p->_class != PIN_CLASS_DI){
			continue;
		}
		
		if(p->mdp){
			// transfer some bytes from p->mdp to p->dp
	#define _trans(type)\
		*((type*)p->dp) = *((type*)p->mdp);
			switch(p->type){
			case PIN_T_BOOL:
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
			if(p->type == PIN_T_BOOL && p->flags & PIN_F_INVERTED){
				p->dp->u.d.ui8 = !p->dp->u.d.ui8;
			}
		}

	}
}

static void _sample_out(IBBlk * bb)
{
	int i;
	IBlk * b;
	struct blk_pin_t *p;

	b = __ucast__(IBBlk, IBlk, bb);
	
	for(i=b->h.n_pins, p=bb->pins; i; i--, p++){
		if(p->_class != PIN_CLASS_DO){
			continue;
		}
		if(!p->mdp){
			assert(0);
			continue;
		}
		// transfer some bytes from p->dp to p->mdp
#define _trans(type)\
	*((type*)p->mdp) = *((type*)p->dp);
		switch(p->type){
		case PIN_T_BOOL:
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
		if(p->type == PIN_T_BOOL && p->flags & PIN_F_INVERTED){
			p->mdp->u.d.ui8 = !p->mdp->u.d.ui8;
		}
	}
}

#ifdef __VXWORKS__
#include "fppLib.h"
#else
#define FP_CONTEXT int
#define fppSave(c) *c=0
#define fppRestore(c) *c=1
#endif

void ki_exec_algorithm(IBBlk * b, struct blk_func_t * f)
{
	FP_CONTEXT c;
	/* value/result calling convention */
	_sample_in(b);
	fppSave(&c);
	((F8_FUNCTION)(f->address))();
	fppRestore(&c);
	_sample_out(b);
}

void ki_exec_fbd()
{
	PRTK_LIST_ENTRY li;
	
	/* do autoscan blocks */
	li = g_kernel.m_1131_blocks.Flink;
	while(li != &g_kernel.m_1131_blocks && KERN_S_RUNNING == g_kernel.status.state){
		g_kernel.current_bblk = RTK_CONTAINING_RECORD(li, IBBlk, bh.x_link);
		g_kernel.current_blk = __ucast__(IBBlk, IBlk, g_kernel.current_bblk);
		ki_exec_algorithm(g_kernel.current_bblk, &g_kernel.current_bblk->funcs[0]);
		li = li->Flink;
	}	
}

/*
	see docs/binary.txt
*/
static __bool bblk_save_volatile(IBlk * b, struct kernel_t * kernel)
{
	IBBlk * bb;
	f8_u8 * p;
	f8_u16 volatile_size;

	bb = __dcast__(IBlk, IBBlk, b);

	/* 
	put data section into volatile memory 

	stuff that should go into volatile memory includes:

	...
	*) data section(initialized data/read only data)
	<------------ .bss section begins here
	*) dynamic pins
	!! *) service proc table pointer !! skipped
	*) if in memory, other parts of the bss section
	...
	
	*/
	p = peerPointer;
	volatile_size = bb->bh.n_data + bb->bh.n_bss 
		+ b->h.n_pins * sizeof(struct dyn_pin_t);
	if(p + volatile_size + sizeof(f8_uuid) + sizeof(f8_u16) 
		> 
		peerGuardian
	){
		return __false;
	}
	*((f8_uuid*)p) = b->uuid;
	p += sizeof(f8_uuid);
	*((f8_u16*)p) = volatile_size;
	p += sizeof(f8_u16);
	/* write data section */
	memcpy(p, bb->data_section, bb->bh.n_data);
	p += bb->bh.n_data;
	/* write dynamic pin array */
	memcpy(p, bb->dpins, b->h.n_pins * sizeof(struct dyn_pin_t));
	p += b->h.n_pins * sizeof(struct dyn_pin_t);
	/* write bss section */
	memcpy(p, bb->bss_section, bb->bh.n_bss);
	p += bb->bh.n_bss;

	peerPointer = p;
	
	return __true;
}

static __bool _traverse_save_volatile(ITreeNode *nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	struct kernel_t * kernel = (struct kernel_t*)context;

	b = __dcast__(ITreeNode, IBlk, nd);

	if(b->h.magic != BBLK_MAGIC){
		return __true;
	}
	
	return bblk_save_volatile(b, kernel);
}

__bool fbd_marshel_i_memory(struct kernel_t * kernel)
{
	f8_u8 * marker;
	marker = peerPointer;
	if(!_traverse(kernel->sections, _traverse_save_volatile, kernel)){
		return __false;
	}
	peerHdr.i_mem_size = peerPointer - marker;
	return __true;
}

/*
	load volatile data for a basic block, on entrance the 
	peerPointer is placed right at the beginning 
	of this block's volatile data; returns the bytes
	consumed by this block. or a negative value if failed.

	memory pointed by marker is of the same format
	produced by bblk_save_volatile, that is, a 8-byte
	uuid followed by 2-byte length fields, then data section, 
	dynamic pins and bss section.
*/
static int bblk_load_volatile(
	IBlk * b, 
	struct kernel_t * kernel, 
	f8_u8 * marker
	)
{
	f8_u16 volatile_size;
	IBBlk * bb;
	f8_u8 * p;
	bb = __dcast__(IBlk, IBBlk, b);
	volatile_size = bb->bh.n_data + bb->bh.n_bss + 
		b->h.n_pins * sizeof(struct dyn_pin_t);
	p = marker;
	p += sizeof(f8_uuid);
	if(*((f8_u16*)p) != volatile_size){
		return -1;
	}
	p += sizeof(f8_u16);
	if(p + volatile_size > peerGuardian){
		return -2;
	}
	/* load data section */
	memcpy(bb->data_section, p, bb->bh.n_data);
	p += bb->bh.n_data;
	/* load dynamic pin array */
	memcpy(bb->dpins, p, b->h.n_pins * sizeof(struct dyn_pin_t));
	p += b->h.n_pins * sizeof(struct dyn_pin_t);
	/* load bss section */
	memcpy(bb->bss_section, p, bb->bh.n_bss);
	p += bb->bh.n_bss;
	return p - marker;
}

/*
	load block level volatile data from volatile memory.
*/
__bool fbd_unmarshel_i_memory(struct kernel_t * kernel)
{
	f8_u8 * p;
	f8_uuid * id;
	IBlk * blk;
	int size;
	
	p = peerPointer;
	
	while(p < peerGuardian){
		/* unmarshal b-block */
		id = (f8_uuid*)p;
		blk = (IBlk*)ke_get_hash(kernel->blk_hash, id);
		if(!blk){
			return __false;
		}
		size = bblk_load_volatile(blk, kernel, p);
		if(size < 0){
			return __false;
		}
		p += size;
	}

	return __true;
}

void ki_init_bblk(struct kernel_t * kernel, IBBlk * bb)
{
	IBlk * b;
	struct blk_func_t * fn;
	int i;
	b = __ucast__(IBBlk, IBlk, bb);
	/* reset init data */
	memcpy(bb->data_section, bb->init_data, bb->bh.n_data);
	memset(bb->bss_section, 0, bb->bh.n_bss);
	for(i=0, fn = bb->funcs; i<b->h.u1.n_funcs; i++, fn++){
		if(fn->flags & FUNC_F_INIT){
			ki_exec_algorithm(bb, fn);
		}
	}
}

void ki_finalize_bblk(struct kernel_t * kernel, IBBlk * bb)
{
	struct blk_func_t * fn;
	int i;
	IBlk *b;
	b = __ucast__(IBBlk, IBlk, bb);
	for(i=0, fn = bb->funcs; i<b->h.u1.n_funcs; i++, fn++){
		if(fn->flags & FUNC_F_FINAL){
			ki_exec_algorithm(bb, fn);
		}
	}
#if defined(CLEAR_MEM_ON_STOP)
	/* reset init data */
	memcpy(bb->data_section, bb->init_data, bb->bh.n_data);
	memset(bb->bss_section, 0, bb->bh.n_bss);
	memset(bb->dpins,0,b->h.n_pins*sizeof(struct dyn_pin_t));
#endif
}

static __bool _traverse_init(ITreeNode *nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	IBBlk * bb;
	struct kernel_t * kernel = (struct kernel_t *)context;

	b = __dcast__(ITreeNode, IBlk, nd);
	kernel->current_blk = b;
	if(b->h.magic != BBLK_MAGIC){
		return __true;
	}
	bb = __dcast__(IBlk, IBBlk, b);
	// _trace_blk(b, "Initing");
	kernel->current_bblk = bb;
	kernel->current_pin = 0;
	ki_init_bblk(kernel, bb);
	return __true;
}

void fbd_start(struct kernel_t * kernel)
{
	_traverse(kernel->sections, _traverse_init, kernel);
}

static __bool _traverse_final(ITreeNode *nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	IBBlk * bb;
	struct kernel_t * kernel = (struct kernel_t *)context;
	
	b = __dcast__(ITreeNode, IBlk, nd);
	kernel->current_blk = b;
	if(b->h.magic != BBLK_MAGIC){
		return __true;
	}
	bb = __dcast__(IBlk, IBBlk, b);

	// _trace_blk(b, "Finalizing");
	kernel->current_bblk = bb;
	ki_finalize_bblk(kernel, bb);
	return __true;
}

void fbd_stop(struct kernel_t * kernel)
{
	_traverse(kernel->sections, _traverse_final, kernel);
}

static void _print_dpin(int type, struct dyn_pin_t *dp)
{
	switch(type){
	case PIN_T_CHAR:
		printf("%c, char", dp->u.d.i8);
		break;
	case PIN_T_SHORT:
		printf("%d, short", dp->u.d.i16);
		break;
	case PIN_T_INTEGER:
		printf("%d, integer", dp->u.d.i32);
		break;
	case PIN_T_BYTE:
		printf("%d, byte", dp->u.d.ui8);
		break;
	case PIN_T_WORD:
		printf("%d, word", dp->u.d.ui16);
		break;
	case PIN_T_DWORD:
		printf("%d, dword", dp->u.d.ui32);
		break;
	case PIN_T_BOOL:
		printf("%s, bool", dp->u.d.ui8? "true" : "false");
		break;
	case PIN_T_TIMER:
		printf(
			"%d, timer", 
			dp->u.e.interval
			);
		break;
	case PIN_T_FLOAT:
		printf("%f, float", dp->u.d.flt);
		break;
	case PIN_T_DOUBLE:
	case PIN_T_DATE:
		printf("%f, double", dp->u.d.dbl);
		break;
	default:
		printf("N.A.");
		break;
	}
}

#if defined(__VXWORKS__)
void fbd_show(const char * buf)
{
	__u32 is[4];
	f8_uuid id;
	f8_u16 * _is = (f8_u16*)&id;
	IBlk * blk;
	IBBlk * bblk;
	int i;
	struct blk_pin_t * p;
	struct dyn_pin_t * dp;

	if(!buf)
		return;
	/* accept uuid enclosed by braces */
	if(*buf == '{'){
		buf++;
	}
	sscanf(buf, "%x%*c%x%*c%x%*c%x", is, is+1, is+2, is+3);
	_is[0] = is[0];
	_is[1] = is[1];
	_is[2] = is[2];
	_is[3] = is[3];
	blk=ke_get_blk(&g_kernel,&id);
	if(blk && blk->h.magic == BBLK_MAGIC){
		bblk=__dcast__(IBlk,IBBlk,blk);
		printf("Dump of blk %s\n", buf);
		printf(".data @ 0x%08x \n ", bblk->data_section);
		io_dump_memory(bblk->data_section,0,bblk->bh.n_data);
		printf(".bss @ 0x%08x \n ", bblk->bss_section);
		io_dump_memory(bblk->bss_section,0,bblk->bh.n_bss);
#if 0
		printf(".data @ 0x%08x \n ", bblk->data_section);
		io_dump_memory(bblk->data_section,0,bblk->bh.n_data);
#endif
		printf("pin dump:\n");
		for(i=0,p=bblk->pins,dp=bblk->dpins;i<blk->h.n_pins;i++,p++,dp++){
			printf("P%02d=",i);
			_print_dpin(p->type, p->dp);
			printf(",");
			_print_dpin(p->type, dp);
			putchar('\n');
		}
	}
}
#endif

