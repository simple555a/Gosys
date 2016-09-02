/********************************************************************
	created:	2003/03/14  14:38
	filename: 	E:\cvs\pmc2\ctrller\fbin\bblk.cpp
	author:		Jackie, jhunter@tsinghua.org.cn
	
	purpose:	f8 binary utility
	read/write of basic block stream.
*********************************************************************/
#include "precomp.h"

__define_vtbl__(IBBlk)
__define_vtbl_end__(IBBlk)

/*
	IBBlk::relocate()
	when the binary image loaded into memory, this proc will do the
	relocation job, once relocated, the block is ready to be scheduled
	and configurated.
*/
static __bool IBBlk_relocate(IBBlk * __this)
{
	int i;
	struct blk_pin_t * p;
	struct blk_func_t * func;
	struct blk_hdr_t * h;
	char *buf;
	IBlk *me;
	int	n_eventin = 0;

	me = __ucast__(IBBlk, IBlk, __this);
	
	h = &me->h;
	buf = (char *)h;

	/* backup initialized data */
	memcpy(__this->init_data, __this->data_section, __this->bh.n_data);

	/*
		patch pin array
	*/
	for(i=0, p=__this->pins; i<h->n_pins; i++, p++){
		p->blk = me;
		p->ulink = 0;
		p->dp = __this->dpins + i;
		p->u1.x.real_pin = p;
		p->mdp = 0;
		switch(p->_class){
		case PIN_CLASS_EI:
			n_eventin++;
			break;
		case PIN_CLASS_EO:			
			break;
		case PIN_CLASS_DO:
			p->dp->u.d.i64 = 0;
			break;
		}
		if(p->_class == PIN_CLASS_DO){
			// silently fix the invalid binding types
			if(p->binding == PIN_B_CONST)
			{
				p->binding = PIN_B_MEMORY;
				p->u2.mem_addr.section = -1;
			}
		}else if(p->_class == PIN_CLASS_DI){
			//
		}
		RtkInitializeTripleListHead(&p->u1.link);
	}

	for(i=0, func=__this->funcs; i<h->u1.n_funcs; i++, func++){
		func->address = (f8_ptr)(__this->code_section + func->address);
	}

	// basic block is forced read-only
	me->h.flags |= BLK_F_READONLY;
	if(!n_eventin){
		me->h.flags |= BLK_F_AUTOSCAN;
	}

	return __true;
}

static void IBBlk_nullify(IBBlk * bblk)
{
	IBlk * blk;
	blk = __ucast__(IBBlk, IBlk, bblk);
	bblk->code_section = 0;
	bblk->data_section = 0;
	bblk->bss_section = 0;
	bblk->init_data = 0;
	bblk->pins = 0;
	bblk->dpins = 0;
	bblk->funcs = 0;
}

static void IBBlk_release_memory(IBBlk * bblk)
{
	if(bblk->pins){
		mm_free(g_hBlkHeap, bblk->pins);
	}

	IBBlk_nullify(bblk);
}

/*
	see docs/binary.txt
*/
static __bool IBBlk_load(IBlk * blk, IF8Stream * s)
{
	IBBlk * bblk;
	struct blk_hdr_t *h;
	int bytes_left;
	__uint len;
	BLK_SERVICE_PROC ** bsp;
	__uint heap_size;

	bblk = __dcast__(IBlk, IBBlk, blk);
	h = &blk->h;

	IBBlk_nullify(bblk);

	bytes_left = __vcall__(s, left, (s));
	if( bytes_left <= sizeof(struct blk_hdr_t) ){
		// pointer goes beyond stream
		goto __failed;
	}

	if( __vcall__(s, get, (s, h, sizeof(struct blk_hdr_t))) 
		!=
		sizeof(struct blk_hdr_t)
		){
		goto __failed;;
	}

	if( h->magic != BBLK_MAGIC){
		goto __failed;
	}

	if(!__vcall__(s, get, (s, &bblk->bh, sizeof(bblk->bh)))){
		goto __failed;
	}

	if(!__vcall__(s, get, (s, &blk->uuid, sizeof(blk->uuid)))){
		goto __failed;
	}

	/*
		the heap contains all the dynamic memory consumed
		by this BBlk, note the n_data is counted twice in
		heap_size, which means there're two data_section in the
		heap, one of them is used as the backup of data_section, 
		the other is used at runtime. thus when the control 
		program is restarted, we can use the backup data_section 
		to initialize the runtime data_section.
	*/
	len = sizeof(struct blk_pin_t) * h->n_pins
		+ sizeof(struct blk_func_t) * h->u1.n_funcs
		+ h->u2.n_codes
		+ bblk->bh.n_data
		+ sizeof(struct dyn_pin_t) * h->n_pins
		;
	heap_size = len
		+ sizeof(BLK_SERVICE_PROC*)
		+ bblk->bh.n_bss
		+ bblk->bh.n_data
		;

	/* 
		load execution body
	*/
	bblk->pins = (struct blk_pin_t*)mm_alloc(g_hBlkHeap, heap_size, 0);
	if(!bblk->pins){
		goto __failed;
	}

	if(__vcall__(s, get, (s, bblk->pins, len)) != len){
		goto __failed;
	}

	bblk->funcs = (struct blk_func_t*)(&bblk->pins[h->n_pins]);
	bblk->code_section = (f8_byte*)&bblk->funcs[h->u1.n_funcs];
	bblk->data_section = (f8_byte*)&bblk->code_section[h->u2.n_codes];
	bblk->dpins = (struct dyn_pin_t*)(
		((f8_byte*)&bblk->data_section[bblk->bh.n_data])
		);
	bsp = (BLK_SERVICE_PROC**)&bblk->dpins[h->n_pins];
	bblk->bss_section = (f8_byte*)&bsp[1];
	bblk->init_data = &bblk->bss_section[bblk->bh.n_bss];
	assert(bblk->init_data + bblk->bh.n_data == 
		(f8_byte*)bblk->pins + heap_size
		);

	if(!IBBlk_relocate(bblk)){
		goto __failed;
	}

	return __true;

__failed:
	IBBlk_release_memory(bblk);
	IBBlk_nullify(bblk);

	return __false;
}

