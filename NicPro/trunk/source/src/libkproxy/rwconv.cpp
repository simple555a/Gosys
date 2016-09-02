/*
	unpack the resources of a composite block
*/
#include "precomp.h"

CRWConverter::CRWConverter(IBlk *b)
{
	par_blk = b;
	par_cblk = __dcast__(IBlk, ICBlk, (b));
}

void CRWConverter::convert_blk(const char * key, const void *buf, int len)
{
	CArgsEx args;
	IBlk *b;
	int idx;

	args.parse(key+2, "!");
	idx = atoi(args[0]);
	
	b = first_subblk(par_blk);
	while(idx--){
		b = next_blk(b);
	}

	// make sure resource present
	create_resource(&b->uuid);

	if(args.argc == 1){
		// name resource
		set_res_name(&b->uuid, (const char *)buf);
		return;
	}

	set_res_buf(&b->uuid, buf, len, args[1]);
}

void CRWConverter::convert_pin(const char * key, const void *buf, int len)
{
	CArgsEx args;
	struct blk_pin_t *p;
	struct cblk_pin_t *cp;
	int idx;

	args.parse(key+2, "!");
	idx = atoi(args[0]);
	
	p = __vcall__(par_blk, pin_by_id, (par_blk, idx));
	assert(p);
	assert(p->_class == PIN_CLASS_EXP);
	cp = SAFE_CONTAINING_RECORD(p, struct cblk_pin_t, p);

	// make sure resource present
	create_resource(&cp->uuid);

	if(args.argc == 1){
		// name resource
		set_res_name(&cp->uuid, (const char *)buf);
		return;
	}

	set_res_buf(&cp->uuid, buf, len, args[1]);
}

void CRWConverter::convert_lnk(const char * key, const void *buf, int len)
{
	CArgsEx args;
	struct cblk_link_t *b;
	int idx;

	args.parse(key+2, "!");
	idx = atoi(args[0]);
	
	b = ICBlk_first_link(par_cblk);
	while(idx--){
		b = ICBlk_next_link(b);
	}

	// make sure resource present
	create_resource(&b->uuid);

	if(args.argc == 1){
		// name resource
		set_res_name(&b->uuid, (const char *)buf);
		return;
	}

	set_res_buf(&b->uuid, buf, len, args[1]);
}

/*
	find sub resources, and dispatch 'em to corresponding
	subblocks/links/pins
*/
__bool CRWConverter::_ei_proc(
	const char * key,
	const void * buf,
	int len,
	int context
	)
{
	CArgsEx args;
	int i;
	CRWConverter * c = (CRWConverter*)context;

	if(strlen(key) < 3){
		return __true;
	}
	if(*key != '!'){
		return __true;
	}
	if(!strchr("bpl", key[1])){
		return __true;
	}

	args.parse(key, "!");
	if(args.argc < 2){
		return __true;
	}
	for(i=strlen(args[0]) - 1; i>=1; i--){
		if(!isdigit(args[0][i])){
			return __true;
		}
	}

	// this is a sub-resource
	// ok, this resource item should be converted into a 
	// sub-item

	switch(key[1]){
	case 'b':
		c->convert_blk(key, buf, len);
		break;
	case 'p':
		c->convert_pin(key, buf, len);
		break;
	case 'l':
		c->convert_lnk(key, buf, len);
		break;
	}
	
	// remove this item
	del_res_item(&c->par_blk->h.uuid, key);

	return __true;
}

int CRWConverter::convert()
{
	char	uname[64];
	f8_uuid newId;

	create_f8_uuid(&newId);
	enum_res_items(&par_blk->h.uuid, _ei_proc, (__int)this);
	sprintf(uname, "autocreated");

	// now detach from template resource
	unload_resource(&par_blk->h.uuid);
	create_resource(&newId);
	par_blk->h.uuid = newId;
	// set_res_name(&newId, uname);

	// modify flags
	par_blk->h.flags &= ~BLK_F_READONLY;

	return F8_SUCCESS;
}
