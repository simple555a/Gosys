/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\pmcd\config.cpp
	file path:	E:\vss\pmc2\src\pmcd
	author:	Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Online configuration	
	history:
	created:	10:15:2002   10:25
*********************************************************************/
#pragma warning(disable : 4786)
#include "precomp.h"
#pragma hdrstop

#include <set>
#include <string>
using namespace std;

#define answer_failed(p)\
	send_rtk_data_with_guid(\
		vbus_config, \
		&p->src, \
		&p->guid, \
		p->packet_type | PF_ACK | PF_Failed\
	)

#define answer_ok(p)\
	send_rtk_data_with_guid(\
		vbus_config, \
		&p->src, \
		&p->guid, \
		p->packet_type | PF_ACK\
	)

static void _on_d_nodes(const RTK_PACKET * data)
{
	DEBUG_PRINTF((
		"Acknowledge existance to %s:%d.\n",
		(char*)CHostName(data->src.host),
		data->src.port
		));
	broadcast_rtk_data_with_guid(
		vbus_config,
		PORT_ALL_CLIENT,
		(RTK_GUID*)data->data,
		data->packet_type | PF_ACK				
		);
}

static void _on_d_groups(const RTK_PACKET * data)
{
	if( !lock_rtdb(false, 5000) ){
		return;
	}
	RTK_CURSOR cursor;
	RTK_GROUP  *grp, *p;
	__uint count, heapsize;

	count = cursor_get_subitem_count(HNODE_LOCAL_MACHINE);
	if(!count){
		unlock_rtdb();
		send_rtk_data_with_guid(
			vbus_config,
			&data->src,
			(RTK_GUID*)data->data,
			PT_DiscoverGroups | PF_ACK
			);
		return;
	}
	heapsize = count*sizeof(RTK_GROUP);
	char * heap=new char[heapsize];
	if(!heap){
		utils_error("memalloc failed in PT_DiscoverGroups.\n");
		unlock_rtdb();
		send_rtk_data_with_guid(
			vbus_config,
			&data->src,
			(RTK_GUID*)data->data,
			PT_DiscoverGroups | PF_Failed | PF_ACK
			);
		return;
	}
	p = (RTK_GROUP*)(heap);
	cursor = cursor_open_first_subitem(HNODE_LOCAL_MACHINE);
	if(cursor){
		while(!cursor_is_end(cursor)){
			grp = (RTK_GROUP*)cursor_get_item(cursor);
			*p = *grp;
			host_to_node(&g_ThisNode->key, &p->node);
			p++;
			cursor_move_next(cursor);
		}
		close_handle(cursor);
	}
	
	unlock_rtdb();

	broadcast_rtk_data_with_guid(
		vbus_config,
		PORT_ALL_CLIENT,
		(RTK_GUID*)data->data,
		PT_DiscoverGroups | PF_ACK,
		heap,
		heapsize
		);
	delete heap;
	
	DEBUG_PRINTF((
		"Acknowledge discover_groups with %d items to %s:%d.\n", 
		count,
		(char*)CHostName(data->src.host),
		data->src.port
		));
}

