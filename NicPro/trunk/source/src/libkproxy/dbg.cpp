/*
	This module contains some debug procedures.
	by J.Hunter, 2003/9/18
*/
#pragma warning(disable:4018)
#include "precomp.h"

static void _print_dpin(int type, dyn_pin_t *dp)
{
	switch(type){
	case PIN_T_CHAR:
		utils_trace("%c, char", dp->u.d.i8);
		break;
	case PIN_T_SHORT:
		utils_trace("%d, short", dp->u.d.i16);
		break;
	case PIN_T_INTEGER:
		utils_trace("%d, integer", dp->u.d.i32);
		break;
	case PIN_T_BYTE:
		utils_trace("%d, byte", dp->u.d.ui8);
		break;
	case PIN_T_WORD:
		utils_trace("%d, word", dp->u.d.ui16);
		break;
	case PIN_T_DWORD:
		utils_trace("%d, dword", dp->u.d.ui32);
		break;
	case PIN_T_BOOL:
		utils_trace("%s, bool", dp->u.d.ui8? "true" : "false");
		break;
	case PIN_T_TIMER:
		utils_trace(
			"%d, timer", 
			dp->u.e.interval
			);
		break;
	case PIN_T_FLOAT:
		utils_trace("%f, float", dp->u.d.flt);
		break;
	case PIN_T_DOUBLE:
	case PIN_T_DATE:
		utils_trace("%f, double", dp->u.d.dbl);
		break;
	default:
		utils_trace("N.A.");
		break;
	}
}

static void _print_pin(char * name, struct blk_pin_t * pin)
{
	struct dyn_pin_t *dp;
	
	utils_trace("\t%s = ", name);
	if(!pin){
		utils_trace("NO SUCH PIN");
		return;
	}
	pin = IBlk_real_pin(pin);
		
	if(pin->_class != PIN_CLASS_DI && pin->_class != PIN_CLASS_DO){
		utils_trace("N.A.");
		return;
	}

	/*
	if(pin->_class == PIN_CLASS_DI){
		if(!IsTripleListEmpty(&pin->link)){
			pin = SAFE_CONTAINING_RECORD(pin->link.Head, blk_pin_t, link);
		}
	}
	*/

	dp = pin->dp;

	if(!dp){
		utils_trace(" N.A.");
		return;
	}

	_print_dpin(pin->type, dp);

	if(pin->_class == PIN_CLASS_DI || pin->_class == PIN_CLASS_DO){
		if(pin->binding != PIN_B_CONST){
			utils_trace(
				",@%%%d%05d", 
				pin->u2.mem_addr.section, 
				pin->u2.mem_addr.addr
				);
		}
	}
}

static void _dump_blk(IBlk *b, const char * bname)
{
	int i;
	struct blk_pin_t *p;
	const char *name;

	utils_trace("pins of block %s\n", bname);
	for(i=0; i<b->h.n_pins; i++){
		p = __vcall__(b, pin_by_id, (b, i));
		name = IBlk_pin_name(p);
		_print_pin((char*)name, p);
		utils_trace("\n");
	}
}

int on_print(char * path, void *k)
{
	struct blk_pin_t * pin;
	IBlk *blk;
	CArgs a;
	int i;

	parse_arg_ex(path, &a, " ,");
	if(a.argc < 2){
		utils_error("Usage : p <pin|blk> [,<pin|blk>...]\n");
		return -1;
	}

	for(i=1; i<a.argc; i++){
		blk = IBlk_blk_by_path(sections, a.argv[i]);
		if(blk){
			_dump_blk(blk, a.argv[i]);
		}else{
			pin = IBlk_pin_by_path(sections, a.argv[i]);
			_print_pin(a.argv[i], pin);
			utils_trace("\n");
		}
	}
	
	return 0;
}

