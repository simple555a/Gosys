/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\pmcd\operation.cpp
	file path:	E:\vss\pmc2\src\pmcd
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Operation packet acknowledge
	history:
	created:	10:15:2002   10:25
*********************************************************************/
/*
改动历史：
Date       Version  Modified by  Modify
2014-08-22 1.0.0.3     mox       1.添加_on_write_device3(PCRTK_PACKET data)函数，此函数只将标签点值置为0，不发送操作记录广播。
2014-08-26 1.0.0.3     mox       1.添加_on_write_device4(PCRTK_PACKET data)函数，此函数不改变标签点的值，只发送操作记录广播。
*/

#include "precomp.h"

static void _on_write_device(PCRTK_PACKET data)
{
	__uint count, i;
	PTAG_NAME names;
	PTAG_VALUE values;
	__bool ok;

	count = data->data_size / (sizeof(*names) + sizeof(*values));
	if(data->data_size % (sizeof(*names) + sizeof(*values))){
		return;
	}
	names = (PTAG_NAME)data->data;
	values = (PTAG_VALUE)(data->data + sizeof(*names)*count);
	

	// 2003/7/13, write_device should be acknowledged
	// and there should be only one tag per request packet
	assert(count == 1);

	for(i=0; i<count; i++){
		if(!is_same_key(&names[i].node, &g_ThisNode->key)){
			continue;
		}
		ok = io_write_device(&names[i].sname, values+i);
		if(lock_rtdb(__false, 100)){
			PRTK_TAG tte;
			tte = query_tag(HNODE_LOCAL_MACHINE, &names[i].sname);
			if(tte){
				write_device_alarm(NULL, tte, &values[i], &data->src.host);
			}
			unlock_rtdb();
		}
		send_rtk_data_with_guid(
			vbus_operation,
			&data->src,
			&data->guid,
			data->packet_type | (ok? PF_ACK : PF_Failed)
		);
	}
}

static void _on_write_device2(PCRTK_PACKET data)
{
	__uint count, i;
	PTAG_NAME names;
	PCpmc_value_t values;
	__bool ok;

	count = data->data_size / (sizeof(*names) + sizeof(*values));
	if(data->data_size % (sizeof(*names) + sizeof(*values))){
		return;
	}
	names = (PTAG_NAME)data->data;
	values = (PCpmc_value_t)(data->data + sizeof(*names)*count);

	// 2003/7/13, write_device should be acknowledged
	// and there should be only one tag per request packet
	assert(count == 1);

	for(i=0; i<count; i++, names++, values++){
		if(!is_same_key(&names->node, &g_ThisNode->key)){
			continue;
		}
		ok = io_write_device_ex(&names->sname, values);
		/* 2006-3-14 forbid write-device alarm originated from pmcsvr.exe and libpmcdb.dll */
#if 0
		if(lock_rtdb(__false, 100)){
			PRTK_TAG tte;
			tte = query_tag(HNODE_LOCAL_MACHINE, &names->sname);
			if(tte){
				write_device_alarm(NULL, tte, &values->Value, &data->src.host);
			}
			unlock_rtdb();
		}
#endif
		send_rtk_data_with_guid(
			vbus_operation,
			&data->src,
			&data->guid,
			data->packet_type | (ok? PF_ACK : PF_Failed)
		);
	}
}


static void _on_write_device3(PCRTK_PACKET data)
{
	__uint count, i;
	PTAG_NAME names;
	PTAG_VALUE values;
	__bool ok;

	count = data->data_size / (sizeof(*names) + sizeof(*values));
	if(data->data_size % (sizeof(*names) + sizeof(*values))){
		return;
	}
	names = (PTAG_NAME)data->data;
	values = (PTAG_VALUE)(data->data + sizeof(*names)*count);

	assert(count == 1);

	for(i=0; i<count; i++){
		if(!is_same_key(&names[i].node, &g_ThisNode->key)){
			continue;
		}
		ok = io_write_device(&names[i].sname, values+i);
		if(lock_rtdb(__false, 100)){
			PRTK_TAG tte;
			tte = query_tag(HNODE_LOCAL_MACHINE, &names[i].sname);
			//if(tte){
				//write_device_alarm(NULL, tte, &values[i], &data->src.host);//不广播操作日志动作
			///}
			unlock_rtdb();
		}
		send_rtk_data_with_guid(
			vbus_operation,
			&data->src,
			&data->guid,
			data->packet_type | (ok? PF_ACK : PF_Failed)
			);
	}
}

