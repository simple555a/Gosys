/*
	F8 executive entry point.
	by J.Hunter, jhunter@tsinghua.org.cn

	2003/10/29 a quick and dirty implementation
*/
#include "precomp.h"
#pragma hdrstop
#include <time.h>
#include "zlib.h"

//#define AGENT_DEBUG 

#ifdef AGENT_DEBUG
int agentDebug = 0;
#define AGENT_LOG(x) if(agentDebug) printf x
#ifdef LOCAL
#undef LOCAL
#endif
#define LOCAL
#else
#define AGENT_LOG(x)
#ifndef LOCAL
#define LOCAL static 
#endif
#endif

f8_u16 agentPort = F8_CFG_TCP_PORT;
f8_u32 agentTimeOut = 30;

#define MAX_CLIENTS 16

/*
	this is the agent class which handles a client 
	connection.
*/
struct kern_agent
{
	char				static_buf[10240];
	struct sockaddr_in	m_peer;
	F8_SOCKET		m_socket;
	__bool			m_bStopRequestPending;
	#ifdef __LINUX__
	pthread_t			m_taskId;
	#else
	int				m_taskId;
	#endif
	struct			kernel_t * kernel;
#define AGENT_ENABLE_CONFIG 1
	int				m_enabledFeatures;
};

LOCAL  struct kern_agent agents[MAX_CLIENTS];
LOCAL  struct kern_agent * configuratorAgent;

LOCAL  int agentTask(struct kern_agent * a);
LOCAL  void agentFinal(struct kern_agent * a);
LOCAL  __bool agentOnCmd(struct kern_agent * a);
LOCAL  __bool agentSkip(struct kern_agent * a, __uint bytes);
LOCAL f8_status setConfigurator(struct kern_agent * a, int bSet);

LOCAL  int agentTask(struct kern_agent * a)
{
	fd_set rset;
	int ret;
	struct timeval timeout;
	struct sockaddr_in prAddr;
	f8_uint watchDog = 0;

	ret = sizeof(prAddr);
	getpeername(a->m_socket, (struct sockaddr*)&prAddr, &ret);
	printf(
		"Agent %d for client %s:%d started.\n",
		a - agents,
		inet_ntoa(prAddr.sin_addr),
		ntohs(prAddr.sin_port)
		);
	FD_ZERO(&rset);	 
	
	while(!a->m_bStopRequestPending){
		FD_SET(a->m_socket, &rset);
		timeout.tv_sec = 0;
		timeout.tv_usec = 200*1000;
		ret = select(a->m_socket+1, &rset, NULL, NULL, &timeout);
		if(ret > 0){
			watchDog = 0;
			if(FD_ISSET(a->m_socket, &rset)){
				FD_CLR(a->m_socket, &rset);
				if(!agentOnCmd(a)){
					agentFinal(a);					
					return -1;
				}				
			}			   
		}else if(ret < 0){
			break;
		}else{
			//
			watchDog++;
		}
		if(watchDog >= 5*agentTimeOut){
			printf("No communication with %s:%d for %d seconds, shutting down.\n",
				inet_ntoa(prAddr.sin_addr),
				ntohs(prAddr.sin_port),
				agentTimeOut
				);
			break;
		}
	}

	agentFinal(a);

	return 0;
}

#ifdef _WIN32
LOCAL  void agentTaskStub(void * a)
{
	agentTask((struct kern_agent*)a);
}
#elif defined(__LINUX__)
LOCAL  void * agentTaskStub(void * a)
{
	agentTask((struct kern_agent*)a);
}
#endif

LOCAL  void agentFinal(struct kern_agent * a)
{
	if(!IS_INVALID_SOCKET(a->m_socket)){
		closesocket(a->m_socket);
	}
	printf(
		"Connection with %s:%d shutdown.\n",
		inet_ntoa(a->m_peer.sin_addr),
		ntohs(a->m_peer.sin_port)
		);
	setConfigurator(a,0);
	a->m_taskId = ERROR;
}

#define EXPECTED_BPS (64*1024)

