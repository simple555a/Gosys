#pragma warning(disable:4018)
#include "precomp.h"

struct yy_buffer_state * yy_scan_string(const char *);
void yy_delete_buffer(struct yy_buffer_state *);

#pragma hdrstop
#include <map>

/*++
algorithm description

each composite block is associated with a variable list which describes
the variables belonging to that block. if the block is the root block(sections),
the variables are called ``global variables'', otherwise they are called local
variables. different blocks may have variables with the same name.

global variables and local variables are both allocated from a single register
heap. a map with key type of mem_addr_t is used as the index to help 
translating memory address back to variables(the blk_var_t object).

each block has a variable map with a key of type string, which is used to
translate name to the underlying blk_var_t object.

--*/

class address_less{
public:
	bool operator()(
		const struct mem_addr_t & a, 
		const struct mem_addr_t & b
		) const
	{
		if(a.section < b.section){
			return true;
		}else if(a.section > b.section){
			return false;
		}
		if(a.addr < b.addr){
			return true;
		}
		return false;
	}
};

class name_less{
public:
	bool operator()(
		const string & a, 
		const string & b
		) const
	{
		return strcmpi(a.c_str(),b.c_str())<0? true : false;
	}
};

typedef list<blk_var_t> var_list_t;
typedef map<mem_addr_t, blk_var_t *, address_less> addr_idx_t;
typedef map<string, blk_var_t *,name_less> name_idx_t;

struct db_t{
	// IBlk		*blk;
	f8_uuid		blk_uuid;
	var_list_t	variables;
	name_idx_t	nameIndices;
};

typedef map<f8_uuid, db_t> db_map_t;

static db_map_t   variables;
static addr_idx_t addrIndices;

string _dpinValue(int type, dyn_pin_t * dp)
{
	char buf[32];
	switch(type){
	case PIN_T_CHAR:
		sprintf(buf,"%c", dp->u.d.i8);
		break;
	case PIN_T_SHORT:
		sprintf(buf,"%d", dp->u.d.i16);
		break;
	case PIN_T_INTEGER:
		sprintf(buf,"%d", dp->u.d.i32);
		break;
	case PIN_T_BYTE:
		sprintf(buf,"%d", dp->u.d.ui8);
		break;
	case PIN_T_WORD:
		sprintf(buf,"%d", dp->u.d.ui16);
		break;
	case PIN_T_DWORD:
		sprintf(buf,"%d", dp->u.d.ui32);
		break;
	case PIN_T_BOOL:
		sprintf(buf,"%s",dp->u.d.i8? "true" : "false");
		break;
	case PIN_T_TIMER:
		sprintf(buf,
			"%d", 
			dp->u.e.interval
			);
		break;
	case PIN_T_FLOAT:
		sprintf(buf,"%f", dp->u.d.flt);
		break;
	case PIN_T_DOUBLE:
	case PIN_T_DATE:
		sprintf(buf,"%f", dp->u.d.dbl);
		break;
	default:
		sprintf(buf,"N.A.");
		break;
	}
	return buf;
}

int on_lv(char *_v, void * k)
{	
	int i, ndb;
	db_map_t::iterator it;
	struct blk_var_t * v;
	IBlk * blk;

	utils_trace("%-7s %-16s %-16s %-4s %-8s %s\n", 
		"Addr.",
		"Variable",
		"Scope",
		"Ref",
		"Init"
		"Comment"
		);
	for(it = variables.begin(), ndb = variables.size(); ndb; ndb--, it++){
		var_list_t::iterator p;
		var_list_t * vlist;
		vlist = &it->second.variables;
		blk = ke_get_blk(proxy_adapter->kernel, &it->second.blk_uuid);
		if(it->second.blk_uuid == BLK_ID_ROOT){
			for(p = vlist->begin(), i=vlist->size(); i; i--, p++){
				v = &(*p);
				utils_trace(
					"%%%d%05d %-16s %-16s %-4d %s\n",
					v->addr.section,
					v->addr.addr,
					v->name,
					"global",
					v->ref_count,
					_dpinValue(v->type,&v->initVal).c_str(),
					v->comment
					);
			}
		}else{
			char path[F8_MAX_PATH];
			IBlk_path_by_blk(blk, path, sizeof(path));
			for(p = vlist->begin(), i=vlist->size(); i; i--, p++){
				v = &(*p);
				utils_trace(
					"%%%d%05d %-16s %-16s %-2d %s\n",
					v->addr.section,
					v->addr.addr,
					v->name,
					path,
					v->ref_count,
					_dpinValue(v->type,&v->initVal).c_str(),
					v->comment
					);
			}
		}
	}

	return F8_SUCCESS;
}

