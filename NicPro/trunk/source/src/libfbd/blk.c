#include "precomp.h"

__define_vtbl__(IBlk)	
__define_vtbl_end__(IBlk)

FBD_API __uint IBlk_level(IBlk * b)
{
	__uint l;
	ITreeNode * t;
	t = __ucast__(IBlk, ITreeNode, b);
	l = 0;
	while(t){
		l++;
		t = t->uplink;
	}
	return l;
}

/*
	find a pin's direct-peer pin, if any
	this function must be called on the target pin(because a source
	pin might have several peers)
	the peer need not be at the same level as p.
*/
FBD_API struct blk_pin_t * IBlk_peer(struct blk_pin_t *p)
{
	assert(!IBlk_is_output(p));
	switch(p->_class){
	case PIN_CLASS_EI:
	case PIN_CLASS_DI:
		return SAFE_CONTAINING_RECORD(
			p->u1.link.Head, 
			struct blk_pin_t, 
			u1.link
			);
	}

	return 0;
}

/*
	test if two pins are compatible to be connected.
	to be compatible, the two pins must be 
	1) one of them must be input pin, while the other output;
	2) as with data pins, subtype must be compatible, i.e., 
	both integer/bool/float;

	N.B.,`to be compatible' is different from `to be linkable',
	in order to be linkable, two pins must be at the same
	level, i.e., their containing blocks must have a same parent
	block, therefore, s and t might not be at the same level
	when executing this function.
*/
__bool IBlk_compatible(struct blk_pin_t * s, struct blk_pin_t * t)
{
	if(!IBlk_connectable(s) || !IBlk_connectable(t)){
		return __false;
	}

	s = IBlk_real_pin(s);
	t = IBlk_real_pin(t);

	switch(s->_class){
	case PIN_CLASS_EI:
		return(t->_class == PIN_CLASS_EO? __true : __false);
			return __true;
		
	case PIN_CLASS_EO:
		return (t->_class == PIN_CLASS_EI? __true : __false);
		
	case PIN_CLASS_DI:
		if(t->_class != PIN_CLASS_DO){
			return __false;
		}		
		break;
		
	case PIN_CLASS_DO:
		if(t->_class != PIN_CLASS_DI){
			return __false;
		}		
		break;
		
	default:
		assert(0);
		return __false;
	}


	if( s->type == t->type ){
		return __true;
	}

#define mkcompt(t1,t2) \
if((s->type==PIN_T_##t1 && t->type == PIN_T_##t2) || (s->type==PIN_T_##t2 && t->type == PIN_T_##t1)) \
	return __true;

	mkcompt(INTEGER, DWORD)
	mkcompt(BYTE, CHAR)
	mkcompt(SHORT, WORD)
	mkcompt(DATE, DOUBLE)

	return __false;
}

FBD_API __bool IBlk_linkable(struct blk_pin_t * source, struct blk_pin_t * target)
{
	IBlk *b;
	b = parent_blk(source->blk);
	if(b != parent_blk(target->blk) || !b){
		return __false;
	}
	// 2006-6-6, cannot connect two pins from the same block, as in the graphical
	// configuration environment the link will be hidden under the block, thus it will
	// become un-editable
	if(source->blk == target->blk){
		return __false;
	}
	return IBlk_compatible(source, target);
}

FBD_API __bool IBlk_is_output(struct blk_pin_t * t)
{
	t = IBlk_real_pin(t);
	if(t->_class == PIN_CLASS_EI || t->_class ==  PIN_CLASS_DI){
		return __false;
	}
	return __true;
}

/*
	IBlk::connect(p1, p2)
	make connection on two pins
*/
__bool IBlk_connect(struct blk_pin_t * s, struct blk_pin_t * t)
{
	assert(IBlk_compatible(s, t));

	s = IBlk_real_pin(s);
	t = IBlk_real_pin(t);

	s->binding = PIN_B_MEMORY;
	t->binding = PIN_B_MEMORY;

	switch(s->_class){
	case PIN_CLASS_EI:
	case PIN_CLASS_DI:
		RtkInsertTailTripleList(&t->u1.link, &s->u1.link);
		break;
	case PIN_CLASS_EO:
	case PIN_CLASS_DO:
		RtkInsertTailTripleList(&s->u1.link, &t->u1.link);
		break;
	default:
#ifdef HAVE_STDIO
		assert(0);
#endif
		break;
	}

	return __true;
}

/*
	allow multiple links on the same pin?
*/
FBD_API __bool IBlk_is_multiple(struct blk_pin_t * t)
{
	t = IBlk_real_pin(t);
	if(t->_class == PIN_CLASS_DO || t->_class == PIN_CLASS_EO){
		return __true;
	}
	return __false;
}

/*
	determine if two basic pins are connected together
*/
FBD_API __bool IBlk_connected(struct blk_pin_t *s, struct blk_pin_t * t)
{
	switch(s->_class){
	case PIN_CLASS_EO:
	case PIN_CLASS_DO:
		if(t->u1.link.Head == &s->u1.link){
			return __true;
		}
		break;
	default:
		assert(0);
	}

	return __false;
}

/*
	physically disconnect a basic pin from its peer(s)
*/
__bool IBlk_disconnect(struct blk_pin_t * t)
{
	struct blk_pin_t * s;

	t = IBlk_real_pin(t);
	s = IBlk_peer(t);

	assert(!IBlk_is_output(t));

	if(RtkIsListEmpty(&t->u1.link)){
		return __false;
	}

	t->binding = PIN_B_CONST;
	t->mdp = 0;

	RtkRemoveEntryTripleList(&t->u1.link);
	
	return __true;
}

/*
	test if a new link object is allowed to be placed on the pin
	object.
*/
FBD_API __bool IBlk_connectable(struct blk_pin_t * p)
{
	p = IBlk_real_pin(p);
	if(p->flags & PIN_F_HIDDEN){
		return __false;
	}
	switch(p->_class){
	case PIN_CLASS_EI:
		return !(p->blk->h.flags & BLK_F_AUTOSCAN) 
			&& 
			RtkIsTripleListEmpty(&p->u1.link)? 
			__true : __false;
		break;
	case PIN_CLASS_DI:
		return RtkIsTripleListEmpty(&p->u1.link)? __true : __false;
	case PIN_CLASS_DO:
	case PIN_CLASS_EO:
		return __true;
	}
	return __false;
}

/*
*/
FBD_API __bool IBlk_exportable(struct blk_pin_t * p)
{
	IBlk * b;
	b = parent_blk(p->blk);
	if(!b){
		return __false;
	}
	if(b->h.flags & BLK_F_READONLY){
		return __false;
	}
	p = IBlk_real_pin(p);
	switch(p->_class){
	case PIN_CLASS_EI:
	case PIN_CLASS_DI:
		return RtkIsTripleListEmpty(&p->u1.link)? __true : __false;
	case PIN_CLASS_DO:
	case PIN_CLASS_EO:
		return __true;
	}
	return __false;
}

FBD_API IBlk * IBlk_blk_by_id(IBlk * _this, __u16 id)
{
	if(id >= _this->h.u1.n_subblocks){
		return 0;
	}
	_this = first_subblk(_this);
	while(_this && id){
		_this = next_blk(_this);
		id--;
	}
	return _this;
}

FBD_API __u16 IBlk_id_by_blk(IBlk * _this)
{
	__u16 n;
	IBlk * par;
	par = parent_blk(_this);
	if( !par ){
		return 0;
	}
	n = 0;
	par = first_subblk(par);
	while(par != _this){
		par = next_blk(par);
		n++;
	}
	return n;
}

/*
	find a physical pin object from the logical pin object
	(the two might be the same)
*/
FBD_API struct blk_pin_t * IBlk_real_pin(struct blk_pin_t *p)
{
	switch(p->_class){
	case PIN_CLASS_EXP:
		return p->u1.x.real_pin;
	default:
		return p;
	}
}

FBD_API IBlk * IBlk_subblk(IBlk * _this, f8_uuid * uuid)
{
	_this = first_subblk(_this);
	while(_this){
		if(_this->uuid == *uuid){
			return _this;
		}
		_this = next_blk(_this);
	}
	return _this;
}

static int _get_binary_type(IF8Stream * s)
{
	f8_i16 ftype;
	__vcall__(s, get, (s, &ftype, sizeof(ftype)));
	__vcall__(s, seek, (s, -2, SEEK_CUR));
	return ftype;
}

FBD_API IBlk * IBlk_from_stream(IF8Stream * s)
{
	IBlk * blk;
	IBBlk * bblk;
	ICBlk * cblk;
	
	switch(_get_binary_type(s)){
	case BBLK_MAGIC:
		bblk = IBBlk_from_stream(s);
		blk = __ucast__(IBBlk, IBlk, bblk);
		break;
	case CBLK_MAGIC:
		cblk = ICBlk_from_stream(s);
		blk = __ucast__(ICBlk, IBlk, cblk);
		break;
	default:
		blk = 0;
	}

	return blk;
}

/*
	called before umount.
	saying good-bye to buddies.
*/
static void IBlk_before_umount(ITreeNode * me)
{
	IBlk *blk, *pb;
	struct blk_pin_t * p;
	struct cblk_link_t *lk, *nlk;

	blk = __dcast__(ITreeNode, IBlk, me);
	pb = parent_blk(blk);
	
	if(pb){
		assert(pb->h.magic == CBLK_MAGIC);
		assert(pb->h.u1.n_subblocks >= 1);
		pb->h.u1.n_subblocks--;
	}

	p = __vcall__(blk, first_pin, (blk));
	while(p){
		/*
			Notify parent block that we're about to remove 
			this pin defs. N.B. this will automaticly remove 
			associated connections.
		*/
		if(p->ulink){
			ICBlk_del_pin(p->ulink);
			p->ulink = 0;
		}
		lk = ICBlk_first_connection(p);
		while(lk){
			nlk = ICBlk_next_connection(p, lk);
			ICBlk_unlink(lk);
			lk = nlk;
		}
		p = __vcall__(blk, next_pin, (p));
	}
}

static void IBlk_after_umount(ITreeNode * me)
{
	/*
		because block objects can only be created on the
		heap, it's safe and reasonable to destroy them 
		after umounted
	*/
	delete_blk(__dcast__(ITreeNode, IBlk, me));
}

FBD_API __uint IBlk_pinsize(struct blk_pin_t * p)
{
	switch(p->type){
	case PIN_T_BOOL:
	case PIN_T_CHAR:
	case PIN_T_BYTE:
		return 1;
	case PIN_T_SHORT:
	case PIN_T_WORD:
		return 2;
	case PIN_T_INTEGER:
	case PIN_T_DWORD:
	case PIN_T_FLOAT:
		return 4;
	case PIN_T_DOUBLE:
	case PIN_T_DATE:
		return 8;
	}
	return 0;
}


#ifdef _WIN32

FBD_API IBlk * delphi_parent(IBlk * b)
{
	return parent_blk(b);
}

FBD_API IBlk * delphi_firstsub(IBlk * b)
{	
	b = first_subblk(b);
	return b;
}

FBD_API IBlk * delphi_next(IBlk * b)
{
	b = next_blk(b);
	return b;
}

#endif

FBD_API void delete_blk(IBlk * __this)
{
	IBBlk * bblk;
	ICBlk * cblk;

	if(!__this){
		return;
	}

	switch(__this->h.magic){
	case BBLK_MAGIC:
		bblk = __dcast__(IBlk, IBBlk, __this);
		__destroy__(bblk);
		mm_free(g_hBlkHeap, bblk);
		break;
	case CBLK_MAGIC:
		cblk = __dcast__(IBlk, ICBlk, __this);
		__destroy__(cblk);
		mm_free(g_hBlkHeap, cblk);
		break;
	}
}

__global_constructor__(IBlk)
{
	__v_copy__(IBlk, ITreeNode);
	__override__(IBlk, ITreeNode, before_umount);
	__override__(IBlk, ITreeNode, after_umount);
}

__constructor__(IBlk)
{
	if(__this){
		__touch__();
		__create_base__(IBlk, ITreeNode, __this);
		memset(&__this->h, 0, sizeof(__this->h));
	}
	return __this;
}

/*
	hub of all IBlk desendant's destructors
*/
__destructor__(IBlk)
{
#ifdef F8_CONFIGURATOR
	extern __bool IBlk_unload_resource(IBlk *blk);
	IBlk_unload_resource(__this);
#endif
	__destroy_base__(IBlk, ITreeNode, __this);
	return __this;
}

#ifndef _MSC_VER
FBD_API void umount_blk(IBlk * blk)
{
	ITreeNode * nd;
	nd = __ucast__(IBlk, ITreeNode, blk);
	ITreeNode_umount_children(nd);
	__vcall__(nd, before_umount, (nd));
	ITreeNode_umount(nd);
	__vcall__(nd, after_umount, (nd));
}
#endif

