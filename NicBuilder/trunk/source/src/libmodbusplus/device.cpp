/*
	Modicon Quantum-TX device driver
	by J.Hunter,Jul.,2001
*/
#include "precomp.h"

#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "libspac.lib")
#pragma comment(lib, "libpowermgr.lib")
#pragma comment(lib, "librtk.lib")

class CModiconDevice : public CPeriodicThread{
public:
	modicon m;
	CModiconDevice(__uint delay)
	{
		m_dwPeriod = delay;
	}
	__bool init(int,int,int,int,int);
	virtual void task();
};

struct write_device_parameter{
    CModiconDevice	*d;
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
	int WriteDevice(write_device_parameter * wdp, MBXPATHID path);
};

static HANDLE g_hModicon;
static CWriter * g_Writer;

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

static void _add_lrc_and_cr(char * buf,int len)
{
	int i,A;
	if(len<2) return;
	A = 0;
	for(i=1;i<len;i++){
	   A = A ^ buf[i];
	};
	sprintf(buf+len,"%02X",A);
	len+=2;
	buf[len++]=13;
	buf[len++]=0;
}

static modicon * _modicon_by_tag(PCRTK_TAG t)
{
	PDEVICE_INFO dev;
	CModiconDevice * d;
	
	__try{
		dev = (PDEVICE_INFO)t->d.DeviceObj;
		d = (CModiconDevice *)dev->OwnerField;
		return &d->m;
	}__except(EXCEPTION_CONTINUE_EXECUTION){ /* __except(utils_exception_filter()){*/
	}

	return 0;
}

__bool CModiconDevice::init(int did, int c0, int c1, int c3, int c4)
{
	return m.construct(did, c0, c1, c3, c4);
}

void CModiconDevice::task()
{
	CModiconDevice * ap=this;
	bool stopping=false;
	// 这两个变量分别指示当前要写入读数据命令和返回数据的前端
	int   current_modicon=0;
	MBXPATHID path;
	int  code;

	if(get_power_state() != PWR_RUNNING){
		return;
	}
	path.wPath=0;
	path.wPathType=0;

	if(code = MbxOpenMp(g_hModicon,&path)){
		utils_error(">> modicon : MbxOpenMp failed with %d.\n",code);
		delete ap;
	}

	m.h_device=g_hModicon;
	m.path=path;
	
	m.flush_h_buffer();
	
	MbxCloseMp(g_hModicon,path);
}

__uint CWriter::run()
{
	MBXPATHID path;
	int  code;
	write_device_parameter *wdp;

	path.wPath=0;
	path.wPathType=0;

	if(code = MbxOpenMp(g_hModicon,&path)){
		utils_error(">> modicon : MbxOpenMp failed with %d.\n",code);
		return 0;
	}

	MSG msg;	
	while(GetMessage(&msg, NULL, 0, 0)){
		if(msg.message == WM_USER+426){
			wdp = (write_device_parameter*)msg.wParam;
			WriteDevice(wdp, path);	
			delete wdp;
		}			 
	}		 
	
	MbxCloseMp(g_hModicon,path);

	return 1;
}

int CWriter::WriteDevice(write_device_parameter * wdp, MBXPATHID path)
{
	int ret=false;
	TAG_TYPE tp;

	modicon * m;
	PRTK_TAG tte = wdp->tte;
	PTAG_VALUE tv = &wdp->tv;

	m = _modicon_by_tag(wdp->tte);
	if(!m){
		return 0;
	}

	if(tte->d.BinaryAddress[0]){		
		int z,r;
		z=tte->d.BinaryAddress[2];
		r=*((__u16*)&tte->d.BinaryAddress[3]);
		tp = get_value_type(tte->s.Flags);
		if(dt_real4 == tp){ 
			// scale if nessary
			float fv=tv->fltValue;
			char na[4];
			memset(na,-1,4);
			if( !( 0 == memcmp(&tte->d.BinaryAddress[6], na ,4) ) ){
				float fa, fb;
				memcpy(&fa, &tte->d.BinaryAddress[6], sizeof(float));
				memcpy(&fb, &tte->d.BinaryAddress[10], sizeof(float));
				if( fabs(fa) > 1.e-10 ){
					fv = (fv - fb) / fa;
				}
			}
			if(m->write_float_value(z,r,fv,path,tte->d.BinaryAddress[5])){
				ret=true;
			};
		}else{
			if(m->write_bool_value(z,r,tv->iValue,path)){
				ret=true;
			}
		}
	}	
	
	return ret;
}

