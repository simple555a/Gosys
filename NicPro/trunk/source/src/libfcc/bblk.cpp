/*
	created:	2002/12/19  16:44
	filename: 	E:\vss\pmc2\src\drivers\m8\mcc\code_gen.cpp
	author:		Jackie, jhunter@tsinghua.org.cn
	
	purpose:	Code generator
	Revision history:
	Dec.20, 2002, first workable version
	Dec.26, 2002, generate big-endian codes
	Jul.14, 2003, native code parser and syntax analyzer added
	Aug.26, 2003, C-support added
	Aug.27, 2003, 1-dimensional array and m_idata data block support
	added.
	Aug.28, 2003, uuid support added
*/
#include "precomp.h"

#include <windows.h>
#include <string>
#include <librtk.h>

using namespace std;
 

#ifdef __LINUX__
#define stricmp strcasecmp
#endif

char * _get_temp_file()
{
	return NULL;
}

class CBblkCompiler
{
public:
	CBblkCompiler();
	__bool generate(CSymbol * prg, FILE * fp);
	int	ram_size;
	int	rom_size;
	int res_size;
public:
	void clean();
	__bool parse_1(CSymbol * prg);
	__bool parse_2(CSymbol * prg);	
	__bool parse_3(FILE *fp);

	__bool load_comments(CSymbol * comments);
	__bool load_event_pin(CSymbol * ei);
	__bool load_data_pin(CSymbol * eo);
	__bool load_interface(CSymbol * itrfc);
	__bool load_implementation(CSymbol * m_funcs);

	int	find_function(char * name);
	int gen_funcs(IMemStream * s);
	__bool add_function(const char * name, int address);

public:
	int		n_ievents;
	struct blk_hdr_t m_hdr;
	struct bblk_hdr_t m_bhdr;
	struct blk_pin_t m_pins[MAX_BBLK_ITEMS];
	struct dyn_pin_t m_dpins[MAX_BBLK_ITEMS];
	struct blk_func_t m_funcs[MAX_BBLK_ITEMS];
	char	m_codebuf[MAX_CODES];
};

/*
  advance file pointer if necessary so that it's properly aligned
*/
static void _patch_file(FILE * fp)
{
	static char align[F8_ALIGN_SIZE];
	int pos;
	pos = ftell(fp);
	if(pos & (F8_ALIGN_SIZE-1)){
		pos = F8_ALIGN_SIZE - (pos & (F8_ALIGN_SIZE - 1));
		fwrite(align, 1, pos, fp);
	}
}

#define __PATCH()	_patch_file(fp)
#define __WRITE(item)	fwrite(&(item), 1, sizeof(item), fp);
#define __WRITE_V(item, n)	for(i=0;i<(n); i++){__WRITE(item[i]);}
	
CBblkCompiler::CBblkCompiler()
{
	clean();
}

void CBblkCompiler::clean()
{
	memset(this, 0, sizeof(*this));
}

static const char * _get_sproperty(CSymbol *node, const char * pname)
{
	CProperty * p;
	CTreeNode * pp;
	pp = node->GetFirstChild();
	for(;pp;pp = pp->GetNextSibling()){
		p = dynamic_cast<CProperty*>(pp);
		if( !p ){
			continue;
		}
		if( !strcmp(p->name, pname) ){
			return p->val.text;
		}
	}
	return NULL;
}

static CProperty * _get_property(CSymbol *node, const char * pname)
{
	CProperty * p;
	CTreeNode * pp;
	pp = node->GetFirstChild();
	for(;pp;pp = pp->GetNextSibling()){
		p = dynamic_cast<CProperty*>(pp);
		if( !p ){
			continue;
		}
		if( !strcmp(p->name, pname) ){
			return p;
		}
	}
	return NULL;
}

static __bool _get_iproperty(CSymbol *node, const char * pname, int &val)
{
	CProperty * p;
	p = dynamic_cast<CProperty*>(node->GetFirstChild());
	while(p){
		if( !strcmp(p->name, pname) ){
			val = p->val.ival;
			return __true;
		}
		p = dynamic_cast<CProperty*>(p->GetNextSibling());
	}
	return __false;
}

