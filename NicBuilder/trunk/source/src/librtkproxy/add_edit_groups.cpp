/********************************************************************
	PC-Based monitoring & controlling system source
	product version : 2.03
	filename: 	e:\vss\pmc2\src\rtkproxy\add_groups.cpp
	file path:	e:\vss\pmc2\src\rtkproxy
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Add groups to running DCS	
	history:
	created:	11/8/2002   17:19

    在线组态---在线增加group
*********************************************************************/
#include "precomp.h"

/*CAddEditGroupsTransaction*******************************************************/
class CAddEditGroupsTransaction : public CWaitableTransaction
{
public:
	CAddEditGroupsTransaction(
		__uint count, 
		PCNODE_KEY nodes, 
		PCRTK_GROUP groups,
		__uint timeout, 
		__bool bEdit=false
		)
	{
		void	*buf;
		left = count;
		
		m_bEdit = bEdit;
		/*将nodes和groups中的字符串复制给buf
		buf中的存储的顺序为：[nodes..groups..]
		*/
		buf = alloc_buffer_v(                                //librtk.dll
			nodes, sizeof(nodes[0])*count, 
			groups, sizeof(groups[0])*count, 
			0
			);
		if(!buf){
			return;
		}
		add_transaction(vbus_config, this);//将vbus_config强转为CRtkVBus*, 然后把this插入链表tList中      list<CTransaction*>
		//客户端发出的数据报，当在pmc.exe中添加一个组程序将执行到这里
		broadcast_rtk_data_with_guid(
			vbus_config,                  //链表g_buses <VBUS_ID, CRtkVbus*>的一个键值对的值
			PORT_ALL_SERVER,               //用于赋给数据包头RTK_PACKET的成员dest.port
			&Guid,                        //链表s_lst  <RTK_GUID, RTK_LIST_ENTRY>的键，在类CTransaction的构造函数中创建
			bEdit? PT_EditGroup : PT_AddGroup,  //用于赋给数据包头RTK_PACKET的成员packet_type
			buf,                                //将要发送的数据包内容(除了数据包头，注意data[1])
			(sizeof(nodes[0]) + sizeof(groups[0]))*count  //将要发送的数据包内容的大小
			);
		free_buffer(buf);
		Wait(timeout);
	}
private:
	virtual void OnEvent(PCRTK_PACKET p)
	{
		left -= _on_add_edit_groups(p, true);
		if(left <= 0){
			SetEvent();
		}
	}
public:
	__uint	left;
	__bool	m_bEdit;
};


//pmc客户端添加组
PROXY_API __uint PMC_API add_groups(
	__uint count,
	PCNODE_KEY nodes,
	PCRTK_GROUP groups,
	__uint timeout
	)
{
	CAddEditGroupsTransaction * trans = new 
		CAddEditGroupsTransaction(count, nodes, groups, timeout, false); 
	if(!trans){
		return 0;
	}
	trans->life = 30;//          trans的父类的父类CTransaction中有成员life
	return count - trans->left;
}

//pmc客户端编辑组，如添加描述、等
PROXY_API __uint PMC_API edit_groups(
	__uint count,
	PCNODE_KEY nodes,
	PCRTK_GROUP groups,
	__uint timeout
	)
{
	CAddEditGroupsTransaction * trans = new 
		CAddEditGroupsTransaction(count, nodes, groups, timeout, true);
	if(!trans){
		return 0;
	}
	trans->life = 30;//          trans的父类的父类CTransaction中有成员life
	return count - trans->left;
}
