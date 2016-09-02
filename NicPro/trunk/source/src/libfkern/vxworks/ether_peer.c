/*
	ethernet based IPeer implementation
*/
#ifdef __VXWORKS__
#include <vxworks.h>
#include <taskLib.h>
#include <timers.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <endLib.h>
#include <etherLib.h>
#include <cacheLib.h>

#define USE_81X9_END_DRIVER
// #define USE_ETHER_HOOK

#if 0
/*
optional value:0/1/2
ether port 0 is used as the primary port to communicate with host machine;
port 1 is the secondary host link port.
port 2 is used to communicate with the peer
however port 2 as designed by chenjun is broken for now, so we have to use
port 1 instead.
*/
#define PEER_PORT 1
#else
#define PEER_PORT 2
#endif

#ifdef USE_81X9_END_DRIVER
extern STATUS rtl81x9Hook(END_OBJ*,STATUS (*hook)(END_OBJ*,M_BLK_ID));
extern STATUS rtl81x9Unhook(END_OBJ*);
extern int rtl81x9ReceiveDirect(END_OBJ *pDrvCtrl, char * buf, int len, int milliseconds);
extern STATUS rtl81x9SendDirect(END_OBJ*,void *data,int len);
extern int rtl81x9Lock(END_OBJ*);
extern void rtl81x9Unlock(END_OBJ*, int cookie);

extern END_OBJ * rtl81x9Objects[4];

#define peerEtherPort rtl81x9Objects[PEER_PORT]
#else
#include "rtl8139.c"
#endif

extern int rtl81x9Resources[4][4];

#define HSP_CONNECTION_TIMEOUT 1
#define HSP_LINE_SPEED (10*1000) /* bits per millisecond */
#define HSP_ARBITRATION_TIMEOUT 10
#define PEER_RTU 1500
#define HSP_PROTOCOL 0xf8cf
#define DEFAULT_PRIMARY_LIFE 500

__class__(IEtherPeer)
	__inherit__(IPeer)
__class_end__(IEtherPeer)

__vtable__(IEtherPeer)
	__inherit__(IPeer_vtbl)
__vtable_end__(IEtherPeer)

#if defined(HSP_DEBUG) && 0
LOCAL void _dumpWrite(const char * buf)
{
	struct peer_cmd_t * tp;
#if 0
	static int lastCmd;
	static int lastCount;
	tp = (struct peer_cmd_t*)buf;
	if(tp->cmd == lastCmd){
		lastCount++;
	}else{
		hsp_log(("send cmd:%04x, %d times,code=%d,p0=%x,p1=%x\n", lastCmd, lastCount,tp->code,tp->params[0],tp->params[1]));
		lastCmd = tp->cmd;
		lastCount = 1;
	}
	/* for sniffer use */
	if(g_kernel.status.state != KERN_S_STANDBY){
		memcpy((void*)buf,"PRIMRY",6);
	}else{
		memcpy((void*)buf,"STNDBY",6);
	}
#else
	tp = (struct peer_cmd_t*)buf;
	hsp_log(("send cmd:%04x,code %d,p0=%x,p1=%x\n", tp->cmd,tp->code,tp->params[0],tp->params[1]));
#endif
}
#else
#define _dumpWrite(a)
#endif

LOCAL int IEtherPeer_read(IPeer * _this, void * buf, int len, int timeout)
{
	char _buf[1514];
	ENET_HDR * c;
	int ret;
	c = (ENET_HDR*)_buf;
#ifdef USE_81X9_END_DRIVER
	ret = rtl81x9ReceiveDirect(peerEtherPort, buf, len, timeout);
#else
	ret = rtl81x9_read(peerEtherPort, _buf, sizeof(_buf), timeout);
#endif
	if(ret < sizeof(*c)){
		return F8_BUSY;
	}
	if(c ->type != HSP_PROTOCOL && c->type != ntohs(HSP_PROTOCOL)){
		return F8_INVALID_DATA;
	}
	ret -= sizeof(*c);
	if(ret > len){
		return F8_BUFFER_TOO_SMALL;
	}
	memcpy(buf, &c[1], ret);
	return ret;
}

/*
	send packet over redundancy port, return 0 if ok, <0 if failed.
*/
LOCAL int IEtherPeer_write(IPeer * _this, const void * buf, int len)
{
	char _buf[1514];
	ENET_HDR * c;
	int ret;
	c = (ENET_HDR*)_buf;
	
	if(len > PEER_RTU){
		return F8_INVALID_DATA;
	}
	/* host-standby port is a dedicated port, so broadcast is equivalent to unicast */
	memset(c->dst, 0xff, 6);
	memcpy(c->src, ACTECH_ETHER_ID, 6);
	c->src[5]=ke_get_flag(FKERN_KEY_A_B)+0x8a;
	c->type = ntohs(HSP_PROTOCOL);
	memcpy(&c[1], buf, len);
#ifdef USE_81X9_END_DRIVER
	ret = rtl81x9SendDirect(peerEtherPort, _buf, len+sizeof(*c));
#else
	ret = rtl81x9_write(peerEtherPort, _buf, len+sizeof(*c));
#endif
	_dumpWrite(buf);
	if(ret < 0){
		return F8_BUSY;
	}
	return F8_SUCCESS;
}

LOCAL int IEtherPeer_lock(IPeer * _this)
{
	// return rtl81x9Lock(peerEtherPort);
	// return rtl81x9_disable_intr(peerEtherPort);
	intDisable(rtl81x9Resources[PEER_PORT][1]);
	return 0;
}

