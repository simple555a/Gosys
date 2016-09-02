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
__int g_QueryInterval=1000;      //��do_load()����Ҫ�õ�rtk_sleep(g_QueryInterval)     װ�ر�����ʱ�õ����ȫ�ֱ���
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
 *����˵���������صı�ǩ��ֵ����Ϊ���ݰ��д�������ֵ
 *          1���β�packet��data��������һ��CHANGED_DATA�Ͷ��CHANGED_DATA_ENTRY
 *          2�������α���ƴ�packet.src.host�ڵ��Ӧ�ı�ǩ�洢���ڴ棬��CHANGED_DATA_ENTRY�еı�ǩ��ֵ������Щ�ڴ棬�����µ��ֵ
 *          3����¼δ�ҵ��ı�ǩ���͸��������Ƿ������ˣ��򽫶�ʧ�ĵ����CWaitingList�У����ǿͻ��ˣ��򲻴棬���ء�
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
		hNode = open_node(&name.node);      //librtdb.dll        �򿪽ڵ��α�
	}
	if(!hNode){
		unlock_rtdb();
		return 0;
	}
	hGroup = open_group(hNode, &cd->group);     //librtdb.dll       �����α�
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
		hTag = open_tag_g(hGroup, &t[i].name);                       //�򿪱�ǩ�α�
		if(hTag){
			pTag = (PRTK_TAG)cursor_get_item(hTag);   //librtdb.dll       //�򿪱�ǩ�洢���ڴ�
			pTag->d.Value = t[i].value;                                   //�����ݰ��еĶ�ֵ̬������ǩ�洢���ڴ�,����������
			// pTag->d.Value.Flags = t[i].value.Flags;
			mark_fresh(pTag);                                            //��¼�������ݵ�ʱ��
			close_handle(hTag);
			retcount++;                                                  //��¼�����˼�������
		}else{                                                           //else����hTagΪ0�����α����δ�ڱ����ҵ������ݰ��б�ǩ��һ���ı�ǩ�α꣬
			if(g_Worker && nMissing < MAX_MISSING_PERMITED){             //������ñ�ǩ��ʧ����¼��ʧ�ı�ǩ���ͱ�ǩ����     
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
	//����ʧ�ı�ǩ����CWaitingList��
	if(nMissing && g_Worker){
		if(nMissing > 16){
			memset(&missing[0].sname.tag, 0, sizeof(TAG_KEY));
			g_Worker->inform_load(missing, 1);//Ϊʲôֻ����������������
		}else{
			g_Worker->inform_load(missing, nMissing);//Ϊʲôֻ����������������
		}
	}

	return retcount;
}

/*
 *�������ܣ����ݶಥ���еĽڵ����������������ص�database��������_EVENT��ֵ��ǰ���Ǳ��ش��жಥ���еĽڵ�
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
	hNode = open_node(&nk);                 //�򿪽ڵ��α�
	if(!hNode){
		unlock_rtdb();
		return 0;
	}
	
	count = p->data_size / sizeof(RTK_GROUP);
	png = (PRTK_GROUP)p->data;
	for(__uint i=0; i<count; i++, png++){
		hGroup = open_group(hNode, &png->key);    //�����α�
		pg = (PRTK_GROUP)cursor_get_item(hGroup);  //ͨ�����α�Ѱ�Ҵ洢����Ϣ��RTK_GROUP
		if(pg){
			memcpy(
				pg->description, 
				png->description, 
				sizeof(pg->description)
				);
			pg->period = png->period;			
		}else{
			hGroup = create_group(hNode, png);         //û���ҵ�RTK_GROUP���򴴽����α�
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
		if(pg /* && bGenerateEvents */){           //��_EVENT�м�¼��ӵ�����
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
 *�������ܣ������ಥ���е�����ʽtag����Ϣ���ڱ��ش�����Ӧ�Ľڵ㡢�顢��ǩ������¼ʱ�䡣
 *         ��bGenerateEvents=1�����Ὣ�����ı�ǩ������_EVENT
 *��    �أ����ݰ��к��е�tag������
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
    
    //�����ݰ�������ΪPT_DiscoverTags,���ͷ�CWaitingLisd�ж�Ӧ��Ԫ�أ���Ԫ���еĽڵ������ݰ��еĽڵ�һ��
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
	hNode = create_node(&nodekey, 0);          //�����ڵ��α�
	hTag = hGroup = 0;
	name.node = nodekey;
	grp.node = nodekey;
	if(hNode){
		for(i=0; i<tagcount; i++){
			hGroup = open_group(hNode, &ack[i].tag.group);   //�����α�
			grp.key = ack[i].tag.group;
			if(!hGroup){
				hGroup = create_group(hNode, &grp);             //�����α�ʧ�ܣ��򴴽���
			}
			if(hGroup){
				bEditing = query_tag_g(hGroup, &ack[i].tag.key)? __true : __false; //��ѯ��ǩ�α��Ƿ����,��ѯ�������bEditing
				hTag = create_tag(hGroup, &ack[i].tag.key, &ack[i].tag.s);         //������ǩ�α�
				pTag  = (RTK_TAG*)cursor_get_item(hTag);                           //�ñ�ǩ�α��ҵ��洢��ǩ��RTK_TAG
				if(pTag){
					*pTag = ack[i].tag;                                            //�����ݰ��еı�ǩ������RTK_TAG
					/*
						2003/5/27, leave tag's dynamic flags as they were
						2003/6/24, mark tag as newly received, but the valid flag
						is kept as set by server
					*/
					mark_fresh(pTag);  //��¼ʱ��
					close_handle(hTag);
					if(bGenerateEvents){
						name.sname.group = ack[i].tag.group;
						name.sname.tag   = ack[i].tag.key;
						rtk_queue_event(PT_AddTag, &name, &p->src);                //��_EVENT�����ӱ�ǩ
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
 *��������˵����ɾ�����ص����α꣬����_EVENT�м�¼ɾ������������
 *������������ݰ�--���ߺ���Ҫɾ����Щ�飬���Աdata�ɶ��GROUP_KEY��ɡ�
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
	hNode = open_node(&nk);             //�򿪵��α�
	if(hNode){
		for(i=0, pgk=(PGROUP_KEY)packet->data; i<count; i++, pgk++){
			name.sname.group = *pgk;
			hGroup = open_group(hNode, pgk);   //�����α�
			if(hGroup){
				cursor_delete(hGroup);          //ɾ�����α�
				rtk_queue_event(PT_DropGroup, &name, &packet->src); //����ɾ�������α��Ӧ������ӵ�_EVENT�У���
				ret++;
			}
		}
		close_handle(hNode);
	}
	unlock_rtdb();
	return ret;
}

/*
 *�������ܣ�ɾ����ǩ�α꣬����_EVENT��ɾ���ı�ǩ��
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
		hTag = open_tag_f(name);   //�򿪱�ǩ�α�
		if(hTag){
			cursor_delete(hTag);     //ɾ����ǩ�α�
		}
		rtk_queue_event(PT_DropTag, name, &packet->src);     //��¼ɾ���ı�ǩ�α굽_EVENT???
	}
	unlock_rtdb();
}

/*
 *�������ܣ���_EVENT�м�¼Ҫ��ӵ��豸
 *������������ݰ�p�����д洢��Ҫ��ӵ��豸����Ϣ
 */
void _on_add_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_AddDevice, &p->src, p->data);
}

/*
 *�������ܣ���_EVENT�м�¼Ҫ�޸ĵ��豸
 *������������ݰ�p�����д洢��Ҫ�޸ĵ��豸����Ϣ
 */
void _on_edit_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_EditDevice, &p->src, p->data);
}

/*
 *�������ܣ���_EVENT�м�¼Ҫɾ�����豸
 *������������ݰ�p�����д洢��Ҫɾ�����豸����Ϣ
 */
void _on_del_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_DelDevice, &p->src, p->data);
}

