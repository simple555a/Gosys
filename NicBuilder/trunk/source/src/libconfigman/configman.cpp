#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <utils.h>
#include <rtknet.h>
#include <transaction.h>
#include <configman.h>
#include <rtk_packet.h>

using namespace std;

#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtknet.lib")

class CGetString : public CWaitableTransaction
{
public:
	CGetString(
		PCNODE_KEY node, 
		const char * s, 
		const char * k, 
		__uint t
		)
	{
		RTK_ADDR addr;
		void *buf;		
		buf = alloc_buffer_v(
			s, strlen(s)+1,
			k, strlen(k)+1,
			0);
		if(!buf){
			return;
		}
		node_to_host(node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		add_transaction(vbus_config, this);
		ok = false;
		send_rtk_data_with_guid(
			vbus_config,
			&addr,
			&Guid,
			PT_GetString,
			buf,
			strlen(s) + strlen(k) + 2
			);
		free_buffer(buf);
		Wait(t);	
	}

	virtual void OnEvent(PCRTK_PACKET packet)
	{
		ok = true;
		strncpy(m_buff, packet->data, sizeof(m_buff));
		m_buff[sizeof(m_buff) - 1] = 0;
		SetEvent();
	}
public:
	bool ok;
	char m_buff[1024];
};

CONFIGMAN_API __bool PMC_API get_config_string(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	char * buf,
	__uint length,
	__uint timeout
	)
{
	CGetString tr(node, section, key, timeout);
	*buf = 0;
	if(tr.ok){
		strncpy(buf, tr.m_buff, length);
		buf[length - 1] = 0;
	}
	return tr.ok;
}

class CGetStringEx : public CWaitableTransaction
{
public:
	CGetStringEx(
		PCNODE_KEY node, 
		const char * f,
		const char * s, 
		const char * k, 
		__uint t
		)
	{
		RTK_ADDR addr;
		void *buf;		
		buf = alloc_buffer_v(
			f, strlen(f)+1,
			s, strlen(s)+1,
			k, strlen(k)+1,
			0);
		if(!buf){
			return;
		}
		node_to_host(node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		add_transaction(vbus_config, this);
		ok = false;
		send_rtk_data_with_guid(
			vbus_config,
			&addr,
			&Guid,
			PT_GetStringEx,
			buf,
			strlen(f) + strlen(s) + strlen(k) + 3
			);
		free_buffer(buf);
		Wait(t);	
	}

	virtual void OnEvent(PCRTK_PACKET packet)
	{
		ok = true;
		strncpy(m_buff, packet->data, sizeof(m_buff));
		m_buff[sizeof(m_buff) - 1] = 0;
		SetEvent();
	}
public:
	bool ok;
	char m_buff[1024];
};

CONFIGMAN_API __bool PMC_API get_config_string_ex(
	PCNODE_KEY node,
	const char * file,
	const char * section,
	const char * key,
	char * buf,
	__uint length,
	__uint timeout
	)
{
	CGetStringEx tr(node, file, section, key, timeout);
	*buf = 0;
	if(tr.ok){
		strncpy(buf, tr.m_buff, length);
		buf[length - 1] = 0;
	}
	return tr.ok;
}

class CSetString : public CWaitableTransaction
{
public:
	CSetString(
		PCNODE_KEY node, 
		const char * s, 
		const char * k, 
		const char * v, 
		__uint t
		)
	{
		RTK_ADDR addr;
		void *buf;
		buf = alloc_buffer_v(
			s, strlen(s)+1,
			k, strlen(k)+1,
			v, strlen(v) +1,
			0);
		if(!buf){
			return;
		}
		node_to_host(node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		add_transaction(vbus_config, this);
		ok = false;
		send_rtk_data_with_guid(
			vbus_config,
			&addr,
			&Guid,
			PT_SetString,
			buf,
			strlen(s) + strlen(k) + strlen(v) + 3
			);
		free_buffer(buf);
		Wait(t);	
	}

	virtual void OnEvent(PCRTK_PACKET packet)
	{
		ok = true;
		SetEvent();
	}
public:
	bool ok;
};

CONFIGMAN_API __bool PMC_API set_config_string(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	const char * value,
	__uint timeout
	)
{
	CSetString tr(node, section, key, value, timeout);
	return tr.ok;
}

CONFIGMAN_API __bool PMC_API get_config_int(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	__int *buf,
	__uint timeout
	)
{
	return __false;
}

CONFIGMAN_API __bool PMC_API set_config_int(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	__int value,
	__uint timeout
	)
{
	return __false;
}

class CSetStringEx : public CWaitableTransaction
{
public:
	CSetStringEx(
		PCNODE_KEY node, 
		const char * f,
		const char * s, 
		const char * k, 
		const char * v, 
		__uint t
		)
	{
		RTK_ADDR addr;
		void *buf;
		buf = alloc_buffer_v(
			f, strlen(f)+1,
			s, strlen(s)+1,
			k, strlen(k)+1,
			v, strlen(v) +1,
			0);
		if(!buf){
			return;
		}
		node_to_host(node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		add_transaction(vbus_config, this);
		ok = false;
		send_rtk_data_with_guid(
			vbus_config,
			&addr,
			&Guid,
			PT_SetStringEx,
			buf,
			strlen(f) + strlen(s) + strlen(k) + strlen(v) + 4
			);
		free_buffer(buf);
		Wait(t);	
	}

	virtual void OnEvent(PCRTK_PACKET packet)
	{
		ok = true;
		SetEvent();
	}
public:
	bool ok;
};

CONFIGMAN_API __bool PMC_API set_config_string_ex(
	PCNODE_KEY node,
	const char * f,
	const char * section,
	const char * key,
	const char * value,
	__uint timeout
	)
{
	CSetStringEx tr(node, f, section, key, value, timeout);
	return tr.ok;
}
