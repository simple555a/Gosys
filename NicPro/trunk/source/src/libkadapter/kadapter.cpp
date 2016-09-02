#include "precomp.h"
#pragma hdrstop
#include <rtk_thread.h>

static __bool _import_pin_proc(ITreeNode * nd, __bool firstVisit, __uint context)
{
	IBlk * blk;
	IBBlk * bb;
	int i;
	struct blk_pin_t * p;

	blk = __dcast__(ITreeNode, IBlk, nd);
	if(blk->h.magic != BBLK_MAGIC){
		return __true;
	}
	bb = __dcast__(IBlk, IBBlk, blk);
	p = bb->pins;
	for(i=blk->h.n_pins; i; i--, p++){
		if(p->_class != PIN_CLASS_DI && p->_class != PIN_CLASS_DO){
			continue;
		}
		if(!p->mdp){
			continue;
		}
		// transfer some bytes from p->mdp to p->dp
#define _trans(type)\
	*((type*)p->dp) = *((type*)p->mdp);
		switch(p->type){
		case PIN_T_BOOL:
		case PIN_T_BYTE:
		case PIN_T_CHAR:
			_trans(__i8);
			break;
		case PIN_T_WORD:
		case PIN_T_SHORT:
			_trans(__i16);
			break;
		case PIN_T_DWORD:
		case PIN_T_INTEGER:
		case PIN_T_FLOAT:
			_trans(__i32);
			break;
		case PIN_T_DOUBLE:
		case PIN_T_DATE:
			_trans(__i64);
			break;
		}
#undef _trans
		if(p->type == PIN_T_BOOL && p->flags & PIN_F_INVERTED){
			p->dp->u.d.ui8 = !p->dp->u.d.ui8;
		}		
	}

	return __true;
}

extern "C" int uncompress(void *, unsigned int *, const void *, unsigned int);

#define USE_NON_COMPRESSED_XMEM

static __uint RTK_API synchronizer(__uint _adapter)
{
	struct kern_conf_cmd cmd;
	char buf[KERN_POOL_SIZE*2];
	int i;
	f8_uint sizes[KERN_NUM_SECTIONS];
	IKAdapter * adapter = (IKAdapter *)_adapter;
	HF8_KERNEL kernel = adapter->kernel;
	IBlk * sections = ke_get_blk(kernel, NULL);
	char *p;
	int memsize;

	utils_debug("Status synchronizer thread started.\n");
	
	while(!stop_request_pending(adapter->hImportPinThread)){
		if((adapter->flags & ADAPTER_F_ONLINE) && !((adapter->flags & ADAPTER_F_PAUSED))){
			ke_get_mem_sizes(kernel, sizes);
			memsize = 0;
			for(i=0; i<KERN_NUM_SECTIONS; i++){
				memsize += sizes[i];
			}
			if(memsize > KERN_POOL_SIZE){
				break;
			}

			// ask for status data
			memset(&cmd, 0, sizeof(cmd));	
			OLD_ID(&cmd) = sections->h.uuid;
			NEW_ID(&cmd) = BLK_ID_NONE; 
			cmd.obuffer = buf;
			cmd.osize = memsize;
			cmd.cmd = F8_GET_X_MEMORY;

			__vcall__(adapter, exec_remote, (adapter, &cmd, 15000));

			if(F8_SUCCESSED(cmd.retcode) && cmd.osize == memsize){
				p = buf;
				for(i=0; i<KERN_NUM_SECTIONS; i++){
					if(sizes[i]){
						memcpy(ke_get_section(kernel, i), p, sizes[i]);
						p += sizes[i];
					}
				}
				ke_lock(kernel, 0);
				_traverse(sections, _import_pin_proc, _adapter);
				ke_unlock(kernel, 0);
			}
		}
		rtk_sleep(250);
	}

	ke_lock(kernel, 0);
	// finally, clear the local copy of x-memory
	ke_get_mem_sizes(kernel, sizes);
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		if(sizes[i]){
			memset(ke_get_section(kernel, i), 0, sizes[i]);
		}
	}
	_traverse(sections, _import_pin_proc, _adapter);
	ke_unlock(kernel, 0);
	
	utils_debug("Status synchronizer thread exited.\n");

	return 0;
}

KADAPTER_API __bool IKAdapter_begin_transfer(IKAdapter * adapter)
{
	if(adapter->hImportPinThread){
		return __true;
	}
	adapter->hImportPinThread = create_rthread(
		synchronizer, 
		(__uint)adapter
		);
	if(adapter->hImportPinThread){
		start_rthread(adapter->hImportPinThread);
		return __true;
	}
	return __false;
}

