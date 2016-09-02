/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	E:\vss\pmc2\src\pmcd\rtkserver.cpp
	file path:	E:\vss\pmc2\src\pmcd
	author: 	Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Realtime kernel server
	history:
	created:	10:15:2002	 10:26
*********************************************************************/
#include "precomp.h"
#include "rtkgui.h"


static int restart();

CRtkService serv;
__uint PrimaryWatchDogCounter;
__uint MaxPrimaryWatchDogCounter;
__uint PrimaryCheckInterval;
__bool g_bAllowBackup;

char * restart_time(bool calc=false)
{
	time_t ltime;
	struct tm *thetime;
	static unsigned char strTm[100];
	static bool first=true;

	if(!calc){
		return (char*)strTm;
	}
	
	time (&ltime);
	thetime = localtime(&ltime);

	strftime((char *)strTm, 100, "%#c", (const struct tm *)thetime);
	return (char*)strTm;
}

static bool bExit = false;

static void _ctrl_c(int code)
{
	if(!bExit){
		printf("Aborting....\n");
		bExit = TRUE;
	}
}

#define SERVICE_KEY "pmcd"
#define SERVICE_COMMENT "PMC Realtime Database Server"

CRtkService::CRtkService() :
#ifdef _WIN32
		  CNTService(TEXT(SERVICE_KEY), TEXT(SERVICE_COMMENT)),
#endif
		  m_hStop(0)
{
#ifndef NDEBUG
	m_dwStartType = SERVICE_DEMAND_START;
#endif
	m_bPending = __false;

	RTKGUIFILEVERSION v;
	RtkGuiGetFileVersion(&v, "pmcd.exe");
	char vbuf[128];
	sprintf(
		vbuf, 
		"%s %d.%d.%d.%d", 
		SERVICE_COMMENT, 
		v.dwMajorVersion, 
		v.dwMinorVersion,
		v.dwReleaseVersion,
		v.dwBuildNumber
		);
	versionString = vbuf;
}

bool CRtkService::_init()
{
	char serverName[rtkm_node_key_length + 1];

	m_bPending = __true;
	
	PrimaryCheckInterval = GetPrivateProfileInt(
		"PMC",
		"PrimaryCheckInterval",
		500,
		get_config_file()
		);
	if(PrimaryCheckInterval < 50 || PrimaryCheckInterval > 5000){
		utils_error(
			"Warning : PrimaryCheckInterval=%d is invalid, reset to 500.\n",
			PrimaryCheckInterval
			);
		PrimaryCheckInterval = 500;
	}
	MaxPrimaryWatchDogCounter = GetPrivateProfileInt(
		"PMC",
		"PrimaryWatchDogCounter",
		8,
		get_config_file()
		);
	if(MaxPrimaryWatchDogCounter < 3 || MaxPrimaryWatchDogCounter > 50){
		utils_error(
			"Warning : PrimaryWatchDogCounter=%d is invalid, reset to 20.\n",
			PrimaryWatchDogCounter
			);
		MaxPrimaryWatchDogCounter = 8;
	}
	PrimaryWatchDogCounter = MaxPrimaryWatchDogCounter;

	
	m_bPending = __false;

	init_powermgr();

#ifdef _WIN32
	init_spac();//初始化得到security descriptor、mutex对象句柄数组mutics[]和event对象句柄数组events[]
#endif
	
	init_server_shell();//创建g_shell

	init_config();//空函数。。。
	
	GetPrivateProfileString(
		"PMC",
		"ServerName",
		"",
		serverName,
		sizeof(serverName),
		get_config_file()
		);
	//若pmc.ini中没有给出节点名，取本机名为节点名
	if(!serverName[0]){
		DWORD len;
		len = sizeof(serverName);
		GetComputerName(serverName, &len);
	}
	
	//初始化网络，即设定一些值，创建链表g_buses,<bus_id, CRtkVBus*>，并开启5个VBUS线程
	if(!init_network(RTK_INIT_AS_SERVER, serverName)){
		return false;
	}
	//给g_buses中的4个元素的CRtkVBus的成员赋值
	connect_vbus(
		BUSID_RTDB, 
		VBUS_CONNECT_AS_SERVER, 
		on_rtdb, 
		rtdb_filter
		);
	connect_vbus(
		BUSID_SYSTEM, 
		VBUS_CONNECT_AS_SERVER, 
		on_system, 
		server_filter
		);
	connect_vbus(
		BUSID_OPERATION, 
		VBUS_CONNECT_AS_SERVER, 
		on_operation, 
		server_filter
		);
	connect_vbus(
		BUSID_CONFIG, 
		VBUS_CONNECT_AS_SERVER, 
		on_config, 
		server_filter
		);
	utils_trace("Networking Subsystem initialized.\n"); 	

	register_power_callback(power_state_changed, 0);//创建一个CALLBACK_ITEM，用形参给其成员赋值，并链接上RTK_LIST_ENTRY
	// checking primary site
	if(!setup_running_state()){
		return false;
	}	

	setup_rtdb();//在pmc.ini中寻找组名，加载组名对应的*.csv文件中的标签到内存，并开启一个线程。
	utils_trace("Realtime Database Subsystem initialized.\n");
	init_ioss();
	utils_trace("IO Susbsystem initialized.\n");	

	utils_trace("%s started...\n",  versionString.c_str());

	if(get_power_state() == PWR_RUNNING){
		online_broadcast();//发送一个报警数据包，RTK_ALARM_PACKET型
	}

	return true;
}

