/********************************************************************
	created:	2003/03/14  14:38
	filename: 	E:\cvs\pmc2\ctrller\fbin\bblk.cpp
	author:		Jackie, jhunter@tsinghua.org.cn
	
	purpose:	f8 binary utility
	read/write of basic block stream.
*********************************************************************/
#include "precomp.h"

__define_vtbl__(ICBlk)
__define_vtbl_end__(ICBlk)

#define _first_link(cblk)\
	(RtkIsListEmpty(&cblk->links)? 0 : \
		(SAFE_CONTAINING_RECORD(cblk->links.Flink, struct cblk_link_t, sibling))\
	)
#define _next_link(link)\
	(link->sibling.Flink==&link->blk->links? 0 : \
		(SAFE_CONTAINING_RECORD(link->sibling.Flink, struct cblk_link_t, sibling))\
	)
#define _first_pin(cblk)\
	(RtkIsListEmpty(&cblk->pins)? 0 : \
		(SAFE_CONTAINING_RECORD(cblk->pins.Flink, struct cblk_pin_t, sibling))\
	)
#define _next_pin(cblk, pin)\
	(pin->sibling.Flink==&cblk->pins? 0 : \
		(SAFE_CONTAINING_RECORD(pin->sibling.Flink, struct cblk_pin_t, sibling))\
	)
#define locate_pin(blk, pd)\
do{\
	IBlk * sb;\
	sb = IBlk_blk_by_id(blk, (__u16)(pd).id.bid);\
	if(sb){\
		(pd).pin = __vcall__(sb, pin_by_id, (sb, (pd).id.pid));\
	}else{\
		(pd).pin = 0;\
	}\
}while(0)

/*
	ICBlk::load(IF8Stream * s)
	this will construct an in-memory composite block object.
	the most exciting part of F8 controller program.
*/
static __bool ICBlk_load(IBlk * blk, IF8Stream * s)
{
	ICBlk * cblk;
	struct blk_hdr_t * h, _h;
	int i;
	IBlk * sb;
	union blk_pindef_t pd, ps, pt;
	f8_uuid uuid;

	cblk = __dcast__(IBlk, ICBlk, blk);
	h = &_h;

	if(__vcall__(s, get, (s, h, sizeof(struct blk_hdr_t))) 
		!= 
		sizeof(struct blk_hdr_t) 
	){
		return __false;
	}

	blk->h = *h;
	blk->h.u1.n_subblocks = 0;
	blk->h.u2.n_links = 0;
	blk->h.n_pins = 0;

	if(!__vcall__(s, get, (s, &blk->uuid, sizeof(blk->uuid)))){
		goto __failed;
	}

	/*
		load sub-blocks
	*/
	for(i=0; i<h->u1.n_subblocks; i++){
		sb = IBlk_from_stream(s);
		if(!sb){
			goto __failed;
		}
		if(!ICBlk_add_blk(cblk, sb)){
			delete_blk(sb);
			return __false;
		}
	}

	/*
		load links
	*/
	for(i=0; i<h->u2.n_links; i++){
		if(!__vcall__(s, get, (s, &uuid, sizeof(uuid)))){
			goto __failed;
		}

		if(!__vcall__(s, get, (s, &ps, sizeof(union blk_pindef_t)))){
			goto __failed;
		}
		
		if(!__vcall__(s, get, (s, &pt, sizeof(union blk_pindef_t)))){
			goto __failed;
		}
		
		locate_pin(blk, ps);
		locate_pin(blk, pt);

		if(!ps.pin || !pt.pin){
			goto __failed;
		}

		if(!ICBlk_link(&uuid, ps.pin, pt.pin)){
			goto __failed;
		}
	}

	/*
		load exports
	*/
	for(i=0; i<h->n_pins; i++){
		if(!__vcall__(s, get, (s, &uuid, sizeof(uuid)))){
			goto __failed;
		} 

		if(!__vcall__(s, get, (s, &pd, sizeof(union blk_pindef_t)))){
			goto __failed;
		}

		locate_pin(blk, pd);
		if(!pd.pin){
			return __false;
		}

		if(!ICBlk_add_pin(&uuid, pd.pin)){
			return __false;
		}
	}

	return __true;

__failed:
	return __false;
}

