/*
	PC based Monitoring System source file
	version 1.0
	by J.Hunter,jhunter@263.net,July 2001

	This is the proxy implementation for the Serial Port Access Coordinator(SPAC).
*/
#include "precomp.h"

#pragma comment(linker, "/base:0x76620000")

	static HANDLE mutics[rtkm_max_spac_ports];
	static HANDLE events[rtkm_max_spac_ports][2];

#undef DEBUG_PRINTF
#define DEBUG_PRINTF(A) 0

spac_shm * ports; // 2013-1-28 Zander
/*
	Initialize SPAC library.
*/
int LIBSPAC_API PMC_API  init_spac()
{
	int i;
	char name[32];
	int res;
	try{
		PSECURITY_DESCRIPTOR	pSD;
		SECURITY_ATTRIBUTES		sa,*_sa;
		
		//分配一个security descriptor
		pSD = (PSECURITY_DESCRIPTOR) LocalAlloc( LPTR,
			SECURITY_DESCRIPTOR_MIN_LENGTH);
		
		if (pSD == NULL){
			DEBUG_PRINTF(("Error creating security descriptor.\n"));
			return 0;
		}
		
		//初始化security descriptor
		if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)){
			DEBUG_PRINTF(("Error initializing security descriptor.\n"));
			return 0;
		}
		
		// Add a NULL DACL to the security descriptor..
		// see MSDN Knowledge base Q106387 for detail
		//设置security descriptor
		if (!SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE)){
			DEBUG_PRINTF(("Error setting security descriptor.\n"));
			return 0;
		}

		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = TRUE;

		_sa=&sa;
		//_sa=NULL;
        //创建mutex对象和event对象，将返回的句柄赋给mutics[]和events[]
		for(i=0;i<rtkm_max_spac_ports;i++){
			sprintf(name,"rtkm_spac_mutex_%d",i+1);
			if(!mutics[i]) mutics[i]=CreateMutex(_sa,0,name);
			if(!mutics[i]) mutics[i]=OpenMutex(MUTEX_ALL_ACCESS,0,name);
			if(!mutics[i]){
				throw(-1);
			}
			sprintf(name,"rtkm_spac_event_0_%d",i+1);
			if(!events[i][0]) events[i][0]=CreateEvent(_sa, FALSE, FALSE, name);
			if(!events[i][0]) events[i][0]=OpenEvent(EVENT_ALL_ACCESS,false,name);
			if(!events[i][0]){
				throw(-2);
			}			 
			sprintf(name,"rtkm_spac_event_1_%d",i+1);
			if(!events[i][1]) events[i][1]=CreateEvent(_sa, FALSE, 0, name);
			if(!events[i][0]) events[i][1]=OpenEvent(EVENT_ALL_ACCESS,false,name);
			if(!events[i][1]){
				throw(-3);
			}			 
		}
		res=1;
	}catch(int){
		DEBUG_PRINTF(("Error creating mutics, code %d,%d.\n", e,GetLastError()));
		uninit_spac();
		res=0;
	}
	return res;
}

/*
	UnInitialize SPAC library.
*/
int LIBSPAC_API PMC_API  uninit_spac()
{
	int i;
	for(i=0;i<rtkm_max_spac_ports;i++){
		if(mutics[i]){
			CloseHandle(mutics[i]);
			mutics[i]=0;
		}
		if(events[i][0]) {
			CloseHandle(events[i][0]);
			events[i][0]=0;
		}
		if(events[i][1]) {
			CloseHandle(events[i][1]);
			events[i][1]=0;
		}
	}
	return 1;
}

/*
	Acquire exclusive access to the specified port.
*/
int LIBSPAC_API PMC_API  lock_port(int port_no,int time_out)
{
	int res;
	if(port_no<1 || port_no>rtkm_max_spac_ports){
		DEBUG_PRINTF((">> in lock_port(),%d : No that port\n",port_no));
		return 0;
	}
	res=WaitForSingleObject(mutics[port_no-1],time_out);
	if(WAIT_TIMEOUT == res){
		DEBUG_PRINTF((">>lock_port : Wait failed.\n"));
		return 0;
	}
	return 1;
}

/*
	Release exclusive access to the specified port.
*/
int LIBSPAC_API PMC_API  unlock_port(int port_no)
{
	if(port_no<1 || port_no>rtkm_max_spac_ports){
		DEBUG_PRINTF((">> in unlock_port(),%d : No that port.\n",port_no));
		return 0;
	}
	ReleaseMutex(mutics[port_no-1]);
	return 0;
}

