/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 3.5
	author : Jackie Pan, panxiaohui@actech-mc.com
	
	purpose : PMC F8 Device Driver
	history : 
	created : by J.Hunter, Nov.,2003

	jan16,2007, added failcount to give tags a relaxation time before being marked
	as invalid

	jan17,2007, standby station is no longer valid for connection because write operation
	is not supported on standby station.
*********************************************************************/
#define IStream foo_IStream
#include <pmc.h>
#undef IStream
#include <f8.h>
#include <f8_kadapter.h>
#include <ddk.h>
#include <rtk_thread.h>
#include <shell.h>

/*
	dyn_pin_t is a 8-byte data structure that represent a pin's
	dynamic state, this will contribute to the bulk of the information
	that are transferred on the control network.
*/

#pragma comment(linker, "/base:0x76520000")
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "libspac.lib")
#pragma comment(lib, "libpowermgr.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libkadapter.lib")
#pragma comment(lib, "libfkern.lib")
#pragma comment(lib, "librtkshell.lib")
#pragma comment(lib, "libcobj.lib")

struct connection_t{
	sockaddr_in peer;
};

#include <pshpack1.h>
struct var_def_t{
	f8_uuid uuid;//f8_vm
	struct mem_addr_t addr;//f8_binary
};
#include <poppack.h>

/*
 * 函数功能：比较两个地址偏移量上的变量名
 * 参数说明：
 *        _variables    基地址        IN
 *		  elem1         地址偏移量    IN
 *		  elem2         地址偏移量    IN
 * 返回值：
 *           0  相等
 *           -1 elem1的变量名 < elem2的变量名
 *           1  elme2的变量名 > elem2的变量名
 */
static int __cdecl compareName(void * _variables, const void *elem1, const void *elem2 )
{
	struct blk_var_t *a, *b, *variables;//f8_kernel
	variables = (blk_var_t*)_variables;
	a = variables + *((int*)elem1);
	b = variables + *((int*)elem2);
	return strcmp(a->name, b->name);
}

/* 
 * 函数功能：比较指定变量名和地址偏移量的变量名。
 * 参数说明：
 *           _variables   基地址        IN
 *           elem1        指定的变量名  IN
 *           elem2        地址偏移量    IN
 * 返回值：
 *           0   相等
 *           -1  elem1 < elem2的变量名
 *           1   elem1 > elem2的变量名
 */
static int __cdecl compareNameIndex(void * _variables, const void *elem1, const void *elem2 )
{
	struct blk_var_t *b, *variables;
	char * a;
	variables = (blk_var_t*)_variables;
	a = (char *)elem1;
	b = variables + *((int*)elem2);
	return strcmp(a, b->name);
}

struct CF8Device : public CRtkThread,CLock{
public:
	enum{
		PMCF8_MAX_VARIABLES = 4096,
	};
	PDEVICE_INFO device;
	HF8_KERNEL kernel;
	ITcpAdapter *tcpAdapter;
	IKAdapter * adapter;
	connection_t * activeConnection;
	connection_t connections[4];
	__int debugLevel;
	__uint refresh;
	ctrller_config ctrllerConfig;
	__u8 * xMemPointers[KERN_NUM_SECTIONS];
	struct kstatus_t tstatus;
	
	struct blk_var_t variables[PMCF8_MAX_VARIABLES];
	struct blk_var_t workBuf[PMCF8_MAX_VARIABLES];
	int nameIndex[PMCF8_MAX_VARIABLES];
	PRTK_LIST_ENTRY idIndex;
	__uint varCount;
	int failCount;
	int maxFailCount;

	CF8Device();	
	~CF8Device();
	bool config(PDEVICE_INFO dev);
	bool checkConnection();
	f8_status uploadConfiguration();
	f8_status uploadVariableTable();
	void validateVariables();
	virtual __uint run();
	__bool translate(PRTK_TAG tag);
	void update(PCRTK_TAG tte, pmc_value_t * pt, PCRTK_TIME tm);
	__bool write(PRTK_TAG tte, PCTAG_VALUE value);

	__bool isStandby;
	__bool verMatch;
};