f8_status delete_variable(struct blk_var_t * var)
{
	db_map_t::iterator dbit;
	var_list_t::iterator varit;
	name_idx_t::iterator nit;
	addr_idx_t::iterator addrit;
	var_list_t * vlist;
	int i;
	
	free_reg(&var->addr, var->size);	
	
	dbit = variables.find(var->blk_uuid);
	assert(dbit != variables.end());
	
	vlist = &dbit->second.variables;
	for(i=vlist->size(), varit = vlist->begin(); i; i--, varit++){
		if(varit->uuid == var->uuid){
			break;
		}
	}
	assert(varit != vlist->end());

	// delete from name-indices
	nit = dbit->second.nameIndices.find(var->name);
	assert(nit != dbit->second.nameIndices.end());
	dbit->second.nameIndices.erase(nit);	

	// delete from address-indices
	addrit = addrIndices.find(var->addr);
	assert(addrit != addrIndices.end());
	addrIndices.erase(addrit);

	// delete resource
	unload_resource(&var->uuid);
	
	// delete from var-map
	dbit->second.variables.erase(varit);

	// last variable in the pertaining table?
	if(!dbit->second.variables.size()){
		variables.erase(dbit);
	}	
	
	return F8_SUCCESS;
}

f8_status undefine_variable(struct blk_var_t * var)
{
	// undefine variable
	if(var->ref_count){
		return F8_PENDING_REFERENCES;
	}

	var->blk_uuid = var->blk->uuid;

	return delete_variable(var);
}

int on_uset(char *v, void * k)
{
	CArgs args;
	const struct blk_var_t * var;

	parse_arg_ex(v, &args, FS);
	if(args.argc != 2){
		return F8_SYNTAX;
	}
	var = query_var_by_fullname(args.argv[1]);
	if(!var){
		return F8_OBJECT_NOT_FOUND;
	}
	if(BLK_IS_READONLY(var->blk)){
		return F8_PERMISSION;
	}

	_shproc_prolog_();

	/* deletion of a global variable leads to version change */
	if(var->blk == sections && (proxy_adapter->flags & ADAPTER_F_ONLINE)){
		cmd.cmd = F8_DEL_VARIABLE;
		cmd.isize = sizeof(var->uuid);
		create_f8_uuid(&NEW_ID(&cmd));
		*((f8_uuid*)cmd.ibuffer) = var->uuid;
		__vcall__(proxy_adapter, exec_remote, (proxy_adapter, &cmd, 5000));
		if(!successful){
			return cmd.retcode;
		}
		sections->h.uuid = NEW_ID(&cmd);
	}
	
	cmd.retcode = undefine_variable((struct blk_var_t*)var);
	if(F8_FAILED(cmd.retcode)){
		return cmd.retcode;
	}

	/*
		2005/7/5
		deletion of a variable changes the memory map, leading
		to a possibility of PMC client to accidentally overwrite memory
		by setting a variable's value. We prevent this by changing
		prog version, thus write operation from PMC client is rejected
		by the running kernel.

		2005/7/6
		see comments in HISTORY
	*/
	return cmd.retcode;
}