int on_pp(char * v, void *k)
{
	CArgsEx args;
	struct blk_pin_t * p;
	int i;

	args.parse(v, FS);
	if(args.argc < 2){
		return 0;
	}
	p = IBlk_pin_by_path(sections, args.argv[1]);
	for(i=0; i<40; i++){
		_print_pin(args.argv[1], p);
		utils_trace("\r");
		rtk_sleep(500);
	}
	return 0;
}

int on_disasm(char *v, void *k)
{
#ifdef _WIN32
	CArgsEx a;
	IBlk * b;

	a.parse(v, FS);
	if(a.argc < 2){
		utils_error("Usage : disasm <block path>\n");
		return F8_SYNTAX;
	}
	b = IBlk_blk_by_path(sections, a.argv[1]);
	if(!b){
		utils_error("Error : cannot open %s\n", a.argv[1]);
		return F8_PATH_NOT_FOUND;
	}
	blk_disassembly(b);
	return F8_SUCCESS;
#else
	return F8_UNIMPLEMENTED;
#endif
}

int on_error(char *v, void *k)
{
	int err;
	CArgsEx a;
	int i;

	a.parse(v, FS);
	for(i=1; i<a.argc; i++){
		err = atoi(a.argv[i]);
		utils_trace("%d=%s\n", err, _errmsg(err));
	}
	return F8_SUCCESS;
}


/*
	list contents under specified path, if no pathed is specified, 
	'/' is assumed.
*/
int on_ls(char * v, void * k)
{
	CArgsEx a;
	IBlk * blk;
	char uname[64], uname1[64];
	__bool bLongFormat = __false;
	char * path = 0;

	BEGIN_ARGS
		ARG("-l", bLongFormat, "tbool", "long listing")
		ARG("-p:", path, "char *", "path to list")
	END_ARGS
	
	a.parse(v, " \t\n");
	if(a.argc < 2){
		utils_error("Usage : ls <path>\n");
		return F8_SYNTAX;
	}
	
	if(process_args_1(a.argc, a.argv) < 0){
		print_usage_1();
		return F8_SYNTAX;
	}

	if(!path){
		print_usage_1();
		return F8_SYNTAX;
	}

	blk = IBlk_blk_by_path(sections, path);
	if(blk){
		utils_trace(
			"%-20s %-20s TYPE %-16s %s\n", 
			"CLASS_UUID",			
			"UUID",			
			"CLASS_NAME",
			"NAME"			
			);

		IBlk * sb;
		sb = first_subblk(blk);
		while(sb){
			const char *name, *_typename;
			f8_uuid_to_string(&sb->uuid, uname, sizeof(uname));
			f8_uuid_to_string(&sb->h.uuid, uname1, sizeof(uname1));
			name = IBlk_name(sb);
			_typename = IBlk_class_name(sb);
			if(bLongFormat){
				utils_trace(
					"%-20s %-20s %c%c   %-16s %s\n",
					uname1,
					uname,
					sb->h.magic & 0xff, 
					sb->h.flags & BLK_F_READONLY? 'R' : 'W',
					_typename,
					name
					);
			}else{
				utils_trace("%s\n", name);
			}
			sb = next_blk(sb);
		}
	}else{
		utils_error("Path not found.\n");
	}

	return 0;
}

