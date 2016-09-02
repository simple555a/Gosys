#include <pmc.h>
#include <winsock2.h>
//#include <winsvc.h>

#pragma comment(linker, "/base:0x76520000")
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib, "advapi32.lib")

#define NETCOUNTER 150

static unsigned int r[255];	// cpu - run
static unsigned int s[255];	// cpu - stop
static unsigned int p[255];	// cpu - sync(&standby)
static unsigned int u[255];	// cpu - standby
static unsigned int h[255];	// host alive
static unsigned int n1[255];// 网络状态
static unsigned int n2[255];// 网络状态
static unsigned int dr[255];// netd.exe本地的pmcd.exe的运行状态
static unsigned int ds[255];// netd.exe本地的pmcd.exe的停止状态
static unsigned int dp[255];// netd.exe本地的pmcd.exe的热备状态

/*
static int QueryService(char * Name)
{
	SC_HANDLE	schSCManager;
	SC_HANDLE	schService;
	SERVICE_STATUS st;
	char reg_path[MAX_PATH];
	char path[MAX_PATH];

	utils_trace("Querying service %s : ",Name);
	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );	
	if(!schSCManager) {
		wperror("OpenSCManager Failed.");
		return 0;
	}
	schService = OpenService( schSCManager,
							  Name,
							  SERVICE_ALL_ACCESS
							  );
	if(!schService){
		utils_trace("Not installed.\n");
		CloseServiceHandle(schSCManager);
		return 0;
	}
	utils_trace("Installed,");
	QueryServiceStatus(schService,&st);
	switch(st.dwCurrentState){
#define ssss(a) case a:utils_trace(#a"\n");break;
	ssss(SERVICE_STOPPED)
	ssss(SERVICE_START_PENDING)
	ssss(SERVICE_STOP_PENDING)
	ssss(SERVICE_RUNNING)
	ssss(SERVICE_CONTINUE_PENDING)
	ssss(SERVICE_PAUSE_PENDING)
	ssss(SERVICE_PAUSED)
	default:utils_trace("Unknown state\n");break;
	}
	_snprintf(
		reg_path,
		sizeof(reg_path),
		"local_machine\\system\\currentcontrolset\\services\\%s",
		Name
		);
	get_registry_string(reg_path,"ImagePath",path,sizeof(path));
	utils_trace("Driver Path=%s\n",path);
	CloseServiceHandle(schService);
	CloseServiceHandle( schSCManager );
	return st.dwCurrentState;
}
*/

IOSS_DLL_EXPORT void update_tag(PCRTK_TAG tte, pmc_value_t * pt, PCRTK_TIME)
{
	int i;
	memcpy(&i, &tte->d.BinaryAddress[1], 4);

	switch(tte->d.BinaryAddress[0]){
	case 'a':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = r[i];
		break;
	case 'b':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = s[i];
		break;
	case 'c':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = p[i];
		break;
	case 'd':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = u[i];
		break;
	case 'e':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = h[i];
		break;
	case 'f':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = n1[i];
		break;
	case 'g':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = n2[i];
		break;
	case 'h':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = dr[i];
		break;
	case 'i':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = ds[i];
		break;
	case 'j':
		pt->Flags |= TF_Valid;
		pt->Value.iValue = dp[i];
	default:
		;
	}
}

IOSS_DLL_EXPORT __bool address_translate(PRTK_TAG tte)
{
	CArgs args;
	parse_arg_ex(tte->s.Address, & args, ".");
	memset(tte->d.BinaryAddress,0,sizeof(tte->d.BinaryAddress));
	int add;
	if(utils_argc == 2){
		if(0 == strcmpi(args.argv[0], "r")){
			tte->d.BinaryAddress[0] = 'a';
		}else if(0 == strcmpi(args.argv[0], "s")){
			tte->d.BinaryAddress[0] = 'b';
		}else if(0 == strcmpi(args.argv[0], "p")){
			tte->d.BinaryAddress[0] = 'c';
		}else if(0 == strcmpi(args.argv[0], "u")){
			tte->d.BinaryAddress[0] = 'd';
		}else if(0 == strcmpi(args.argv[0], "h")){
			tte->d.BinaryAddress[0] = 'e';
		}else if(0 == strcmpi(args.argv[0], "n1")){
			tte->d.BinaryAddress[0] = 'f';
		}else if(0 == strcmpi(args.argv[0], "n2")){
			tte->d.BinaryAddress[0] = 'g';
		}else if(0 == strcmpi(args.argv[0], "dr")){
			tte->d.BinaryAddress[0] = 'h';
		}else if(0 == strcmpi(args.argv[0], "ds")){
			tte->d.BinaryAddress[0] = 'i';
		}else if(0 == strcmpi(args.argv[0], "dp")){
			tte->d.BinaryAddress[0] = 'j';
		}else{
			return __false;
		}
		add = atoi(args.argv[1]);
		memcpy(&tte->d.BinaryAddress[1], &add, 4);
		return __true;
	}else{
		return __false;
	}
}

class CNetListern : public CRtkThread
{
public:
	CNetListern()
	{
	}
	virtual __uint run();
};


