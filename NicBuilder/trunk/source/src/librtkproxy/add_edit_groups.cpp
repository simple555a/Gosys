/********************************************************************
	PC-Based monitoring & controlling system source
	product version : 2.03
	filename: 	e:\vss\pmc2\src\rtkproxy\add_groups.cpp
	file path:	e:\vss\pmc2\src\rtkproxy
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Add groups to running DCS	
	history:
	created:	11/8/2002   17:19

    ������̬---��������group
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
		/*��nodes��groups�е��ַ������Ƹ�buf
		buf�еĴ洢��˳��Ϊ��[nodes..groups..]
		*/
		buf = alloc_buffer_v(                                //librtk.dll
			nodes, sizeof(nodes[0])*count, 
			groups, sizeof(groups[0])*count, 
			0
			);
		if(!buf){
			return;
		}
		add_transaction(vbus_config, this);//��vbus_configǿתΪCRtkVBus*, Ȼ���this��������tList��      list<CTransaction*>
		//�ͻ��˷��������ݱ�������pmc.exe�����һ�������ִ�е�����
		broadcast_rtk_data_with_guid(
			vbus_config,                  //����g_buses <VBUS_ID, CRtkVbus*>��һ����ֵ�Ե�ֵ
			PORT_ALL_SERVER,               //���ڸ������ݰ�ͷRTK_PACKET�ĳ�Աdest.port
			&Guid,                        //����s_lst  <RTK_GUID, RTK_LIST_ENTRY>�ļ�������CTransaction�Ĺ��캯���д���
			bEdit? PT_EditGroup : PT_AddGroup,  //���ڸ������ݰ�ͷRTK_PACKET�ĳ�Աpacket_type
			buf,                                //��Ҫ���͵����ݰ�����(�������ݰ�ͷ��ע��data[1])
			(sizeof(nodes[0]) + sizeof(groups[0]))*count  //��Ҫ���͵����ݰ����ݵĴ�С
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


//pmc�ͻ��������
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
	trans->life = 30;//          trans�ĸ���ĸ���CTransaction���г�Աlife
	return count - trans->left;
}

//pmc�ͻ��˱༭�飬�������������
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
	trans->life = 30;//          trans�ĸ���ĸ���CTransaction���г�Աlife
	return count - trans->left;
}