/*
	Read from port.
	The port MUST!!! be locked before reading.
*/
int LIBSPAC_API PMC_API  read_port(int pno, char * buf, int *size,int timeout)
{	
	if(pno<1 || pno>rtkm_max_spac_ports){
		DEBUG_PRINTF((">> in read_port(),%d : No that port.\n",pno));
		return 0;
	}
	spac_shm * shm=&ports[pno-1];
	/* prepare command packet */
	shm->i.cmd_code=SPAC_READ; /* reading */
	shm->i.param[0]=*size; 
	shm->o.ret_code=0;
	*size=0;
	ResetEvent(events[pno-1][1]);
	SetEvent(events[pno-1][0]);
	if( WAIT_TIMEOUT == WaitForSingleObject(events[pno-1][1], timeout) ){
		DEBUG_PRINTF((">> in read_port : wait failed.\n"));
		return 0;
	}
	/* failed? */
	if(1 != shm->o.ret_code){
		DEBUG_PRINTF((">> in read_port : read failed.\n"));
		return 0;
	}
	/* receive data */
	*size=shm->o.param[0];
	memcpy(buf,(char*)shm->buffer,*size);
	return 1;
}

/*
	Write to port.
	The port MUST!!! be locked before writing.
*/
int LIBSPAC_API PMC_API  write_port(int pno, char * buf, int size,int timeout)
{
	if(pno<1 || pno>rtkm_max_spac_ports){
		DEBUG_PRINTF((">> in write_port(),%d : No that port.\n",pno));
		return 0;
	}
	if(size > rtkm_spac_buffer_length){
		DEBUG_PRINTF((">> in write_port(),%d : buffer too large.\n",size));
		return 0;
	}
	spac_shm * shm = &ports[pno-1];
	/* prepare command packet */
	shm->i.cmd_code=SPAC_WRITE; /* writing */
	shm->i.param[0]=size;
	memcpy((char*)shm->buffer,buf,size);
	ResetEvent(events[pno-1][1]);
	SetEvent(events[pno-1][0]);
	if( WAIT_TIMEOUT == WaitForSingleObject(events[pno-1][1],timeout) ){
		DEBUG_PRINTF((">> in write_port : Wait failed.\n"));
		return 0;
	}
	return 1;
}

int LIBSPAC_API PMC_API  flush_port(int pno, int timeout)
{
	if(pno<1 || pno>rtkm_max_spac_ports){
		DEBUG_PRINTF((">> in open_port(),%d : No that port.\n", pno));
		return 0;
	}
	spac_shm * shm = &ports[pno-1];
	/* prepare command packet */
	shm->i.cmd_code = SPAC_FLUSH; /* flush */
	shm->o.ret_code = 0;
	ResetEvent(events[pno-1][1]);
	SetEvent(events[pno-1][0]);
	if( WAIT_TIMEOUT == WaitForSingleObject(events[pno-1][1], timeout) ){
		DEBUG_PRINTF((">> in flush_port : Wait failed.\n"));
		return 0;
	}
	return 1;
}

int LIBSPAC_API PMC_API  open_port(int pno, int timeout)
{
	if(pno<1 || pno>rtkm_max_spac_ports){
		DEBUG_PRINTF((">> in open_port(),%d : No that port.\n", pno));
		return 0;
	}
	spac_shm * shm = &ports[pno-1];
	/* prepare command packet */
	shm->i.cmd_code = SPAC_OPEN; /* open */
	shm->o.ret_code = 0;
	ResetEvent(events[pno-1][1]);
	SetEvent(events[pno-1][0]);
	if( WAIT_TIMEOUT == WaitForSingleObject(events[pno-1][1],timeout) ){
		DEBUG_PRINTF((">> in open_port : Wait failed.\n"));
		return 0;
	}
	return 1;
}

int LIBSPAC_API PMC_API  close_port(int pno, int timeout)
{
	if(pno<1 || pno>rtkm_max_spac_ports){
		DEBUG_PRINTF((">> in close_port(),%d : No that port.\n",pno));
		return 0;
	}
	spac_shm * shm = &ports[pno-1];
	/* prepare command packet */
	shm->i.cmd_code = SPAC_CLOSE; /* close */
	shm->o.ret_code = 0;
	ResetEvent(events[pno-1][1]);
	SetEvent(events[pno-1][0]);
	if( WAIT_TIMEOUT == WaitForSingleObject(events[pno-1][1], timeout) ){
		DEBUG_PRINTF((">> in close_port : Wait failed.\n"));
		return 0;
	}
	return 1;
}
