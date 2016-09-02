/*
	Modicon Quantum-TX device driver
	by J.Hunter,Jul.,2001
*/
#include "precomp.h"

#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "libspac.lib")
#pragma comment(lib, "libpowermgr.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "Ws2_32.lib")

static void _dump(char * buf, int l)
{
	int i,j;
	char  * p;

	for(i=0, p=buf; i<(l>>4); i++){
		for(j=0; j<16; j++, p++){
			*p=='\r'? 0:utils_debug("%c ",(unsigned)*p);
		}
		utils_debug("\n");
	}

	for(i=0;i<(l%16);i++,p++){
		*p=='\r'? 0:utils_debug("%c ",(unsigned)*p);
	}
	
	utils_debug("\n");
}

/*
二进制地址格式：
开关量：
	BinaryAddress[0,1]:区号
	BinaryAddress[3,4]:位地址
模拟量:
	common:
		BinaryAddress[0, 1]:区号
		BinaryAddress[3, 4]:寄存器地址
		BinaryAddress[6-9]:fa
		BinaryAddress[10-13]:fb
*/
struct hw_addr_t{
	__u16  section;
	__u16  reg_bit;
	r4i32_t fa;
	r4i32_t fb;
};

struct write_device_parameter{
    class CModiconDevice	*d;
    PRTK_TAG	tte;
    TAG_VALUE tv;
};

class CWriter : public CRtkThread
{
public:
	CWriter()
	{
	}
	virtual __uint run();
	int WriteDevice(write_device_parameter * wdp);
};

class CModiconDevice : public CPeriodicThread{
public:
	modicon m;
	CWriter writer;
	int connection_dog_count;

	CModiconDevice()
	{
		m_dwPeriod = 150;
	}
	__bool init(char*, char*);
	virtual void task();
};

__bool CModiconDevice::init(char * address, char * parameter)
{
	CArgs args;
	int  c0,c1,c3,c4,i;
	
	m_dwPeriod = DEFAULT_ACQ_DELAY;
	parse_arg_ex(parameter, &args, ",");
	
	if(args.argc < 4){
		utils_error(
			">> Error : modicon, %s, invalid configuration.\n", 
			parameter
			);
		return 0;
	}
	c0=atoi(args.argv[0]);
	c1=atoi(args.argv[1]);
	c3=atoi(args.argv[2]);
	c4=atoi(args.argv[3]);
	
	m.m_ConTimeout = GetPrivateProfileInt(
		"Modicon",
		"CTO",
		DEFAULT_CONN_TIMEOUT,
		get_config_file()
		);
	m.m_ReadTimeout = GetPrivateProfileInt(
		"Modicon",
		"RTO",
		DEFAULT_READ_TIMEOUT,
		get_config_file()
		);
	m.m_MaxFails = GetPrivateProfileInt(
		"Modicon",
		"MaxFail",
		DEFAULT_MAX_FAILS,
		get_config_file()
		);
	m.m_batchCount = GetPrivateProfileInt(
		"Modicon",
		"BatchCount",
		DEFAULT_BATCH_COUNT,
		get_config_file()
		);
	for(i=4; i<args.argc; i++){
		if(!strnicmp(args.argv[i], "CTO=", 4)){
			m.m_ConTimeout = atoi(args.argv[i]+4);
		}else if(!strnicmp(args.argv[i], "RTO=", 4)){
			m.m_ReadTimeout = atoi(args.argv[i]+4);
		}else if(!strnicmp(args.argv[i], "MaxFail=", 8)){
			m.m_MaxFails = atoi(args.argv[i]+8);
		}else if(!strnicmp(args.argv[i], "Delay=", 6)){
			m_dwPeriod = atoi(args.argv[i]+6);
		}else if(!strnicmp(args.argv[i], "Batch=", 6)){
			m.m_batchCount = atoi(args.argv[i] + 6);
		}else if(!stricmp(args.argv[i], "debug")){
			m.debug = true;
		}
	}

	// 3区和4区的寄存器个数必须是偶数
	if(c3&1){
		utils_error(
			">> Warning(modicon) : %s, invalid configuration.\n", 
			parameter
			);
		c3++;
	}
	if(c4&1){
		utils_error(
			">> Warning(modicon) : %s, invalid configuration.\n", 
			parameter
			);
		c4++;
	}
	
	/*
		device address must be ';' instead of ',' segmented as for
		2003/5/30
	*/
	parse_arg_ex(address, &args, ";,");

	__bool ret = m.construct(args.argv[0], args.argv[1], c0, c1, c3, c4);

	utils_trace(
		">> Modicon device @ %s(%s)\n"
		">> ConfigString : %d,%d,%d,%d,CTO=%d, RTO=%d, MaxFail=%d, Batch=%d\n",
		args.argv[0],
		args.argv[1][0]? args.argv[1] : "N.A.",
		c0, c1, c3, c4,
		m.m_ConTimeout,
		m.m_ReadTimeout,
		m.m_MaxFails,
		m.m_batchCount
		);
	connection_dog_count = 32768;
	return ret;
}

