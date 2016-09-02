/*
	field bus controller configuration shell
	jackie, jhunter@tsinghua.org.cn
	2003/3/24
*/
#include "precomp.h"
#pragma hdrstop

#include <crtdbg.h>

#pragma comment (lib, "libfbd.lib")
#pragma comment (lib, "libfbdar.lib")
#pragma comment (lib, "libfkern.lib")
#pragma comment (lib, "libfcc.lib")
#pragma comment (lib, "libCPres.lib")
#pragma comment (lib, "libkadapter.lib")
#pragma comment (lib, "libcobj.lib")
#pragma comment (lib, "libutils.lib")
#pragma comment (lib, "librtkshell.lib")
#pragma comment (lib, "librtk.lib")
#pragma comment (lib, "librtknet.lib")

char * g_szLibPath=".";
static HRTK_SHELL g_shell;

KPROXY_API IKAdapter * proxy_adapter;
KPROXY_API __bool proxy_silent;
char	*x_buffer;
__uint	x_buf_len;
__bool	b_ShellExit;
char	yy_buffer[1024];
int		yy_pointer;
char	fileName[F8_MAX_PATH];
string	g_sProjectPath;
IBlk	*sections;
__bool	g_bDirty;
HVBUS vbus;

static int on_test(char * cmd, void * k);
int on_cpu(char *v, void * k);
int on_lock(char *,void*);
int on_unlock(char *,void*);
int on_sync(char *,void*);
int on_check(char *,void*);
int on_editvar(char *,void*);
int on_targetexec(char *,void*);

static struct shell_command commands[]={
	{"q", "exit simulator.", on_exit},
	{"help", "show available commands.", on_help},
	{"?", "show available commands.", on_help},

	{"open", "open project.", on_open},
	{"create", "create project", on_create},
	{"close", "open project.", on_close},
	{"save", "save project", on_save},
	{"online", "go online", on_online},
	{"offline", "go offline", on_offline},	
	{"uload", "upload configuration", on_uload},
	{"dload", "download configuration", on_dload},
	{"check","check program", on_check},
	{"fix", "fix project", on_fix},
	{"sync","keep x-memory synchronized with target.", on_sync},

	{"ver", "show local and target version", on_ver},

	{"mount", "insert a block into an task.", on_mount},
	{"umount", "delete a block from an task.", on_umount},
	{"cp", "copy a block to a new position", on_cp},
	{"mv", "move or rename a block", on_mv},
	{"lib", "save block in library", on_entlib},

	{"reset", "reset controller.", on_reset},
	{"start", "start controller", on_start},
	{"stop", "stop controller", on_stop},
	{"break", "pause controller", on_break},
	{"continue", "continue controller", on_continue},
	{"suspend", "suspend controller", on_suspend},
	{"resume", "resume controller", on_resume},
	{"lock","lock controller(prevent other configurator) ", on_lock},
	{"unlock","uclaim configurator", on_unlock},
	
	{"link", "make connection", on_link},
	{"ulink", "delete connection", on_ulink},

	{"exp", "export a pin", on_exp},
	{"uexp", "unexport a pin", on_uexp},
	{"bind", "set pin binding", on_bind},
	{"ubind", "remove pin memory binding.", on_ubind},
	{"invert", "invert boolean pin", on_invert},
	
	{"p", "print pin value", on_print},
	{"pp", "print pin value", on_pp},
	{"ls", "list subblocks.", on_ls},
	{"u", "unassembly block", on_disasm},
	{"mmap", "show memory map", on_mmap},
	{"d", "dump exchange memory", on_dump},
	{"vres", "dump resource", on_dump_res},
	{"error", "show error message for error number.", on_error},
	{"whouses", "find out who uses the variable", on_whouses},
	{"whoowns", "find out who owns the address", on_whoowns},
	{"log", "show or clear the target log buffer", on_log},

	{"set", "set variable", on_set},
	{"uset", "unset variable", on_uset},
	{"lv", "list variables", on_lv},
	{"modify", "modify variable", on_modify},
	{"editvar","rename variable and comment", on_editvar},

	{"device", "view/modify device configuration", on_device},
	{"memsize", "view/modify memory section sizes", on_memsize},
	{"memusage", "view memory section usage", on_memusage},
	{"station", "view station statistics", on_station},
	{"cpu","view cpu status", on_cpu},

	{"play", "play script commands", on_play},
	{"place", "place block or links", on_place},
	{"export", "export project in text format", on_export},
	{"targetexec","execute command directly on target.",on_targetexec},
	
	{"test", "test", on_test},

	{0,0,0},
};

static int on_test(char * arg, void * k)
{
	int i;
	int count=10;
	char * cmd = "uload";
	BEGIN_ARGS
		ARG("-c:", count, "int", "count")
		ARG("-a:", cmd, "char *", "command to stress")
	END_ARGS
	CArgs args;
	parse_arg_ex(arg, &args, FS);
	if(process_args_1(args.argc, args.argv) < 0){
		print_usage_1();
		return F8_SYNTAX;
	}
	for(i=0; i<count; i++){
		kproxy_shell(cmd);
	}
	return 0;
}

string _blk_path(IBlk * b)
{
	char path[F8_MAX_PATH];
	IBlk_path_by_blk(b, path, sizeof(path));
	path[sizeof(path) - 1] = 0;
	return path;
}