__bool CBblkCompiler::load_comments(CSymbol * sym)
{
	CSymbol * cmt;
	CSymbol * val;
	
	cmt = (CSymbol*)sym->GetFirstChild();
	while(cmt){
		val = (CSymbol*)cmt->GetFirstChild();
		assert(val);
		set_res_buf_v(
			&m_hdr.uuid,
			val->val.text,
			strlen(val->val.text) + 1,
			"%s",
			cmt->val.text
			);
		cmt = (CSymbol*)cmt->GetNextSibling();
	}

	return __true;
}

/*
  construct event pin
*/
__bool CBblkCompiler::load_event_pin(CSymbol * e)
{
	struct blk_pin_t *t;
	struct dyn_pin_t *dt;
	CProperty * o;
	int dir, i;

	t = &m_pins[m_hdr.n_pins];
	dt = &m_dpins[m_hdr.n_pins];

	// check validity
	// an event definition can have only these properties
	// direction/invoke/timer
	
	o = dynamic_cast<CProperty*>(e->GetFirstChild());
	while(o){		
		if( strcmpi(o->name, "direction") &&
			strcmpi(o->name, "timer") &&
			strcmpi(o->name, "invoke") &&
			strcmpi(o->name, "type") &&
			strcmpi(o->name, "hidden")
			){
			fprintf(
				stderr, 
				"Error : line %d, '%s' attribute is not applicable for events.\n",
				e->line,
				o->name
				);
			return __false;
		}
		o = dynamic_cast<CProperty*>(o->GetNextSibling());
	}
	
	set_res_buf_v(
		&m_hdr.uuid,
		e->val.text,
		strlen(e->val.text) + 1,
		"!p%d!name",
		m_hdr.n_pins
		);
	
	if( !_get_iproperty(e, "direction", dir) ){
		fprintf(
			stderr,
			"Error : line %d, event '%s' defined without direction.\n", 
			e->line, 
			e->val.text
			);
		return __false;
	}


	t->type = PIN_T_EVENT;

	if(_get_property(e, "hidden")){
		t->flags |= PIN_F_HIDDEN;
	} 

	if(dir == TOK_IN){
		t->_class = PIN_CLASS_EI;
		n_ievents++;

		// for inputs, get its invoke list
		t->u2.invoke  = (f8_ptr)_get_sproperty(e, "invoke");
		if(!t->u2.invoke){
			fprintf(
				stderr,
				"Error : line %d, input event '%s' defined without invocation.\n", 
				e->line, 
				e->val.text
				);
			return __false;
		}

		// it should not have 'timer' property
		if( _get_property(e, "timer") ){
			fprintf(
				stderr,
				"Error : line %d, event '%s' defined with 'timer' attribute.\n", 
				e->line, 
				e->val.text
				);
			return __false;
		}
	}else{
		t->_class = PIN_CLASS_EO;

		if( _get_iproperty(e, "timer", dt->u.e.interval) ){
			t->type = PIN_T_TIMER;
		}

		// it should not have 'timer' property
		if( _get_property(e, "invoke") ){
			fprintf(
				stderr,
				"Error : line %d, 'invoke' attribute not applicable for output event '%s'.\n", 
				e->line, 
				e->val.text
				);
			return __false;
		}
	}

	if(_get_sproperty(e, "invoke")){
		// add a function record
		// if this function has already been added?
		i = find_function((char*)t->u2.invoke);
		if(i<0){
			if(!add_function((const char*)t->u2.invoke, -1)){
				fprintf(
					stderr,
					"Error : line %d : %s, too many invokes.\n",
					e->line,
					e->val.text
					);
			}
		}
	}

	return __true;
}

