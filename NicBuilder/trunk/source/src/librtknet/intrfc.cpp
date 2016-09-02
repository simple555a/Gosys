#include "precomp.h"
#pragma hdrstop

/*
  this function retrieves all the local interfaces
  that support multicast except loopback interfaces
  the results are stored in global variables
*/

#ifdef _WIN32
static void _fill_buffer(INTERFACE_INFO * localAddr, int & localAddrCount)
{
	int wsError;
	SOCKET s;
	DWORD bytesReturned;
	int numg_localAddr;
	int i;

	if((s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0)) == INVALID_SOCKET){
		wperror("Socket creation\n");
		return;
	}
	
	// Enumerate all IP interfaces	
	wsError = WSAIoctl(s, 
		SIO_GET_INTERFACE_LIST, NULL, 
		0, 
		localAddr,
		sizeof(INTERFACE_INFO)*localAddrCount, 
		&bytesReturned, 
		NULL, 
		NULL
		);
	if (wsError == SOCKET_ERROR){
		utils_error("WSAIoctl fails with error %d\n", GetLastError());
		closesocket(s);
		return;
	}
	
	closesocket(s);

	numg_localAddr = (bytesReturned/sizeof(INTERFACE_INFO));
	localAddrCount=numg_localAddr;

	/* fix broadcast address */
	for(i=0; i<numg_localAddr; i++){
		INTERFACE_INFO * p = &localAddr[i];
		__ulong *ad, *adb, *adm;
		adb = &((PSOCKADDR_IN)&p->iiBroadcastAddress)->sin_addr.S_un.S_addr;
		ad  = &((PSOCKADDR_IN)&p->iiAddress)->sin_addr.S_un.S_addr;
		adm = &((PSOCKADDR_IN)&p->iiNetmask)->sin_addr.S_un.S_addr;
		*adb = (*ad & *adm) ^ ~*adm;
	}
}
#elif defined(__LINUX__)
static void _fill_buffer(INTERFACE_INFO * localAddr, int & localAddrCount)
{
	int s;
	int i;
	ifreq ifq[10];

	s = socket(AF_INET, SOCK_DGRAM, 0);
	struct ifconf ifc;
	ifc.ifc_len = sizeof(ifq);
	ifc.ifc_req = ifq;
	ioctl(s, SIOCGIFCONF, &ifc);
	localAddrCount = ifc.ifc_len / sizeof(struct ifreq);
	for(i=0; i<localAddrCount; i++){
		ioctl(s, SIOCGIFFLAGS, &ifq[i]);
		localAddr[i].iiFlags = ifq[i].ifr_ifru.ifru_flags;
		ioctl(s, SIOCGIFADDR, &ifq[i]);
		localAddr[i].iiAddress = ifq[i].ifr_ifru.ifru_addr;
		ioctl(s, SIOCGIFBRDADDR, &ifq[i]);
		localAddr[i].iiBroadcastAddress = ifq[i].ifr_ifru.ifru_broadaddr;
		ioctl(s, SIOCGIFNETMASK, &ifq[i]);
		localAddr[i].iiNetmask = ifq[i].ifr_ifru.ifru_netmask;
	}
	close(s);
}
#endif

static void dump_interface_list(INTERFACE_INFO * localAddr, int localAddrCount)
{
	int			i;
	char		*pAddrString;
	SOCKADDR_IN	*pAddrInet;
	u_long		SetFlags;

	// Display interface information
	for (i=0; i<localAddrCount; i++){
		pAddrInet = (SOCKADDR_IN*)&localAddr[i].iiAddress;
		pAddrString = inet_ntoa(pAddrInet->sin_addr);
		if (pAddrString)
			utils_trace("ip:%s", pAddrString);
		
		pAddrInet = (SOCKADDR_IN*)&localAddr[i].iiNetmask;
		pAddrString = inet_ntoa(pAddrInet->sin_addr);
		if (pAddrString)
			utils_trace(",mask:%s", pAddrString);
		pAddrInet = (SOCKADDR_IN*)&localAddr[i].iiBroadcastAddress;
		SetFlags = localAddr[i].iiFlags;
		if (SetFlags & IFF_BROADCAST){
			pAddrString = inet_ntoa(pAddrInet->sin_addr);
			utils_trace(",bcast(%s)", pAddrString);
		}			
		if (SetFlags & IFF_MULTICAST)
			utils_trace(",mcast");
		
#ifdef _WIN32
		if (SetFlags & IFF_POINTTOPOINT)
			utils_trace(",p2p");
#endif

		if ( SetFlags & IFF_LOOPBACK ){
			utils_trace(",loop");
		}

		if( localAddr[i].iiFlags & IFF_DISABLED ){
			utils_trace(",disabled");
		}else if( !(localAddr[i].iiFlags & IFF_UP) ){
			utils_trace(",down");
		}else{
			utils_trace(",up");
		}

		utils_trace("\n");
	}
}

RTKNET_API void PMC_API get_interface_list(
	INTERFACE_INFO * localAddr, 
	int & localAddrCount,
	bool bSilent
	)
{
	int		i;
	u_long	SetFlags;
	char	disabled[256];
	char	*strAddr;
	SOCKADDR_IN	*pAddrInet;

	if(!bSilent){
		utils_trace("Scanning Interfaces . . .\n");
	}

	_fill_buffer(localAddr, localAddrCount);

	GetPrivateProfileString(
		"vbus",
		"disabled",
		"",
		disabled,
		sizeof(disabled),
		get_config_file()
		);

	// Display interface information
	for (i=0; i<localAddrCount; i++){
		SetFlags = localAddr[i].iiFlags;
#ifdef rtkm_networked_version
		// loopback interface must be disabled
		if ( SetFlags & IFF_LOOPBACK ){
			localAddr[i].iiFlags &= ~IFF_UP;
		}
#elif defined(rtkm_looponly_version)
		// non-loopback interface must be disabled
		if (!(SetFlags & IFF_LOOPBACK)){
			localAddr[i].iiFlags &= ~IFF_UP;
		}
#endif
		pAddrInet = (SOCKADDR_IN*)&localAddr[i].iiAddress;
		strAddr = inet_ntoa(pAddrInet->sin_addr);
		if(strAddr){
			if(strstr(disabled, strAddr)){
				localAddr[i].iiFlags |= IFF_DISABLED;
				localAddr[i].iiFlags &= ~IFF_UP;
			}else{
				localAddr[i].iiFlags &= ~IFF_DISABLED;
			}
		}		
	}

	if(!bSilent){
		dump_interface_list(localAddr, localAddrCount);
	}
}
