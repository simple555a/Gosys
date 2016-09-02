#include "precomp.h"
#pragma hdrstop

#include <map>

#define MAP_ENTRIES (32*1024)

struct reg_map_ent{
	void * owner;
	f8_uint magic;
};

struct reg_map{
	int total_items;
	int free_items;
	// the extra 8 entries make it easy for iteration through the array
	reg_map_ent bitmap[MAP_ENTRIES + 8];
};

reg_map g_regmap[KERN_NUM_SECTIONS];

/*
	for a newly loaded block, the class CRegRequirementList 
	will memorize its register usage map, and nothing is done 
	after the block is actually inserted into the current
	block hive, but if the block is not successfully inserted,
	this class is used to track and delete the previously
	allocated registers.
*/
class CRegRequirementList
{
public:
	CRegRequirementList()
	{		
		m_errcode = F8_SUCCESS;		
	}
	__bool Alloc(struct blk_pin_t * p);	
	__bool Alloc(struct blk_var_t * v);
	__bool Add(struct blk_var_t * v);
	void   Add(
		struct blk_pin_t * p, 
		struct mem_addr_t * addr, 
		int length
		);
	void	Commit(__bool bCharge);
	void	Free();
public:
	struct	reg_item{
		f8_uuid		blk_id;
		int			pin_id;
		void	    *owner;
		mem_addr_t	addr;
		int			length;
	};
	struct var_item{
		int			ref_count;
		f8_uuid		blk_id;
		blk_var_t	*var;
	};
	typedef std::list<reg_item>			reglist;
	typedef std::map<f8_uuid,var_item>	varlist;
	reglist		m_rlist;
	varlist		m_vlist;
	f8_status	m_errcode;
	bool		m_bGatherForDelete;
};

__bool is_valid_address(const struct mem_addr_t * addr)
{
	if(addr->section >= KERN_NUM_SECTIONS){
		return __false;
	}
	if(addr->addr >= g_regmap[addr->section].total_items){
		return __false;
	}
	return __true;
}

void set_address_owner(
	void * owner, 
	struct mem_addr_t * addr, 
	int length, 
	f8_uint magic, 
	__bool charge
	)
{
	reg_map * rm;
	reg_map_ent * e;
	int i;

	rm = &g_regmap[addr->section];
	e = rm->bitmap + addr->addr;
	for(i=0; i<length; i++, e++){
		e->owner = owner;
		e->magic = magic;
	}
	if(charge){
		rm->free_items -= length;
	}
}

void * get_address_owner(const struct mem_addr_t * addr, int length, f8_uint * magic)
{
	reg_map * rm;
	reg_map_ent * e;
	int i;

	if(!addr){
		return 0;
	}
	if(!_is_valid_section(addr->section)){
		return 0;
	}
	if(addr->addr >= ke_get_section_size(proxy_adapter->kernel, addr->section)){
		return 0;
	}

	rm = &g_regmap[addr->section];
	e = rm->bitmap + addr->addr;
	for(i=0; i<length; i++, e++){
		if(e->owner){
			if(magic){
				*magic = e->magic;
			}
			return e->owner;
		}
	}

	return 0;
}

/*
	Allocate a register slot from the specified register repository.
	
	A very naive first-fit algorithm is used, because in mose cases
	the allocation is requested with uniform sizes.
	
	Parameters:

	owner : IN
		specify the owner;

	addr : IN/OUT
		on input, the `section' field is used to select a
		register repository(0~KERN_NUM_SECTIONS-1), on output, the `addr' field 
		is filled if successful;

	length : IN
		specify how many bytes of data is requested.
		
	magic: IN
		allocate for variable or for pin
*/
__bool alloc_reg(
	void * owner, 
	struct mem_addr_t * addr, 
	int length, 
	f8_uint magic
	)
{
	int j;
	reg_map * rm;
	reg_map_ent *ent, *e2, *last;

	assert(owner);
	assert(addr->section < KERN_NUM_SECTIONS);
	assert(length <= 8);

	rm = &g_regmap[addr->section];
	ent = rm->bitmap;
	if(rm->free_items < length){
		return __false;
	}

	last = &rm->bitmap[rm->total_items - length];
	while(ent < last){
		if(ent->owner){
			ent++;
			continue;
		}
		for(j=0, e2=ent; j<length; j++, e2++){
			if(e2->owner){
				break;
			}
		}
		if(j == length){
			addr->addr = ent - rm->bitmap;
			set_address_owner(owner, addr, length, magic, __true);
			return __true;
		}
		ent = e2 + 1;
	}

	return __false;
}