/*
  construct data pin
*/
__bool CBblkCompiler::load_data_pin(CSymbol * e)
{
	struct blk_pin_t *t;
	CProperty * o;
	int dir;
	int tp;

	t = &m_pins[m_hdr.n_pins];

	// check validity
	// an data definition can have only these properties
	// direction/const
	
	o = dynamic_cast<CProperty*>(e->GetFirstChild());
	while(o){		
		if( strcmpi(o->name, "direction") &&
			strcmpi(o->name, "const") &&
			strcmpi(o->name, "type") &&
			strcmpi(o->name, "hidden")
			){
			fprintf(
				stderr, 
				"Error : line %d, '%s' attribute is not applicable for data pins.\n",
				e->line,
				o->name
				);
			return __false;
		}
		o = dynamic_cast<CProperty*>(o->GetNextSibling());
	}

	set_res_buf_v(
		&m_hdr.uuid,
		e->val.text,
		strlen(e->val.text) + 1,
		"!p%d!name",
		m_hdr.n_pins
		);
	
	if( !_get_iproperty(e, "type", tp) ){
		assert(0);
	}

	t->type = (f8_u8)tp;	

	if( !_get_iproperty(e, "direction", dir) ){
		fprintf(
			stderr,
			"Error : line %d, pin '%s' has no direction attribute.\n",
			e->line,
			e->val.text
			);
		return __false;
	}	

	if(_get_property(e, "hidden")){
		t->flags |= PIN_F_HIDDEN;
	}
	
	if(dir == TOK_IN){
		t->_class = PIN_CLASS_DI;
		t->binding = PIN_B_CONST;
	}else{
		t->_class = PIN_CLASS_DO;
		t->binding = PIN_B_MEMORY;
	}

	return __true;
}

__bool CBblkCompiler::load_interface(CSymbol * itrfc)
{
	CSymbol * var;
	blk_pin_t * p;
	int tp;

	var = (CSymbol*)itrfc->GetFirstChild();
	p = m_pins + m_hdr.n_pins;
	
	if(!var){
		fprintf(
			stderr, 
			"Error : a function block must have at least one pin defined.\n"
			);
		return __false;
	}

	while(var){
		// first we determin pin types for this var decl
		if( !_get_iproperty(var, "type", tp) ){
			fprintf(
				stderr,
				"Error : line %d, '%s' defined without type.\n", 
				var->line, 
				var->val.text
				);
			return __false;
		}

		switch(tp){

		case PIN_T_EVENT: 
			if( !load_event_pin(var) ){
				return __false;
			}
			break;

		case PIN_T_INTEGER:
		case PIN_T_BOOL:
		case PIN_T_FLOAT:
		case PIN_T_CHAR:
		case PIN_T_SHORT:
		case PIN_T_BYTE:
		case PIN_T_WORD:
		case PIN_T_DWORD:
		case PIN_T_DOUBLE:
		case PIN_T_DATE:
			if(!load_data_pin(var)){
				return __false;
			}
			break;

		case PIN_T_ARRAY:
			fprintf(
				stderr,
				"Error : line %d, array type is not valid in interface section.\n",
				var->line
				);
			return __false;

		default:
			assert(0);
			break;
		}

		m_hdr.n_pins++;
		var = (CSymbol*)var->GetNextSibling();
	}

	if(!n_ievents){
		// if the block has no input events, then it's automatically 
		// marked as 1131-style
		m_hdr.flags |= BLK_F_AUTOSCAN;
	}
	
	return __true;
}

int CBblkCompiler::find_function(char * name)
{
	int i;
	char * funcName;

	for(i=0; i<m_hdr.u1.n_funcs; i++){
		funcName = get_res_buf_v(
			&m_hdr.uuid, 
			0,
			"!f%d!name",
			i
			);
		if(!stricmp(funcName, name)){
			return i;
		}
	}

	return -1;
}

