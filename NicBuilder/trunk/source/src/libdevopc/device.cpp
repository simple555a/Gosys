//---------------------------------------------------------
// OPC Custom 1.0
// Synchronous IO
//---------------------------------------------------------
#include "precomp.h"
#pragma hdrstop

#include "opc/opcda_i.c"

COpcDevice::COpcDevice()
{
	m_readAsBstr = __false;;
	m_debugLevel = 0;
	opcState = 0;
	m_dwPeriod = 150;
	m_bMsgLoops = __true;
	m_TagCount = 0;
	m_debugClientHandle = __false;
	RtkInitializeListHead(&m_TagList);
}

__uint COpcDevice::on_msg(__uint msg, __uint wparam, __uint lparam)
{
	write_device_parameter *wdp;

	if(msg == WM_USER+1){ 
		// add a tag to server
		AddItemToServer((PMC_OPC_TAG*)wparam);
	}else if(msg == WM_USER+2){ 
		// write a tag
		wdp = (write_device_parameter*)wparam;
		WriteItem(wdp->hItem, &wdp->newValue);
		delete wdp;
	}

	return 1;
}

void COpcDevice::destructor()
{
	stop(INFINITE);
	delete this;
}

void COpcDevice::pre_run()
{
	if(startOPC()){
		opcState = 1;
	}

	notify(EVT_INIT_COMPLETE);
}

void COpcDevice::task()
{
	if(opcState == 0){
		return;
	}

	updateTagCollection();
}

void COpcDevice::post_run()
{
	stopOPC();
}

//---------------------------------------------------------
// LocalInit
// This is generic initialization for a task using COM
//---------------------------------------------------------
__bool COpcDevice::LocalInit(void)
{
	HRESULT r1;

	// General COM initialization...
	m_pIMalloc =NULL;
	m_pOPC = NULL;
	m_pItemMgt =NULL;
	m_GrpSrvHandle = 0;
	m_pSyncIO = NULL;
	m_bComInited = false;
	client_handle = 0;
    
	r1 = CoInitialize(NULL);

	if (FAILED(r1)){
		utils_error("libpmcopc - CoInitialize error\n");
		return __false;
	}

	m_bComInited = true;
	// Also get access to the COM memory manager
	r1 = CoGetMalloc(MEMCTX_TASK, &m_pIMalloc);

	if (FAILED(r1)){
		utils_error("libpmcopc - GetMalloc failed!\n");
		m_pIMalloc=NULL;
		return __false;
	}

	return __true;
}


//---------------------------------------------------------
// Create the requested OPC Server
//---------------------------------------------------------
static IUnknown * connectDcom(WCHAR * node, CLSID & clsid)
{		
	HRESULT r1;
	COSERVERINFO si;
	MULTI_QI	mqi;
	
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&mqi, sizeof(mqi));
	
	mqi.pIID = &IID_IUnknown;
	si.pwszName = node;
	
	// Create an OPC Server Class Factory
	//
	r1 = CoCreateInstanceEx(
		clsid,
		NULL, 
		CLSCTX_REMOTE_SERVER,
		&si,
		1,
		&mqi
		);
	if (FAILED(r1)){
		wperror("CoCreateInstanceEx");
		return 0;
	}

	return mqi.pItf;
}

//---------------------------------------------------------
// CreateOpcServer
// Create the requested OPC Server
//---------------------------------------------------------
__bool COpcDevice::CreateOpcServer()
{   	
	HRESULT r1;
	IUnknown *pUNK;

	/*
		parse m_opcSource
		m_opcSource should be one of the following format:

		1) <prog id>, e.g., 
		'invensys.dataServer.opc.1'

		2) <guid>, e.g., 
		'{F5A2DE4D-1F8C-4d3a-A327-959226EB6BD3}'
		
		3) \\<site name>\<prog id>
		not supported yet, because I just have no idea to translate
		a remote prog id to guid.
		
		4) \\<site name>\<guid>, e.g.,
		\\ifix-node\{F5A2DE4D-1F8C-4d3a-A327-959226EB6BD3}

		site name may be any UNC that DCOM recognizes, i.e., alpha-name
		or IP address when Netbios over Tcp is installed.

		jhunter, 2005/4/11
	*/
	WCHAR * serverId;
	if(m_opcSource[0] == '\\' && m_opcSource[1] == '\\'){
		// remote OPC server requested
		m_opcServerSite = wcstok(m_opcSource, L"\\");
		if(!m_opcServerSite){
			return __false;
		}
		serverId = wcstok(NULL, L"\\");
	}else{
		m_opcServerSite = L"127.0.0.1";
		serverId = m_opcSource;
	}
	// Get the CLSID from the Name
	if(FAILED(r1 = CLSIDFromString(serverId, &m_opcServerGuid))){
		r1 = CLSIDFromProgID(m_opcSource, &m_opcServerGuid);
	}
	if(FAILED(r1)){
		wperror("CLSIDFromProgID");
		return __false;
	}

	pUNK = connectDcom(m_opcServerSite, m_opcServerGuid);
    
	if (!pUNK){
		return __false;
	}
  
	utils_traceW(
		L"OPC server \\\\%s\\%s connected.\n",
		m_opcServerSite,
		serverId
		);
	m_pOPC=NULL; 
	if(pUNK){
		// Request an IOPCServer interface from the object.
		r1 = pUNK->QueryInterface(IID_IOPCServer, (void**)&m_pOPC);
		if (FAILED(r1)){
			utils_error("%08x\n", r1);
			wperror("libpmcopc - QueryInterface()");
			pUNK->Release();
			return __false;
		}
		pUNK->Release();
		pUNK=NULL;
	}
	
	return __true;
}

