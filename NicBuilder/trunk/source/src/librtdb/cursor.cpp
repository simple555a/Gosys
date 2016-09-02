/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\librtdb\cursor.cpp
	file path:	E:\vss\pmc2\src\librtdb
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Cursor support	
	history:
	created:	10:9:2002   15:10
*********************************************************************/
#include "precomp.h"

RTDB_API __bool PMC_API cursor_move_next(RTK_CURSOR cursor)
{
	if(!cursor){
		return __false;
	}
	if(is_htag(cursor)){
		(*((_HTAG*)cursor))++;
		return ((_HTAG*)cursor)->is_end();
	}else if(is_hgroup(cursor)){
		(*((_HGROUP*)cursor))++;
		return ((_HGROUP*)cursor)->is_end();
	}else{
		(*((_HNODE*)cursor))++;
		return ((_HNODE*)cursor)->is_end();
	}
}

RTDB_API __bool PMC_API cursor_is_end(RTK_CURSOR cursor)
{
	if(!cursor){
		return __true;
	}
	if(is_htag(cursor)){
		return ((_HTAG*)cursor)->is_end();
	}else if(is_hgroup(cursor)){
		return ((_HGROUP*)cursor)->is_end();
	}else{
		return ((_HNODE*)cursor)->is_end();
	}
}

RTDB_API __bool PMC_API cursor_move_end(RTK_CURSOR cursor)
{
	if(!cursor){
		return __false;
	}
	if(is_htag(cursor)){
		return ((_HTAG*)cursor)->move_end();
	}else if(is_hgroup(cursor)){
		return ((_HGROUP*)cursor)->move_end();
	}else{
		return ((_HNODE*)cursor)->move_end();
	}
}

RTDB_API void * PMC_API cursor_get_item(RTK_CURSOR cursor)
{
	if(!cursor){
		return 0;
	}
	if(is_htag(cursor)){
		return (RTK_CURSOR)(*(_HTAG*)cursor);
	}else if(is_hgroup(cursor)){
		return (RTK_CURSOR)(*(_HGROUP*)cursor);
	}else{
		return (RTK_CURSOR)(*(_HNODE*)cursor);
	}
}

RTDB_API RTK_CURSOR PMC_API cursor_open_first_subitem(RTK_CURSOR cursor)
{
	/*
	DEBUG_PRINTF((
		"open_first, type=0x%08x\n", 
		((_HTAG*)cursor)->cursor_type
		));
	*/
	if(!cursor){
		return __false;
	}
	if(is_hnode(cursor)){
		RTK_NODE *nd;
		nd = (RTK_NODE*)cursor_get_item(cursor);
		if(!nd){
			return 0;
		}
		return (RTK_CURSOR)_HGROUP::first_item((GROUP_TABLE*)nd->itemtable);
	}else if(is_hgroup(cursor)){
		RTK_GROUP *grp;
		grp = (RTK_GROUP*)cursor_get_item(cursor);
		if(!grp){
			return 0;
		}
		return (RTK_CURSOR)_HTAG::first_item((TAG_TABLE*)grp->itemtable);
	}
	/*
	else if(is_htag(cursor)){
		assert(0);
	}
	*/
	return 0;
}

/*
	memory leak!!!!!!!!!!!!!
	2002/10/18
	seems corrected, 
	2002/11/1

	2003/3/31
	not memory leak, but wild pointer, a potential bug. see comments below.
*/
RTDB_API __bool PMC_API cursor_delete(RTK_CURSOR cursor)
{
	RTK_CURSOR sub_item;

	if(!cursor){
		return __false;
	}
	/*
	DEBUG_PRINTF((
		"delete cursor, type=0x%08x\n", 
		((_HTAG*)cursor)->cursor_type
		));
	*/
	sub_item = cursor_open_first_subitem(cursor);
	while(sub_item){
		cursor_delete(sub_item);
		sub_item = cursor_open_first_subitem(cursor);
	}
	
	if(is_htag(cursor)){
		_HTAG *tag;
		tag = (_HTAG*)cursor;
		FIRE_EVENT(OnDropTag, ((PRTK_TAG)cursor_get_item(cursor)));
		tag->erase();
		g_dwTags--;
	}else if(is_hgroup(cursor)){
		_HGROUP *item;
		item = (_HGROUP*)cursor;
		RTK_GROUP * grp_item = (RTK_GROUP *)cursor_get_item(cursor);
		if(grp_item){
			FIRE_EVENT(OnDropGroup, (grp_item));
			delete (TAG_TABLE*)grp_item->itemtable;
		}
		item->erase();
	}else if(is_hnode(cursor)){
		_HNODE *node;
		node = (_HNODE*)cursor;
		RTK_NODE * nod_item = (RTK_NODE *)cursor_get_item(cursor);
		if(nod_item){
			FIRE_EVENT(OnDropNode, (nod_item));
			delete (GROUP_TABLE*)nod_item->itemtable;
		}
		node->erase();
	}

	// 2003/3/31
	// reaching here, the pointer contained in cursor is not valid
	// but codes in the cursor's destructor has some reference counting
	// which is performed on the already freeed memory, thus not
	// safe. reference counting has been commented out.
	close_handle(cursor);
	
	return __true;
}

RTDB_API __uint	PMC_API cursor_get_subitem_count(RTK_CURSOR cursor)
{
	void * item = cursor_get_item(cursor);
	if(!item){
		return 0;
	}
	if(is_hgroup(cursor)){
		TAG_TABLE * table;
		table = (TAG_TABLE*)((RTK_GROUP*)item)->itemtable;
		if(!table){
			return 0;
		}
		return table->size();
	}else if(is_hnode(cursor)){
		GROUP_TABLE * table;
		table = (GROUP_TABLE*)((RTK_NODE*)item)->itemtable;
		if(!table){
			return 0;
		}
		return table->size();
	}else if(is_htag(cursor)){
		return 0;
	}
	return 0;
}

RTDB_API __bool PMC_API cursor_clear_subitems(RTK_CURSOR cursor)
{
	void * item = cursor_get_item(cursor);
	if(!item){
		return __false;
	}
	if(is_hgroup(cursor)){
		TAG_TABLE * table;
		table = (TAG_TABLE*)((RTK_GROUP*)item)->itemtable;
		if(!table){
			return __false;
		}
		table->clear();
		return __true;
	}else if(is_hnode(cursor)){
		GROUP_TABLE * table;
		table = (GROUP_TABLE*)((RTK_NODE*)item)->itemtable;
		if(!table){
			return __false;
		}
		table->clear();
		return __true;
	}else if(is_htag(cursor)){
		return __false;
	}
	return __false;
}

