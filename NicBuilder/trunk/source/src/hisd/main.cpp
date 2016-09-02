/*
	PMC Archiving sub-system
	
	jhunter, jhunter@tsinghua.org.cn
	2004/8

	2007/1/21 fixed a bug, symptom: server will hang on insufficient memory
	when client is querying at a high frequency. buffer manager will not correctly 
	activate the required archive buffer.
*/
#include "precomp.h"
#include "hisdLog.h"

CRecorder * g_recorder;
CHislogService * g_service;

static void loadSettings()
{
	__uint val;
	
#define _load(name, vname, m, M, _def)\
	val = GetPrivateProfileInt("hisd", #name, _def, get_config_file());\
	if(val <= M && val >= m){\
		vname = val;\
		utils_trace("%s = %d\n", #name, val);\
	}else{\
		utils_error("Invalid setting : %s=%d\n", #name, val);\
		vname = _def;\
	}
	
	_load(RecordInterval, hislogRecordInterval, 100, 60*1000, 500)
	_load(ArchiveTimeSpan, hislogArchiveTimeSpan, 15, 86400*7, 900)
	_load(AutoSaveInterval, hislogAutoSaveInterval, 60*10, 3600, 60*10)
	_load(MaxArchiveSize, hislogMaxArchiveSize, 10*1024, 32*1024*1024, 512*1024)
	_load(QueryThreadPoolSize, hislogNumQueryThreads, 1, 10, 1)
	_load(DiskWaterLevel, hislogDiskWaterLevel, 512, 10240, 1024);
	_load(HLogDay, hislogDay, 1, 31*12, 31);
	_load(MaxActiveArchive, hislogMaxActiveArchive, 8, 1024, 25*3600/hislogArchiveTimeSpan);
}

CHislogService::CHislogService()
	: CNTService(TEXT(HISLOG_SERVICE_NAME), TEXT(HISLOG_SERVICE_COMMENT))
{
}

void CHislogService::init()
{
	utils_debug(
		"Starting archive sub-system version %d.%d, please standby...\n",
		ARCHIVER_VERSION >> 8,
		ARCHIVER_VERSION & 0xff
		);
	
	init_proxy(
		0,
//		PROXY_INIT_AS_SERVER |
		PROXY_ENABLE_BGLOADER | 
		PROXY_SILENT
		);
	
	loadSettings();
	
	// start recorder
	bufferMgr.init();

	if(!g_bNoBuffer){
		g_recorder->start();
		g_recorder->wait(CRtkThread::EVT_INIT_COMPLETE, INFINITE);
	}
	
	initQueryProcessor();
	initDebugShell();

	utils_debug("Archive sub-system started.\n");
}

void CHislogService::uninit()
{
	utils_debug("Shutting down archive sub-system, please standby....\n");
	
	uninitDebugShell();
	uninitQueryProcessor();
	
	if(!g_bNoBuffer){
		g_recorder->stop(INFINITE);
	}
	
	bufferMgr.uninit();
	
	uninit_proxy();

	utils_debug("Archive sub-system stopped.\n");
}

// ignore parameters in this sample service
void CHislogService::Run(DWORD, LPTSTR *) 
{
	DWORD res;
	HANDLE handles[2];
	MSG msg;
	RTK_TIME now,last;
	
	// create event so that the daemon could be manipulated from
	// outside the process.
	SECURITY_ATTRIBUTES sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.nLength = sizeof(sd);
	m_EvtConfigChanged = CreateEvent(&sd, FALSE, FALSE, HISLOG_NOTIFY_EVENT);
	m_EvtExit = CreateEvent(&sd, FALSE, FALSE, HISLOG_EXIT_EVENT);
	if(!m_EvtConfigChanged || !m_EvtExit){
		return;
	}
	
	init();
	
	ReportStatus(SERVICE_RUNNING);
	
	handles[0] = m_EvtConfigChanged;
	handles[1] = m_EvtExit;

	rtk_time_mark(&last);
	bool bExit=false;

	while(!bExit){
		rtk_time_mark(&now);
		if((rtk_time_diff(&now,&last) > 30) || (activeArchives>4*hislogMaxActiveArchive)){
		//	bufferMgr.unloadAgedBuffers();
			last=now;
		}
		res = MsgWaitForMultipleObjects(2, handles, FALSE, 1000, QS_ALLEVENTS);
		switch(res){
		case WAIT_OBJECT_0:
			uninit();
			init();
			break;
			
		case WAIT_OBJECT_0 + 1:
			bExit=true;
			break;
			
		case WAIT_OBJECT_0 + 2:
			while(PeekMessage(&msg,NULL,0,0,TRUE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				rtk_time_mark(&now);
				if((rtk_time_diff(&now,&last) > 30) || (activeArchives>4*hislogMaxActiveArchive)){
					//bufferMgr.unloadAgedBuffers();
					last=now;
				}			
			}
		}
	}

	uninit();
}

void CHislogService::Stop() 
{
	// report to the SCM that we're about to stop
	// Note that the service might Sleep(), so we have to tell
	// the SCM
	// "The next operation may take me up to 3 seconds. Please be patient."
	ReportStatus(SERVICE_STOP_PENDING, 3000);
	SetEvent(this->m_EvtExit);
}

static hislog_key_t stringToTime(const char * s)
{
	pmc_value_t t1, t2;
	
	set_value_type(t1.Flags, dt_string);
	t1.Value.str = (char *)s;
	set_value_type(t2.Flags, dt_date);
	pmc_type_cast(&t1, &t2);

	return (hislog_key_t)t2.Value.dbl;
}

extern void randomArchive(
	hislog_key_t t1,
	hislog_key_t t2,
	__uint step
	);

__bool hisdConsoleShell;

int main( int argc, char ** argv ) 
{	
	CHislogService serv;
	CRecorder recorder;
	
	int i;
	char * projDir;
	DWORD fileAttrib;

	g_recorder = &recorder;
	
	for(i=1; i<argc; i++){
		if(!strnicmp(argv[i], "-p:", 3)){
			projDir = argv[i] + 3;
			fileAttrib = GetFileAttributes(projDir);
			if(fileAttrib == (DWORD)-1|| !(fileAttrib & FILE_ATTRIBUTE_DIRECTORY)	){
				utils_error("Invalid project directory : %s\n", argv[i] + 3);
			}else{
				set_working_dir(projDir);
			}
		}else if(!stricmp(argv[i], "-noarchive")){
			/* do not load archive or save in-memory buffer to archive */
			g_bNoArchive = __true;
		}else if(!stricmp(argv[i], "-nobuffer")){
			g_bNoBuffer = __true;
		}else if(!strnicmp(argv[i], "-random:", 8)){
			CArgs aa;
			parse_arg_ex(argv[i] + 8, &aa, ",");
			if(aa.argc == 3){
				loadSettings();
				randomArchive(
					stringToTime(aa.argv[0]), 
					stringToTime(aa.argv[1]), 
					atoi(aa.argv[2])
					);
				return 0;
			}
		}else if(!stricmp(argv[i],"-d") || !stricmp(argv[i],"-shell")){
			hisdConsoleShell=__true;
		}
	}

	//2012-8-15 日志功能
	logIsEnabled = __false;//__true为启动日志记录功能
	open_log();

	g_service = &serv;
	if(g_bNoArchive || g_bNoBuffer){
		serv.Run(argc, argv);
		return 0;
	}

	serv.RegisterService(argc, argv);

	close_log();//2012-8-15

	return 0;
}