static void _on_d_tags(const RTK_PACKET * data)
{
	ONLINE_CONFIG::discover_tags *dt;

	if(data->data_size < sizeof(*dt)){
		return;
	}

	dt = (ONLINE_CONFIG::discover_tags*)data->data;

	ONLINE_CONFIG::reload_tags_ack *heap, *p;
	RTK_CURSOR	hGroup, hTag;
	RTK_TAG		*pTag;
	__uint		count, dtasize;

#ifndef NDEBUG
	RTK_TIME t1, t2;
	rtk_time_mark(&t1);
#endif

	if(!lock_rtdb(false, 5000)){
		return;
	}
	
	hGroup = open_group(HNODE_LOCAL_MACHINE, &dt->group);
	if(!hGroup){
		unlock_rtdb();
		return;
	}
	count = cursor_get_subitem_count(hGroup);
	dtasize = sizeof(ONLINE_CONFIG::reload_tags_ack)*count;
	heap = new ONLINE_CONFIG::reload_tags_ack[count];
	if(!heap){
		close_handle(hGroup);
		unlock_rtdb();
		return;
	}
	hTag = cursor_open_first_subitem(hGroup);
	p = heap;
	while(!cursor_is_end(hTag)){
		pTag = (RTK_TAG*)cursor_get_item(hTag);
		p->tag = *pTag;
		host_to_node(&g_ThisNode->key, &p->tag.node);
		p++;
		cursor_move_next(hTag);
	}
	close_handle(hTag);
	close_handle(hGroup);	
	unlock_rtdb();
	
	// 2003/7/13
	// the acknowledge are broadcasted so that all clients will benifit
	broadcast_rtk_data_with_guid(
		vbus_config,
		PORT_ALL_CLIENT,
		&dt->ack_guid,
		PT_DiscoverTags | PF_ACK,
		heap,
		dtasize
		);

#ifndef NDEBUG
	rtk_time_mark(&t2);
	DEBUG_PRINTF((
		"Acknowledge discover_tags on '%s' with %d items to %s:%d in %.3f milliseconds.\n", 
		(char*)CGroupName(dt->group),
		count,
		(char*)CHostName(data->src.host),
		data->src.port,
		rtk_time_diff(&t2, &t1)*1000
		));
#endif
	
	delete heap;
}

static void _on_reload_tags(const RTK_PACKET * data)
{
	ONLINE_CONFIG::reload_tags_ack *ack;
	TAG_NAME				*n;
	__uint count, i, tcount, hitcount;

	count = data->data_size / sizeof(TAG_NAME);
	ack = new ONLINE_CONFIG::reload_tags_ack[count];
	if(!ack){
		return;
	}
	if(!lock_rtdb(false, 5000)){
		delete ack;
		return;
	}
	n = (TAG_NAME*)data->data;
	hitcount = 0;
	for(i=0, tcount=0; i<count; i++){
		if(!is_same_key(&n[i].node, &g_ThisNode->key)){
			continue;
		}
		RTK_TAG *pTag;
		hitcount++;
		pTag = query_tag(HNODE_LOCAL_MACHINE, &n[i].sname);
		if(pTag){
#if 0
			utils_debug(
				"%s.%s=", 
				n[i].sname.group.Data, n[i].sname.tag.Data
				);
			RTK_TAG * tag=pTag;
			if(tag){
				if(tag->s.Type == DT_Logical){
					printf(tag->d.Value.b? "On" : "Off");
				}else{
					printf("%f", tag->d.Value.fltValue);
				}
				printf(",%s", tag->d.Flags & TF_Valid? "Valid" : "Invalid");
				printf("\n");
			}
#endif
			ack[tcount].tag	= *pTag;
			host_to_node(&g_ThisNode->key, &ack[tcount].tag.node);
			tcount++;
		}
	}
	unlock_rtdb();

	if(hitcount){
		send_rtk_data_with_guid(
			vbus_config,
			&data->src,
			&data->guid,
			data->packet_type | PF_ACK,
			ack,
			tcount*sizeof(ack[0])
			);
		utils_trace(
			"Acknowledge reload_tags with %d of %d items to %s:%d\n",
			tcount,
			count,
			(char*)CHostName(data->src.host),
			data->src.port
			);
	}

	delete ack;
}