/*
  build internal representation
  convert syntax tree to a series of structures.
*/
__bool CBblkCompiler::parse_1(CSymbol * prg)
{
	CSymbol * uuid;
	CSymbol * itrfc;
	CSymbol * imple;
	CSymbol * cmts;
	CSymbol * attrs;

	// fill header
	uuid  = _get_node_by_type(prg, TOK_UUID);
	attrs = _get_node_by_type(prg, TOK_ATTRIBUTE);
	itrfc = _get_node_by_type(prg, TOK_INTERFACE);
	imple = _get_node_by_type(prg, TOK_IMPLEMENTATION);
	cmts  = _get_node_by_type(prg, TOK_COMMENTS);

	// find export variables
	memset(&m_hdr, 0, sizeof(m_hdr));

	m_hdr.uuid = uuid->val.uuid;

	if(!create_resource(&m_hdr.uuid)){
		return __false;
	}

	if(_get_property(attrs, "autoscan")){
		m_hdr.flags |= BLK_F_AUTOSCAN;
		add_function("main", -1);
	}

	if(_get_property(attrs, "input")){
		m_hdr.flags |= BLK_F_INPUT;
	}

	if(_get_property(attrs, "output")){
		m_hdr.flags |= BLK_F_OUTPUT;
	}

	if(_get_property(attrs,"system"))
		m_hdr.flags |= BLK_F_SYSTEM;
		
	set_res_name(
		&m_hdr.uuid,
		prg->val.text
		);
	set_res_int(&m_hdr.uuid, "!readonly", 1);

	if(!load_comments(cmts)){
		return __false;
	}
	
	if(!load_interface(itrfc)){
		return __false;
	}

	if( !load_implementation(imple) ){
		return __false;
	}

	return __true;
}

/*
	fixup data related to m_pins.
	1) fixups invoke reference
	2) pin classes and offsets
*/
__bool CBblkCompiler::parse_2(CSymbol * prg)
{
	int i;
	blk_pin_t * p;
	CSymbol *sym;

	sym = (CSymbol*)_get_node_by_type(g_program, TOK_INTERFACE);
	if( !sym ){
		return __true;
	}

	sym = (CSymbol*)sym->GetFirstChild();

	// generate invoke list for each event input
	for(i=0, p=m_pins; 
		i<m_hdr.n_pins && sym; 
		i++, p++, sym = (CSymbol*)sym->GetNextSibling()
	){
		switch(p->_class){
		case PIN_CLASS_EI:
			p->u2.invoke = find_function((char*)p->u2.invoke);
			if(((int)p->u2.invoke) < 0){
				fprintf(
					stderr,
					"Line %d, event '%s' uses undefined function '%s' in invoke list\n.",
					sym->line, 
					sym->val.text, 
					p->u2.invoke
					);
				return __false;
			}
			// ok, it's valid
			// from now on, the 'invoke' fields is no longer a string
			// but an index into the func array
			break;
		}
	}

	return __true;
}


/*
	see docs/binary.txt
*/
__bool CBblkCompiler::parse_3(FILE *fp)
{	
	int i;
	f8_int	x_order = 0;
	f8_uuid	id0;
	BLK_SERVICE_PROC * bsp = 0;

	fseek(fp, 0, SEEK_SET);

	// header
	m_hdr.magic = BBLK_MAGIC;

	__WRITE(m_hdr);

	__WRITE(m_bhdr);
	
	create_f8_uuid(&id0);

	__WRITE(id0);

	/* m_pins */
	__WRITE_V(m_pins, m_hdr.n_pins);	

	/* m_funcs */
	__WRITE_V(m_funcs, m_hdr.u1.n_funcs);

	ram_size = ftell(fp);

	/* code */	
	fwrite(m_codebuf, 1, m_hdr.u2.n_codes + m_bhdr.n_data, fp);
	
	/* write dynamic pins */
	__WRITE_V(m_dpins, m_hdr.n_pins);

	rom_size = ftell(fp) - ram_size;

	return __true;
}

__bool CBblkCompiler::generate(CSymbol * prg, FILE * fp)
{
	clean();
	
	if( !parse_1(prg) ){
		return __false;
	}
	if( !parse_2(prg) ){
		return __false;
	}
	if( !parse_3(fp) ){
		return __false;
	}

	return __true;
}

static void print_type(int t)
{
#define dd(t) case sym_##t:printf("%s", #t);break;
	switch(t){
		dd(program);
		dd(timer);
		dd(direction);
		dd(vardecl);
		dd(operand);
		dd(id);
		dd(integer_number);
		dd(real_number);
		dd(event_in);
		dd(event_out);
		dd(data_in);
		dd(data_out);
		dd(invoke);
		dd(type);
		dd(constants);
		dd(varlist);
		dd(functionlist);
	default:
		printf("type=%d", t);
		break;
	}
}