char * _errmsg(int err)
{
	if(!err){
		return "F8_SUCCESS";
	}
#define _err_(n) case n: return #n;
	if(event_module(err) == MODNUM_NONE){  
		switch(event_code(err)){
		_err_(F8_SUCCESS);
		_err_(F8_PATH_NOT_FOUND);
		_err_(F8_NAME_DUPLICATE);
		_err_(F8_LOW_MEMORY);
		_err_(F8_INVALID_DATA);
		_err_(F8_PERMISSION);
		_err_(F8_INVALID_OPERATION);
		_err_(F8_TOO_MANY_TIMERS);
		_err_(F8_ACCESS);
		_err_(F8_CONFIG);
		_err_(F8_BUSY);
		_err_(F8_TIMEOUT);
		_err_(F8_SYNTAX);
		_err_(F8_FILE_NOT_FOUND);
		_err_(F8_ADAPTER_OFFLINE);
		_err_(F8_BUFFER_TOO_SMALL);
		_err_(F8_CONNECTION_LOST);
		_err_(F8_NO_SCHEDULER);
		_err_(F8_CONNECTION_REFUSED);
		_err_(F8_VERSION_MISMATCH);
		_err_(F8_NOT_A_BBLK);
		_err_(F8_NOT_A_CBLK);
		_err_(F8_ADD_BLOCK_FAILED);
		_err_(F8_OBJECT_NOT_FOUND);
		_err_(F8_INTERNAL_ERROR);
		_err_(F8_ALREADY_ONLINE);
		_err_(F8_ALREADY_OFFLINE);
		_err_(F8_RES_NOT_FOUND);
		_err_(F8_INVALID_NAME);
		_err_(F8_PIN_INCOMPATIBLE);
		_err_(F8_INVALID_ADDRESS);
		_err_(F8_UNIMPLEMENTED);
		_err_(F8_TYPE_MISMATCH);
		_err_(F8_SIZE_MISMATCH);
		_err_(F8_BOUND);
		_err_(F8_UNBOUND);
		_err_(F8_GENERAL_CONFIG_ERR);
		_err_(F8_ADDRESS_OCCUPIED);
		_err_(F8_INVALID_AUTOSCAN);
		_err_(F8_X_PROG_NOT_FOUND);
		_err_(F8_LOW_REGISTERS);
		_err_(F8_UNKNOWN_COMMAND);
		_err_(F8_ALREADY_STARTED);
		_err_(F8_ALREADY_STOPPED);
		_err_(F8_VAR_ALREADY_DEFINED);
		_err_(F8_UNRESOLVED_REFERENCES);
		_err_(F8_PENDING_REFERENCES);
		_err_(F8_PIN_NOT_EXPORTABLE);
		_err_(F8_PIN_NOT_BINDABLE);
		_err_(F8_NO_SOCKET);
		_err_(F8_NO_CONNECTION);
		_err_(F8_DATABASE_LOCKED);
		_err_(F8_OUT_OF_SYNCHRONIZATION);
		_err_(F8_COLLISION);
		_err_(F8_INVALID_COMPRESSED_DATA);
		}
	}else if(event_module(err) == MODNUM_KERNEL){
		switch(event_code(err)){
		_err_(F8_INVALID_TIMER_ID);
		_err_(F8_NO_MEMORY);
		_err_(F8_NO_TIMER);
		_err_(F8_INCONSISTENCY);
		_err_(F8_FIRE_NON_EVENT);
		_err_(F8_TIMER_TYPE);

		_err_(F8_PROG_RESUMED);
		_err_(F8_PROG_DOWNLOADED);
		_err_(F8_EXEC_STARTED);
		_err_(F8_EXEC_STOPPED);
		_err_(F8_EXEC_DOWNLOADED);				
		_err_(F8_PRIMARY_FAILURE);
		_err_(F8_MODE_SWITCH);
		_err_(F8_IOBUS_COLLISION);
		}
	}

	return "Unknown";
}