static void _on_add_edit_tags(const RTK_PACKET * data)
{
	__uint count, i;
	ONLINE_CONFIG::add_tags_ack ack;
	PTAG_NAME			names;
	PSTATIC_TAG_DATA	s;
	__bool				bEdit;
	RTK_CURSOR			h;
	PRTK_TAG			pTag;
	RTK_CURSOR			hgroup;

	count = data->data_size / (sizeof(TAG_NAME) + sizeof(STATIC_TAG_DATA));
	if( data->data_size % (sizeof(TAG_NAME) + sizeof(STATIC_TAG_DATA)) ){
		// invalid data packet
		return;
	}
	names = (PTAG_NAME)data->data;
	s = (PSTATIC_TAG_DATA)(data->data + sizeof(TAG_NAME)*count);
	
	bEdit = PACKET_TYPE(data)==PT_EditTag? __true : __false;
	for(i=0; i<count; i++){
		if(!is_same_key(&names[i].node, &g_ThisNode->key)){
			continue;
		}
		
		if(!lock_rtdb(__true, 5000)){
			continue;
		}

		h = 0;
		pTag = 0;
		
		if(bEdit){
			h = open_tag(HNODE_LOCAL_MACHINE, &names[i].sname);
			pTag = (PRTK_TAG)cursor_get_item(h);
			if(pTag){				
				pTag->s = s[i];

				// dangerous, since the dynamic portion of the tag
				// is not owned by config stub, rather by the ioss.
				// let the ioss manager it through its registered
				// callbacks
				
				// ZeroMemory(&pTag->d, sizeof(pTag->d));
				
				fire_rtdb_event(EV_ModifyTag, pTag);
			}
		}else{
			hgroup = open_group(HNODE_LOCAL_MACHINE, &names[i].sname.group);
			if(hgroup){
				h = create_tag(hgroup , &names[i].sname.tag, s+i);
				pTag = (PRTK_TAG)cursor_get_item(h);
			}			
			close_handle(hgroup);
		}
		
		{
			hgroup = open_group(HNODE_LOCAL_MACHINE, &names[i].sname.group);
			PRTK_GROUP pgroup = (PRTK_GROUP)cursor_get_item(hgroup);
			if(pgroup){
				pgroup->d.flags |= GF_Dirty;				
			}
			close_handle(hgroup);
		}

		if(h){
			assert(pTag);
			ack.tag   = *pTag;
			close_handle(h);
		}
		
		unlock_rtdb();
		
		if(pTag){
			host_to_node(&g_ThisNode->key, &ack.tag.node);
			broadcast_rtk_data_with_guid(
				vbus_config,
				PORT_ALL_CLIENT,
				&data->guid,
				data->packet_type | PF_ACK,
				&ack,
				sizeof(ack)
				);
		}
	}
}

static void _on_drop_tags(PCRTK_PACKET data)
{
	__uint		count, i;
	PTAG_NAME	name;	

	count = data->data_size / sizeof(TAG_NAME);
	if( data->data_size % sizeof(TAG_NAME) ){
		// invalid data packet
		return;
	}
	name = (PTAG_NAME)data->data;
	
	for(i=0; i<count; i++, name++){
		if(!is_same_key(&name->node, &g_ThisNode->key)){
			continue;
		}
		RTK_CURSOR h;
		if(!lock_rtdb(__true, 5000)){
			continue;
		}
		h = open_tag(HNODE_LOCAL_MACHINE, &name->sname);
		if(h){
			cursor_delete(h);
		}
		{
			RTK_CURSOR hgroup = open_group(HNODE_LOCAL_MACHINE, &name->sname.group);
			PRTK_GROUP pgroup = (PRTK_GROUP)cursor_get_item(hgroup);
			if(pgroup){
				pgroup->d.flags |= GF_Dirty;				
			}
			close_handle(hgroup);
		}
		unlock_rtdb();
		if(h){
			broadcast_rtk_data_with_guid(
				vbus_config,
				PORT_ALL_CLIENT,
				&data->guid,
				PT_DropTag | PF_ACK,
				name,
				sizeof(TAG_NAME)
				);
		}
	}
}