void CModiconDevice::task()
{
	if(get_power_state() != PWR_RUNNING){
		return;
	}
	m.update_h_cache();
	connection_dog_count += m_dwPeriod;
	// build connections every 10 seconds
	if(connection_dog_count > 32768){
		m.build_conections();
		connection_dog_count = connection_dog_count % 32768;
	}
}

static modicon * _modicon_by_tag(PCRTK_TAG t)
{
	PDEVICE_INFO dev;
	CModiconDevice * d;
	
	dev = (PDEVICE_INFO)t->d.DeviceObj;
	d = (CModiconDevice *)dev->OwnerField;
	if(d){
		return &d->m;
	}
	return 0;
}

__uint CWriter::run()
{
	write_device_parameter *wdp;

	MSG msg;	
	while(GetMessage(&msg, NULL, 0, 0)){
		if(msg.message == WM_USER+426){
			wdp = (write_device_parameter*)msg.wParam;
			WriteDevice(wdp);
			delete wdp;
		}			 
	}		 
	
	return 1;
}

int CWriter::WriteDevice(write_device_parameter * wdp)
{
	int ret=false;
	modicon * m;
	PRTK_TAG tte = wdp->tte;
	PTAG_VALUE tv = &wdp->tv;
	struct hw_addr_t * addr;
	TAG_VALUE v;
	TAG_TYPE tp;

	m = _modicon_by_tag(wdp->tte);
	if(!m){
		return 0;
	}

	addr = (struct hw_addr_t*)tte->d.BinaryAddress;

	tp = get_value_type(tte->s.Flags);
	if(addr->section == 4){ 
		switch(tp){
		case dt_real4:
			if(fabs(addr->fa.flt) > 1e-5){
				v.flt = (tv->flt - addr->fb.flt) / addr->fa.flt;
			}else{
				v.flt = tv->flt;
			}
			break;
		case dt_int16:
			if(addr->fa.i != 0){
				v.i16 = (tv->i16 - addr->fb.i) / addr->fa.i;
			}else{
				v.i16 = tv->i16;
			}
			break;
		case dt_int32:
			if(addr->fa.i != 0){
				v.i32 = (tv->i32 - addr->fb.i) / addr->fa.i;
			}else{
				v.i32 = tv->i32;
			}
			break;
		case dt_bool:
			v.b = tv->b? 1 : 0;
			break;
		default:
			assert(0);
			break;
		}
		
		if(m->write_register(addr->reg_bit,tp, v)){
			ret = true;
		}
	}else if(addr->section == 0){
		if(m->write_coil(addr->reg_bit, tv->b)){
			ret = true;
		}
	}else{
		assert(0);
	}
	
	return ret;
}

