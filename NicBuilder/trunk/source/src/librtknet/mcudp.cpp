/*
	May 20, 2003

	Possible BUG, m_saLocal may be changed on first read/write on
	unbound sockets.

	2003/9/10 fixed bug, the destoryed socket object leaves an unclosed
	window, which might cause trouble.
*/
#include "precomp.h"
#pragma hdrstop

#define Check()

#ifdef _WIN32
static LRESULT WINAPI WndProc(HWND hwnd, UINT iMessage, UINT wParam, LONG lParam);
#endif

CMulticastSocket::CMulticastSocket()
{
	m_hWnd=0;
	m_hSocket=0;
	m_bMulticast=false;
	m_bReuseAddr = false;
	m_localAddrCount = 0;
}

CMulticastSocket::~CMulticastSocket()
{
	Close();
}

int CMulticastSocket::Close()
{
	if(!IsInvalidSocket(m_hSocket)){
		if(m_bMulticast){
			LeaveGroup();
		}else{
			closesocket(m_hSocket);
		}
	}
#ifdef _WIN32
	if(m_hWnd){
		SendMessage((HWND)m_hWnd, WM_CLOSE, 0, 0);
		m_hWnd = 0;
	}
#endif
	m_hSocket = INVALID_SOCKET;
	return 0;
}
 
int CMulticastSocket::SetSockOpt(int optname,const void * optval,int optlen,int level)
{
	return setsockopt(m_hSocket,level,optname,(char*)optval,(socklen_t)optlen);
}

int CMulticastSocket::GetSockOpt(int optname,const void * optval,int *optlen,int level)
{
	int res=getsockopt(m_hSocket,level,optname,(char*)optval, (socklen_t*)optlen);
	Check();
	return res;
}

int CMulticastSocket::Bind(int port, char * addr)
{
	int ret;
	int len;
	if(m_bReuseAddr){
		int true_val=1;
		SetSockOpt(SO_REUSEADDR,(const char*)&true_val,sizeof(true_val));
	}
	
	memset(&m_saLocal,0,sizeof(m_saLocal));
	m_saLocal.sin_port=htons((unsigned short)port);
	m_saLocal.sin_family=AF_INET;
	if(addr){		
		m_saLocal.sin_addr.s_addr=inet_addr(addr);
	}else{
		m_saLocal.sin_addr.s_addr = ADDR_ANY;
	}
	ret =  bind(m_hSocket,(sockaddr*)&m_saLocal,sizeof(m_saLocal));
	len = sizeof(m_saLocal);
	getsockname(m_hSocket, (sockaddr*)&m_saLocal, &len);
	return ret;
}

int CMulticastSocket::Create(int port,int /* sock_type */)
{
	if(Socket()) 
		return SOCKET_ERROR;
	if(Bind(port,0)) 
		return SOCKET_ERROR;
	if(SetAsynchronous()) 
		return SOCKET_ERROR;
	return 0;
}

int CMulticastSocket::Socket(int)
{
	m_hSocket=socket(AF_INET, SOCK_DGRAM, 0);
	if( !IsInvalidSocket(m_hSocket) ){
		memset(&m_saLocal, 0, sizeof(SOCKADDR_IN));
		int len = sizeof(m_saLocal);
		getsockname(m_hSocket, (sockaddr*)&m_saLocal, (socklen_t*)&len);
	}
	return IsInvalidSocket(m_hSocket)? SOCKET_ERROR : 0;
}

int CMulticastSocket::SetAsynchronous()
{
#ifdef _WIN32	
	if(!m_hWnd) {
		// Create a window.
		if ((m_hWnd = CreateWindow(
		  "EDIT",
		  "",
		  WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,
		  CW_USEDEFAULT,
		  CW_USEDEFAULT,
		  CW_USEDEFAULT,
		  NULL,
		  NULL,
		  NULL,
		  NULL)) == NULL)
		{
			utils_error("CreateWindow() failed with error %d\n", GetLastError());
			Check();
			return SOCKET_ERROR;
		}
	}
	if(m_hWnd) {
		SetWindowLong((HWND)m_hWnd, GWL_WNDPROC, (LONG)WndProc);
		SetWindowLong((HWND)m_hWnd, GWL_USERDATA, (LONG)this);
		if(WSAAsyncSelect(m_hSocket, (HWND)m_hWnd, WM_SOCKET_READY,
			FD_READ|FD_ADDRESS_LIST_CHANGE|FD_ROUTING_INTERFACE_CHANGE
			)
		){
			return SOCKET_ERROR;
		}
		u_long len;
		WSAIoctl(
			Handle(),
			SIO_ADDRESS_LIST_CHANGE,
			0,0,0,0,(unsigned long*)&len,0,0
			);
	}
	return m_hWnd? 0 : SOCKET_ERROR;
#else
	return 0;
#endif
}

