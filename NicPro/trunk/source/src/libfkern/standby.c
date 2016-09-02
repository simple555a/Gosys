#define HSP_DEBUG
/*
2005-10-10 jht
stop/wait protocol is used as the flow control mechanism for
primary/standby communication over a 100m ethernet.

The runtime behaviour of the primary and standby site are 
both modelled with a finite state machine(fsm), so it's better 
to have some knowledge of fsm for fully understanding this
implementation.

*/

#include "precomp.h"
#pragma hdrstop

#include "zlib.h"

#ifndef LOCAL
#define LOCAL static
#endif

/*
fsm actions
*/
#define Pr_connect 1
#define Pr_send_volatile 2
#define Pr_query_status 3
#define Pr_disconnect 4
#define Pr_abort 5
#define Pr_ack 0x8000

void mode_show();

/* marshalled command packet */
struct peer_cmd_t{
	char tag[6]; /* for sniffer use */
	f8_u16 cmd;
	f8_status code;
	f8_u16 port;
	int ack; /* not really used for the stop/wait protocol, but might be used later */
	int seq;
	ktime_t time;
	f8_u32 params[4];
};

/* fsm states */
enum hsp_state_t{
	hsp_s_stopped=0,
	hsp_s_idle=1,
	hsp_s_connecting=2,
	hsp_s_arbitrate=3,

	hsp_s_active=8,

/*final states, above 10*/
	hsp_s_linedown=10,
	hsp_s_timeout=11,
	hsp_s_out_of_sync=12,
	hsp_s_aborted=13,
	hsp_s_complete=14,
	hsp_s_collision=15,
};

#ifdef __VXWORKS__
#if defined(HSP_DEBUG)
int hspDebug;
#define hsp_log(x) if(hspDebug && kernelState!=KERN_S_HALTED)\
	logMsg("%d:", tickGet(),2,3,4,5,6), logMsg x
#else
#define hsp_log(x)
#endif
#include "vxworks/ether_peer.c"
#elif defined(_WIN32)
#if defined(HSP_DEBUG)
#define hsp_log(x) //printf x
#else
#define hsp_log(x)
#endif
#include "win32/udp_peer.c"
#endif

#include "standby_socket.c"

LOCAL int IPeer_write(IPeer * _this, const void * buf, int len)
{
	return F8_UNIMPLEMENTED;
}

LOCAL int IPeer_read(IPeer * _this, void * buf, int len, int timeout)
{
	return F8_UNIMPLEMENTED;
}

LOCAL int IPeer_lock(IPeer * _this)
{
	return 0;
}

LOCAL void IPeer_unlock(IPeer * _this, int cookie)
{
}

LOCAL int IPeer_write_zero_copy(IPeer * _this,...)
{
	va_list args;
	void * buf=(void*)-1;
	int len,totalLen=0;
	char rtu[PEER_RTU];
	char *p;

	va_start(args, _this);
	p=rtu;
	while(buf){
		buf = va_arg(args, void *);
		len = va_arg(args, int);
		if(buf){
			if(len+totalLen <= PEER_RTU){
				memcpy(p, buf, len);
				p += len;
				totalLen += len;
			}else{
				return F8_BUFFER_TOO_SMALL;
			}
		}
	}
	return __vcall__(_this, write, (_this, rtu, totalLen));
}

void IPeer_global_create()
{
	__set_vfunc__(IPeer, write);
	__set_vfunc__(IPeer, read);
	__set_vfunc__(IPeer, write_zero_copy);
	__set_vfunc__(IPeer, lock);
	__set_vfunc__(IPeer, unlock);
#ifdef INCLUDE_STANDBY_SOCKET
{
	int i;
	RtkInitializeListHead(&freeSocks);
	RtkInitializeListHead(&activeSocks);
	for(i=0;i<MAX_CMD_SOCKS;i++){
		RtkInsertTailList(&freeSocks, &cmdSocks[i].l);
	}
}
#endif
}

__constructor__(IPeer)
{
	__this->eventProc = 0;
	memset(__this->peerAddress, 0, sizeof(__this->peerAddress));
	return __this;
}

__destructor__(IPeer)
{
	return __this;
}

__define_vtbl__(IPeer)
__define_vtbl_end__(IPeer)


