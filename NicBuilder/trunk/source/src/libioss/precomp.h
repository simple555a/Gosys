#ifndef _ioss_precomp_h
#define _ioss_precomp_h


#ifdef _MSC_VER
#pragma warning(disable : 4786)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef __LINUX__
#include <dlfcn.h>
#endif

#include <math.h>
#include <string>
#include <map>
#include <list>

#include <librtdb.h>
#include <ioss.h>

#include <object.h>
#include <rtknet.h>
#include <rtk_packet.h>
#include <object.h>



#pragma warning(disable : 4786)

using namespace std;

class CChangedTagManager 
{
public:
	CChangedTagManager()
	{
		buffer = 0;
		maxitems = 0;
		cds = 0;
	}
	virtual ~CChangedTagManager()
	{
		destroy();
	}
	__bool	init(__uint maxtags, PCGROUP_KEY gk);
	void	add_tag(PCGROUP_KEY gk, PCRTK_TAG tte, PCpmc_value_t tv);
	__bool	broadcast();
	void	destroy();
	inline __uint	get_items()
	{
		return cds->count;
	}
private:
	void				*buffer;
	RTK_PACKET			*packet;
	CHANGED_DATA		*cds;
	CHANGED_DATA_ENTRY	*entries;
	__uint				maxitems;
};

class CDBRefresher : public CPeriodicThread
{
public:
	CDBRefresher();
	~CDBRefresher();

	static __bool init();
	static __bool uninit();
private:
	void	delete_groups();

protected:
	void	check_exception(
		const GROUP_KEY * gk, 
		PRTK_TAG tte, 
		struct pmc_value_t *tv, 
		PCRTK_TIME now
		);	
	int		add_tags(RTK_CURSOR hgroup, string & strTagFile);	
	__bool	load_group(const char * group_name);	

	virtual void task();

public:
	static void post_db_init();
	static void post_db_init_device(PDEVICE_INFO d);
	static void OnAddGroup(PRTK_GROUP);
	static void OnDropGroup(PRTK_GROUP);
	static void OnEditGroup(PRTK_GROUP);
	static void OnAddTag(PRTK_TAG);
	static void OnDropTag(PRTK_TAG);

protected:
	CChangedTagManager	ctm;

protected:
	/*
		group extension for refreshing
	*/
	struct GROUP_R_EXT{
		GROUP_KEY	key;
		__uint		max_quantums;
		__int		phase;
		__uint		refresh_count;
	};
	typedef std::list<GROUP_R_EXT> R_LIST;
	R_LIST rlist;
};

extern CDBRefresher g_Refresher;

PDRIVER_INFO get_driver(PCDEVICE_KEY dev);

__bool pnp_probe_devices();
__bool pnp_stop_devices();

typedef std::list<DRIVER_INFO> DRIVER_LIST;
extern DRIVER_LIST g_Drivers;
typedef std::map<DEVICE_KEY, PDEVICE_INFO>	DEVICE_LIST;
extern DEVICE_LIST g_Devices;

class CInfoEvent : public CEvent{
public:
	CInfoEvent() : CEvent(0, __true, __false), Result(0)
	{
	}
	__uint Result;
};

#define _WM_ADD_GROUP			(WM_USER + 0)
#define _WM_DROP_GROUP			(WM_USER + 1)
#define _WM_EDIT_GROUP			(WM_USER + 2)
#define _WM_CLAIM_OWNERSHIP		(WM_USER + 3)
#define _WM_DISCLAIM_OWNERSHIP	(WM_USER + 4)
#define _WM_PAUSE				(WM_USER + 5)
#define _WM_RESUME				(WM_USER + 6)

extern __r4	g_fltMaxChangeRate;
extern __r4	g_fltTagLife;

void attach_tag_to_device(PRTK_TAG tag, PDEVICE_INFO d);
void detach_tag_from_device(PRTK_TAG tag);
void attach_device_tags(PDEVICE_INFO d);
void detach_device_tags(PDEVICE_INFO d);

extern int PMC_API get_exception(
	const GROUP_KEY * gk, 
	PRTK_TAG tte, 
	PCpmc_value_t val, 
	PCRTK_TIME now
	);



#endif //_ioss_precomp_h