/*
  Returns :
  0 : if failed
  bytes read   : if OK
*/
int CMulticastSocket::ReceiveFrom(void * buf,int len,char * addr,int & port)
{
	sockaddr_in _addr;
	int res;

	*addr = 0;
	port = 0;
	res = ReceiveFrom(buf, len, &_addr);
	if(res){
		strcpy(addr,inet_ntoa(_addr.sin_addr));
	}
	port=ntohs(_addr.sin_port);
	return res;
}

/*
  Returns :
  true : if OK
  false : if failed
*/
int CMulticastSocket::JoinGroup(char *strGroupIP, int nGroupPort, int nTTL, bool bLoopback)
{
	strncpy(this->group_ip, strGroupIP, sizeof(group_ip));
	group_ip[sizeof(group_ip) - 1] = 0;
	this->group_port=nGroupPort;
	this->group_ttl=nTTL;
	this->group_loopback=bLoopback;
	m_bMulticast=true;

	/* Create socket for receiving packets from multicast group */
	Socket();
	if(SOCKET_ERROR == m_hSocket){
		wperror("WSASocket");
		return SOCKET_ERROR;		
	}
	if(m_hSocket==INVALID_SOCKET){
		return SOCKET_ERROR;
	}
	int true_val=1;
	if(SOCKET_ERROR == SetSockOpt(SO_REUSEADDR, (const char*)&true_val, sizeof(true_val))){
		wperror("ReUseAddr");
		return SOCKET_ERROR;
	}
	if(SOCKET_ERROR == Bind(nGroupPort)){
		wperror("Bind");
		return SOCKET_ERROR;
	}
	
	/* Fill m_saHostGroup_in for sending datagrams */
	memset(&m_saHostGroup, 0, sizeof(m_saHostGroup));
	m_saHostGroup.sin_family = AF_INET;
	m_saHostGroup.sin_addr.s_addr = inet_addr(strGroupIP);
	m_saHostGroup.sin_port = htons((__u16)nGroupPort);

	int addrCount = sizeof(m_localAddr) / sizeof(m_localAddr[0]);
	m_localAddrCount = 0;
	get_interface_list(m_localAddr, addrCount, true);
	m_localAddrCount = addrCount;
	/* Join the multicast group */
	int i;
	struct ip_mreq mreq;
	ZeroMemory(&mreq, sizeof(mreq));
	mreq.imr_multiaddr = m_saHostGroup.sin_addr;	
	for(i=0; i<m_localAddrCount; i++){
		if(!(m_localAddr[i].iiFlags & IFF_UP)){
			continue;
		}
		if( !(m_localAddr[i].iiFlags & IFF_MULTICAST)){
			continue;
		}		 
		mreq.imr_interface = ((sockaddr_in*)&m_localAddr[i].iiAddress)->sin_addr;
		DEBUG_PRINTF(("join %s on %s...\n", strGroupIP, inet_ntoa(mreq.imr_interface)));
		if(SOCKET_ERROR == SetSockOpt(IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq), IPPROTO_IP)){
			wperror("AddMemberShip");
		}
	}
	
	//set TTL
	if( SOCKET_ERROR == SetSockOpt(IP_MULTICAST_TTL,(const char*)&nTTL,sizeof(nTTL),IPPROTO_IP)){
		wperror("SetSockOpt-TTL");
	}
	
	true_val=bLoopback;
	if( SOCKET_ERROR == SetSockOpt(IP_MULTICAST_LOOP,(const char*)&true_val,sizeof(true_val),IPPROTO_IP)){
		wperror("SetSockOpt-Loop");
	}

#ifdef _WIN32
	true_val=bLoopback;
	DWORD bytes_returned;
	WSAIoctl(m_hSocket,SIO_MULTIPOINT_LOOPBACK,&true_val,sizeof(true_val),
			 0,0,&bytes_returned,0,0);
	Check();
	
	SetAsynchronous();
#endif
	
	return 0;
}

int CMulticastSocket::LeaveGroup()
{
	m_bMulticast=false;
	int i;
	struct ip_mreq mreq;
	ZeroMemory(&mreq, sizeof(mreq));
	mreq.imr_multiaddr = m_saHostGroup.sin_addr;	
	for(i=0; i<m_localAddrCount; i++){		
		if(!(m_localAddr[i].iiFlags & IFF_UP)){
			continue;
		}
		mreq.imr_interface = 
			((sockaddr_in*)&m_localAddr[i].iiAddress)->sin_addr;
#ifndef NDEBUG
		char addr[32];
		strcpy(addr, inet_ntoa(m_saHostGroup.sin_addr));
		DEBUG_PRINTF((
			"leaving %s on %s...\n", 
			addr, 
			inet_ntoa(mreq.imr_interface)
			));
#endif
		SetSockOpt(
			IP_DROP_MEMBERSHIP, 
			(char *)&mreq, 
			sizeof(mreq), 
			IPPROTO_IP
			);
		Check();
	}
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
	return 0;
}

/*
  Returns :
  bytes sent: if OK
  0 		: if failed
*/
int CMulticastSocket::SendTo(void *buf, int len, sockaddr_in * addr)
{
	return sendto(
		m_hSocket, 
		(const _SOCKBUF)buf, 
		len, 
		0, 
		(sockaddr*)addr, 
		sizeof(*addr)
		);
}