f8_u8 peerData[F8_VOLATILE_MEM_SIZE];
f8_u8 peerDataZipped[F8_COMPRESSED_VOLATILE_SIZE];
struct kpeer_hdr_t peerHdr;
f8_u8 * peerPointer;
f8_u8 * peerGuardian;
ktime_t hspTimeOffset;
int hspConnTimeout=HSP_CONNECTION_TIMEOUT;

#if defined(HSP_DEBUG)
LOCAL ktime_t tBase;
#endif

/* tune this for performance and reliability */
f8_uint primaryLife=DEFAULT_PRIMARY_LIFE;

#define ki_get_primary_life() (primaryLife*10*1000)

/* primary/standby communication, state machine variables*/
LOCAL volatile f8_uint lastFrameOffset;
LOCAL volatile f8_uint lastFrameLength;
LOCAL volatile enum hsp_state_t _state;
LOCAL int volatile peerCounters[10];

const char * state_name(enum hsp_state_t s)
{
#define _(n) case hsp_s_##n: return #n
	switch(s){
		_(stopped);
		_(idle);
		_(connecting);
		_(arbitrate);
		_(active);
		_(linedown);
		_(timeout);
		_(out_of_sync);
		_(aborted);
		_(complete);
		_(collision);
	default:
		assert(0);
		return "unknown";
	}
#undef _
}

#define get_state() _state
#if defined(HSP_DEBUG)
void _set_state(enum hsp_state_t s,const char * fnc, int line)
{
	if(s == _state){
		return;
	}
#if defined(HSP_DEBUG)
	{
		ktime_t t;
		t = ke_get_time() - tBase;
		hsp_log(("%d, state change %s->%s, file %s,line %d\n", (int)(t/10000),state_name(_state),state_name(s),fnc,line));
	}
#endif
	_state = s;
}
#define set_state(s) _set_state(s,__FILE__,__LINE__)
#else
#define set_state(s) _state = s 
#endif

/*
	primary site fsm
*/
LOCAL void primary_on_packet(struct peer_cmd_t * tp, int len)
{
	char * data;
	int dataLen;
	static int bytesLeft;

	data = (char*)&tp[1];
	dataLen = len - sizeof(*tp);
	if(dataLen < 0){
		return;
	}

	if(tp->cmd == Pr_connect || tp->cmd == Pr_send_volatile){
		tp->cmd = Pr_abort;
		tp->code = F8_COLLISION;
		tp->time = ke_get_time();
		peer_write(tp, sizeof *tp);
		set_state(hsp_s_collision);
		return;
	}

	/* switch-case structure is the classical form of a software fsm
	implementation.
	*/
	switch(get_state()){
	case hsp_s_connecting:
		if(tp->cmd == (Pr_connect | Pr_ack)){
			if(dataLen >= sizeof(g_kernel.peer_status)){
				memcpy(&g_kernel.peer_status, data, sizeof g_kernel.peer_status);
			}
			if(F8_FAILED(tp->code)){
				if(tp->code == F8_COLLISION){
					set_state(hsp_s_collision);
				}else{
					set_state(hsp_s_aborted);
				}
				break;
			}
			/* connection accepted, send first data packet */
			tp->cmd = Pr_send_volatile;
			tp->code = F8_SUCCESS;
			tp->seq = 0;
			tp->ack = 0;
			bytesLeft = peerHdr.zipped_data_len;
			if(bytesLeft < PEER_RTU - sizeof *tp){
				lastFrameLength = bytesLeft;
			}else{
				lastFrameLength = PEER_RTU - sizeof *tp;
			}
			tp->params[0] = lastFrameOffset = 0;
			tp->params[1] = lastFrameLength;
			tp->time = ke_get_time();
			if(peer_write2(tp, sizeof(*tp),peerDataZipped,lastFrameLength) == 0){
				set_state(hsp_s_active);
			}else{
				set_state(hsp_s_linedown);
			}
			hsp_log(("connection built, sending first packet %d bytes.\n", lastFrameLength));
		}
		break;
		
	case hsp_s_active:
		/* */
		if(tp->cmd == (Pr_send_volatile | Pr_ack)){
			if(tp->params[0] != lastFrameOffset){
				/* out of sync */
				hsp_log(("out of sync, bad offset %08x, expected %08x\n", tp->params[0], lastFrameOffset));
				peerCounters[4] = tp->params[0];
				set_state(hsp_s_out_of_sync);
				break;
			}
			lastFrameOffset += lastFrameLength;
			tp->params[0] = lastFrameOffset;
			bytesLeft -= lastFrameLength;
			if(!bytesLeft){
				tp->cmd = Pr_disconnect;
				tp->code = F8_SUCCESS;
				tp->time = ke_get_time();
				/* tp->seq = 0x12345678;  magic */
				peer_write(tp, sizeof(*tp));
				set_state(hsp_s_complete);
				peerCounters[2]++;
				break;
			}
			if(bytesLeft < PEER_RTU - sizeof *tp){
				lastFrameLength = bytesLeft;
			}else{
				lastFrameLength = PEER_RTU - sizeof *tp;
			}
			tp->cmd = Pr_send_volatile;
			tp->code = F8_SUCCESS;
			tp->params[1] = lastFrameLength;
			tp->time = ke_get_time();
			if(peer_write2(tp,sizeof(*tp),peerDataZipped+lastFrameOffset,lastFrameLength) < 0){
				set_state(hsp_s_linedown);
			}
		}else if(tp->cmd == Pr_abort){
			if(tp->code == F8_COLLISION){
				set_state(hsp_s_collision);
			}else{
				set_state(hsp_s_aborted);
			}
		}else{
			hsp_log(("out of sync, bad command %04x\n", tp->cmd));
			set_state(hsp_s_out_of_sync);
		}
		break;
		
	default:
		/* only query_status cmd is processed, other cmds are ignored */
		;
		break;
	}
}


