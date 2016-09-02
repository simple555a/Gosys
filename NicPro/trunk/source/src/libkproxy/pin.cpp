/*
	This module manages configurations related to pins.
	by J.Hunter, 2003/9/18
*/
#include "precomp.h"
struct yy_buffer_state * yy_scan_string(const char *);
void yy_delete_buffer(struct yy_buffer_state *);

int on_exp(char *v, void *k)
{
	CArgsEx a;
	struct blk_pin_t * p;
	f8_uuid	uuid;
	__u16	idx;

	_shproc_prolog_();

	a.parse(v, FS);
	if(a.argc < 3){
		utils_error("Usage : export <name> <pin path>\n");
		return F8_SYNTAX;
	}

	p = IBlk_pin_by_path(sections, a[2]);
	if(!p){
		return F8_OBJECT_NOT_FOUND;
	}

	if(parent_blk(p->blk)->h.flags & BLK_F_READONLY){
		return F8_PERMISSION;
	}

	if(IBlk_pin_by_name(parent_blk(p->blk), a[1])){
		return F8_NAME_DUPLICATE;
	}

	if(query_var_by_pin(p)){
		return F8_PIN_NOT_EXPORTABLE;
	}

	create_f8_uuid(&uuid);
	idx = __vcall__(p->blk, id_by_pin, (p));

	__vcall__(s, put, (s, &uuid, sizeof(f8_uuid)));
	__vcall__(s, put, (s, &p->blk->uuid, sizeof(f8_uuid)));	
	__vcall__(s, put, (s, &idx, sizeof(idx)));
	
	cmd.cmd = F8_EXPORT;
	cmd.isize = ms.pointer;
	create_f8_uuid(&NEW_ID(&cmd));

	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, psp->timeout));

	if(!successful){
		return cmd.retcode;
	}

	p = IBlk_pin_by_path(sections, a[2]);
	assert(p);
	p = p->ulink;
	assert(p);

	create_resource(&uuid);
	set_res_name(&uuid, a[1]);	

	return F8_SUCCESS;
}

int on_uexp(char *v, void *k)
{
	CArgsEx a;
	IBlk	*blk;
	__u16	idx;
	struct blk_pin_t * pin;
	f8_uuid	saved_uuid;
	struct cblk_pin_t * cpin;
	struct blk_var_t * var;

	_shproc_prolog_();

	a.parse(v, FS);
	if(a.argc < 2){
		utils_error("Usage : uexp <pin path>\n");
		return F8_SYNTAX;
	}

	pin = IBlk_pin_by_path(sections, a.argv[1]);
	if(!pin){
		return F8_PATH_NOT_FOUND;
	}
	blk = pin->blk;

	if(pin->blk->h.flags & BLK_F_READONLY){
		return F8_ACCESS;
	}

	if(pin->_class != PIN_CLASS_EXP){
		return F8_INVALID_OPERATION;
	}

	if(ICBlk_first_connection(pin)){
		return F8_PENDING_REFERENCES;
	}

	var = (struct blk_var_t*)query_var_by_pin(pin);
	if(var){
		return F8_PENDING_REFERENCES;
	}
	
	cpin = CONTAINING_RECORD(pin, struct cblk_pin_t, p);
	saved_uuid = cpin->uuid;
	__vcall__(s, put, (s, &blk->uuid, sizeof(f8_uuid)));
	idx = __vcall__(blk, id_by_pin, (pin));
	__vcall__(s, put, (s, &idx, sizeof(idx)));

	cmd.cmd = F8_UNEXPORT;
	cmd.isize = ms.pointer;
	create_f8_uuid(&NEW_ID(&cmd));

	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, psp->timeout));

	if(!successful){
		return cmd.retcode;
	}else{
	}

	unload_resource(&saved_uuid);

	return F8_SUCCESS;
}