//---------------------------------------------------------
// LocalCleanup
// This is generic cleanup for any task using COM.
//---------------------------------------------------------
void COpcDevice::LocalCleanup(void)
{
	m_ListLock.Lock();
	PRTK_LIST_ENTRY lk;
	while(!RtkIsListEmpty(&m_TagList)){
		lk = m_TagList.Flink;
		PMC_OPC_TAG * tag;
		tag = SAFE_CONTAINING_RECORD(lk, PMC_OPC_TAG, link);
		if(tag->pItemResult){
			m_pIMalloc->Free(tag->pItemResult);
		}
		RtkRemoveEntryList(&tag->link);
		m_TagCount--;
		*((PMC_OPC_TAG**)tag->tte->d.BinaryAddress) = 0;
		delete tag;
	}
	m_ListLock.UnLock();
	
	// Finally, release the memory manager
	// as well as COM
	//
	if(m_pIMalloc){
		m_pIMalloc->Release();
		m_pIMalloc = NULL;
	}
	
	if(m_bComInited){
		CoUninitialize();
		m_bComInited = false;
	}
}

//---------------------------------------------------------
//CloseServer
//Disconnect the OPC-Server
//---------------------------------------------------------  
void COpcDevice::CloseServer()
{
	if(!m_pOPC){		
		return;
	}

	m_pOPC->Release();
	m_pOPC=NULL;
}

__bool COpcDevice::config(const char * s)
{
	CArgs args;
	parse_arg_ex(s, &args, " ");
	int debugLevel = 0;
	__bool readAsBstr = __false;
	int loop = m_dwPeriod;
	BEGIN_ARGS
		ARG("debug=", debugLevel, "int", "debugging level")
		ARG("bstr", readAsBstr, "tbool", "read values as BSTR")
		ARG("loop=", loop, "int", "loop interval in milliseconds.")
		ARG("handledebug", m_debugClientHandle, "tbool", "debug client handle increment")
	END_ARGS
	process_args_ex(
		args.argc, 
		args.argv, 
		__CmdLineArgValues,
		0,
		0
		);
	m_debugLevel = debugLevel;
	m_readAsBstr = readAsBstr;
	m_dwPeriod= loop;

	return __true;
}

//---------------------------------------------------------
//AddGroupToServer
//Add one group to server by name of group 
//--------------------------------------------------------- 
__bool COpcDevice::AddGroupToServer(LPWSTR szName)
{
	LONG TimBias;
	FLOAT PercDeadband;
	DWORD dwLCID;
	DWORD RevUpRate;        //out

	if(m_GrpSrvHandle !=0){
		utils_error("There is already a group\n");
		return __false;
	}
	if(!m_pOPC){
		utils_error("No Server connected!");
		return __false;
	}

	HRESULT r1;
	TimBias= 0;
	PercDeadband=0.0;
	dwLCID=0x409;
	r1=m_pOPC->AddGroup(
		szName,
		TRUE,
		m_dwPeriod,
		1,
		&TimBias,
		&PercDeadband,
		dwLCID,
		&m_GrpSrvHandle,
		&RevUpRate,
		IID_IOPCItemMgt,
		(LPUNKNOWN*)&m_pItemMgt
		);

	if(FAILED(r1)){
		utils_error("Can't add group to server!");
		m_pItemMgt=NULL;
		m_GrpSrvHandle=0;
		return __false;
	}

	r1=m_pItemMgt->QueryInterface(IID_IOPCSyncIO,(void**)&m_pSyncIO);
	if(FAILED(r1)){
		utils_error("No IOPCSyncIO Found\n");
		return __false;
	}

	return __true;
}