/*
	information needed to define a variable is contained
	in the `v' parameter.
	
	on return, the _lv will point to the defined variable
	structure.

	input parameters:

	- v->blk

	- v->uuid

	- v->name

	- v->comment

	- v->addr
	
	note this field might be invalid(section is INVALID_SECTION), 
	and in this case the address for this variable will be 
	allocated automatically, otherwise the specified address will
	be used, if an address conflict is detected by reg-map
	layer, the previously owner for the address will be 
	deprived of ownership.

	two things will be done here:
	1) the internal variable database is updated
	2) the register map is updated.
*/
f8_status define_variable(const struct blk_var_t * v, struct blk_var_t ** _lv)
{
	// find the associated db, or create one if not exist
	var_list_t::iterator varit;
	db_map_t::iterator it;
	db_t	ldb, *db;
	struct blk_var_t * lv;

	/* if an address is specified, then it must be valid*/
	if(v->addr.section != MEM_SECTION_INVALID){
		if(!is_valid_address(&v->addr)){
			return F8_INVALID_ADDRESS;
		}
	}
	
	if(query_var(v->blk, v->name)){
		return F8_VAR_ALREADY_DEFINED;
	}

	it = variables.find(v->blk->uuid);
	if(it == variables.end()){
		ldb.blk_uuid = v->blk->uuid;
		pair<db_map_t::iterator, bool> pi;
		pi = variables.insert(
			db_map_t::value_type(v->blk->uuid, ldb)
			);
		if(!pi.second){
			return F8_LOW_MEMORY;
		}
		it = pi.first;
	}

	db = &it->second;

	if(v->addr.section == MEM_SECTION_INVALID){
		/*
			if the variable is not given an address, it's
			allocated here
		*/
		varit = db->variables.insert(db->variables.end(), *v);
		lv = &(*varit);
		lv->addr.section = ke_section_by_type(v->type);
		lv->size = _type_size(lv->type);
		if(lv->addr.section == MEM_SECTION_INVALID){
			db->variables.erase(varit);
			return F8_INVALID_DATA;
		}
		if(!alloc_reg(lv, &lv->addr, lv->size, VAR_MAGIC)){
			db->variables.erase(varit);
			return F8_LOW_REGISTERS;
		}
	}else{
		varit = db->variables.insert(db->variables.end(), *v);
		lv = &(*varit);
		lv->size = _type_size(lv->type);
		f8_uint magic;
		void * oldOwner;
		oldOwner = get_address_owner(&lv->addr, lv->size, &magic);
		if(oldOwner){
			utils_error(
				"Warning : depriving register %d%05d from %08x(%c%c)\n",
				lv->addr.section,
				lv->addr.addr,
				oldOwner,
				magic & 0xff,
				magic >>8
				);
		}
		set_address_owner((void*)v, &lv->addr, lv->size, VAR_MAGIC, __true);
	}
	addrIndices[lv->addr] = lv;
	db->nameIndices[lv->name] = lv;

	// 
	create_resource(&lv->uuid);
	set_res_buf(&lv->uuid, lv->name, -1, "name");
	if(lv->comment){
		set_res_buf(&lv->uuid, lv->comment, -1, "comment");
		lv->comment = get_res_buf(&lv->uuid, 0, "comment");
	}else{
		lv->comment = "";
	}

	lv->ref_count = 0;
	lv->magic = BLK_VAR_MAGIC;
	lv->size = _type_size(lv->type);
		
	if(_lv){
		*_lv = lv;
	}

	return F8_SUCCESS;
}

static __bool is_valid_var_name(const char * name)
{
	return is_valid_pin_name(name);
}

static f8_status _dpinByText(int type,const char *tv,dyn_pin_t & v)
{
	switch(type){
		case PIN_T_CHAR:
			v.u.d.i8=atoi(tv);
			break;
		case PIN_T_BOOL:
			if(!strcmpi(tv,"true") || !strcmpi(tv,"on") || !strcmpi(tv,"1") || !strcmpi(tv,"t"))
				v.u.d.ui8=1;
			if(!strcmpi(tv,"false") || !strcmpi(tv,"off") || !strcmpi(tv,"0") || !strcmpi(tv,"f"))
				v.u.d.ui8=1;
			break;
		case PIN_T_BYTE:
			v.u.d.ui8=atoi(tv);
			break;
		case PIN_T_SHORT:
		case PIN_T_WORD:
			v.u.d.i16=atoi(tv);
			break;
		case PIN_T_DWORD:
		case PIN_T_INTEGER:
			v.u.d.i32=atoi(tv);
			break;
		case PIN_T_FLOAT:
			v.u.d.flt=(f8_r4)atoi(tv);
			break;
		case PIN_T_DOUBLE:
			v.u.d.dbl=atoi(tv);
			break;
		case PIN_T_DATE:
			if(strnicmp(tv,"t#",2))
				return F8_SYNTAX;
			if(tolower(tv[strlen(tv)-1] != 's'))
				return F8_SYNTAX;
			if(tolower(tv[strlen(tv)-2] == 'm')){
				sscanf(tv+2,"%fms",&v.u.d.flt);
			}else if(isdigit(tv[strlen(tv)-2])){
				sscanf(tv+2,"%fs",&v.u.d.flt);
			}else{
				return F8_SYNTAX;
			}
			v.u.d.dbl=(f8_r8)v.u.d.flt;
			break;
		default:
			return F8_INVALID_DATA;
	}
	return F8_SUCCESS;
}

