#include "precomp.h"

static __uint _on_add_edit_groups(PCRTK_PACKET p)
{
	RTK_CURSOR hNode, hGroup;
	PRTK_GROUP png, pg=0;
	__bool		bEdit;
	__uint count;

	if(p->data_size % sizeof(RTK_GROUP)){
		return 0;
	}
	if( !lock_rtdb(__true, 100) ){
		return 0;
	}
	hNode = HNODE_LOCAL_MACHINE;
	
	count = p->data_size / sizeof(RTK_GROUP);
	png = (PRTK_GROUP)p->data;
	bEdit = PACKET_TYPE(p) == PT_EditGroup? __true : __false;
	for(__uint i=0; i<count; i++, png++){
		hGroup = open_group(hNode, &png->key);
		pg = (PRTK_GROUP)cursor_get_item(hGroup);
		if(pg){
			memcpy(
				pg->description, 
				png->description, 
				sizeof(pg->description)
				);
			pg->period = png->period;
			fire_rtdb_event(EV_ModifyGroup, pg);
		}else{
			hGroup = create_group(hNode, png);
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
	}
	unlock_rtdb();

	return count;
}

static __uint _on_add_edit_tags(PCRTK_PACKET p)
{
	ONLINE_CONFIG::reload_tags_ack *ack;
	RTK_CURSOR	hNode, hTag, hGroup;
	RTK_TAG		*pTag;
	RTK_GROUP	grp;
	__bool		bEdit;
	
	__uint tagcount, i;

	ack = (ONLINE_CONFIG::reload_tags_ack *)p->data;
	tagcount = p->data_size / sizeof(*ack);
	if(p->data_size % sizeof(*ack)){
		return 0;
	}
	ZeroMemory(&grp, sizeof(grp));

	if( !lock_rtdb(__true, 100) ){
		return 0;
	}

	hNode = HNODE_LOCAL_MACHINE;
	hTag = hGroup = 0;
	host_to_node(&g_ThisNode->key, &grp.node);

	if(PACKET_TYPE(p) == PT_EditTag){
		bEdit = __true;
	}else{
		bEdit = __false;
	}

	for(i=0; i<tagcount; i++){
		hGroup = open_group(hNode, &ack[i].tag.group);
		grp.key = ack[i].tag.group;
		if(!hGroup){
			hGroup = create_group(hNode, &grp);
		}
		if(hGroup){
			hTag = create_tag(hGroup, &ack[i].tag.key, &ack[i].tag.s);
			pTag  = (RTK_TAG*)cursor_get_item(hTag);
			if(pTag){
				*pTag = ack[i].tag;
				mark_expired(pTag);
				close_handle(hTag);
				if(bEdit){
					// add-tag event is auto-fired
					fire_rtdb_event(EV_ModifyTag, pTag);
				}				
			}					
		}
		close_handle(hGroup);
	}

	unlock_rtdb();

	return tagcount;
}

static __uint PMC_API _on_drop_groups(PCRTK_PACKET packet)
{
	__uint count, i, ret=0;
	RTK_CURSOR hNode, hGroup;
	PGROUP_KEY pgk;
	TAG_NAME name;

	count = packet->data_size / sizeof(GROUP_KEY);
	if(packet->data_size % sizeof(GROUP_KEY)){
		return 0;
	}
	if( !lock_rtdb(true, 100) ){
		return 0;
	}
	hNode = HNODE_LOCAL_MACHINE;

	for(i=0, pgk=(PGROUP_KEY)packet->data; i<count; i++, pgk++){
		name.sname.group = *pgk;
		hGroup = open_group(hNode, pgk);
		if(hGroup){
			cursor_delete(hGroup);
			ret++;
		}
	}

	unlock_rtdb();
	
	return ret;
}

static void PMC_API _on_drop_tags(PCRTK_PACKET packet)
{
	__uint		count, i;
	PTAG_NAME	name;
	RTK_CURSOR	hNode;

	count = packet->data_size / sizeof(TAG_NAME);
	if(packet->data_size % sizeof(TAG_NAME)){
		return;
	}
	if( !lock_rtdb(true, 100) ){
		return;
	}
	hNode = HNODE_LOCAL_MACHINE;
	for(i=0, name=(PTAG_NAME)packet->data; i<count; i++, name++){
		RTK_CURSOR hTag;
		hTag = open_tag(hNode, &name->sname);
		if(hTag){
			cursor_delete(hTag);
		}
	}

	unlock_rtdb();
}

static void _on_add_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_AddDevice, &p->src, p->data);         //librtkproxy.dll  dispatch.cpp
}

void _on_edit_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_EditDevice, &p->src, p->data);        //librtkproxy.dll  dispatch.cpp
}

void _on_del_device(PCRTK_PACKET p)
{
	if(p->packet_type & PF_Failed){
		return;
	}
	rtk_queue_event(PT_DelDevice, &p->src, p->data);         //librtkproxy.dll  dispatch.cpp
}