static void _on_add_edit_groups(const RTK_PACKET * data)
{
	PNODE_KEY	nk;
	PRTK_GROUP	grp, pngrp;
	__uint		count, i;
	__bool		bEdit;
	RTK_CURSOR	h;

	count = data->data_size / (sizeof(*nk) + sizeof(*grp));
	if(data->data_size % (sizeof(*nk) + sizeof(*grp))){
		return;
	}
	nk = (PNODE_KEY)data->data;
	grp = (PRTK_GROUP)(data->data + sizeof(*nk)*count);
	bEdit = data->packet_type == PT_EditGroup? __true : __false;
	for(i=0; i<count; i++, nk++, grp++){
		if(!is_same_key(nk, &g_ThisNode->key) ){
			continue;
		}
		utils_debug("%s group %s\n", 
			bEdit? "Editing" : "Adding",
			(char *)CGroupName(grp->key)
			);

		if(!lock_rtdb(__true, 5000)){
			continue;
		}

		h = 0;
		pngrp = 0;
		
		if(bEdit){
			h = open_group(HNODE_LOCAL_MACHINE, &grp->key);
			if(h){
				pngrp = (PRTK_GROUP)cursor_get_item(h);
				pngrp->period = grp->period;
				pngrp->d.flags |= GF_Dirty;
				strncpy(
					pngrp->description, 
					grp->description, 
					sizeof(pngrp->description)
					);
				pngrp->description[sizeof(pngrp->description) - 1] = 0;
				// ModifyGroup event has to be manually fired
				// (while AddGroup auto-fires)
				fire_rtdb_event(EV_ModifyGroup, pngrp);
			}
		}else{
			h = create_group(HNODE_LOCAL_MACHINE, grp);
			pngrp = (PRTK_GROUP)cursor_get_item(h);
			pngrp->d.flags |= GF_Dirty;
		}

		if(h){
			assert(pngrp);
			*grp = *pngrp;
			close_handle(h);
		}
		
		unlock_rtdb();

		if(pngrp){
			broadcast_rtk_data_with_guid(
				vbus_config,
				PORT_ALL_CLIENT,
				&data->guid,
				data->packet_type | PF_ACK,
				pngrp,
				sizeof(RTK_GROUP)
				);
		}
	}
}

static void _on_drop_groups(PCRTK_PACKET data)
{
	__uint		count, i;
	PNODE_KEY	nk;
	PGROUP_KEY	gk;

	count = data->data_size / (sizeof(*nk)  + sizeof(*gk));
	if( data->data_size % (sizeof(*nk)  + sizeof(*gk)) ){
		// invalid data packet
		return;
	}
	nk = (PNODE_KEY)data->data;
	gk = (PGROUP_KEY)(data->data + sizeof(*nk) * count);
	
	for(i=0; i<count; i++, nk++, gk++){
		if(!is_same_key(nk, &g_ThisNode->key)){
			continue;
		}
		RTK_CURSOR h;
		if(!lock_rtdb(__true, 5000)){
			continue;
		}
		h = open_group(HNODE_LOCAL_MACHINE, gk);
		if(h){
			cursor_delete(h);
		}
		unlock_rtdb();
		if(h){
			broadcast_rtk_data_with_guid(
				vbus_config,
				PORT_ALL_CLIENT,
				&data->guid,
				PT_DropGroup | PF_ACK,
				gk,
				sizeof(*gk)
				);
		}
	}
}

static void _on_d_devices(PCRTK_PACKET data)
{
	DEVICE_INFO *buf;
	__uint count;
	count = 0;
	io_get_devices(0, &count);
	buf = new DEVICE_INFO[count];
	if(!buf){
		return;
	}
	io_get_devices(buf, &count);
	send_rtk_data_with_guid(
		vbus_config,
		&data->src,
		&data->guid,
		data->packet_type | PF_ACK,
		buf,
		count*sizeof(buf[0])
		);
	delete buf;
}

static void _on_d_pictures(PCRTK_PACKET data)
{
#ifdef _WIN32
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	__uint count = 0;
	PICT_KEY buf[128];
	string sWildcard;

	ZeroMemory(buf, sizeof(buf));
	sWildcard = string(get_working_dir()) + "file/*.gdf";
	hFind = FindFirstFile(sWildcard.data(), &fd);
	if(INVALID_HANDLE_VALUE == hFind){
		answer_failed(data);
		return;
	}
	buf[count++] = CPictName(fd.cFileName);
	while(FindNextFile(hFind, &fd) && count<128){
		buf[count++] = CPictName(fd.cFileName);
	}
	if(hFind){
		FindClose(hFind);
	}
	send_rtk_data_with_guid(
		vbus_config,
		&data->src,
		&data->guid,
		data->packet_type | PF_ACK,
		buf,
		sizeof(*buf)*count
		);
#else
// support on linux platform is not yet implementated
#endif
};