IOSS_DLL_EXPORT __bool load(PDRIVER_INFO)
{
	int code;
	
	if( code=MbxOpen(0,&g_hModicon) ){
		utils_error(">> oops!!! MbxOpen failed with code %d.\n", code);
		return 0;
	}

	g_Writer = new CWriter();
	if(!g_Writer){
		utils_error(">> Oops!!! cannot create Modicon writer.\n");
		return 0;
	}

	if(!g_Writer->start()){
		delete g_Writer;
		g_Writer = 0;
		utils_error(">> Oops!!! cannot start Modicon writer.\n");
		return 0;
	}

	return 1;
}

IOSS_DLL_EXPORT __bool unload()
{
	if(g_Writer){
		g_Writer->stop();
		delete g_Writer;
	}
	g_Writer = 0;
	if(g_hModicon){
		MbxClose(g_hModicon);
	}
	g_hModicon = 0;
	return 1;
}

/*****************************************************************************

	将文本格式的地址信息翻译成二进制格式
	文本地址格式：
	开关量： <region>.<bit_offset>
	注意为了和Modibus协议一致，这里的bit_offset从1开始编号
	模拟量：
		3x: <region>.<register_offset>.[a/b]
		4x: <region>.<register_offset>.(r4|i2|ui2).[a/b]
	二进制地址格式：
	开关量：
		BinaryAddress[0]:0非法地址，1合法地址
		BinaryAddress[1]:Not used.
		BinaryAddress[2]:区号
		BinaryAddress[3],[4]:位地址
	模拟量:
		common:
			BinaryAddress[0]:0非法地址，1合法地址
			BinaryAddress[1]:Not used
			BinaryAddress[2]:区号
			BinaryAddress[3],[4]:寄存器地址
			BinaryAddress[6-9]:fa
			BinaryAddress[10-13]:fb
		4x:
			BinaryAddress[5]:data type

*****************************************************************************/
IOSS_DLL_EXPORT __bool address_translate(PRTK_TAG tte)
{
	CArgs argAll, args, argAb;
	modicon	* m;

	m = _modicon_by_tag(tte);

	if(!m){
		return __false;
	}

	parse_arg_ex(tte->s.Address,&argAll," ");
	parse_arg_ex(argAll.argv[0],&args,".");
	parse_arg_ex(tte->s.Address,&args,".");

	int  z,r;
	float fa,fb;
	bool b_scale=false;

	tte->d.BinaryAddress[0]=0;
	tte->d.BinaryAddress[5] = 0; // by default, analog data is uint2

	if(args.argc < 2){
		utils_error(">> Modicon, invalid address : %s\n",tte->s.Address);
		return __false;
	}
	
	z=atoi(args.argv[0]);
	r=atoi(args.argv[1]);

	if(z!=0 && z!=1 && z!=3 && z!=4){
		return __false;
	}	
	
	if(dt_real4 == get_value_type(tte->s.Flags)){
		// 必须是3区或者4区, 并且不能超过组态时指定的最大寄存器个数
		if(z==3){
			if( r>0 && r<=m->c[2]){
				tte->d.BinaryAddress[0]=1;
				tte->d.BinaryAddress[2]=z;
				*((__u16*)&tte->d.BinaryAddress[3])=r;
			}
			if(argAll.argc > 1){
				//got ax+b
				parse_arg_ex(argAll.argv[1],&argAb,"/");
				if(2 == argAb.argc){
					fa=atof(argAb.argv[0]);
					fb=atof(argAb.argv[1]);
					if(fabs(fa) > 1.e-10){
						b_scale = true;
					}else{
						return __false;
					}
				}
			}
		}else if(z==4){
			if( r>0 && r<=m->c[3] ){
				tte->d.BinaryAddress[0]=1;
				tte->d.BinaryAddress[2]=z;
				*((__u16*)&tte->d.BinaryAddress[3])=r;
			}
			if(args.argc < 4){
				return __false;
			}
			if(0 == strcmpi(args.argv[3], "ui2")){
				tte->d.BinaryAddress[5] = 0;
			}else if(0 == strcmpi(args.argv[3], "i2")){
				tte->d.BinaryAddress[5] = 1;
			}else if(0 == strcmpi(args.argv[3], "r4")){
				// 对于浮点数，寄存器地址必须是 "1 + 2的倍数", 并且不能是最后一个寄存器
				if(!(r & 1) || r == m->c[3]){
					tte->d.BinaryAddress[0]=0;
					return __false;
				}
				tte->d.BinaryAddress[5] = 2;
			}else{
				return __false;
			}
			if(argAll.argc > 1){
				//got ax+b
				parse_arg_ex(argAll.argv[1],&argAb,"/");
				if(2 == argAb.argc){
					fa=atof(argAb.argv[0]);
					fb=atof(argAb.argv[1]);
					if(fabs(fa) > 1.e-10){
						b_scale = true;
					}else{
						return __false;
					}
				}
			}
		}else{
			return __false;
		}
	}else{
		// 必须是0区或者1区，并且位地址不能超过组态时指定的最大寄存器个数
		if( r > 0 && r <= m->c[z] * 16 && ( z == 0 || z == 1 ) ){
			tte->d.BinaryAddress[0]=1;
			tte->d.BinaryAddress[2]=z;
			*((__u16*)&tte->d.BinaryAddress[3])=r;
		}
	}

	if(b_scale){
		memcpy(&tte->d.BinaryAddress[6], &fa, sizeof(float));
		memcpy(&tte->d.BinaryAddress[10], &fb, sizeof(float));
	}else{
		memset(&tte->d.BinaryAddress[6], -1, 8);
	}

	if(tte->d.BinaryAddress[0]){
		return __true;
	}

	return __false;
}

