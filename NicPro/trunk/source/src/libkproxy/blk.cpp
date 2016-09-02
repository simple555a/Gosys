 /*
	This module manages configurations related to blocks.
	by J.Hunter, 2003/9/18
*/
#include "precomp.h"

#define LOCK_INSTANCE_RES	1
#define LOCK_CLASS_RES		2

static int _make_writable(IBlk * blk);

static __bool _ei_proc(
	const char * key, 
	const void * buf, 
	__int len, 
	__int context
	)
{
	if(((char*)buf)[len - 1] == 0){
		utils_trace("\t%s = %s\n", key, buf);
	}
	return __true;
}

static __bool _er_proc(
	const f8_uuid * id, 
	__int context
	)
{
	char buf[64];
	f8_uuid_to_string(id, buf, sizeof(buf));
	utils_trace("Contents of resource : %s\n", buf);
	enum_res_items(id, _ei_proc, 0);
	return __true;
}

void _dump_res(const f8_uuid * id)
{
	if(id){
		enum_res_items(id, _ei_proc, 0);
	}else{
		enum_resources(_er_proc, 0);
	}	
}

/*
	generate uuids for contained links/pins/subblocks
*/
static __bool _inst_proc(ITreeNode * nd, __bool firstVisit, __uint not_used)
{
	IBlk * blk;
	ICBlk * cblk;
	struct cblk_link_t * lk;
	struct blk_pin_t  * p;
	struct cblk_pin_t * cp;
	f8_uuid	oldUuid;

	blk = __dcast__(ITreeNode, IBlk, nd);
	
	oldUuid = blk->uuid;
	create_f8_uuid(&blk->uuid);
	move_resource(&oldUuid, &blk->uuid);

	if(blk->h.magic == CBLK_MAGIC){
		cblk = __dcast__(IBlk, ICBlk, (blk));
		
		lk = ICBlk_first_link(cblk);
		while(lk){
			oldUuid = lk->uuid;
			create_f8_uuid(&lk->uuid);
			move_resource(&oldUuid, &lk->uuid);
			lk = ICBlk_next_link(lk);
		}

		p = __vcall__(blk, first_pin, (blk));
		while(p){
			cp = SAFE_CONTAINING_RECORD(p, struct cblk_pin_t, p);
			oldUuid = cp->uuid;
			create_f8_uuid(&cp->uuid);
			move_resource(&oldUuid, &cp->uuid);
			p = __vcall__(blk, next_pin, (p));
		}
	}
	
	return __true;
}

/*
*/
static __bool _dup_inst_proc(ITreeNode * nd, __bool firstVisit, __uint not_used)
{
	IBlk * blk;
	ICBlk * cblk;
	struct cblk_link_t * lk;
	struct blk_pin_t  * p;
	struct cblk_pin_t * cp;
	f8_uuid	oldUuid;

	blk = __dcast__(ITreeNode, IBlk, nd);
	
	oldUuid = blk->uuid;
	create_f8_uuid(&blk->uuid);
	assert(query_resource(&oldUuid) > 0);
	copy_resource(&oldUuid, &blk->uuid);

	load_resource(&blk->h.uuid);

	if(blk->h.magic == CBLK_MAGIC){
		cblk = __dcast__(IBlk, ICBlk, (blk));
		
		lk = ICBlk_first_link(cblk);
		while(lk){
			oldUuid = lk->uuid;
			create_f8_uuid(&lk->uuid);
			assert(query_resource(&oldUuid) > 0);
			copy_resource(&oldUuid, &lk->uuid);
			lk = ICBlk_next_link(lk);
		}

		p = __vcall__(blk, first_pin, (blk));
		while(p){
			cp = SAFE_CONTAINING_RECORD(p, struct cblk_pin_t, p);
			oldUuid = cp->uuid;
			create_f8_uuid(&cp->uuid);
			assert(query_resource(&oldUuid) > 0);
			copy_resource(&oldUuid, &cp->uuid);
			p = __vcall__(blk, next_pin, (p));
		}
	}
	
	return __true;
}

