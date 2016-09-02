#include "precomp.h"
#pragma hdrstop

#ifdef _WIN32
#include <pmckey.h>
#pragma comment(linker, "/base:0x69000000")
#endif

static int g_iInitCount;
static CLock * _g_InitLock;
#define g_InitLock (*(CLock*)_g_InitLock)
__r4	g_fltTagLife;
__int g_NodeLife;
/* 2005-10-26 this sleep proves to be vital for large 
system(greater than 7000 points)*/
__int g_QueryInterval=1000;      //在do_load()中需要用到rtk_sleep(g_QueryInterval)     装载变量点时用到这个全局变量
CProxyWorker * g_Worker;
static RTDB_DISPATCH_TABLE g_DBDispatchTable;
static int g_iInitFlag;

/*
	If a rtdb network packet contains some tags not present
	locally, then a maximum number of MAX_MISSING_PERMITED
	tags will be checked and reloaded from corresponding servers,
	this helps reduce network overhead on system startup.
	
*/
#define MAX_MISSING_PERMITED 128

/*
 *函数说明：将本地的标签的值更新为数据包中传过来的值
 *          1、形参packet的data部分是由一个CHANGED_DATA和多个CHANGED_DATA_ENTRY
 *          2、利用游标机制打开packet.src.host节点对应的标签存储的内存，将CHANGED_DATA_ENTRY中的标签的值赋给这些内存，即更新点的值
 *          3、记录未找到的标签名和个数，若是服务器端，则将丢失的点存入CWaitingList中，若是客户端，则不存，返回。
 */
PROXY_API __uint PMC_API update_changed_data(
	PCRTK_PACKET packet,
	__bool bThisIsAServer
	)
{
	CHANGED_DATA_ENTRY * t;
	CHANGED_DATA	* cd;
	__uint		i, nMissing;
	RTK_CURSOR	hNode, hGroup, hTag;
	TAG_NAME	name;
	PRTK_TAG	pTag;
	__uint		retcount = 0;
	TAG_NAME	missing[MAX_MISSING_PERMITED];

	if(packet->data_size <= sizeof(CHANGED_DATA)){
		return 0;
	}
	cd = (CHANGED_DATA*)packet->data;
	t = (CHANGED_DATA_ENTRY*)&cd[1];
	if(packet->data_size != cd->count * sizeof(CHANGED_DATA_ENTRY)
		+ sizeof(CHANGED_DATA)
	){
		return 0;
	}

#if 0
	for(i=0; i<cd->count; i++){
		utils_debug("%s.%s.%s=%f\n", 
			(char*)CHostName(packet->src.host),
			(char*)CGroupName(cd->group),
			(char*)CTagName(t[i].name),
			t[i].value.Value.fltValue
			);
	}
#endif

	memset(&name, 0, sizeof(name));
	host_to_node(&packet->src.host, &name.node);
	name.sname.group = cd->group;
	if(!lock_rtdb(__false, 100)){
		return 0;
	}
	if(bThisIsAServer){
		if(!(packet->src.host == g_ThisNode->key)){
			unlock_rtdb();
			return 0;
		}
		hNode = HNODE_LOCAL_MACHINE;
	}else{
		hNode = open_node(&name.node);      //librtdb.dll        打开节点游标
	}
	if(!hNode){
		unlock_rtdb();
		return 0;
	}
	hGroup = open_group(hNode, &cd->group);     //librtdb.dll       打开组游标
	if(!hGroup){
		if(!bThisIsAServer){
			close_handle(hNode);
		}		
		unlock_rtdb();
		if(g_Worker){
			g_Worker->inform_load(&name, 1);
		}
		// 2003/5/27, in current implementation all following
		// tags are guaranteed to be in the same group, so there
		// is no need to proceed.
		return 0;
	}
	nMissing = 0;
	for(i=0; i<cd->count; i++){
		hTag = open_tag_g(hGroup, &t[i].name);                       //打开标签游标
		if(hTag){
			pTag = (PRTK_TAG)cursor_get_item(hTag);   //librtdb.dll       //打开标签存储的内存
			pTag->d.Value = t[i].value;                                   //将数据包中的动态值赋给标签存储的内存,即更新数据
			// pTag->d.Value.Flags = t[i].value.Flags;
			mark_fresh(pTag);                                            //记录更新数据的时间
			close_handle(hTag);
			retcount++;                                                  //记录更新了几个数据
		}else{                                                           //else表明hTag为0，即游标机制未在本地找到与数据包中标签名一样的标签游标，
			if(g_Worker && nMissing < MAX_MISSING_PERMITED){             //则表明该标签丢失，记录丢失的标签名和标签个数     
				name.sname.tag = t[i].name;
				missing[nMissing] = name;
				nMissing++;				
			}
		}
	}
	close_handle(hGroup);
	if(!bThisIsAServer){
		close_handle(hNode);
	}
	unlock_rtdb();
	//将丢失的标签存入CWaitingList中
	if(nMissing && g_Worker){
		if(nMissing > 16){
			memset(&missing[0].sname.tag, 0, sizeof(TAG_KEY));
			g_Worker->inform_load(missing, 1);//为什么只传入了两个参数？
		}else{
			g_Worker->inform_load(missing, nMissing);//为什么只传入了两个参数？
		}
	}

	return retcount;
}

