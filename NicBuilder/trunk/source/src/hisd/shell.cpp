#include "precomp.h"
#pragma hdrstop
#include <shell.h>

class CHisdShell : public CRtkThread{
public:
	virtual __uint run();
	void init();
	void uninit();
	void execute(PCRTK_PACKET d);
	
	static __bool bExit;
	static HRTK_SHELL shell;
	static CRWLock lock;
};

static CHisdShell hisdShell;
HRTK_SHELL CHisdShell::shell;
CRWLock CHisdShell::lock;
__bool CHisdShell::bExit;

static void listConfig()
{
#define _show(name, vname)\
	utils_trace("%s = 0x%08x - [ %d ]\n", #name, vname, vname);
	
	_show(RecordInterval, hislogRecordInterval)
	_show(ArchiveTimeSpan, hislogArchiveTimeSpan)
	_show(AutoSaveInterval, hislogAutoSaveInterval)
	_show(MaxArchiveSize, hislogMaxArchiveSize)
	_show(QueryThreadPoolSize, hislogNumQueryThreads)
}

/*
	list buffers
*/
int on_list(char * v, void * p)
{
	int id;
	CArgs args;
	
	//extern void dbgListBuffers(int n);

	parse_arg_ex(v, &args);
	
	if(args.argc >= 2){
		if(!strcmpi(args.argv[1], "config")){
			listConfig();
			return 0;
		}
		id = atoi(args.argv[1]);
	}else{
		id = -1;
	}
	
	//dbgListBuffers(id);
	
	return 0;
}

int on_exit(char *v, void *p)
{
	SetEvent(g_service->m_EvtExit);
	CHisdShell::bExit=__true;
	return 0;
}

int on_restart(char * v, void * p)
{
	SetEvent(g_service->m_EvtConfigChanged);
	return 0;
}

static shell_command cmds[] = 
{
	{"list", "list buffers or config", on_list},
	{"exit", "exit archive subsystem", on_exit},
	{"restart", "restart archive subsystem", on_restart},
	{0, 0, 0}
};

void CHisdShell::init()
{
	lock.WriteLock();
	shell = shell_create(cmds);
	lock.Release();
}

void CHisdShell::uninit()
{
	lock.WriteLock();
	shell_destroy(shell);
	shell = 0;
	lock.Release();
}

__uint CHisdShell::run()
{
	char line[20480];

	while(!bExit){
		printf("# ");
		fgets(line, sizeof line, stdin);
		shell_execute(shell,line,this);
	}
	
	return 0;
}

void CHisdShell::execute(PCRTK_PACKET d)
{
	if(!shell){
		return;
	}
	lock.ReadLock();
	shell_execute(shell, d->data, (void*)d);
	lock.Release();
}

extern __bool hisdConsoleShell;

void initDebugShell()
{
	hisdShell.init();
	if(hisdConsoleShell){
		hisdShell.start();
	}
}

void uninitDebugShell()
{
	if(hisdConsoleShell){
		hisdShell.stop(INFINITE);
	}
	hisdShell.uninit();
}

void hislogDebugShell(PCRTK_PACKET d)
{
	hisdShell.execute(d);
}

