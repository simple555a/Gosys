/*
改动记录
Date       Version   Modified By   Modify
2012-7-13  1.0.0.4   Zander Sun    1.增加时间校时功能，即发送上位机时间给实时库--workThread()
2012-7-17  1.0.0.6   Zander Sun    1.把net1与net2的读取过程从workThread()中移至init_data()，避免反复去读取配置文件
                                   2.增加从配置文件中读取系统时间标签名和是否发送系统时间标识--init_data()
								   3.调用系统时间发送函数,增加b_st的判断和是否可配置系统时间标签名的分支—workThread()
2012-7-17  1.0.0.7   Zander Sun    1.修改系统时间标签名的读取函数的形参，配置文件路径通过调用get_config_file()得到，
                                     而不是直接字符串输入。-s命令下的服务不识别直接字符串输入，-d识别。--init_data()
								   
*/
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <crtdbg.h>
#include <math.h>
#include <winsvc.h>

#include <io.h>			//!! TCW MOD
#include <fcntl.h>		//!! TCW MOD
//#include <fstream.h>

#include <process.h>
#include <pmc.h>

#include "netService.h"
#include "LimitSingleInstance.h"

#include "sendtime.h"//2012-7-13


#pragma comment(lib,"libntservice.lib")

#define F8_DBSYNCD_PORT 10933

typedef struct gmem_slot{
	unsigned short flag;
	unsigned short addr;
	char data[32]; //RTK_TAG中标签的动态实时值
}gmem_slot_t;
#define GSLOT_VALID 0x55aa
#define GSLOT_COUNT 32
#define OUT_SIZE sizeof(gmem_slot_t)*GSLOT_COUNT
#define F8_DBSYNCD_PORT 10933
static int b_senddata=0;//是否发送指定标签的实时值， 1：发送  0：不发送
static gmem_slot_t buf_data[GSLOT_COUNT];
typedef struct gmem_slot_tags{
	int b_used;//0：该结构体不可用  或  1：该结构体可用
	char type;//'a':模拟量，'b':开关量
	int addr;//[0, 1024)
	TAG_NAME tags[32];//标签名
}gmem_slot_tags_t;
static gmem_slot_tags_t buf_tags[GSLOT_COUNT];

typedef struct sync_time_t{
	unsigned short flag;
	WORD time[9];			//unsigned short
} sync_t;
static sync_t buf_time;
static int b_sendtime=0;//是否发送当前时间，1：发送   0：不发送

//2012-7-17
static int b_st = 0;//是否发送当前时间给实时数据库
static char timeTN[128];//从*.ini中读取的系统时间标签名
char net1[20], net2[20];

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "librtkproxy.lib")

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

static CLimitSingleInstance g_SingleInstanceObj(TEXT("PMC_NETD")); 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetService::CNetService()
		: CNTService(TEXT("netd"), TEXT("PMC - Host net state service"))
{

}

CNetService::~CNetService()
{

}

void str2addr(char *str, struct in_addr *addr)
{
	CArgs args;
	parse_arg_ex(str, &args, ".");
	int i1, i2, i3, i4;
	i1 = atoi(args.argv[0]);
	i2 = atoi(args.argv[1]);
	i3 = atoi(args.argv[2]);
	i4 = atoi(args.argv[3]);
	addr->S_un.S_un_b.s_b1 = i1;
	addr->S_un.S_un_b.s_b2 = i2;
	addr->S_un.S_un_b.s_b3 = i3;
	addr->S_un.S_un_b.s_b4 = i4;
}

static bool bStop;

static int QueryService(char * Name)
{
	SC_HANDLE	schSCManager;
	SC_HANDLE	schService;
	SERVICE_STATUS st;

	//utils_trace("Querying service %s : ",Name);
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
		//utils_trace("Not installed.\n");
		CloseServiceHandle(schSCManager);
		return 0;
	}
	//utils_trace("Installed,");
	QueryServiceStatus(schService,&st);