LOCAL void IEtherPeer_unlock(IPeer * _this, int cookie)
{
	// rtl81x9Unlock(peerEtherPort, cookie);
	// rtl81x9_enable_intr(peerEtherPort,cookie);
	intEnable(rtl81x9Resources[PEER_PORT][1]);
}

/* check if local site has priority over peer site */
LOCAL __bool check_priority()
{
#if 0
	if(!g_kernel.peer_status.flags){
		/* peer is not present */
		return __true;
	}
#endif
	/* station A has priority over station B */
	return g_kernel.peer->localAddress[5]<g_kernel.peer->peerAddress[5]?1:0;
}

void IEtherPeer_global_create(void)
{
	__v_copy__(IEtherPeer, IPeer);
	__override__(IEtherPeer, IPeer, read);
	__override__(IEtherPeer, IPeer, write);
	__override__(IEtherPeer, IPeer, lock);
	__override__(IEtherPeer, IPeer, unlock);
}

IEtherPeer * IEtherPeer_create(IEtherPeer * __this)
{
	__touch__();
	__create_base__(IEtherPeer, IPeer, __this);
	return __this;
}

void * IEtherPeer_destroy(IEtherPeer * __this)
{
	__destroy_base__(IEtherPeer, IPeer, __this);
	return __this;
}

__define_vtbl__(IEtherPeer)
__define_vtbl_end__(IEtherPeer)

#ifdef __never_defined
static void foo(){};
#endif

LOCAL void IEtherPeer_on_packet(void * buf, int len)
{
	ENET_HDR * e;
	e = (ENET_HDR*)buf;
	if(len < sizeof(*e)){
		return;
	}
#ifndef USE_81X9_END_DRIVER
	if(e->type != HSP_PROTOCOL && e->type != htons(HSP_PROTOCOL)){
		return;
	}
#endif
	if(g_kernel.peer && g_kernel.peer->eventProc){
		memcpy(g_kernel.peer->peerAddress, e->src, 6);
		g_kernel.peer->eventProc(HSE_PEER_RX,&e[1], len-sizeof(*e),0,0);
	}
}

#ifdef USE_81X9_END_DRIVER

#ifndef USE_ETHER_HOOK
LOCAL STATUS etherHook(END_OBJ * o, M_BLK_ID p)
{
	struct ether_header * h;
	h=(struct ether_header * )p->pClBlk->clNode.pClBuf;
	if(p->pClBlk->clSize > sizeof(*h) && h->ether_type==htons(HSP_PROTOCOL)){
		IEtherPeer_on_packet(h, p->pClBlk->clSize);
		netMblkClChainFree(p);
		return OK;
	}
	return ERROR;
}
#else
LOCAL BOOL etherHook
          (
          struct ifnet *pIf,    /* interface packet was received on */
          char         *buffer, /* received packet */
          int          length   /* length of received packet */
          )
{
	struct ether_header * h=(struct ether_header *)buffer;
#if 0
{
			printf(
				"%02x:%02x:%02x:%02x:%02x:%02x->%02x:%02x:%02x:%02x:%02x:%02x, type %04x, len=%d.\n", 
				h->ether_shost[0],
				h->ether_shost[1],
				h->ether_shost[2],
				h->ether_shost[3],
				h->ether_shost[4],
				h->ether_shost[5],
				h->ether_dhost[0],
				h->ether_dhost[1],
				h->ether_dhost[2],
				h->ether_dhost[3],
				h->ether_dhost[4],
				h->ether_dhost[5],
				h->ether_type,
				length
				);
}
#endif
	if(h->ether_type == HSP_PROTOCOL || h->ether_type==ntohs(HSP_PROTOCOL)){
		IEtherPeer_on_packet(h, length);
		return TRUE;
	}
	return FALSE;
}
#endif
#endif

void create_peer()
{
	IEtherPeer * peer;
	IPeer_global_create();
	IEtherPeer_global_create();
	__new__(IEtherPeer, peer);
	g_kernel.peer = __ucast__(IEtherPeer, IPeer, peer);
	memcpy(g_kernel.peer->localAddress,ACTECH_ETHER_ID,5);
	g_kernel.peer->localAddress[5]=sysGetA_BSetting()+0x8a;
#ifdef USE_81X9_END_DRIVER
#ifdef USE_ETHER_HOOK
	etherInputHookAdd(etherHook,"rtl",PEER_PORT);
#else
	rtl81x9Hook(peerEtherPort, etherHook);
#endif
#else
	peerEtherPort->devMemory = (char*)rtl81x9Resources[PEER_PORT][0];
	peerEtherPort->ilevel = rtl81x9Resources[PEER_PORT][1];
	peerEtherPort->ivec = rtl81x9Resources[PEER_PORT][2];
	rtl81x9_init(peerEtherPort);
	rtl81x9_start(peerEtherPort);
	peerEtherPort->on_packet = IEtherPeer_on_packet;
#endif
}

void destroy_peer()
{
	IEtherPeer * peer;
#ifdef USE_81X9_END_DRIVER
	rtl81x9Unhook(peerEtherPort);
#else
	peerEtherPort->on_packet = 0;
	rtl81x9_stop(peerEtherPort);
	rtl81x9_uninit(peerEtherPort);
#endif
	peer = __dcast__(IPeer, IEtherPeer, g_kernel.peer);
	__delete__(peer);
}

#endif

