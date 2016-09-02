#ifndef __global_h__
#define __global_h__

#define YYDEBUG 1

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#pragma warning(disable: 4786)

#include <malloc.h>
#include <assert.h>
#include <map>
#include <string>

#pragma warning(disable: 4786)

using namespace std;

#include <rtk_object.h>
#include <f8.h>

extern int bblk_debug;
extern char * bblk_text;
extern FILE * bblk_in;
extern int yyline;
extern int cblk_debug;
extern char * cblk_text;
extern FILE * cblk_in;
extern char * g_szLibPath;

extern char * input_file;
extern char * output_file;
extern char * symbol_file;
extern FILE * fout;
extern FILE * fsym;
extern __bool b_tree;
extern __bool b_disassemby;


extern int bblk_parse (void);
extern int bblk_lex(void);
extern int bblk_error(const char *);

extern int cblk_parse (void);
extern int cblk_lex(void);
extern int cblk_error(const char *);
extern __bool type_from_name(const char * text, int * token);
extern string fblock_file_by_type(const char * type);

int bblk_compile();
int cblk_compile();
int bblk_disassembly(IBBlk *);
int cblk_disassembly(ICBlk *, f8_bool bRecursively);

class CSymbol;
#define YYSTYPE CSymbol *

#include "bblk_yy.hxx" 
#include "cblk_yy.hxx"

enum NODE_TYPE{
	sym_program,
	sym_event,
	sym_timer,
	sym_direction,
	sym_vardecl,
	sym_property,
	sym_invoke,
	sym_type,
	sym_const,
	sym_function,
	sym_operand,
	sym_id,
	sym_integer_number,
	sym_real_number,
	sym_constants,
	sym_varlist,
	sym_functionlist,

	sym_event_in,
	sym_event_out,
	sym_data_in,
	sym_data_out,
};
 
#define fixup_var		(0x1L<<3)
#define fixup_label		(0x1L<<5)
#define fixup_function	(0x1L<<8)

typedef long blk_fixup_type;

struct blk_fixup_t{
	blk_fixup_type type;
	f8_u32	index;
	f8_ptr	address;
}PACKED;

class CSymbol : public CTreeNode
{
public:
	int type;
	CSymbol(int t)
	{
		type = t;
		sibling = 0;
		memset(&val, 0, sizeof(val));
		line = yyline;
	}
	~CSymbol()
	{
		if(val.text){
			free(val.text);
		}
	}
	// overload of CTreeNode::AddChild
	void AddChild(CSymbol * child)
	{
		CSymbol * nd;
		CTreeNode::AddChild(0, child);
		nd = child;
		while(nd->sibling){
			CTreeNode::AddChild(0, nd->sibling);
			nd = nd->sibling;
		}
	}
	CSymbol * AddSibling(CSymbol * sib)
	{
		CSymbol * n;
		if( !sibling ){
			sibling = sib;
			return sib;
		}
		n = sibling;
		while(n->sibling){
			n = n->sibling;
		}
		n->sibling = sib;
		return sib;
	}
	CSymbol * sibling;
	struct value_t{
		int		ival;
		float	fval; 
		char	*text;
		f8_uuid	uuid;
	}val;
	int line;
};

class CProperty : public CSymbol
{
public:
	CProperty(const char * n, const char * v) : CSymbol(sym_property)
	{
		name = strdup(n);
		if(v){
			val.text = strdup(v);
		}else{
			val.text = NULL;
		}
		b_allocated = true;
	}
	CProperty(const char * n, int ival) : CSymbol(sym_property)
	{
		name = strdup(n);
		this->val.ival = ival;
		b_allocated = false;
	}
	~CProperty()
	{
		if(b_allocated && val.text){
			free(val.text);
		}
		free(name);
	}
	bool b_allocated;
	char * name;
};

typedef map<string,int> s_i_map;

extern s_i_map g_fblocknames;

extern CSymbol * g_program;

extern char * g_szTempDir;

__bool _save_res(const f8_uuid * id);
void _dump_res(const f8_uuid * id);
CSymbol * _get_node_by_type(CSymbol *root, int type);

void save_res_to_file_stream(f8_uuid * id, FILE * fp);

#endif