const char * _type_name(int type)
{
	switch(type){
	case PIN_T_CHAR:
		return "char";
	case PIN_T_SHORT:
		return "short";
	case PIN_T_INTEGER:
		return "integer";
	case PIN_T_BYTE:
		return "byte";
	case PIN_T_WORD:
		return "word";
	case PIN_T_DWORD:
		return "dword";
	case PIN_T_FLOAT:
		return "float";
	case PIN_T_DOUBLE:
		return "double";
	case PIN_T_BOOL:
		return "bool";
	case PIN_T_EVENT:
		return "event";
	case PIN_T_TIMER:
		return "timer";
	case PIN_T_DATE:
		return "date";
	}
	return "<unknown>";
}

__uint _type_size(__u8 type)
{
	switch(type){
	case PIN_T_INTEGER:
		return 4;
	case PIN_T_BOOL:
		return 1;
	case PIN_T_FLOAT:
		return 4;
	case PIN_T_TIMER:
		return 8;
	case PIN_T_BYTE:
		return 1;
	case PIN_T_WORD:
		return 2;
	case PIN_T_DOUBLE:
		return 8;
	case PIN_T_DATE:
		return 8;
	case PIN_T_CHAR:
		return 1;
	case PIN_T_SHORT:
		return 2;
	case PIN_T_DWORD:
		return 4;
	}
	return (__uint)-1;
}

f8_status memaddr_by_text(const char * text, struct mem_addr_t * addr)
{
	if(text[0] != '%'){
		return F8_INVALID_ADDRESS;
	}
	if(strlen(text) < 3){
		return F8_INVALID_ADDRESS;
	}
	if(!strchr("0134", text[1])){
		return F8_INVALID_ADDRESS;
	}
	addr->section = text[1] - '0';
	addr->addr = atoi(text + 2);

	return F8_SUCCESS;
}

