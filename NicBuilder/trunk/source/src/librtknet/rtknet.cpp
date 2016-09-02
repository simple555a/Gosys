/*
	PC based Monitoring System source file
	version 1.0
	by J.Hunter,jhunter@263.net,May 2001

	Network layer for Real-Time kernel
	Created : Feb. 2001
*/  
//= UPDATE RECORD
//= Version    Update for    Update by    Date           Update                           
//= 1.0.0.2    (1)           Zander Sun   2013-10-10     基于init_network()和uninit_network()简化后,
//=                                                      增加init_timesvr_network()和uninit_timesvr_network(),
//=                                                      校时服务只需要一个vbus多播组


#include "precomp.h"
#pragma hdrstop

#include <zlib.h>

#ifdef _WIN32
#pragma comment(linker, "/base:0x72400000")
#endif

RTKNET_API HVBUS vbus_alarm;
RTKNET_API HVBUS vbus_rtdb;
RTKNET_API HVBUS vbus_system;
RTKNET_API HVBUS vbus_operation;
RTKNET_API HVBUS vbus_config;

CLock g_InitLock;
static int g_iInitCount;

RTKNET_API PNODE_TABLE_ENTRY g_ThisNode;

typedef map<VBUS_ID, CRtkVBus*> VBUS_LIST;
static VBUS_LIST g_buses;

/*
 * 函数说明：1、对g_InitLock、g_iInitCount操作，即读写锁操作
             2、从pmc.ini中读取得到g_TransactionLife、CPacketMan::m_mtu,并计算得到CPacketMan::m_dwHeaderSize、CPacketMan::m_dwEffectiveLoad
			 3、初始化使用WS2_32.DLL
			 4、查找本机网卡信息，并屏蔽掉pmc.ini中指定的网卡
			 5、取本机名赋给g_ThisNode或指定的local_addr赋给g_ThisNode
			 6、创建5个CRtkVBus,形成5个键值对，插入g_buses，并相应的开启5个线程
 */
RTKNET_API __bool PMC_API init_network(
	__int init_flag, 
	const char * local_addr
	)
{
	INTERFACE_INFO ii[10];
	int	iicount;
	char transactionLife[64];
	char defaultTransactionLife[32];

	__bool retval;	  
	
	g_InitLock.Lock();
	if(g_iInitCount){
		g_iInitCount++;
		g_InitLock.UnLock();
		return __true;
	}
	
	g_iInitCount++;
	
	utils_trace("Starting VBUS over TCP/IP, Version 1.0...\n");

	sprintf(defaultTransactionLife, "%d", rtkm_default_transaction_life);
	GetPrivateProfileString(
		"vbus",
		"TransactionLife",
		defaultTransactionLife,
		transactionLife,
		sizeof(transactionLife),
		get_config_file()
		);
	g_TransactionLife = atof(transactionLife);
	if(g_TransactionLife < 0){
		utils_error(
			"Warning : TransactionLife=%.3f invalid.\n",
			g_TransactionLife
			);
		g_TransactionLife = rtkm_default_transaction_life;
	}
	if(g_TransactionLife < 10){
		utils_error(
			"Warning : TransactionLife=%.3f too short, may cause trouble.\n",
			g_TransactionLife
			);
	}else{
		utils_trace(
			"TransactionLife set to %.3f seconds.\n",
			g_TransactionLife
			);
	}

	CPacketMan::m_mtu = GetPrivateProfileInt(
		"VBUS",
		"MTU",
		MAX_PACKET_SIZE,
		get_config_file()
		);
	if(CPacketMan::m_mtu > MAX_PACKET_SIZE*8){
		utils_error("VBUS::MTU = %d invalid.\n", CPacketMan::m_mtu);
	}
	utils_trace("VBUS::MTU is set to %d.\n", CPacketMan::m_mtu);
	CPacketMan::m_dwHeaderSize = sizeof(RTK_PACKET) - 1;
	CPacketMan::m_dwEffectiveLoad = CPacketMan::m_mtu - CPacketMan::m_dwHeaderSize;

#ifdef _WIN32
	WSAData wsa;
	if( WSAStartup(0x0202,&wsa) ){
		wperror("WSAStartup failed");
	}
#endif	
	
	iicount = 10;
	get_interface_list(ii, iicount, false);   //libvbus.dll

	retval=__true;
    
	//取本机名赋给g_ThisNode, set_personality()只能返回true.....
	if( !set_personality(local_addr) ) {
		retval=__false;
	}
	
	if(retval && !(init_flag & RTK_SKIP_STANDARD_BUSES)){
		__uint busFlags = 0;
		if(init_flag & RTK_INIT_AS_SERVER){
			busFlags = VBUS_CONNECT_AS_SERVER;
		}
		//开启5个线程
		vbus_alarm	   = connect_vbus(BUSID_ALARM, busFlags, 0, 0);
		vbus_rtdb	   = connect_vbus(BUSID_RTDB, busFlags, 0, 0);
		vbus_operation = connect_vbus(BUSID_OPERATION, busFlags, 0, 0);
		vbus_system    = connect_vbus(BUSID_SYSTEM, busFlags, 0, 0);
		vbus_config	   = connect_vbus(BUSID_CONFIG, busFlags, 0, 0);	
	}	

	utils_trace("VBUS over TCP/IP started.\n");

	return retval;
}

