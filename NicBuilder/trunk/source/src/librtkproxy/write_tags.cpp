/*
改动历史：
Date       Version  Modified by  Modify
2012-7-13  1.0.0.2  Zander Sun   1.添加case dt_date: -- proxy_write_tags_ex()
2014-8-22  1.0.0.3      mox      1.添加proxy_write_tags_x，用来走标签点只置0，不记录的接口
                                 2.在类class CWriteTagTransaction中添加Query_x()
2014-8-25  1.0.0.3      mox      1.添加proxy_write_tags_y，不改变标签点的值，只记录操作信息

*/
#include "precomp.h"

class CWriteTagTransaction : public CWaitableTransaction
{
public:
	CWriteTagTransaction(char * usr = NULL)
	{
		memset(usrname,0,32);
		add_transaction(vbus_operation, this);
		if (usr)
		{
			memcpy(usrname,usr,strlen(usr)+1);
		} 
	}
	__bool Query(
		PCTAG_NAME name, 
		PCTAG_VALUE value,
		__uint timeout
		)
	{
		char heap[sizeof(TAG_NAME) + sizeof(pmc_value_t)];
		__uint heapsize;
		RTK_ADDR addr;
		
		heapsize = sizeof(*name) + sizeof(*value); + sizeof("xingmo");
		memcpy(heap, name, sizeof(*name));
		memcpy(heap + sizeof(*name), value, sizeof(*value));
		
		node_to_host(&name->node, &addr.host);
		addr.port = PORT_ALL_SERVER;
		
		send_rtk_data_with_guid(
			vbus_operation,
			&addr,
			&Guid,
			PT_Operation,
			heap,
			heapsize
			);
		return Wait(timeout);
	}

	__bool Query_x(
		PCTAG_NAME name, 
		PCTAG_VALUE value,
		__uint timeout
		)
	{
		char heap[sizeof(TAG_NAME) + sizeof(pmc_value_t)];
		__uint heapsize;
		RTK_ADDR addr;

		heapsize = sizeof(*name) + sizeof(*value);
		memcpy(heap, name, sizeof(*name));
		memcpy(heap + sizeof(*name), value, sizeof(*value));

		node_to_host(&name->node, &addr.host);
		addr.port = PORT_ALL_SERVER;

		send_rtk_data_with_guid(
			vbus_operation,
			&addr,
			&Guid,
			PT_Operation3,
			heap,
			heapsize
			);
		return Wait(timeout);
	}

	__bool Query_y(
		PCTAG_NAME name, 
		PCTAG_VALUE value,
		__uint timeout
		)
	{
		char heap[sizeof(TAG_NAME) + sizeof(pmc_value_t)];
		__uint heapsize;
		RTK_ADDR addr;

		heapsize = sizeof(*name) + sizeof(*value);
		memcpy(heap, name, sizeof(*name));
		memcpy(heap + sizeof(*name), value, sizeof(*value));

		node_to_host(&name->node, &addr.host);
		addr.port = PORT_ALL_SERVER;

		send_rtk_data_with_guid(
			vbus_operation,
			&addr,
			&Guid,
			PT_Operation4,
			heap,
			heapsize
			);
		return Wait(timeout);
	}

	__bool Query_z(
		PCTAG_NAME name, 
		PCTAG_VALUE value,
		__uint timeout
		)
	{
		char heap[sizeof(TAG_NAME) + sizeof(pmc_value_t) + sizeof(usrname)];
		__uint heapsize;
		RTK_ADDR addr;


		heapsize = sizeof(TAG_NAME) + sizeof(pmc_value_t) + sizeof(usrname);
		ZeroMemory(heap,heapsize);
		memcpy(heap, name, sizeof(*name));
		memcpy(heap + sizeof(TAG_NAME), value, sizeof(pmc_value_t));
		memcpy(heap + sizeof(TAG_NAME) +sizeof(pmc_value_t), usrname, sizeof(usrname));

		node_to_host(&name->node, &addr.host);
		addr.port = PORT_ALL_SERVER;

		send_rtk_data_with_guid(
			vbus_operation,
			&addr,
			&Guid,
			PT_Operation5,
			heap,
			heapsize
			);
		return Wait(timeout);
	}
private:
	virtual void OnEvent(PCRTK_PACKET p)
	{
		SetEvent();
	}
};

