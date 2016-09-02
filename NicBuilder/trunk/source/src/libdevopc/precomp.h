#define _WIN32_WINNT 0x0500
#define	 RTK_PLUGINS_EXPORTS
#include <pmc.h>
#include <object.h>
#include <rtk_list_entry.h>

#include <opc/opcda.h>
#include <opc/opcerror.h>

#include <string>
using namespace std;


#define OPC_SRC_LENGTH 64
#define CONNECTION_LENGTH 64
#define ITEMID_LENGTH 128

#define GROUP_NAME L"group1"

struct PMC_OPC_TAG{
	__bool		valid;
	WCHAR		itemId[ITEMID_LENGTH];
	VARENUM		itemType;
	VARIANT	itemValue;
	TAG_NAME	tagName;
	RTK_LIST_ENTRY link;
	PRTK_TAG tte;
	OPCITEMRESULT *pItemResult;	
};

struct write_device_parameter{
	OPCHANDLE	hItem;
	VARIANT	newValue;
};

class COpcDevice : public CPeriodicThread{
public:
	// Global interface to the COM memory manager
	
	IMalloc *m_pIMalloc;
	
	IOPCServer      *m_pOPC;        //interface of server object
	IOPCItemMgt     *m_pItemMgt;    //interface of group object
	OPCHANDLE       m_GrpSrvHandle;
	IOPCSyncIO      *m_pSyncIO;
	bool			m_bComInited;
	DWORD			client_handle;
	RTK_LIST_ENTRY	m_TagList;
	int				m_TagCount;

	WCHAR	m_opcSource[OPC_SRC_LENGTH];
	WCHAR	* m_opcServerSite;
	GUID	m_opcServerGuid;
	int		opcState;
	CLock	m_ListLock;

	__bool	m_readAsBstr;
	int		m_debugLevel;
	__bool	m_debugClientHandle;
	
	COpcDevice();
	
	void destructor();

	virtual void task();
	virtual void pre_run();
	virtual void post_run();
	virtual __uint on_msg(
		__uint msg, 
		__uint wparam, 
		__uint lparam
		);
	__bool config(const char * s);
	__bool startOPC();
	void stopOPC();
	
	__bool LocalInit(void);
	__bool CreateOpcServer();
	void   LocalCleanup(void);
	void   CloseServer();
	__bool AddGroupToServer(LPWSTR szName);
	void   RemoveGroup();
	__bool AddItemToServer(PMC_OPC_TAG * tag);
	__bool WriteItem(OPCHANDLE, VARIANT * pNewValue);

	__bool	isRealType(WCHAR *add);
	__bool	addOneTag(PRTK_TAG tte);
	void	RemoveOneTag(PRTK_TAG tte);
	void readOneTag(PCRTK_TAG tte, struct pmc_value_t  * pt);
	void	updateTagCollection();
};

