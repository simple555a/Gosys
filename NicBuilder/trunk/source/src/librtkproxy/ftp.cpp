#include "precomp.h"
#pragma hdrstop

using namespace std;

/*CGetFileSize**********CDownload**************************************************/
class CGetFileSize : public CWaitableTransaction
{
public:
	CGetFileSize(PCNODE_KEY node, const char * path)
	{
		RTK_ADDR addr;
		node_to_host(node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		add_transaction(vbus_system, this);
		ok = __false;
#ifndef NDEBUG
		{
			char buf[64];
			Guid.to_string(buf);
			DEBUG_PRINTF(("GFS on GUID:%s\n", buf));
		}
#endif
		send_rtk_data_with_guid(
			vbus_system,
			&addr,
			&Guid,
			PT_GetFileSize,
			path,
			strlen(path) + 1
			);
		Wait(1500);
	}
	virtual void OnEvent(PCRTK_PACKET p)
	{
#ifndef NDEBUG
		{
			char buf[64];
			Guid.to_string(buf);
			DEBUG_PRINTF((
				"GFS acked on GUID:%s, bytes=%d, source=%s:%d\n", 
				buf, 
				p->data_size,
				(char*)CHostName(p->src.host),
				p->src.port
				));
		}
#endif
		if(p->data_size != sizeof(__uint)){
			ok = false;
		}else{
			size = *((__uint*)p->data);
			ok = true;
		}
		SetEvent();
	}
	__bool ok;
	__uint size;
};

class CDownload : public CWaitableTransaction
{
public:
	CDownload(
		PCNODE_KEY node, 
		const char * path,
		char  **buffer, 
		__uint *buffersize,
		__uint timeout
		)
	{
		buf = buffer;
		this->buffersize = buffersize;
		RTK_ADDR addr;
		node_to_host(node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		file = string((char *)CNodeName(*node)) + ":" + path;
		ok = false;
		add_transaction(vbus_system, this);
		send_rtk_data_with_guid(
			vbus_system,
			&addr,
			&Guid,
			PT_GetFile,
			path,
			strlen(path) + 1
			);
		Wait(timeout, true);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		if(packet->packet_type & PF_Failed){
			SetEvent();
			return;
		}
		if(!packet->data_size){
			*buf = strdup("");
			*buffersize = 0;
			return;
		}
		*buf = (char *)malloc(packet->data_size);
		if(!buf){
			SetEvent();
			return;
		}
		memcpy(*buf, packet->data, packet->data_size);
		*buffersize = packet->data_size;
		ok = true;
		string s;
		s = file + " 下载完毕.";
		rtk_queue_event(PT_Progress, 100, 0, s.data());
		SetEvent();
	}
	virtual void OnProgress(__uint total, __uint completed, PCRTK_PACKET p)
	{
		string s;
		s = "正在下载 " + file;
		rtk_queue_event(PT_Progress, total, completed, s.data());
	}
	bool	ok;
	string file;
	char   **buf;
	__uint *buffersize;
};

PROXY_API __bool PMC_API proxy_get_file_size(
	PCNODE_KEY node,
	const char * path,
	__uint *size)
{
	*size = 0;
	CGetFileSize *gcs = new CGetFileSize(node, path);
	if(!gcs){
		return __false;
	}
	if(!gcs->ok){		
		return __false;
	}
	*size = gcs->size;
	return __true;	
}

PROXY_API __bool PMC_API download_file(
	PCNODE_KEY node,
	const char * path,
	char **buffer,
	__uint *buffersize
	)
{
	__uint filesize;
	if(!proxy_get_file_size(node, path, &filesize)){
		*buffersize = 0;
		return __false;
	}

	if(!filesize){
		*buffer = strdup("");
		*buffersize = 0;
		return __true;
	}

	__uint timeout;
	timeout = (filesize + 1023)/1024*100;

	CDownload *gc = new CDownload(node, path, buffer, buffersize, timeout);
	if(!gc){
		return __false;
	}
	if(!gc->ok){
		return __false;
	}

	return __true;
}

/*CUpload****************************************************************************/
class CUpload : public CWaitableTransaction
{
public:
	CUpload(
		PCNODE_KEY node, 
		const char * path,
		char  *buffer, 
		__uint buffersize,
		__uint timeout
		)
	{
		__u32 namelen = strlen(path);
		ok = false;
		buf = alloc_buffer_v(
			&namelen,
			sizeof(namelen),
			path,
			strlen(path) + 1,
			buffer,
			buffersize,
			0);
		if(!buf){
			return;
		}
		RTK_ADDR addr;
		node_to_host(node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		file = string((char *)CNodeName(*node)) + ":" + path;
		ok = false;
#ifndef NDEBUG
		{
			char buf[64];
			Guid.to_string(buf);
			DEBUG_PRINTF(("PUTF on %s, GUID:%s\n", file.data(), buf));
		}
#endif
		add_transaction(vbus_system, this);
		send_rtk_data_with_guid(
			vbus_system,
			&addr,
			&Guid,
			PT_PutFile,
			buf,
			sizeof(buffersize) + strlen(path) + 1 + buffersize
			);
		free_buffer(buf);
		Wait(timeout, true);
	}
	virtual void OnEvent(PCRTK_PACKET p)
	{
#ifndef NDEBUG
		{
			char buf[64];
			Guid.to_string(buf);
			DEBUG_PRINTF((
				"PUTF acked on GUID:%s, type=%08x, bytes=%d, source=%s:%d\n", 
				buf, 
				p->packet_type,
				p->data_size,
				(char*)CHostName(p->src.host),
				p->src.port
				));
		}
#endif
		if(p->packet_type & PF_Failed){
			SetEvent();
			return;
		}
		ok = true;
		string s;
		s = file + " 上载完毕.";
		rtk_queue_event(PT_Progress, 100, 0, s.data());
		SetEvent();
	}
	bool	ok;
	string	file;
	void	*buf;
};

PROXY_API __bool PMC_API upload_file(
	PCNODE_KEY node,
	const char * path,
	char *buffer,
	__uint buffersize,
	__uint timeout
	)
{
	CUpload *up = new CUpload(node, path, buffer, buffersize, timeout);
	if(!up){
		return __false;
	}
	return up->ok;
}

/*CDeleteFIle**********************************************************************/
class CDeleteFile : public CWaitableTransaction
{
public:
	CDeleteFile(PCNODE_KEY node, const char * path, __uint timeout)
	{
		RTK_ADDR addr;
		add_transaction(vbus_system, this);
		node_to_host(node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		ok = false;
		send_rtk_data_with_guid(
			vbus_system,
			&addr,
			&Guid,
			PT_DeleteFile,
			path,
			strlen(path) + 1
			);		
		Wait(timeout);
	}
	virtual void OnEvent(PCRTK_PACKET p)
	{
		if(p->packet_type & PF_Failed){
			ok = false;
			SetEvent();
			return;
		}
		ok = true;
		SetEvent();
	}
	bool ok;
};

PROXY_API __bool PMC_API proxy_delete_file(
	PCNODE_KEY node,
	const char * path,
	__uint timeout
	)
{
	CDeleteFile *tr = new CDeleteFile(node, path, timeout);
	return tr->ok;
}

/*CDiscoverFiles*******************************************************************/
class CDiscoverFiles : public CWaitableTransaction
{
public:
	CDiscoverFiles(PCNODE_KEY node, const char * path, const char * w, char * b, __uint s, __uint timeout)
	{
		void *vbuf;
		RTK_ADDR dest;
		
		ok = false;
		vbuf = alloc_buffer_v(
			path,
			strlen(path) + 1,
			w,
			strlen(w) + 1,
			0
			);
		if(!vbuf){
			return;
		}
		bufsize = s;
		buf = b;
		*buf = '\0';
		add_transaction(vbus_config, this);		
		node_to_host(node, &dest.host);
		dest.port = PORT_ALL_SERVER;
		send_rtk_data_with_guid(
			vbus_config, 
			&dest,
			&Guid,
			PT_DiscoverFiles,
			vbuf,
			strlen(path) + strlen(w) + 2
			);
		Wait(timeout);
		free_buffer(vbuf);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		if(packet->data_size > bufsize){
			ok = false;
			SetEvent();
			return;
		}
		memcpy(buf, packet->data, packet->data_size);
		ok = true;
		SetEvent();
	}
public:
	__uint		bufsize;
	char		*buf;
	bool		ok;
};

PROXY_API __uint PMC_API discover_files(
	PCNODE_KEY node,
	const char * path,
	const char * w,
	char *buffer,
	__uint buffersize,
	__uint timeout
	)
{
	CDiscoverFiles *eye = new CDiscoverFiles(node, path, w, buffer, buffersize, timeout);
	if(!eye){
		return 0;
	}
	if(!eye->ok){
		return 0;
	}
	char *p;
	p = buffer;
	__uint i=0;
	while(*p){
		i++;
		p += strlen(p) + 1;
	}
	return i;
}

PROXY_API void PMC_API proxy_release_data(void * buf)
{
	free(buf);
}