LOCAL  __bool recvPacket(
	F8_SOCKET s, 
	char * buf, 
	int left
	)
{
	int bytes;
	fd_set sset;
	struct timeval tout;
	int timeout = 1000;
	int to;
	clock_t t1, t2;
#ifdef AGENT_DEBUG
	int totalBytes = left;
#endif

	to = left * 1000 / EXPECTED_BPS;
	if(to < timeout){
		to = timeout;
	}
	t1 = clock();
	while(left && to > 0){
		FD_ZERO(&sset);
		FD_SET(s, &sset);
		AGENT_LOG(("Expecting %d bytes, %d left.\n", totalBytes, left));
		tout.tv_sec = 0;
		tout.tv_usec = timeout * 1000;
		select(s + 1, &sset, 0, 0, &tout);
		if(!FD_ISSET(s, &sset)){
			return __false;
		}
		bytes = recv(s, buf, left, 0);
		if(bytes < 0){
			return __false;
		}
		left -= bytes;
		buf += bytes;
		t2 = clock();
		to -= (t2 - t1) * 1000 / CLOCKS_PER_SEC;
		t1 = t2;
	}
	if(left){
		return __false;
	}
	return __true;
}

LOCAL  __bool sendPacket(
	F8_SOCKET s, 
	const char * buf, 
	int left
	)
{
	int bytes;
	fd_set sset;
	struct timeval tout;
	int timeout = 1000;
	int to;
	clock_t	t1, t2;
#ifdef AGENT_DEBUG
	int totalBytes = left;
#endif
	
	to = left * 1000 / EXPECTED_BPS;
	if(to < timeout){
		to = timeout;
	}
	t1 = clock();
	while(left && to > 0){
		FD_ZERO(&sset);
		FD_SET(s, &sset);
		tout.tv_sec = 0;
		tout.tv_usec = timeout * 1000;
		AGENT_LOG(("Sending %d bytes, %d left.\n", totalBytes, left));
		select(s + 1, 0, &sset, 0, &tout);
		if(!FD_ISSET(s, &sset)){
			return __false;
		}
		bytes = send(s, buf, left, 0);
		if(bytes < 0){
			return __false;
		}
		left -= bytes;
		buf += bytes;
		t2 = clock();
		to -= (t2 - t1) * 1000 / CLOCKS_PER_SEC;
		t1 = t2;
	}
	
	if(left){
		return __false;
	}
	
	return __true;
}

LOCAL  __bool agentSkip(struct kern_agent * a, __uint bytes)
{	
	__uint sbytes;

	AGENT_LOG(("Skipping %d bytes\n", bytes));
	
	sbytes = sizeof(a->static_buf);
	if(sbytes > bytes){
		sbytes = bytes;
	}

	while(sbytes > 0){
		if(recv(a->m_socket, a->static_buf, sbytes, 0) <= 0){
			return __false;
		}
		bytes -= sbytes;
		sbytes = sizeof(a->static_buf);
		if(sbytes > bytes){
			sbytes = bytes;
		}
	}

	return __true;
}

LOCAL  void _uploadFile(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	IMemStream ms;
	if(!cmd->isize){
		cmd->retcode = F8_INVALID_DATA;
		cmd->osize = 0;
		return;
	}
	// check input file name is zero-terminated
	if(((char*)cmd->ibuffer)[cmd->isize - 1] != '\0'){
		cmd->retcode = F8_INVALID_DATA;
		cmd->osize = 0;
		return;
	}
	__create__(IMemStream, &ms);
	IMemStream_bind(&ms, cmd->obuffer, cmd->osize);
	if(!IMemStream_insert_file(&ms, cmd->ibuffer)){
		cmd->retcode = F8_FILE_NOT_FOUND;
		cmd->osize = 0;
		return;
	}
	cmd->osize = ms.pointer;
	cmd->retcode = F8_SUCCESS;
}

LOCAL  void _downloadFile(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	IMemStream ms;
	struct c_bstr_t fileName;

	fileName.buffer = 0;

	cmd->osize = 0;
	if(!cmd->isize){
		cmd->retcode = F8_INVALID_DATA;
		return;
	}
	// 
	__create__(IMemStream, &ms);
	IMemStream_bind(&ms, cmd->ibuffer, cmd->isize);
	if(!bstr_from_stream(&fileName, __ucast__(IMemStream, IF8Stream, &ms))){
		cmd->retcode = F8_INVALID_DATA;
		return;
	}
	if(!IMemStream_to_file(&ms, fileName.buffer)){
		cmd->retcode = F8_FILE_NOT_FOUND;
	}else{
		cmd->retcode = F8_SUCCESS;
	}
	//
	c_free_bstr(&fileName);
	;
}