/*
 *函数功能：根据多播包中的节点名和组名，给本地的database的组名和_EVENT赋值，前提是本地存有多播包中的节点
 */
__uint _on_add_edit_groups(PCRTK_PACKET p, bool /* bGenerateEvents */)
{
	RTK_CURSOR hNode, hGroup;
	NODE_KEY nk;
	PRTK_GROUP png, pg=0;
	__uint count;

	if(p->data_size % sizeof(RTK_GROUP)){
		return 0;
	}
	if(!lock_rtdb(__true, 100)){
		return 0;
	}
	host_to_node(&p->src.host, &nk);
	hNode = open_node(&nk);                 //打开节点游标
	if(!hNode){
		unlock_rtdb();
		return 0;
	}
	
	count = p->data_size / sizeof(RTK_GROUP);
	png = (PRTK_GROUP)p->data;
	for(__uint i=0; i<count; i++, png++){
		hGroup = open_group(hNode, &png->key);    //打开组游标
		pg = (PRTK_GROUP)cursor_get_item(hGroup);  //通过组游标寻找存储组信息的RTK_GROUP
		if(pg){
			memcpy(
				pg->description, 
				png->description, 
				sizeof(pg->description)
				);
			pg->period = png->period;			
		}else{
			hGroup = create_group(hNode, png);         //没有找到RTK_GROUP，则创建组游标
			pg = (PRTK_GROUP)cursor_get_item(hGroup);
		}
		if(pg){
			pg->d.flags = png->d.flags;
		}
		DEBUG_PRINTF((
			"Got group %s, flag=%08x\n", 
			(char *)CGroupName(png->key), 
			png->d.flags
			));
		close_handle(hGroup);
		if(pg /* && bGenerateEvents */){           //在_EVENT中记录添加的组名
			TAG_NAME name;
			name.node = nk;
			name.sname.group = png->key;
			rtk_queue_event(
				PT_AddGroup,
				&name,
				&p->src
				);
		}
	}
	close_handle(hNode);
	unlock_rtdb();

	return count;
}

/*
 *函数功能：解析多播包中的三段式tag的信息，在本地创建相应的节点、组、标签，并记录时间。
 *         若bGenerateEvents=1，还会将创建的标签名赋给_EVENT
 *返    回：数据包中含有的tag的数量
 */
