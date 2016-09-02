/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\pmcd\system.cpp
	file path:	e:\vss\pmc2\src\pmcd
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	System message handling	
	history:
	created:	10:15:2002   10:26
*********************************************************************/
#include "precomp.h"

#ifndef _WIN32
#define DeleteFile(file) (unlink(file)? __false : __true)
#endif

#define answer_failed()\
	send_rtk_data_with_guid(\
		vbus_system, \
		&p->src, \
		&p->guid, \
		p->packet_type | PF_ACK | PF_Failed\
	)
#define answer_ok()\
	send_rtk_data_with_guid(\
		vbus_system, \
		&p->src, \
		&p->guid, \
		p->packet_type | PF_ACK\
	)

static void _on_get_file(PCRTK_PACKET p)
{
	string sFile;
	sFile = get_working_dir();
	if(!p->data_size){
		answer_failed();
		return;
	}

	if(p->data[p->data_size] != '\0'){
		answer_failed();
		return;
	}

	FILE *fp;
	sFile += p->data;
	fp = fopen(sFile.data(), "rb");
	if(!fp){
		answer_failed();
		return;
	}
	char *buf;
	__uint len = get_file_length(sFile.data());
	buf = new char[len];
	if(!buf){
		fclose(fp);
		answer_failed();
		return;
	}
	if( !fread(buf, len, 1, fp) ){
		delete buf;
		fclose(fp);
		answer_failed();
		return;
	}
	send_rtk_data_with_guid(
		vbus_system,
		&p->src,
		&p->guid,
		PT_GetFile | PF_ACK,
		buf,
		len
		);
	delete buf;
	fclose(fp);
}

static void _on_get_file_size(PCRTK_PACKET p)
{
	string sFile;

	sFile = get_working_dir();
	if(!p->data_size){
		DEBUG_PRINTF(("GFS:Invalid C0001.\n"));
		answer_failed();
		return;
	}
	if(p->data[p->data_size] != '\0'){
		DEBUG_PRINTF(("GFS:Invalid C0002.\n"));
		answer_failed();
		return;
	}
	__uint len;
	sFile += p->data;
	if(!file_exists(sFile.data())){
		DEBUG_PRINTF(("GFS:Invalid C0003(%s).\n", p->data));
		answer_failed();
		len = 0;
		return;
	}else{
		len = get_file_length(sFile.data());
	}
	
	DEBUG_PRINTF(("GFS:%s=%d bytes.\n", p->data, len));

	send_rtk_data_with_guid(
		vbus_system,
		&p->src,
		&p->guid,
		p->packet_type | PF_ACK,
		&len,
		sizeof(len)
		);
}

static void _on_put_file(PCRTK_PACKET p)
{
	string sFile;
	sFile = get_working_dir();
	FILE *fp;
	const char *name, *data;
	
	if(!p->data_size){
		answer_failed();
		return;
	}
	__u32 namelen;
	namelen = *((__u32*)p->data);
	if(namelen >= p->data_size){
		answer_failed();
		return;
	}
	name = p->data + sizeof(__u32);
	data = name + namelen + 1;
	if(name[namelen] != '\0'){
		answer_failed();
		return;
	}
	sFile += name;
	fp = fopen(sFile.data(), "wb");
	if(!fp){
		answer_failed();
		return;
	}
	__u32 datalen;
	datalen = p->data_size - namelen - sizeof(__u32) - 1;
	if( fwrite(data, 1, datalen, fp) != datalen ){
		fclose(fp);
		answer_failed();
		return;
	}
	__bool ack = send_rtk_data_with_guid(
		vbus_system,
		&p->src,
		&p->guid,
		PT_PutFile | PF_ACK,
		"OK",
		3
		);
	fclose(fp);
	utils_debug(
		"FILE(%s) uploaded, %d bytes, ack=%d.\n", 
		sFile.data(), 
		datalen, 
		ack
		);
}

static void _on_delete_file(PCRTK_PACKET p)
{
	string sFile;
	sFile = get_working_dir();
	if(!p->data_size){
		answer_failed();
		return;
	}

	if(p->data[p->data_size] != '\0'){
		answer_failed();
		return;
	}
	sFile += p->data;
	if( !DeleteFile(sFile.data()) ){
		answer_failed();
	}else{
		answer_ok();
	}
}

void PMC_API on_system(const RTK_PACKET * data, HVBUS b)
{
	{
		static __bool priorityBoosted = __false;
		if(!priorityBoosted){
			DWORD boost;
			boost = GetPrivateProfileInt(
				"vbus", 
				"SystemBoost", 
				THREAD_PRIORITY_ABOVE_NORMAL,
				get_config_file()
				);
			SetThreadPriority(GetCurrentThread(), boost);
			priorityBoosted = __true;
		}
	}

	switch(data->packet_type){
	case PT_System:
		if(data->data[data->data_size - 1] != '\0'){
			return;
		}
		utils_trace(
			"Got system message from %s:%d, contents:%s\n", 
			(char *)CHostName(data->src.host), data->src.port, data->data
			);
		shell_execute(g_shell, (char *)data->data, (void *)data);
		break;
	case PT_GetFile:
		_on_get_file(data);
		break;
	case PT_GetFileSize:
		_on_get_file_size(data);
		break;
	case PT_PutFile:
		_on_put_file(data);
		break;
	case PT_DeleteFile:
		_on_delete_file(data);
		break;
	case PT_ServerClaim:
#if 0
		PrimaryWatchDogCounter++;
		if(PrimaryWatchDogCounter > MaxPrimaryWatchDogCounter){
			PrimaryWatchDogCounter = MaxPrimaryWatchDogCounter;
		}
#else
		PrimaryWatchDogCounter = MaxPrimaryWatchDogCounter;
		printf("------go into the on_system() function!!!------------\n");

#endif
		break;
	}
}