int on_dump(char *v, void *k)
{
	static int sec;
	static int addr;
	int		ss,aa;
	CArgs	args;
	f8_u8	* p;
	char format='\1'; /* invalid format */
	struct blk_var_t * var;
	int len=1;
	int i;

	__uint sizes[KERN_NUM_SECTIONS];

	ke_get_mem_sizes(proxy_adapter->kernel, sizes);

	parse_arg_ex(v, &args, FS);
	if(args.argc > 1){
		var=query_var(0,args.argv[1]);
		if(!var){
			ss = args.argv[1][0] - '0';
			aa = atoi(args.argv[1] + 1);
			if(!_is_valid_section(ss)) 
				return F8_INVALID_ADDRESS;
			if(aa < 0 || aa > sizes[ss])
				return F8_INVALID_ADDRESS;
			sec = ss;
			addr = aa;
		}else{
			sec=var->addr.section;
			addr=var->addr.addr;
		}
		if(args.argc > 2){
		}
	}else{
		addr += 16;
		if(addr > sizes[sec]){
			addr -= sizes[sec];
		}
	}

	if(args.argc > 2){
		for(i=2;i<args.argc;i++){
			if(tolower(*args.argv[i]) == 'l'){
				len=atoi(args.argv[i]+1);
			}else{
				format=args.argv[i][0];
			}
		}
	}

	if(len <= 0)
		len=1;
	if(!strchr("wdFb",format))
		switch(sec){
		case MEM_SECTION_WORD:
			format='w';
			break;
		case MEM_SECTION_R4:
			format='d';
			break;
		case MEM_SECTION_R8:
			format='F';
			break;
		default:
			format='b';
			break;
		}

	for(i=0,p=ke_get_section(proxy_adapter->kernel, sec)+addr;i<len;i++){
		utils_trace("%d%05d : ",sec,addr+i*16);
		for(ss = 0;ss< 16;){
			switch(format){
			case 'b':
				utils_trace("%02x ", *p);
				ss++;
				p++;
				break;
			case 'w':
				utils_trace("%04x ", *((f8_u16*)p));
				ss+=2;
				p+=2;
				break;
			case 'd':
				utils_trace("%08x ", *((f8_u32*)p));
				ss+=4;
				p+=4;
				break;
			case 'f':
				utils_trace("%f ", *((f8_r4*)p));
				ss+=4;
				p+=4;
				break;
			case 'F':
				utils_trace("%f[%08x-%08x] ", *((f8_r8*)p),*((f8_u32*)(p+4)),*((f8_u32*)p));
				ss+=8;
				p+=8;
				break;
			}
		}
		utils_trace("\n");
	}
	
	utils_trace("\n");
	
	return F8_SUCCESS;
}

static __bool _ei_proc(
	const char * key, 
	const void * buf, 
	__int len, 
	__int context
	)
{
	utils_trace("\t%s = (%d bytes)", key, len);
	if(buf){
		if(((char*)buf)[len - 1] == 0){
			utils_trace("%s", buf);
		}
	}
	utils_trace("\n");
	return __true;
}

static __bool _er_proc(
	const f8_uuid * id, 
	__int context
	)
{
	char buf[64];
	IBlk * blk;
	struct blk_ent_t ent;

	f8_uuid_to_string(id, buf, sizeof(buf));
	utils_trace("%s", buf);
	if(blk = ke_get_blk(proxy_adapter->kernel, id)){
		char path[F8_MAX_PATH];
		IBlk_path_by_blk(blk, path, sizeof(path));
		utils_trace("(Instance of %s)\n", path);
	}else if(blklib_query(id, &ent)){
		utils_trace("(Class of %s)\n", ent.name);
	}else if((const f8_uuid)*id == sections->h.uuid){
		utils_trace("(Class of /)\n");
	}else{
		utils_trace("\n");
	}
	utils_trace("\tRefCount=%d\n", query_resource(id));
	enum_res_items(id, _ei_proc, 0);
	return __true;
}