/*
	modify a pin's binding state
	constant binding/memory binding
	syntax :
	setp <pin_path> <pin_binding> [inverted]
*/
int on_bind(char *v, void *k)
{
	CArgsEx a;
	struct blk_pin_t *p;
	int		token;
	__u16	pid;
	struct	conf_mod_pin mp;
	struct  mem_addr_t oldAddr;
	int		length;
	bool	bShouldRestoreOldRegSettingIfFailed;
	blk_var_t *oldVar = 0;
	blk_var_t *var = 0;
	IBlk	*blk;

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
	utils_trim(a.argv[1],  '"');
	utils_trim(a.argv[2],  '"');
	p = IBlk_pin_by_path(sections, a[1]);
	if(!p){
		return F8_OBJECT_NOT_FOUND;
	}

	if(p->ulink){
		return F8_PIN_NOT_BINDABLE;
	}

	blk = parent_blk(p->blk);
	if(blk && BLK_IS_READONLY(blk)){
		return F8_PERMISSION;
	}

	p = IBlk_real_pin(p);

	if(p->_class != PIN_CLASS_DI && p->_class != PIN_CLASS_DO){
		/*
			event pins cannot have binding
		*/
		return F8_INVALID_OPERATION;
	}

	if(!RtkIsTripleListEmpty(&p->u1.link)){
		/*
			only output pins can have bindings when attached
		*/
		if(!IBlk_is_output(p)){
			return F8_INVALID_OPERATION;
		}		
	}

	length = _type_size(p->type);
	oldAddr = p->u2.mem_addr;
	bShouldRestoreOldRegSettingIfFailed = false;
	oldVar = query_var_by_addr(&oldAddr);

#if 0
	strcpy(yy_buffer, a[2]);
	yy_pointer = 0;
#endif

	struct yy_buffer_state * bs;
//	struct yy_buffer_state * yy_scan_string(const char *);
	extern char * yytext;
	
//	void yy_delete_buffer(struct yy_buffer_state *);
	bs = yy_scan_string(a[2]);
	token = yylex();
	strcpy(yy_buffer, yytext);
	yy_delete_buffer(bs);
	
	if(IBlk_is_output(p) && token != TOK_ID){
		return F8_INVALID_OPERATION;
	}

	
	switch(token){
	case TOK_NUMBER:		
		if(is_numeric_pin(p)){
			mp.u.ival = atoi(a[2]);
		}else if(p->type == PIN_T_FLOAT){
			mp.u.fval = (__r4)atoi(a[2]);
		}else if(p->type == PIN_T_DOUBLE){
			mp.u.dval = atoi(a[2]);
		}else{
			return F8_TYPE_MISMATCH;
		}
		mp.binding = PIN_B_CONST;
		break;
	case TOK_HEX_NUMBER:
		sscanf(a[2], "%x", &mp.u.ival);
		if(is_numeric_pin(p)){			
		}else if(p->type == PIN_T_FLOAT){
			mp.u.fval = (__r4)mp.u.ival;
		}else if(p->type == PIN_T_DOUBLE){
			mp.u.dval = mp.u.ival;
		}else{
			return F8_TYPE_MISMATCH;
		}
		mp.binding = PIN_B_CONST;
		break;
	case TOK_REAL_NUMBER:		
		if(p->type == PIN_T_FLOAT){
			mp.u.fval = (__r4)atof(a[2]);
		}else if(p->type == PIN_T_DOUBLE){
			mp.u.dval = atof(a[2]);
		}else{
			return F8_TYPE_MISMATCH;
		}
		mp.binding = PIN_B_CONST;
		break;
	case TOK_DATE_S:
		if(p->type != PIN_T_DATE){
			return F8_TYPE_MISMATCH;
		}
		yy_buffer[strlen(yy_buffer) - 1] = 0;
		mp.u.dval = atof(yy_buffer + 2);
		mp.binding = PIN_B_CONST;
		break;
	case TOK_DATE_MS:
		if(p->type != PIN_T_DATE){
			return F8_TYPE_MISMATCH;
		}
		yy_buffer[strlen(yy_buffer) - 2] = 0;
		mp.u.dval = atof(yy_buffer + 2) / 1000;
		mp.binding = PIN_B_CONST;
		break;
	case TOK_ID:
		var = (blk_var_t*)query_var_by_fullname(yy_buffer);
		if(!var){
			var = (blk_var_t*)query_var(parent_blk(p->blk), yy_buffer);
		}		
		if(!var){
			var = (blk_var_t*)query_var(0, yy_buffer);
		}
		if(!var){
			return F8_OBJECT_NOT_FOUND;
		}

		if(var->type != p->type){
			return F8_TYPE_MISMATCH;
		}

		mp.u.addr = var->addr;
		if(p->binding == PIN_B_MEMORY){
			if(oldAddr.addr == mp.u.addr.addr 
				&& 
				oldAddr.section == mp.u.addr.section
			){
				return F8_SUCCESS;
			}
			if(get_address_owner(&oldAddr, length, 0) == (void*)p){
				bShouldRestoreOldRegSettingIfFailed = true;
				free_reg(&oldAddr, length);
			}
		}
		
		mp.binding = PIN_B_MEMORY;
		break;
	default:
		return F8_SYNTAX;
	}

	// ok, all test passed
	cmd.cmd = F8_MODIFY_PIN;
	create_f8_uuid(&NEW_ID(&cmd));
	__vcall__(s, put, (s, &p->blk->uuid, sizeof(f8_uuid)));
	pid = __vcall__(p->blk, id_by_pin, (p));
	__vcall__(s, put, (s, &pid, sizeof(pid)));
	__vcall__(s, put, (s, &mp, sizeof(mp)));
	cmd.isize = ms.pointer;

	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, psp->timeout));

	if(!successful){
		if(bShouldRestoreOldRegSettingIfFailed){
			set_address_owner(p, &oldAddr, length, PIN_MAGIC, __true);
		}
	}else{
		if(var){
			var->ref_count++;
		}
		if(oldVar){
			oldVar->ref_count--;
		}
	}
	
	return cmd.retcode;
}

