#include "precomp.h"

//
class CAddEditTagsTransaction : public CWaitableTransaction
{
public:
	CAddEditTagsTransaction(
		__uint count, 
		PCTAG_NAME names, 
		STATIC_TAG_DATA *s, 
		__uint timeout, 
		__bool bEdit=false
		)
	{
		void	*buf;
		left = count;
		
		m_bEdit = bEdit;
		buf = alloc_buffer_v(names, sizeof(names[0])*count, s, sizeof(s[0])*count, 0);
		if(!buf){
			return;
		}
		add_transaction(vbus_config, this);
		broadcast_rtk_data_with_guid(
			vbus_config,
			PORT_ALL_SERVER,
			&Guid,
			bEdit? PT_EditTag : PT_AddTag,
			buf,
			(sizeof(names[0]) + sizeof(s[0]))*count
			);
		free_buffer(buf);
		Wait(timeout);
	}
private:
	virtual void OnEvent(PCRTK_PACKET p)
	{
		left -= _on_add_edit_tags(p, true);
		if(left <= 0){
			SetEvent();
		}
	}
public:
	__uint	left;
	__bool	m_bEdit;
};

//����CAddEditTagsTransaction��ִ�������Ĺ��캯��������PT_addTag�͵Ķಥ��
PROXY_API __uint PMC_API add_tags(
	__uint count, 
	PCTAG_NAME names, 
	STATIC_TAG_DATA *s, 
	__uint timeout
	)
{
	// 
	CAddEditTagsTransaction * trans = new CAddEditTagsTransaction(count, names, s, timeout);
	if(!trans){
		return 0;
	}
	return count - trans->left;
}

//����CAddEditTagsTransaction��ִ�������Ĺ��캯��������PT_EditTag�͵Ķಥ��
PROXY_API __uint PMC_API edit_tags(
	__uint count, 
	PCTAG_NAME names, 
	STATIC_TAG_DATA *s, 
	__uint timeout
	)
{
	// 
	CAddEditTagsTransaction * trans = new CAddEditTagsTransaction(count, names, s, timeout, true);
	if(!trans){
		return 0;
	}
	return count - trans->left;
}