/*
  Returns :
  bytes sent: if OK
  0 		: if failed
*/
int CMulticastSocket::SendTo(void *buf, int len, int port, char * addr)
{
	sockaddr_in _addr;
	_addr.sin_addr.s_addr=inet_addr(addr);
	if( _addr.sin_addr.s_addr == INADDR_NONE
		&& strcmp(addr,"255.255.255.255")
		){
		// try resolve it another way
		struct hostent * h;
		h=gethostbyname(addr);
		if(!h){
			utils_error("Resolving %s failed.\n",addr);
			return 0;
		}
		_addr.sin_addr=*((in_addr*)h->h_addr_list[0]);
	}
	_addr.sin_port=htons((unsigned short)port);
	// _addr.sin_port=port;
	_addr.sin_family=AF_INET;
	return sendto(
		m_hSocket, 
		(const _SOCKBUF)buf, 
		len, 
		0, 
		(sockaddr*)&_addr, 
		sizeof(_addr)
		);
}

int CMulticastSocket::OnData(void * buf,int len,int port,char * src_addr)
{
	// DEBUG_PRINTF(("Got %d bytes data from %s:%d.\n",len,src_addr,port));
	return len;
}

int CMulticastSocket::OnReceive(int nErrorCode)
{
	if(nErrorCode) {
		return 0;
	}
	char src[128];
	int port;
	char data[MAX_PACKET_SIZE+1];
	memset(data,0,sizeof(data));
	int res=ReceiveFrom(data,sizeof(data),src,port);
	if(res){
		OnData(data,res,port,src);
	}
	return res;
}

int CMulticastSocket::GetLocalAddress(char * ip,int ip_length,int * port)
{	 
	strncpy(ip, inet_ntoa(m_saLocal.sin_addr), ip_length);
	ip[ip_length - 1] = 0;
	*port=ntohs(m_saLocal.sin_port);
	return 0;
}

int CMulticastSocket::LocalPort()
{
	sockaddr_in addr;
	int len=sizeof(addr);
	if( SOCKET_ERROR 
		== 
		getsockname(Handle(), (sockaddr*)&addr, (socklen_t*)&len) 
	){
		return -1;
	}
	return ntohs(addr.sin_port);
}

int CMulticastSocket::ReceiveFrom(void * buf, int len, sockaddr_in * addr)
{
	memset(addr, 0, sizeof(sockaddr_in));
	int from_len=sizeof(sockaddr_in);
	int res=recvfrom(
		m_hSocket, 
		(_SOCKBUF)buf, 
		len, 
		0, 
		(sockaddr*)addr, 
		(socklen_t*)&from_len
		);
	if(SOCKET_ERROR == res) {
		if(debug_level >= dbg_level_verbose){
			//utils_error("recvfrom failed,code %d\n",WSAGetLastError());
		}
		return 0;
	}
	return res;
}

void CMulticastSocket::RejoinGroup()
{
	if(!m_bMulticast){
		return;
	}
	LeaveGroup();
	JoinGroup(
		group_ip, 
		group_port, 
		group_ttl,
		group_loopback? true : false
		);
}

int CMulticastSocket::OnEvent(int errorcode,int eventid,int /* wParam */)
{
#ifdef _WIN32
	int res=1,len;
	if(errorcode){
		utils_error("Net Error on socket %d\n",m_hSocket);
		return 0;
	}
	switch(eventid){
	case FD_READ:
		OnReceive(errorcode);
		break;
	case FD_WRITE:
		assert(0);
		break;
	case FD_ADDRESS_LIST_CHANGE:			
		res=WSAIoctl(
			Handle(),
			SIO_ADDRESS_LIST_CHANGE,
			0,0,0,0,(unsigned long*)&len,0,0
			);
		if(SOCKET_ERROR == res){
			int e=GetLastError();
			if(WSAEWOULDBLOCK != e){
				utils_error("ListChange Error Code=%d\n",e);
			}
		}
		if(m_bMulticast){
			RejoinGroup();
		}
		break;
	}
	return res;
#else
	return 0;
#endif
}

#ifdef _WIN32
//=============================================================================
static LRESULT WINAPI WndProc(HWND hwnd, UINT iMessage, UINT wParam,
			LONG lParam)
{
	CMulticastSocket * udp;
	switch (iMessage) {
	case WM_SOCKET_READY:
		udp=(CMulticastSocket *)GetWindowLong(hwnd,GWL_USERDATA);
		if(udp)
			return udp->OnEvent(
				WSAGETSELECTERROR(lParam),
				WSAGETSELECTEVENT(lParam),
				wParam
				);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_CREATE:
		return 0;
		//break;		
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}
#endif

bool CMulticastSocket::isLocalAddress(sockaddr_in & addr)
{
	for(int i=0; i<m_localAddrCount; i++){
		if(((sockaddr_in*)&m_localAddr[i].iiAddress)->sin_addr.S_un.S_addr ==
			addr.sin_addr.S_un.S_addr
			)
		{
			return true;
		}
	}
	return false;
}

