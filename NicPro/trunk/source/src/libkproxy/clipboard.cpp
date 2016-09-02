/*
	Clipboard manager
	by J.Hunter, 2004/2/23
	by Zander.Sun, 2013-1-27
*/
#pragma warning(disable:4018)
#include "precomp.h"

static f8_u16 s_F8Format;

class CSelectedObjects{
public:

	CSelectedObjects();
	virtual ~CSelectedObjects();

	f8_status copy(f8_uuid source, f8_u16 *magics, void **pointers, int count);
	f8_status paste(IBlk * target);

	typedef list<paste_obj_t>	paste_obj_ts;
	paste_obj_ts m_objects;
};

/*
	save the instance resource into the stream
*/
static __bool _save_inst_res_proc(ITreeNode * nd, __bool firstVisit, f8_uint _s)
{
	IBlk * blk;
	ICBlk * cblk;
	struct cblk_link_t * lk;
	struct blk_pin_t  * p;
	struct cblk_pin_t * cp;
	IF8Stream * s;

	s = (IF8Stream*)_s;

	blk = __dcast__(ITreeNode, IBlk, nd);
	
	save_res_stream(&blk->uuid, s);

	if(blk->h.magic == CBLK_MAGIC){
		cblk = __dcast__(IBlk, ICBlk, (blk));
		
		lk = ICBlk_first_link(cblk);
		while(lk){
			save_res_stream(&lk->uuid, s);
			lk = ICBlk_next_link(lk);
		}

		p = __vcall__(blk, first_pin, (blk));
		while(p){
			cp = CONTAINING_RECORD(p, struct cblk_pin_t, p);
			save_res_stream(&cp->uuid, s);
			p = __vcall__(blk, next_pin, (p));
		}
	}
	
	return __true;
}

