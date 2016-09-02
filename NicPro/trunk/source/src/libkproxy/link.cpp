 /*
	This module manages configurations related to links.
	by J.Hunter, 2003/9/18
*/
#include "precomp.h"

#define DBG_MMAP	0

/*
	sp : source pin
	tp : target pin

	they must be linkable(compatible and target pin is free)
	
	expected errors :
	1) network error
*/
int _link_pins(struct blk_pin_t * sp, struct blk_pin_t * tp, struct cblk_link_t ** rlk)
{
	struct blk_pin_t *rsp, *rtp;
	struct cblk_link_t * lk;
	bool	bShouldRestoreAddrIfFailed[2] = {false, false};
	mem_addr_t oldAddr[2];
	int	length;
	__u16	id;
	f8_uuid	uuid;
	IBlk * pb;

	_shproc_prolog_no_k_();

	if(rlk){
		*rlk = 0;
	}
	
	pb = parent_blk(sp->blk);
	if(pb->h.flags & BLK_F_READONLY){
		return F8_PERMISSION;
	}

	rsp = IBlk_real_pin (sp);
	rtp = IBlk_real_pin (tp);

#if DBG_MMAP
	utils_debug("mmap before link:\n");
	on_mmap("mmap", 0);
#endif

	if(rsp->_class == PIN_CLASS_DO){
		length = _type_size(rsp->type);
		if(rtp->binding == PIN_B_MEMORY){
			if(get_address_owner(&rtp->u2.mem_addr, length, 0) == rtp){
				bShouldRestoreAddrIfFailed[1] = true;
				free_reg(&rtp->u2.mem_addr, length);
				oldAddr[1] = rtp->u2.mem_addr;
			}
		}
	}
	
	create_f8_uuid(&uuid);
	__vcall__(s, put, (s, &uuid, sizeof(f8_uuid)));
	
	__vcall__(s, put, (s, &sp->blk->uuid, sizeof(f8_uuid)));
	id = __vcall__(sp->blk, id_by_pin, (sp));
	__vcall__(s, put, (s, &id, sizeof(id)));

	__vcall__(s, put, (s, &tp->blk->uuid, sizeof(f8_uuid)));
	id = __vcall__(tp->blk, id_by_pin, (tp));
	__vcall__(s, put, (s, &id, sizeof(id)));

	if(rsp->_class == PIN_CLASS_DO){
		__vcall__(s, put, (s, &rsp->u2.mem_addr, sizeof(mem_addr_t)));
	}

	cmd.cmd = F8_LINK;
	cmd.isize = ms.pointer;
	create_f8_uuid(&NEW_ID(&cmd));
	
#if DBG_MMAP
	utils_debug("mmap after link:\n");
	on_mmap("mmap", 0);
#endif

	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, 2000));

	if(!successful){
		if(rsp->_class == PIN_CLASS_DO){
			if(bShouldRestoreAddrIfFailed[1]){
				rtp->u2.mem_addr = oldAddr[1];
				set_address_owner(rtp, &oldAddr[1], length, PIN_MAGIC, __true);
			}
		}
		return cmd.retcode;
	}

	lk = ICBlk_get_link(__dcast__(IBlk, ICBlk, pb), &uuid);
	assert(lk);	
	create_resource(&lk->uuid);

	if(rlk){
		*rlk = lk;
	}
	
	return F8_SUCCESS;
}

int on_link(char *v, void *k)
{
	CArgsEx a;
	struct blk_pin_t *sp, *tp, *rsp;

	a.parse(v, FS);

	if(a.argc < 3){
		utils_error("Usage : link <source pin> <target pin>\n");
		return F8_SYNTAX;
	}

	utils_trim(a.argv[1],  '"');
	utils_trim(a.argv[2], '"');

	sp = IBlk_pin_by_path(sections, a.argv[1]);
	tp = IBlk_pin_by_path(sections, a.argv[2]);
	if(!sp || !tp){
		return F8_PATH_NOT_FOUND;
	}
	
	if(!IBlk_is_output(sp)){
		rsp = sp;
		sp = tp;
		tp = rsp;
	}

	if(!IBlk_linkable(sp, tp)){
		return F8_PIN_INCOMPATIBLE;
	}

	return _link_pins(sp, tp, 0);
}

int on_ulink(char *v, void *k)
{
	CArgsEx a;
	IBlk * pb;
	struct blk_pin_t *t;
	struct cblk_link_t *lk;
	f8_uuid	saved_uuid;

	_shproc_prolog_();
		
	a.parse(v, FS);

	if(a.argc < 2){
		utils_error("Usage : unlink <target pin> \n");
		return F8_SYNTAX;
	}

	t = IBlk_pin_by_path(sections, a.argv[1]);
	if( IBlk_is_output(t)){
		utils_error(
			"Error : links can only be removed from the target end.\n"
			);
		return F8_SYNTAX;
	}

	pb = parent_blk(t->blk);
	if(pb->h.flags & BLK_F_READONLY){
		return F8_PERMISSION;
	}

	lk = ICBlk_first_connection(t);
	if(!lk){
		return F8_OBJECT_NOT_FOUND;
	}

	saved_uuid = lk->uuid;

	__vcall__(s, put, (s, &pb->uuid, sizeof(f8_uuid)));
	__vcall__(s, put, (s, &lk->uuid, sizeof(f8_uuid)));
	
	cmd.cmd = F8_UNLINK;
	cmd.isize = ms.pointer;
	create_f8_uuid(&NEW_ID(&cmd));
	
	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, psp->timeout));

	if(!successful){
		return cmd.retcode;
	}

	unload_resource(&saved_uuid);

	return F8_SUCCESS;
}