LOCAL f8_status primary_sync()
{
	struct peer_cmd_t tp;
	int i, cookie;
	enum hsp_state_t st;
	long size2;
	
	st = get_state();
	if(st == hsp_s_collision){
		if(!check_priority()){
			ki_switch_to(KERN_S_STANDBY);
			return F8_SUCCESS;
		}
	}
	
	ki_save_volatile(&g_kernel);

	cookie = peer_lock();

	/* prepare volatile data */
	size2=sizeof(peerDataZipped);
	if(compress(peerDataZipped, &size2, peerData, peerPointer-peerData) == Z_OK){
		peerHdr.zipped_data_len=size2;
	}else{
		/* might continue with un-compressed data */
		return F8_LOW_MEMORY;
	}
	
	/* start automata */
	// set_state(hsp_s_connecting);
	_state = hsp_s_connecting;
	memset(&tp, 0, sizeof(tp));
	tp.cmd = Pr_connect;
	tp.code = F8_SUCCESS;
	tp.time = ke_get_time();
	//peer_flush();
	if(peer_write3(&tp, sizeof(tp), &g_kernel.status, sizeof(g_kernel.status), &peerHdr, sizeof(peerHdr)) < 0){
		set_state(hsp_s_idle);
		peer_unlock(cookie);
		return F8_CONNECTION_LOST;
	}
	peer_unlock(cookie);

	/* wait 5 times to see if connection established */
	for(i=0; i<5; i++){
		if(get_state() == hsp_s_active || get_state() >= 10){
			break;
		}
		miliSleep(hspConnTimeout);
	}
	st = get_state();
	if(st == hsp_s_active){
		/* expected transfer time */
		////i = size2*8/HSP_LINE_SPEED;
		////if(!i) i=1;
		// i is packet count, chenj 2009-3-5
		i=size2/1024+1;
		while(i > 0){
			if(get_state() >= 10){
				break;
			}			
			////miliSleep(hspConnTimeout);
			////i -= hspConnTimeout;
			miliSleep(2);
			i--;
		}
	}
	st = get_state();
	// set_state(hsp_s_idle);
	_state = hsp_s_idle;

	if(st == hsp_s_complete){
		return F8_SUCCESS;
	}else if(st == hsp_s_collision){
		ki_switch_to(KERN_S_ARBITRATING);
	}else{
		peerCounters[3] = st;
		peerCounters[5]++;
		// hsp_log(("primary timeout %d\n", i));
		/* reset peer state */
		memset(&g_kernel.peer_status, 0, sizeof g_kernel.peer_status);
	}
	
	return F8_TIMEOUT;
}


/*
	In standby phase, both standby station and primary station
	are operated in a state-machine-driven manner. the automata
	accepts two types of events: 1) when peer layer gives us a
	packet; 2) when standby phase of execution cycle occurs or
	timer ticks if auxClock-based-timeout-detection is enabled. 

	Note the first event is (possibly) triggered in an interrupt context.
	We must protect the data manipulated by the event proc, to
	this end, peer layer has provided two procs: peer_lock/peer_unlock.

	Data that must be protected include:
	1) g_kernel.status, read by the event proc
	2) g_kernel.peer_status, read/write by the event proc
	3) automata state, read/write by the event proc

	standby_on_packet()
	- runs in the interrupt context
*/

