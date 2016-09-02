/*
	ethernet based IPeer implementation
*/
#ifdef _WIN32

#include <rtknet.h>

#include <rtk_thread.h>


#define HSP_CONNECTION_TIMEOUT 100
#define HSP_LINE_SPEED (100*1000/1000) /* bits per milisecond */
#define HSP_ARBITRATION_TIMEOUT 100
#define DEFAULT_PRIMARY_LIFE 1000
#define PEER_RTU (8192-sizeof(RTK_PACKET))

__class__(IUdpPeer)
	__inherit__(IPeer)
	HVBUS vbus;
	RTK_HANDLE lock;
__class_end__(IUdpPeer)

__vtable__(IUdpPeer)
	__inherit__(IPeer_vtbl)
__vtable_end__(IUdpPeer)

static void foo(){};

LOCAL int IUdpPeer_write(IPeer * _this, const void * buf, int len)
{
	IUdpPeer * __this = __dcast__(IPeer, IUdpPeer, _this);
	if(__this->vbus){
#if defined(HSP_DEBUG)
		static int lastCmd;
		static int lastCount;
		struct peer_cmd_t * tp;
		tp = (struct peer_cmd_t*)buf;
		if(tp->cmd == lastCmd){
			lastCount++;
		}else{
			hsp_log(("send cmd:%04x, %d times\n", lastCmd, lastCount));
			lastCmd = tp->cmd;
			lastCount = 1;
		}
		/* for sniffer use */
		if(g_kernel.status.state != KERN_S_STANDBY){
			memcpy((void*)buf,"PRIMRY",6);
			if(((struct peer_cmd_t*)buf)->cmd == 0x8002)
				__asm int 3;
		}else{
			memcpy((void*)buf,"STNDBY",6);
		}
#endif
		broadcast_rtk_data(__this->vbus, PORT_ALL, 0xf8, buf, len);
	}
	return F8_SUCCESS;
}

/* vbus can only operate in asynchronous manner */
LOCAL int IUdpPeer_read(IPeer * _this, void * buf, int len, int to)
{
	return F8_UNIMPLEMENTED;
}

LOCAL int IUdpPeer_lock(IPeer * _this)
{
	IUdpPeer * __this = __dcast__(IPeer, IUdpPeer, _this);
	lock_rtk_object(__this->lock);
	return 0;
}

LOCAL void IUdpPeer_unlock(IPeer * _this, int cookie)
{
	IUdpPeer * __this = __dcast__(IPeer, IUdpPeer, _this);
	unlock_rtk_object(__this->lock);
	return;
}

LOCAL void RTK_API IUdpPeer_on_data(PCRTK_PACKET data, HVBUS bus)
{
	if(g_kernel.peer && g_kernel.peer->eventProc){
		memcpy(g_kernel.peer->peerAddress, &data->src.host, 8);
		memcpy(g_kernel.peer->peerAddress+4, &data->src.port,2);
		if(data->packet_type == 0xf8){
			g_kernel.peer->eventProc(HSE_PEER_RX, (void*)data->data,data->data_size,0,0);
		}
	}
}

/* check if local site has priority over peer site */
LOCAL __bool check_priority()
{
	__bool ret;
	ret = memcmp(g_kernel.peer->localAddress, g_kernel.peer->peerAddress, 10)<0?1:0;
	return ret;
}


#if 0
int RTK_API IUdpPeer_filter(HVBUS bus, int event, int data)
{
}
#endif

__constructor__(IUdpPeer)
{
	__touch__();
	__create_base__(IUdpPeer, IPeer, __this);
	__this->vbus = connect_vbus(0xf8, 0, IUdpPeer_on_data, 0);
	__this->lock = create_rtk_lock(0);
	return __this;
}

__destructor__(IUdpPeer)
{
	disconnect_vbus(__this->vbus);
	close_handle(__this->lock);
	__destroy_base__(IUdpPeer, IPeer, __this);
	return __this;
}

void IUdpPeer_global_create()
{
	__v_copy__(IUdpPeer, IPeer);
	__override__(IUdpPeer, IPeer, read);
	__override__(IUdpPeer, IPeer, write);
	__override__(IUdpPeer, IPeer, lock);
	__override__(IUdpPeer, IPeer, unlock);
}

__define_vtbl__(IUdpPeer)
__define_vtbl_end__(IUdpPeer)

#ifdef mama_i_wanna_go_home
void foo(){}
#endif

LOCAL void create_peer()
{
	IUdpPeer * peer;
	init_network(0, 0);
	IPeer_global_create();
	IUdpPeer_global_create();	
	__new__(IUdpPeer, peer);
	g_kernel.peer = __ucast__(IUdpPeer, IPeer, peer);
}

LOCAL void destroy_peer()
{
	IUdpPeer * peer;
	peer = __dcast__(IPeer, IUdpPeer, g_kernel.peer);
	__delete__(peer);
	uninit_network();
}

#endif