void free_reg(struct mem_addr_t * addr, int length)
{
	int i;
	struct reg_map_ent * ent;
	struct reg_map * rm;
	void * owner;

	assert(addr->section < KERN_NUM_SECTIONS);

	rm = &g_regmap[addr->section];
	if(addr->addr > rm->total_items - length + 1){
		assert(0);
		return;
	}
	ent = rm->bitmap + addr->addr;

	owner = ent->owner;
	for(i=0; i<length; i++, ent++){
		if(ent->owner != owner){
			assert(0);
			// free non-contiguous region
			return;
		}
	}

	for(i=0, ent = rm->bitmap + addr->addr; i < length; i++, ent++){
		ent->owner = 0;
	}

	rm->free_items += length;
}

/*
	called upon new project
*/
void reset_reg_map()
{
	int i;
	
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		memset(g_regmap[i].bitmap, 0, sizeof(g_regmap[i].bitmap));
		g_regmap[i].total_items = ke_get_section_size(proxy_adapter->kernel, i);
		g_regmap[i].free_items = g_regmap[i].total_items;
	}
}

static __bool _e_var(struct blk_var_t * v, __uint context)
{
	CRegRequirementList * req;

	req = (CRegRequirementList*)context;
	req->Add(v);

	return __true;
}

/*
	finding out registers usage info for the block hive
*/
static __bool _gather_reg_usage(ITreeNode * nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	struct blk_pin_t *p, *rp;
	int length;
	CRegRequirementList * req;
	struct blk_var_t * var;
	char pName[F8_MAX_PATH];

	b = __dcast__(ITreeNode, IBlk, nd);

	req = (CRegRequirementList *)context;

	if(b->h.magic == CBLK_MAGIC && req->m_bGatherForDelete){
		enum_variables(b, _e_var, (__uint)req);
	}	
	
	for(p = __vcall__(b, first_pin, (b)); p; p = __vcall__(b, next_pin, (p))){
		/*
			if the pin is exported, then the variable reference or memory address
			assignment is counted on the exported pins
		*/
		if(p->ulink){
			continue;
		}
		rp = IBlk_real_pin(p);
		if(rp->_class != PIN_CLASS_DI && rp->_class != PIN_CLASS_DO){
			continue;
		}
		if(rp->binding != PIN_B_MEMORY){
			continue;
		}
		length = _type_size(rp->type);
		var = (blk_var_t*)query_var_by_pin(p);
		if(var){
			req->Add(var);
		}else{
			if(rp->_class == PIN_CLASS_DI){
				if(RtkIsTripleListEmpty(&rp->u1.link)){
					/*
					a data input pin can have a memory address only in
					two cases: 1) linked. or 2) connected with variable.
					*/
					IBlk_path_by_pin(p, pName, sizeof(pName));
					utils_error(
						"%s - DI pin address %%%d%05d incorrect.\n", 
						pName, 
						rp->u2.mem_addr.section, 
						rp->u2.mem_addr.addr
						);
					req->m_errcode = F8_INVALID_DATA;
				}
			}else{
				if(!is_address_valid(proxy_adapter->kernel, rp, &rp->u2.mem_addr)){
					req->m_errcode = F8_INVALID_DATA;
					IBlk_path_by_pin(p, pName, sizeof(pName));
					utils_error(
						"%s - DO pin address %%%d%05d incorrect.\n", 
						pName, 
						rp->u2.mem_addr.section, 
						rp->u2.mem_addr.addr
						);
				}else{
					req->Add(rp, &rp->u2.mem_addr, length);
				}
			}
		}
	}

	return __true;
}

/*
	reconstruct register usage map.
	called upon opening project and uploading project
*/
__bool build_reg_map()
{
	CRegRequirementList req;

	req.m_bGatherForDelete = false;
	_traverse(sections, _gather_reg_usage, (__uint)&req);
	if(F8_FAILED(req.m_errcode)){
		utils_error("Warning : _gather_reg_usage failed %d(%s).\n", req.m_errcode, _errmsg(req.m_errcode));
	}
	req.Commit(__true);
	
	return __true;
}

void CRegRequirementList::Add(
	struct blk_pin_t * p, 
	struct mem_addr_t * addr, 
	int length
	)
{
	reg_item	item;
	item.owner = p;
	item.addr = *addr;
	item.length = length;
	item.blk_id = p->blk->uuid;
	item.pin_id = __vcall__(p->blk, id_by_pin, (p));
	m_rlist.insert(m_rlist.begin(), item);
}

