#include "precomp.h"

class CReadTagsTransaction : public CWaitableTransaction
{
public:
	void update(
		__uint count, 
		PCTAG_NAME names, 
		__uint timeout
		)
	{
		left = count;
		add_transaction(vbus_rtdb, this);

		broadcast_rtk_data_with_guid(
			vbus_rtdb,
			PORT_ALL_SERVER,
			&Guid,
			PT_QueryData,
			names,
			sizeof(*names)*count
			);

		Wait(timeout);
	}

private:
	virtual void OnEvent(PCRTK_PACKET p)
	{
		left -= _on_queried_data(p);
		if(left <= 0){
			SetEvent();
		}
	}
private:
	__uint	left;
};

PROXY_API __uint PMC_API proxy_read_tags(
	__uint count, 
	PCTAG_NAME names, 
	Ppmc_value_t values, 
	__uint timeout
	)
{
	__uint existed;
	return proxy_read_tags_ex(count, names, values, timeout, &existed);
}

PROXY_API __uint PMC_API proxy_read_tags_ex(
	__uint count, 
	PCTAG_NAME names, 
	Ppmc_value_t values, 
	__uint timeout,
	__uint *existed
	)
{
	__uint valids;
	CReadTagsTransaction *rain;

	if(timeout){		
		rain = new CReadTagsTransaction();
		if(!rain){
			return 0;
		}
		rain->update(count, names, timeout);
	}
	valids =  _read_tags(count, names, values, *existed);
	return valids;
}