static void _on_get_config_string(PCRTK_PACKET data)
{
	const char * s;
	const char * k;
	char buf[1024];
	s = data->data;
	k = s + strlen(s) + 1;
	GetPrivateProfileString(
		s, k, "", buf, sizeof(buf), 
		get_config_file()
		);
#if 0
	printf("%s.%s=%s\n", s, k, buf);
#endif
	send_rtk_data_with_guid(
		vbus_config,
		&data->src,
		&data->guid,
		data->packet_type | PF_ACK,
		buf,
		strlen(buf) + 1
		);
}

static void _on_set_config_string(PCRTK_PACKET data)
{
	const char * s;
	const char * k;
	const char * v;
	s = data->data;
	k = s + strlen(s) + 1;
	v = k + strlen(k) + 1;
	WritePrivateProfileString(s, k, v, get_config_file());
	send_rtk_data_with_guid(
		vbus_config,
		&data->src,
		&data->guid,
		data->packet_type | PF_ACK
		);
}

class CConfigString : public set<string>
{
public:
	CConfigString(const char * section, const char * key)
	{
		GetPrivateProfileString(
			section,
			key,
			"",
			buffer,
			sizeof(buffer),
			get_config_file()
			);
		// strupr(buffer);
		s = section;
		k = key;
	}
	void split()
	{
		clear();
		CArgs args;
		int i;
		parse_arg_ex(buffer, &args, ",");
		for(i=0; i<args.argc; i++){
			insert(args.argv[i]);
		}
	}
	void write()
	{
		char *p;
		size_t left, bytes;
		p = buffer;
		iterator it;
		left = sizeof(buffer);
		*buffer = 0;
		it = begin();
		while(it != end()){
			bytes = _snprintf(p, left, "%s,", (*it).data());
			p += bytes;
			left -= bytes;
			it++;
		}
		WritePrivateProfileString(s, k, buffer, get_config_file());
	}
	char buffer[2048];
	const char *s,*k;
};

static void _find_files(
	char * &buf, 
	__int & left, 
	const char * path, 
	const char * wildcards
	)
{
#ifdef _WIN32
	__uint bytes;
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	string sWildcard;

	if(!left){
		return;
	}	
	sWildcard = string(get_working_dir()) + path + "/" + wildcards;
	hFind = FindFirstFile(sWildcard.data(), &fd);
	if(INVALID_HANDLE_VALUE == hFind){
		return;
	}
	do{
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			continue;
		}
		bytes = _snprintf(buf, left, "%s", 
			(string(path) + "/" + fd.cFileName).data()
			) + 1;
		buf += bytes;
		left -= bytes;
	}while(FindNextFile(hFind, &fd) && left);
	FindClose(hFind);
#endif
}

static void _on_d_configfiles(PCRTK_PACKET data)
{
	char buf[1024*100];	
	char *p;
	__int left=sizeof(buf) - 1;
	__int bytes;
	ZeroMemory(buf, sizeof(buf));
	p = buf;
	_find_files(p, left, "config", "*.*");
	_find_files(p, left, "files", "*.*");	
	_find_files(p, left, "infs", "*.*");
	bytes = sizeof(buf) - left + 1;
	if(bytes < 0){
		answer_failed(data);
		return;
	}
	send_rtk_data_with_guid(
		vbus_config,
		&data->src,
		&data->guid,
		data->packet_type | PF_ACK,
		buf,
		bytes
		);
}

static void _on_d_files(PCRTK_PACKET data)
{
	char buf[1024*100];	
	const char *path, *w;
	char *p;
	__int left=sizeof(buf) - 1;
	__int bytes;
	if(data->data[data->data_size] != '\0'){
		answer_failed(data);
		return;
	}	
	path = data->data;
	w = path + strlen(path) + 1;
	if(strlen(path) >= data->data_size){
		answer_failed(data);
		return;
	}
	ZeroMemory(buf, sizeof(buf));
	p = buf;
	_find_files(p, left, path, w);
	bytes = sizeof(buf) - left + 1;
	if(bytes < 0){
		answer_failed(data);
		return;
	}
	send_rtk_data_with_guid(
		vbus_config,
		&data->src,
		&data->guid,
		data->packet_type | PF_ACK,
		buf,
		bytes
		);
}