__bool CRegRequirementList::Add(struct blk_var_t * v)
{
	var_item	item;
	varlist::iterator vit;
	pair<varlist::iterator, bool> pi;

	vit = m_vlist.find(v->uuid);
	if(vit == m_vlist.end()){
		item.blk_id = v->blk->uuid;
		item.var = v;
		item.ref_count = 1;
		pi = m_vlist.insert(varlist::value_type(v->uuid, item));
		if(!pi.second){
			m_errcode = F8_LOW_MEMORY;
			char idName[F8_MAX_PATH];
			f8_uuid_to_string(&v->uuid, idName, sizeof(idName));
			utils_error("unable to add variable %s(%s)\n", v->name, idName);
		}
	}else{
		vit->second.ref_count++;
	}	

	return __true;
}

void CRegRequirementList::Commit(__bool bCharge)
{
	reglist::iterator it;
	varlist::iterator vit;
	reg_item	*item;
	var_item	*vitem;
	int i;

	for(i = m_rlist.size(), it = m_rlist.begin(); i; i--, it++){
		item = &(*it);
		/*
			fixup owner
		*/
		if(item->blk_id != BLK_ID_NONE){
			IBlk * blk;
			blk = ke_get_blk(proxy_adapter->kernel, &item->blk_id);
			assert(blk);
			item->owner = __vcall__(blk, pin_by_id, (blk, item->pin_id));
			assert(item->owner);
		}
		set_address_owner(
			item->owner, 
			&item->addr, 
			item->length, 
			PIN_MAGIC,
			bCharge
			);
	}

	for(i = m_vlist.size(), vit = m_vlist.begin(); i; i--, vit++){
		vitem = &(vit->second);
		vitem->var->blk = ke_get_blk(
			proxy_adapter->kernel, 
			&vitem->var->blk->uuid
			);
		vitem->var->ref_count += vitem->ref_count;
	}

	m_rlist.clear();
	m_vlist.clear();
}

__bool CRegRequirementList::Alloc(struct blk_pin_t * p)
{
	reg_item	item;

	p->u2.mem_addr.section = ke_section_by_type(p->type);
	item.length = _type_size(p->type);

	if(!alloc_reg((void*)p, &p->u2.mem_addr, item.length, PIN_MAGIC)){
		m_errcode = F8_LOW_REGISTERS;
		return __false;
	}

	item.blk_id = p->blk->uuid;
	item.pin_id = __vcall__(p->blk, id_by_pin, (p));
	item.owner = p;
	item.addr = p->u2.mem_addr;
	m_rlist.insert(m_rlist.end(), item);

	return __true;
}

__bool CRegRequirementList::Alloc(struct blk_var_t * v)
{
	var_item	item;

	if(m_vlist.find(v->uuid) != m_vlist.end()){
		m_errcode = F8_NAME_DUPLICATE;
		return __false;
	}

	m_errcode = define_variable(v, &v);
	if(F8_FAILED(m_errcode)){
		return __false;
	}

	item.blk_id = v->blk->uuid;
	item.var = v;
	item.ref_count = 0;

	m_vlist[v->uuid] = item;

	return __true;
}

void CRegRequirementList::Free()
{
	reglist::iterator it;
	varlist::iterator vit;
	reg_item	*item;
	var_item	*vitem;
	int i;
	
	for(i = m_rlist.size(), it = m_rlist.begin(); i; i--, it++){
		item = &(*it);
		free_reg(&item->addr, item->length);
	}

	for(i = m_vlist.size(), vit = m_vlist.begin(); i; i--, vit++){
		vitem = &(vit->second);
		if(vitem->var->blk == sections){
			//
			// reference to a global variable
			//
			vitem->var->ref_count -= vitem->ref_count;
		}else{
			vitem->var->blk_uuid = vitem->blk_id;
			delete_variable(vitem->var);
		}
	}

	m_rlist.clear();
	m_vlist.clear();
}