LOCAL  void _downloadResource(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	cmd->retcode = F8_UNIMPLEMENTED;
}

LOCAL  void _uploadResource(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	cmd->retcode = F8_UNIMPLEMENTED;
}

/*
	the f8 kernel merely store the variable table, the control logic runs
	independantly of the variable table.
*/
LOCAL  void _downloadVartable(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	__uint count;
	cmd->osize = 0;
	if(configuratorAgent != a){
		cmd->retcode = F8_DATABASE_LOCKED;
		return;
	}
	if(cmd->isize % sizeof(struct blk_var_t)){
		cmd->retcode = F8_INVALID_DATA;
		return;
	}
	count = cmd->isize / sizeof(struct blk_var_t);
	if(!ke_lock(a->kernel, 1)){
		cmd->retcode=F8_DATABASE_LOCKED;
		return;
	}
	ki_reset_var_table();
	if(!count){
		cmd->retcode = F8_SUCCESS;
		ke_unlock(a->kernel, 1);
		return;
	}
	cmd->retcode = ki_load_var_table(count, (struct blk_var_t*)cmd->ibuffer);
	if(F8_SUCCESSED(cmd->retcode)){
		a->kernel->sections->h.uuid = NEW_ID(cmd);
		a->kernel->status.prog_id = NEW_ID(cmd);
	}
	ke_unlock(a->kernel, 1);
	cmd->osize = 0;
}

LOCAL  void _uploadVartable(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	IMemStream ss;
	IF8Stream * s;
	
	__create__(IMemStream, &ss);
	IMemStream_bind(&ss, cmd->obuffer, cmd->osize);
	s = __ucast__(IMemStream, IF8Stream, &ss);

	ke_lock(a->kernel, 0);
	cmd->retcode = ki_save_var_table(s);
	ke_unlock(a->kernel, 0);
	if(F8_SUCCESSED(cmd->retcode)){
		cmd->osize = ss.pointer;
	}else{
		cmd->osize = 0;
	}
}

LOCAL  void _addVariable(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	struct blk_var_t * v;
	cmd->osize = 0;
	if(configuratorAgent != a){
		cmd->retcode = F8_DATABASE_LOCKED;
		return;
	}
	if(cmd->isize != sizeof(struct blk_var_t)){
		cmd->retcode = F8_INVALID_DATA;
		return;
	}
	v = (struct blk_var_t*)cmd->ibuffer;
	if(!ke_lock(a->kernel, 1)){
		cmd->retcode=F8_DATABASE_LOCKED;
		return;
	}
	cmd->retcode = ki_add_var(v);
	if(F8_SUCCESSED(cmd->retcode)){
		a->kernel->sections->h.uuid = NEW_ID(cmd);
		a->kernel->status.prog_id = NEW_ID(cmd);
	}
	ke_unlock(a->kernel, 1);
}

LOCAL void _editVariable(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	struct blk_var_t * v;
	cmd->osize = 0;
	if(configuratorAgent != a){
		cmd->retcode = F8_DATABASE_LOCKED;
		return;
	}
	if(cmd->isize != sizeof(struct blk_var_t)){
		cmd->retcode = F8_INVALID_DATA;
		return;
	}
	v = (struct blk_var_t*)cmd->ibuffer;
	if(!ke_lock(a->kernel, 1)){
		cmd->retcode=F8_DATABASE_LOCKED;
		return;
	}
	cmd->retcode = ki_edit_var(v);
	if(F8_SUCCESSED(cmd->retcode)){
		a->kernel->sections->h.uuid = NEW_ID(cmd);
		a->kernel->status.prog_id = NEW_ID(cmd);
	}
	ke_unlock(a->kernel, 1);
}