#if 0	
	char reg_path[MAX_PATH];
	char path[MAX_PATH];

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
#endif
	
	CloseServiceHandle(schService);
	CloseServiceHandle( schSCManager );
	return st.dwCurrentState;
}

// query rtdb service
static int rtdb_stat = 0;
static int serv_stat = 0;

static unsigned short dayOfTheYear (int year, int mon, int day) 
{   
    /* Days since Jan. 1 in non-leap yr */ 
    static unsigned short jdays[12] = { 
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 }; 
    /* leap year is divisible by 4 but not by 100, or by 400 */ 
    return ( jdays[mon-1] + day 
             + ((( (((year%4) == 0) && ((year%100) != 0)) 
                   || ((year%400) == 0)) 
                && (mon>2)) ? 1 : 0) ); 
} 

static void init_data()
{
	int i,j;
	char sec[8],tag[8];
	char temp[128];
	
	char *p_workdir;
	char p_gmemini[256];

    //2012-7-17
	//从*.ini文件中读取系统时间标签名 和 是否发送系统时间给实时库的标识
	GetPrivateProfileString("netd", "timeTag", "Default.Default.Default", timeTN, sizeof(timeTN)-1, get_config_file());//2012-7-17
	b_st = GetPrivateProfileInt("netd", "sendSystemTime", 0, get_config_file());

	b_sendtime = GetPrivateProfileInt("netd", "sendtime", 0, get_config_file());
	b_senddata = GetPrivateProfileInt("netd", "senddata", 0, get_config_file());

	
	utils_trace("Zander2, b_st = %d\n", b_st);
	utils_trace("Zander3, tagName = %s\n", timeTN);

	//读配置文件  2012-7-17
	GetPrivateProfileString("netd", "net1", "192.168.0.255",net1, sizeof(net1)-1, "./config/NicBuilder.ini");
	GetPrivateProfileString("netd", "net2", "192.168.1.255",net2, sizeof(net2)-1, "./config/NicBuilder.ini");

	printf("sendtime=%d, senddata=%d\n", b_sendtime, b_senddata);

	printf("config file is: %s\n", get_config_file());

	memset(p_gmemini, 0, sizeof(p_gmemini));
	p_workdir = get_working_dir();
	strcpy(p_gmemini, p_workdir);
	strcat(p_gmemini, "config/gmem.ini");
	printf("gmem file is: %s\n", p_gmemini);


	for(i=0;i<GSLOT_COUNT;i++){
		sprintf(sec,"%d",i);
		GetPrivateProfileString(sec, "type", "n", temp, sizeof(temp)-1, p_gmemini);
		buf_tags[i].type = temp[0];
		GetPrivateProfileString(sec, "address", "-1",temp, sizeof(temp)-1, p_gmemini);
		buf_tags[i].addr = atoi(temp);

		buf_tags[i].b_used=0;
		if(buf_tags[i].addr>=0 && buf_tags[i].addr<1024){
			if(buf_tags[i].type=='a'){
				buf_tags[i].b_used=1;
				for(j=0;j<8;j++){
					sprintf(tag,"%d",j);
					GetPrivateProfileString(sec, tag, "",temp, sizeof(temp)-1, p_gmemini);
					tagname_by_text(&buf_tags[i].tags[j], temp);
				}
			}else if(buf_tags[i].type=='b'){
				buf_tags[i].b_used=1;
				for(j=0;j<32;j++){
					sprintf(tag,"%d",j);
					GetPrivateProfileString(sec, tag, "",temp, sizeof(temp)-1, p_gmemini);
					tagname_by_text(&buf_tags[i].tags[j], temp);
				}
			}
		}
	}

	memset(buf_data, 0, sizeof(buf_data));
	for(i=0;i<GSLOT_COUNT;i++){
		if(buf_tags[i].b_used){
			buf_data[i].flag = 0x55aa;
			buf_data[i].addr = buf_tags[i].addr;
		}
	}
}