RTKNET_API void PMC_API uninit_network(void)
{
	g_InitLock.Lock();
	
	g_iInitCount--;
	if(g_iInitCount){
		g_InitLock.UnLock();
		return;
	}
	
	HVBUS bus;
	VBUS_LIST::iterator it;
	it = g_buses.begin();
	while( it != g_buses.end() ){
		bus = (HVBUS)it->second;
		it++;
		disconnect_vbus(bus);		
	}
	
#ifdef _WIN32
	WSACleanup();
#endif	  
}

/*
 *函数功能：启动VBUS。
            若VBUS链表g_buses找到了对应ID的CRtkVBus，则执行connect操作；
			若VBUS链表g_buses没有找到对应ID的CRtkVBus,则新建一个CRtkVBus，执行start操作（即开启线程），并将<ID, CRtkVBus>insert到g_buses中。
 *输入参数：VBUS的ID号--busid
 *返回值：找到的或新建的CRtkVBus*
*/
RTKNET_API HVBUS PMC_API connect_vbus(
	__uint busid,               // classic
	__uint flags,               // server or client
	VBUS_RECEIVE_PROC r_proc,   // function pointer for receive packet
	VBUS_EVENT_PROC f_proc      // function pointer for event(or send packet)????
)
{
	VBUS_LIST::iterator it;
	CRtkVBus * st = NULL;
    
	//在VBUS_LIST中寻找busid对应的CRtkVBus*
	it = g_buses.find(busid);
	//找到了，就赋给st
	if( it != g_buses.end() ){
		st = (CRtkVBus*)it->second;
	}
	
	//找到了，然后执行st的成员函数connect,返回！
	if(st){
		st->connect(flags, r_proc, f_proc);
		return st;
	}
    
	//没找到，则新建一个CRtkVBus，并对其成员busid, flags, r_proc, f_proc, m_iRtkPort赋值
	st = new CRtkVBus(busid, flags, r_proc, f_proc);
	if(!st){
		return __false;
	}
	
	//
	if(!st->start()){
		delete st;
		utils_error("Bus %d start failed.\n", busid);
		return __false;
	}
	
	if(WAIT_OBJECT_0 != st->wait(CRtkThread::EVT_INIT_COMPLETE, 2000000000)){
		utils_error("Warning : VBus %d initialization timeout.\n", busid);
	}
    //
	g_buses.insert(VBUS_LIST::value_type(busid, st));

	return (HVBUS)st;
}

RTKNET_API void PMC_API disconnect_vbus(HVBUS channel)
{
	VBUS_LIST::iterator it;
	CRtkVBus * st;
	
	VBUS_DATA d;
	d.size = sizeof(d);
	get_vbus_data(channel, &d);
	it = g_buses.find(d.id);
	if( it != g_buses.end() ){
		g_buses.erase(it);
	}

	st = reinterpret_cast<CRtkVBus *>(channel);
	if( st->stop(5000) ){
		delete st;
	}
#define _check_(bus) if(st == vbus_##bus) vbus_##bus = 0;
	_check_(alarm);
	_check_(rtdb);
	_check_(system);
	_check_(config);
	_check_(operation);
}