LOCAL  void _delVariable(struct kern_agent * a, struct kern_conf_cmd * cmd)
{
	f8_uuid * id;
	cmd->osize = 0;
	if(configuratorAgent != a){
		cmd->retcode = F8_DATABASE_LOCKED;
		return;
	}
	if(cmd->isize != sizeof(f8_uuid)){
		cmd->retcode = F8_INVALID_DATA;
		return;
	}
	id = (f8_uuid*)cmd->ibuffer;
	if(!ke_lock(a->kernel, 1)){
		cmd->retcode=F8_DATABASE_LOCKED;
		return;
	}
	cmd->retcode = ki_remove_var(id);
	if(F8_SUCCESSED(cmd->retcode)){
		a->kernel->sections->h.uuid = NEW_ID(cmd);
		a->kernel->status.prog_id = NEW_ID(cmd);
	}
	ke_unlock(a->kernel, 1);
}

LOCAL f8_status setConfigurator(struct kern_agent * a, int bSet)
{
	f8_status code;
	if(bSet){
		if(ke_lock(&g_kernel, 1)){
			if(configuratorAgent){
				code=F8_ACCESS;
			}else{
				code=F8_SUCCESS;
				configuratorAgent = a;
			}
			ke_set_flag(FKERN_LED_SOFT_LOCK,1);
			ke_unlock(&g_kernel, 1);
		}else{
			code=F8_ACCESS;
		}
	}else{
		if(a == configuratorAgent){
			configuratorAgent = 0;
			ke_lock(a->kernel, 0);
			ki_save_nvram(a->kernel);
			ke_unlock(a->kernel,0);
			ke_set_flag(FKERN_LED_SOFT_LOCK,0);
			ke_set_flag(FKERN_LED_SOFT_STOP,0);
		}
		code=F8_SUCCESS;
	}
	return code;
}

LOCAL __bool agentExecuteCmd(struct kern_agent *a, struct kern_conf_cmd * cmd)
{
	__bool ret = __false;
	unsigned char *buf2=0;
	unsigned long len;
	
// auto-decompression
	if(cmd->cmd & F8CMD_Compressed_Buffer){
		cmd->cmd &= ~F8CMD_Compressed_Buffer;
		len=*((f8_u32*)cmd->ibuffer);
		buf2=(unsigned char*)mm_alloc(kern_heap,len,0);
		if(!buf2){
			cmd->osize = 0;
			cmd->retcode = F8_LOW_MEMORY;
			if(!sendPacket(a->m_socket, (const char *)cmd, sizeof(*cmd))){
				goto __final__;
			}
			return __true;
		}

		//ok, we've got buffer, decompress it
		if(uncompress(buf2,&len,(unsigned char*)cmd->ibuffer+sizeof(f8_u32),cmd->isize)!=Z_OK){
			mm_free(kern_heap,buf2);
			buf2=0;
			if(!sendPacket(a->m_socket, (const char *)cmd, sizeof(*cmd))){
				goto __final__;
			}
			return __true;
		}

		// use the de-compressed data as input
		len=cmd->isize;
		cmd->isize=*((f8_u32*)cmd->ibuffer);
		cmd->ibuffer=buf2;
		// clear flags which might confuse ke_config
		cmd->cmd &= 0xff;
	}else{
		len=cmd->isize;
	}

	AGENT_LOG(("Got cmd:%d(%s), i=%d, xi=%d.\n", cmd->cmd, cpmap[cmd->cmd].name,len, cmd->isize));

	/*
	several commands are only understood by the running kernel,
	they are handled here.
	*/
	switch(cmd->cmd){
	case F8_LOCK_CONTROLLER:
		cmd->osize = 0;
		if(cmd->isize < sizeof(f8_i32)){
			cmd->retcode=F8_INVALID_DATA;
			break;
		}
		cmd->retcode=setConfigurator(a,*(f8_i32*)cmd->ibuffer);
		break;
		
	case F8_GET_LOCK_STATUS:
		if(cmd->osize < sizeof(struct sockaddr_in)){
			cmd->retcode = F8_INVALID_DATA;
			break;
		}
		ke_lock(a->kernel, 0);
		if(configuratorAgent){
			*((struct sockaddr_in*)cmd->obuffer) = configuratorAgent->m_peer;
		}else{
			memset(cmd->obuffer, 0, sizeof(struct sockaddr_in));
		}
		cmd->osize = sizeof(struct sockaddr_in);
		ke_unlock(a->kernel, 0);
		break;
		
	case F8_UPLOAD:
		_uploadFile(a, cmd);
		break;
		
	case F8_DOWNLOAD:
		_downloadFile(a, cmd);
		break;
		
	case F8_DOWNLOAD_VARTABLE:
		_downloadVartable(a, cmd);
		break;
		
	case F8_UPLOAD_VARTABLE:
		_uploadVartable(a, cmd);
		break;

	case F8_ADD_VARIABLE:
		_addVariable(a, cmd);
		break;

	case F8_DEL_VARIABLE:
		_delVariable(a, cmd);
		break;

	case F8_EDIT_VARIABLE:
		_editVariable(a,cmd);
		break;

	default:
		ke_config(a->kernel, cmd, (__bool)(configuratorAgent==a? 1 : 0));
		break;
	}

	AGENT_LOG(("Cmd processed, code %d, ", cmd->retcode));

// ok, input buffer is no longer used, de-allocate it
	if(buf2){
		mm_free(kern_heap,buf2);
		buf2=0;
	}

// auto-compression
	if(cmd->osize > 4096){
		len=cmd->osize*2;
		buf2=(unsigned char *)mm_alloc(kern_heap,len,0);
		len-=sizeof(f8_u32);
		if(buf2){
			if(compress(buf2+sizeof(f8_u32), &len, cmd->obuffer, cmd->osize)==Z_OK){
				cmd->obuffer=buf2;
				// record the original size
				*((f8_u32*)cmd->obuffer)=cmd->osize;
				cmd->osize=len+sizeof(f8_u32);
				cmd->cmd |= F8CMD_Compressed_Buffer;
			}else{
				// the original buffer is used
			}
		}
	}

	AGENT_LOG(("o=%d, xo=%d\n",
		cmd->cmd & F8CMD_Compressed_Buffer? *((f8_u32*)cmd->obuffer) : cmd->osize,
		cmd->cmd & F8CMD_Compressed_Buffer? len : 0
		));

	if(!sendPacket(a->m_socket, (const char *)cmd, sizeof(*cmd))){
		goto __final__;
	}

	if(cmd->osize){
		if(!sendPacket(a->m_socket, cmd->obuffer, cmd->osize)){
			goto __final__;
		}
	}

	ret = __true;

__final__:
	if(buf2){
		mm_free(kern_heap,buf2);
	}
	
	return ret;
}

