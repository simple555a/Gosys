/*
	Configuration Manager
	by J.Hunter, 2003/9/20
*/
#include "precomp.h"

struct hist_item{
	LIST_ENTRY			link;
	/* uuid state of the configuration before executing cmd */
	f8_uuid				state_id;
	struct kern_conf_cmd cmd;
	struct kern_conf_cmd undo_cmd;
};

static LIST_ENTRY g_HistItems;

int push_command(
	f8_uuid * cur, 
	f8_uuid * next, 
	struct kern_conf_cmd * cmd,
	struct kern_conf_cmd * undo_cmd
	)
{
	return F8_SUCCESS;
}

int rollback(f8_uuid * state_id)
{
	return F8_SUCCESS;
}

int clear_his()
{
	return F8_SUCCESS;
}

int load_his(f8_uuid * blk_id)
{
	return F8_SUCCESS;
}

int save_his(f8_uuid * blk_id)
{
	return F8_SUCCESS;
}

int on_ver(char *v, void *k)
{
	char name[64];

	_shproc_prolog_();

	/*
	if(proxy_adapter->status != ADAPTER_ONLINE){
		return F8_ADAPTER_OFFLINE;
	}
	*/

	cmd.cmd = F8_GET_VERSION;
	__vcall__(proxy_adapter, exec_local, (proxy_adapter, &cmd));
	if(!successful){
		return cmd.retcode;
	}
	f8_uuid_to_string(&OLD_ID(&cmd), name, sizeof(name));
	utils_trace(" Local Version : %s\n", name);
	utils_trace("Target Version : ");
	__vcall__(proxy_adapter, exec_remote, (proxy_adapter, &cmd, psp->timeout));
	if(!successful){
		utils_trace("Undetermined.\n");
		return cmd.retcode;
	}
	f8_uuid_to_string(&OLD_ID(&cmd), name, sizeof(name));
	utils_trace("%s\n", name);

	return F8_SUCCESS;
}

void load_config()
{
	char * p;
	int size;
	struct kern_conf_cmd cmd;

	memset(&cmd, 0, sizeof(cmd));
	p = get_res_buf(&sections->uuid, &size, "sysconfig");
	if(p){
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd = F8_SET_SYS_CONFIG;
		OLD_ID(&cmd) = BLK_ID_ANY;
		NEW_ID(&cmd) = BLK_ID_NONE;
		cmd.isize = size;
		cmd.ibuffer = p;
		cmd.obuffer = 0;
		cmd.osize = 0;
		__vcall__(
			proxy_adapter, 
			exec_local, 
			(proxy_adapter, &cmd)
			);
	}else{
		ctrller_config cfg;
		memset(&cfg, 0, sizeof(cfg));
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd = F8_SET_SYS_CONFIG;
		OLD_ID(&cmd) = BLK_ID_ANY;
		NEW_ID(&cmd) = BLK_ID_NONE;
		cmd.isize = sizeof(cfg);
		cmd.ibuffer = (char*)&cfg;
		cmd.obuffer = 0;
		cmd.osize = 0;

		/* load default memory layout */
		cfg.x_mem_sizes[MEM_SECTION_BYTE] = 12*1024;
		cfg.x_mem_sizes[MEM_SECTION_WORD] = 12*1024;
		cfg.x_mem_sizes[MEM_SECTION_R4] = 32*1024;
		cfg.x_mem_sizes[MEM_SECTION_R8] = 32*1024;
		cfg.x_mem_sizes[MEM_SECTION_ANYSIZE] = 0*1024;
		cfg.x_mem_sizes[MEM_SECTION_BIT] = 12*1024;
		
		__vcall__(
			proxy_adapter, 
			exec_local, 
			(proxy_adapter, &cmd)
			);
	}
}

void save_config()
{
	_shproc_prolog_no_k_();

	struct ctrller_config cfg;
	cmd.cmd = F8_GET_SYS_CONFIG;
	cmd.osize = sizeof(cfg);
	cmd.obuffer = (char *)&cfg;
	__vcall__(proxy_adapter, exec_local, (proxy_adapter, &cmd));
	set_res_buf(
		&sections->uuid, 
		&cfg, 
		sizeof(cfg), 
		"sysconfig"
		);
}