/* 
 * 函数功能：构造函数。给类相关成员赋初值。
 * 参数说明：无。
 * 返回值：无
 */
CF8Device::CF8Device()
{
	int i;

	device = 0;
	kernel = 0;
	tcpAdapter = 0;
	adapter = 0;
	debugLevel = 0;
	refresh = 250;
	activeConnection = connections;
	memset(connections, 0, sizeof(connections));	
	for(i=0; i<2; i++){
		connections[i].peer.sin_addr.s_addr = INADDR_NONE;
	}
	__new__(ITcpAdapter, tcpAdapter);
	/*
	tcpAdapter = (ITcpAdapter*)__malloc__(sizeof(ITcpAdapter));
	if (tcpAdapter)
	{
		tcpAdapter->vtbl = get_ITcpAdapter_vtable();
		ITcpAdapter_create(tcpAdapter);
	}
	*/

	adapter = __ucast__(ITcpAdapter, IKAdapter, tcpAdapter);
	/*
	adapter = (tcpAdapter? (& (((ITcpAdapter*)(tcpAdapter))->__inherited_IKAdapter)) : 0);
	*/
	kernel = adapter->kernel;
	varCount = 0;
	idIndex = ke_init_hash();//初始化索引链表
	failCount=0;
	maxFailCount=10;

	isStandby = true;
	verMatch = false;

	Lock();
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		xMemPointers[i] = ke_get_section(kernel, i);//结构体kernel_t的成员
	}
	UnLock();
}

/* 
 * 函数功能: 析构函数
 * 参数说明：无
 * 返回值：无
 */
CF8Device::~CF8Device()
{
	__delete__(tcpAdapter);//free();destroy()
	ke_uninit_hash(idIndex);//free索引链表
}

/* 
 * 函数功能：
 * 参数说明：
 * 返回值：
 */