LOCAL void standby_on_packet(struct peer_cmd_t * tp, int len)
{
	char * data;
	int dataLen;
	
	data = (char*)&tp[1];
	dataLen = len - sizeof(*tp);
	
	switch(get_state()){
	case hsp_s_idle:
		/* respond to connection command */
		if(tp->cmd != Pr_connect){
			/* invalid command, stay in in-active state */
			break;
		}

		if(dataLen != sizeof(g_kernel.peer_status)+sizeof peerHdr){
			tp->cmd = Pr_connect | Pr_ack;
			tp->code = F8_INVALID_DATA;
			tp->time = ke_get_time();
			peer_write2(tp, sizeof(*tp), &g_kernel.status, sizeof(g_kernel.status));
			break;
		}

		memcpy(&g_kernel.peer_status, data, sizeof(g_kernel.peer_status));

#if 0
		if(ke_get_peer_flag(FKERN_LED_SOFT_LOCK) || kernelState==KERN_S_STOPPED){
			/* no sync phase is performed while either controller is under controll 
			of a configuration agent */
			tp->code = F8_DATABASE_LOCKED;
			set_state(hsp_s_aborted);
		}else 
#endif
		if(g_kernel.status.prog_id == g_kernel.peer_status.prog_id){
			memcpy(&peerHdr, data+sizeof g_kernel.status, sizeof peerHdr);
			tp->code = F8_SUCCESS;
			/* ok, establish connection */
			/* start new session */
			lastFrameOffset = 0;
			lastFrameLength = 0;
			set_state(hsp_s_active);
		}else{
			tp->code = F8_VERSION_MISMATCH;
			set_state(hsp_s_aborted);
		}

		if(peerHdr.zipped_data_len > sizeof(peerDataZipped)){
			tp->code=F8_LOW_MEMORY;
			set_state(hsp_s_aborted);
		}
		
		tp->cmd |= Pr_ack;
		tp->time = ke_get_time();
		peer_write2(tp, sizeof(*tp), &g_kernel.status, sizeof(g_kernel.status));
		break;

	case hsp_s_active:
		/* the active state, ready to accept data */
		if(tp->cmd == Pr_send_volatile){
			/* upload data request */
			#if 0
			hsp_log(("got piece %08x,length=%d,we have %08x/%d\n", tp->params[0], tp->params[1],lastFrameOffset,lastFrameLength));
			#endif
			if(tp->params[0] != lastFrameOffset+lastFrameLength && tp->params[0] != lastFrameOffset){
				/* out of synchronization, terminate session */
				hsp_log(("Out of synchronization,%x,expected %x\n", tp->params[0], lastFrameOffset+lastFrameLength));
				/* dumpBuf(e, len); */
				tp->cmd = Pr_abort;
				tp->code = F8_OUT_OF_SYNCHRONIZATION;
				tp->time = ke_get_time();
				peer_write(tp, sizeof(*tp));
				peerCounters[4] = tp->params[0];
				set_state(hsp_s_out_of_sync);
				break;
			}
			if(tp->params[0] == lastFrameOffset+lastFrameLength){
				if(tp->params[0]+tp->params[1] > peerHdr.zipped_data_len){
					hsp_log(("Out of synchronization, out of memory\n"));
					/* dumpBuf(e, len); */
					tp->cmd = Pr_abort;
					tp->code = F8_LOW_MEMORY;
					tp->time = ke_get_time();
					peer_write(tp, sizeof(*tp));
					set_state(hsp_s_out_of_sync);
					break;
				}
				/* ok quite valid packet*/
				lastFrameOffset = tp->params[0];
				lastFrameLength = tp->params[1];
				memcpy(peerDataZipped+lastFrameOffset, data, lastFrameLength);
			}
			tp->cmd |= Pr_ack;
			tp->code = F8_SUCCESS;
			tp->time = ke_get_time();
			tp->params[0] = lastFrameOffset;
			tp->params[1] = lastFrameLength;
			if(peer_write(tp, sizeof *tp) < 0){
				set_state(hsp_s_linedown);
				break;
			}
		}else if(tp->cmd == Pr_abort){
			/* client has terminated session */
			set_state(hsp_s_aborted);
		}else if(tp->cmd == Pr_disconnect){
			if(lastFrameOffset + lastFrameLength == peerHdr.zipped_data_len){
				set_state(hsp_s_complete);
			}else{
				set_state(hsp_s_aborted);
			}
		}else{
			/* invalid command, ignore it */
			hsp_log(("out of synchronization, bad command %04x, expecting %x\n", tp->cmd, 
				lastFrameOffset+lastFrameLength));
			set_state(hsp_s_out_of_sync);
		}
		break;

	default:
		;
		break;
	}
}