/*
	define variable
*/
int on_set(char *v, void * k)
{
	CArgs args;
	char * type = 0;
	char * name = 0;
	char * path = 0;
	char * comment = "";
	char * initVal=0;
	struct blk_var_t var,*lv;

	
	BEGIN_ARGS
		ARG("type=", type, "char *", "variable type")
		ARG("name=", name, "char *", "variable name")
		ARG("comment=", comment, "char *", "variable comment")
		ARG("scope=", path, "char *", "pertaining block(for local variable only)")	
		ARG("init=",initVal,"char *","initial value")
	END_ARGS

	_shproc_prolog_();

	parse_arg_ex(v, &args, FS);

	if(process_args_1(args.argc, args.argv) < 0){
		print_usage_1();
		return F8_SYNTAX;
	}

	if(!type || !name){
		print_usage_1();
		return F8_SYNTAX;
	}

	if(!type[0] || !name[0]){
		print_usage_1();
		return F8_SYNTAX;
	}
	
	memset(&var, 0, sizeof(var));

	if(path){
		var.blk = IBlk_blk_by_path(sections, path);
	}else{
		var.blk = sections;
	}
	if(!var.blk){
		return F8_PATH_NOT_FOUND;
	}

	if(!is_valid_var_name(name))
		return F8_INVALID_NAME;
	
	if(BLK_IS_READONLY(var.blk)){
		return F8_PERMISSION;
	}

	if(!strcmp(type, "char")){
		var.type = PIN_T_CHAR;
	}else if(!strcmp(type, "short")){
		var.addr.section = MEM_SECTION_WORD;
	}else if(!strcmp(type, "integer")){
		var.type = PIN_T_INTEGER;
	}else if(!strcmp(type, "byte")){
		var.type = PIN_T_BYTE;
	}else if(!strcmp(type, "word")){
		var.type = PIN_T_WORD;
	}else if(!strcmp(type, "dword")){
		var.type = PIN_T_DWORD;
	}else if(!strcmp(type, "float")){
		var.type = PIN_T_FLOAT;
	}else if(!strcmp(type, "double")){
		var.type = PIN_T_DOUBLE;
	}else if(!strcmp(type, "date")){
		var.type = PIN_T_DATE;
	}else if(!strcmp(type, "bool")){
		var.type = PIN_T_BOOL;
	}else{
		var.type = atoi(type);;
	}

	if(initVal){
		if(F8_FAILED(cmd.retcode=_dpinByText(var.type,initVal,var.initVal)))
			return cmd.retcode;
	}
	
	strncpy(var.name, name, sizeof(var.name));
	var.name[sizeof(var.name) - 1] = '\0';
	var.addr.section = MEM_SECTION_INVALID;
	var.comment = comment;
	create_f8_uuid(&var.uuid);

	cmd.retcode = define_variable(&var, &lv);
	if(F8_FAILED(cmd.retcode)){
		return cmd.retcode;
	}

	create_f8_uuid(&NEW_ID(&cmd));
	
	/* for global variables, synchronization with controller is needed */
	if(var.blk == sections && (proxy_adapter->flags & ADAPTER_F_ONLINE)){
		cmd.isize = sizeof(var);
		cmd.cmd = F8_ADD_VARIABLE;
		struct blk_var_t *nv=(struct blk_var_t*)cmd.ibuffer;
		*nv= var;
		__vcall__(proxy_adapter, exec_remote, (proxy_adapter, &cmd, 5000));
		if(!successful){
			undefine_variable(lv);
			return cmd.retcode;
		}
	}

	
	/* update local version */
	sections->h.uuid = NEW_ID(&cmd);
	
	return cmd.retcode;
}

static __bool _del_var(struct blk_var_t * v, __uint context)
{
	unload_resource(&v->uuid);
	return __true;
}

void unload_variables()
{
	enum_variables(0, _del_var, 0);
	// del_res_item(&sections->uuid, "variables");
	variables.clear();
	addrIndices.clear();
}

/*
	f8_status load_variables()

	load variable definition from resource, see
	save_variables() for the mechanism used.

	this proc is called during project loading.

	for each defined variable, this procedure will modify
	the register map, setting the owner field of the consumed
	memory region to the variable object, the information
	is later used to track the memory usage.
*/
f8_status load_variables()
{
	blk_var_t * v;
	int i, length;

	/*
		load variable definition from resource
	*/
	v = (blk_var_t*)get_res_buf(
		&sections->uuid, 
		&length, 
		"variables"
		);
	if(!v){
		return F8_SUCCESS;
	}
	length /= sizeof(blk_var_t);
	for(i=0; i<length; i++, v++){
		if(v->magic != BLK_VAR_MAGIC){
			continue;
		}
		v->blk = ke_get_blk(proxy_adapter->kernel, &v->blk_uuid);
		/*
			some variable-length information is placed else where in the 
			registry(resource)
		*/
		v->comment = get_res_buf(&v->uuid, 0, "comment");
		v->ref_count = 0;
		/*
			define the variable, the memory address of the variable
			is already specified in the blk_var_t structure.
			the register map is modified within this call.
		*/
		if(F8_FAILED(define_variable(v, 0))){
			utils_error(
				"Error defining variable '%s'.\n", 
				v->name
				);
		}
	}

	return F8_SUCCESS;
}