LOCAL  __bool agentOnCmd(struct kern_agent * a)
{	
	struct kern_conf_cmd cmd;
	__bool ret = __false;
	static  int seqNo = 0;
	int bytes;
	char * heap=0;
	
	cmd.ibuffer = 0;
	// firstly we receive the command header
	// we expect this can be done with a single call of recv
	bytes = recv(a->m_socket, (char*)&cmd, sizeof(cmd), 0);
	// printf("%d bytes command got\n", bytes);
	if(bytes <= 0){
		goto __final__;
	}
	if(bytes < sizeof(struct kern_conf_cmd)){
		goto __final__;
	}
	if(cmd.isize + cmd.osize < sizeof(a->static_buf)){
		cmd.ibuffer = a->static_buf;
		cmd.obuffer = (char *)cmd.ibuffer + cmd.isize;
	}else if(cmd.isize + cmd.osize > 16*1024*1024){
		/* packet to large, cannot handle it */
		goto __final__;
	}else if(cmd.isize + cmd.osize != 0){
		heap=cmd.ibuffer = (char*)mm_alloc(kern_heap,cmd.isize + cmd.osize,0);
		if(0 == cmd.ibuffer){
			AGENT_LOG((
				"Agent %d - cmd=%d,isize=%d,osize=%d\n", 
				a - agents,
				cmd.cmd, 
				cmd.isize, 
				cmd.osize
				));
			cmd.osize = 0;
			cmd.retcode = F8_LOW_MEMORY;
			if(!sendPacket(a->m_socket, (const char *)&cmd, sizeof(cmd))){
				goto __final__;
			}
			if(!agentSkip(a, cmd.isize)){
				goto __final__;
			}
			return __true;
		}

		cmd.obuffer = (char *)cmd.ibuffer + cmd.isize;
	}else{
		cmd.ibuffer = 0;
		cmd.obuffer = 0;
	}

	// ok, the buffer is allocated, now we receive the full command
	// with all parameters
	// we expect there might be more than one calls to recv, so
	// a helper proc recvPacket is needed.
	if(cmd.isize){	
		if(!recvPacket(a->m_socket, cmd.ibuffer, cmd.isize)){
			printf("Agents %d - error receiving full command.\n", a - agents);
			goto __final__;
		}
	}

	ret=agentExecuteCmd(a, &cmd);
	
__final__:
	if(heap){
		mm_free(kern_heap,heap);
	}

	return ret;
}