/*
	save variable definitions in instance resource
	of target block.
*/
static void _save_var_def(IBlk * blk, IBlk * target)
{
	// pack variables
	f8_uint i;
	f8_uint nv = get_var_count(blk);
	if(!nv){
		return;
	}
	blk_var_t vvv[32], *vv;
	if(nv < 32){
		vv = vvv;
	}else{
		vv = new blk_var_t[nv];
	}
	query_var_list(blk, vv, &nv);
	set_res_buf(
		&target->h.uuid, 
		vv, 
		sizeof(blk_var_t)*nv, 
		"variables"
		);
	for(i=0; i<nv; i++){
		set_res_buf_v(
			&target->h.uuid, 
			vv[i].name, 
			-1, 
			"!v%d!name", 
			i
			);
		set_res_buf_v(
			&target->h.uuid, 
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

static void _save_var_ref(IBlk * blk, IBlk * target)
{
	struct blk_pin_t * p;
	const struct blk_var_t * v;
	char vname[F8_MAX_PATH];
	IBlk * rootBlk;
	int count;

	rootBlk = ke_get_blk(proxy_adapter->kernel, 0);

	count = 0;
	for(p = __vcall__(blk, first_pin, (blk)); p; p = __vcall__(blk, next_pin, (p))){
		/*
			if the pin is exported, then the variable reference or memory address
			assignment is counted on the exported pins
		*/
		if(p->ulink){
			continue;
		}
		/*
			2006-6-23
			DO pins are not copied.
		*/
		if(p->_class == PIN_CLASS_DO)
			continue;
		v = query_var_by_pin(p);
		if(!v){
			continue;
		}
		/*
			note a variable reference is targeted either to a global
			variable, or to a local variable of the parent block.
		*/
		if(!v->blk || v->blk == rootBlk){
			sprintf(vname, "/.%s", v->name);
		}else{
			assert(v->blk == parent_blk(blk));
			sprintf(vname, "%s", v->name);
		}
		/*
			write the variable reference to instance resource
		*/
		set_res_buf_v(
			&target->uuid, 
			IBlk_pin_name(p),
			-1,
			"VRef-%d-S",
			count
			);
		set_res_buf_v(
			&target->uuid, 
			vname,
			-1,
			"VRef-%d-T",
			count
			);
		count++;
	}
	set_res_int(&target->uuid, "VRefCount", count);
#if 1
	{
		char idName[32];
		f8_uuid_to_string(&target->uuid, idName, sizeof(idName));
		utils_debug("%d variables defined on %s\n", count, idName);
	}
#endif
}

static __bool _pack_variable_defs(ITreeNode * nd, __bool firstVisit, f8_uint _s)
{
	IBlk * blk;
	IBlk * target;

	target = (IBlk*)_s;
	blk = __dcast__(ITreeNode, IBlk, nd);
	if(query_resource(&target->uuid) < 0){
		create_resource(&target->uuid);
	}
	_save_var_def(blk, target);
	return __true;
}

static __bool _pack_variable_refs(ITreeNode * nd, __bool firstVisit, f8_uint _s)
{
	IBlk * blk;
	IBlk * target;

	blk = __dcast__(ITreeNode, IBlk, nd);
	target = (IBlk*)_s;
	if(query_resource(&target->uuid) < 0){
		create_resource(&target->uuid);
	}
	_save_var_ref(blk, target);
	return __true;
}

static __bool _unpack_variable_refs(ITreeNode * nd, __bool firstVisit, f8_uint _s)
{
	IBlk * blk;
	int i;
	int count;
	char * pname, * vname;
	struct blk_pin_t * p;
	const struct blk_var_t * v;
	char cmd[2048];
	char ppath[F8_MAX_PATH];
	char vpath[F8_MAX_PATH];

	blk = __dcast__(ITreeNode, IBlk, nd);
	count = get_res_int(&blk->uuid, "VRefCount", 0);
#if 1
	{
		char idName[32];
		f8_uuid_to_string(&blk->uuid, idName, sizeof(idName));
		utils_debug("%d variables defined on %s\n", count, idName);
	}
#endif

	if(count){
		for(i=0; i<count; i++){
			pname = get_res_buf_v(&blk->uuid, 0, "VRef-%d-S", i);
			vname = get_res_buf_v(&blk->uuid, 0, "VRef-%d-T", i);
			if(!pname || !vname){
				utils_error(
					"Invalid variable reference declaration in stream, "
					"missing source or target.\n"
					);
				continue;
			}
			
			/* ok, now associate the source pin and target variable */
			p = IBlk_pin_by_name(blk, pname);
			IBlk_path_by_pin(p, ppath, sizeof(ppath));
			if(!p){
				utils_error(
					"Invalid variable reference declaration in stream, "
					"missing source.\n"
					);
				continue;
			}

			if(*vname == '/'){
				v = query_var(0, vname + 2);
				strncpy(vpath, vname, sizeof(vpath));
			}else{
				IBlk_path_by_blk(parent_blk(blk), vpath, sizeof(vpath));
				strncat(vpath, ".", sizeof(vpath));
				strncat(vpath, vname, sizeof(vpath));
				v = query_var(parent_blk(blk), vname);
			}
			if(!v){
				utils_error(
					"Invalid variable reference declaration in stream, "
					"missing target.\n"
					);
				continue;
			}
			_snprintf(cmd, sizeof(cmd), "bind %s %s\n", ppath, vpath);
			if(F8_FAILED(kproxy_shell(cmd))){
				return __false;
			}
		}
	}

	return __true;
}

static void _unique_name(IBlk * blk, char nm[F8_MAX_PATH])
{
	int n;
	IBlk * sb;
	n = blk->h.u1.n_subblocks;
	sprintf(nm, "B%d", n);
	sb = IBlk_subblk_by_name(blk, nm);
	while(sb){
		n++;
		sprintf(nm, "B%d", n);
		sb = IBlk_subblk_by_name(blk, nm);
	}
}

/*
	extract a block from the stream, then mount it under
	the block specified by target
*/
static int _paste_blk(IBlk * target, IBlk * b, IBlk **nblk)
{
	char nm[F8_MAX_PATH];
	int	ret;

	_unique_name(target, nm);

	_instanciate_blk(b);

#if 0
	printf("Before _mount_blk()\n---------------------------\n");
	kproxy_shell("vres");
#endif
	ret = _mount_blk(target, b, nm, false, NULL);
#if 0
	printf("After _mount_blk()\n---------------------------\n");
	kproxy_shell("vres");
#endif
	if(F8_FAILED(ret)){
		delete_blk(b);
		return ret;
	}

	b = IBlk_subblk_by_name(target, nm);
	assert(b);
	_traverse(b, _unpack_variable_refs, 0);

	if(nblk){
		*nblk = b;
	}

	return F8_SUCCESS;
}

f8_status CSelectedObjects::paste(IBlk * target)
{
	HANDLE hMem;
	IMemStream ms;
	IF8Stream *s;
	f8_status ret;
	IBlk * b;
	paste_obj_t obj;
	struct cblk_link_t slk, * rlk;
	int nBlocks;
	IBlk * b1, * b2;
	struct blk_pin_t * p1, * p2;

	if(!IsClipboardFormatAvailable(s_F8Format)){
		return F8_INVALID_DATA;
	}
	if(!OpenClipboard(NULL)){
		return F8_INVALID_DATA;
	}
	hMem = GetClipboardData(s_F8Format);
	if(!hMem){
		CloseClipboard();
	}

	__create__(IMemStream, &ms);
	ms.buffer = (char *)GlobalLock(hMem);
	ms.length = GlobalSize(hMem);
	ms.pointer = 0;
	s = __ucast__(IMemStream, IF8Stream, &ms);
	
	while(!__vcall__(s, eos, (s))){
		b = IBlk_from_stream(s);
		if(!b){
			break;
		}
		
		load_res_stream(s);
		probe_blk_resources(b);

		ret = _paste_blk(target, b, (IBlk**)&(obj.obj));
		delete_blk(b);
		if(F8_FAILED(ret)){
			goto __failed;
		}

		obj.magic = BLK_MAGIC;
		m_objects.insert(m_objects.end(), obj);
	}
	
	nBlocks = m_objects.size();

	while(!__vcall__(s, eos, (s))){
		if(!__vcall__(s, get, (s, &slk, sizeof(slk)))){
			break;
		}
		if(slk.s.id.bid >= nBlocks || slk.t.id.bid >= nBlocks){
			/*
			*/
			utils_error(
			"Error in stream, link (%d.%d->%d.%d) cannot be pasted\n",
			slk.s.id.bid,
			slk.s.id.pid,
			slk.t.id.bid,
			slk.t.id.pid
			);
			continue;
		}
		b1 = (IBlk*)kproxy_get_pasted_object(this, slk.s.id.bid)->obj;
		b2 = (IBlk*)kproxy_get_pasted_object(this, slk.t.id.bid)->obj;
		assert(b1 && b2);
		p1 = __vcall__(b1, pin_by_id, (b1, slk.s.id.pid));
		p2 = __vcall__(b2, pin_by_id, (b2, slk.t.id.pid));
		if(!p1 || !p2){
			utils_error(
				"Error in stream, link (%d.%d->%d.%d) cannot be pasted\n",
				slk.s.id.bid,
				slk.s.id.pid,
				slk.t.id.bid,
				slk.t.id.pid
				);
			continue;
		}
		_link_pins(p1, p2, &rlk);
		obj.obj = rlk;
		obj.magic = LNK_MAGIC;
		if(obj.obj){
			m_objects.insert(m_objects.end(), obj);
			/*
				note although load_res_stream_as, like
				load_res_stream, won't perform reference
				counting, the resource has already been
				referenced in call to _link_pins(), so there
				is no need for another reference.
			*/
			if(!load_res_stream_as(s, &rlk->uuid)){
				utils_error(
					"Error loading resource for link (%d.%d->%d.%d)\n",
					slk.s.id.bid,
					slk.s.id.pid,
					slk.t.id.bid,
					slk.t.id.pid
					);
			}
		}else{
			utils_error(
				"Error pasting link (%d.%d->%d.%d).\n",
				slk.s.id.bid,
				slk.s.id.pid,
				slk.t.id.bid,
				slk.t.id.pid
				);
		}
	}

	GlobalUnlock(hMem);
	CloseClipboard();

	return F8_SUCCESS;
__failed:
	GlobalUnlock(hMem);
	CloseClipboard();
	return ret;
}

/*
	copy(f8_uuid source, f8_u16 *magics, void **pointers, int count)
	
	source : the block at whose level the copy is taken
	magics : type of the pointers
	pointers : the objects to be copied
	count : number of objects to be copied
*/
f8_status CSelectedObjects::copy(f8_uuid source, f8_u16 *magics, void **pointers, int count)
{
	int i;
	ITempStream ts;
	IF8Stream *s;
	f8_status ret;
	IBlk *b;
	struct cblk_link_t * lk;
	struct cblk_link_t slk;
	int j;
	f8_uint length;
	HANDLE hMem;
	void * pMem;
	struct copy_paste_info *cpi;
	int cpiSize;

	if(!OpenClipboard(NULL)){
		return F8_INVALID_DATA;
	}

	cpiSize = (count-1)*sizeof(paste_obj_t) + sizeof(copy_paste_info);
	cpi = (struct copy_paste_info*)malloc(
		cpiSize
		);
	if(!cpi){
		CloseClipboard();
		return F8_LOW_MEMORY;
	}
	cpi->source = source;
	cpi->objCount = count;

	EmptyClipboard();
  
	__create__(ITempStream, &ts);
	s = __ucast__(ITempStream, IF8Stream, &ts);

	// __vcall__(s, put, (s, cpi, cpiSize));

	// firstly we put all the blocks into a stream

	for(i=0; i<count; i++){
		if(magics[i] == BLK_MAGIC){
			ret = _duplicate_blk((IBlk*)pointers[i], &b);

			if(F8_FAILED(ret)){
				goto __failed;
			}
			if(!__vcall__(b, save, (b, s))){
				ret = F8_INVALID_DATA;
				goto __failed;
			}

			// for each block object, we put the variable definition
			// and references into the stream as well as the instance
			// resources
			_traverse((IBlk*)pointers[i], _pack_variable_defs, b);
			_traverse((IBlk*)pointers[i], _pack_variable_refs, b);
			_traverse(b, _save_inst_res_proc, s);

			delete_blk(b);
		}
	}

	/*
		then we process the links copied.
	*/
	for(i=0; i<count; i++){
		if(magics[i] == LNK_MAGIC){
			lk = (struct cblk_link_t*)pointers[i];
			slk.s.id.bid = 0;
			/* lookup the pointers array, get the relative offset
			of the source/target block in this array
			*/
			for(j=0; j<count; j++){
				if(pointers[j] == (void*)lk->s.pin->blk){
					break;
				}
				if(magics[j] == BLK_MAGIC){
					slk.s.id.bid++;
				}
			}
			if(slk.s.id.bid >= count){
				utils_error("Selected link cannot be copied.\n");
				continue;
			}
			slk.s.id.pid = __vcall__(
				lk->s.pin->blk, 
				id_by_pin, 
				(lk->s.pin)
				);
			slk.t.id.bid = 0;
			for(j=0; j<count; j++){
				if(pointers[j] == (void*)lk->t.pin->blk){
					break;
				}
				if(magics[j] == BLK_MAGIC){
					slk.t.id.bid++;
				}
			}
			if(slk.t.id.bid >= count){
				utils_error("Selected link cannot be copied.\n");
				continue;
			}
			slk.t.id.pid = __vcall__(
				lk->t.pin->blk, 
				id_by_pin, 
				(lk->t.pin)
				);

			DEBUG_PRINTF((
				"copying link %d.%d -> %d.%d\n", 
				slk.s.id.bid,
				slk.s.id.pid,
				slk.t.id.bid,
				slk.t.id.pid
				));

			__vcall__(s, put, (s, &slk, sizeof(slk)));
			save_res_stream(&lk->uuid, s);
		}
	}

	length = __vcall__(s, tell, (s));
	hMem = GlobalAlloc(GMEM_MOVEABLE, length);
	if(hMem){
		pMem = GlobalLock(hMem);
		__vcall__(s, seek, (s, 0, SEEK_SET));
		__vcall__(s, get, (s, pMem, length));
		SetClipboardData(s_F8Format, hMem);
		GlobalUnlock(hMem);
	}
	ret = F8_SUCCESS;

__failed:
	if(cpi){
		free(cpi);
	}
	CloseClipboard();
	__destroy__(&ts);
	return ret;
}

CSelectedObjects::CSelectedObjects()
{
}

CSelectedObjects::~CSelectedObjects()
{
}

KPROXY_API int kproxy_copy(f8_uuid *source, f8_u16 *magic, void **src, int count)
{
	CSelectedObjects foo;
	return foo.copy(*source, magic, src, count);
}

KPROXY_API void * kproxy_paste(IBlk * target)
{
	CSelectedObjects * foo;
	foo = new CSelectedObjects;
	if(!foo){
		return 0;
	}
	foo->paste(target);
	autoSave(0,0);
	return foo;
}

KPROXY_API void kproxy_delete_paste(void * h)
{
	CSelectedObjects * o = (CSelectedObjects*)h;
	delete o;
}

KPROXY_API paste_obj_t * kproxy_get_pasted_object(void * h, int index)
{
	CSelectedObjects * so;
	CSelectedObjects::paste_obj_ts::iterator it;

	so = (CSelectedObjects*)h;
	if(index >= so->m_objects.size()){
		return 0;
	}
	it = so->m_objects.begin();
	for(; index; index--, it++);

	return &(*it);
}

KPROXY_API f8_u16 kproxy_get_clipboard_format()
{
	return s_F8Format;
}

#if 0

/*
	cboard clear
	cboard copy <block path>
	cboard addlink <link terminal pin path>
	cboard paste <target path>
*/
int on_cboard(char * v, void * k)
{
	CArgs args;
	const struct blk_var_t * var;
	struct mem_addr_t addr;

	parse_arg_ex(v, &args, FS);

	if(args.argc < 2){
		return F8_SYNTAX;
	}

	if(!s_F8Format.handle){
		return F8_LOW_MEMORY;
	}

	if(!strcmp(args.argv[1], "clear")){
		__vcall__(
			&s_ClipboardStream, 
			seek, 
			(&s_ClipboardStream, 0, SEEK_SET)
			);
		return F8_SUCCESS;
	}

	if(!strcmp(args.argv[1], "clear")){
	}
}

#endif

void init_clipboard()
{
	s_F8Format = RegisterClipboardFormat(
		"F8 Conductor Configuration Objects"
		);
}