/*
	IBBlk::save()
	this proc do the reverse job of load()
*/
static __bool IBBlk_save(IBlk * _this, IF8Stream * s)
{
	struct blk_func_t * func;
	struct blk_hdr_t * h;
	char *buf;
	IBBlk * __this;
	int i;
	
	__this = __dcast__(IBlk, IBBlk, _this);
	h = &_this->h;
	buf = (char *)h;

	// write header
	__vcall__(s, put, (s, &_this->h, sizeof(struct blk_hdr_t)) );

	// write extended header
	__vcall__(s, put, (s, &__this->bh, sizeof(struct bblk_hdr_t)));

	// write guid
	__vcall__(s, put, (s, &_this->uuid, sizeof(f8_uuid)));

	// write static pins
	__vcall__(
		s, 
		put, 
		(s, __this->pins, sizeof(struct blk_pin_t)*_this->h.n_pins)
		);

	// write function
	for(i=0, func=__this->funcs; i<h->u1.n_funcs; i++, func++){
		struct blk_func_t t;
		t = *func;
		t.address = func->address - (f8_ptr)__this->code_section;
		__vcall__(s, put, (s, &t, sizeof(t)));
	}

	// write code
	__vcall__(s, put, (s, __this->code_section, h->u2.n_codes));

	// write data
	__vcall__(s, put, (s, __this->data_section, __this->bh.n_data));

	// write dynamic pins
	__vcall__(
		s, 
		put, 
		(s, __this->dpins, sizeof(struct dyn_pin_t)*_this->h.n_pins)
		);

	return __true;
}
 
static struct blk_pin_t * IBBlk_pin_by_id(IBlk * _this, __u16 id)
{
	IBBlk * me;
	
	if(id >= _this->h.n_pins){
		return 0;
	}
	me = __dcast__(IBlk, IBBlk, _this);
	return me->pins + id;
}

static __u16 IBBlk_id_by_pin(struct blk_pin_t * pin)
{
	IBBlk * bblk = __dcast__(IBlk, IBBlk, pin->blk);
	assert(pin->blk->h.magic == BBLK_MAGIC);
	return pin - bblk->pins;
}

static struct blk_pin_t * IBBlk_first_pin(IBlk * _this)
{
	IBBlk * me;
	
	if(!_this->h.n_pins){
		return 0;
	}
	me = __dcast__(IBlk, IBBlk, _this);
	return me->pins;
}

static struct blk_pin_t * IBBlk_next_pin(struct blk_pin_t * p)
{
	IBBlk * me;
	me = __dcast__(IBlk, IBBlk, p->blk);
	
	if( p - me->pins >= p->blk->h.n_pins - 1){
		return 0;
	}
	return p+1;
}

IBBlk * IBBlk_from_stream(IF8Stream * s)
{
	IBBlk * blk;
	IBlk  * b;
	
	blk = mm_alloc(g_hBlkHeap, sizeof(IBBlk), 0);	

	if(!blk){
		return 0;
	}
	__create__(IBBlk, blk);
	
	b = __ucast__(IBBlk, IBlk, blk);
	if( !__vcall__(b, load, (b, s)) ){
		delete_blk(b);
		return 0;
	}

	return blk;
}

__global_constructor__(IBBlk)
{
	__v_copy__(IBBlk, IBlk);
	__override__(IBBlk, IBlk, load);
	__override__(IBBlk, IBlk, save);
	__override__(IBBlk, IBlk, pin_by_id);
	__override__(IBBlk, IBlk, id_by_pin);
	__override__(IBBlk, IBlk, first_pin);
	__override__(IBBlk, IBlk, next_pin);
}

__constructor__(IBBlk)
{
	if(__this){
		__touch__();
		__create_base__(IBBlk, IBlk, __this);
		__ucast__(IBBlk, IBlk, __this)->h.magic = BBLK_MAGIC;
		IBBlk_nullify(__this);
	}
	return __this;
}

__destructor__(IBBlk)
{
	if(__this){
		assert(__ucast__(IBBlk, IBlk, __this)->h.magic == BBLK_MAGIC);
		IBBlk_release_memory(__this);
		__destroy_base__(IBBlk, IBlk, __this);
	}
	return __this;
}