__uint _on_add_edit_tags(PCRTK_PACKET p, bool bGenerateEvents)
{
	ONLINE_CONFIG::reload_tags_ack *ack;
	RTK_CURSOR	hNode, hTag, hGroup;
	RTK_TAG		*pTag;
	NODE_KEY	nodekey;
	TAG_NAME	name;
	RTK_GROUP	grp;
	__bool bEditing;
	
	__uint tagcount, i;

	ack = (ONLINE_CONFIG::reload_tags_ack *)p->data;
	tagcount = p->data_size / sizeof(*ack);
	if(p->data_size % sizeof(*ack)){
		return 0;
	}
	if(!tagcount){
		return 0;
	}
	ZeroMemory(&grp, sizeof(grp));
    
    //若数据包的类型为PT_DiscoverTags,则释放CWaitingLisd中对应的元素，该元素中的节点与数据包中的节点一致
	if((PACKET_TYPE(p) == PT_DiscoverTags) && g_Worker){
		TAG_NAME tn;
		memset(&tn, 0, sizeof(tn));
		tn.node = ack[0].tag.node;
		tn.sname.group = ack[0].tag.group;
		g_Worker->cancel_load(&tn);
	}

	if(!lock_rtdb(__true, 100)){
		return 0;
	}
    
	host_to_node(&p->src.host, &nodekey);
	hNode = create_node(&nodekey, 0);          //创建节点游标
	hTag = hGroup = 0;
	name.node = nodekey;
	grp.node = nodekey;
	if(hNode){
		for(i=0; i<tagcount; i++){
			hGroup = open_group(hNode, &ack[i].tag.group);   //打开组游标
			grp.key = ack[i].tag.group;
			if(!hGroup){
				hGroup = create_group(hNode, &grp);             //打开组游标失败，则创建组
			}
			if(hGroup){
				bEditing = query_tag_g(hGroup, &ack[i].tag.key)? __true : __false; //查询标签游标是否存在,查询结果存入bEditing
				hTag = create_tag(hGroup, &ack[i].tag.key, &ack[i].tag.s);         //创建标签游标
				pTag  = (RTK_TAG*)cursor_get_item(hTag);                           //用标签游标找到存储标签的RTK_TAG
				if(pTag){
					*pTag = ack[i].tag;                                            //将数据包中的标签名赋给RTK_TAG
					/*
						2003/5/27, leave tag's dynamic flags as they were
						2003/6/24, mark tag as newly received, but the valid flag
						is kept as set by server
					*/
					mark_fresh(pTag);  //纪录时间
					close_handle(hTag);
					if(bGenerateEvents){
						name.sname.group = ack[i].tag.group;
						name.sname.tag   = ack[i].tag.key;
						rtk_queue_event(PT_AddTag, &name, &p->src);                //在_EVENT中增加标签
					}
					if(bEditing){
						fire_rtdb_event(EV_ModifyTag, pTag);         //librtdb.dll
					}
				}					
			}
			close_handle(hGroup);
		}
		close_handle(hNode);
	}

	unlock_rtdb();

	return tagcount;
}

/*
 *函数功能说明：删除本地的组游标，并在_EVENT中记录删除的组名？？
 *输入参数：数据包--告诉函数要删除哪些组，其成员data由多个GROUP_KEY组成。
 */
__uint PMC_API _on_drop_groups(PCRTK_PACKET packet)
{
	__uint count, i, ret=0;
	RTK_CURSOR hNode, hGroup;
	PGROUP_KEY pgk;
	TAG_NAME name;
	NODE_KEY nk;

	count = packet->data_size / sizeof(GROUP_KEY);
	if(packet->data_size % sizeof(GROUP_KEY)){
		return 0;
	}
	if( !lock_rtdb(true, 100) ){
		return 0;
	}
	host_to_node(&packet->src.host, &nk);
	name.node = nk;
	hNode = open_node(&nk);             //打开点游标
	if(hNode){
		for(i=0, pgk=(PGROUP_KEY)packet->data; i<count; i++, pgk++){
			name.sname.group = *pgk;
			hGroup = open_group(hNode, pgk);   //打开组游标
			if(hGroup){
				cursor_delete(hGroup);          //删除组游标
				rtk_queue_event(PT_DropGroup, &name, &packet->src); //将已删除的组游标对应的组添加到_EVENT中？？
				ret++;
			}
		}
		close_handle(hNode);
	}
	unlock_rtdb();
	return ret;
}

/*
 *函数功能：删除标签游标，并在_EVENT中删除的标签名
 */
void PMC_API _on_drop_tags(PCRTK_PACKET packet)
{
	__uint		count, i;
	PTAG_NAME	name;

	count = packet->data_size / sizeof(TAG_NAME);
	if(packet->data_size % sizeof(TAG_NAME)){
		return;
	}
	if( !lock_rtdb(true, 100) ){
		return;
	}
	for(i=0, name=(PTAG_NAME)packet->data; i<count; i++, name++){
		RTK_CURSOR hTag;
		hTag = open_tag_f(name);   //打开标签游标
		if(hTag){
			cursor_delete(hTag);     //删除标签游标
		}
		rtk_queue_event(PT_DropTag, name, &packet->src);     //记录删除的标签游标到_EVENT???
	}
	unlock_rtdb();
}