/*
	f8_status save_variables()
	
	save variable definitions into resource database
	
	information of the variables is split into two parts, one part
	is the name/comment, stored in resource entry pertaining
	to the uuid of the variable; the other part is the memory
	address allocation, stored in a global array, pertaining to the
	`variables' item of the resource of the project root block.
*/
f8_status save_variables()
{
	__uint i, j;
	struct blk_var_t *vArray, stackArray[64], *heapArray = 0, *vPointer;
	db_map_t::iterator dbit;
	__uint varCount = 0;
	IBlk * blk;

	dbit = variables.begin();
	for(i=variables.size(); i; i--, dbit++){
		varCount += dbit->second.variables.size();
	}

	if(varCount > 64){
		vArray = heapArray = new blk_var_t[varCount];
	}else{
		vArray = stackArray;
	}

	dbit = variables.begin();
	varCount = 0;
	for(i=variables.size(); i; i--, dbit++){
		/*
		save variables in this scope
		*/
		blk = ke_get_blk(proxy_adapter->kernel, &dbit->second.blk_uuid);
		j = query_var_list(blk, 0, 0);
		assert(j==dbit->second.variables.size());
		vPointer = &vArray[varCount];
		query_var_list(blk, vPointer, &j);
		for(;j;j--, vPointer++){
			assert(vPointer->blk == blk);
			vPointer->blk_uuid = blk->uuid;
			set_res_buf_ex(
				&vPointer->uuid, 
				vPointer->name, 
				-1, 
				"name",
				__true
				);
			if(vPointer->comment && vPointer->comment[0]){
				set_res_buf_ex(
					&vPointer->uuid,
					vPointer->comment,
					-1,
					"comment",
					__true
					);
			}
		}
#if 0
		{
			char bName[F8_MAX_PATH];
			IBlk_path_by_blk(blk,bName,sizeof(bName));
			utils_debug("%d variables saved for %s\n", dbit->second.variables.size(),bName);
		}
#endif
		
		varCount += dbit->second.variables.size();
	}

	set_res_buf(
		&sections->uuid, 
		vArray,
		varCount * sizeof(blk_var_t),
		"variables"
		);
	
	if(heapArray){
		delete heapArray;
	}

	return F8_SUCCESS;
}

/*
	if blk is NULL, return all variables pertaining to the project;
	else only variables defined within the blk scope is returned.
	if blk is the root block, then actually all global variables are
	returned.
*/
KPROXY_API f8_uint query_var_list(
	IBlk * blk,
	struct blk_var_t * buffer,
	f8_uint * items
	)
{
	db_map_t::iterator dbit;
	var_list_t::iterator p;
	db_t * vdb;
	f8_uint varCount;
	f8_uint copyCount;
	f8_uint i, j;

	if(blk){
		dbit = variables.find(blk->uuid);
		if(dbit == variables.end()){
			return 0;
		}

		vdb = &dbit->second;

		if(items){
			if(vdb->variables.size() > *items){
				copyCount = *items;
			}else{
				copyCount = vdb->variables.size();
			}
		}else{
			copyCount = 0;
		}

		for(i=0, p=vdb->variables.begin(); i<copyCount; i++, p++){
			buffer[i] = *p;
		}

		if(items){
			*items = copyCount;
		}

		return vdb->variables.size();
	}else{
		dbit = variables.begin();
		varCount = 0;
		for(i=variables.size(); i; i--, dbit++){
			varCount += dbit->second.variables.size();
		}
		if(items){
			if(varCount > *items){
				copyCount = *items;
			}else{
				copyCount = varCount;
			}
			*items = copyCount;
		}else{
			copyCount = 0;
		}
		dbit = variables.begin();
		/*
			now copyCount is temporarily used to indicate 
			how many items left to be copied
		*/
		for(i=variables.size(); i && copyCount; i--, dbit++){
			blk = ke_get_blk(ke_get_default_kernel(), &dbit->second.blk_uuid);
			vdb = &dbit->second;
			for(j=vdb->variables.size(), p=vdb->variables.begin(); j; j--, p++, copyCount--, buffer++){
				*buffer = *p;
			}
		}
		
		return varCount;
	}
}

KPROXY_API __uint enum_variables(
	IBlk * blk, 
	_ENUM_VAR_PROC proc, 
	__uint context
	)
{
	int n;
	int count;
	var_list_t::iterator p;
	blk_var_t * v;
	db_t * vdb;
	db_map_t::iterator dbit;

	if(blk){
		dbit = variables.find(blk->uuid);
		if(dbit == variables.end()){
			return 0;
		}
		vdb = &dbit->second;
		n = vdb->variables.size();
		count=0;
		for(p=vdb->variables.begin();n;n--,p++,count++){
			v = &(*p);
			proc(v, context);
		}
	}else{
		dbit = variables.begin();
		count = 0;
		while(dbit != variables.end()){
			vdb = &dbit->second;
			n = vdb->variables.size();
			for(p=vdb->variables.begin();n;n--,p++,count++){
				v = &(*p);
				proc(v, context);
			}
			dbit++;
		}
	}
	return count;
}

