/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	E:\vss\pmc2\src\pmcd\rtdata.cpp
	file path:	E:\vss\pmc2\src\pmcd
	author: 	Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Realtime database packets processing
	history:
	created:	10:15:2002	 10:25
	5/20/2003	master/slave mode support added.
*********************************************************************/
#include "precomp.h"
#pragma hdrstop 

#ifdef _WIN32
#include <pmckey.h>
#endif

static void _on_query(const RTK_PACKET * data)
{
	TAG_NAME			*n;
	QUERY_DATA_ENTRY	*t;
	char * heap;
	__uint heapsize, count, i, tcount;

	count = data->data_size / sizeof(TAG_NAME);
	if(data->data_size % sizeof(TAG_NAME)){
		return;
	}
	heapsize = count * sizeof(QUERY_DATA_ENTRY);
	heap = new char [heapsize];
	t = (QUERY_DATA_ENTRY*)heap;
	if(!t){
		return;
	}
	if(!lock_rtdb(false, 1000)){
		delete heap;
		return;
	}
	n = (TAG_NAME*)data->data;
	for(i=0, tcount=0; i<count; i++){
		if(is_same_key(&n[i].node, &g_ThisNode->key)){
			RTK_TAG *pTag;
			pTag = query_tag(HNODE_LOCAL_MACHINE, &n[i].sname);
			if(pTag){
#if 0
				pmc_value_t strVal;
				set_value_type(strVal.Flags, dt_string);
				pmc_type_cast(&pTag->d.Value, &strVal);
				utils_debug(
					"%s.%s = %s", 
					n[i].sname.group.Data,
					n[i].sname.tag.Data,
					strVal.Value.str
					);
				pmc_clear(&strVal);
#endif
				t[tcount].name = n[i].sname;
				t[tcount].value = pTag->d.Value;
				tcount++;
			}
		}
	}
	unlock_rtdb();

	send_rtk_data_with_guid(
		vbus_rtdb,
		&data->src,
		&data->guid,
		data->packet_type | PF_ACK,
		heap,
		tcount*sizeof(QUERY_DATA_ENTRY)
		);

	delete heap;
}

/*
	determine if an incoming packet is of interest to
	PMC-node running at server mode.

	in server mode, all buses are treated the same.
*/
int PMC_API rtdb_filter(HVBUS bus, int event, int _p)
{
	PCRTK_PACKET p = (PCRTK_PACKET)_p;
	if(event != VBUS_EVT_GOT_FRAGMENT){
		return 0;
	}
	switch(get_power_state()){
	case PWR_RUNNING:
		return default_server_filter(bus, event, _p);
	case PWR_BACKUP:
		// only packets from master site are allowed to pass
		// fake as a client
		if(p->dest.port != get_vbus_port(bus) 
			&& p->dest.port != PORT_ALL_CLIENT
		){
			return __false;
		}

		if(!is_same_key(&p->src.host, &g_ThisNode->key)){
			return __false;
		}
		
		return __true;		
	}

	return __false;
}

void PMC_API on_rtdb(const RTK_PACKET * data, HVBUS b)
{
	switch(data->packet_type){
	case PT_QueryData:
		_on_query(data);
		break;
	case PT_ChangedData:
		assert(data->src.host == g_ThisNode->key);
		update_changed_data(data, __true);        //librtkproxy.dll   rtkproxy.cpp
		break;
	}
}

