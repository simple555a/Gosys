#include "precomp.h"
#pragma hdrstop

/*
 *函数说明：给数据包中的节点对应的本地RTK_NODE的成员dogcount赋值，并根据数据包类型更新实时数据库的（动态）值
 */
void PMC_API _on_rtdb(PCRTK_PACKET packet, HVBUS bus)
{
	// check for unknown node
	TAG_NAME	tn;
	PRTK_NODE pNode;

#if 0
	if(get_power_state() != PWR_RUNNING){         //libpowermgr.dll
		return;
	}
#endif

	if(g_Worker){      //g_Worker全局变量
		if(lock_rtdb(__false, 100)){
			RTK_CURSOR hNode;
			host_to_node(&packet->src.host, &tn.node);
			hNode = open_node(&tn.node);           //打开节点游标
			pNode = (PRTK_NODE)cursor_get_item(hNode);//利用节点游标找到存储该节点的RTK_NODE
			if(pNode){
				pNode->dogcount = g_NodeLife;            //给RTK_NODE的成员赋值
			}
			close_handle(hNode);
			unlock_rtdb();
			if(!hNode){                   //若打开游标失败，则将该节点的标签插入CWaitingList,组名和标签名都设为0，然后插入CWaitingList
				memset(&tn.sname, 0, sizeof(tn.sname));
				// unknown node
				g_Worker->inform_load(&tn, 1);         
			}			
			if(!hNode){
				return;
			}
		}
	}
    //根据数据包类型，更新数据
	switch(PACKET_TYPE(packet)){
	case PT_ChangedData:
		update_changed_data(packet, __false); //更新实时数据库(服务器端或代理)数据，在CWaitingList中记录丢失的点（数据包中有，而实时数据库没有的点）
		break;
	case PT_QueryData:
		_on_queried_data(packet);
		break;
	}
}

/*
 *函数功能：根据数据包的类型，执行相应的配置操作，并记录在_EVENT中
 */
void PMC_API _on_config(PCRTK_PACKET packet, HVBUS bus)  //bus没用上？？？？
{
#if 0
	if(get_power_state() != PWR_RUNNING){
		return;
	}
#endif

#define HANDLE_IT(type, func)\
	case type:\
		func(packet);\
		break
	switch(packet->packet_type){
	case PT_AddTag | PF_ACK:
	case PT_EditTag | PF_ACK:
		_on_add_edit_tags(packet, true);
		break;
	case PT_DropTag | PF_ACK:
		_on_drop_tags(packet);
		break;
	case PT_AddGroup | PF_ACK:
	case PT_EditGroup | PF_ACK:
		_on_add_edit_groups(packet, true);
		break;
	case PT_DropGroup | PF_ACK:
		_on_drop_groups(packet);
		break;
	HANDLE_IT(PT_AddDevice, _on_add_device);
	HANDLE_IT(PT_EditDevice, _on_edit_device);
	HANDLE_IT(PT_DelDevice, _on_del_device);
	HANDLE_IT(PT_DiscoverTags | PF_ACK, _on_add_edit_tags);
	}
}

/*
 *函数功能：在线处理报警数据包
 */
void PMC_API _on_alarm(PCRTK_PACKET packet, HVBUS bus)
{
	TAG_NAME	tn;
	RTK_ALARM_PACKET *p;
	//RTK_CURSOR crNode;

#if 0
	if(get_power_state() != PWR_RUNNING){
		return;
	}
#endif

	switch(packet->packet_type){
	case PT_Alarm:
		p = (RTK_ALARM_PACKET*)packet->data;
		if(p->Class == AC_Online){                           //若数据包类型为AC_Online，则记下数据包的节点名，并存入CWaitingList中
			host_to_node(&packet->src.host, &tn.node);
			DEBUG_PRINTF((
				"Got online broadcast from %s.\n",
				(char*)CNodeName(tn.node)
				));
			if(g_Worker){
				memset(&tn.sname, 0, sizeof(tn.sname));
				g_Worker->inform_load(&tn, 1);
			}
		}else if(p->Class == AC_Offline){                  //若数据包类型为AC_Offline，则无作为
			/*
			host_to_node(&packet->src.host, &tn.node);
			DEBUG_PRINTF((
				"Got offline broadcast from %s.\n",
				(char*)CNodeName(tn.node)
				));			
			if(lock_rtdb(__true, 500)){
				crNode = open_node(&tn.node);
				if(crNode){
					cursor_delete(crNode);
				}
				unlock_rtdb();
			}
			*/
		}
		rtk_queue_event(            //给_EVENT的各成员赋值
			PT_Alarm, 
			p->Rank, 
			p->Class, 
			p->Msg, 
			&packet->src
			);
		break;
	}
}

/*
	_on_system()
*/
/*
 *函数功能：根据配置文件pmc.ini的SystemBoost的值，设置当前线程的优先级
 */
void PMC_API _on_system(PCRTK_PACKET packet, HVBUS bus)
{
#if 0
	if(get_power_state() != PWR_RUNNING){
		return;
	}
#endif
	{
		static __bool priorityBoosted = __false;
		if(!priorityBoosted){
			DWORD boost;
			boost = GetPrivateProfileInt(      //从配置文件pmc.ini中取boost
				"vbus", 
				"SystemBoost", 
				THREAD_PRIORITY_ABOVE_NORMAL,
				get_config_file()
				);
			SetThreadPriority(GetCurrentThread(), boost); //设置当前线程的优先级
			priorityBoosted = __true;
		}
	}

	if(packet->packet_type != PT_ServerClaim){
		return;
	}
	if(!lock_rtdb(__false, 1000)){
		return;
	}
	RTK_CURSOR	hNode;
	PRTK_NODE	pNode;
	TAG_NAME	tn;
	host_to_node(&packet->src.host, &tn.node);
	hNode = open_node(&tn.node);                          
	pNode = (PRTK_NODE)cursor_get_item(hNode);
	if(pNode){
		pNode->dogcount = g_NodeLife;              //更新RTK_NODE的成员dogcount
	}
	close_handle(hNode);
	unlock_rtdb();
	if(!pNode && g_Worker){                     //
		memset(&tn.sname, 0, sizeof(tn.sname));
		g_Worker->inform_load(&tn, 1);
	}
}