/*
 *函数功能：在_EVENT中记录要添加的设备
 *输入参数：数据包p，其中存储着要添加的设备的信息
 */
void _on_add_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_AddDevice, &p->src, p->data);
}

/*
 *函数功能：在_EVENT中记录要修改的设备
 *输入参数：数据包p，其中存储着要修改的设备的信息
 */
void _on_edit_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_EditDevice, &p->src, p->data);
}

/*
 *函数功能：在_EVENT中记录要删除的设备
 *输入参数：数据包p，其中存储着要删除的设备的信息
 */
void _on_del_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_DelDevice, &p->src, p->data);
}

/*
 *函数功能：利用游标机制提取count个标签的动态值，赋给values
 */
__uint _fastcall _read_tags(
	__uint count, 
	PCTAG_NAME names, 
	Ppmc_value_t values,
	__uint & existed
	)
{
	RTK_CURSOR	hNode, hTag;
	NODE_KEY	cachedNode;
	RTK_TAG		*pTag;
	__uint		i, valids;

	valids = 0;
	existed = 0;
	ZeroMemory(values, sizeof(values[0]) * count);
	if(!lock_rtdb(false, 1000)){
		return 0;
	}
	hNode = 0;
	hTag = 0;
	RTK_TIME now;
	rtk_time_mark(&now);
	hNode = 0;
	memset(&cachedNode, 0, sizeof(cachedNode));
	for(i=0; i<count; i++){
		if(!(cachedNode == names[i].node)){
			close_handle(hNode);
			hNode = 0;			
		}
		if(!hNode){
			hNode = open_node(&names[i].node);   //打开节点游标
			cachedNode = names[i].node;
		}
		if(!hNode){
			values[i].Flags &= ~TF_Valid;
			continue;
		}		
		hTag = open_tag(hNode, &names[i].sname);   //打开标签游标
		if(hTag){
			existed++;
			pTag = (RTK_TAG*)cursor_get_item(hTag);        //由标签游标找到存储标签的RTK_TAG
			double diff;
			diff = rtk_time_diff(&now, &pTag->d.CachedTime);  //计算RTK_TAG中的时间和当前时间的差值
			if(diff > (g_fltTagLife*2)){
				mark_expired(pTag);
			}
			values[i] = pTag->d.Value;     //提取标签的动态值到values[]
			valids++;
			close_handle(hTag);
		}else{
			values[i].Flags &= ~TF_Valid;
		}		
	}
	close_handle(hNode);
	unlock_rtdb();
	return valids;
}

/*
 *函数功能：给全局变量g_NodeLife、g_fltTagLife、g_QueryInterval和CProxyWorker的成员m_MaxBgLoads赋值。
            优先通过配置文件pmc.ini给它们赋值，若pmc.ini中赋值不成功，则直接赋值
 *输入参数：bSilent,决定是否打印出相关信息
 *输出参数：g_NodeLife、g_fltTagLife、g_QueryInterval、CProxyWorker::m_MaxBgLoads
 *返 回 值：无
*/
void _load_settings(__bool bSilent)
{
	char buf[32];

	if(!bSilent){
		utils_trace("Loading proxy settings...\n");
	}
    
	/*1*/
	g_NodeLife = GetPrivateProfileInt(
		"PMC",
		"NodeLife",
		rtkm_default_node_life,
		get_config_file()
		);
	if(g_NodeLife < 200 || g_NodeLife > 60000){
		g_NodeLife = rtkm_default_node_life;
	}
	if(!bSilent){
		utils_trace(
			"Node will be kicked after idle for %d ms.\n", 
			g_NodeLife
			);
	}
	
	/*2*/
	*buf = 0;
	GetPrivateProfileString(
		"PMC",
		"TagLife",
		"120",
		buf,
		sizeof(buf),
		get_config_file()
		);
	g_fltTagLife = (__r4)atof(buf);
	if(g_fltTagLife < 5){
		utils_error(
			"TagLife=%.1fs is not valid, reset to %.1fs.\n", 
			g_fltTagLife,
			rtkm_default_tag_life
			);
		g_fltTagLife = rtkm_default_tag_life;
	}else{
		if(!bSilent){
			utils_trace("TagLife is set to %.1fs\n", g_fltTagLife);
		}
	}
    
	/*3*/
	CProxyWorker::m_MaxBgLoads = GetPrivateProfileInt(
		"PMC",
		"MaxBgLoad",
		256,
		get_config_file()
		);
	if(!bSilent){
		utils_trace("BGLoader's queue length is %d entries.\n", CProxyWorker::m_MaxBgLoads);
	}
	
	/*4*/
	g_QueryInterval = GetPrivateProfileInt(
		"PMC",
		"QueryInterval",
		1000,
		get_config_file()
		);
	if(g_QueryInterval < 1000 || g_QueryInterval > 30000){
		if(!bSilent){
			utils_error("QueryInterval=%d is invalid, setting to default.");
		}
		g_QueryInterval = 1000;
	}
	if(!bSilent){
		utils_trace("Query interval set to %d.\n", g_QueryInterval);
	}
}