/*
	patch the memory address in a block hive, and remember
	all register allocated.
*/
static __bool _alloc_reg_proc(ITreeNode * nd, __bool firstVisit, __uint context)
{
	IBlk * b, *pb;
	struct blk_pin_t *p, *rp;
	CRegRequirementList * req;
	char path[1024];
	blk_var_t * v;

	req = (CRegRequirementList*)context;
	b = __dcast__(ITreeNode, IBlk, nd);
	
	pb = parent_blk(b);
		
	for(p = __vcall__(b, first_pin, (b)); 
		p; 
		p = __vcall__(b, next_pin, (p))
	){
		if(p->_class == PIN_CLASS_EI || p->_class == PIN_CLASS_EO){
			continue;
		}

		if(p->ulink){
			// postpone register allocation until the outmost 
			// parent block is being processed
			continue;
		}

		// ok, do allocation
		rp = IBlk_real_pin(p);
		if(rp->binding == PIN_B_CONST){
			if(rp->_class == PIN_CLASS_DI){
				memcpy(rp->dp, p->dp, _type_size(rp->type));
				continue;
			}else{
				req->m_errcode = F8_INVALID_DATA;
				return __false;
			}			
		}else if(rp->binding != PIN_B_MEMORY){
			req->m_errcode = F8_INVALID_DATA;
			return __false;
		}

		if(pb && p->u2.mem_addr.section == MEM_SECTION_VAR){
			//
			// the pin references a variable
			//
			if(v = (blk_var_t * )query_var_by_id(pb, p->u2.mem_addr.addr)){
				// 
				// note!! address is fixed on real pin
				//
				if(v->type != rp->type){
					req->m_errcode = F8_INVALID_DATA;
					return __false;
				}
				rp->u2.mem_addr = v->addr;
				req->Add(v);
			}else{
				IBlk_path_by_pin(p, path, sizeof(path));
				utils_error(
					"%s references undefined variable %d.\n", 
					path, 
					p->u2.mem_addr.addr
					);
				req->m_errcode = F8_UNRESOLVED_REFERENCES;
				return __false;
			}
		}else{
			//
			// the pin has an address on its own
			//
			if(!req->Alloc(rp)){
				return __false;
			}
		}
	}

	return __true;
}

static __bool _alloc_var_proc(ITreeNode * nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	ICBlk * cb;
	struct blk_var_t *v, var;
	int len, i;
	CRegRequirementList * req;

	req = (CRegRequirementList*)context;

	b = __dcast__(ITreeNode, IBlk, nd);
	if(b->h.magic != CBLK_MAGIC){
		return __true;
	}

	//
	//
	//	now unpack the variable definitions
	//
	//

	cb = __dcast__(IBlk, ICBlk, b);
	v = (struct blk_var_t*)get_res_buf(&b->h.uuid, &len, "variables");
	if(!v){
		return __true;
	}
	if(len % sizeof(struct blk_var_t)){
		req->m_errcode = F8_INVALID_DATA;
		return __false;
	}
	len /= sizeof(struct blk_var_t);
	for(i=0; i<len; i++, v++){
		var = *v;
		var.blk = b;
		var.comment = get_res_buf_v(&b->h.uuid, 0, "!v%d!comment", i);
		var.addr.section = MEM_SECTION_INVALID;
		create_f8_uuid(&var.uuid);
		if(!req->Alloc(&var)){
			return __false;
		}
	}

	return __true;
}

f8_status alloc_blk_reg_list(IBlk * b, HREG_LIST *h)
{
	CRegRequirementList * req;
	f8_status code;

	req = new CRegRequirementList;
	if(!req){
		return F8_LOW_MEMORY;
	}
	
	_traverse(b, _alloc_var_proc, (__uint)req);
	if(F8_FAILED(req->m_errcode)){
		*h = 0;
		code = req->m_errcode;
		delete req;
		return code;
	}

	_traverse(b, _alloc_reg_proc, (__uint)req);
	if(F8_FAILED(req->m_errcode)){
		*h = 0;
		code = req->m_errcode;
		delete req;
		return code;
	}
	
	*h = (HREG_LIST)req;

	return F8_SUCCESS;
}

void commit_reg_list(HREG_LIST h, __bool bCharge)
{
	CRegRequirementList * req;

	req = (CRegRequirementList*)h;
	req->Commit(bCharge);
}

/*
	find an existing block's register usage.
*/
f8_status gather_blk_reg_list(IBlk *b, HREG_LIST *h)
{
	CRegRequirementList * req;
	req = new CRegRequirementList;
	if(!req){
		return F8_LOW_MEMORY;
	}
	req->m_bGatherForDelete = true;
	_traverse(b, _gather_reg_usage, (__uint)req);
	*h = (HREG_LIST)req;
	return F8_SUCCESS;
}

void delete_blk_reg_list(HREG_LIST h, bool delete_and_free)
{
	CRegRequirementList * req;
	req = (CRegRequirementList*)h;
	if(delete_and_free){
		req->Free();
	}
	delete req;
}

__bool get_address_usage(
	const mem_addr_t * addr, 
	int length, 
	address_usage * usage
	)
{
	reg_map * rm;
	reg_map_ent * e;
	int i;

	if(!addr){
		return __false;
	}
	if(!_is_valid_section(addr->section)){
		return __false;
	}
	if(addr->addr >= ke_get_section_size(proxy_adapter->kernel, addr->section)){
		return __false;
	}

	memset(usage, 0, sizeof(*usage));
	
	rm = &g_regmap[addr->section];
	e = rm->bitmap + addr->addr;
	for(i=0; i<length; i++, e++){
		usage->owners[i] = e->owner;
		if(e->owner){
			usage->count_owners++;
		}
	}

	return __true;
}