string _pin_path(struct blk_pin_t * p)
{
	char path[F8_MAX_PATH];
	IBlk_path_by_pin(p, path, sizeof(path));
	path[sizeof(path) - 1] = 0;
	return path;
}

int on_exit(char *, void *k)
{
	b_ShellExit = __true;
	return 0;
}

int on_help(char * v, void *k)
{
	CArgsEx b;
	int i;
	int count;

	count = sizeof(commands)/sizeof(commands[0]) - 1;

	b.parse(v, FS);
	if(b.argc == 1){
		for(i=0; i<count; i++){
			utils_trace(
				"\t%10s : %s\n", 
				commands[i].name, 
				commands[i].brief_description
				);
		}
	}else{
		for(i=1; i<b.argc; i++){
			int j;
			for(j=0; j<count; j++){
				if(strcmp(commands[j].name, b[i])){
					continue;
				}
				utils_trace(
					"\t%10s : %s\n", 
					commands[j].name, 
					commands[j].brief_description
					);
				break;
			}
			if(j >= count){
				// not found
				utils_error("%s : not found.\n", b[i]);
			}
		}
	}

	return F8_SUCCESS;
}

KPROXY_API f8_int kproxy_shell(const char * cmd)
{
	return kproxy_shell_ex(cmd, 2000, 0);
}

KPROXY_API f8_int kproxy_shell_ex(
	const char * cmd, 
	__uint timeout, 
	void * reserved
	)
{
	f8_uuid	oldUuid;
	struct proxy_shell_param psp;
	int code;
	CArgsExA a;
	int i;

	oldUuid = sections->h.uuid;
	a.parse(cmd, ";", 256, strlen(cmd) + 1, __true);
	for(i=0; i<a.argc; i++){
		psp.timeout = timeout;	
		if(!shell_execute_ex(g_shell, a.argv[i], &psp, &code)){
			code = F8_UNKNOWN_COMMAND;
		}
	}
	
	if(F8_FAILED(code)){
		utils_error(
			"<* PROXY_SHELL:`%s' returns %d(%s) *>\n", 
			cmd, 
			code, 
			_errmsg(code)
			);
	}else if(!proxy_silent){
		utils_debug(
			"<* PROXY_SHELL:`%s' returns %d(%s) *>\n", 
			cmd,
			code, 
			_errmsg(code)
			);
	}

/* if executing inside on_play(), no autosave is made as that will
degrade performance */
	if(!proxy_silent)
		autoSave(0,0);
	
	if(oldUuid != sections->h.uuid){
		move_resource(&oldUuid, &sections->h.uuid);
	}
	
	return code;
}

KPROXY_API f8_int kproxy_shell_v(
	const char * cmd
	...
	)
{
	char ss[4096];
	va_list arg;
	va_start(arg, cmd);
	_vsnprintf(ss, sizeof(ss), cmd, arg);
	va_end(arg);
	return kproxy_shell(ss);
}

static void RTK_API sr(PCRTK_PACKET p, HVBUS bus)
{
#if 0
	{
	char name[128];
	p->guid.to_string(name);
	DEBUG_PRINTF((
		"Got packet, ID=%s:%d/%d/%d, Type:0x%08x, "
		"Src=%s:%d, Size=%d, Offset=%d\n",
		name, 
		p->grp_id,
		p->frgmt_id, 
		p->total_frag, 
		p->packet_type,
		p->src.host.Data, 
		p->src.port,
		p->data_size, 
		p->offset
		));
	}
#endif

	send_rtk_data_with_guid(
		bus, 
		&p->src, 
		&p->guid, 
		p->packet_type | PF_ACK, 
		0,
		0
		);
}

KPROXY_API f8_bool kproxy_init(int leakDetect)
{
	ITcpAdapter *a;	

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(leakDetect);
	
	/* initialize and start kernel */
	ex_init();
	
	/* KERN_F_ENABLE_INPUT */

	x_buffer = (char *)__malloc__(LARGE_BUF_SIZE);
	x_buf_len = LARGE_BUF_SIZE;
	b_ShellExit = __false;	

	__new__(ITcpAdapter, a);
	proxy_adapter = __ucast__(ITcpAdapter, IKAdapter, a);

	sections = ke_get_blk(proxy_adapter->kernel, 0);
	assert(sections->uuid == BLK_ID_ROOT);

	create_resource(&sections->uuid);
	create_f8_uuid(&sections->h.uuid);
	create_resource(&sections->h.uuid);

	memset(fileName, 0 , sizeof(fileName));

	g_shell = shell_create(commands);
	
	reset_reg_map();
	init_blklib();
	init_clipboard();

	g_bDirty = __false;


	init_network(RTK_INIT_AS_SERVER, 0);
	vbus = connect_vbus(0xf8, VBUS_CONNECT_AS_SERVER, sr, 0);
	
	return __true;
}

KPROXY_API void kproxy_uninit()
{
	uninit_blklib();
	uninit_network();
	
	if(proxy_adapter){
		on_offline(0, 0);
		delete_adapter(proxy_adapter);
		proxy_adapter = 0;
	}
	if(x_buffer){
		__free__(x_buffer);
		x_buffer = 0;
	}
	
	ex_uninit();
}

KPROXY_API HF8_KERNEL kproxy_get_kernel()
{
	return proxy_adapter->kernel;
}

KPROXY_API IKAdapter * kproxy_get_adapter()
{
	return proxy_adapter;
}

