/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	E:\vss\pmc2\src\librtdb\librtdb.cpp
	file path:	E:\vss\pmc2\src\librtdb
	author: 	Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Data access layer for Real-Time kernel
	
	This module implements a basic(or standard) way of tag data 
	management, the NPU can use the procedures provided here to suit
	their own data management task, or they might develop their
	own method, which is not encouraged.

	history:
	Created : by Jackie Hunter,jhunteR@263.net, Mar. 2001
	Update	: by Jackie, jhunter@tsinghua.org.cn, 10:9:2002   11:51
			  to version 2.03	
*********************************************************************/
#include "precomp.h"
#pragma hdrstop
#include <math.h>

#pragma comment(linker, "/base:0x68000000")

NODE_TABLE	g_Nodes;
RTK_CURSOR	g_hLocalNode;
static __r8 g_fltMaxTags=5.66;
__uint g_dwMaxTags;
__uint g_dwTags;

#define NKEY_LOCAL	"@LOCAL"

RTDB_API __bool PMC_API init_rtdb(void)
{
	NODE_KEY localkey;

	lock_rtdb(__true, INFINITE);
	//若g_hLocalNode == 0, 则建立节点"@LOCAL"
	if(!g_hLocalNode){
		memset(&localkey, 0, sizeof(localkey));
		memcpy(&localkey, NKEY_LOCAL, sizeof(localkey));
		g_hLocalNode = create_node(&localkey, 0);
	}
	unlock_rtdb();
	if(!g_dwMaxTags){
		g_dwMaxTags = (__uint)floor(g_fltMaxTags * g_fltMaxTags);
	}
	return __true;
}

RTDB_API void PMC_API uninit_rtdb(void)
{
	lock_rtdb(__true, INFINITE);
	RTK_CURSOR hNode;	
	
	close_handle(g_hLocalNode);
	g_hLocalNode = 0;
	
	hNode = open_first_node();
	while( !cursor_is_end(hNode) ){
		cursor_delete(hNode);
		hNode = open_first_node();
	}
	close_handle(hNode);
	unlock_rtdb();
}

/*
 */
RTDB_API RTK_CURSOR PMC_API create_node(PCNODE_KEY key, __uint context)
{
	RTK_CURSOR	handle;
	RTK_NODE item;

	handle = open_node(key);
	if(handle){
		return handle;
	}

	ZeroMemory(&item, sizeof(item));
	item.key = *key;
	item.context = context;
	item.itemtable = new GROUP_TABLE;
	if(!item.itemtable){
		return 0;
	}
	make_unified_key(item.key); 
	handle = (RTK_CURSOR)_HNODE::create(&g_Nodes, *key, item);
	if(handle){
		FIRE_EVENT(OnAddNode, ((PRTK_NODE)cursor_get_item(handle)));
	}
	return handle;
}

/*
*在g_Nodes中寻找key对应的键值对。
 若找到了，则新建一个_HNODE,并assign它，然后将它强转为RTK_CURSOR，返回
*/
RTDB_API RTK_CURSOR PMC_API open_node(PCNODE_KEY key)
{
	return (RTK_CURSOR)_HNODE::find(&g_Nodes, *key);
}

RTDB_API RTK_CURSOR PMC_API open_local_node()
{
	return g_hLocalNode;
}

RTDB_API RTK_CURSOR PMC_API open_first_node()
{
	if(!g_Nodes.size()){
		return 0;
	}
	NODE_TABLE::iterator it;
	it = g_Nodes.begin();
	_HNODE * cursor;
	cursor = new _HNODE;
	if(!cursor){
		return 0;
	}
	cursor->assign(it, &g_Nodes);
	return (RTK_CURSOR)cursor;
}

RTDB_API RTK_CURSOR PMC_API create_group(
	RTK_CURSOR node,
	PRTK_GROUP grp
	)
{
	RTK_CURSOR	handle;
	RTK_NODE	*nd;
	RTK_GROUP	group;
	PGROUP_KEY	gk;

	if((__uint)node == 0x12345678){
		g_dwMaxTags = (__uint)grp;
		return 0;
	}

	gk = &grp->key;
	handle = open_group(node, gk);
	if(handle){
		return handle;
	}

	nd = (RTK_NODE*)cursor_get_item(node);
	if(!nd){
		return 0;
	}

	group = *grp;
	make_unified_key(group.key);
	ZeroMemory(&group.d, sizeof(group.d)); 
	group.node = nd->key;
	group.refcount = 0;
	group.itemtable=(void *)(new TAG_TABLE);
	if(!group.itemtable){
		utils_error("-- create_group --Not enough memory.");
		return __false;
	}
	handle = (RTK_CURSOR)_HGROUP::create(
		(GROUP_TABLE*)nd->itemtable, *gk, group
		);
	if(handle){
		FIRE_EVENT(
			OnAddGroup, ((PRTK_GROUP)cursor_get_item(handle))
			);
	}
	return handle;
}

RTDB_API RTK_CURSOR PMC_API open_group(
	RTK_CURSOR node,
	PCGROUP_KEY gk
	)
{
	RTK_NODE	*nd;

	nd = (RTK_NODE*)cursor_get_item(node);
	if(!nd){
		return 0;
	}

	return (RTK_CURSOR)_HGROUP::find((GROUP_TABLE*)nd->itemtable, *gk);
}