int on_dump_res(char *v, void *k)
{
	CArgs args;
	int i;
	IBlk *b;
	char uuidBuf[64];

	parse_arg_ex(v, &args, FS);
	if(args.argc < 2){
		i = enum_resources(_er_proc, 0);
		utils_trace("%d resources listed.\n", i);
	}
	for(i=1; i<args.argc; i++){
		b = IBlk_blk_by_path(sections, args.argv[i]);
		if(!b){
			utils_error("%s : not found.\n", args.argv[i]);
			continue;
		}
		utils_trace(" =========== Class resource of block %s\n", args.argv[i]);
		f8_uuid_to_string(&b->h.uuid, uuidBuf, sizeof(uuidBuf));
		utils_trace("Uuid=%s\n", uuidBuf);
		utils_trace("RefCount = %d\n", query_resource(&b->h.uuid));
		enum_res_items(&b->h.uuid, _ei_proc, 0);

		utils_trace(" =========== Instance resource of block %s\n", args.argv[i]);
		f8_uuid_to_string(&b->uuid, uuidBuf, sizeof(uuidBuf));
		utils_trace("Uuid=%s\n", uuidBuf);
		utils_trace("RefCount = %d\n", query_resource(&b->uuid));
		enum_res_items(&b->uuid, _ei_proc, 0);

		if(b->h.magic == CBLK_MAGIC){
		}
	}
	return F8_SUCCESS;
}

static __bool _find_owner(ITreeNode * nd, __bool firstVisit, __uint context)
{
	IBlk * b;
	struct blk_pin_t * p;
	mem_addr_t * addr;

	addr = (mem_addr_t*)context;

	b = __dcast__(ITreeNode, IBlk, nd);
	if(b->h.magic != BBLK_MAGIC){
		return __true;
	}
	for(p = __vcall__(b, first_pin, (b)); p; p = __vcall__(b, next_pin, (p))){
		if(p->_class == PIN_CLASS_EI || p->_class == PIN_CLASS_EO){
			continue;
		}
		if(p->binding == PIN_B_CONST){
			continue;
		}
		if(addr->section == p->u2.mem_addr.section 
			&& 
			addr->addr == p->u2.mem_addr.addr
		){
			char path[F8_MAX_PATH];
			IBlk_path_by_pin(p, path, sizeof(path));
			utils_trace("%s\n", path);
		}
	}

	return __true;
}

int on_whouses(char *v, void * k)
{
	CArgs args;
	struct blk_var_t * var;
	struct mem_addr_t addr;

	parse_arg_ex(v, &args, FS);

	if(args.argc != 2){
		return F8_SYNTAX;
	}

	var = query_var_by_fullname(args.argv[1]);
	if(!var){
		if(strlen(args.argv[1]) < 2){
			return F8_SYNTAX;
		}
		addr.section = args.argv[1][0] - '0';
		addr.addr = atoi(args.argv[1] + 1);
		if(!is_address_valid(proxy_adapter->kernel, 0, &addr)){
			return F8_INVALID_ADDRESS;
		}
	}else{
		addr = var->addr;
	}

	_traverse(sections, _find_owner, (__uint)&addr);


	return F8_SUCCESS;
}

int on_whoowns(char *v, void * k)
{
	void * o;
	f8_uint magic;
	char ppath[F8_MAX_PATH];
	CArgs args;
	struct blk_var_t * var;
	struct mem_addr_t addr;

	parse_arg_ex(v, &args, FS);

	if(args.argc != 2){
		return F8_SYNTAX;
	}

	if(strlen(args.argv[1]) < 2){
		return F8_SYNTAX;
	}
	addr.section = args.argv[1][0] - '0';
	addr.addr = atoi(args.argv[1] + 1);
	if(!is_address_valid(proxy_adapter->kernel, 0, &addr)){
		return F8_INVALID_ADDRESS;
	}

	o = get_address_owner(&addr, 1, &magic);

	if(o){
		if(magic == PIN_MAGIC){
			IBlk_path_by_pin((blk_pin_t*)o, ppath, sizeof(ppath));
			utils_trace("%s\n", ppath);
		}else if(magic == VAR_MAGIC){
			var = (blk_var_t*)o;
			IBlk_path_by_blk(var->blk, ppath, sizeof(ppath));
			utils_trace("%s.%s\n", ppath, var->name);
		}
	}

	return F8_SUCCESS;
}