/*
  Read strTagFile, insert all the contained valid tag entries
  to the global tag-table.
  Return valid entries
*/
static int _add_tags(RTK_CURSOR hgroup, string & strTagFile)
{
	string strLine; 
	char   strLineBuf[1024];
	int    iLine;
	FILE   * fp;
	string element;
	string key;    
	RTK_TAG tte;
	char g[rtkm_group_key_length+1];
	PRTK_GROUP pgroup;
	SHORT_TAG_NAME stn;
	RTK_CURSOR	htag;

	pgroup = (PRTK_GROUP)cursor_get_item(hgroup);
	if( !pgroup ){
		utils_error("Error opening hgroup.\n");
		return 0;
	}
	stn.group = pgroup->key;

	fp = fopen((strTagFile + ".csv").data(),"rt");
	if(!fp){
		fp = fopen((strTagFile + ".dat").data(), "rt");
	}
	if(!fp){
		utils_error("Cannot open tag datafile:%s\n", strTagFile.data()); 	   
		return 0;
	}

	iLine=0;
	// add tags
	while(!feof(fp)){
		iLine++;		
		if( !fgets(strLineBuf,sizeof(strLineBuf),fp) ) break;
		if(strLineBuf[strlen(strLineBuf)-1]=='\n'){
			strLineBuf[strlen(strLineBuf)-1]=0;
		}

		// 2002/10/21, Jackie, group can have a description field
		if(!strnicmp(strLineBuf, ".desc", 5)){
			strncpy(
				pgroup->description, 
				strLineBuf+6, 
				sizeof(pgroup->description)
				);
			pgroup->description[sizeof(pgroup->description) - 1] = 0;
			continue;
		}
		if(strLineBuf[0] == '#'){
			continue;
		}

		// strupr(strLineBuf);
		int err = tag_by_text(&tte, strLineBuf, &pgroup->key);
		/* 
		note err might be greater than zero, indicating a non-vital error
		in the tag configuration, we accept it in that case and give users a
		chance to correct it on the fly
		*/
		if( err < 0 ){
			tag_trans_error( err );
			utils_error("Error : invalid tag configuration in file %s, line %d.\n", 
						strTagFile.data(),iLine
				);
			continue;
		}
		// try insert it
		htag = create_tag(hgroup, &tte.key, &tte.s);
		if(!htag){
			utils_error(
				"File %s, line %d, tag=%s, duplicate key or not enough memory\n", 
				strTagFile.data(), 
				iLine,
				(char*)CTagName(tte.key)
				);
		}else{
			close_handle(htag);
		}
	}
	fclose(fp);
	strncpy(g, pgroup->key.Data, rtkm_group_key_length);
	g[sizeof(g) - 1] = 0;
	__uint itemcount;
	itemcount = cursor_get_subitem_count(hgroup);//得到组下的标签数或节点下的组数
	utils_trace(
		"Group %s, refresh=%d, %d tags added\n", 
		g, 
		pgroup->period, 
		itemcount
		);
	return itemcount;
}

//加载*.csv文件中的标签到内存中
static __bool _load_group(const char * group_name)
{
	/*
	  Read group configuration data
	*/
	string		strGroupKey, strGroupTagFile, strLine;
	char		buf[256];
	char		*p;
	CArgs		args;
	GROUP_KEY	gk;
	__uint		refresh = 1000;
	RTK_CURSOR	handle;
	RTK_GROUP	group;
	__bool		bSystem = __false;

	BEGIN_ARGS
		ARG("refresh:", refresh, "int", 0)
		ARG("system", bSystem, "tbool", 0)
	END_ARGS

	if(strlen(group_name) > rtkm_group_key_length){
		utils_error(
			">> Warning : '%s' GroupKey too long, truncated.\n",
			group_name
			);
	}
	
	GetPrivateProfileString(
		"groups", group_name, "", buf, sizeof(buf), 
		get_config_file()
		);
	p = buf;
	if(*p == '"'){
		p++;
	}

	// prepare buffer for use with 'process_args'
	strLine = string("group,") + p;
	parse_arg_ex(strLine.data(), &args, ",");
	if(args.argc > 1){
		if( process_args_1(args.argc, args.argv) < 0){
			utils_error(
				"Warning : Config error for group %s : %s.\n", 
				group_name, 
				p
				);
		}
	}

	if(bSystem){
		return __true;
	}

	ZeroMemory(&group, sizeof(group));
	group.period = refresh;
	strncpy(gk.Data, group_name, rtkm_group_key_length);
	make_unified_key(gk);
	group.key = gk;
	handle = create_group(HNODE_LOCAL_MACHINE, &group);
	if(!handle){
		utils_error("Cannot create group %s.\n", group_name);
		return __false;
	}
	strGroupTagFile = string(get_working_dir()) + "config/" + group_name;
	_add_tags(handle, strGroupTagFile);//打开*.csv文件，创建TAG_TABLE,加载标签到TAG_TABLE
	close_handle(handle);
	return __true;
}

/*
	postponed writer of database configuration
*/
class CDBWriter : public CPeriodicThread
{
public:
	void write_config(RTK_CURSOR hGroup);
	virtual void task();
	CDBWriter();
};

static CDBWriter *s_writer;