static __bool _remove_pin_bindings(ITreeNode * nd, __bool firstVisit, __uint context)
{
	IBlk * blk;
	int i;
	struct blk_pin_t * p;

	blk = __dcast__(ITreeNode, IBlk, nd);
	if(blk->h.magic != BBLK_MAGIC){
		return __true;
	}
	
	for(i=0, p=__vcall__(blk, first_pin, (blk)); 
		i<blk->h.n_pins; 
		i++, p++
	){
		if(p->_class == PIN_CLASS_DI){
			/*
				if the pin is connected to another pin,
				then remove the assigned address
				note in later time the address might be
				re-constructed by a link-operation
			*/
			if(!RtkIsTripleListEmpty(&p->u1.link)){
				p->binding = PIN_B_CONST;
			}
		}
	}

	return __true;
}

int _instanciate_blk(IBlk * b)
{
	_traverse(b, _inst_proc, 0);
	return F8_SUCCESS;
}

/*
	duplicate a block. perserve everything except instance UUIDs
*/
int _duplicate_blk(IBlk * src, IBlk **b)
{
	ITempStream * ts;
	IF8Stream		* s;

	__new__(ITempStream, ts);
	if(!ts){
		return F8_LOW_MEMORY;
	}

	// duplicate structure
	s = __ucast__(ITempStream, IF8Stream, ts);
	__vcall__(src, save, (src, s));
	__vcall__(s, seek, (s, 0, SEEK_SET));
	*b = IBlk_from_stream(s);

	// duplicate instance resource
	if(*b){
		_traverse(*b, _dup_inst_proc, 0);
		_traverse(*b, _remove_pin_bindings, 0);
	}

	__delete__(ts);

	return *b? F8_SUCCESS : F8_INVALID_DATA;
}

/*
	make a stream package describing the variable
	info. consumed by block denoted by ``src'' into
	instance resource of ``blk''.
*/
void _pack_variables(IBlk * src, IBlk * blk)
{
	// pack variables
	f8_uint i;
	f8_uint nv = get_var_count(src);
	if(!nv){
		return;
	}
	blk_var_t vvv[32], *vv;
	if(nv < 32){
		vv = vvv;
	}else{
		vv = new blk_var_t[nv];
	}
	query_var_list(src, vv, &nv);
	set_res_buf(
		&blk->h.uuid, 
		vv, 
		sizeof(blk_var_t)*nv, 
		"variables"
		);
	for(i=0; i<nv; i++){
		set_res_buf_v(
			&blk->h.uuid, 
			vv[i].name, 
			-1, 
			"!v%d!name", 
			i
			);
		set_res_buf_v(
			&blk->h.uuid, 
			vv[i].comment,
			-1, 
			"!v%d!comment", 
			i
			);
	}
	if(vv != vvv){
		delete vv;
	}
}