void init_config();
#ifdef _WIN32
__uint RTK_API f8Agent(__uint foo)
#else
int f8Agent()
#endif
{
	F8_SOCKET s, s2;
	struct sockaddr_in inAddr, prAddr;
	int addrLen;
	int i;
	fd_set rset;
	int ret;
	struct timeval timeout;
	char agentName[16];
	struct kernel_t * kernel;

	taskFlag = TSK_RUNNING;
	kernel = &g_kernel;
	
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(IS_INVALID_SOCKET(s)){
		perror("agentSocket");
		goto __final__;
	}

	memset(&inAddr, 0, sizeof(inAddr));
	inAddr.sin_addr.s_addr = INADDR_ANY;
	inAddr.sin_family = AF_INET;
	inAddr.sin_port = htons(agentPort);
	if(bind(s, (const struct sockaddr*)&inAddr, sizeof(inAddr))){
		perror("agentBind");
		goto __final__;
	}
	if(listen(s, MAX_CLIENTS)){
		perror("listen");
		goto __final__;
	}	

	for(i=0; i<MAX_CLIENTS; i++){
		agents[i].m_taskId = ERROR;		
	}

	FD_ZERO(&rset);
	while(taskFlag != TSK_STOPPING){
		FD_SET(s, &rset);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		ret = select(s+1, &rset, NULL, NULL, &timeout);
		if(ret > 0){
			if(FD_ISSET(s, &rset)){
				FD_CLR(s, &rset);
				addrLen = sizeof(prAddr);
				memset(&prAddr, 0, sizeof(prAddr));
				s2 = accept(s, (struct sockaddr*)&prAddr, &addrLen);
				if(s2 != SOCKET_ERROR){
					printf(
						"Socket connection accepted from %s:%d\n",
						inet_ntoa(prAddr.sin_addr),
						ntohs(prAddr.sin_port)
						);
					for(i=0; i<MAX_CLIENTS; i++){
						if(ERROR == agents[i].m_taskId){
							break;
						}
					}
					if(i < MAX_CLIENTS){
						agents[i].m_socket = s2;
						sprintf(agentName, "f8Agent%d", i);
						agents[i].m_bStopRequestPending = __false;
						agents[i].m_peer = prAddr;
						agents[i].kernel = kernel;
#ifdef __VXWORKS__
						agents[i].m_taskId = taskSpawn(
							agentName,	/* name */
							52,			/* priority */
							VX_FP_TASK,	/* options */
							128*1024,	/* stack size */
							agentTask,	/* entry point */
							(int)&agents[i],	/* arg 1 */
							0,0,0,0,0,0,0,0,0	/* arg 2 ~ 10 */
							);
#elif defined(_WIN32)
						agents[i].m_taskId = _beginthread(
							agentTaskStub, 
							128*1024, 
							&agents[i]
							);
#else
						pthread_create(
							&agents[i].m_taskId,
							NULL,
							agentTaskStub, 
							&agents[i]
							);
#endif
					}else{
						AGENT_LOG(("Error : no more agent available.\n"));
						closesocket(s2);
					}
				}
			}
		}else if(ret < 0){
			AGENT_LOG(("KServer : master socket failure.\n"));
			break;
		}else{
			//
		}
	}
	
	closesocket(s);
	for(i=0; i<MAX_CLIENTS; i++){
		if(ERROR != agents[i].m_taskId){
			agents[i].m_bStopRequestPending = __true;
			while(ERROR != agents[i].m_taskId){
				miliSleep(1);
			}
		}
	}
	
__final__:
#ifndef _WIN32
	agentThreadId = ERROR;
#endif
	return 0;
}