bool CRtkService::_uninit()
{
	bool is_server;

	is_server = get_power_state()==PWR_RUNNING? true : false;

	utils_trace("Please stand-by ...\n");		
	
	switch_to_backup();

	uninit_ioss();
	utils_trace("IO subsystem stopped.\n");
	destroy_rtdb();
	utils_trace("Realtime Database Subsystem closed.\n");	
#ifdef _WIN32
	uninit_spac();
#endif
	utils_trace("IO Subsystem closed.\n");

	if(is_server){
		if(!bring_up_backups()){
			offline_broadcast();
		}
	}
	
	unregister_power_callback(power_state_changed, 0);

	uninit_network();
	utils_trace("Networking subsystem closed.\n");

	utils_trace("%s stopped.\n",  versionString.c_str());

	uninit_config();
	uninit_server_shell();
	
	return true;
}

// ignore parameters in this sample service
#ifdef _WIN32
void CRtkService :: Run(DWORD,LPTSTR*)
#else
void CRtkService::Run(__uint, char **)
#endif
{
	
	__uint res;

	// report to the SCM that we're about to start
	utils_trace("Starting %s...\n", versionString.c_str());

	ReportStatus(SERVICE_START_PENDING);

	signal(SIGINT, _ctrl_c);

	try{
		if(!_init()){
			throw(this);
		}
	
		ReportStatus(SERVICE_RUNNING);		
		__int logCounter = 24*3600*1000/PrimaryCheckInterval;
		
		while(!bExit){
			res = m_hStop.Wait(PrimaryCheckInterval);
			switch(res){
			case WAIT_OBJECT_0:
				switch(m_dwStopReason){
				case 0:
					bExit = true;
					break;
				case 1:
					_restart();
					ReportStatus(SERVICE_RUNNING);
					break;
				case 2:
					break;
				}
				if(get_power_state()==PWR_RUNNING){
					notify_active();
				}
				break;
			case WAIT_TIMEOUT:
				switch(get_power_state()){
				case PWR_RUNNING:
					notify_active();
					break;
				case PWR_BACKUP:
					PrimaryWatchDogCounter--;
					if(PrimaryWatchDogCounter <= 0){
						utils_trace("Primary site failed, resuming responsibility.\n");
						switch_to_primary();
					}
					break;
				}
				logCounter--;
				if(logCounter <= 0){
					close_log();
					open_log();
					logCounter = 24*3600*1000/PrimaryCheckInterval;
				}
				break;
			}
		}
		_uninit();
	}catch(CRtkService *){
		utils_error("Aborted.\n");
	}

	ReportStatus(SERVICE_STOPPED);
}

bool CRtkService::_restart()
{
	utils_trace("Restarting %s...\n", versionString.c_str());
	if(!_uninit()){
		return false;
	}
	if(!_init()){
		return false;
	}
	restart_time(true);

	return true;
}

void CRtkService::Restart()
{
	if(m_bPending){
		return;
	}
	m_bPending = __true;
	ReportStatus(SERVICE_STOP_PENDING, 11000);
	m_dwStopReason = 1;
	m_hStop.Fire();
}

void CRtkService::Stop()
{
	if(m_bPending){
		return;
	}
	m_bPending = __true;
	utils_trace("PMC Runtime Kernel is shutting down....\n");
	ReportStatus(SERVICE_STOP_PENDING, 11000);
	m_dwStopReason = 0;
	m_hStop.Fire();
}





