/*
static PDEVICE_INFO _add_device_1(
	PDEVICE_KEY k, 
	PVENDOR_KEY v, 
	PDEVTYPE_KEY t,
	const char * addr,
	const char * init
	)
{
	PDRIVER_INFO pDriver;
	PDEVICE_INFO pDevice;

	pDriver = 0;
	// create the device
	pDevice = io_create_device(pDriver, k);
	if(!pDevice){		
		return 0;
	}
	string inf;
	inf = string(get_exe_dir()) + "infs/" 
		+ (char*)CVendorName(*v) + ".inf";
	pDevice->v.key = *v;
	GetPrivateProfileString(
		"oem",
		"vendor",
		"",
		pDevice->v.description,
		sizeof(pDevice->v.description),
		inf.data()
		);
	pDevice->k = *k;
	GetPrivateProfileString(
		(char*)CDevTypeName(*t),
		"description",
		"",
		pDevice->t.description,
		sizeof(pDevice->t.description),
		inf.data()
		);
	strncpy(pDevice->address, addr, sizeof(pDevice->address));
	pDevice->address[sizeof(pDevice->address) - 1] = 0;
	strncpy(pDevice->parameter, init, sizeof(pDevice->parameter));
	pDevice->parameter[sizeof(pDevice->parameter) - 1] = 0;

	return pDevice;
}
*/

static void _on_add_device(PCRTK_PACKET data)
{
	if(data->data_size < 
		sizeof(VENDOR_KEY) + sizeof(DEVTYPE_KEY) + sizeof(DEVICE_KEY) + 2
	){
		answer_failed(data);
		return;
	}
	PDEVICE_KEY k = (PDEVICE_KEY)data->data;
	PVENDOR_KEY v = (PVENDOR_KEY)((char*)k + sizeof(*k));
	PDEVTYPE_KEY t = (PDEVTYPE_KEY)((char*)v + sizeof(*v));
	char *addr, *init;
	addr = (char*)t + sizeof(*t);
	if(data->data[ data->data_size ] != '\0'){
		answer_failed(data);
		return;
	}
	init = addr + strlen(addr) + 1;
	if(init >= data->data + data->data_size){
		answer_failed(data);
		return;
	}
	PDEVICE_INFO pdev;
	pdev = io_open_device(k);
	if(pdev){
		answer_failed(data);
		return;
	}
	CConfigString cs("devices", "load");
	cs.split();
	cs.insert((char*)CDeviceName(*k));
	cs.write();
	string line;
	line = (char*)CVendorName(*v);
	line += ".";
	line += (char*)CDevTypeName(*t);
	line += ",";
	line += addr;
	line += ",";
	line += init;
	WritePrivateProfileString(
		"devices",
		(char*)CDeviceName(*k),
		line.data(),
		get_config_file()
		);
	// pdev = _add_device_1(k,v,t,addr,init);
	pdev = io_probe_device((char*)CDeviceName(*k), __true);
	if(!pdev){
		answer_failed(data);
	}else{
		broadcast_rtk_data_with_guid(
			vbus_config,
			PORT_ALL_CLIENT,
			&data->guid,
			data->packet_type | PF_ACK,
			pdev,
			sizeof(DEVICE_INFO)
			);
	}
}

static void _on_del_device(PCRTK_PACKET data)
{
	if(data->data_size != sizeof(DEVICE_KEY)){
		answer_failed(data);
		return;
	}

	PDEVICE_KEY k; 
	k = (PDEVICE_KEY)data->data;
	PDEVICE_INFO pdev;
	
	if(!lock_rtdb(__true, 1000)){
		answer_failed(data);
		return;
	}
	
	pdev = io_open_device(k);
	if(!pdev){
		unlock_rtdb();
		answer_failed(data);
		return;
	}
	
	pdev->flags |= DF_Deleted;
	CConfigString cs("devices", "load");
	cs.split();
	CConfigString::iterator it = cs.find((char*)CDeviceName(*k));
	if(it != cs.end()){
		cs.erase(it);
		cs.write();
	}
	if(!io_stop_device(pdev)){
		answer_failed(data);
	}
	if(!io_delete_device(pdev)){
		answer_failed(data);
	}

	unlock_rtdb();
	
	broadcast_rtk_data_with_guid(
		vbus_config,
		PORT_ALL_CLIENT,
		&data->guid,
		data->packet_type | PF_ACK,
		data->data,
		data->data_size
		);
}