static __bool translateConceptAddress(
	modicon * m, PRTK_TAG tte, hw_addr_t * addr
	)
{
	char section;
	TAG_TYPE tp;
	
	section = tte->s.Address[0];
	if(!strchr("0134", section)){
		return __false;
	}
	section -= '0';
	addr->section = section;
	
	if(tte->s.Address[1]){
		// yes, there're some remaining bytes
		// actually we place less constraints than concept on
		// the address definition, in concept the '3' is not a 
		// good address, but we accept it and take it as
		// '30'
		addr->reg_bit = atoi(tte->s.Address + 1);
	}

	tp = get_value_type(tte->s.Flags);
	switch(addr->section){
		case 0:
		case 1:
			if(tp != dt_bool){
				return __false;
			}
			if(addr->reg_bit > m->c[addr->section] * 16){
				return __false;
			}
			break;
		case 3:
		case 4:
			if(addr->reg_bit > m->c[addr->section-1]){
				return __false;
			}
			break;
		default:
			assert(0);
	}
	
	if(addr->section == 1 || addr->section == 3){
		tte->d.Value.Flags |= TF_ReadOnly;
	}else{
		tte->d.Value.Flags &= ~TF_ReadOnly;
	}
	
	return __true;
}

static __bool translatePmc10Address(
	modicon	* m, PRTK_TAG tte, hw_addr_t * addr
	)
{
	CArgs argAll, args, argAb;
	TAG_TYPE tp;
	
	parse_arg_ex(tte->s.Address,&argAll, " ");
	parse_arg_ex(argAll.argv[0], &args, ";.,-=!");

	int  z,r;
	float fa = 1., fb = 0.;

	if(args.argc < 2){
		utils_error(">> Modicon, invalid address : %s\n",tte->s.Address);
		return 0;
	}
	
	z = atoi(args.argv[0]);
	r = atoi(args.argv[1]);

	if(z!=0 && z!=1 && z!=3 && z!=4){
		return 0;
	}	

	tp = get_value_type(tte->s.Flags);
	
	if(tp != dt_bool 
		&& tp != dt_real4 
		&& tp != dt_int16
		&& tp != dt_int32
	){
		/* data type not supported yet */
		return __false;
	}
	
	addr->section = z;
	addr->reg_bit = r;
	if(z == 3 || z == 4){
		if(!(r>0 && r<=m->c[z-1])){
			return __false;
		}
		
		if(argAll.argc > 1 && tp != dt_bool){
			//got ax+b
			parse_arg_ex(argAll.argv[1],&argAb,"/");
			if(argAb.argc > 2){
				switch(tp){
				case dt_real4:
					addr->fa.flt=(__r4)atof(argAb.argv[0]);
					addr->fb.flt=(__r4)atof(argAb.argv[1]);
					break;
				case dt_int16:
				case dt_int32:
					addr->fa.i = atoi(argAb.argv[0]);
					addr->fb.i = atoi(argAb.argv[1]);
					break;
				}
			}
		}
	}else if (z == 0 || z == 1){
		if(tp != dt_bool){
			return __false;
		}
		
		// 可以是0/1，并且位地址不能超过组态时指定的最大寄存器个数
		if( r > 0 && r <= m->c[z] * 16){
			addr->section = z;
			addr->reg_bit = r;
		}else{
			return __false;
		}
	}else{
		/* invalid section */
		return __false;
	}

	if(addr->section == 1 || addr->section == 3){
		tte->d.Value.Flags |= TF_ReadOnly;
	}else{
		tte->d.Value.Flags &= ~TF_ReadOnly;
	}
	
	return __true;
}

/*****************************************************************************

	将文本格式的地址信息翻译成二进制格式
	
	文本地址格式：
	开关量： <region>.<bit_offset>
	注意为了和Modibus协议一致，这里的bit_offset从1开始编号
	模拟量：
		3x: <region>.<register_offset> [a/b]
		4x: <region>.<register_offset> [a/b]
	另外一种和concept兼容的格式也支持
	即 : ("%0*d", n) n=2..6

*****************************************************************************/
IOSS_DLL_EXPORT __bool address_translate(PRTK_TAG tte)
{
	modicon	* m;
	struct hw_addr_t * addr;
	__bool ok;

	m = _modicon_by_tag(tte);
	
	if(!m){
		return __false;
	}

	addr = (struct hw_addr_t*)tte->d.BinaryAddress;
	memset(addr, 0, sizeof(hw_addr_t));
	
	char * p;
	p = tte->s.Address;
	while(*p++){
		if(!isdigit(*p)){
			break;
		}
	}

	if(*p){
		// pmc 1.0 address
		ok = translatePmc10Address(m, tte, addr);
	}else{
		ok = translateConceptAddress(m, tte, addr);
	}

	return ok;
}