/*
	standby_sync()
	- standby phase of the BPC
*/
LOCAL f8_status standby_sync()
{
	int cookie;
	ktime_t t;
	f8_uint i, size;
	long size2;
	f8_status ret = F8_BUSY;
	f8_u8 st;
	static ktime_t syncTime;
	
	t = ke_get_time();

	/* check automata */
	cookie = peer_lock();

	st = get_state();
	switch(st){
	case hsp_s_complete:
		size = 0;
		ke_toggle_flag(FKERN_LED_DBG1);
		syncTime=t;
		ke_set_flag(FKERN_LED_SYNCHRONIZED,1);
		for(i=0; i<KERN_NUM_SECTIONS; i++){
			if(peerHdr.x_mem_sizes[i] != g_kernel.x_mem_sizes[i]){
				ret = F8_VERSION_MISMATCH;
				set_state(hsp_s_idle);
				goto __done;
			}
			size += peerHdr.x_mem_sizes[i];
		}
		size += sizeof(struct marshalled_timer_t) * peerHdr.timer_q_len +
			sizeof(struct marshalled_event_t) * peerHdr.event_q_len +
			peerHdr.i_mem_size;
		if(size + sizeof(struct kpeer_hdr_t) > F8_VOLATILE_MEM_SIZE){
			ret = F8_LOW_MEMORY;
			set_state(hsp_s_idle);
			goto __done;
		}

		size2=sizeof(peerData);
		if(uncompress(peerData, &size2, peerDataZipped, peerHdr.zipped_data_len) != Z_OK){
			ret=F8_INVALID_DATA;
			set_state(hsp_s_idle);
			break;
		}
		
		/* indicate how much memory we can use */
		peerGuardian = peerData + size2;
		ki_load_volatile(&g_kernel);
		
		/* start another session */
		//peer_flush();
		set_state(hsp_s_idle);
		ret = F8_SUCCESS;
		peerCounters[1]++;
		break;

	case hsp_s_idle:
		break;
		
	case hsp_s_active:
		if(!ke_get_flag(FKERN_LED_SYNCHRONIZED)){
			//peer_flush();
			set_state(hsp_s_idle);
		}
		break;

	default:
		/* last error */
		peerCounters[5]++;
		peerCounters[3] = st;
		//peer_flush();
		set_state(hsp_s_idle);
		break;
	}
	
__done:	
	peer_unlock(cookie);

	if(t>syncTime+ki_get_primary_life()*5)
		ke_set_flag(FKERN_LED_SYNCHRONIZED,0);

	if(!ke_get_flag(FKERN_LED_SOFT_STOP)){
		if(t > g_kernel.peer->lastMsgTime + ki_get_primary_life()||
			(peerState!=KERN_S_RUNNING && !ke_get_peer_flag(FKERN_LED_SOFT_LOCK))
		){
			/* primary failure detected */
			/* try switch to primary state */
			if(g_kernel.peer_status.prog_id == g_kernel.status.prog_id && ke_get_flag(FKERN_LED_SYNCHRONIZED)){
				/* adjust kernel clock */
				kern_time_bias += hspTimeOffset;
				ki_log(&g_kernel, F8_PRIMARY_FAILURE,0,0,0);
				ki_switch_to(KERN_S_ARBITRATING);
				set_state(hsp_s_idle);
			}
			/* reset peer to unknown state */
			memset(&g_kernel.peer_status, 0, sizeof g_kernel.peer_status);
		}
	}
	
	return ret;
}

