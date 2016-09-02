/*
	F8 field bus control logic program compiler.
	jackie, 2003/3/18
*/
#include "precomp.h"
#include "math.h"

#pragma comment (lib, "librtk.lib")
#pragma comment (lib, "libutils.lib")
#pragma comment (lib, "libcobj.lib")
#pragma comment (lib, "libCPres.lib")
#pragma comment (lib, "libfbd.lib")
#pragma comment (lib, "libfbdar.lib")

int yyline=1;
CSymbol * g_program;
char * input_file;
char * output_file;
char * symbol_file;

FILE * fout, * fsym;
char * g_szLibPath=".";
__bool b_tree = __false;;
__bool b_disassembly = __false;
__bool b_recursively = __false;
__bool b_dumpGrammar = __false;

#ifndef YYDEBUG
int bblk_debug, cblk_debug;
#endif

CSymbol * _get_node_by_type(CSymbol *root, int type)
{
	CSymbol *child;
	child = (CSymbol*)root->GetFirstChild();
	while(child){
		if(child->type == type){
			return child;
		}
		child = (CSymbol*)child->GetNextSibling();
	}
	return NULL;
}

static __u16 get_source_type(const char * file)
{
	__u16 ftype = 0;
	FILE *fp;
	fp = fopen(file, "rt");
	if( !fp ){
		fprintf(stderr, "Cannot open %s for reading.\n", file);
		return 0;
	}
	char line[1024];
	while( !feof(fp) ){
		memset(line, 0, sizeof(line));
		if( !fgets(line, sizeof(line), fp) ){
			break;
		}
		CArgsEx args;
		args.parse(line, " \t\r\n");
		if( !args.argc ){
			continue;
		}
		// ok, this is the first non-empty line of the source file
		// it must be a comment line or else a program header line
		if(args.argv[0][0] == '#'){
			continue;
		}
		if( !strcmpi(args.argv[0], "fblock") ){
			ftype = BBLK_MAGIC;
			break;
		}
		if( !strcmpi(args.argv[0], "cblock") ){
			ftype = CBLK_MAGIC;
			break;
		}
		break;
	}
	fclose(fp);
	return ftype;
}

static int get_binary_type(const char * file)
{
	__u16 ftype = 0;
	FILE *fp;
	fp = fopen(file, "rb");
	if( !fp ){
		fprintf(stderr, "Cannot open %s for reading.\n", file);
		return 0;
	}
	fread(&ftype, 1, sizeof(ftype), fp);
	fclose(fp);
	return ftype;
}

FCC_API int blk_disassembly(IBlk * blk)
{
	return blk_disassembly_ex(blk, f8_false);
}

static char *disbuf;
static int dislen;

int RTK_API _puts(MSG_LEVEL, char * s)
{
	int l;
	l = sprintf(disbuf, "%s", s);
	disbuf += l;
	return l;
}

FCC_API int blk_disassembly_b(IBlk * blk, char * buf, int len)
{
	int (UTILS_API *oldputs)(MSG_LEVEL, char *) = utils_puts;
	disbuf = buf;
	dislen = len;
	utils_puts = _puts;
	blk_disassembly(blk);
	utils_puts = oldputs;
	return 0;
}

extern void dumpBblkGrammar();
extern void dumpCblkGrammar();

FCC_API int fcc_main_v(int c, char **v)
{
	char _outputfile[F8_MAX_PATH];
	char _symbolfile[F8_MAX_PATH];
	__bool yaccDebug = __false;
	
	BEGIN_ARGS
		ARG("-i:", input_file, "char *", "input file.")
		ARG("-o:", output_file, "char *", "output file.")
		ARG("-s:", symbol_file, "char *", "symbol file.")
		ARG("-r", b_recursively, "tbool", "disassembly recursively")
		ARG("-dis", b_disassembly, "tbool", "disassembly input file.")
		ARG("-dg", b_dumpGrammar, "tbool", "show grammar.")
		ARG("-tree", b_tree, "tbool", "show parse tree.")
		ARG("-dy", yaccDebug, "tbool", "show yacc parse internals.")
		ARG("-L:", g_szLibPath, "char *", "function block library path.")
	END_ARGS

	__u16 ftype;
	IBlk * blk;

	b_tree = __false;;
	b_disassembly = __false;
	b_recursively = __false;
	b_dumpGrammar = __false;

	if( process_args_1(c, v) < 0){
		print_usage_1();
		return -1;
	}	

	if(!GetEnvironmentVariable("PMCF8", _symbolfile, sizeof(_symbolfile))){
		SetEnvironmentVariable("PMCF8", get_exe_dir());
	}

	if(b_dumpGrammar){
		utils_trace("Basic block grammar:\n");
		dumpBblkGrammar();
		utils_trace("Composite block grammar:\n");
		dumpCblkGrammar();
	}
	
	if(!input_file){
		utils_error("fcc: No input file.\n");
		return -1;
	}

	if(b_disassembly){
		blklib_load_by_file(input_file, &blk);
		if( !blk ){
			utils_error("Cannot load %s.\n", input_file);
			return -1;
		}
		blk_disassembly_ex(blk, b_recursively);
		delete_blk(blk);
		return 0;
	}
	
	if(!output_file){
		output_file = _outputfile;
		if( !strnicmp(input_file + strlen(input_file) - 2, ".f", 2) ){
			strcpy(output_file, input_file);
			output_file[strlen(output_file) - 2] = 0;			
		}else{
			strcpy(output_file, input_file);
		}
		strcat(output_file, ".blk");
	}
	
	if(!symbol_file){
		symbol_file = _symbolfile;
		if( !strnicmp(input_file + strlen(input_file) - 2, ".f", 2) ){
			strcpy(symbol_file, input_file);
			symbol_file[strlen(symbol_file) - 2] = 0;			
		}else{
			strcpy(symbol_file, input_file);
		}
		strcat(symbol_file, ".sym");
	}

	bblk_debug = cblk_debug = yaccDebug;

	printf("%s\n", input_file);
	ftype = get_source_type(input_file);
	switch(ftype){
	case BBLK_MAGIC:
		return bblk_compile();
		break;
	case CBLK_MAGIC:
		return cblk_compile();
		break;
	default:
		fprintf(stderr, "Cannot determine source type(missing `fblock' or `cblock').");
		break;
	}

	return -1;
}

FCC_API int fcc_main(const char * v)
{
	CArgs a;

	parse_arg_ex(v, &a);
	return fcc_main_v(a.argc, a.argv);
}

__bool _save_res(const f8_uuid * id)
{
	char resfile[F8_MAX_PATH];

	f8_uuid_to_string(id, resfile, sizeof(resfile));
	strncat(resfile, ".res", sizeof(resfile));
	resfile[sizeof(resfile) - 1] = 0;
	IFileStream * fs = IFileStream_from_file(resfile, "wb");
	if(!fs){
		fprintf(stderr, "Error opening %s for writing.\n", resfile);
		return __false;
	}
	IF8Stream *s = __ucast__(IFileStream, IF8Stream, fs);
	__bool ret = save_res_stream(id, s);
	__delete__(fs);

	return ret;
}

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

FCC_API int blk_disassembly_ex(
	IBlk * blk,
	f8_bool bRecursively
	)
{
	switch(blk->h.magic){
	case BBLK_MAGIC:
		return bblk_disassembly(__dcast__(IBlk, IBBlk, blk));		

	case CBLK_MAGIC:
		return cblk_disassembly(
			__dcast__(IBlk, ICBlk, blk), 
			bRecursively
			);
	}

	return 0;
}