KADAPTER_API __bool IKAdapter_end_transfer(IKAdapter * adapter)
{
	if(adapter->hImportPinThread){
		if(!stop_rthread(adapter->hImportPinThread)){
			assert(__false);
		}
		close_handle(adapter->hImportPinThread);
		adapter->hImportPinThread = 0;
	}
	return __true;
}

KADAPTER_API __bool IKAdapter_pause_transfer(IKAdapter * adapter)
{
	//
	adapter->flags |= ADAPTER_F_PAUSED;
	return __true;
}

KADAPTER_API __bool IKAdapter_resume_transfer(IKAdapter * adapter)
{
	adapter->flags &= ~ADAPTER_F_PAUSED;
	return __true;
}

f8_status IKAdapter_exec0(IKAdapter * _this, __u16 cmdno, __uint timeout)
{
	struct kern_conf_cmd cmd;
	memset(&cmd, 0, sizeof(cmd));
	cmd.cmd = cmdno;
	__vcall__(_this, exec, (_this, &cmd, timeout));
	return cmd.retcode;
}

void IKAdapter_exec_local(IKAdapter *_this, struct kern_conf_cmd *cmd)
{
	ke_config(_this->kernel, cmd, __true);
	return;
}

void IKAdapter_exec_remote(IKAdapter *_this, kern_conf_cmd * cmd, __uint timeout)
{
	cmd->retcode = F8_INVALID_OPERATION;
}

/*
	IKAdapter_exec

	configuration command router.
	
	executes a configuration command packet, for some of
	the commands, mostly the commands that will alter the
	control program, the adapter behaves differently according
	to current online status. If the command won't alter
	the current control program, then it's processed locally.
*/
void IKAdapter_exec(IKAdapter *_this, struct kern_conf_cmd *cmd, __uint timeout)
{
	struct kern_conf_cmd saved_cmd;

	static char cmdRouting[256];
	static __bool cmdRoutingInited;
	
	if(!cmdRoutingInited){
		cmdRoutingInited=__true;
#define define_cmd(name,value,exclusive,routing,foo1,foo2,foo3)\
	cmdRouting[name]=F8CMD_route_##routing;	
#include "f8_cmd.h"
	}

	switch(cmdRouting[cmd->cmd]){
	case 0:
		if(_online(_this)){
			saved_cmd = *cmd;
			__vcall__(
				_this, 
				exec_remote, 
				(_this, &saved_cmd, timeout)
				);
			if(saved_cmd.retcode != F8_SUCCESS){
				*cmd = saved_cmd;
				return;
			}
		}
		__vcall__(_this, exec_local, (_this, cmd));
		break;
		
	case 1:
		if(!_online(_this)){
			cmd->retcode=F8_CONNECTION_LOST;
			cmd->osize=0;
		}else{
			__vcall__(
				_this, 
				exec_remote, 
				(_this, cmd, timeout)
				);
		}
		break;
		
	case 2:
		// no need to make network traffic
		__vcall__(_this, exec_local, (_this, cmd));
		break;
	}	
}

void IKAdapter_fire_event(IKAdapter *_this, f8_int e, f8_int p1, f8_int p2)
{
	if(_this->eventSink){
		_this->eventSink(_this, e, p1, p2);
	}
}

KADAPTER_API void delete_adapter(IKAdapter * a)
{
#define _chk(tp,ttp)\
	case tp:\
	{\
		ttp * v;\
		v = __dcast__(IKAdapter, ttp, a);\
		__delete__(v);\
		return;\
	}

	switch(a->type){
	_chk(AT_FILE_ADAPTER, IFileAdapter);
	_chk(AT_SHM_ADAPTER, IShmAdapter);
	_chk(AT_TCP_ADAPTER, ITcpAdapter);
	}
}

__define_vtbl__(IKAdapter)
__define_vtbl_end__(IKAdapter)

__global_constructor__(IKAdapter)
{
	__set_vfunc__(IKAdapter, exec0);
	__set_vfunc__(IKAdapter, exec);
	__set_vfunc__(IKAdapter, exec_local);
	__set_vfunc__(IKAdapter, exec_remote);
}

__constructor__(IKAdapter)
{
	if(__this){
		__touch__();
		__this->type = 0;
		__this->flags = 0;
		__this->eventSink = 0;
		__this->hImportPinThread = 0;
		__this->kernel = ke_alloc_kernel();
		__this->msgQueue = create_msg_queue(__this);
		IKAdapter_begin_transfer(__this);
	}
	return __this;
}

__destructor__(IKAdapter)
{
	if(__this){
		IKAdapter_end_transfer(__this);
		if(__this->kernel){
			ke_free_kernel(__this->kernel);
			__this->kernel = 0;
		}
		if(__this->msgQueue){
			destroy_msg_queue(__this->msgQueue);
			__this->msgQueue = 0;
		}
	}
	return __this;
}

