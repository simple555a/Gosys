#include "precomp.h"

/***************************************************************************/
class CAddDevice : public CWaitableTransaction
{
public:
	CAddDevice(
		PCNODE_KEY node,
		PCDEVICE_KEY key,
		PCVENDOR_KEY v,
		PCDEVTYPE_KEY t,
		const char * addr,
		const char * init_string, 
		__uint timeout
		)
	{
		RTK_ADDR dest;
		node_to_host(node, &dest.host);
		dest.port = PORT_ALL_SERVER;
		ok = false;
		__uint size;
		if(!init_string){
			init_string = "";
		}
		size = strlen(addr) + 1 + strlen(init_string) + 1 + sizeof(*v) + sizeof(*t) + sizeof(*key);
		void *buf;
		buf = alloc_buffer_v(
			key,
			sizeof(*key),
			v, 
			sizeof(*v), 
			t,
			sizeof(*t),
			addr,
			strlen(addr) + 1,
			init_string, 
			strlen(init_string)+1,
			0
			);
		if(!buf){
			return;
		}
		add_transaction(vbus_config, this);
		send_rtk_data_with_guid(
			vbus_config,
			&dest,
			&Guid,
			PT_AddDevice,
			buf,
			size);
		free_buffer(buf);
		Wait(timeout);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		_on_add_device(packet);
		if(packet->packet_type & PF_Failed){
			SetEvent();
			return;
		}
		ok = true;
		SetEvent();
	}
	bool ok;
};

/*
*/
PROXY_API __bool PMC_API proxy_add_device(
	PCNODE_KEY node,
	PCDEVICE_KEY key,
	PCVENDOR_KEY v,
	PCDEVTYPE_KEY t,
	const char * addr,
	const char * init_string, 
	__uint timeout
	)
{
	CAddDevice *tr = new CAddDevice(node, key, v, t, addr, init_string, timeout);
	if(!tr){
		return __false;
	}
	return tr->ok;
}

/*********************************************************************************************/
class CDelDevice : public CWaitableTransaction
{
public:
	CDelDevice(
		PCNODE_KEY node, 
		PCDEVICE_KEY key, 
		__uint timeout
		)
	{
		RTK_ADDR dest;
		node_to_host(node, &dest.host);
		dest.port = PORT_ALL_SERVER;
		ok = false;
		add_transaction(vbus_config, this);
		send_rtk_data_with_guid(
			vbus_config,
			&dest,
			&Guid,
			PT_DelDevice,
			key,
			sizeof(*key)
			);
		Wait(timeout);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		_on_del_device(packet);
		if(packet->packet_type & PF_Failed){
			SetEvent();
			return;
		}
		ok = true;
		SetEvent();
	}
	bool ok;
};

/*
 *
 */
PROXY_API __bool PMC_API proxy_del_device(
	PCNODE_KEY node,
	PCDEVICE_KEY key,
	__uint timeout
	)
{
	CDelDevice *tr = new CDelDevice(node, key, timeout);
	if(!tr){
		return __false;
	}
	return tr->ok;
}

/*******************************************************************************/
class CEditDevice : public CWaitableTransaction
{
public:
	CEditDevice(
		PCNODE_KEY node, 
		PCDEVICE_KEY key, 
		const char * addr,
		const char * init_string, 
		__uint timeout
		)
	{
		RTK_ADDR dest;
		node_to_host(node, &dest.host);
		dest.port = PORT_ALL_SERVER;
		ok = false;
		__uint size;
		if(!init_string){
			init_string = "";
		}
		size = strlen(addr) + 1 + strlen(init_string) + 1 + 
			sizeof(*key);
		void *buf;
		buf = alloc_buffer_v(
			key, 
			sizeof(*key), 
			addr,
			strlen(addr) + 1,
			init_string, 
			strlen(init_string)+1,
			0
			);
		if(!buf){
			return;
		}
		add_transaction(vbus_config, this);
		send_rtk_data_with_guid(
			vbus_config,
			&dest,
			&Guid,
			PT_EditDevice,
			buf,
			size);
		free_buffer(buf);
		Wait(timeout);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		_on_edit_device(packet);
		if(packet->packet_type & PF_Failed){
			SetEvent();
			return;
		}
		ok = true;
		SetEvent();
	}
	bool ok;
};

PROXY_API __bool PMC_API proxy_edit_device(
	PCNODE_KEY node,
	PCDEVICE_KEY key, 
	const char * addr,
	const char * init_string, 
	__uint timeout
	)
{
	CEditDevice *tr = new CEditDevice(node, key, addr, init_string, timeout);
	if(!tr){
		return __false;
	}
	return tr->ok;
}