/*
	make a read/write block readonly and save to block library.

	*) check if the block is valid for a readonly block, i.e., all 
	subblocks are readonly;

	*) the block hive cannot reference non-local variable
	
	*) modify the block's flag to read-only

	*) remove all memory bindings from pin, recursively

	*) save block binary block to a file, then put the file in 
	block library.

	*) convert read/write resources to read-only resources, then
	put into block library.
*/
static int _uninstanciate(IBlk * blk, const char * name)
{
	IBlk * sb;
	IBlk * tb;
	f8_status rt;
	const struct blk_var_t * v;
	struct blk_pin_t *p, *rp;

	if(blk->h.flags & BLK_F_READONLY){
		return F8_INVALID_OPERATION;
	}

	sb = first_subblk(blk);
	while(sb){
		if(!(sb->h.flags & BLK_F_READONLY)){
			return F8_INVALID_OPERATION;
		}
		if(sections != blk){
			p = __vcall__(sb, first_pin, (sb));
			while(p){
				v = query_var_by_pin(p);
				if(v && v->blk != blk){
					// non-local variable refrence detected
					/*
						this is not allowed as it will cause
						the saved block unusable when
						inserted to a new project.
					*/
					return F8_UNRESOLVED_REFERENCES;
				}
				p = __vcall__(sb, next_pin, (p));
			}
		}
		sb = next_blk(sb);
	}

	rt = _duplicate_blk(blk, &tb);
	if(F8_FAILED(rt)){
		return rt;
	}
	
#if defined(never_defined)
	blk_disassembly_ex(blk, f8_true);
	blk_disassembly_ex(tb, f8_true);
#endif

	sb = first_subblk(tb);
	while(sb){
		for(p = __vcall__(sb, first_pin, (sb)); p; p = __vcall__(sb, next_pin, (p))){
			//
			// if there's an EXPLICIT reference to some
			// variable?
			//
			rp = IBlk_real_pin(p);
			if(rp->_class == PIN_CLASS_EI || rp->_class == PIN_CLASS_EO){
				continue;
			}
			v = query_var_by_pin(p);			
			if(v){
				p->binding = PIN_B_MEMORY;
				p->u2.mem_addr.section = MEM_SECTION_VAR;
				p->u2.mem_addr.addr = get_var_id(v);
			}else{
				if(rp->_class == PIN_CLASS_DI){
					p->binding = PIN_B_CONST;
					memcpy(p->dp, rp->dp, _type_size(rp->type));
				}else{
					p->binding = PIN_B_MEMORY;
				}
				p->u2.mem_addr.section = MEM_SECTION_INVALID;
			}
		}
		sb = next_blk(sb);
	}		

	CROConverter conv(tb);
	conv.convert();

	_pack_variables(blk, tb);
	
	// _dump_res(&tb->h.uuid);

	set_res_name(&tb->h.uuid, name);
	rt = blklib_save(name, tb, 0);

	delete_blk(tb);

	return rt;
}

/*
	mark a block as read/write, this involves two steps:
	1) convert readonly resources to read/write resources
	2) modify the block's flags as writable
	N.B. resource items might get lost during convertion.
*/
static int _make_writable(IBlk * blk)
{
	if(blk->h.magic == BBLK_MAGIC){
		return F8_NOT_A_CBLK;
	}
	if(!(blk->h.flags & BLK_F_READONLY)){
		// already read/write
		return F8_ACCESS;
	}

	CRWConverter c(blk);
	c.convert();

	return F8_SUCCESS;
}

/*
	a block can be mounted from various sources:
	
	1) the on-disk block database
	2) a block in the same controll program hierachy
	3) windows clipboard
	
	the common thing is that we can construct a IBlk for all these 
	situations, and then work on this IBlk to do the rest of the mount 
	task, such an intermediate IBlk is called a `standard block', a 
	standard block is characterized by the following:

	1) a valid name;
	2) pertaining memory allocated through the f8-binary library;
	3) resources for all objects contained in the block tree(pins/
	links/subblocks);
	4) with register allocations for pins of PIN_B_LINK bindings;
	5) with or without register allocations for pins of PIN_B_MEMORY 
	bindings;
	6) all instance ids are generated randomly to prevent duplicate
	ids.

	到达这里的时候，一定要保证所有需要的信息都没有丢失。
	Parameters:
*/
int _mount_blk(
	IBlk * peer, 
	IBlk * sb,	/* an instanciated block */
	const char * name,
	bool keepMemoryBindings,
	void * k
	)
{
	HREG_LIST hRegList;
	f8_status code;

	_shproc_prolog_();

	__vcall__(s, put, (s, &peer->uuid, sizeof(f8_uuid)));	

#if 0
	kproxy_shell("memusage");
#endif

	code = alloc_blk_reg_list(sb, &hRegList);
#if 0
	kproxy_shell("memusage");
#endif
	if(F8_FAILED(code)){
		return code;
	}

	__vcall__(sb, save, (sb, s));
	
	// ok,command stream ok, now emitting the command
	cmd.cmd = F8_MOUNT_BLOCK;
	cmd.isize = ms.pointer;
	create_f8_uuid(&NEW_ID(&cmd));

	__vcall__(
		proxy_adapter, 
		exec, 
		(proxy_adapter, &cmd, 2000)
		);

	/*
		reaching here, there might be two blocks with the same 
		instance ID, and this is the only exception to the normal 
		usage of instance ID.
	*/
	if(successful){
		sb = ke_get_blk(proxy_adapter->kernel, &sb->uuid);
		assert(sb);
		set_res_name(&sb->uuid, name);
		
		/* 
		lock the resources for an additional time, thus 
		when the block is deleted, the resources will not
		evapourize
		*/
		probe_blk_resources(sb);
		commit_reg_list(hRegList, __false);
#if 0
		kproxy_shell("memusage");
#endif

		delete_blk_reg_list(hRegList, false);
	}else{
		delete_blk_reg_list(hRegList, true);
	}	

	return cmd.retcode;
}