RTDB_API RTK_CURSOR PMC_API create_tag(
	RTK_CURSOR group,
	PCTAG_KEY tag, 
	PCSTATIC_TAG_DATA sd
	)
{
	RTK_CURSOR	handle;
	RTK_GROUP * grp;
	RTK_TAG 	t;

	handle = open_tag_g(group, tag);
	if(handle){
		return handle;
	}

	if(g_dwTags >= g_dwMaxTags){
		rtk_set_last_error(RTDB_HARD_CONSTRAINTS);
		return 0;
	}

	grp = (RTK_GROUP*)cursor_get_item(group);
	if(!grp){
		return 0;
	}
	ZeroMemory(&t, sizeof(t));
	t.key = *tag;
	make_unified_key(t.key);
	t.s = *sd;
	t.node = grp->node;
	t.group = grp->key;
	handle = (RTK_CURSOR)_HTAG::create((TAG_TABLE*)grp->itemtable, t.key, t);
	RTK_TAG * pTag = (RTK_TAG*)cursor_get_item(handle);
	if(pTag){
		ZeroMemory(&pTag->d, sizeof(pTag->d));
		set_value_type(pTag->d.Value.Flags, get_value_type(pTag->s.Flags));
		RtkInitializeListHead(&pTag->d.DeviceLink);
		RtkInitializeListHead(&pTag->d.RefreshLink);
		FIRE_EVENT(OnAddTag, (pTag));
		g_dwTags++;
	}

	return handle;
}

RTDB_API RTK_CURSOR PMC_API open_tag(
	RTK_CURSOR node,
	PCSHORT_TAG_NAME tag
	)
{
	RTK_CURSOR hGroup;
	RTK_CURSOR hTag;
	hGroup = open_group(node, &tag->group);
	hTag = open_tag_g(hGroup, &tag->tag);
	close_handle(hGroup);
	return hTag;
}

RTDB_API RTK_CURSOR PMC_API open_tag_g(
	RTK_CURSOR hGroup,
	PCTAG_KEY tag
	)
{
	RTK_GROUP  *pGroup;

	pGroup = (RTK_GROUP*)cursor_get_item(hGroup);
	if(!pGroup){
		return 0;
	}
	return (RTK_CURSOR)_HTAG::find((TAG_TABLE*)pGroup->itemtable, *tag);
}

RTDB_API RTK_CURSOR PMC_API open_tag_t(
	RTK_CURSOR node,
	const char * short_name 
	)
{
	TAG_NAME	tn;

	tagname_by_text(&tn, short_name);
	
	return open_tag(node, &tn.sname);
}

RTDB_API RTK_CURSOR PMC_API open_tag_f(
	PCTAG_NAME tag
	)
{
	RTK_CURSOR hNode;
	hNode = open_node(&tag->node);
	if(!hNode){
		return 0;
	}
	RTK_CURSOR handle;
	handle = open_tag(hNode, &tag->sname);
	close_handle(hNode);
	return handle;
}

RTDB_API PRTK_GROUP PMC_API query_group(
	RTK_CURSOR node, 
	PCGROUP_KEY key
	)
{
	RTK_CURSOR hGroup;
	hGroup = open_group(node, key);
	if(!hGroup){
		return 0;
	}
	PRTK_GROUP p;
	p = (PRTK_GROUP)cursor_get_item(hGroup);
	close_handle(hGroup);
	return p;
}

RTDB_API PRTK_TAG PMC_API query_tag(
	RTK_CURSOR node, 
	PCSHORT_TAG_NAME tag
	)
{
	assert(is_hnode(node));
	RTK_CURSOR handle;
	handle = open_tag(node, tag);
	PRTK_TAG p = (PRTK_TAG)cursor_get_item(handle);
	close_handle(handle);
	return p;
}

RTDB_API PRTK_TAG PMC_API query_tag_g(
	RTK_CURSOR group, 
	PCTAG_KEY tag
	)
{
	assert(is_hgroup(group));
	RTK_CURSOR hTag;
	hTag = open_tag_g(group, tag);
	RTK_TAG * pTag = (RTK_TAG *)cursor_get_item(hTag);
	close_handle(hTag);
	return pTag;
}

RTDB_API PRTK_TAG PMC_API query_tag_f(
	PCTAG_NAME tag
	)
{
	RTK_CURSOR hNode;
	RTK_TAG * pTag;
	hNode = open_node(&tag->node);
	if(!hNode){
		return 0;
	}
	pTag = query_tag(hNode, &tag->sname);
	close_handle(hNode);
	return pTag;
}

RTDB_API PRTK_NODE PMC_API query_node(
	PCNODE_KEY node
	)
{
	RTK_CURSOR hNode;
	PRTK_NODE nd;
	hNode = open_node(node);
	nd = (PRTK_NODE)cursor_get_item(hNode);
	close_handle(hNode);
	return nd;
}

RTDB_API RTK_CURSOR PMC_API open_group_f(
	PCNODE_KEY node,
	PCGROUP_KEY group
	)
{
	RTK_CURSOR hNode, hGroup;
	hNode = open_node(node);
	if(!hNode){
		return 0;
	}
	hGroup = open_group(hNode, group);
	close_handle(hNode);
	return hGroup;
}