/*
	proxy_write_tags
	description : inform the DB server to output a value to the corresponding
	device.
*/
PROXY_API __uint PMC_API proxy_write_tags(
	__uint count,
	PCTAG_NAME names, 
	PCTAG_VALUE values
	)
{
	__uint i;
	__uint j;
	__uint ok;

	ok = 0;
	for(i=0; i<count; i++, names++, values++){
		CWriteTagTransaction * tr;
		tr = new  CWriteTagTransaction();
		if(!tr){
			continue;
		}
		for(j=0; j<3; j++){
			tr->life = 20;
			if(tr->Query(names, values, 1000)){
				ok++;
				break;
			}
			rtk_sleep(200);
		}		
	}

	return ok;
}

//proxy_write_tags_x,用来调用Query_x。mox+++
PROXY_API __uint PMC_API proxy_write_tags_x(
	__uint count,
	PCTAG_NAME names, 
	PCTAG_VALUE values
	)
{
	__uint i;
	__uint j;
	__uint ok;

	ok = 0;
	for(i=0; i<count; i++, names++, values++){
		CWriteTagTransaction * tr;
		tr = new CWriteTagTransaction();
		if(!tr){
			continue;
		}
		for(j=0; j<3; j++){
			tr->life = 20;
			if(tr->Query_x(names, values, 1000)){
				ok++;
				break;
			}
			rtk_sleep(200);
		}		
	}

	return ok;
}

//proxy_write_tags_y,用来调用Query_y。mox+++
PROXY_API __uint PMC_API proxy_write_tags_y(
	__uint count,
	PCTAG_NAME names, 
	PCTAG_VALUE values
	)
{
	__uint i;
	__uint j;
	__uint ok;

	ok = 0;
	for(i=0; i<count; i++, names++, values++){
		CWriteTagTransaction * tr;
		tr = new CWriteTagTransaction();
		if(!tr){
			continue;
		}
		for(j=0; j<3; j++){
			tr->life = 20;
			if(tr->Query_y(names, values, 1000)){
				ok++;
				break;
			}
			rtk_sleep(200);
		}		
	}

	return ok;
}

PROXY_API __uint PMC_API proxy_write_tags_z(
	__uint count,
	PCTAG_NAME names, 
	PCTAG_VALUE values,
	char * usrname
	)
{
	__uint i;
	__uint j;
	__uint ok;

	ok = 0;
	for(i=0; i<count; i++, names++, values++){
		CWriteTagTransaction * tr;
		tr = new CWriteTagTransaction(usrname);
		if(!tr){
			continue;
		}
		for(j=0; j<3; j++){
			tr->life = 20;
			if(tr->Query_z(names, values, 1000)){
				ok++;
				break;
			}
			rtk_sleep(200);
		}		
	}

	return ok;
}

class CWriteTagTransaction2 : public CWaitableTransaction
{
public:
	CWriteTagTransaction2()
	{
		add_transaction(vbus_operation, this);
	}
	__bool Query(
		PCTAG_NAME name, 
		PCpmc_value_t value,
		__uint timeout
		)
	{
		char heap[sizeof(TAG_NAME) + sizeof(pmc_value_t)];
		__uint heapsize;
		RTK_ADDR addr;

		heapsize = sizeof(*name) + sizeof(*value);
		memcpy(heap, name, sizeof(*name));
		memcpy(heap + sizeof(*name), value, sizeof(*value));

		node_to_host(&name->node, &addr.host);
		addr.port = PORT_ALL_SERVER;

		send_rtk_data_with_guid(
			vbus_operation,
			&addr,
			&Guid,
			PT_Operation2,
			heap,
			heapsize
			);
		return Wait(timeout);
	}
private:
	virtual void OnEvent(PCRTK_PACKET p)
	{
		SetEvent();
	}
};

PROXY_API __uint PMC_API proxy_write_tags_ex(
	__uint count,
	PCTAG_NAME names, 
	PCpmc_value_t values
	)
{
	__uint i;
	__uint j;
	__uint ok;
	CWriteTagTransaction2 * tr;

	ok = 0;
	for(i=0; i<count; i++, names++, values++){
		switch(get_value_type(values->Flags)){
		case dt_bool:
		case dt_int8:
		case dt_int16:
		case dt_int32:
		case dt_int64:
		case dt_uint64:
		case dt_uint8:
		case dt_uint16:
		case dt_uint32:
		case dt_real4:
		case dt_real8:
		case dt_date://2012-7-13
			tr = new CWriteTagTransaction2();
			if(!tr){
				continue;
			}
			for(j=0; j<3; j++){
				tr->life = 10;
				if(tr->Query(names, values, 1000)){
					ok++;
					break;
				}
				rtk_sleep(200);
			}
			break;
		default:
			// non-numeric types cannot be written
			;
		}
	}

	return ok;
}

