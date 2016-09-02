#include "precomp.h"

class CDropTagsTransaction : public CWaitableTransaction
{
public:
	CDropTagsTransaction(__uint count, PCTAG_NAME names, __uint timeout)
	{
		void	*buf;
		left = count;
		
		buf = alloc_buffer_v(names, sizeof(names[0])*count, 0);
		if(!buf){
			return;
		}
		add_transaction(vbus_config, this);
		broadcast_rtk_data_with_guid(
			vbus_config,
			PORT_ALL_SERVER,
			&Guid,
			PT_DropTag,
			buf,
			sizeof(names[0])*count
			);
		free_buffer(buf);
		Wait(timeout);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		_on_drop_tags(packet);
		left--;
		if(left <= 0){
			SetEvent();
		}
	}
	__uint left;
};

PROXY_API __uint PMC_API drop_tags(
	__uint count, 
	PCTAG_NAME names, 
	__uint timeout
	)
{
	CDropTagsTransaction * trans = new 
		CDropTagsTransaction(count, names, timeout);
	if(!trans){
		return 0;
	}
	return count - trans->left;
}