/*
 *函数功能：对数据包作压缩的判断和操作，再调用CRtkVBus::SendPacket()发送数据包
 */
RTKNET_API __uint PMC_API send_rtk_packet(
	HVBUS channel,
	RTK_PACKET * p
)
{
	CRtkVBus * vbus;
	__uint ret;
	__u8 * buf=0;
	
	vbus = (CRtkVBus*)channel;
	if(!vbus){
		return 0;
	}
	//包的数据量过大，则进行数据包压缩
	if(p->data_size > 1024){
		__ulong size=p->data_size*3/2;
		buf=new __u8[size];
		PRTK_PACKET np=(PRTK_PACKET)buf;
		*np=*p;		
		size-=sizeof(__u32);
		if(compress((__u8*)np->data+sizeof(__u32), &size, (__u8*)p->data, p->data_size) == Z_OK){
			/* record uncompressed size, thus receiving client might allocate buffer 
			accordingly */
			*((__u32*)np->data) = p->data_size;
			p=np;
			p->packet_type |= PF_Compressed;
			p->data_size = size + sizeof(__u32);
		}
	}	

	ret =  vbus->SendPacket(p);
	
	if(buf){
		delete buf;
	}
	
	return ret;	
}

RTKNET_API __uint PMC_API send_rtk_data(
	HVBUS channel,
	const RTK_ADDR * addr,
	__u32 packet_type,
	const void * data,
	__uint data_len
)
{
	RTK_GUID guid;
	guid.create();
	return send_rtk_data_with_guid(channel,
		addr, &guid, packet_type, data, data_len);
}

RTKNET_API __uint PMC_API send_rtk_data_with_guid(
	HVBUS channel,
	const RTK_ADDR * addr,
	const RTK_GUID * guid,
	__uint packet_type,
	const void * data,
	__uint data_len
)
{
	return send_rtk_data_with_guid_ex(
		channel,
		addr,
		guid,
		packet_type,
		0,
		data,
		data_len
		);
}

/*
	all send_xxx functions will reach here
*/
/*
 *给要发送的数据包包头的成员赋值，然后调用send_rtk_packet()
*/
RTKNET_API __uint PMC_API send_rtk_data_with_guid_ex(
	HVBUS channel,
	const RTK_ADDR * addr,
	const RTK_GUID * guid,
	__uint packet_type,
	__u16 grp_id,
	const void * data,
	__uint data_len
)
{
	RTK_PACKET *p;

	__uint retval;
	char stack_buf[512];

	if(!channel){
		return 0;
	}

	if(data_len < sizeof(stack_buf)-sizeof(RTK_PACKET)){
		p = (RTK_PACKET*)stack_buf;
	}else{
		p = (RTK_PACKET*)(new char[sizeof(RTK_PACKET) + data_len]);
		if(!p){
			return 0;
		}
	}
	ZeroMemory(p, sizeof(RTK_PACKET));
	p->version = RTK_VERSION;	
	p->dest = *addr;	
	p->guid = *guid;
	// p->packet_size = data_len + sizeof(RTK_PACKET) - sizeof(p->data);	
	p->data_size = data_len;
	p->packet_type = packet_type;
	p->grp_id = grp_id;
	if(p->data){
		memcpy(p->data, data, data_len);
	}
	
	retval = send_rtk_packet(channel, p);

	if((char*)p != stack_buf){
		delete p;
	}

	return retval;
}

RTKNET_API __uint PMC_API broadcast_rtk_data(
	HVBUS channel,
	__u16 port,
	__u32 packet_type,
	const void * data,
	__uint data_len
	)
{
	RTK_GUID guid;
	guid.create();
	return broadcast_rtk_data_with_guid(
		channel,
		port,
		&guid,
		packet_type,
		data,
		data_len);
}

RTKNET_API __uint PMC_API broadcast_rtk_data_with_guid(
	HVBUS channel,
	__u16 port,
	const RTK_GUID * guid,
	__u32 packet_type,
	const void * data,
	__uint data_len
	)
{
	RTK_ADDR dest;
	mk_bcast_addr(&dest.host);     //dest.host都赋给'*'
	dest.port = port;
	return send_rtk_data_with_guid(
		channel, 
		&dest, 
		guid,
		packet_type, 
		data, 
		data_len
		);
}