static void _on_write_device4(PCRTK_PACKET data)
{
	__uint count, i;
	PTAG_NAME names;
	PTAG_VALUE values;
	__bool ok;

	count = data->data_size / (sizeof(*names) + sizeof(*values));
	if(data->data_size % (sizeof(*names) + sizeof(*values))){
		return;
	}
	names = (PTAG_NAME)data->data;
	values = (PTAG_VALUE)(data->data + sizeof(*names)*count);

	assert(count == 1);

	for(i=0; i<count; i++){
		if(!is_same_key(&names[i].node, &g_ThisNode->key)){
			continue;
		}
		//ok = io_write_device(&names[i].sname, values+i);//不对具体的标签点进行具体的写值操作
		ok = 1;
		if(lock_rtdb(__false, 100)){
			//PRTK_TAG tte;
			//tte = query_tag(HNODE_LOCAL_MACHINE, &names[i].sname);
			//if(tte){
				//write_device_alarm(NULL, tte, &values[i], &data->src.host);
			    TAG_KEY * operName = &names[i].sname.tag;//将用户名作为参数传入
                write_device_alarm_y(NULL, &values[i], &data->src.host, operName);
			//}
			unlock_rtdb();
		}
		send_rtk_data_with_guid(
			vbus_operation,
			&data->src,
			&data->guid,
			data->packet_type | (ok? PF_ACK : PF_Failed)
			);
	}
}

static void _on_write_device5(PCRTK_PACKET data)
{
	__uint count, i;
	PTAG_NAME names;
	PTAG_VALUE values;
    char* usrName;
	__bool ok;

	count = data->data_size / (sizeof(TAG_NAME) + sizeof(pmc_value_t)+ 32);
	if(data->data_size % (sizeof(TAG_NAME) + sizeof(pmc_value_t)+ 32 )){
		return;
	}
	names = (PTAG_NAME)data->data;
	values = (PTAG_VALUE)(data->data + sizeof(TAG_NAME)*count);
	usrName = (char *)(data->data + sizeof(TAG_NAME)*count+  sizeof(pmc_value_t));

	assert(count == 1);

	for(i=0; i<count; i++){
		if(!is_same_key(&names[i].node, &g_ThisNode->key)){
			continue;
		}
		ok = io_write_device(&names[i].sname, values+i);//对具体的标签点进行具体的写值操作
        //解析出用户名和模拟量的新旧的值
		if(lock_rtdb(__false, 100)){
			PRTK_TAG tte;
			tte = query_tag(HNODE_LOCAL_MACHINE, &names[i].sname);
			if(tte){
			write_device_alarm_z(NULL, tte, &values[i], &data->src.host, usrName);
			}
			unlock_rtdb();
		}
		send_rtk_data_with_guid(
			vbus_operation,
			&data->src,
			&data->guid,
			data->packet_type | (ok? PF_ACK : PF_Failed)
			);
	}
}
void PMC_API on_operation(PCRTK_PACKET data, HVBUS bus)
{
	switch(data->packet_type){
	case PT_Operation:
		_on_write_device(data);
		break;
	case PT_Operation2:
		_on_write_device2(data);
		break;
	case PT_Operation3:
		_on_write_device3(data);
		break;
	case PT_Operation4:
		_on_write_device4(data);//用户登陆、注销、关闭、启动的操作记录广播
		break;
	case PT_Operation5:
		_on_write_device5(data);//模拟量、开关量置值操作记录广播
		break;
	}
}