LOCAL void peer_rx_event(struct peer_cmd_t * tp, int len)
{
#if 0
		logMsg("%d bytes from 81x9\n", len);
#endif
#if defined(HSP_DEBUG)
#if 0
	{
		static int lastCmd;
		static int lastCount;
		if(tp->cmd == lastCmd){
			lastCount++;
		}else{
			hsp_log(("got cmd:%04x, %d times,code=%d,p0=%x,p1=%x\n", lastCmd, lastCount,tp->code,tp->params[0],tp->params[1]));
			lastCmd = tp->cmd;
			lastCount = 1;
		}
	}
#else
	hsp_log(("got cmd:%04x,state=%d,code=%d,p0=%x,p1=%x\n", tp->cmd,kernelState,tp->code,tp->params[0],tp->params[1]));
#endif
#endif
	
	peerCounters[0]++;
	hspTimeOffset = tp->time - g_kernel.peer->lastMsgTime;

	if(tp->cmd == Pr_query_status){
		tp->cmd |= Pr_ack;
		tp->code = F8_SUCCESS;
		tp->time = ke_get_time();
		peer_write3(tp, sizeof *tp, &g_kernel.status, sizeof g_kernel.status, &peerHdr, sizeof peerHdr);
		return;
	}else if(tp->cmd == (Pr_query_status | Pr_ack)){
		if(len == sizeof *tp + sizeof(g_kernel.peer_status) + sizeof(peerHdr)){
			memcpy(&g_kernel.peer_status, &tp[1], sizeof g_kernel.peer_status);
			if(get_state() == hsp_s_arbitrate){
				set_state(hsp_s_complete);
			}
		}
		return;
	}

#if 0	
	if(m.verbose)
		logMsg("cmd %d,len %d,offset 0x%x\n", tp->cmd,tp->params[1], tp->params[0]);
#endif

	switch(kernelState){
	case KERN_S_RUNNING:
		primary_on_packet(tp, len);
		break;
		
	case KERN_S_STANDBY:
		standby_on_packet(tp, len);
		break;

	case KERN_S_ARBITRATING:
		// logMsg("Arbitrating...\n");
		break;
		
	default:
			/* ignore it */
		;
	}
}

/*
	fsm driver routine-1, this procedure might be called
	from arbitrary context
*/
LOCAL void peer_event(int code, void * e1, int e2,int e3,int e4)
{
	int c;
	ke_toggle_flag(FKERN_LED_BUSB);
	switch(code){
	case HSE_PEER_COLLISION:
		c=peer_lock();
		set_state(hsp_s_collision);
		peer_unlock(c);
		break;
	case HSE_PEER_RX:
		g_kernel.peer->lastMsgTime = ke_get_time();
		if(e2 < sizeof(struct peer_cmd_t)){
			return;
		}
		peer_rx_event((struct peer_cmd_t*)e1,e2);
		break;
	case HSE_PEER_STATUS_BROADCAST:
		c=peer_lock();
		/*reset watchdog*/
		g_kernel.peer->lastMsgTime=ke_get_time();
#ifdef __VXWORKS__
		memcpy(g_kernel.peer->peerAddress,((struct ether_header*)e1)->ether_shost,6);
#endif
		g_kernel.peer_status = *((struct kstatus_t*)e2);
		if(kernelState == KERN_S_RUNNING && peerState==KERN_S_RUNNING){
			ki_log(&g_kernel, F8_IOBUS_COLLISION,0,0,0);
			set_state(hsp_s_collision);
		}
		peer_unlock(c);
		break;
	}	
}

/*
	void peer_arbitrate()
	
	setup the initial kernel state by monitoring hot-standby port
	and IO bus activities.
*/
LOCAL void peer_arbitrate() 
{
	int cookie;
	f8_u8 st;
	int i;
	
	struct peer_cmd_t tp;

	for(i=0;i<3;i++){
		cookie = peer_lock();
		set_state(hsp_s_arbitrate);
		tp.cmd = Pr_query_status;
		tp.code = F8_SUCCESS;
		tp.time = ke_get_time();
		//peer_flush();
		peer_write(&tp, sizeof tp);
		peer_unlock(cookie);
		
		miliSleep(HSP_ARBITRATION_TIMEOUT);
		
		/* is there an answer? */
		cookie = peer_lock();
		st = get_state();
		set_state(hsp_s_idle);
		peer_unlock(cookie);
		if(st==hsp_s_complete)
			break;
	}
	// mode_show();
	// logMsg("Peer is %s, state=%s\n", ke_get_mode_name(peerState, state_name(st)));
	
	if(st == hsp_s_complete){
		switch(peerState){
		case KERN_S_RUNNING:
			ki_switch_to(KERN_S_STANDBY);
			return;
		case KERN_S_ARBITRATING:
			if(check_priority())
				ki_switch_to(KERN_S_RUNNING);
			else
				ki_switch_to(KERN_S_STANDBY);
			break;
		default:
			ki_switch_to(KERN_S_RUNNING);
			break;
		}
	}else{
		ki_switch_to(KERN_S_RUNNING);
	}

	hsp_log(("arbitration finished, mode is:%d\n", kernelState));

	// ki_switch_to(KERN_S_HALTED);
}