static void dump_map(int idx)
{
	mem_addr_t addr;
	char path[F8_MAX_PATH];
	int i;
	if(!_is_valid_section(idx)){
		return;
	}
	reg_map * rm = &g_regmap[idx];
	reg_map_ent * e = rm->bitmap;
	void * owner = 0;
	const blk_var_t * var;

	addr.section = idx;

	for(i=0; i<rm->total_items; i++, e++){		
		if(!e->owner){
			continue;
		}
		addr.addr = i;
		if(e->owner != owner){
			owner = e->owner;			
			utils_trace(
				"%%%d%05d owned by ",
				idx,
				i
				);
			var = query_var_by_addr(&addr);
			if(var){
				IBlk_path_by_blk(var->blk, path, sizeof(path));
				utils_trace("{%s}.%s", path, var->name);
			}else{
				IBlk_path_by_pin((struct blk_pin_t*)owner, path, sizeof(path));
				utils_trace("%s", path);
			}
			utils_trace("\n");
		}
	}
}

int on_mmap(char *v, void *k)
{
	CArgs args;
	int i;

	parse_arg_ex(v, &args, FS);
	if(args.argc == 1){		
		for(i=0; i<KERN_NUM_SECTIONS; i++){
			utils_trace("Dump of memory section %d\n", i);
			dump_map(i);
		}
	}else{
		for(i=1; i<args.argc; i++){
			dump_map(atoi(args.argv[i]));
		}
	}

	return F8_SUCCESS;
}

/*
	expand or shrink section memory size

	usage

	memsize [section_id] [size]
*/
int on_memsize(char *v, void *k)
{
	int i;
	CArgs a;
	_shproc_prolog_();

	parse_arg_exA(v, &a, FS);
	if(a.argc == 1){
		for(i=0; i<KERN_NUM_SECTIONS; i++){
			utils_trace("memsize %d %d \n", i, g_regmap[i].total_items);
		}
	}else if(a.argc == 3){
		int newSize;
		i = atoi(a.argv[1]);
		newSize = atoi(a.argv[2]);
		if(newSize < g_regmap[i].total_items){
			// memory shrinking is not implemented
			utils_error("memory shrinking is dangerous.\n");
			// return F8_UNIMPLEMENTED;
		}
		if(newSize > MAP_ENTRIES){
			return F8_LOW_REGISTERS;
		}

		if(proxy_adapter->flags & ADAPTER_F_ONLINE){
			return F8_INVALID_OPERATION;
		}
		/*
			load memory configuration
		*/
		struct ctrller_config cfg;
		cmd.cmd = F8_GET_SYS_CONFIG;
		cmd.osize = sizeof(struct ctrller_config);
		cmd.obuffer = (char *)&cfg;
		__vcall__(
			proxy_adapter,
			exec,
			(proxy_adapter, &cmd, 5000)
			);
		if(!successful){
			return cmd.retcode;
		}

		cfg.x_mem_sizes[i]=newSize;
		cmd.cmd=F8_SET_SYS_CONFIG;
		cmd.isize=sizeof(cfg);
		cmd.osize=0;
		cmd.ibuffer=&cfg;
		__vcall__(
			proxy_adapter,
			exec,
			(proxy_adapter, &cmd, 5000)
			);
		if(!successful){
			return cmd.retcode;
		}
			
		g_regmap[i].free_items += newSize - g_regmap[i].total_items;
		g_regmap[i].total_items = newSize;
	}
	return F8_SUCCESS;
}

int on_memusage(char *v, void *k)
{
	int i;
	reg_map * m;
	
	for(i=0, m=g_regmap; i<KERN_NUM_SECTIONS; i++, m++){
		utils_trace(
			"section %d total=%d used=%d free=%d\n", 
			i, 
			m->total_items, 
			m->total_items-m->free_items, 
			m->free_items
			);
	}
	return F8_SUCCESS;
}

KPROXY_API void kproxy_get_mem_usage(int sizes[KERN_NUM_SECTIONS][2])
{
	int i;
	reg_map * m;
	
	for(i=0, m=g_regmap; i<KERN_NUM_SECTIONS; i++, m++){
		sizes[i][0]=m->total_items;
		sizes[i][1]=m->total_items-m->free_items;
	}
}

