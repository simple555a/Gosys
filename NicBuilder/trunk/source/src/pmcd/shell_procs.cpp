/*
	PC based Monitoring	System source file
	version	1.0
	by J.Hunter,jhunter@263.net,May	2001
*/
#include "precomp.h"

extern int on_debug(char *,void	*);
extern int on_trace(char *,void	*);
extern int on_error(char *,void	*);
extern int on_restart(char *,void *);
extern int on_exit(char	*,void *);
extern int on_reloadhis(char *,void*);
extern int on_reset_spac(char *, void*);
extern int on_switch(char *, void*);
extern int on_rename(char *, void *);
extern int on_commitlog(char *, void *);

#define	_cmd_(c) {#c,#c,on_##c},

static shell_command _valid_cmds[]={
	_cmd_(debug)
	_cmd_(trace)
	_cmd_(error)
	_cmd_(restart)
	_cmd_(exit)
#ifdef _WIN32
	_cmd_(reloadhis)
	_cmd_(reset_spac)
#endif
	_cmd_(switch)
	_cmd_(rename)
	_cmd_(commitlog)
	//_cmd_()
	//_cmd_()
	//_cmd_()
	{000}
};

HRTK_SHELL g_shell;

//´´½¨g_shell
__bool init_server_shell(void)
{
	g_shell	= (HRTK_SHELL)shell_create(_valid_cmds);
	return __true;
}

void uninit_server_shell(void)
{
	shell_destroy(g_shell);
	g_shell = 0;
}

#ifdef _WIN32
int	on_reloadhis(char *, void*)
{
	HANDLE h;
	h =	OpenEvent(EVENT_MODIFY_STATE, FALSE, HISLOG_NOTIFY_EVENT);
	if(h){
		SetEvent(h);
		CloseHandle(h);
	}
	return 1;
}

int	on_reset_spac(char *, void*)
{
	HANDLE h;
	h =	OpenEvent(EVENT_MODIFY_STATE, FALSE, SPAC_RESET_EVENT);
	if(h){
		SetEvent(h);
		CloseHandle(h);
	}
	return 1;
}
#endif

int on_switch(char * v, void *d)
{
	__bool ok = __false;
	CArgs args;

	PCRTK_PACKET p = (PCRTK_PACKET)d;
	
	parse_arg_ex(v, &args, " ");
	if(args.argc >= 2){
		if(!strcmpi(args.argv[1], "primary")){
			ok = switch_to_primary();
		}else if(!strcmpi(args.argv[1], "backup")){
			ok = switch_to_backup();
		}
	}
	
	send_rtk_data_with_guid(
		vbus_system,
		&p->src,
		&p->guid,
		ok ? PT_System : PT_System | PF_Failed | PF_ACK,
		0,
		0
		);
	
	return 0;
}

int on_commitlog(char *,void *)
{
	close_log();
	open_log();
	return 0;
}