/*
	proxy's RTDB hook, when a node is added into the database.
	reset its watch dog counter to full life.
*/
static void _on_add_node(PRTK_NODE node)
{
	node->dogcount = g_NodeLife;
}

/*
 *说明：在删除CWaitList中节点node的相关信息前判断g_Worker是否为空。
 *      因为，删除操作实际是调用g_Worker的成员函数purge_load_queue()
 */
static void _on_del_node(PRTK_NODE node)
{
	if(g_Worker){
		g_Worker->purge_load_queue(&node->key);
	}
}

/*
 *函数功能：将本地的标签的值更新为数据包中传过来的值,
 *备    注：与update_changed_data()的差别是形参的数据包的内容和没有bThisIsAServer，一个是QUERY_DATA_ENTRY，一个是CHANGED_DATA和CHANGED_DATA_ENTRY
 */
__uint _on_queried_data(PCRTK_PACKET p)
{
	RTK_CURSOR	hNode, hTag, hGroup;
	RTK_TAG		*pTag;
	TAG_NAME	name;
	TAG_NAME	missing[MAX_MISSING_PERMITED];
	__uint		nMissing;
	
	QUERY_DATA_ENTRY *t;
	__uint tagcount, retcount, i;
    
	//p->data是由整数个QUERY_DATA_ENTRY组成
	t = (QUERY_DATA_ENTRY *)p->data;
	tagcount = p->data_size / sizeof(*t);
	if(p->data_size % sizeof(QUERY_DATA_ENTRY)){
		return 0;
	}
	retcount = 0;
	if(!lock_rtdb(__false, 100)){
		return 0;
	}
	memset(&name, 0, sizeof(name));
	host_to_node(&p->src.host, &name.node);
	hNode = open_node(&name.node);//打开节点游标
	hTag = hGroup = 0;
	
	if(!hNode){		
		unlock_rtdb();
		if(g_Worker){			
			g_Worker->inform_load(&name, 1);   //为什么只有两个形参？？
		}
		return 0;
	}

	nMissing = 0;
	for(i=0; i<tagcount; i++){
		hGroup = open_group(hNode, &t[i].name.group);   //打开组游标
		if(hGroup){                                     //打开组游标成功
			hTag = open_tag_g(hGroup, &t[i].name.tag);   //打开标签游标
			if(hTag){                                    //打开标签游标成功
				pTag = (RTK_TAG*)cursor_get_item(hTag);   //由标签游标找到存储标签的RTK_TAG
				pTag->d.Value = t[i].value;             //将数据包中的标签值赋给RTK_TAG的d.Value
				mark_fresh(pTag);                      //记录时间
				close_handle(hTag);
				retcount++;
			}else{                                    //打开标签游标失败，表明该标签丢失
				if(g_Worker){
					name.sname = t[i].name;            //记录丢失的标签名和数量
					missing[nMissing] = name;
					nMissing++;
				}
			}
			close_handle(hGroup);
		}else{                                          //打开组游标失败，表明组丢失了
			if(g_Worker){
				missing[nMissing].node = name.node;       //记录丢失的组名和其所属的节点名
				missing[nMissing].sname.group = t[i].name.group;
				memset(&missing[nMissing].sname.tag, 0, sizeof(TAG_KEY));
				nMissing++;
			}
		}
	}
	close_handle(hNode);
	unlock_rtdb();
	if(g_Worker && nMissing){
		g_Worker->inform_load(missing, nMissing);  //将丢失的组或标签添加到CWaitingList中，为什么只有两个形参？？？
	}
	return retcount;
}