/***************************************************************************
	从硬件缓冲区中得到变量的工程值
	输入：
	ni	-->  NPU名
	tte -->  变量
	he	-->  硬件缓冲区

	returns :
	
	true if the tag has been sucessfully updated, only returns false in case
	of unrecoverable fault.

	NOTE : you should always return true in this function, even if the tag
	is physically invalid(e.g., the containing IO station is powered off)
***************************************************************************/
IOSS_DLL_EXPORT void update_tag(PCRTK_TAG tte, pmc_value_t * pt, PCRTK_TIME)
{
	struct hw_addr_t * addr;
	modicon * m;
	TAG_TYPE tp;
	
	m = _modicon_by_tag(tte);
	if(!m){
		return;
	}

	memset(pt, 0, sizeof(pmc_value_t));
	
	addr = (struct hw_addr_t*)tte->d.BinaryAddress;

	tp = get_value_type(tte->s.Flags);
	
	switch(tp){
	case dt_real4:
	case dt_int16:
	case dt_int32:
		if(m->read_analog(addr->section, addr->reg_bit, tp, &pt->Value)){
			pt->Flags |= TF_Valid;
		}
		break;
		
	case dt_bool:
		if(m->read_digital(addr->section, addr->reg_bit, &pt->Value.b)){
			pt->Flags |= TF_Valid;
		}
		break;
		
	default:
		/* other types not supported */
		pt->Flags &= ~TF_Valid;
		break;
	}

	// scale value if nessesary
	if(addr->fa.i != 0 && (pt->Flags & TF_Valid)){
		switch(tp){
		case dt_real4:
			pt->Value.flt = pt->Value.flt * addr->fa.flt + addr->fb.flt;
			break;
		case dt_int16:
			pt->Value.i16 = pt->Value.i16 * addr->fa.i + addr->fb.i;
			break;
		case dt_int32:
			pt->Value.i32 = pt->Value.i32 * addr->fa.i + addr->fb.i;
			break;
		}
	}
}

/*
	创建一个modicon设备对象
	handle->parameter指定了该设备的启动参数
	handle->address指定了该设备的地址
*/
IOSS_DLL_EXPORT __bool start_device(PDEVICE_INFO handle)
{
	CModiconDevice * d;
	d = new CModiconDevice();
	if(!d){
		utils_error("Error(modicon): allocating modicon object.\n");
		return 0;
	}

	// ok,创建modicon对象
	if(!d->init(handle->address, handle->parameter)){
		utils_error(
			">>Error(modicon) creating modbustcp object on %s.\n", 
			handle->address
			);
		delete d;
		return 0;
	}

	// ok, start the thread
	handle->OwnerField = d;

	if(!d->start()){
		delete d;
		return 0;
	}

	if(!d->writer.start()){
		utils_error(">> Oops!!! cannot start Modicon writer.\n");
		return 0;
	}

	return 1;
}

IOSS_DLL_EXPORT __bool stop_device(PDEVICE_INFO handle)
{
	CModiconDevice * dev;
	dev = (CModiconDevice *)handle->OwnerField;

	if(!dev){
		return 0;
	}

	dev->writer.stop();

	if(!dev->stop(3000)){
		utils_error(
			"Warning : modicon device on '%s' didn't stop in 3 seconds.\n",
			handle->address
			);
	}else{		
		delete dev;
	}	

	return 1;
}

IOSS_DLL_EXPORT __bool write_device(PRTK_TAG tte, PCTAG_VALUE value)
{
	write_device_parameter * wdp;
	wdp=new write_device_parameter;
	if(!wdp){
		DEBUG_PRINTF((">> modicon , Shit!! error new write_device_parameter\n"));
		return 0;
	}
	wdp->tte=tte;
	wdp->tv=*value;

	PDEVICE_INFO dev;
	dev = (PDEVICE_INFO)tte->d.DeviceObj;

	CModiconDevice * d;
	d = (CModiconDevice *)dev->OwnerField;

	__bool ret = d->writer.post_message(WM_USER+426, (WPARAM)wdp, 0);
	if(!ret){
		delete wdp;
	}
	return ret;
}