/*
	in backup mode, the daemon monitors data packets sent by
	the primary site and acts accordingly.
*/
void PMC_API on_backup_config(PCRTK_PACKET data)
{
#define HANDLE_IT(type, func)\
	case type | PF_ACK:\
		func(data);\
		break
	
	if(data->packet_type & PF_Failed){
		return;
	}
     //librtkproxy.dll rtkproxy.cpp
	switch(data->packet_type){
		HANDLE_IT(PT_AddTag, _on_add_edit_tags);
		HANDLE_IT(PT_EditTag, _on_add_edit_tags);
		HANDLE_IT(PT_DropTag, _on_drop_tags);
		HANDLE_IT(PT_AddGroup, _on_add_edit_groups);
		HANDLE_IT(PT_EditGroup, _on_add_edit_groups);
		HANDLE_IT(PT_DropGroup, _on_drop_groups);
		HANDLE_IT(PT_AddDevice, _on_add_device);
		HANDLE_IT(PT_EditDevice, _on_edit_device);
		HANDLE_IT(PT_DelDevice, _on_del_device);
	}
}

/*
	when a PMC daemon is started, it first checks if there's
	already a daemon running on the network with the same nodekey,
	if yes, the newly started daemon automaticly runs in backup 
	mode until demanded to switch to primary mode.
*/
bool setup_running_state()
{
	__uint items;
	NODE_KEY	nodeKey;
	char		buffer[8192];
	string	dirname;

	*buffer = 0;
	
	/*
		running-state check must be done in backup mode.
	*/
	set_power_state(PWR_BACKUP);

	host_to_node(&g_ThisNode->key, &nodeKey);
	utils_trace("Finding primary site....\n");
	items = discover_configfiles(&nodeKey, buffer, sizeof(buffer), 2000);      //librtkproxy.dll     discover.cpp
	if(!items){
		utils_trace("No primary site found, starting as primary.\n");
		set_power_state(PWR_RUNNING);
	}else{
		if(!g_bAllowBackup){
			utils_error(
				"Primary site unexpected, "
				"use '-2' command line option to force start as backup.\n"
				);
			return __false;
		}
		utils_trace("Primary site found, server will be started in backup mode.\n");
#if 0
		__uint	 i, filesize;
		char		*item, *filebuf;
		FILE		*fp;
		utils_trace("Primary site found, downloading configurations....\n");
		item = buffer;
		dirname = get_working_dir();
		for(i=0; i<items && *item; i++){
			utils_trace("Downloading file (%d of %d) %s....\n", i+1, items, item);
			if(!download_file(&nodeKey, item, &filebuf, &filesize)){       //librtkproxy.dll   ftp.cpp
				utils_error("Unable to download  '%s'\n", item);
				return false;
			}
			fp = fopen((dirname + "/" + item).data(), "wb");
			if(!fp){
				utils_error("Unable to write to '%s'\n", item);
				proxy_release_data(filebuf);                         //librtkproxy.dll   ftp.cpp
				return false;
			}
			fwrite(filebuf, 1, filesize, fp);
			fclose(fp);
			proxy_release_data(filebuf);                     //librtkproxy.dll   ftp.cpp
			item += strlen(item) + 1;
			rtk_sleep(100);
		}
#endif
	}
	
	return true;
}

__bool switch_to_primary()
{
	UTILS_TIME	t1, t2;
	__bool	ret;

	time_mark(&t1);
	utils_trace(">> %s : switching to primary mode ....\n", _texttime().data());
	ret = set_power_state(PWR_RUNNING);
	time_mark(&t2);

	utils_trace(
		">> Mode switch %s , time elapsed is %.3f seconds.\n", 
		ret? "OK" : "FAILED.", 
		time_diff(&t2, &t1)
		);

	return ret;
}

__bool switch_to_backup()
{
	UTILS_TIME	t1, t2;
	__bool	ret;

	time_mark(&t1);
	utils_trace(">> %s : switching to backup mode ....\n", _texttime().data());
	ret = set_power_state(PWR_BACKUP);
	time_mark(&t2);

	utils_trace(
		">> Mode switch %s , time elapsed is %.3f seconds.\n", 
		ret? "OK" : "FAILED.", 
		time_diff(&t2, &t1)
		);

	return ret;
}

class CBringupBackups : public CWaitableTransaction
{
public:
	CBringupBackups(__uint timeout)
	{
		RTK_ADDR dest;
		char	* msg = "switch primary";

		dest.host = g_ThisNode->key;
		dest.port = PORT_ALL_SLAVE;
		m_bOk = false;
		m_bTimeout = true;
		add_transaction(vbus_system, this);
		send_rtk_data_with_guid(
			vbus_system,
			&dest,
			&Guid,
			PT_System,
			msg,
			strlen(msg) + 1
			);
		Wait(timeout);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		m_bTimeout = false;
		if(packet->packet_type & PF_Failed){
			SetEvent();
			return;
		}
		m_bOk = true;
		SetEvent();
	}
	bool m_bOk;
	bool m_bTimeout;
};

bool bring_up_backups()
{
	CBringupBackups * tr;
	
	/*
		mode probing must be done in backup mode
	*/
	assert(PWR_BACKUP == get_power_state());
	
	utils_trace("Bringing up backup sites ...\n");
	tr = new CBringupBackups(2000);
	if(tr->m_bOk){
		utils_trace("Backup site acknowledged, no off-line broadcast will be made.\n");
		return true;
	}else if(tr->m_bTimeout){
		utils_trace("Backup site not found, continue unloading.\n");
	}else{
		utils_trace("Backup site mode-switch failed, continue unloading.\n");
	}
	return false;
}

bool notify_active()
{
	return true;
}