KPROXY_API blk_var_t * query_var_by_fullname(
	const char * name
	)
{
	CArgs args;
	IBlk * blk;
	const char * vname;
	
	parse_arg_ex(name, &args, ".");

	if(args.argc == 2){
		blk = IBlk_blk_by_path(sections, args.argv[0]);
		vname = args.argv[1];
	}else{
		blk = sections;
		vname = name;
	}
	if(!blk){
		return 0;
	}
	return query_var(blk, vname);
}

KPROXY_API blk_var_t * query_var(
	IBlk * blk,
	const char * name
	)
{
	db_map_t::iterator dbit;
	name_idx_t::iterator p;
	db_t * vdb;

	if(!blk){
		blk = sections;
	}

	dbit = variables.find(blk->uuid);
	if(dbit == variables.end()){
		return 0;
	}

	vdb = &dbit->second;

	p = vdb->nameIndices.find(name);
	if( p == vdb->nameIndices.end()){
		return 0;
	}
	return p->second;
}

KPROXY_API blk_var_t * query_var_by_addr(
	struct mem_addr_t * addr
	)
{
	addr_idx_t::iterator it;
	
	it = addrIndices.find(*addr);
	if(it == addrIndices.end()){
		return 0;
	}
	
	return it->second;
}

KPROXY_API f8_int get_var_id(const struct blk_var_t * v)
{
	db_map_t::iterator dbit;
	var_list_t::iterator varit;
	var_list_t * vlist;
	int i;

	dbit = variables.find(v->blk->uuid);
	if(dbit == variables.end()){
		return -1;
	}
	vlist = &dbit->second.variables;
	for(i=vlist->size(), varit = vlist->begin(); i; i--, varit++){
		if(varit->uuid == v->uuid){
			return vlist->size() - i;
		}
	}
	return -1;
}

/*
	get the variable of the sequential number denoted
	by `id', b is the defining block.
	note : the id is dynamically changed due to variable
	insertion/deletion, so it's only viable during a save/load
	sequence.
*/
KPROXY_API struct blk_var_t * query_var_by_id(
	IBlk * b, 
	f8_int id
	)
{
	db_map_t::iterator dbit;
	var_list_t::iterator varit;

	dbit = variables.find(b->uuid);
	if(dbit == variables.end()){
		return 0;
	}
	if(id >= dbit->second.variables.size()){
		return 0;
	}
	varit = dbit->second.variables.begin();
	for(;id;id--,varit++);
	return &(*varit);
}

/*
	query if there's an explicit reference to some
	variable on the pin.
*/
KPROXY_API struct blk_var_t * query_var_by_pin(
	struct blk_pin_t * p
	)
{
	blk_var_t * var;
	struct blk_pin_t * rp;

	rp = IBlk_real_pin(p);
	if(rp->_class != PIN_CLASS_DI && rp->_class != PIN_CLASS_DO){
		return 0;
	}
	if(rp->_class == PIN_CLASS_DI && !RtkIsTripleListEmpty(&rp->u1.link)){
		return 0;
	}
	if(rp->binding != PIN_B_MEMORY){
		return 0;
	}
	var = query_var_by_addr(&rp->u2.mem_addr);
	if(!var){
		return 0;
	}
	assert(var->type == rp->type);
	if(var->blk != parent_blk(p->blk) && var->blk != sections){
		// the variable is referenced by an exported instance of 
		// this pin, and this will be treated differently
		return 0;
	}	
	return var;
}

KPROXY_API f8_uint get_var_count(IBlk * blk)
{
	db_map_t::iterator dbit;
	name_idx_t::iterator p;
	db_t * vdb;

	if(!blk){
		blk = sections;
	}

	dbit = variables.find(blk->uuid);
	if(dbit == variables.end()){
		return 0;
	}

	vdb = &dbit->second;

	return vdb->variables.size();
}

