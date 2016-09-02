#include "precomp.h"

PRTDB_DISPATCH_TABLE	g_Handlers[MAX_HANDLERS];
__int					g_iHandlers;

/*
函数功能：将实时库分派结构添加到分派结构数组g_Handlers中
参数说明：待添加的实时库分派结构
返 回 值：0  数组g_Handlers已满，添加失败
          1  函数执行成功
*/
RTDB_API __bool PMC_API hook_rtdb_events(PRTDB_DISPATCH_TABLE table)
{
	int i;
    
	//1 
	if(g_iHandlers >= MAX_HANDLERS || !table){
		return __false;
	}
    
	//2 
	for(i=0; i<g_iHandlers; i++){
		if(g_Handlers[i] == table){
			// cannot hook another
			return __true;
		}
	}
	
	g_Handlers[g_iHandlers] = table;
	g_iHandlers++;
	
	return __true;
}

/*
函数功能：删除实时库分派结构数组中指定的分派结构
参数说明：
          table 待删除的实时库分派结构
返 回 值：0 __false       分派结构数组已空
                     或   待删除的分派结构为空
					 或   在分派结构数组中没有找到待删除的分派结构
*/
RTDB_API __bool PMC_API unhook_rtdb_events(PRTDB_DISPATCH_TABLE table)
{
	__int i;
	if(!g_iHandlers || !table){
		return __false;
	}
	for(i=0; i<g_iHandlers; i++){
		if(g_Handlers[i] == table){
			break;
		}
	}
	if(i>=g_iHandlers){
		return __false;
	}
	for(; i<g_iHandlers - 1; i++){
		g_Handlers[i] = g_Handlers[i+1];
	}
	g_iHandlers--;
	return __true;
}

RTDB_API void PMC_API fire_rtdb_event(RTDB_EVENT id, void * p1, void *)
{
	switch(id){
	case EV_AddTag:
		FIRE_EVENT(OnAddTag, ((PRTK_TAG)p1));
		break;
	case EV_DropTag:
		FIRE_EVENT(OnDropTag, ((PRTK_TAG)p1));
		break;
	case EV_ModifyTag:
		FIRE_EVENT(OnModifyTag, ((PRTK_TAG)p1));
		break;
	case EV_AddGroup:
		FIRE_EVENT(OnAddGroup, ((PRTK_GROUP)p1));
		break;
	case EV_DropGroup:
		FIRE_EVENT(OnDropGroup, ((PRTK_GROUP)p1));
		break;
	case EV_ModifyGroup:
		FIRE_EVENT(OnModifyGroup, ((PRTK_GROUP)p1));
		break;
	case EV_AddNode:
		FIRE_EVENT(OnAddNode, ((PRTK_NODE)p1));
		break;
	case EV_DropNode:
		FIRE_EVENT(OnDropNode, ((PRTK_NODE)p1));
		break;
	default:
		break;
	}
}