static void visit_no_recursive(CSymbol * nd, int k)
{
	for(;k;k--){
		printf("    ");
	}
	printf("line %d,", nd->line);
	if(nd->type == sym_property){
		if(nd->val.text){
			printf("property,%s=%s", ((CProperty*)nd)->name, ((CProperty*)nd)->val.text);
		}else{
			printf("property,%s=%d", ((CProperty*)nd)->name, ((CProperty*)nd)->val.ival);
		}
	}else{
		print_type(nd->type);
		if(nd->val.text){
			printf(",%s", nd->val.text);
		}else{
			if(nd->type == sym_real_number){
				printf(",%f", nd->val.fval);
			}else if(nd->type == TOK_UUID){
				__u16 * u=(__u16*)&nd->val.uuid;
				printf(",%04x-", u[0]);
				printf("%04x-", u[1]);
				printf("%04x-", u[2]);
				printf("%04x", u[3]);
			}else{
				printf(",%d", nd->val.ival);
			}			
		}
	}
	printf("\n");
}

static __bool visit(CTreeNode *_nd, __uint)
{
	CSymbol *nd = (CSymbol*)_nd;
	CSymbol *rt = nd;
	int level = 0;
	while(rt != g_program){
		level++;
		rt = (CSymbol*)rt->GetParent();
	}
	visit_no_recursive(nd, level);
	return __true;
}

static void dump_tree(CSymbol *root)
{
	root->ForEach(visit, __true);
}

int bblk_error(const char * s)
{
	fprintf(stderr, "Syntax error on line %d:%s\n", yyline, s);
	return 0;
}

int bblk_compile()
{
	CBblkCompiler gen;
	__bool ok;

	bblk_in = fopen(input_file, "rt");
	if( !bblk_in ){
		fprintf(stderr, "%s: Can't open.\n", input_file);
		return -1;
	}
	fout = fopen(output_file, "wb");
	if( !fout ){
		fprintf(stderr, "%s: Can't open.\n", output_file);
		fclose(bblk_in);
		return -1;
	}
	if(bblk_parse()){
		return -1;
	}
	if(g_program){
		if(b_tree){
			dump_tree(g_program);
		}
		ok = gen.generate(g_program, fout);
	}
	fclose(fout);
	if(ok){
		int ces_size;
		IFileStream * fs = IFileStream_from_file(output_file, "a+b");
		if(fs){			
			IF8Stream * s = __ucast__(IFileStream, IF8Stream, fs);
			ces_size = gen.ram_size + gen.rom_size;
			save_res_stream(&gen.m_hdr.uuid, s);
			gen.res_size = __vcall__(s, tell, (s)) - ces_size;
			printf(
				"Ok, %d RAM, %d ROM(%d text, %d data, %d pins), %d bss, %d res, total %d.\n",
				gen.ram_size,
				gen.rom_size,
				gen.m_hdr.u2.n_codes,
				gen.m_bhdr.n_data,
				gen.m_hdr.n_pins * sizeof(dyn_pin_t),
				gen.m_bhdr.n_bss,
				gen.res_size,
				__vcall__(s, tell, (s))
				);
			__delete__(fs);
		}
	}

	return 0;
}

static IBBlk * g_curblk;

static __uint _type_size(__u8 type)
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

const char * _direction(int _class)
{
	switch(_class){
	case PIN_CLASS_EI:
	case PIN_CLASS_DI:
		return " in";

	case PIN_CLASS_EO:
	case PIN_CLASS_DO:
		return " out";
	}

	return "";
}