/* initialize the device object */
bool CF8Device::config(PDEVICE_INFO dev)
{
	int i;
	device = dev;
	dev->OwnerField = this;
	
	CArgs args;
	
	BEGIN_ARGS
		ARG("debug=", debugLevel, "int", "debug level")
		ARG("refresh=", refresh, "unsigned int", "refresh time")
		ARG("failcount=",maxFailCount,"unsigned int","allowable failcount")
	END_ARGS
    /*
	CCmdLineArgA __CmdLineArgValues[]={
		{"debug=", &debugLevel, "int", "debug Level"},
		{"refresh=", &refresh, "unsighed int", "refresh time"},
		{"failcount=", &maxFailCount, "unsigned int", "allowable failcount"},
		{0, 0, 0, 0}
	};
	*/	
	
	//解析初始化命令？？
	parse_arg_ex(dev->parameter, &args, " ");
	if(process_args_ex(args.argc, args.argv, __CmdLineArgValues, 0, 0) < 0){
		print_usage_1();
		return false;
	}
    
	//解析设备地址
	parse_arg_ex(dev->address, &args, ";");
	if(args.argc > 2){
		utils_error("F8 - `%s': more than two addresses specified, rest ignored.\n", dev->address);
		args.argc = 2;
	}
    
	//
	for(i=0;i<4;i++)
		connections[i].peer.sin_addr.s_addr=INADDR_NONE;
    
	//处理控制器的地址
	for(i=0; i<args.argc; i++){
		connection_t & con = connections[i];
		con.peer.sin_family = AF_INET;
		char *p;
		p=strchr(args.argv[i],':');
		if(p){
			*p=0;
			con.peer.sin_addr.s_addr = inet_addr(args.argv[i]);
			con.peer.sin_port=htons(atoi(p+1));
		}else{
			con.peer.sin_addr.s_addr = inet_addr(args.argv[i]);
			con.peer.sin_port = htons(F8_CFG_TCP_PORT);
		}
		connections[i+2]=con;
		if(con.peer.sin_addr.s_addr == INADDR_NONE){
			struct hostent * he;
			he = gethostbyname(args.argv[i]);
			if(!he){
				utils_error("F8 -  `%s' is not a valid internet address.\n", args.argv[i]);
				continue;
			}
			con.peer.sin_addr = *((struct in_addr*)he->h_addr); 	
		}		
		if(con.peer.sin_addr.s_addr != INADDR_NONE){
			connections[i+2].peer.sin_addr.s_addr+=0x1000000;
		}
	}

	utils_debug("F8 - Available servers : ");
	for(i=0;i<4;i++){
		if(connections[i].peer.sin_addr.s_addr!=INADDR_NONE){
			utils_debug("%s ",inet_ntoa(connections[i].peer.sin_addr));
		}
	}
	utils_debug("\n");

	if(connections[0].peer.sin_addr.s_addr == INADDR_NONE && connections[1].peer.sin_addr.s_addr == INADDR_NONE){
		return false;
	}
	
	return true;
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
bool CF8Device::checkConnection()
{
	if(get_power_state() != PWR_RUNNING){
		isStandby = true;
	}else{
		isStandby = false;
	}
	
	if(!(adapter->flags & ADAPTER_F_ONLINE)){
		if(activeConnection->peer.sin_addr.s_addr!=INADDR_NONE){
			char url[F8_MAX_PATH];
			sprintf(url, "%s:%d", inet_ntoa(activeConnection->peer.sin_addr),htons(activeConnection->peer.sin_port));
			f8_status code;
			code = __vcall__(adapter, online, (adapter, url, 1000));
/*#define __vcall__(__obj, __func, __parm)\
	(	_CObj_CheckPureVCall( \
	(void*)(((__obj)->vtbl)->__func), \
#__func, \
	__FILE__, __LINE__\
	), \
	((__obj)->vtbl)->__func\
	) \
	__parm*/
			/*(  _CObj_CheckPureVCall
			        ( 
			         (void*) (((adapter)->vtbl)->online), 
                      #online,
			          __FILE__, 
					  __LINE__
			        ), 
			          ((adapter)->vtbl)->online)
			(adapter, url, 1000);*/

			if(F8_SUCCESSED(code)){
				utils_trace("F8>> station %s connected.\n",url);
			}else{
				utils_trace("F8>> station %s connect failed.\n", url);
			}
		}else{
			utils_trace("Err addr\n");
		}
	}
	
	if(adapter->flags & ADAPTER_F_ONLINE){
		// check version and cpu state
		struct kern_conf_cmd cmd;
		struct kstatus_t st;
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd = F8_GET_STATUS;
		cmd.isize = 0;
		cmd.obuffer = (char *)&st;
		cmd.osize = sizeof(st);
		__vcall__(adapter, exec_remote, (adapter, &cmd, 10000));

		if(!F8_SUCCESSED(cmd.retcode)){
			utils_debug("[F8] error -1\n");
			return false;
		}

		if(st.state==KERN_S_RUNNING /* || (st.state==KERN_S_STANDBY && st.flags & (1<<FKERN_LED_SYNCHRONIZED))*/){
			if(st.prog_id == tstatus.prog_id){
				verMatch = true;
				return true;
			}else{
				verMatch = false;
				/* program version changed, reload it */
				utils_debug("F8 - station %s:%d version changed, reloading...\n",
					inet_ntoa(activeConnection->peer.sin_addr),ntohs(activeConnection->peer.sin_port));
				/* upload controller configuration */
				if(F8_SUCCESS == uploadVariableTable()){
					uploadConfiguration();
				}
			}
		}else{
			utils_error("F8 - station %s:%d not available[state=%d,synchronized=%s], finding alternatives...\n",
				inet_ntoa(activeConnection->peer.sin_addr),ntohs(activeConnection->peer.sin_port),
				st.state,st.flags&(1<<FKERN_LED_SYNCHRONIZED)?"true":"false");
			__vcall__(adapter,offline,(adapter));
		}
	}

	if(!(adapter->flags & ADAPTER_F_ONLINE)){
		// connection cannot establish
		if(activeConnection == connections+3){
			activeConnection=connections;
		}else{
			activeConnection++;
		}
		return false;
	}
	
	return true;
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
f8_status CF8Device::uploadConfiguration()
{
	if(!(adapter->flags & ADAPTER_F_ONLINE)){
		return F8_ADAPTER_OFFLINE;
	}
	Lock();

	// check version
	struct kern_conf_cmd cmd;
	struct kstatus_t st;
	memset(&cmd, 0, sizeof(cmd));
	cmd.cmd = F8_GET_STATUS;
	cmd.isize = 0;
	cmd.obuffer = (char *)&st;
	cmd.osize = sizeof(st);
	__vcall__(adapter, exec_remote, (adapter, &cmd, 10000));
    /*
	(   _CObj_CheckPureVCall(
		(void*)(((adapter)->vtbl)->exec_remote), 
        #exec_remote, 
		__FILE__, __LINE__),
		((adapter)->vtbl)->exec_remote)(adapter, &cmd, 10000);
	*/

	if(!F8_SUCCESSED(cmd.retcode)){
		return cmd.retcode;
	}

	memset(&cmd, 0, sizeof(cmd));
	cmd.cmd = F8_GET_SYS_CONFIG;
	cmd.isize = 0;
	cmd.obuffer = (char *)&ctrllerConfig;
	cmd.osize = sizeof(ctrllerConfig);
	__vcall__(adapter, exec_remote, (adapter, &cmd, 10000));
	if(!F8_SUCCESSED(cmd.retcode)){
		return cmd.retcode;
	}
	cmd.cmd = F8_SET_SYS_CONFIG;
	cmd.isize = sizeof(ctrllerConfig);
	cmd.ibuffer = (char *)&ctrllerConfig;
	cmd.osize = 0;
	__vcall__(adapter, exec_local, (adapter, &cmd));

	tstatus = st;
	/* relocate memory pointers */
	for(int i=0; i<KERN_NUM_SECTIONS; i++){
		xMemPointers[i] = ke_get_section(kernel, i);
	}
	
	UnLock();
	return F8_SUCCESS;
}

/* 
 * 函数功能:由设备信息得到RTK_TAG，并translate()
 * 参数说明：
 * 返回值：
 */
void CF8Device::validateVariables()
{
	if(!lock_rtdb(__true, 1000)){
		return;
	}
	PRTK_LIST_ENTRY li;
	PRTK_TAG tte;
	li = device->tags.Flink;
	while(li != &device->tags){
		tte = RTK_CONTAINING_RECORD(li, RTK_TAG, d.DeviceLink);
		/*由RTK_LIST_ENTRY反推出RTK_TAG的地址
		tte = (RTK_TAG*)((char*)li - (int)(int*)&((RTK_TAG*)0->d.DeviceLink));
		*/
		translate(tte);
		li = li->Flink;
	}
	unlock_rtdb();
}

/* 
 * 函数功能:刷数
 * 参数说明：
 * 返回值：
 */
f8_status CF8Device::uploadVariableTable()
{
	if(!(adapter->flags & ADAPTER_F_ONLINE)){
		return F8_ADAPTER_OFFLINE;
	}

	memset(workBuf, 0, sizeof(blk_var_t)*PMCF8_MAX_VARIABLES);
	
	struct kern_conf_cmd cmd;
	memset(&cmd, 0, sizeof(cmd));
	cmd.cmd = F8_UPLOAD_VARTABLE;
	cmd.isize = 0;
	cmd.obuffer = workBuf;
	cmd.osize = sizeof(blk_var_t)*PMCF8_MAX_VARIABLES;
	__vcall__(adapter, exec_remote, (adapter, &cmd, 10000));

	int count;
	if(!F8_SUCCESSED(cmd.retcode)){
		return cmd.retcode;
	}
	count = cmd.osize / sizeof(blk_var_t);
	int i;
	blk_var_t * v;

	utils_debug("F8 - %d variables uploaded.\n",count);
	{
		char path[256];
		sprintf(path,"%s/vars.bin",get_working_dir());
		FILE *fp=fopen(path,"wb");
		if(fp){
			fwrite(workBuf,1,cmd.osize,fp);//将控制器中刷上来的值备份到vars.bin
			fclose(fp);
		}
	}
	
	if(!lock_rtdb(__true,1000))
		return F8_ACCESS;
	ke_clear_hash(idIndex);
	memcpy(variables,workBuf,sizeof(variables));
	for(i=0, v=variables; i<count; i++, v++){
		nameIndex[i] = i;
		ke_set_hash(idIndex, &v->uuid, v);
	}
	qsort_r(nameIndex, count, sizeof(nameIndex[0]), compareName, variables);
	varCount=count;
	unlock_rtdb();

	// utils_debug("%d variables reloaded.\n", count);
#if 0
	for(i=0;i<varCount;i++)
		utils_debug("var:%s\n",workBuf[nameIndex[i]].name);
#endif
	/* validate all variable definitions */
	validateVariables();
	
	return F8_SUCCESS;
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
/*
	1) periodicly verify connection
	2) synchronize variable table
	3) perform device write
*/
__uint CF8Device::run()
{
	MSG msg;
	__bool bExit = __false;
	HANDLE hStop;

	/* establish connection */
	IKAdapter_begin_transfer(adapter);

	hStop = m_EvtStop.Handle();
	while(!bExit){
		if(checkConnection())
			failCount=0;
		else{
			failCount++;
			if(failCount>=maxFailCount)
				failCount=maxFailCount;
		}
		switch(MsgWaitForMultipleObjects(1, &hStop, TRUE, 2000, QS_POSTMESSAGE)){
		case WAIT_OBJECT_0:
			bExit = __true;
			break;
		default:
			while(PeekMessage(&msg, 0, 0, 0, TRUE)){
				switch(msg.message){
				case WM_QUIT:
					bExit = __true;
					break;
				case WM_USER:
					/* write device */
					break;
				}
			}
		}
	}

	__vcall__(adapter, offline, (adapter));
	
	return 0;
}

/* 
 * 函数功能:由RTK_TAG关联到设备驱动中的变量variables[]
 * 参数说明：
 * 返回值：
 */
__bool CF8Device::translate(PRTK_TAG tag)
{
	struct var_def_t * def = (struct var_def_t * )tag->d.BinaryAddress;
	const struct blk_var_t * v;
	char name[64];
	int * vv;
	tag->d.Value.Flags &= ~TF_Translated;
	if(!tag->s.Address[0]){
		strncpy(name,(char*)CTagName(tag->key),sizeof(name));
	}else{
		strncpy(name,tag->s.Address,sizeof(name));
	}
	name[sizeof(name)-1]=0;
	// utils_debug("Searching %s\n", name);
	vv = (int *)bsearch_r(
		name, 
		nameIndex, 
		varCount, 
		sizeof(nameIndex[0]), 
		compareNameIndex, 
		variables
		);
	if(!vv){
		return __false;
	}
	v = &variables[*vv];
	if(v->addr.section >= KERN_NUM_SECTIONS){
		utils_debug("F8->%s,section %d,addr %d invalid\n",name,v->addr.section,v->addr.addr);
		return __false;
	}
	def->uuid = v->uuid;
	def->addr = v->addr;
	tag->d.Value.Flags |= TF_Translated;
	return __true;
}

/* 
 * 函数功能:取出镜像中指定标签的值
 * 参数说明：[in]tte     [out]pt    tm木有用。。
 * 返回值：
 */
void CF8Device::update(PCRTK_TAG tte, pmc_value_t * pt, PCRTK_TIME tm)
{
	struct dyn_pin_t * dp;
	struct blk_var_t * v;
	struct var_def_t * def;
	struct pmc_value_t foo;

	if(!(adapter->flags & ADAPTER_F_ONLINE) && failCount>=maxFailCount){
		return;
	}

	//if(!verMatch){
	//	pt->Flags |= TF_Valid;
	//	return;
	//}

	def = (var_def_t*)tte->d.BinaryAddress;
	dp = (struct dyn_pin_t*)(xMemPointers[def->addr.section] + def->addr.addr);
	v = (blk_var_t*)ke_get_hash(idIndex, &def->uuid);
	if(!v){
		return;
	}
	memset(&foo, 0, sizeof(foo));
	switch(v->type){
	case PIN_T_BOOL:
		foo.Value.b = dp->u.d.i8? __true : __false;
		set_value_type(foo.Flags, dt_bool);
		break;
	case PIN_T_SHORT:
		foo.Value.i16 = dp->u.d.i16;
		set_value_type(foo.Flags, dt_int16);
		break;
	case PIN_T_BYTE:
		foo.Value.u8 = dp->u.d.ui8;
		set_value_type(foo.Flags, dt_uint8);
		break;
	case PIN_T_DWORD:
		foo.Value.u32 = dp->u.d.ui32;
		set_value_type(foo.Flags, dt_uint);
		break;
	case PIN_T_WORD:
		foo.Value.u16 = dp->u.d.ui16;
		set_value_type(foo.Flags, dt_uint16);
		break;
	case PIN_T_INTEGER:
		foo.Value.i32 = dp->u.d.i32;
		set_value_type(foo.Flags, dt_int);
		break;
	case PIN_T_CHAR:
		foo.Value.i8 = dp->u.d.i8;
		set_value_type(foo.Flags, dt_int8);
		break;
	case PIN_T_FLOAT:
		foo.Value.flt = dp->u.d.flt;
		set_value_type(foo.Flags, dt_real4);
		break;
	case PIN_T_DOUBLE:
		foo.Value.dbl = dp->u.d.dbl;
		set_value_type(foo.Flags, dt_real8);
		break;
	case PIN_T_DATE:
		foo.Value.dbl = dp->u.d.dbl;
		set_value_type(foo.Flags, dt_date);
		break;
	default:
		return;
	}

	set_value_type(pt->Flags, get_value_type(tte->s.Flags));
	pmc_type_cast(&foo, pt);
	pt->Flags |= TF_Valid;
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
static __bool value_to_f8_pin(
	int requestedType, 
	dyn_pin_t * p, 
	const pmc_value_t * val
	)
{
	pmc_value_t v2;
	switch(requestedType){
	case PIN_T_INTEGER:
		set_value_type(v2.Flags, dt_int);
		break;
	case PIN_T_BOOL	:
		set_value_type(v2.Flags, dt_bool);
		break;
	case PIN_T_FLOAT	:
		set_value_type(v2.Flags, dt_real4);
		break;
	case PIN_T_BYTE	:
		set_value_type(v2.Flags, dt_uint8);
		break;
	case PIN_T_WORD	:
		set_value_type(v2.Flags, dt_uint16);
		break;
	case PIN_T_DOUBLE:
		set_value_type(v2.Flags, dt_real8);
		break;
	case PIN_T_DATE	:
		set_value_type(v2.Flags, dt_date);
		break;
	case PIN_T_CHAR	:
		set_value_type(v2.Flags, dt_int8);
		break;
	case PIN_T_SHORT	:
		set_value_type(v2.Flags, dt_int16);
		break;
	case PIN_T_DWORD	:
		set_value_type(v2.Flags, dt_uint32);
		break;
	default:
		return __false;
	}
	if(get_value_type(v2.Flags) != get_value_type(val->Flags)){
		pmc_type_cast(val, &v2);
		val = &v2;
	}
	switch(requestedType){
	case PIN_T_INTEGER:
		p->u.d.i32 = val->Value.i;
		break;
	case PIN_T_BOOL	:
		p->u.d.i8 = val->Value.i8;
		break;
	case PIN_T_FLOAT	:
		p->u.d.flt = val->Value.flt;
		break;
	case PIN_T_BYTE	:
		p->u.d.ui8 = val->Value.u8;
		break;
	case PIN_T_WORD	:
		p->u.d.ui16 = val->Value.u16;
		break;
	case PIN_T_DOUBLE:
		p->u.d.dbl = val->Value.dbl;
		break;
	case PIN_T_DATE	:
		p->u.d.dbl = val->Value.dbl;
		break;
	case PIN_T_CHAR	:
		p->u.d.i8 = val->Value.i8;
		break;
	case PIN_T_SHORT	:
		p->u.d.i16 = val->Value.i16;
		break;
	case PIN_T_DWORD	:
		p->u.d.ui32 = val->Value.u32;
		break;
	default:
		return __false;
	}

	return __true;
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
static __uint _type_size(__u8 type)
{
	switch(type){
	case PIN_T_INTEGER:
		return 4;
	case PIN_T_BOOL:
		return 1;
	case PIN_T_FLOAT:
		return 4;
	case PIN_T_TIMER:
		return 8;
	case PIN_T_BYTE:
		return 1;
	case PIN_T_WORD:
		return 2;
	case PIN_T_DOUBLE:
		return 8;
	case PIN_T_DATE:
		return 8;
	case PIN_T_CHAR:
		return 1;
	case PIN_T_SHORT:
		return 2;
	case PIN_T_DWORD:
		return 4;
	}
	return (__uint)-1;
}

/* 
 * 函数功能:指定标签的值写入镜像，并发送到控制器
 * 参数说明：
 * 返回值：
 */
__bool CF8Device::write(PRTK_TAG tte, PCTAG_VALUE value)
{
	if(isStandby){
		return __false;
	}

	if(!verMatch){
		return __false;
	}

	struct dyn_pin_t * dp;
	struct blk_var_t * v;
	struct var_def_t * def;
	struct kern_conf_cmd cmd;
	struct conf_mod_mem mp;
	IBlk * sections;

	if(!(adapter->flags & ADAPTER_F_ONLINE)){
		return __false;
	}

	def = (var_def_t*)tte->d.BinaryAddress;
	dp = (struct dyn_pin_t*)(xMemPointers[def->addr.section] + def->addr.addr);
	v = (blk_var_t*)ke_get_hash(idIndex, &def->uuid);
	if(!v){
		return __false;
	}
	
	ZeroMemory(&cmd, sizeof(cmd));
	cmd.cmd = F8_SET_MEM_VALUE;
	sections = ke_get_blk(kernel, 0);
	OLD_ID(&cmd)=tstatus.prog_id;
	NEW_ID(&cmd)=BLK_ID_NONE;
	mp.addr = v->addr;
	mp.len = _type_size(v->type);
	mp.value.u.d.ui64 = 0; 

	// cast value into F8 type
	pmc_value_t vv;
	vv.Flags = tte->d.Value.Flags;
	vv.Value = *value;
	//将实时库数据类型转化为控制器数据类型
	//vv --> mp.value
	if(!value_to_f8_pin(v->type, &mp.value, &vv)){
		return __false;
	}
    
	//mp -->cmd
	cmd.ibuffer = &mp;
	cmd.isize = sizeof(mp);
	//将写入的值发送给控制器
	__vcall__(
		adapter, 
		exec_remote, 
		(adapter, &cmd, 500)
		);//发送并接受了数据包
	/*ITcpAdapter_exec_remote()*/

	return F8_SUCCESSED(cmd.retcode);
}

/* 
 * 函数功能:指定标签的值写入镜像，并发送到控制器
 * 参数说明：
 * 返回值：
 */
IOSS_DLL_EXPORT __bool write_device(PRTK_TAG tte, PCTAG_VALUE value)
{
	struct CF8Device * f;
	f = (CF8Device*)tte->d.DeviceObj->OwnerField;
	return f->write(tte, value);
}

/* 
 * 函数功能:取出镜像中指定标签的值
 * 参数说明：[in]tte     [out]pt    tm木有用。。
 * 返回值：
 */
IOSS_DLL_EXPORT void update_tag(PCRTK_TAG tte, pmc_value_t * pt, PCRTK_TIME tm)
{
	struct CF8Device * f;
	f = (CF8Device*)tte->d.DeviceObj->OwnerField;
	f->update(tte, pt, tm);
}

/* 
 * 函数功能：
 * 参数说明：
 * 返回值：
 */
IOSS_DLL_EXPORT __bool load(PDRIVER_INFO driverObj)
{
	return __true;
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
IOSS_DLL_EXPORT __bool unload()
{
	return __true;
}

/* 
 * 函数功能:根据设备信息，开辟一个镜像，并开启一个线程用于镜像与控制器的通讯
 * 参数说明：
 * 返回值：
 */
/*
	parameter spec : 
*/
IOSS_DLL_EXPORT __bool start_device(PDEVICE_INFO h)
{
	struct CF8Device * devObj;
	
	/* create the device object and associate it with the ioss created one */
	devObj = new CF8Device;
	if(!devObj->config(h)){
		delete devObj;
		return __false;
	}

	// ok, start the thread
	if(!devObj->start()){
		delete devObj;
		return __false;
	}

	return __true;
}

/* 
 * 函数功能:根据设备信息，删除镜像，关闭线程
 * 参数说明：
 * 返回值：
 */
IOSS_DLL_EXPORT __bool stop_device(PDEVICE_INFO h)
{
	struct CF8Device * devObj;
	devObj = (struct CF8Device*)h->OwnerField;

	if(!devObj){
		return __false;
	}
	
	if(devObj->stop(5000)){
		delete devObj;
		return __true;
	}

	return __false;
}

/* 
 * 函数功能:translate标签，由镜像中的内容 改变指定标签(RTK_TAG)的属性, 并未发送数据报
 * 参数说明：
 * 返回值：
 */
/*
	address is the variable name, only global variables are accessible.
*/
IOSS_DLL_EXPORT IOSS_STATUS PMC_API dispatch(
	PDEVICE_INFO device,
	int majorCode,
	int param
	)
{
	PRTK_TAG tte;
	CF8Device * dev;

	dev = (struct CF8Device *)device->OwnerField;
	
	switch(majorCode){
	case DISP_DB_ADD_TAG:
		tte = (PRTK_TAG)param;
		assert(dev == (struct CF8Device *)tte->d.DeviceObj->OwnerField);
		if(!dev){
			return IOSS_FAILED;
		}
		dev->translate(tte);
		break;
	case DISP_POWER_STATE_CHANGED:
		if(get_power_state() != PWR_RUNNING){
			//IKAdapter_pause_transfer(dev->adapter);
		}else{
			//IKAdapter_resume_transfer(dev->adapter);
		}
		break;
	}

	return IOSS_SUCCESS;
}

static bool bExit;
static DEVICE_INFO info;
static HRTK_SHELL shell;

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
int on_exit(char *c,void *k)
{
	bExit=true;
	return 0;
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
static struct shell_command f8test_commands[]={
	{"q", "exit test.", on_exit},
};

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
static bool initTest()
{
	WSADATA data;

	WSAStartup(0x202,&data);
	set_power_state(PWR_RUNNING);
	shell=shell_create(f8test_commands);

	RtkInitializeListHead(&info.tags);

	init_rtdb();

	if(!start_device(&info)){
		return false;
	}

	return true;
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
static void uninitTest()
{
	stop_device(&info);
	uninit_rtdb();
	WSACleanup();	
}

/* 
 * 函数功能:
 * 参数说明：
 * 返回值：
 */
extern "C" __declspec(dllexport) int pmcf8_test(HINSTANCE h,int,const char * cmdLine,int)
{
	CArgs a;
	char * deviceName="f8";
	char configLine[1024];

	BEGIN_ARGS
		ARG("-d:",deviceName,"char *","device name entry in config.ini")
	END_ARGS

	
	utils_debug("cmdline is '%s'\n",cmdLine);
	
	parse_arg_ex(cmdLine,&a," \t\n");
	if(process_args_0(a.argc,a.argv) < 0){
		print_usage_1();
		return -1;
	}

	utils_debug("Loading setting for device '%s' from %s\n",deviceName,get_config_file());
	
	GetPrivateProfileString("devices",deviceName,"",configLine,sizeof(configLine),get_config_file());
	if(!*configLine){
		utils_error("'%s' is not valid configuration\n",configLine);
		return -2;
	}
	parse_arg_ex(configLine,&a,",");
	if(a.argc < 2){
		utils_error("'%s' is not valid configuration\n",configLine);
		return -2;
	}
	strcpy(info.address,a.argv[1]);
	strcpy(info.parameter,a.argv[2]);

	initTest();
	
	while(!bExit){
		char line[1024];
		int code;
		utils_trace("# ");
		utils_gets(line,sizeof(line));
		if(!*line)
			continue;
		shell_execute_ex(shell,line,0,&code);
	}

	uninitTest();
	
	return 0;
}
