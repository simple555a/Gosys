/*
PC based Monitoring System source file
version 1.0
by J.Hunter,jhunter@263.net,May 2001
*/
#include "precomp.h"

#pragma comment (lib,"libutils.lib")
#pragma comment (lib,"librtk.lib")
#pragma comment (lib,"libspac.lib")
#pragma comment (lib,"libntservice.lib")

#define DEFAULT_RESET_TIMEOUT	6000
static int BASE_RESET_TIMEOUT = DEFAULT_RESET_TIMEOUT;

bool g_bCommDebug;

#ifdef USE_DETOURS
extern HANDLE g_ComHandles[16];
extern void twith();
extern void twithout();
#else
#define twith()
#define twithout()
#endif

static int init(void);
static int uninit(void);

CLimitSingleInstance g_SingleInstanceObj(TEXT("PMC_PCMSPAC")); 

class CScheduler : 
	public CRtkThread, CSerialPort
{
public:
	CScheduler(int m_iPortNo);
	virtual ~CScheduler();
	virtual __uint run();
	__bool check_and_open()
	{
		if(!opened()){
			open();
		}
		return opened();
	}
protected:
	HANDLE m_hEvents[2];
};
 
class CScheduler * g_Brokers[rtkm_max_spac_ports];

static int _restart();

CScheduler::CScheduler(int pno)
	: CSerialPort(pno)
{
	char name[32];
	spac_shm * shm = (spac_shm *)&ports[m_iPortNo-1];	

	SECURITY_DESCRIPTOR	sd, *pSD;
	SECURITY_ATTRIBUTES sa,*_sa;
	
	this->m_iPortNo = m_iPortNo;
	pSD = &sd;
	
	if (pSD == NULL){
		utils_error("Error creating security descriptor.\n");
		return;
	}
	
	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)){
		utils_error("Error initializing security descriptor.\n");
		return;
	}
	
	// Add a NULL DACL to the security descriptor..
	// see MSDN Knowledge base Q106387 for detail
	
	if (!SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE)){
		utils_error("Error setting security descriptor.\n");
		return;
	}
	
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = TRUE;
	
	_sa=&sa;
	
	sprintf(name,"rtkm_spac_event_0_%d",m_iPortNo);
	m_hEvents[0]=CreateEvent(_sa, 0, 0, name);
	if(!m_hEvents[0]){
		m_hEvents[0]=OpenEvent(EVENT_ALL_ACCESS,0,name);
	}
	sprintf(name,"rtkm_spac_event_1_%d",m_iPortNo);
	m_hEvents[1]=CreateEvent(_sa, 0, 0, name);
	if(!m_hEvents[1]){
		m_hEvents[1]=OpenEvent(EVENT_ALL_ACCESS,0,name);
	}
	shm->is_open=false;
	if(!m_hEvents[0] || !m_hEvents[1]){
		utils_error(
			">> COM%d, Error creating event, code %d.\n",
			m_iPortNo,
			GetLastError()
			);
		m_hEvents[0]? CloseHandle(m_hEvents[0]) : 0;
		m_hEvents[1]? CloseHandle(m_hEvents[1]) : 0;
		return;
	}
	ports[m_iPortNo-1].port_no=m_iPortNo;	 
}

CScheduler::~CScheduler()
{
	m_hEvents[0]? CloseHandle(m_hEvents[0]) : 0;
	m_hEvents[0] = 0;
	m_hEvents[1]? CloseHandle(m_hEvents[1]) : 0;
	m_hEvents[1] = 0;
}

__uint CScheduler::run()
{	 
	int len, cmd;
	spac_shm	* shm;

	shm = (spac_shm *)&ports[m_iPortNo-1];
	
	notify(CRtkThread::EVT_INIT_COMPLETE);
	
	HANDLE ev[2];
	ev[0]=m_hEvents[0];
	ev[1]=m_EvtStop.Handle();
	
	bool stopping=false;

	while(!stopping){
		switch(WaitForMultipleObjects(2,ev,0,INFINITE) ) {
		case WAIT_FAILED:
			utils_error("wait failed %d\n",GetLastError()); 		   
			continue;
			break;
		case WAIT_OBJECT_0+1:
			stopping=true;
			continue;
			break;
		}
		
		cmd = shm->i.cmd_code;
		len = shm->i.param[0];
		// prevent re-entrance
		shm->i.cmd_code=-1;
		shm->o.ret_code = 0;
		shm->o.param[0] = 0;

		// case WAIT_OBJECT_0
		switch(cmd){
		case SPAC_READ:
			check_and_open();
			shm->o.ret_code = read(
				shm->buffer, 
				len, 
				(__uint*)&shm->o.param[0]
				);
			break;
		case SPAC_WRITE:
			check_and_open();
			shm->o.ret_code = write(
				shm->buffer, 
				len, 
				(__uint*)&shm->o.param[0]
				);
			break;
		case SPAC_FLUSH:
			shm->o.ret_code = flush();			
			break;
		case SPAC_OPEN:
			shm->o.ret_code = check_and_open();
			break;
		case SPAC_CLOSE:
			shm->o.ret_code = close();
			break;
		default:
			utils_error("Unknown command code %d\n",shm->i.cmd_code);
			shm->o.ret_code = 0;			
			break;
		}

		check_error();

		SetEvent(m_hEvents[1]);
	}
	
	close();

	utils_error(
		"Port %d scheduler stopped, %d sent(%d bytes), %d got(%d bytes), %d errors\n",
		m_iPortNo,
		m_iSent,
		m_iSentBytes,
		m_iGot,
		m_iGotBytes,
		m_iErrCount
		);

	return 1;
}

