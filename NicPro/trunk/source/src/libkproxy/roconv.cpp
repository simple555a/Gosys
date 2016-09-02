/*
	folding all instance resources of sub-blocks/pins/links/variables
	of a composite block.
*/
#include "precomp.h"

#define ConvBlock	0
#define ConvLink	1
#define ConvPin		2
#define ConvSelf	3

CROConverter::CROConverter(IBlk *b)
{
	par_blk = b;
}

void CROConverter::convert_blk(const char * key, const void *buf, int len)
{
	IBlk *b;
	int idx;
	char newKey[F8_MAX_KEY_LENGTH];
	
	b = (IBlk*)curObj;
	idx = IBlk_id_by_blk(b);
	sprintf(newKey, "!b%d!%s", idx, key);

	set_res_buf(&par_blk->h.uuid, buf, len, newKey);
}

void CROConverter::convert_pin(const char * key, const void *buf, int len)
{
	struct cblk_pin_t *cp;
	int idx;
	char newKey[F8_MAX_KEY_LENGTH];

	cp = (cblk_pin_t*)curObj;
	idx = __vcall__(par_blk, id_by_pin, (&cp->p));
	sprintf(newKey, "!p%d!%s", idx, key);

	set_res_buf(&par_blk->h.uuid, buf, len, newKey);
}

void CROConverter::convert_lnk(const char * key, const void *buf, int len)
{
	struct cblk_link_t *b;
	char newKey[F8_MAX_KEY_LENGTH];

	b = (cblk_link_t*)curObj;
	sprintf(newKey, "!l%d!%s", curIndex, key);

	set_res_buf(&par_blk->h.uuid, buf, len, newKey);
}

__bool CROConverter::_ei_proc(
	const char * key,
	const void * buf,
	int len,
	int context
	)
{
	CROConverter * c = (CROConverter*)context;

	// ok, this resource item should be converted into a 
	// sub-item

	switch(c->curAction){
	case ConvBlock:
		c->convert_blk(key, buf, len);
		break;
	case ConvLink:
		c->convert_lnk(key, buf, len);		
		break;
	case ConvPin:
		c->convert_pin(key, buf, len);
		break;
	case ConvSelf:
		break;
	}
	
	return __true;
}

/*
	make block a read-only block
	all subblocks are guarranteed to be readonly already,
	because block is traversed in post-order.
*/
void CROConverter::make_readonly(IBlk * blk)
{
	f8_uuid oldId;
	IBlk	*sb;
	struct cblk_link_t * lk;
	struct cblk_pin_t  * cp;
	struct blk_pin_t * p;

	par_blk = blk;
	par_cblk = __dcast__(IBlk, ICBlk, blk);

	oldId = blk->h.uuid;
	create_f8_uuid(&blk->h.uuid);
	if(query_resource(&oldId) >= 0){
		move_resource(&oldId, &blk->h.uuid);
	}else{
		create_resource(&blk->h.uuid);
	}

	// duplicate resource
	curAction = ConvSelf;
	
	// copy instance resource of subblocks/links/pins to 
	// class resource of current block
	curAction = ConvBlock;	
	curIndex = 0;
	sb = first_subblk(blk);
	while(sb){
		curObj = sb;
		enum_res_items(&sb->uuid, _ei_proc, (int)this);
		sb = next_blk(sb);
		curIndex++;
	}

	curAction = ConvLink;
	lk = ICBlk_first_link(par_cblk);
	curIndex = 0;
	while(lk){
		curObj = lk;
		enum_res_items(&lk->uuid, _ei_proc, (int)this);
		lk = ICBlk_next_link(lk);
		curIndex++;
	}
	
	curAction = ConvPin;
	p = __vcall__(blk, first_pin, (blk));
	curIndex = 0;
	while(p){
		cp = SAFE_CONTAINING_RECORD(p, struct cblk_pin_t, p);
		curObj = cp;
		enum_res_items(&cp->uuid, _ei_proc, (int)this);
		p = __vcall__(blk, next_pin, (p));
		curIndex++;
	}


	// set flag to read-only
	blk->h.flags |= BLK_F_READONLY;
	set_res_int(&blk->h.uuid, "!readonly", 1);
}

/*
	gather class resources and save to a stream
	1) class resource of current block
	2) instance resources of all sub blocks
	3) instance resources of all links
	4) instance resources of all exported pins
*/
int CROConverter::convert()
{
	make_readonly(par_blk);
	return F8_SUCCESS;
}