//---------------------------------------------------------
//RemoveGroup
//Remove one group
//---------------------------------------------------------
void COpcDevice::RemoveGroup()
{
	HRESULT r1;

	if(!m_pOPC){		
		return;
	}
	
	if(m_GrpSrvHandle ==0){
		return;
	}
	
	r1=m_pOPC->RemoveGroup(m_GrpSrvHandle,TRUE);
	
	if(m_pSyncIO){
		m_pSyncIO->Release();
		m_pSyncIO = NULL;
	}
	
	if(m_GrpSrvHandle){
		m_pItemMgt->Release();
		m_pItemMgt=NULL;
		m_GrpSrvHandle=0;
	}	
}

//---------------------------------------------------------
//AddItemToServer
//Add items to server. Result of items will be  stored in '*m_pItemResult'
//---------------------------------------------------------
__bool COpcDevice::AddItemToServer(PMC_OPC_TAG * tag)
{
	OPCITEMDEF  item;
	HRESULT     *pError;
	HRESULT     r1; 
	__bool ret = __true;

	if(!m_pOPC){
		utils_error("No server connected!\n");
		return __false;
	}
	
	if(m_GrpSrvHandle ==0){			
		utils_error("No group found!\n");
		return __false;
	}

	m_ListLock.Lock();

	if(tag->pItemResult){
		m_pIMalloc->Free(tag->pItemResult);
		tag->pItemResult = 0;
	}

	item.szAccessPath=L"";
	item.szItemID = tag->itemId;
	item.bActive=TRUE;
	client_handle++;
	item.hClient=client_handle;
	item.dwBlobSize=0;
	item.pBlob=NULL;
	item.vtRequestedDataType=tag->itemType;

	//using IOPCItemMgt::AddItems(), look at OPC spezifikation
	r1=m_pItemMgt->AddItems(1, &item, &(tag->pItemResult), &pError);
	if(FAILED(r1) && (r1 !=S_FALSE)){
		utils_trace("Add Items failed!\n");
		ret = __false;
	}else{
		if(FAILED(pError)){
			utils_trace("Error at adding Item!\n");
			ret = __false;
		}
	}

	if(!ret && tag->pItemResult){
		m_pIMalloc->Free(tag->pItemResult);
		tag->pItemResult = 0;
	}
	
	m_pIMalloc->Free(pError);

	/*
		2005/6/27, jackie
		fix a suspected bug of RSLinx OPC server, discontinuous client handle
		might cause problem
	*/
	if(!ret){
		if(!m_debugClientHandle){
			client_handle--;
		}
	}
	
	m_ListLock.UnLock();

	return ret;
}
 
//---------------------------------------------------------
//WriteItem
//Write item values by given itemIDs
//---------------------------------------------------------
__bool COpcDevice::WriteItem(OPCHANDLE hItem, VARIANT *pNewValue)
{
	HRESULT     *pError;	
	HRESULT     r2;
	BOOL        found=FALSE;

	//using IOPCSyncIO::Write(), look at OPC specification
	r2=m_pSyncIO->Write(1, &hItem, pNewValue, &pError);

	if(FAILED(r2)){
		utils_trace("Error at writing item value\n");
		return __false;
	}else{
		if(pError && FAILED(pError)){
			utils_trace("Error at writing Item\n");
			return __false;
		}
	}

	utils_trace("Success at writing Item\n");
	m_pIMalloc->Free(pError);

	return __true;
}

#if 1
#define pmc_type_to_variant_type pmcType2VariantType
/*
	this should be made a library function
*/
VARENUM PMC_API pmc_type_to_variant_type(__u8 t)
{
	static VARENUM typeTable[]={
		VT_EMPTY, VT_BOOL, VT_R4, VT_I4, VT_R8, 
		VT_DATE, VT_I1, VT_I2, VT_I8, VT_UI1,
		VT_UI2, VT_UI4, VT_UI8, VT_LPSTR, VT_BSTR
	};
	if(t >= sizeof(typeTable) / sizeof(typeTable[0])){
		return VT_EMPTY;
	}
	return typeTable[t];
}
#endif

__bool COpcDevice::addOneTag(PRTK_TAG tte)
{
	bool	bNewlyCreated = false;
	PMC_OPC_TAG *tag;

	int n;
	WCHAR temp[ITEMID_LENGTH];
	n = MultiByteToWideChar(
		0, 
		0, 
		tte->s.Address, 
		-1, 
		temp, 
		ITEMID_LENGTH
		);

	tag = *((PMC_OPC_TAG**)tte->d.BinaryAddress);
	if(!tag){
		tag = new PMC_OPC_TAG;
		if(!tag){
			return __false;
		}
		memset(tag, 0, sizeof(PMC_OPC_TAG));
		RtkInitializeListHead(&tag->link);
		*((PMC_OPC_TAG**)tte->d.BinaryAddress) = tag;
		bNewlyCreated = true;
		tag->tte = tte;
	}	

	m_ListLock.Lock();

	memset(tag->itemId, 0, sizeof(tag->itemId));
	wcscat(tag->itemId, temp);

	/*
		read all values as BSTR will permite more values
		to be read, observed with iFix 3.5, a weird behaviour
	*/
	if(m_readAsBstr){
		tag->itemType = VT_BSTR;
	}else{
		tag->itemType = (VARENUM)pmc_type_to_variant_type(
			get_value_type(tte->s.Flags)
			);
	}
	if(bNewlyCreated){
		RtkInsertTailList(&m_TagList, &tag->link);
		m_TagCount++;
	}
	
	post_message(WM_USER+1, (__uint)tag, 0);

	m_ListLock.UnLock();

	return __true;
}