static void clear_state(int a)
{
	r[a]=s[a]=p[a]=u[a]=0;
}

#define F8_DBSYNCD_PORT 10933
static bool isListern = true;
__uint CNetListern::run()
{
	/*
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if(err != 0){
		return err;
	}

	if(LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2){
		WSACleanup();
		return 0;
	}
	*/
    //创建套接口
	SOCKET sockSrv1=socket(AF_INET,SOCK_DGRAM,0);
	SOCKET sockSrv2=socket(AF_INET,SOCK_DGRAM,0);
    
	//将套接口设置为：允许非阻塞模式
	unsigned long ul = 1;
	int nRet;
	nRet = ioctlsocket(sockSrv1, FIONBIO, &ul);
	nRet = ioctlsocket(sockSrv2, FIONBIO, &ul);

	SOCKADDR_IN addrSrv;
	//将INADDR_ANY从主机字节顺序转化为网络字节顺序
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;

	addrSrv.sin_port=htons(F8_DBSYNCD_PORT);/*TCP/IP网络字节顺序*/
	bind(sockSrv1,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	addrSrv.sin_port=htons(F8_DBSYNCD_PORT + 1);
	bind(sockSrv2,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//将本地地址addrSrv与套接口sockSrv2捆绑

	SOCKADDR_IN addrClient;
	int len=sizeof(SOCKADDR);
	char recvBuf[1500];
	int i;
	int datalen;//成功接收的字符数
	int addr;

	while(isListern){
		len = sizeof(SOCKADDR);
		/*接收远程主机经指定的socket传来的数据，并把数据传到由参数buf指向的内存空间。*/
		datalen = recvfrom(sockSrv1,recvBuf,sizeof(recvBuf),0,(SOCKADDR*)&addrClient,&len);
		if(datalen > 0){
			addr = addrClient.sin_addr.S_un.S_un_b.s_b4;
			n1[addr] = NETCOUNTER;
			clear_state(addr);//r s p u
			if(datalen == 1152){
				r[addr] = NETCOUNTER;
			}else{
				if(strncmp(recvBuf, "cpu - stop!", 11) == 0){
					s[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "cpu - sync!", 14) == 0){
					p[addr] = NETCOUNTER;					
				}else if(strncmp(recvBuf, "cpu - standby!", 14) == 0){
					u[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "host - alive!", 13) == 0){
					h[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "rtdb - run!", 11) == 0){
					dr[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "rtdb - stop!", 12) == 0){
					ds[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "rtdb - standby!", 15) == 0){
					dp[addr] = NETCOUNTER;
				}
			}
		}

		len = sizeof(SOCKADDR);
		datalen = recvfrom(sockSrv2,recvBuf,sizeof(recvBuf),0,(SOCKADDR*)&addrClient,&len);
		if(datalen > 0){
			addr = addrClient.sin_addr.S_un.S_un_b.s_b4;
			n2[addr] = NETCOUNTER;
			clear_state(addr);
			if(datalen == 1152){
				r[addr] = NETCOUNTER;
			}else{
				if(strncmp(recvBuf, "cpu - stop!", 11) == 0){
					s[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "cpu - sync!", 14) == 0){
					p[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "cpu - standby!", 14) == 0){
					u[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "host - alive!", 13) == 0){
					h[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "rtdb - run!", 11) == 0){
					dr[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "rtdb - stop!", 12) == 0){
					ds[addr] = NETCOUNTER;
				}else if(strncmp(recvBuf, "rtdb - standby!", 15) == 0){
					dp[addr] = NETCOUNTER;
				}					
			}
		}

		Sleep(10);

		for(i = 0; i < 255; i++){
			if(r[i] > 0){
				r[i]--;
			}
			if(s[i] > 0){
				s[i]--;
			}
			if(p[i] > 0){
				p[i]--;
			}
			if(u[i] > 0){
				u[i]--;
			}
			if(h[i] > 0){
				h[i]--;
			}
			if(n1[i] > 0){
				n1[i]--;
			}
			if(n2[i] > 0){
				n2[i]--;
			}
			if(dr[i] > 0){
				dr[i]--;
			}
			if(ds[i] > 0){
				ds[i]--;
			}
			if(dp[i] > 0){
				dp[i]--;
			}
		}
	}

	closesocket(sockSrv1);
	//WSACleanup(); 

	return 0;
}

IOSS_DLL_EXPORT __bool start_device(PDEVICE_INFO handle)
{
	CNetListern *pnl;
	pnl = new CNetListern;
	if(!pnl){
		return __false;
	}

	handle->OwnerField = pnl;
	if(!pnl->start()){
		delete pnl;
		return __false;
	}

	return __true;
}

IOSS_DLL_EXPORT __bool stop_device(PDEVICE_INFO handle)
{
	CNetListern *pnl;
	pnl = (CNetListern*)handle->OwnerField;

	if(!pnl){
		return __false;
	}

	isListern = false;
	if(!pnl->stop(3000)){
		utils_error(
			"Warning : f8net device on '%s' didn't stop in 3 seconds.\n",
			handle->address
			);
	}else{		
		delete pnl;
	}	

	return __true;
}