static __bool ICBlk_save(IBlk * _this, IF8Stream * s)
{
	IBlk * sb;
	struct cblk_link_t * l;
	struct cblk_pin_t  * p;
	union blk_pindef_t pd;
	ICBlk * cblk;
	
	cblk = __dcast__(IBlk, ICBlk, _this);

	// write header
	__vcall__(s, put, (s, &_this->h, sizeof(_this->h)));

	// write guid
	__vcall__(s, put, (s, &_this->uuid, sizeof(f8_uuid)));

	// write sub-blocks
	sb = first_subblk(_this);
	while(sb){
		if(!__vcall__(sb, save, (sb, s))){
			return __false;
		}
		sb = next_blk(sb);
	}

	// write link-defs
	l = _first_link(cblk);
	while(l){
		__vcall__(s, put, (s, &l->uuid, sizeof(l->uuid)));
		pd.id.bid = IBlk_id_by_blk(l->s.pin->blk);
		pd.id.pid = __vcall__(l->s.pin->blk, id_by_pin, (l->s.pin));
		__vcall__(s, put, (s, &pd, sizeof(pd)));
		pd.id.bid = IBlk_id_by_blk(l->t.pin->blk);
		pd.id.pid = __vcall__(l->t.pin->blk, id_by_pin, (l->t.pin));
		__vcall__(s, put, (s, &pd, sizeof(pd)));
		l = _next_link(l);
	}

	// write pins
	p = _first_pin(cblk);
	while(p){
		__vcall__(s, put, (s, &p->uuid, sizeof(p->uuid)));
		pd.id.bid = IBlk_id_by_blk(p->p.u1.x.id.llink->blk);
		pd.id.pid = __vcall__(p->p.u1.x.id.llink->blk, id_by_pin, (p->p.u1.x.id.llink));
		__vcall__(s, put, (s, &pd, sizeof(pd)));
		p = _next_pin(cblk, p);
	}

	return __true;
}

ICBlk * ICBlk_from_stream	(IF8Stream * s)
{
	ICBlk * blk;
	IBlk  * b;

	blk = mm_alloc(g_hBlkHeap, sizeof(ICBlk), 0);
	if(!blk){
		return 0;
	}
	
	__create__(ICBlk, blk);
	
	b = __ucast__(ICBlk, IBlk, blk);

	if(!__vcall__(b, load, (b, s))){
		delete_blk(b);
		return 0;
	}
	return blk;
}

/*
	dynamic configuration support of ICBlk		
 */
FBD_API __bool ICBlk_add_blk(ICBlk * _this, IBlk * b)
{
	ITreeNode *t1;
	ITreeNode *t2;
	IBlk * me;

	me = __ucast__(ICBlk, IBlk, _this);

	if( IBlk_subblk(me, &b->uuid)){
		// Already exists
		return __false;
	}

	t1 = __ucast2__(ICBlk, IBlk, ITreeNode, _this);
	t2 = __ucast__(IBlk, ITreeNode, b);

	if( !ITreeNode_mount(t1, t2, __true)){
		return __false;
	}

	me->h.u1.n_subblocks++;
	
	return __true;
}

/*
	Remove a subblock from composite block
*/
FBD_API __bool ICBlk_del_blk(IBlk * b)
{
	umount_blk(b);
	return __true;
}

FBD_API struct cblk_pin_t * ICBlk_add_pin(f8_uuid * id, struct blk_pin_t * p)
{
	ICBlk * a;
	IBlk * b;
	struct cblk_pin_t * t;


	b = parent_blk(p->blk);
	if(!b){
		return 0;
	}
	assert(b->h.magic== CBLK_MAGIC);
	a = __dcast__(IBlk, ICBlk, b);
	
	// re-export not allowed
	if(p->ulink){
		return 0;
	}
	
	// ok, go on
	t = (struct cblk_pin_t *)mm_alloc(
		g_hBlkHeap, 
		sizeof(struct cblk_pin_t), 
		0
		);
	if( !t ){
		return 0;
	}

	memset(t, 0, sizeof(struct cblk_pin_t));
	
	t->uuid = *id;

	// fake a pin structure	
	t->p._class = PIN_CLASS_EXP;
	t->p.blk = b;
	t->p.type = 0;
	t->p.flags = 0;

	t->p.ulink = 0;
	t->p.u1.x.id.llink = p;
	t->p.u1.x.real_pin = IBlk_real_pin(p);
	t->p.dp = p->dp;
	p->ulink = &t->p;
	RtkInsertTailList(&a->pins, &t->sibling);
	b->h.n_pins++;

	return t;
}