void COpcDevice::readOneTag(PCRTK_TAG tte, pmc_value_t * pt)
{
	PMC_OPC_TAG	* tag;
	
	m_ListLock.Lock();

	tag = *((PMC_OPC_TAG**)tte->d.BinaryAddress);

	if(!tag){
		goto __done;
	}

	if(tag->valid){
		pmc_value_t tv;
		if(!pmc_variant_to_value(&tag->itemValue, &tv)){
			goto __done;
		}
		if(pmc_type_cast(&tv, pt)){
				pt->Flags |= TF_Valid;
		}
	}

__done:
	m_ListLock.UnLock();
}

__bool COpcDevice::startOPC()
{
	if(!LocalInit()){
		return __false;
	}

	if(!CreateOpcServer()){
		return __false;
	}

	if(!AddGroupToServer(GROUP_NAME)){
		return __false;
	}

	return __true;
}

void COpcDevice::stopOPC()
{
	RemoveGroup();

	CloseServer();

	LocalCleanup();
}

void COpcDevice::updateTagCollection()
{
	OPCITEMSTATE	*pItValues;
	HRESULT			*pErrors;
	HRESULT			r2;
	PRTK_LIST_ENTRY	lk;
	PMC_OPC_TAG		*tag;
	OPCHANDLE		*phServer, *p;
	int				i;
	int				count;

	if(m_debugLevel){
		utils_debug(
			"This is %d,%d,%d\n", 
			opcState,
			this->m_dwPeriod, 
			this->m_debugLevel
			);
	}
	m_ListLock.Lock();

	phServer=new OPCHANDLE[m_TagCount];

	lk = m_TagList.Flink;
	p = phServer;
	count = 0;
	while(lk != &m_TagList){
		tag = SAFE_CONTAINING_RECORD(lk, PMC_OPC_TAG, link);
		if(tag->pItemResult){
			*p = tag->pItemResult->hServer;
			p++;
			count++;
		}
		lk = lk->Flink;
	}

	r2=m_pSyncIO->Read(
		OPC_DS_DEVICE, 
		count, 
		phServer, 
		&pItValues, 
		&pErrors
		);

	if(FAILED(r2)){
		wperror("OPC -- syncRead error ");
		lk = m_TagList.Flink;		
		while(lk != &m_TagList){
			tag = SAFE_CONTAINING_RECORD(lk, PMC_OPC_TAG, link);
			tag->valid = __false;
			lk = lk->Flink;
		}
	}else{
		if(m_debugLevel){
			utils_debug("OK reading item value!\n");
		}
		lk = m_TagList.Flink;
		i = 0;
		while(lk != &m_TagList){
			tag = SAFE_CONTAINING_RECORD(lk, PMC_OPC_TAG, link);
			if(tag->pItemResult){
				if(pErrors && FAILED(pErrors[i])){
					tag->valid = __false;
				}else{
					tag->valid = __true;
					VariantClear(&tag->itemValue);
					tag->itemValue = pItValues[i].vDataValue;
				}
				i++;
			}
			lk = lk->Flink;			
		}
		if(m_debugLevel){
			utils_debug("OK setting item values!\n");
			utils_debug(
				"I'm %d,%d,%d\n", 
				opcState,
				this->m_dwPeriod, 
				this->m_debugLevel
				);
		}
	}

	delete[] phServer;
	m_pIMalloc->Free(pErrors);
	m_pIMalloc->Free(pItValues);

	m_ListLock.UnLock();
}

void COpcDevice::RemoveOneTag(PRTK_TAG tte)
{
	PMC_OPC_TAG * tag;
	
	assert(tte->d.DeviceObj->OwnerField == (void*)this);
	
	m_ListLock.Lock();
	
	tag = *((PMC_OPC_TAG**)tte->d.BinaryAddress);
	if(tag->pItemResult){
		m_pIMalloc->Free(tag->pItemResult);
	}
	RtkRemoveEntryList(&tag->link);
	m_TagCount--;
	*((PMC_OPC_TAG**)tte->d.BinaryAddress) = 0;

	delete tag;
	
	m_ListLock.UnLock();
}