static void _on_edit_device(PCRTK_PACKET data)
{
	if(data->data_size < sizeof(DEVICE_KEY) + 2){
		answer_failed(data);
		return;
	}
	PDEVICE_KEY k;
	char *addr, *init;
	k = (PDEVICE_KEY)data->data;
	addr = (char*)k + sizeof(*k);
	if(data->data[data->data_size] != '\0'){
		answer_failed(data);
		return;
	}
	init = addr + strlen(addr) + 1;
	if(init >= data->data + data->data_size){
		answer_failed(data);
		return;
	}

	if(!lock_rtdb(__true, 1000)){
		answer_failed(data);
		return;
	}
	
	PDEVICE_INFO pdev;
	pdev = io_open_device(k);
	if(!pdev){
		answer_failed(data);
		unlock_rtdb();
		return;
	}
	strncpy(pdev->address, addr, sizeof(pdev->address));
	pdev->address[sizeof(pdev->address) - 1] = 0;
	strncpy(pdev->parameter, init, sizeof(pdev->parameter));
	pdev->parameter[sizeof(pdev->parameter) - 1] = 0;
	string line;
	line = (char*)CVendorName(pdev->v.key);
	line += ".";
	line += (char*)CDevTypeName(pdev->t.key);
	line += ",";
	line += addr;
	line += ",";
	line += init;
	WritePrivateProfileString(
		"devices",
		(char*)CDeviceName(*k),
		line.data(),
		get_config_file()
		);
	/* restart device with new configuration */
	if(!io_stop_device(pdev)){
		answer_failed(data);
		unlock_rtdb();
		return;
	}
	if(!io_start_device(pdev)){
		answer_failed(data);
		unlock_rtdb();
		return;
	}
	unlock_rtdb();
	broadcast_rtk_data_with_guid(
		vbus_config,
		PORT_ALL_CLIENT,
		&data->guid,
		data->packet_type | PF_ACK,
		pdev,
		sizeof(DEVICE_INFO)
		);
}

void PMC_API on_config(const RTK_PACKET * data, HVBUS bus)
{
#undef	HANDLE_IT
#define HANDLE_IT(type, func)\
	case type:\
		func(data);\
		break

	if( get_power_state() == PWR_BACKUP ){
		on_backup_config(data);
		return;
	}

	switch(PACKET_TYPE(data)){
		HANDLE_IT(PT_DiscoverNodes, _on_d_nodes);
		HANDLE_IT(PT_DiscoverGroups, _on_d_groups);
		HANDLE_IT(PT_DiscoverTags,	_on_d_tags);
		HANDLE_IT(PT_DiscoverPictures,	_on_d_pictures);
		HANDLE_IT(PT_ReloadTags,	_on_reload_tags);
		HANDLE_IT(PT_AddTag, _on_add_edit_tags);
		HANDLE_IT(PT_EditTag,	_on_add_edit_tags);
		HANDLE_IT(PT_DropTag,	_on_drop_tags);
		HANDLE_IT(PT_AddGroup, _on_add_edit_groups);
		HANDLE_IT(PT_EditGroup, _on_add_edit_groups);
		HANDLE_IT(PT_DropGroup,_on_drop_groups);
		HANDLE_IT(PT_GetString,_on_get_config_string);
		HANDLE_IT(PT_SetString,_on_set_config_string);
		HANDLE_IT(PT_DiscoverDevices, _on_d_devices);
		HANDLE_IT(PT_DiscoverConfigFiles, _on_d_configfiles);
		HANDLE_IT(PT_AddDevice, _on_add_device);
		HANDLE_IT(PT_DelDevice, _on_del_device);
		HANDLE_IT(PT_EditDevice, _on_edit_device);
		HANDLE_IT(PT_DiscoverFiles, _on_d_files);
	}
}

void init_config()
{
}

void uninit_config()
{
}