/*
 *函数功能：librtkproxy的入口函数
 */
PROXY_API __bool PMC_API init_proxy(HWND hWnd, __uint flags)
{
	char * szWarning = "没有找到软件狗，进入演示状态，数据库最大规模32点";

	if(!_g_InitLock){
		_g_InitLock = new CLock;
	}
	
	/*1*/
	g_InitLock.Lock();
	if(g_iInitCount){
		g_iInitCount++;
		g_InitLock.UnLock();
		return __true;
	}

	g_iInitCount++;
    
	/*2*/
	g_iInitFlag = flags;
	
	_load_settings(__false);//初始化一些全局变量
	
	/*3*/
	g_DBDispatchTable.OnAddNode = _on_add_node;
	g_DBDispatchTable.OnDropNode = _on_del_node;
	hook_rtdb_events(&g_DBDispatchTable);           //librtdb.dll   把形参赋给一个数组g_Handler[]
	
	/*4*/
	if(!(g_iInitFlag & PROXY_INIT_AS_SERVER)){            //密匙控制
		unsigned char run, dev;
		int runLevel, devLevel;
		init_key();                                   //libpmckey.dll
		get_key(&run, &runLevel, &dev, &devLevel);    //libpmckey.dll
		if(!dev){
			/* no dog is attached*/
			/* create_group(0x12345678,xxx) is a loophole */
			create_group((RTK_CURSOR)0x12345678, (PRTK_GROUP)32);       //librtdb.dll
			if(!(flags & PROXY_SILENT)){
				MessageBox(
					hWnd,
					szWarning,
					"PMC Error",
					MB_OK
					);
			}
		}else{
			/* yes, dog is attached, set database limit */
			create_group((RTK_CURSOR)0x12345678, (PRTK_GROUP)devLevel);    //librtdb.dll
		}
	}
	/*5*/
	init_powermgr();   //libpowermgr.dll
	/*6*/
	init_network(0);   //librtknet.dll      新建了5个CRtkVBus,形成5个键值对插入链表g_buses, 开启了5个线程
	/*7*/
	init_rtdb();       //librtdb.dll
	/*8*/
	connect_vbus(BUSID_RTDB, 0, _on_rtdb, default_client_filter);             //librtknet.dll   在g_buses中找到BUSID_RTDB对应的CRtkVBus*,然后赋值
	connect_vbus(BUSID_CONFIG, 0, _on_config, default_client_filter);
	connect_vbus(BUSID_ALARM, 0, _on_alarm, default_client_filter);
	connect_vbus(BUSID_SYSTEM, 0, _on_system, default_client_filter);
	connect_vbus(BUSID_OPERATION, 0, 0, default_client_filter);
	
	/*9*/
	//start dispatcher
	if(flags & PROXY_ENABLE_DISPATCHER){
		start_dispatcher();
	}	
    
	/*10*/
	//创建CProxyWorker，并启动（开启一个线程）
	g_Worker = new CProxyWorker(flags);
	if(!g_Worker){
		g_InitLock.UnLock();
		return __false;		
	}
	g_Worker->start();
    
	//设置线程优先级，g_Worker的线程优先级比一般线程要低
#if defined(_WIN32) && 1
	SetThreadPriority(g_Worker->handle(), THREAD_PRIORITY_BELOW_NORMAL);
#endif
	
	/*11*/
	g_InitLock.UnLock();
	return __true;
}

/*
 *函数功能：uninit_proxy
 */
PROXY_API void PMC_API uninit_proxy()
{
	g_InitLock.Lock();

	g_iInitCount--;
	
	if(g_iInitCount){
		g_InitLock.UnLock();
		return;
	}

	if(g_Worker){
		if(g_Worker->stop(3000)){
			delete g_Worker;
		}		
		g_Worker = 0;
	}

	stop_dispatcher();
	uninit_network();
	uninit_rtdb();
	unhook_rtdb_events(&g_DBDispatchTable);

	g_iInitFlag = 0;
		
	g_InitLock.UnLock();

	delete _g_InitLock;
	_g_InitLock = 0;
}

