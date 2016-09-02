#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#pragma warning (disable : 4786)
#include <windows.h>
#else
#define _fastcall
#endif

#include <list>
#include <stdarg.h>
#include <string>
#include <map>

#include <rtkdefs.h>
#include <librtk.h>
#include <librtdb.h>

#include <rtknet.h>
#include <rtk_packet.h>
#include <transaction.h>

#include <rtk_thread.h>
#include <rtkproxy.h>
#include <powermgr.h>

#include <ioss.h>

#define mark_fresh(pTag)\
do{\
	rtk_time_mark(&(pTag)->d.CachedTime);\
	(pTag)->d.Value.Flags &= ~TF_Expired;\
}while(0)

#define mark_valid(pTag)\
do{\
	(pTag)->d.Value.Flags |= TF_Valid;\
}while(0)

#define mark_expired(pTag)\
do{\
	(pTag)->d.Value.Flags &= ~TF_Valid;\
	(pTag)->d.Value.Flags |= TF_Expired;\
}while(0)

// bus procs
void PMC_API _on_rtdb(PCRTK_PACKET packet, HVBUS bus);
void PMC_API _on_alarm(PCRTK_PACKET packet, HVBUS bus);
void PMC_API _on_config(PCRTK_PACKET packet, HVBUS bus);
void PMC_API _on_system(PCRTK_PACKET packet, HVBUS bus);

#define _on_changed_data update_changed_data

__uint _on_queried_data(PCRTK_PACKET p);
__uint _on_add_edit_tags(PCRTK_PACKET p, bool bGenerateEvents=false);
__uint _on_add_edit_groups(PCRTK_PACKET p, bool bGenerateEvents=false);
void PMC_API _on_drop_tags(PCRTK_PACKET packet);
__uint PMC_API _on_drop_groups(PCRTK_PACKET packet);
void _on_add_device(PCRTK_PACKET p);
void _on_edit_device(PCRTK_PACKET p);
void _on_del_device(PCRTK_PACKET p);
__uint _fastcall _read_tags(__uint count, PCTAG_NAME names, Ppmc_value_t values, __uint & existed);
__bool start_dispatcher();
void   stop_dispatcher();

#define PT_Progress	0x83124571

//
struct task_item_t{
	TAG_NAME tag;
	int failCount;
};

class CProxyWorker : public CPeriodicThread
{
public:
	CProxyWorker(int flag);

	// no lock
	__bool inform_load(PCTAG_NAME jobs, __uint count, bool bForce=false);
	__bool cancel_load(PCTAG_NAME job);
	__bool is_loading(PCTAG_NAME n);
	void purge_load_queue(PCNODE_KEY node);
protected:
	virtual void task();
	typedef list<task_item_t> CWaitingList;
	
	// no lock
	void do_load();
	void kill_nodes();
	void kill_tags();
	__bool load_node(PNODE_KEY nk);
	__bool load_group(PRTK_GROUP_NAME gn);
	__bool load_tag(PCTAG_NAME n);

	CWaitingList	m_Tasks;
public:
	int			m_TaskFlag;
	static __uint	m_MaxBgLoads;
};

extern CProxyWorker * g_Worker;

extern __int	g_NodeLife;
extern __r4		g_fltTagLife;
extern __int g_QueryInterval;
