#include "precomp.h"

class CReloadTags : 
	public CWaitableTransaction
{
public:
	CReloadTags(__uint count, PCTAG_NAME names, __uint timeout)
	{
		left = count;
		add_transaction(vbus_config, this);		
		broadcast_rtk_data_with_guid(
			vbus_config, 
			PORT_ALL_SERVER,
			&Guid,			
			PT_ReloadTags,
			names,
			sizeof(names[0]) * count
			);
		Wait(timeout);
	}
	virtual void OnEvent(PCRTK_PACKET packet)
	{
		left -= _on_add_edit_tags(packet, false);
		if(left <= 0){
			SetEvent();
		}
	}
	__uint	get_left()
	{
		return left;
	}
private:
	__uint	left;
};

PROXY_API __uint PMC_API reload_tags(__uint count, PCTAG_NAME names, __uint timeout)
{
	CReloadTags *loader = new CReloadTags(count, names, timeout);
	return count - loader->get_left();
}