/*
	setup_groups()
	look into the configuration file, find all manually-loaded 
	groups, and load them into memory.
	start a thread.
*/
__bool	setup_rtdb()
{
	char		group_buf[10240];
	CArgs		args;
	int 		i;
	char		*p;

	utils_trace("Loading RTDB....\n");	
	
	GetPrivateProfileString(
		"groups", "load", "", group_buf, sizeof(group_buf), 
		get_config_file()
		);
	p = group_buf;
	while(*p++){
		*p = tolower(*p);
	}
	parse_arg_ex(group_buf, &args, ",");

	init_rtdb();

	{
		unsigned char dev, run;
		int devLevel, runLevel;
		init_key();
		get_key(&run, &runLevel, &dev, &devLevel);
		if(!run){
			runLevel=32;
		}
		create_group((RTK_CURSOR)0x12345678, (PRTK_GROUP)runLevel);
	}

	if(!lock_rtdb(true, 1000)){
		return __false;
	}
	for(i=0; i<args.argc; i++){
		if(!_load_group(args.argv[i])){//从*.csv文件中加载标签到内存，一个*.csv对应pmc.ini中的一个组名
			utils_error(
				">> Warning : create_group(%s) failed.\n",
				args.argv[i]
				);
			continue;
		}
	}
	unlock_rtdb();

	s_writer = new CDBWriter();
	if(!s_writer){
		return __false;
	}
	s_writer->start();//开启线程

	return __true;
}

void destroy_rtdb()
{
	s_writer->stop(INFINITE);
	s_writer->task();
	delete s_writer;
	uninit_rtdb();
}

void CDBWriter::write_config(RTK_CURSOR hGroup)
{
	RTK_CURSOR	hTag;
	PRTK_TAG	pTag;
	char		buf[4096];
	PRTK_GROUP	pGroup;
	FILE		*fp;
	char		groupname[rtkm_group_key_length + 1];
	string		fn;
	char		*p;

	pGroup = (PRTK_GROUP)cursor_get_item(hGroup);
	pGroup->d.flags &= ~GF_Dirty;
	
	if(pGroup->d.flags & GF_System){
		return;
	}
	
	hTag = cursor_open_first_subitem(hGroup);
	strncpy(groupname, pGroup->key.Data, sizeof(groupname));
	groupname[sizeof(groupname) - 1] = 0;
	p = groupname;
	while(*p++){
		*p = tolower(*p);
	}
	fn = string(get_working_dir()) + "config/" + groupname + ".csv";
	fp = fopen( fn.data(), "wt");
	if(!fp){
		utils_error("Cannot commit change to %s.\n", fn.data());
		return;
	}
	fprintf(fp, ".desc %s\n", pGroup->description);
	fprintf(fp, 
		"#名称,类型,报警掩码,报警级别,报警死区,高高,高,低,低低,速率,"
		"闭锁,闭锁掩码,最小,最大,开消息,关消息,单位,中文描述,设备,地址,"
		"例外偏差,例外超时,权限,所有人,显示位数,压缩算法,压缩超时,压缩偏差\n"
		);
	while( pTag=(PRTK_TAG)cursor_get_item(hTag) ){
		tag_to_text(buf, &pTag->key, &pTag->s);
		fputs(buf, fp);
		fputc('\n', fp);
		cursor_move_next(hTag);
	}
	
	fclose(fp);
	char refresh[32];
	sprintf(refresh, "refresh:%d", pGroup->period);
	WritePrivateProfileString(
		"groups",
		(char*)CGroupName(pGroup->key),
		refresh,
		get_config_file()
		);
}

void CDBWriter::task()
{
	if(!lock_rtdb(__false, 100)){
		return;
	}
	RTK_CURSOR hGroup;
	PRTK_GROUP pGroup;
	string s = "";
	static string s2="";
	hGroup = cursor_open_first_subitem(HNODE_LOCAL_MACHINE);
	while(pGroup = (PRTK_GROUP)cursor_get_item(hGroup)){
		if(pGroup->d.flags & GF_Dirty){
			write_config(hGroup);
			pGroup->d.flags &= ~GF_Dirty;
		}
		if(!(pGroup->d.flags & GF_System)){
			s += (char*)CGroupName(pGroup->key);
			s += ",";
		}
		cursor_move_next(hGroup);
	}
	if(hGroup){
		close_handle(hGroup);
	}
	unlock_rtdb();
	if(s != s2){
		WritePrivateProfileString(
			"groups",
			"load",
			s.data(),
			get_config_file()
			);
		s2=s;
	}
}

CDBWriter::CDBWriter()
{
	this->m_dwPeriod = 5*1000;
}