int bblk_disassembly(IBBlk * blk)
{
	int i;
	const char *iname;
	const char *name;
	blk_pin_t * p;
	dyn_pin_t * dp;
	struct blk_hdr_t * m_hdr;
	IBlk * _blk;
	char uuidName[64];

	_blk = __ucast__(IBBlk, IBlk, blk);

	m_hdr = &_blk->h;
	g_curblk = blk;

	iname = IBlk_name(_blk);

	if(!iname){
		iname = "Untitled_block";
	}

	name = get_res_buf_v(
		&_blk->h.uuid,
		0,
		"name"
		);
	if(!name){
		name = "Untitled_class";
	}

	utils_trace("#instance_name %s\n", iname);
	utils_trace("fblock %s;\n", name);	
	utils_trace("attribute(\n");
	if(_blk->h.flags & BLK_F_AUTOSCAN){
		utils_trace("#autoscan_order %d\n", blk->bh.x_order);
		utils_trace("\tautoscan,");
	}
	if(_blk->h.flags & BLK_F_INPUT){
		utils_trace("input,");
	}
	if(_blk->h.flags & BLK_F_OUTPUT){
		utils_trace("output");
	}
	utils_trace("\n);\n");
	
	f8_uuid_to_string(&_blk->h.uuid, uuidName, sizeof(uuidName));
	utils_trace("uuid{%s};\n", uuidName);
	f8_uuid_to_string(&_blk->uuid, uuidName, sizeof(uuidName));
	utils_trace("#instance_uuid{%s};\n", uuidName);

	if(m_hdr->n_pins){
		utils_trace("var\n");
	}
	
	for(i=0, p=blk->pins, dp=blk->dpins; i<m_hdr->n_pins; i++, p++, dp++){
		name = get_res_buf_v(
			&_blk->h.uuid, 
			0,
			"!p%d!name",
			i
			);
		utils_trace(
			"\t%s:%s;%s", 
			name, 
			_type_name(p->type), 
			_direction(p->_class)
			);

		switch(p->binding){
		case PIN_B_CONST:
			utils_trace(",const");
			break;
		case PIN_B_MEMORY:
			utils_trace(
				",mem,%%%d%05d", 
				p->u2.mem_addr.section, 
				p->u2.mem_addr.addr
				);
			break;
			break;
		default:
			utils_trace(",none");
			break;				
		}

		// write attrbutes
		switch(p->_class){
		case PIN_CLASS_EI:
			name = get_res_buf_v(
				&_blk->h.uuid, 
				0,
				"!f%d!name",
				p->u2.invoke
				);
			utils_trace(",invoke(%s)", name);
			utils_trace(";\n");
			break;

		case PIN_CLASS_EO:
			if(p->type == PIN_T_TIMER){
				utils_trace(",timer(%d)", dp->u.e.interval);
			}
			utils_trace(";\n");
			break;

		case PIN_CLASS_DI:
		case PIN_CLASS_DO:
			utils_trace(";\n");
			break;
		}
	}

	for(i=0; i<m_hdr->u1.n_funcs; i++){
		name = get_res_buf_v(
			&_blk->h.uuid, 
			0,
			"!f%d!name",
			i
			);
		utils_trace(
			"function %s; @0x%08x;", 
			name,
			blk->funcs[i].address - (__uint)blk->code_section
			);
		if(blk->funcs[i].flags & FUNC_F_INIT){
			utils_trace("init;");
		}
		if(blk->funcs[i].flags & FUNC_F_FINAL){
			utils_trace("final;");
		}
		utils_trace("\n");
	}
	
	return 0;
}

void _get_cmd_line(char *cmdLine, int size)
{
	char buf[4096];
	GetPrivateProfileString(
		"f8",
		"fcc",
		"cmd /C \"i586-pc-linux-gcc -O9 -fomit-frame-pointer -c -fPIC "
		"-I%pmcf8%/f8/include libfcc-temp.c&flink libfcc-temp.o\"",
		buf,
		sizeof(buf),
		(string(get_working_dir())+"/config/conductor.ini").c_str()
		);
	ExpandEnvironmentStrings(buf, cmdLine, size);
}