FBD_API __bool ICBlk_del_pin(struct blk_pin_t * p)
{
	struct cblk_pin_t *cp;
	struct cblk_link_t *lk;
	
	assert(p->blk->h.magic == CBLK_MAGIC);

	// remove all connections on this pin
	lk = ICBlk_first_connection(p);
	while(lk){
		ICBlk_unlink(lk);
		lk = ICBlk_first_connection(p);
	}	

	/* propagate this delete operation upwards */
	if(p->ulink){
		ICBlk_del_pin(p->ulink);
		/*
			p->ulink is not accessible here because it's already
			been freed
		*/
		p->ulink = NULL;
	}

	/* propagate delete operation downloads */
	p->u1.x.id.llink->ulink = 0;
	
	p->blk->h.n_pins--;

	/* i'm dying, mother! */
	cp = SAFE_CONTAINING_RECORD(p, struct cblk_pin_t, p);
	RtkRemoveEntryList(&cp->sibling);

#ifdef F8_CONFIGURATOR
	unload_resource(&cp->uuid);
#endif

	mm_free(g_hBlkHeap, cp);

	return __true;
}

/*
	return a pin's first logical peer, i.e., pin within the same level in
	the block treelist.
*/
FBD_API struct cblk_link_t * ICBlk_first_connection(struct blk_pin_t *p)
{
	IBlk *b;
	ICBlk *cb;
	int i;
	struct cblk_link_t * lk;

	b = parent_blk(p->blk);
	if(!b){
		// this is the top-level pin
		return 0;
	}
	assert(b);
	assert(b->h.magic == CBLK_MAGIC);
	cb = __dcast__(IBlk, ICBlk, b);

	// search in the link array
	for(i=0, lk=_first_link(cb); i<b->h.u2.n_links; i++, lk=_next_link(lk)){
		if(lk->s.pin == p){
			return lk;
		}
		if(lk->t.pin == p){
			return lk;
		}
	}

	return 0;
}

FBD_API struct cblk_link_t * ICBlk_next_connection(struct blk_pin_t *p, struct cblk_link_t * lk)
{
	IBlk *b;
	ICBlk *cb;
	struct blk_pin_t *rp;

	b = parent_blk(p->blk);
	assert(b);
	assert(b->h.magic == CBLK_MAGIC);
	cb = __dcast__(IBlk, ICBlk, b);

	rp = IBlk_real_pin(p);
	if(rp->_class != PIN_CLASS_DO && rp->_class != PIN_CLASS_EO){
		return 0;
	}
	// search in the link array
	lk = _next_link(lk);
	while(lk){
		if(lk->s.pin == p){
			return lk;
		}
		lk = _next_link(lk);
	}

	return 0;
}

/*
	ICBlk::link(p1, p2)
	allocate data structures to hold this link information, and link the 
	corresponding pin structure
*/
FBD_API struct cblk_link_t * ICBlk_link(f8_uuid * id, struct blk_pin_t * s, struct blk_pin_t * t)
{
	ICBlk *a;
	IBlk  *b;
	struct cblk_link_t * lk;

	if(!IBlk_linkable(s, t)){
		return 0;
	}

	b = parent_blk(s->blk);
	a = __dcast__(IBlk, ICBlk, b);

	if( !IBlk_connect(s, t)){
		return 0;
	}

	if( !IBlk_is_output(s)){
		struct blk_pin_t *tmp;
		tmp = s;
		s = t;
		t = tmp;
	}
	// create a new the link object
	lk = (struct cblk_link_t *)mm_alloc(
		g_hBlkHeap, 
		sizeof(struct cblk_link_t), 
		0
		);
	if(!lk){
		IBlk_disconnect(t);
		return 0;
	}
	memset(lk, 0, sizeof(struct cblk_link_t));
	lk->blk = a;
	lk->uuid = *id;
	lk->s.pin = s;
	lk->t.pin = t;
	RtkInsertTailList(&a->links, &lk->sibling);
	b->h.u2.n_links++;
	
	return lk;
}

FBD_API struct cblk_link_t * ICBlk_get_link(ICBlk * blk, f8_uuid * id)
{
	struct cblk_link_t * lk;
	PRTK_LIST_ENTRY li;

	li = blk->links.Flink;
	while(li != &blk->links){
		lk = SAFE_CONTAINING_RECORD(li, struct cblk_link_t, sibling);
		if(lk->uuid == *id){
			return lk;
		}
		li = li->Flink;
	}
	return 0;
}

FBD_API __u16 ICBlk_id_by_link(struct cblk_link_t * lk)
{
	ICBlk *cb;
	PRTK_LIST_ENTRY li;
	__u16	id;

	cb = lk->blk;
	li = cb->links.Flink;
	id = 0;
	while(li != &lk->sibling){
		li = li->Flink;
		id++;
		assert(li != &cb->links);
	}
	return id;
}