int on_invert(char *v, void *k)
{
	struct blk_pin_t *p;
	CArgsEx a;
	f8_u8	invert;
	__u16	pid;
	IBlk	*blk;
	
	_shproc_prolog_();
	a.parse(v, FS);
	if(a.argc < 2){
		return F8_SYNTAX;
	}
	p = IBlk_pin_by_path(sections, a[1]);
	if(!p){
		return F8_OBJECT_NOT_FOUND;
	}

	blk = parent_blk(p->blk);
	if(blk && BLK_IS_READONLY(blk)){
		return F8_PERMISSION;
	}

	p = IBlk_real_pin(p);
	if(p->type != PIN_T_BOOL){
		return F8_INVALID_OPERATION;
	}
	cmd.cmd = F8_SET_INVERT;
	invert = !(p->flags & PIN_F_INVERTED);
	create_f8_uuid(&NEW_ID(&cmd));
	__vcall__(s, put, (s, &p->blk->uuid, sizeof(f8_uuid)));
	pid = __vcall__(p->blk, id_by_pin, (p));
	__vcall__(s, put, (s, &pid, sizeof(pid)));
	__vcall__(s, put, (s, &invert, sizeof(invert)));
	cmd.isize = ms.pointer;

	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, psp->timeout));

	return cmd.retcode;
}

/*
	remove pin binding
*/
int on_ubind(char *v, void *k)
{
	struct blk_pin_t *p;
	CArgsEx a;
	__u16	pid;
	struct	conf_mod_pin mp;
	int		length;
	struct blk_var_t * var;
	
	_shproc_prolog_();
	a.parse(v, FS);
	if(a.argc < 2){
		return F8_SYNTAX;
	}
	p = IBlk_pin_by_path(sections, a[1]);
	if(!p){
		return F8_OBJECT_NOT_FOUND;
	}
	
	if(BLK_IS_READONLY(parent_blk(p->blk))){
		return F8_PERMISSION;
	}

	var = (struct blk_var_t * )query_var_by_pin(p);
	if(!var){
		return F8_UNBOUND;
	}
	
	p = IBlk_real_pin(p);
	switch(p->_class){
	case PIN_CLASS_EI:
	case PIN_CLASS_EO:
		return F8_INVALID_OPERATION;
	case PIN_CLASS_DI:
		if(!RtkIsTripleListEmpty(&p->u1.link)){
			return F8_INVALID_OPERATION;
		}
		break;
	case PIN_CLASS_DO:
		break;
	}
	
	/* 
		detach the pin from the variable
		for input pins, mark it as const binding
		for output pins, allocate a register for it
	*/
	memset(&mp, 0, sizeof(mp));
	if(p->_class == PIN_CLASS_DI){
		mp.binding = PIN_B_CONST;
		mp.u.dval = 0;
	}else{
		length = _type_size(p->type);
		mp.binding = PIN_B_MEMORY;
		mp.u.addr.section = ke_section_by_type(p->type);
		if(!alloc_reg(p, &mp.u.addr, length, PIN_MAGIC)){
			return F8_LOW_REGISTERS;
		}
	}

	// ok, all test passed
	cmd.cmd = F8_MODIFY_PIN;
	create_f8_uuid(&NEW_ID(&cmd));
	__vcall__(s, put, (s, &p->blk->uuid, sizeof(f8_uuid)));
	pid = __vcall__(p->blk, id_by_pin, (p));
	__vcall__(s, put, (s, &pid, sizeof(pid)));
	__vcall__(s, put, (s, &mp, sizeof(mp)));
	cmd.isize = ms.pointer;

	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, psp->timeout));

	if(!successful){
		if(mp.binding == PIN_B_MEMORY){
			free_reg(&mp.u.addr, length);
		}		
	}else{
		var->ref_count--;
	}

	return cmd.retcode;
}