int on_modify(char *v, void *k)
{
	CArgsEx a;
	CArgs   b;
	int		token;
	struct	conf_mod_mem mp;
	int		length;
	IBlk	*blk;
	const blk_var_t *var = 0;
	const char *vname;

	static char num_pin_type[]={
		PIN_T_BOOL,
		PIN_T_CHAR,
		PIN_T_SHORT,
		PIN_T_INTEGER,
		PIN_T_BYTE,
		PIN_T_WORD,
		PIN_T_DWORD,
		'\0'
	};
#define is_numeric_pin(p) \
	(strchr(num_pin_type, p->type)? __true : __false)

	_shproc_prolog_();

	a.parse(v, FS);
	if(a.argc < 3){
		return F8_SYNTAX;
	}
	parse_arg_ex(a[1], &b, ".");
	if(b.argc == 2){
		blk = IBlk_blk_by_path(sections, b.argv[0]);
		vname = b.argv[1];
	}else{
		blk = 0;
		vname = a[1];
	}
	var = query_var(blk, vname);
	if(!var){
		return F8_OBJECT_NOT_FOUND;
	}

	length = _type_size(var->type);

	struct yy_buffer_state * bs;
//	struct yy_buffer_state * yy_scan_string(const char *);
	extern char * yytext;
	
//	void yy_delete_buffer(struct yy_buffer_state *);
	bs = yy_scan_string(a[2]);
	token = yylex();
	strcpy(yy_buffer, yytext);
	yy_delete_buffer(bs);
	
	switch(token){
	case TOK_NUMBER:		
		if(is_numeric_pin(var)){
			mp.value.u.d.i32 = atoi(a[2]);
		}else if(var->type == PIN_T_FLOAT){
			mp.value.u.d.flt = (f8_r4)atoi(a[2]);
		}else if(var->type == PIN_T_DOUBLE){
			mp.value.u.d.dbl = atoi(a[2]);
		}else{
			return F8_TYPE_MISMATCH;
		}
		break;
	case TOK_HEX_NUMBER:
		sscanf(a[2], "%x", &mp.value.u.d.i32);
		if(is_numeric_pin(var)){			
		}else if(var->type == PIN_T_FLOAT){
			mp.value.u.d.flt = (f8_r4)mp.value.u.d.i32;
		}else if(var->type == PIN_T_DOUBLE){
			mp.value.u.d.dbl = mp.value.u.d.i32;
		}else{
			return F8_TYPE_MISMATCH;
		}
		break;
	case TOK_REAL_NUMBER:		
		if(var->type == PIN_T_FLOAT){
			mp.value.u.d.flt = (f8_r4)atof(a[2]);
		}else if(var->type == PIN_T_DOUBLE){
			mp.value.u.d.dbl = atof(a[2]);
		}else{
			return F8_TYPE_MISMATCH;
		}
		break;
	case TOK_DATE_S:
		if(var->type != PIN_T_DATE){
			return F8_TYPE_MISMATCH;
		}
		yy_buffer[strlen(yy_buffer) - 1] = 0;
		mp.value.u.d.dbl = atof(yy_buffer + 2);
		break;
	case TOK_DATE_MS:
		if(var->type != PIN_T_DATE){
			return F8_TYPE_MISMATCH;
		}
		yy_buffer[strlen(yy_buffer) - 2] = 0;
		mp.value.u.d.dbl = atof(yy_buffer + 2) / 1000;
		break;
	default:
		return F8_SYNTAX;
	}

	mp.addr = var->addr;
	mp.len = _type_size(var->type);
	// ok, all test passed
	cmd.cmd = F8_SET_MEM_VALUE;
	cmd.uuids[0] = BLK_ID_ANY;
	cmd.uuids[1] = BLK_ID_NONE;
	__vcall__(s, put, (s, &mp, sizeof(mp)));
	cmd.isize = ms.pointer;

	__vcall__(
		proxy_adapter, 
		exec, 
		(proxy_adapter, &cmd, psp->timeout)
		);

	return cmd.retcode;
}

/*
	gather variable table information and download it to
	the controller.
*/
f8_status download_var_table()
{
	struct blk_var_t *vArray, *vPointer;
	__uint varCount;
	__uint i;

	_shproc_prolog_no_k_();

	varCount = query_var_list(0, 0, 0);
	if(!varCount){
		return F8_SUCCESS;
	}

	assert(varCount * sizeof(blk_var_t) < x_buf_len);
	vArray = (blk_var_t*)ms.buffer;
	i = query_var_list(0, vArray, &varCount);
	assert(i == varCount);
	vPointer = vArray;
	for(i=0; i<varCount; i++, vPointer++){
		/* patch the blk pointer in blk_var_t to blk uuid*/
		vPointer->blk_uuid = vPointer->blk->uuid;
	}
	
	cmd.isize = varCount * sizeof(blk_var_t);
	cmd.cmd = F8_DOWNLOAD_VARTABLE;
	OLD_ID(&cmd) = BLK_ID_ANY;
	NEW_ID(&cmd) = sections->h.uuid;
	__vcall__(proxy_adapter, exec_remote, (proxy_adapter, &cmd, 10000));
	
	return cmd.retcode;
}