FBD_API struct cblk_link_t * ICBlk_link_by_id(ICBlk *cblk, __u16 id)
{
	PRTK_LIST_ENTRY li;
	
	if(id > __ucast__(ICBlk, IBlk, cblk)->h.u2.n_links){
		return 0;
	}

	li = cblk->links.Flink;
	while(id--){
		li = li->Flink;
		assert(li!=&cblk->links);
	}

	return SAFE_CONTAINING_RECORD(li, struct cblk_link_t, sibling);
}

/*
	remove a connection
*/
FBD_API __bool ICBlk_unlink(struct cblk_link_t * lk)
{
	IBlk * me;

	me = parent_blk(lk->t.pin->blk);
	assert(me->h.magic == CBLK_MAGIC);
	assert(me == __ucast__(ICBlk, IBlk, lk->blk));

	IBlk_disconnect(lk->t.pin);
	RtkRemoveEntryList(&lk->sibling);
	me->h.u2.n_links--;
	// assert(me->h.u2.n_links >= 0);

#ifdef F8_CONFIGURATOR
	unload_resource(&lk->uuid);
#endif

	mm_free(g_hBlkHeap, lk);
	
	return __true;
}

static struct blk_pin_t * ICBlk_pin_by_id(IBlk * blk, __u16 id)
{
	ICBlk * cb;
	struct cblk_pin_t * p;

	if(id >= blk->h.n_pins){
		return 0;
	}
	cb = __dcast__(IBlk, ICBlk, blk);
	p = _first_pin(cb);
	while(p && id){
		p = _next_pin(cb, p);
		id--;
	}
	return &p->p;
}

static __u16 ICBlk_id_by_pin(struct blk_pin_t * pin)
{
	int i;
	ICBlk * cblk;
	PRTK_LIST_ENTRY le;
	struct cblk_pin_t * cp;

	cblk = __dcast__(IBlk, ICBlk, pin->blk);
	le = cblk->pins.Flink;
	i = 0;
	cp = SAFE_CONTAINING_RECORD(pin, struct cblk_pin_t, p);
	while(le != &cblk->links){
		if(le == &cp->sibling){
			return i;
		}
		le = le->Flink;
		i++;
	}

	return -1;
}

static struct blk_pin_t * ICBlk_first_pin(IBlk * _this)
{ 
	ICBlk * me;
	struct cblk_pin_t * pp;
	
	me = __dcast__(IBlk, ICBlk, _this);
	assert(_this->h.magic == CBLK_MAGIC);
	
	pp = _first_pin(me);
	if(pp){
		return &pp->p;
	}
	return 0;
}

static struct blk_pin_t * ICBlk_next_pin(struct blk_pin_t * p)
{
	ICBlk * me;
	struct cblk_pin_t * pp;

	assert(p->blk->h.magic == CBLK_MAGIC);
	me = __dcast__(IBlk, ICBlk, p->blk);
	pp = SAFE_CONTAINING_RECORD(p, struct cblk_pin_t, p);
	pp = _next_pin(me, pp);
	if(pp){
		return &pp->p;
	}
	return 0;
}

FBD_API struct cblk_link_t * ICBlk_first_link(ICBlk * _this)
{
	return _first_link(_this);
}

FBD_API struct cblk_link_t * ICBlk_next_link(struct cblk_link_t * l)
{
	return _next_link(l);
}

__global_constructor__(ICBlk)
{
	__v_copy__(ICBlk, IBlk);
	__override__(ICBlk, IBlk, load);
	__override__(ICBlk, IBlk, save);
	__override__(ICBlk, IBlk, first_pin);
	__override__(ICBlk, IBlk, next_pin);
	__override__(ICBlk, IBlk, pin_by_id);
	__override__(ICBlk, IBlk, id_by_pin);
}

__constructor__(ICBlk)
{
	if(__this){
		__touch__();
		__create_base__(ICBlk, IBlk, __this);
		__ucast__(ICBlk, IBlk, __this)->h.magic = CBLK_MAGIC;
		RtkInitializeListHead(&__this->links);
		RtkInitializeListHead(&__this->pins);
	}
	return __this;
}

__destructor__(ICBlk)
{
	IBlk *blk, *sb, *next;
	if(__this){
		blk = __ucast__(ICBlk, IBlk, __this);
		assert(blk->h.magic == CBLK_MAGIC);
		sb = first_subblk(blk);
		while(sb){
			next = next_blk(sb);
			umount_blk(sb);
			sb = next;
			if(next){
				next = next_blk(sb);
			}
		}
		__destroy_base__(ICBlk, IBlk, __this);
	}
	return __this;
}