/***************************************************************************
	从硬件缓冲区中得到变量的工程值
	输入：
	ni	-->  NPU名
	tte -->  变量
	he	-->  硬件缓冲区
***************************************************************************/
IOSS_DLL_EXPORT void update_tag(PCRTK_TAG tte, pmc_value_t * pt, PCRTK_TIME)
{
	char na[4];
	memset(na, -1, 4);
	float fv, fa, fb;

	memset(pt, 0, sizeof(pmc_value_t));

	modicon * m;

	m = _modicon_by_tag(tte);
	if(!m){
		return;
	}
	
	if(tte->d.BinaryAddress[0]){
		int z, r, at;
		z = tte->d.BinaryAddress[2];
		r = *((__u16*)&tte->d.BinaryAddress[3]);
		at = *((__u16*)&tte->d.BinaryAddress[5]);
		if(dt_real4 == get_value_type(tte->s.Flags)){ 		
			if(m->get_float_value(z, r, &fv, at)){
				// scale value if nessesary
				if(!(0 == memcmp(&tte->d.BinaryAddress[6], na, 4))){
					memcpy(&fa, &tte->d.BinaryAddress[6], sizeof(float));
					memcpy(&fb, &tte->d.BinaryAddress[10], sizeof(float));
					fv = fa * fv + fb;
				}
				pt->Flags |= TF_Valid;
				pt->Value.fltValue = fv;
			}			  
		}else{
			if(m->get_bool_value(z,r,&pt->Value.iValue)){
				pt->Flags |= TF_Valid;
			}
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
	CArgs args;
	int  c0,c1,c3,c4,did;
	CModiconDevice * d;
	
	parse_arg_ex(handle->parameter, &args, ",");
	
	if(args.argc < 5){
		utils_error(">> Error : modicon, %s, invalid configuration.\n", handle->parameter);
		return 0;
	}
	did=atoi(args.argv[0]);
	c0=atoi(args.argv[1]);
	c1=atoi(args.argv[2]);
	c3=atoi(args.argv[3]);
	c4=atoi(args.argv[4]);

	// 3区和4区的寄存器个数必须是偶数
	if(c3&1){
		utils_error(">> Warning : modicon, %s, invalid configuration.\n", handle->parameter);
		c3++;
	}
	if(c4&1){
		utils_error(">> Warning : modicon, %s, invalid configuration.\n", handle->parameter);
		c4++;
	}
	
	d = new CModiconDevice(150);
	if(!d){
		utils_error("Error allocating modicon object.\n");
		return 0;
	}

	// ok,创建modicon对象
	if( !d->init(did,c0,c1,c3,c4) ){
		utils_error(">> Error creating modicon object %d.\n", did);
		delete d;
		return 0;
	}

	// ok, start the thread
	handle->OwnerField = d;

	if(!d->start()){
		delete d;
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

	if( !dev->stop(3000) ){
		return 0;
	}

	delete dev;

	return 1;
}

IOSS_DLL_EXPORT __bool write_device(PRTK_TAG tte, PCTAG_VALUE value)
{
	write_device_parameter * wdp;
	wdp=new write_device_parameter;
	if(!wdp){
		utils_error(">> modicon , Shit!! error new write_device_parameter\n");
		return 0;
	}
	wdp->tte=tte;
	wdp->tv=*value;
	return PostThreadMessage(g_Writer->id(), WM_USER+426, (WPARAM)wdp, 0);
}
