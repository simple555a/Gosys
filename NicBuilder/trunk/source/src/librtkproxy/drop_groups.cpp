/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\rtkproxy\drop_groups.cpp
	file path:	e:\vss\pmc2\src\rtkproxy
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Delete groups from running DCS	
	history:
	created:	11/8/2002   17:19
*********************************************************************/
#include "precomp.h"

class CDropGroupsTransaction : public CWaitableTransaction
{
public:
	CDropGroupsTransaction(
		__uint count, 
		PCNODE_KEY nodes, 
		PCGROUP_KEY names, 
		__uint timeout)
	{
		void	*buf;
		left = count;
		
		buf = alloc_buffer_v(
			nodes, sizeof(nodes[0])*count,
			names, sizeof(names[0])*count, 
			0);
		if(!buf){
			return;
		}
		add_transaction(vbus_config, this);
		broadcast_rtk_data_with_guid(
			vbus_config,
			PORT_ALL_SERVER,
			&Guid,
			PT_DropGroup,
			buf,
			(sizeof(names[0]) + sizeof(nodes[0]))*count
			);
		free_buffer(buf);
		Wait(timeout);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		__uint count;
		count = _on_drop_groups(packet);
		left -= count;
		if(left <= 0){
			SetEvent();
		}
	}
	__uint left;
};

PROXY_API __uint PMC_API drop_groups(
	__uint count, 
	PCNODE_KEY nodes, 
	PCGROUP_KEY names, 
	__uint timeout
	)
{
	CDropGroupsTransaction * trans = new 
		CDropGroupsTransaction(count, nodes, names, timeout);
	if(!trans){
		return 0;
	}
	return count - trans->left;
}