static __bool _remove_variable_table(ITreeNode * nd, __uint context)
{
	IBlk * b;
	b = __dcast__(ITreeNode, IBlk, nd);
	if(b->h.magic == CBLK_MAGIC){
		// remove_variable_table(b);
	}
	return __true;
}

/*
	copy a block to a new position
	syntax:
	cp <source path> <destination path>
*/
int on_cp(char * v ,void *k)
{
	CArgsEx a;
	IBlk *sb;
	IBlk *peer;
	const char *name;
	char * bname;
	IBlk * tblk;
	f8_status retcode;

	a.parse(v, FS);
	if(a.argc != 3){
		return F8_SYNTAX;
	}
	sb = IBlk_blk_by_path(sections, a[1]);
	if(!sb){
		return F8_OBJECT_NOT_FOUND;
	}
	name = IBlk_name(sb);

	peer = IBlk_blk_by_path( sections, a[2]);
	if(!peer){
		/* 
			if peer is not an existing block, then it might be
			a full-qualified new block path name
		*/
		bname = strrchr(a.argv[2], '/');
		if(!bname){
			return F8_PATH_NOT_FOUND;
		}
		if(bname == a[2]){
			return F8_PATH_NOT_FOUND;
		}
		// ok, the peer path is indeed composed of two parts:
		// the path part and the name part
		*bname = 0;
		peer = IBlk_blk_by_path(sections, a[2]);
		if(!peer){
			return F8_PATH_NOT_FOUND;
		}
		name = bname+1;
		if(!is_valid_blk_name(name)){
			return F8_INVALID_DATA;
		}
	}
	if(BLK_IS_READONLY(peer)){
		return F8_PERMISSION;
	}
	if(peer->h.magic != CBLK_MAGIC){
		return F8_NOT_A_CBLK;
	}
	
	if(IBlk_subblk_by_name(peer, name)){
		return F8_NAME_DUPLICATE;
	}

	retcode = _duplicate_blk(sb, &tblk);
	if(F8_FAILED(retcode)){
		return retcode;
	}

	retcode = _mount_blk(peer, tblk, name, false, k);

	delete_blk(tblk);

	g_bDirty = __true;

	return retcode;
}

int on_entlib(char *v, void *k)
{
	CArgsEx args;
	IBlk * b;

	_shproc_prolog_();

	args.parse(v, FS);
	if(args.argc < 3){
		return F8_SYNTAX;
	}
	b = IBlk_blk_by_path(sections, args[1]);
	if(!b){
		return F8_OBJECT_NOT_FOUND;
	}

	if(!is_valid_blk_name(args[2])){
		return F8_INVALID_DATA;
	}

	return _uninstanciate(b, args[2]);
}

