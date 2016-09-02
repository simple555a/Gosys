/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\pmcd\precomp.h
	file path:	E:\vss\pmc2\src\pmcd
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Precompile header file for PMCD.EXE	
	history:
	created:	10:10:2002   16:10
*********************************************************************/
#define RTK_SERVER

#pragma warning(disable : 4786)

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <signal.h>

#include <locale.h>
#include <string.h>
#include <string>

using namespace std;

#include <pmc.h>

#include "ioss.h"


#pragma comment(lib, "libioss.lib")

#include <shell.h>
#include <transaction.h>
#include <rtk_packet.h>
#include <object.h>
#include <vbus.h>

#include <powermgr.h>

// 2003/5/22, PMCD can take advantage of algorithms implemented
// in client proxy
#include <rtkproxy.h>

#include "pmckey.h"
#include "rtkservice.h"

extern __uint id_main_thread;

extern char * up_time();

int PMC_API  server_filter(
	HVBUS bus,
	int event,
	int data
	);

int PMC_API rtdb_filter(
	HVBUS bus,
	int event,
	int data
	);

extern void PMC_API on_rtdb(
	PCRTK_PACKET data,
	HVBUS bus
	);

extern void PMC_API on_system(
	PCRTK_PACKET data,
	HVBUS bus
	);

extern void PMC_API on_operation(
	PCRTK_PACKET data,
	HVBUS bus
	);

extern void PMC_API on_config(
	PCRTK_PACKET data,
	HVBUS bus
	);

extern void PMC_API on_backup_config(
	PCRTK_PACKET data
	);

extern void open_log();
extern void close_log();

extern __bool setup_rtdb();
extern void destroy_rtdb();

extern HRTK_SHELL g_shell;

extern __bool init_server_shell(void);
extern void uninit_server_shell(void);

extern bool setup_running_state();

struct SHELL_PARAMETER{
    char * src_addr;
    int  src_port;
    RTK_PACKET * content;
    char response[MAX_PACKET_SIZE];
};

extern __bool PMC_API power_state_changed(
	int newState, 
	int context
	);

#define mark_expired(pTag)\
	(pTag)->d.Value.Flags &= ~TF_Valid;\
	(pTag)->d.Value.Flags |= TF_Expired;

std::string _texttime();

__bool switch_to_primary();

__bool switch_to_backup();

bool bring_up_backups();

bool notify_active();

void init_config();
void uninit_config();

extern __uint PrimaryWatchDogCounter;
extern __uint MaxPrimaryWatchDogCounter;
extern __bool g_bAllowBackup;
extern __bool logIsEnabled;

int PMC_API online_broadcast();

int PMC_API offline_broadcast();

extern int PMC_API write_device_alarm(
	const GROUP_KEY * gk, 
	const PRTK_TAG tte, 
	const TAG_VALUE * val, 
	const RTK_HOST * src
	);
extern int PMC_API write_device_alarm_y(
	const GROUP_KEY * gk, 
    const TAG_VALUE * val, 
	const RTK_HOST * src,
	const TAG_KEY * operName
	);
extern int PMC_API write_device_alarm_z(
	const GROUP_KEY * gk, 
	const PRTK_TAG tte, 
	const TAG_VALUE * val, 
	const RTK_HOST * src,
    char * operName
	);

extern int PMC_API power_state_change_alarm(int newState);


