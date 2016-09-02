/*
	field bus controller simulator
	jackie, jhunter@tsinghua.org.cn
	2003/3/24
*/
#ifdef _WIN32
#include <winsock2.h>
#endif

#include <f8.h>
#include <f8_kserver.h>
#include <crtdbg.h>

#include <utils.h>

#pragma comment (lib, "libfkern.lib")
#pragma comment (lib, "libutils.lib")

static IBlk * sections;
static HF8_KERNEL g_kernel;

/*
	in WIN32, the executive can run in two mode, i.e., the SoftPLC
	mode or the simulator mode. in SoftPLC mode, the executive will
	try to probe additional IO drivers and perform IOs during execution,
	while in simulator mode, no IO is performed.
*/
KSERVER_API int kserver_main2(int c, char **v)
{
	int flags;
	char * mode = 0;
	int port = 0;
	__bool dbgMode;
	int leakDetect = 0;
	
	BEGIN_ARGS
		ARG("mode=", mode, "char *", "mode specifiers")
		ARG("port=", port, "int", "agent port")
		ARG("leak=", leakDetect, "int", "detect memory leakage")
		ARG("--debug", dbgMode, "tbool", "debug mode")
	END_ARGS

	if(process_args_1(c, v)<0){
		print_usage_1();
		return F8_SYNTAX;
	}
	
	if(leakDetect){
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetBreakAlloc(leakDetect);
	}
	
	if(mode){
		CArgs a;
		parse_arg_ex(mode, &a, "+/|-= ,;:");
		int i;
		flags = 0;
		for(i=0; i<a.argc; i++){
			if(!strcmp(a.argv[i], "sched")){
				flags |= KERN_F_ENABLE_SCHEDULER;
			}else if(!strcmp(a.argv[i], "standby")){
				flags |= KERN_F_ENABLE_STANDBY;
			}else if(!strcmp(a.argv[i], "io")){
				flags |= KERN_F_ENABLE_IO;
			}else if(!strcmp(a.argv[i], "sio")){
				flags |= KERN_F_SERIALIZED_IO;
			}else if(!strcmp(a.argv[i], "agent")){
				flags |= KERN_F_ENABLE_AGENT;
			}else if(!strcmp(a.argv[i], "single")){
				flags = KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_IO | KERN_F_ENABLE_AGENT;
				break;
			}else if(!strcmp(a.argv[i], "all")){
				flags = KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_IO | KERN_F_ENABLE_AGENT
					| KERN_F_ENABLE_STANDBY;
				break;
			}else if(!strcmp(a.argv[i], "headless")){
				flags = KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_AGENT | KERN_F_ENABLE_STANDBY;
				break;
			}
		}
	}else{
		flags = KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_IO | KERN_F_ENABLE_AGENT
			| KERN_F_ENABLE_STANDBY;
	}

	ex_init();
	g_kernel = ke_get_default_kernel();
	ex_start(flags, port);
	sections = ke_get_blk(g_kernel, NULL);

	return F8_SUCCESS;
}

KSERVER_API f8_status kserver_main(const char * cmdline)
{
	CArgs a;
	parse_arg_ex(cmdline, &a, " \t\n");
	return kserver_main2(a.argc, a.argv);
}

KSERVER_API __bool kserver_trivial()
{
	// stop user program first
	stop_kserver();
	// then stop executive
	ex_stop();
	// destroy executive
	ex_uninit();

	return __true;
}


/*
	a wrapper for the F8_START command packet
*/
KSERVER_API __bool start_kserver()
{
	IBlk *t;
	char buf[sizeof(kern_conf_cmd) + sizeof(f8_uuid)];
	struct kern_conf_cmd *cmd;
	
	t = sections;
	cmd = (kern_conf_cmd*)buf;
	memset(cmd, 0, sizeof(kern_conf_cmd));
	cmd->cmd = F8_START;
	OLD_ID(cmd) = t->h.uuid;
	NEW_ID(cmd) = BLK_ID_NONE;
	cmd->isize = sizeof(f8_uuid);
	cmd->ibuffer = (char*)&cmd[1];
	ke_config(g_kernel, cmd, __true);

	return __true;
}

/*
	a wrapper for the F8_STOP command packet
*/
KSERVER_API __bool stop_kserver()
{
	IBlk *t;
	char buf[sizeof(kern_conf_cmd) + sizeof(f8_uuid)];
	struct kern_conf_cmd *cmd;
	
	t = sections;
	
	cmd = (kern_conf_cmd*)buf;
	memset(cmd, 0, sizeof(kern_conf_cmd));
	
	cmd->cmd = F8_STOP;
	OLD_ID(cmd) = t->h.uuid;
	NEW_ID(cmd) = BLK_ID_NONE;
	
	cmd->isize = sizeof(f8_uuid);
	cmd->ibuffer = (char*)&cmd[1];

	ke_config(g_kernel, cmd, __true);

	return __true;
}