/* 
	mount a block into current running kernel
	syntax : 
	mount path=<path> name=<name> {file=<file> | type=<type>}
*/
int on_mount(char * v ,void *k)
{
	CArgsEx args;
	IBlk *peer;
	IBlk * sb;
	f8_status retcode;

	char *path=0;
	char *file=0;
	char *type=0;
	char *name=0;
	char * id=0;
	string exePath;
	__bool readWrite=__false;

	BEGIN_ARGS
		ARG("path=", path, "char *", "specify mount point(parent block)")
		ARG("file=", file, "char *", "specify new block by source file")
		ARG("type=", type, "char *", "specify new block by type")
		ARG("id=", id, "char *", "specify new block by uuid")
		ARG("name=", name, "char *", "specify new block name")
		ARG("rw", readWrite, "tbool", "mount as read/write")
	END_ARGS

	args.parse(v, " \t\n\r");

	if(process_args_1(args.argc, args.argv) < 0){
		print_usage_1();
		return F8_SYNTAX;
	}

	if(!path || !name){
		print_usage_1();
		return F8_SYNTAX;
	}

	peer = IBlk_blk_by_path(sections, path);
	if(!peer){
		return F8_PATH_NOT_FOUND;
	}

	if(!is_valid_blk_name(name)){
		return F8_INVALID_NAME;
	}

	if(IBlk_subblk_by_name(peer, name)){
		return F8_NAME_DUPLICATE;
	}

	if(peer->h.flags & BLK_F_READONLY){
		utils_error("%s : block is readonly.\n", path);
		return F8_ACCESS;
	}

	if(file){
		blklib_load_by_file(file, &sb);
		if(!sb && !strcmpi(file,"blank.blk")){
			exePath=get_exe_dir();
			blklib_load_by_file((exePath + file).c_str(), &sb);
		}
	}else if(type){
		blklib_load_by_type(type, &sb);
	}else if(id){
		blklib_load_by_id(id, &sb);
	}else{
		print_usage_1();
		return F8_SYNTAX;
	}

#if 0
	kproxy_shell("vres");
#endif

	if(!sb){
		return F8_FILE_NOT_FOUND;
	}

	_instanciate_blk(sb);
	if(readWrite){
		_make_writable(sb);
	}

#if 0
	kproxy_shell("vres");
#endif

	retcode = _mount_blk(
		peer, 
		sb, 
		name, 
		false, 
		k
		);
	
#if 0
	kproxy_shell("vres");
#endif

	delete_blk(sb);

#if 0
	kproxy_shell("vres");
#endif
	g_bDirty = __true;

	return retcode;
}

/*
	umount a block
*/
int on_umount(char * v, void *k)
{
	CArgsEx args;
	IBlk *b, *pb;
	HREG_LIST	hRegList;

	_shproc_prolog_();

	args.parse(v, FS);
	if(args.argc != 2){
		return F8_SYNTAX;
	}

	b = IBlk_blk_by_path(sections, args[1]);
	if(!b){
		utils_error("%s : block not found.\n", args[1]);
		return F8_PATH_NOT_FOUND;
	}
	if(b == sections){
		utils_error("Error %d, cannot umount root.\n", F8_ACCESS);
		return F8_ACCESS;
	}
	pb = parent_blk(b);
	if(pb != sections){
		if(pb->h.flags & BLK_F_READONLY){
			utils_error(
				"%s : error %d, parent block is readonly.\n", 
				args[1],
				F8_PERMISSION
				);
			return F8_ACCESS;
		}
	}
	cmd.retcode = gather_blk_reg_list(b, &hRegList);
	if(F8_FAILED(cmd.retcode)){
		return cmd.retcode;
	}

	__vcall__(s, put, (s, &b->uuid, sizeof(b->uuid)));

	cmd.cmd = F8_UMOUNT_BLOCK;
	cmd.isize = ms.pointer;

	__vcall__(
		proxy_adapter, 
		exec, 
		(proxy_adapter, &cmd, 1000)
		);	

	if(successful){
		delete_blk_reg_list(hRegList, true);
	}else{
		delete_blk_reg_list(hRegList, false);
	}

	g_bDirty = __true;

	return cmd.retcode;
}

int on_mv(char *v, void *k)
{
	return F8_SUCCESS;
}