/*
*取本机名赋给g_ThisNode
*取本机名有两个途径：（1）local_addr指定，（2）调用Windows的API--gethoasname()
*/
RTKNET_API int PMC_API set_personality(const char * local_addr)
{
	static NODE_TABLE_ENTRY local;
	if(local_addr){
		local.key = CHostName(local_addr);
	}else{
		char computername[64];
		__uint len = sizeof(computername);
		gethostname(computername, len);
		local.key = CHostName(computername);
		make_unified_key(local.key);
	}
	utils_trace("Personalized as %s\n", (char*)CHostName(local.key));
	g_ThisNode = &local;
	return __true;
}

class CRtkDebugger {
public:
	void get_data(CRtkVBus * bus, PVBUS_DATA d)
	{
		d->id = bus->busid;
		
#define fill2(field, innerField)\
	if(d->size >= offsetof(VBUS_DATA, field) + sizeof(d->field)){\
		d->field = bus->innerField;\
	}
	
#define fill(field) fill2(field, field)

		fill(duplicated_packets);
		fill(in_bytes);
		fill(out_bytes);
		fill(in_packets);
		fill(out_packets);
		fill(total_delay);
		fill2(localAddr, m_saLocal);
		fill2(peerAddr, peerAddress);
		fill(serverGuid);
	}
	
	__u16 get_port(CRtkVBus * bus)
	{
		return bus->m_iRtkPort;
	}

	__u16 set_port(CRtkVBus * bus, __u16 newPort)
	{
		__u16 port = bus->m_iRtkPort;
		return bus->m_iRtkPort = newPort;
	}
};

RTKNET_API __bool PMC_API get_vbus_data(
	HVBUS handle, 
	PVBUS_DATA data
	)
{
	CRtkDebugger dbg;
	dbg.get_data((CRtkVBus*)handle, data);
	return __true;
}

RTKNET_API __u16 PMC_API get_vbus_port(
	HVBUS handle
	)
{
	CRtkDebugger dbg;
	return dbg.get_port((CRtkVBus*)handle);
}

RTKNET_API __u16 PMC_API set_vbus_port(
	HVBUS handle,
	__u16 newPort
	)
{
	CRtkDebugger dbg;
	return dbg.set_port((CRtkVBus*)handle, newPort);
}

RTKNET_API int PMC_API default_server_filter(
	HVBUS bus,
	int event,
	int _p
	)
{
	PCRTK_PACKET p = (PCRTK_PACKET)_p;

	switch(event){
	case VBUS_EVT_GOT_FRAGMENT:
		if(p->dest.port != PORT_ALL && p->dest.port != PORT_ALL_SERVER){
			return __false;
		}
		
		if(!is_same_key(&p->dest.host, &g_ThisNode->key) &&
			strncmp(p->dest.host.Data, RTK_BROADCAST_ADDR, sizeof(RTK_HOST))
			){
			return __false;
		}

		if(PACKET_TYPE(p) == PT_Debug){
			extern void net_debug();
			net_debug();
		}

		return __true;
	break;
	case VBUS_EVT_COLLISION:
		return 0;
	}

	return 0;
}

RTKNET_API int PMC_API default_client_filter(
	HVBUS bus,
	int event,
	int _p
	)
{
	__uint port;
	PCRTK_PACKET p = (PCRTK_PACKET)_p;

	switch(event){
	case VBUS_EVT_GOT_FRAGMENT:
		port = get_vbus_port(bus);
		if(p->dest.port != PORT_ALL){
			if(p->dest.port != PORT_ALL_CLIENT && p->dest.port != port){
				return __false;
			}
		}
		
		if(!is_same_key(&p->dest.host, &g_ThisNode->key) &&
			strncmp(p->dest.host.Data, RTK_BROADCAST_ADDR, sizeof(RTK_HOST))
			){
			return __false;
		}

		if(PACKET_TYPE(p) == PT_Debug){
			extern void net_debug();
			net_debug();
		}

		return __true;
		break;
	}

	return 0;
}