/*
	synchronize primary site and standby site
	runs in the standby phase of a BPC
*/
void peer_sync()
{
	static ktime_t t;
	ktime_t t2=ke_get_time();
	
	if(!t)
		t=ke_get_time();

	if(t2-t>ki_get_primary_life()/10){
		/* broadcast station status */
		io_write_status();
		t=t2;
	}
	
	if(KERN_S_RUNNING == kernelState){
		primary_sync();
	}else if(KERN_S_ARBITRATING==kernelState){
		peer_arbitrate();
	}else{
		miliSleep(20);
		standby_sync();		
	}
}

void init_peer()
{
	//printf("Initializing peer layer.\n");
	peerGuardian = peerData + sizeof peerData;
	peerPointer = peerData;
	create_peer();
	g_kernel.peer->eventProc = peer_event;

#if defined(HSP_DEBUG)
	tBase = ke_get_time();
#endif

	memset((void*)peerCounters, 0, sizeof(peerCounters));

	/* note in VxWorks the auxClock is a shared resource and no
	arbitration mechanism is provided, so only diagnostic code can
	be safely put into _peer_ping.
	
	e.g., windView will use auxClock for profiling purposes.
	*/
#if defined(__VXWORKS__) && 0
	sysAuxClkConnect(_peer_ping, 0);
	sysAuxClkRateSet(20); /* ping peer every 50ms */
	sysAuxClkEnable();
#endif
}

void uninit_peer()
{
#if defined(__VXWORKS__) && 0
	sysAuxClkDisable ();
	sysAuxClkConnect(0, 0);
#endif
	g_kernel.peer->eventProc = 0;
	destroy_peer();
}

/*
	diagnostic code
*/
void peer_show()
{
	int i;
	__u16 * pp;
	__u8 * p8;
	char tbuf[64];
	ktime_t t;
	
	printf("Mode=%s,state=%s,flags=%08x,peermode=%s,%s.\n", ke_get_mode_name(kernelState), 
		state_name(get_state()), g_kernel.status.flags, ke_get_mode_name(peerState),
		ke_get_flag(FKERN_LED_SYNCHRONIZED)? "Synchronized":"Out of sync");
	pp=(__u16*)&g_kernel.status.prog_id;
	printf("  version : %04x-%04x-%04x-%04x\n", pp[0], pp[1], pp[2], pp[3]);
	pp=(__u16*)&g_kernel.peer_status.prog_id;
	printf("p-version : %04x-%04x-%04x-%04x\n", pp[0], pp[1], pp[2], pp[3]);
	p8=(__u8*)g_kernel.peer->localAddress;
	printf("Local addr: %02x-%02x-%02x-%02x-%02x-%02x\n",p8[0],p8[1],p8[2],p8[3],p8[4],p8[5]);
	p8=(__u8*)g_kernel.peer->peerAddress;
	printf("Peer addr : %02x-%02x-%02x-%02x-%02x-%02x\n",p8[0],p8[1],p8[2],p8[3],p8[4],p8[5]);
	t=ke_get_time();
	ke_time_to_string(t,tbuf,sizeof(tbuf));
	printf("Local time: %s\n",tbuf);
	ke_time_to_string(t+hspTimeOffset,tbuf,sizeof(tbuf));
	printf("Peer time : %s\n",tbuf);
	printf("Time Bias : %d milliseconds\n",hspTimeOffset/10/1000);
	printf("Counters  : ");
	for(i=0; i<sizeof peerCounters/sizeof peerCounters[0]; i++){
		printf("%d=%d,", i, peerCounters[i]);
	}
	printf("\n");
	ke_time_to_string(g_kernel.peer->lastMsgTime,tbuf,sizeof(tbuf));
	printf("Last msg from peer arrived at : %s\n",tbuf);
}