//取buf_tags[]中标签的实时值，赋给buf_data[]
static void refresh_data()
{
	int i, j;
	PRTK_TAG p;

	char zero[4];
	zero[0]=0;
	zero[1]=0;
	zero[2]=0;
	zero[3]=0;

	for(i=0;i<GSLOT_COUNT;i++){
		if(buf_tags[i].b_used){
			if(buf_tags[i].type=='a'){
				for(j=0;j<8;j++){
					if(buf_tags[i].tags[j].sname.tag.Data[0]!=0){
						p = query_tag_f(&(buf_tags[i].tags[j]));
						if(p!=0){
							if(p->d.Value.Flags & TF_Valid){
								memcpy(&buf_data[i].data[j*4], &p->d.Value.Value.fltValue, 4);
							}else{
								memcpy(&buf_data[i].data[j*4], zero, 4);
							}
						}
					}					
				}
			}else if(buf_tags[i].type=='b'){
				for(j=0;j<32;j++){
					if(buf_tags[i].tags[j].sname.tag.Data[0]!=0){
						p = query_tag_f(&(buf_tags[i].tags[j]));
						if(p!=0){
							if(p->d.Value.Flags & TF_Valid){
								if(p->d.Value.Value.b){
									buf_data[i].data[j]=1;
								}else{
									buf_data[i].data[j]=0;
								}
							}else{
								buf_data[i].data[j]=0;
							}
						}
					}					
				}
			}
		}
	}
}

//取本地服务的pmcd的状态
static void workThread2(void *p)
{
	while(!bStop){
		Sleep(1000);
		serv_stat = QueryService("pmcd");
		rtdb_stat = GetPrivateProfileInt("pmc", "serverStatus", 0, get_config_file());
	}
}

static void workThread(void *p)
{
	char buf[64] = "host - alive!";
	char bufdr[64] = "rtdb - run!";
	char bufdp[64] = "rtdb - standby!";
	char bufds[64] = "rtdb - stop!";
	char *pbuf;
	int phase=0;

//	printf("config file is: %s\n", net1);
	
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);
    //初始化Winsock服务，使应用程序可以调用任何一个Winsock API函数
	err = WSAStartup(wVersionRequested, &wsaData);
	if(err != 0){
		return;
	}

	if(LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2){
		WSACleanup();
		return;
	}

	SOCKET s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	int tval = 1;
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&tval,sizeof(tval));

	SOCKADDR_IN addr[2];
	memset(addr, 0, sizeof(addr));
	str2addr(net1, &addr[0].sin_addr);//设置ip地址
	addr[0].sin_family = AF_INET;
	addr[0].sin_port = htons(F8_DBSYNCD_PORT);
	
	str2addr(net2, &addr[1].sin_addr);
	addr[1].sin_family = AF_INET;
	addr[1].sin_port = htons(F8_DBSYNCD_PORT + 1);

	while(!bStop){
		if(serv_stat != 0){
			if(serv_stat == 1){//pmcd服务停止
				pbuf = bufds;
			}else{
				if(rtdb_stat == 1){
					pbuf = bufdr;//pmcd服务运行
				}else{
					pbuf = bufdp;//pmcd服务热备
				}
			}
		}else{
			pbuf = buf;//"host alive"
		}
		
		//发送本地实时库服务状态到addr[0]
		sendto(s, pbuf, sizeof(buf), 0, (struct sockaddr*)&addr[0], sizeof(addr[0]));
		Sleep(250);

		//发送指定标签的实时值到addr[0]
		if(b_senddata){
			refresh_data();
			sendto(s, (char *)buf_data, sizeof(buf_data), 0, (struct sockaddr*)&addr[0], sizeof(addr[0]));
		}
		Sleep(200);
         
		//发送当前时间到addr[0]
		if(b_sendtime && phase==1){
			SYSTEMTIME	t;
			GetLocalTime(&t);	
			
			//printf("year: %d\n", t.wYear);	//printf("month:%d\n", t.wMonth);
			buf_time.flag = 0x55bb;
			buf_time.time[0] = t.wYear;
			buf_time.time[1] = t.wMonth-1;
			buf_time.time[2] = t.wDay;
			buf_time.time[3] = t.wDayOfWeek;
			buf_time.time[4] = dayOfTheYear(t.wYear,t.wMonth,t.wDay)-1;

			buf_time.time[5] = t.wHour;
			buf_time.time[6] = t.wMinute;
			buf_time.time[7] = t.wSecond;
			buf_time.time[8] = t.wMilliseconds;		//毫秒，1/1000 s
			sendto(s, (char *)&buf_time, sizeof(buf_time), 0, (struct sockaddr*)&addr[0], sizeof(addr[0]));
		}
		Sleep(50);
		
		//发送本地实时库服务状态到addr[1]
		sendto(s, pbuf, sizeof(buf), 0, (struct sockaddr*)&addr[1], sizeof(addr[1]));
		Sleep(250);
		
		//发送指定标签的实时值到addr[1]
		if(b_senddata){
			refresh_data();
			sendto(s, (char *)buf_data, sizeof(buf_data), 0, (struct sockaddr*)&addr[1], sizeof(addr[1]));
		}
		Sleep(200);
        
		//发送当前时间到addr[1]
		if(b_sendtime && phase==1){
			SYSTEMTIME	t;
			GetLocalTime(&t);	
			
			//printf("year: %d\n", t.wYear);	//printf("month:%d\n", t.wMonth);
			buf_time.flag = 0x55bb;
			buf_time.time[0] = t.wYear;
			buf_time.time[1] = t.wMonth-1;
			buf_time.time[2] = t.wDay;
			buf_time.time[3] = t.wDayOfWeek;
			buf_time.time[4] = dayOfTheYear(t.wYear,t.wMonth,t.wDay)-1;

			buf_time.time[5] = t.wHour;
			buf_time.time[6] = t.wMinute;
			buf_time.time[7] = t.wSecond;
			buf_time.time[8] = t.wMilliseconds;		//毫秒，1/1000 s
			sendto(s, (char *)&buf_time, sizeof(buf_time), 0, (struct sockaddr*)&addr[0], sizeof(addr[0]));
		}
		Sleep(50);
        
		
		//2012-7-13
 		//向实时库写入时间
		//2012-7-17
		if (b_st && phase == 1)
		{
			if (STIME_TAGNAME_CONFIG_OK)
			{
			//	utils_trace("Zander4\n");
				sendTime2pmcdEx(timeTN);
			} 
			else
			{
				sendTime2pmcd();
			}			
		}

		

		phase++;
		phase = phase % 5;
	}

	WSACleanup(); 
}