// Zander 2013-1-6
RTKNET_API __bool PMC_API init_timesvr_network(
	HVBUS vbus,
	__uint bus_id,
	__int networlInitFlags, 
	const char * local_addr
	)
{
	INTERFACE_INFO ii[10];
	int	iicount;
	char transactionLife[64];
	char defaultTransactionLife[32];

	__bool retval;	  

	g_InitLock.Lock();
	if(g_iInitCount){
		g_iInitCount++;
		g_InitLock.UnLock();
		return __true;
	}

	g_iInitCount++;

	utils_trace("Starting VBUS over TCP/IP, Version 1.0...\n");

	sprintf(defaultTransactionLife, "%d", rtkm_default_transaction_life);
	GetPrivateProfileString(
		"vbus",
		"TransactionLife",
		defaultTransactionLife,
		transactionLife,
		sizeof(transactionLife),
		get_config_file()
		);
	g_TransactionLife = atof(transactionLife);
	if(g_TransactionLife < 0){
		utils_error(
			"Warning : TransactionLife=%.3f invalid.\n",
			g_TransactionLife
			);
		g_TransactionLife = rtkm_default_transaction_life;
	}
	if(g_TransactionLife < 10){
		utils_error(
			"Warning : TransactionLife=%.3f too short, may cause trouble.\n",
			g_TransactionLife
			);
	}else{
		utils_trace(
			"TransactionLife set to %.3f seconds.\n",
			g_TransactionLife
			);
	}

	CPacketMan::m_mtu = GetPrivateProfileInt(
		"VBUS",
		"MTU",
		MAX_PACKET_SIZE,
		get_config_file()
		);
	if(CPacketMan::m_mtu > MAX_PACKET_SIZE*8){
		utils_error("VBUS::MTU = %d invalid.\n", CPacketMan::m_mtu);
	}
	utils_trace("VBUS::MTU is set to %d.\n", CPacketMan::m_mtu);
	CPacketMan::m_dwHeaderSize = sizeof(RTK_PACKET) - 1;
	CPacketMan::m_dwEffectiveLoad = CPacketMan::m_mtu - CPacketMan::m_dwHeaderSize;

#ifdef _WIN32
	WSAData wsa;
	if( WSAStartup(0x0202,&wsa) ){
		wperror("WSAStartup failed");
	}
#endif	

	iicount = 10;
	get_interface_list(ii, iicount, false);   //libvbus.dll

	retval=__true;

	// 
	//memcpy(g_ThisNode->key.Data, "timesvr", sizeof("timesvr"));
	
	//取本机名赋给g_ThisNode, set_personality()只能返回true.....
	if( !set_personality(local_addr) ) {
		retval=__false;
	}
    
	

	if(retval && !(networlInitFlags & RTK_SKIP_STANDARD_BUSES)){
		__uint busFlags = 0;
		if(networlInitFlags & RTK_INIT_AS_SERVER){
			busFlags = VBUS_CONNECT_AS_SERVER;
		}
		//开启5个线程
		//vbus_alarm	   = connect_vbus(BUSID_ALARM, busFlags, 0, 0);
		//vbus_rtdb	   = connect_vbus(BUSID_RTDB, busFlags, 0, 0);
		//vbus_operation = connect_vbus(BUSID_OPERATION, busFlags, 0, 0);
		//vbus_system    = connect_vbus(BUSID_SYSTEM, busFlags, 0, 0);
		//vbus_config	   = connect_vbus(BUSID_CONFIG, busFlags, 0, 0);	
		vbus = connect_vbus(bus_id, busFlags, 0, 0);
	}	

	utils_trace("VBUS over TCP/IP started.\n");

	return retval;

}

RTKNET_API void PMC_API uninit_timesvr_network(void)
{
	g_InitLock.Lock();

	g_iInitCount--;
	if(g_iInitCount){
		g_InitLock.UnLock();
		return;
	}

	HVBUS bus;
	VBUS_LIST::iterator it;
	it = g_buses.begin();
	while( it != g_buses.end() ){
		bus = (HVBUS)it->second;
		it++;
		disconnect_vbus(bus);		
	}

#ifdef _WIN32
	WSACleanup();
#endif	  
}