/*
 *�������ܣ������α������ȡcount����ǩ�Ķ�ֵ̬������values
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
			hNode = open_node(&names[i].node);   //�򿪽ڵ��α�
			cachedNode = names[i].node;
		}
		if(!hNode){
			values[i].Flags &= ~TF_Valid;
			continue;
		}		
		hTag = open_tag(hNode, &names[i].sname);   //�򿪱�ǩ�α�
		if(hTag){
			existed++;
			pTag = (RTK_TAG*)cursor_get_item(hTag);        //�ɱ�ǩ�α��ҵ��洢��ǩ��RTK_TAG
			double diff;
			diff = rtk_time_diff(&now, &pTag->d.CachedTime);  //����RTK_TAG�е�ʱ��͵�ǰʱ��Ĳ�ֵ
			if(diff > (g_fltTagLife*2)){
				mark_expired(pTag);
			}
			values[i] = pTag->d.Value;     //��ȡ��ǩ�Ķ�ֵ̬��values[]
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
 *�������ܣ���ȫ�ֱ���g_NodeLife��g_fltTagLife��g_QueryInterval��CProxyWorker�ĳ�Աm_MaxBgLoads��ֵ��
            ����ͨ�������ļ�pmc.ini�����Ǹ�ֵ����pmc.ini�и�ֵ���ɹ�����ֱ�Ӹ�ֵ
 *���������bSilent,�����Ƿ��ӡ�������Ϣ
 *���������g_NodeLife��g_fltTagLife��g_QueryInterval��CProxyWorker::m_MaxBgLoads
 *�� �� ֵ����
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
 *˵������ɾ��CWaitList�нڵ�node�������Ϣǰ�ж�g_Worker�Ƿ�Ϊ�ա�
 *      ��Ϊ��ɾ������ʵ���ǵ���g_Worker�ĳ�Ա����purge_load_queue()
 */