void CNetService::Run(DWORD, LPTSTR *)
{
	// report to the SCM that we're about to start
	ReportStatus(SERVICE_START_PENDING);

    if ( g_SingleInstanceObj.IsAnotherInstanceRunning() ){
        printf("Another netd instance is already running.\n");
        return;
    }
	
	// report SERVICE_RUNNING immediately before you enter the main-loop
	// DON'T FORGET THIS!
	ReportStatus(SERVICE_RUNNING);

    Init();

	// enter main-loop
	// If the Stop() method sets the event, then we will break out of
	// this loop.
    hStop = ::CreateEvent(0, TRUE, FALSE, 0);
    WaitForSingleObject(hStop,INFINITE);
    if(hStop){
        ::CloseHandle(hStop);
    }
}

void CNetService::Stop()
{
	// report to the SCM that we're about to stop
	// Note that the service might Sleep(), so we have to tell
	// the SCM
	// "The next operation may take me up to 11 seconds. Please be patient."
	ReportStatus(SERVICE_STOP_PENDING, 11000);

    bStop = true;
    if(hStop) ::SetEvent(hStop);
}

bool CNetService::Init()
{
	bStop = false;
	
	if(init_proxy(0, PROXY_ENABLE_BGLOADER | PROXY_SILENT)){
		utils_trace("start pmc proxy success\n");
		//return true;
	}else{
		utils_trace("start pmc proxy failed\n");
		//return true;
	}
    //utils_trace("Zander1\n");
	init_data();
 	_beginthread(workThread, 0, 0);
	_beginthread(workThread2, 0, 0);

    return true;
}
