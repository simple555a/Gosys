#include "precomp.h"
#pragma hdrstop

int on_online(char * v, void *k)
{
	CArgsEx args;
	int err;
	const char *tgt;

	_shproc_prolog_();

	args.parse(v, FS);
	if(args.argc < 2){
		tgt = get_res_buf(&sections->uuid, 0, "!target0");
		if(!tgt){
			utils_error("Usage : online <target>\n");
			return F8_SYNTAX;
		}
	}else{
		tgt = args[1];
		set_res_buf(&sections->uuid, tgt, -1, "!target0");
	}	

	err = __vcall__(proxy_adapter, online, (proxy_adapter, tgt, psp->timeout));
	if(F8_SUCCESS != err){
		return err;
	}

	return cmd.retcode;
}

int on_lock(char *v, void *k)
{
	CArgs args;
	f8_i32 config=1;
	_shproc_prolog_();
	parse_arg_ex(v,&args," \r\n");
	if(args.argc > 1){
		if(!strcmpi(args.argv[1],"true"))
			config=1;
		else if(!strcmpi(args.argv[1],"false"))
			config=0;
		else
			return F8_SYNTAX;
	}
	cmd.cmd = F8_LOCK_CONTROLLER;
	cmd.ibuffer=&config;
	cmd.isize=sizeof(config);
	__vcall__(proxy_adapter, exec_remote, (proxy_adapter, &cmd, psp->timeout));
	return cmd.retcode;
}

int on_unlock(char *v,void *k)
{
	return on_lock("lock false",k);
}

int on_offline(char *v, void *k)
{
	__vcall__(proxy_adapter, offline, (proxy_adapter));

	return F8_SUCCESS;
}

/*
	a handy procedure
	execute a command with 0 input bytes and 0 output bytes.
*/
int _ctrller_command(char * v, void * k, __u16 _cmd)
{
	_shproc_prolog_();

	cmd.cmd = _cmd;
	
	__vcall__(
		proxy_adapter, 
		exec,
		(proxy_adapter, &cmd, 1000)
		);

	return cmd.retcode;
}

int on_start(char * v, void *k)
{
	return start_ctrller();
}

int on_stop(char * v, void *k)
{
	return stop_ctrller();
}

int on_break(char * v, void *k)
{
	return _ctrller_command(v, k, F8_BREAK);
}

int on_continue(char * v, void *k)
{
	return _ctrller_command(v, k, F8_CONTINUE);
}

int on_reset(char *, void *k)
{
	_shproc_prolog_();

	cmd.cmd = F8_RESET;
	create_f8_uuid(&NEW_ID(&cmd));
	
	__vcall__(
		proxy_adapter, 
		exec,
		(proxy_adapter, &cmd, 1000)
		);
	g_bDirty = __true;
	return cmd.retcode;
}

int on_suspend(char *v, void * k)
{
	return _ctrller_command(v, k, F8_SUSPEND_STATE);
}

int on_resume(char *v, void *k)
{
	return _ctrller_command(v, k, F8_RESUME_STATE);
}

int on_sync(char *v,void *k)
{
	CArgs a;
	parse_arg_ex(v,&a," ");
	if(a.argc==1){
		utils_trace("Synchronizer is %srunning\n", proxy_adapter->flags & ADAPTER_F_PAUSED?"not ":"");
		return F8_SUCCESS;
	}
	if(!strcmpi(a.argv[1],"on"))
		IKAdapter_resume_transfer(proxy_adapter);
	else if(!strcmpi(a.argv[1],"off"))
		IKAdapter_pause_transfer(proxy_adapter);
	else{
		utils_error("Syntax : sync [on/off]\n");
		return F8_SYNTAX;
	}
	return F8_SUCCESS;
}

int on_targetexec(char *v,void *k)
{
	CArgsEx args;
	char *cmdStr;

	_shproc_prolog_();

	args.parse(v, FS);
	if(args.argc < 2){
		utils_error("Usage : targetexec \"command\"\n");
		return F8_SYNTAX;
	}else{
		cmdStr=v+args.arg_idx[1];
	}
	
	cmd.cmd=F8_TARGET_EXEC;
	cmd.ibuffer=cmdStr;
	cmd.isize = strlen(cmdStr)+1;
	cmd.obuffer=x_buffer;
	cmd.osize=x_buf_len;

	__vcall__(proxy_adapter,exec,(proxy_adapter, &cmd, psp->timeout));

	if(F8_SUCCESSED(cmd.retcode)){
		utils_trace("%s\n",x_buffer);
	}
	
	return cmd.retcode;
}