static void _on_del_node(PRTK_NODE node)
{
	if(g_Worker){
		g_Worker->purge_load_queue(&node->key);
	}
}

/*
 *�������ܣ������صı�ǩ��ֵ����Ϊ���ݰ��д�������ֵ,
 *��    ע����update_changed_data()�Ĳ�����βε����ݰ������ݺ�û��bThisIsAServer��һ����QUERY_DATA_ENTRY��һ����CHANGED_DATA��CHANGED_DATA_ENTRY
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
    
	//p->data����������QUERY_DATA_ENTRY���
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
	hNode = open_node(&name.node);//�򿪽ڵ��α�
	hTag = hGroup = 0;
	
	if(!hNode){		
		unlock_rtdb();
		if(g_Worker){			
			g_Worker->inform_load(&name, 1);   //Ϊʲôֻ�������βΣ���
		}
		return 0;
	}

	nMissing = 0;
	for(i=0; i<tagcount; i++){
		hGroup = open_group(hNode, &t[i].name.group);   //�����α�
		if(hGroup){                                     //�����α�ɹ�
			hTag = open_tag_g(hGroup, &t[i].name.tag);   //�򿪱�ǩ�α�
			if(hTag){                                    //�򿪱�ǩ�α�ɹ�
				pTag = (RTK_TAG*)cursor_get_item(hTag);   //�ɱ�ǩ�α��ҵ��洢��ǩ��RTK_TAG
				pTag->d.Value = t[i].value;             //�����ݰ��еı�ǩֵ����RTK_TAG��d.Value
				mark_fresh(pTag);                      //��¼ʱ��
				close_handle(hTag);
				retcount++;
			}else{                                    //�򿪱�ǩ�α�ʧ�ܣ������ñ�ǩ��ʧ
				if(g_Worker){
					name.sname = t[i].name;            //��¼��ʧ�ı�ǩ��������
					missing[nMissing] = name;
					nMissing++;
				}
			}
			close_handle(hGroup);
		}else{                                          //�����α�ʧ�ܣ������鶪ʧ��
			if(g_Worker){
				missing[nMissing].node = name.node;       //��¼��ʧ���������������Ľڵ���
				missing[nMissing].sname.group = t[i].name.group;
				memset(&missing[nMissing].sname.tag, 0, sizeof(TAG_KEY));
				nMissing++;
			}
		}
	}
	close_handle(hNode);
	unlock_rtdb();
	if(g_Worker && nMissing){
		g_Worker->inform_load(missing, nMissing);  //����ʧ������ǩ��ӵ�CWaitingList�У�Ϊʲôֻ�������βΣ�����
	}
	return retcount;
}

/*
 *�������ܣ�librtkproxy����ں���
 */
PROXY_API __bool PMC_API init_proxy(HWND hWnd, __uint flags)
{
	char * szWarning = "û���ҵ��������������ʾ״̬�����ݿ�����ģ32��";

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
	
	_load_settings(__false);//��ʼ��һЩȫ�ֱ���
	
	/*3*/
	g_DBDispatchTable.OnAddNode = _on_add_node;
	g_DBDispatchTable.OnDropNode = _on_del_node;
	hook_rtdb_events(&g_DBDispatchTable);           //librtdb.dll   ���βθ���һ������g_Handler[]
	
	/*4*/
	if(!(g_iInitFlag & PROXY_INIT_AS_SERVER)){            //�ܳ׿���
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
	init_network(0);   //librtknet.dll      �½���5��CRtkVBus,�γ�5����ֵ�Բ�������g_buses, ������5���߳�
	/*7*/
	init_rtdb();       //librtdb.dll
	/*8*/
	connect_vbus(BUSID_RTDB, 0, _on_rtdb, default_client_filter);             //librtknet.dll   ��g_buses���ҵ�BUSID_RTDB��Ӧ��CRtkVBus*,Ȼ��ֵ
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
	//����CProxyWorker��������������һ���̣߳�
	g_Worker = new CProxyWorker(flags);
	if(!g_Worker){
		g_InitLock.UnLock();
		return __false;		
	}
	g_Worker->start();
    
	//�����߳����ȼ���g_Worker���߳����ȼ���һ���߳�Ҫ��
#if defined(_WIN32) && 1
	SetThreadPriority(g_Worker->handle(), THREAD_PRIORITY_BELOW_NORMAL);
#endif
	
	/*11*/
	g_InitLock.UnLock();
	return __true;
}

/*
 *�������ܣ�uninit_proxy
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