/*
	upload global variable informations.
*/
f8_status upload_var_table()
{
	struct blk_var_t *vPointer;
	__uint varCount;
	__uint i, j;
	IBlk * b;

	_shproc_prolog_no_k_();

	unload_variables();
	
	cmd.cmd = F8_UPLOAD_VARTABLE;
	cmd.isize = 0;
	cmd.ibuffer = 0;
	cmd.osize = x_buf_len;
	cmd.obuffer = x_buffer;
	__vcall__(proxy_adapter, exec_remote, (proxy_adapter, &cmd, 10000));

	if(!successful){
		return cmd.retcode;
	}

	vPointer = (struct blk_var_t *)cmd.obuffer;
	varCount = cmd.osize / sizeof(struct blk_var_t);
	if(cmd.osize % sizeof(struct blk_var_t)){
		return F8_INVALID_DATA;
	}

	for(i=varCount, j=0; i; i--, vPointer++){
		if(vPointer->blk_uuid != 0)
			b=ke_get_blk(proxy_adapter->kernel, &vPointer->blk_uuid);
		else
			b=sections;
		if(!b){
			char blkName[32];
			f8_uuid_to_string(&vPointer->blk_uuid, blkName, sizeof(blkName));
			utils_error("variable `%s' error, no block:%s\n", vPointer->name, blkName);
			continue;
		}
		vPointer->blk = b;
		vPointer->comment = 0;
		if(F8_SUCCESSED(define_variable(vPointer, 0))){
			j++;
		}
	}

	utils_debug("upload_var : %d uploaded, %d defined.\n", varCount, j);
	
	return cmd.retcode;

}

static void edit_var(blk_var_t * var,const char *newName,const char *comment)
{
	db_map_t::iterator dbit;
	db_t *db;
	name_idx_t::iterator nit;

	dbit=variables.find(var->blk->uuid);
	assert(dbit != variables.end());
	name_idx_t::iterator it;
	db=&dbit->second;
	
	if(newName){
		nit=db->nameIndices.find(var->name);
		assert(nit != db->nameIndices.end());
		db->nameIndices.erase(nit);
		strncpy(var->name,newName,sizeof(var->name));
		var->name[sizeof(var->name)-1]=0;
		db->nameIndices[newName] = var;
		set_res_buf(&var->uuid, var->name, -1, "name");
	}
	
	if(comment){
		set_res_buf(&var->uuid, comment, -1, "comment");
		var->comment = get_res_buf(&var->uuid, 0, "comment");
	}
}

int on_editvar(char*v,void*k)
{
	CArgs a;
	char *vName=0;
	char *newName=0;
	char *comment=0;
	blk_var_t * var;
	char * initVal=0;
	string oldComment;

	BEGIN_ARGS
		ARG("name=",vName,"char *","variable name")
		ARG("newname=",newName,"char *","new name")
		ARG("comment=",comment,"char *","comment")
		ARG("init=",initVal,"char *","initial value")
	END_ARGS

	_shproc_prolog_();

	parse_arg_ex(v,&a,FS);
	if(process_args_1(a.argc,a.argv) < 0){
		print_usage_1();
		return F8_SYNTAX;
	}

	if(!vName){
		print_usage_1();
		return F8_SYNTAX;
	}

	if(!newName && !comment){
		print_usage_1();
		return F8_SYNTAX;
	}

	if(newName && !is_valid_var_name(newName))
		return F8_INVALID_NAME;
	
	var=query_var_by_fullname(vName);
	if(!var)
		return F8_PATH_NOT_FOUND;

	if(newName && query_var(var->blk,newName)){
		return F8_NAME_DUPLICATE;
	}
	
	oldComment=var->comment;
	
	if(newName)
		create_f8_uuid(&NEW_ID(&cmd));

	if(initVal){
		if(F8_FAILED(cmd.retcode=_dpinByText(var->type,initVal,var->initVal)))
			return cmd.retcode;
	}
	
	if(newName && (proxy_adapter->flags & ADAPTER_F_ONLINE)){
		cmd.isize = sizeof(struct blk_var_t);
		cmd.cmd = F8_EDIT_VARIABLE;
		struct blk_var_t *nv=(struct blk_var_t*)cmd.ibuffer;
		*nv=*var;
		__vcall__(proxy_adapter,exec_remote,(proxy_adapter,&cmd,2000));
		if(!successful){
			return cmd.retcode;
		}
	}

	edit_var(var,newName,comment);
	
	if(newName)
		sections->h.uuid=NEW_ID(&cmd);
	
	return F8_SUCCESS;
}