IMemStream * x_compile()
{
	char * tempFile = "libfcc-temp.c";
	// now, invoke external compiler to generate .Obj file
	char cmdLine[1024];
	
	utils_trace("Compiling ...\n");

	_get_cmd_line(cmdLine, sizeof(cmdLine));

	unlink("libfcc-temp.obj");
	unlink("libfcc-temp.o");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char fullName[MAX_PATH];
	GetFullPathName(input_file,sizeof(fullName),fullName,0);
	char *p=strrchr(fullName,'\\');
	strcpy(p,"/libfcc-temp.c");
	SetEnvironmentVariable("F8PROG", fullName);

	// utils_debug("Executing : %s\n", cmdLine);
	
	if( !CreateProcess(
		NULL,
		cmdLine,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
		)
		)
	{
		utils_error("Failed spawning '%s '\n", cmdLine);
		return false;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	// gather information in the .obj file to fabricate our m_hdr.m_funcs array
	IMemStream *s;
	s = IMemStream_load_file("libfcc-temp.obj");

#if defined(NDEBUG) && 0
	unlink("libfcc-temp.c");
	unlink("libfcc-temp.obj");
	unlink("libfcc-temp.o");
#endif

	return s;
}

struct func_t{
	int name;
	int address;
};

static bool isCaredFunction(const char * name)
{
	if(!strcmp(name, "main") || !strcmp(name, "_main")){
		return true;
	}if(!strcmp(name, "on_start") || !strcmp(name, "_on_start")){
		return true;
	}if(!strcmp(name, "on_stop") || !strcmp(name, "_on_stop")){
		return true;
	}
	return false;
}

/*
	generate code buffer for current BBLK
*/
int CBblkCompiler::gen_funcs(IMemStream * s)
{
	struct blk_hdr_t * more_hdr;
	struct bblk_hdr_t * more_bhdr;
	int i;
	char * str_heap;
	char * name;
	func_t * more_funcs, * fnc;
	int * heap_size;
	int j;
	char * codes;

	more_hdr = (struct blk_hdr_t*)s->buffer;
	if(s->length < sizeof(blk_hdr_t)){
		fprintf(stderr, "Invalid block template.\n");
		return -1;
	}
	if(m_hdr.u1.n_funcs + more_hdr->u1.n_funcs > MAX_BBLK_ITEMS){
		fprintf(stderr, "Too many functions.\n");
		return -1;
	}
	more_bhdr = (struct bblk_hdr_t*)&more_hdr[1];
	more_funcs = (func_t*)&more_bhdr[1];
	heap_size = (int*)(more_funcs + more_hdr->u1.n_funcs);
	str_heap = (char*)heap_size + sizeof(int);
	codes = str_heap + *heap_size;
	if(codes + more_bhdr->n_data + more_hdr->u2.n_codes !=
		s->buffer + s->length
		)
	{
		fprintf(stderr, "Invalid block template 2.\n");
		return -2;
	}

	/* for 1131 blocks, we add the main function first */
	if(m_hdr.flags & BLK_F_AUTOSCAN){
		for(i=more_hdr->u1.n_funcs, fnc=more_funcs; i; i--, fnc++){
			name = str_heap + fnc->name;
			if(!strcmp(name, "main") || !strcmp(name, "_main")){
				break;
			}
		}
		if(!i){
			fprintf(
				stderr, 
				"Error : main() must be defined for autoscan blocks.\n"
				);
			return -1;
		}
		add_function(name, fnc->address);
	}
	
	for(i=more_hdr->u1.n_funcs, fnc=more_funcs; i; i--, fnc++){
		name = str_heap + fnc->name;
		j = find_function(name);
		if(j >= 0){
			m_funcs[j].address = fnc->address;
		}else{
			if(isCaredFunction(name)){
				add_function(name, fnc->address);
			}
		}
	}
	m_hdr.u2.n_codes = more_hdr->u2.n_codes;
	m_bhdr.n_data = more_bhdr->n_data;
	m_bhdr.n_bss = more_bhdr->n_bss 
		- sizeof(BLK_SERVICE_PROC*) 
		- sizeof(dyn_pin_t)*m_hdr.n_pins
		;
	memcpy(m_codebuf, codes, more_bhdr->n_data + more_hdr->u2.n_codes);

	return 0;
}

/*
	test code,
	generate LCC-compilable code
*/
__bool CBblkCompiler::load_implementation(CSymbol * imple)
{
	int i;
	char *name;
	int  retval;
	char lcc_prolog[]={
		"#define FCC_INVOKED\n"
		"#include <f8_kprocs.h>\n"
		"\n"		
	};
	char lcc_prolog2[]={
		"#define FCC_INVOKED\n"
		"#include <f8.h>\n"
		"\n"		
	};

	char * tempFile = "libfcc-temp.c";
	FILE * fp = fopen(tempFile, "wt");
	if(!fp){
		fprintf(stderr, "%s : cannot open.\n", tempFile);
		return __false;
	}

	fprintf(fp, "%s\n", lcc_prolog);
	if(m_hdr.flags & BLK_F_SYSTEM)
		fprintf(fp,"%s\n",lcc_prolog2);

	fprintf(fp, "static struct pin __dpins__[%d];\n", m_hdr.n_pins);
	fprintf(fp, "static BLK_SERVICE_PROC * __blk_services__;\n");

	for(i=0; i<this->m_hdr.n_pins; i++){
		name = get_res_buf_v(
			&m_hdr.uuid, 
			0,
			"!p%d!name",
			i
			);
		if(m_pins[i].type == PIN_T_EVENT || m_pins[i].type == PIN_T_TIMER){
			fprintf(
				fp,
				"#define %s ((struct event_pin *)%d)\n",
				name,
				i
				);
			
		}else{
			fprintf(
				fp, 
				"#define %s ((&__dpins__[%d])",
				name,
				i
				);
			switch(this->m_pins[i].type){
			case PIN_T_CHAR:
			case PIN_T_BOOL:
				fprintf(fp, "->i8");
				break;
			case PIN_T_SHORT:
				fprintf(fp, "->i16");
				break;
			case PIN_T_INTEGER:
				fprintf(fp, "->i32");
				break;
			case PIN_T_BYTE:
				fprintf(fp, "->ui8");
				break;
			case PIN_T_WORD:
				fprintf(fp, "->ui16");
				break;
			case PIN_T_DWORD:
				fprintf(fp, "->ui32");
				break;
			case PIN_T_FLOAT:
				fprintf(fp, "->flt");
				break;
			case PIN_T_DOUBLE:
			case PIN_T_DATE:
				fprintf(fp, "->dbl");
				break;
			default:
				assert(0);
			}
			
			if(this->m_pins[i]._class == PIN_CLASS_DI){
				/* make the expression R-value only */
				fprintf(fp, " + 0");
			}
			
			fprintf(fp, ")\n");
		}		
	}	
	
	fprintf(fp, "\n");
	{
		char * bibi;
		bibi = input_file;
		while(*bibi++){
			if(*bibi == '\\'){
				*bibi = '/';
			}
		}
	}
	fprintf(
		fp,
		"#line %d \"%s\"\n",
		_get_node_by_type(g_program, TOK_IMPLEMENTATION)->line - 1,
		input_file
		);
	fprintf(fp, "%s", imple->val.text);
	fprintf(fp, "\n");

	fclose(fp);

	IMemStream * ms;
	ms = x_compile();
	if(!ms){
		// fprintf(stderr, "compiler invocation failed.\n");
		return __false;
	}
	retval = gen_funcs(ms);
	
	__delete__(ms);
	
	return retval ? __false : __true;
}

__bool CBblkCompiler::add_function(const char * name, int address)
{
	blk_func_t * func;
	if(m_hdr.u1.n_funcs >= MAX_BBLK_ITEMS){
		return __false;
	}
	set_res_buf_v(
		&m_hdr.uuid,
		name,
		strlen(name) + 1,
		"!f%d!name",
		m_hdr.u1.n_funcs
		);
	func = m_funcs + m_hdr.u1.n_funcs;
	func->address = address;
	if(!strcmp(name, "on_start") || !strcmp(name, "_on_start")){
		func->flags |= FUNC_F_INIT;
	}else if(!strcmp(name, "on_stop") || !strcmp(name, "_on_stop")){
		func->flags |= FUNC_F_FINAL;
	}
	m_hdr.u1.n_funcs++;
	return __true;
}