class CSpacService : public CNTService {
	// the service control manager (SCM) starts "Run()" in a
	// different thread than "Stop()", so we need something
	// for syncronization. This sample uses an "Event". You
	// might use other syncronization methods too (mutexes
	// or whatever you like).
	HANDLE	m_hStop;
	HANDLE	m_hRestart;
public:
	// construct a new service.
	// The first parameter to the baseclass' constructor is the
	// name of the service. This appears in the event-log for
	// instance. The second parameter is the display name, thus
	// what you can read in the "Services" control panel applet.
	// If you ommit the second parameter, then it defaults to be
	// equal to the first one.
	CSpacService ();
	
	// We have to implement at least these two methods:
	virtual void	Run(DWORD, LPTSTR *);
	virtual void	Stop();
};

CSpacService::CSpacService()
: CNTService(TEXT("PMCSPAC"), TEXT("PMC Serial Port Access Coordinator"))
, m_hStop(0)
{
}

// ignore parameters in this sample service
void CSpacService :: Run(DWORD, LPTSTR *) {
	if (g_SingleInstanceObj.IsAnotherInstanceRunning()){
		utils_error("Another PMCSpac Daemon instance is already running.\n");
		return;
	}
	
	// report to the SCM that we're about to start
	utils_trace("Starting PMC Serial Port Access Coordinator Version 1.0...\n");
	ReportStatus(SERVICE_START_PENDING);
	
	m_hStop = ::CreateEvent(0, FALSE, FALSE, 0);
	SECURITY_ATTRIBUTES sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.nLength = sizeof(sd);
	m_hRestart = ::CreateEvent(&sd, FALSE, FALSE, SPAC_RESET_EVENT);
	HANDLE handles[2];
	handles[0] = m_hStop;
	handles[1] = m_hRestart;
	// You might do some more initialization here.
	// Parameter processing for instance ...
	utils_trace("PMC Serial Port Access Coordinator Version 1.0 started...\n");
	init();
	// enter main-loop
	// If the Stop() method sets the event, then we will break out of
	// this loop.
	// report SERVICE_RUNNING immediately before you enter the main-loop
	// DON'T FORGET THIS!
	ReportStatus(SERVICE_RUNNING);	
	bool bStop = false;
	while(!bStop){
		switch(MsgWaitForMultipleObjects(2,handles,false,INFINITE,QS_ALLEVENTS) ){
		case WAIT_OBJECT_0:
			bStop = true;
			break;
		case WAIT_OBJECT_0 + 1:
			_restart();
			break;
		case WAIT_OBJECT_0 + 2:
			MSG msg;
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
		}
	}
	if( m_hStop )	::CloseHandle(m_hStop);
	uninit();
	ReportStatus(SERVICE_STOPPED);
}

static int _restart()
{
	// report to the SCM that we're about to start
	utils_trace("Re-starting PMC Serial Port Access Coordinator Version 1.0...\n");
	uninit();
	init();
	utils_trace("PMC Serial Port Access Coordinator Version 1.0 re-started...\n");
	return 1;
}

void CSpacService :: Stop() {
	// report to the SCM that we're about to stop
	// Note that the service might Sleep(), so we have to tell
	// the SCM
	//	"The next operation may take me up to 11 seconds. Please be patient."
	ReportStatus(SERVICE_STOP_PENDING, 11000);	  
	if( m_hStop )	{
		::SetEvent(m_hStop);
	}
}

static int init(void)
{
	utils_trace(">> Starting scheduler...\n");
	
	for(int i=0;i<rtkm_max_spac_ports;i++){
		g_Brokers[i] = new CScheduler(i+1);
		if(!g_Brokers[i]){
			utils_error("Create thread %d failed.\n",i);
			return 0;
		}
		g_Brokers[i]->start();
		if(WAIT_OBJECT_0 != g_Brokers[i]->wait(CRtkThread::EVT_INIT_COMPLETE, 5000)){
			utils_error("Scheduler thread %d init failed.\n",i);
			return 0;
		}
	}	 
	
	return 1;
}

static int uninit(void)
{
	for(int i=0;i<rtkm_max_spac_ports;i++){
		if(!g_Brokers[i]->stop(1000)){
			utils_error(
				"Warning:thread %d didn't exit in given time, force exit\n",
				i
				);
		}
		delete g_Brokers[i];
		g_Brokers[i] = 0;
	}	 
	return 1;
}

CSpacService serv;

int main(int argc,char ** argv)
{
	char * startupdir=getenv("PMC");
	if(startupdir){
		strupr(startupdir);
		if(startupdir[1] == ':'){
			_chdrive(startupdir[0]-'A'+1);
		}
		chdir(startupdir);
	}
	for(int i=1; i<argc; i++){
		if(!strcmp(argv[i], "--debug")){
			g_bCommDebug = true;
		}
	}
	setlocale(LC_ALL,"chs");
	utils_puts = rtk_logged_puts;
	twith();
	serv.RegisterService(argc, argv);
	twithout();
	return 0;
}
