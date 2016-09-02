fblock D_DT_VAL;
attribute(autoscan);
uuid{a2c2-c36c-8c7e-9cb9};
var
  	AM	:bool;in;	//自动/手动模式
	KO	:bool;in;	//操作员开指令
	AO	:bool;in; 	//自动开指令
	PO	:bool;in;	//联锁开指令
	OPN	:bool;in;	//开反馈
	FO	:bool;in;	//允许开条件
	KO_FB	:bool;in;	//禁止操作员开指令
	LO	:bool;in;	//禁止开指令
	MPP	:bool;in;	//中停/故障复位指令
	LC	:bool;in;	//禁止关指令
	KC_FB	:bool;in;	//禁止操作员关指令
	FZ	:bool;in;	//允许关条件
	CLS	:bool;in;	//关反馈
	PC	:bool;in;	//联锁及保护关指令
	AC	:bool;in;	//自动关指令
	KC	:bool;in;	//操作员关指令
	DIS	:bool;in;	//设备故障
	L_R	:bool;in;	//1:就地；0：远方
	TS	:bool;in;	//1:脉冲指令；0：指令宽度由TD决定
	TP	:date;in;	//脉冲指令宽度
	TD	:date;in;	//驱动超时设定
	
	ONENO	:bool;out;	//打开条件不满足
	ONCMD	:bool;out;	//打开（1）命令
	OFFCMD	:bool;out;	//关闭条件不满足
	OFFENO	:bool;out;	//关闭（1）命令
	MPPT	:bool;out;	//中停/复位输出
	TIMOUT	:bool;out;	//驱动超时故障
	STU	:integer;out;	//设备状态
	V_FO	:bool;out;	//允许开
	V_FZ	:bool;out;	//允许关
	V_PO	:bool;out;	//超驰开
	V_PC	:bool;out;	//超驰关
	ETD	:date;out;	//驱动超时
			
implementation
static bool OP,CL,OpenSet,OpenReset,OpenQ,CloseSet,CloseReset,CloseQ;
static bool TDSet,MPPTDSet,old_TD,old_TP,old_MPPTD,TDQ,TPQ,MPPTDQ;
static date due_TD,due_MPPTD,due_TP;
static bool MPPSet,MPPReset,MPPQ;

void main()
{
	//综合指令
	OP = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO) && FO && !DIS && !L_R && !LO && !PC) && !OPN;
	CL = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC) && FZ && !DIS && !L_R && !LC) && !CLS;
	ONENO = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO)&& !OP);
	OFFENO = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC)&& !CL);
	V_FO = FO;
	V_FZ = FZ;
	
	
	OpenSet = (OP && !CloseSet);
	CloseSet = (CL && !OpenSet);
			
	//打开命令
	if(OpenSet && !CloseSet){
		OpenQ = 1;
	}else if(OpenReset || CloseSet){
		OpenQ = 0;
	}
	V_PO = PO;
	
	//关闭命令
	if(CloseSet && !OpenSet){
		CloseQ = 1;
	}else if(CloseReset || OpenSet){
		CloseQ = 0;
	}
	if(!TS){
		ONCMD = OpenQ && !TIMOUT;
		OFFCMD = CloseQ && !TIMOUT;
	}else{
		ONCMD = OpenQ && TPQ;
		OFFCMD = CloseQ && TPQ;
	}
	V_PC = PC;
	
	//中停指令
	MPPT = MPP;
	
	//驱动故障判断
	MPPSet = MPP;
	MPPReset = OpenSet || CloseSet;
	if(MPPSet){
		MPPQ = 1;
	}else if(MPPReset){
		MPPQ = 0;
	}
	
	TDSet = OpenQ || CloseQ;
	MPPTDSet = OPN && CLS && !MPPQ;
	OpenReset = ((OPN && !MPPTDSet) || MPP || KC || TIMOUT);
	CloseReset = ((CLS && !MPPTDSet) || MPP || KO || TIMOUT);
	
	date now;
	now = gettime();
	if(TDSet){
		if(!old_TD){
			due_TD = now + TD;
			ETD = 0;
		}else if(now > due_TD){
			TDQ = 1;
			ETD = TD;
		}else{
			TDQ = 0;
			ETD = TD + now - due_TD; 
		}
		if(!old_TP){
			due_TP = now + TP;
		}else if(now > due_TP){
			TPQ = 0;
		}else{
			TPQ = 1;
		}
	}else{
		TDQ = 0;
		TPQ = 1;
		ETD = 0;
	}
	old_TD = TDSet;
	old_TP = TDSet;
	
	if(MPPTDSet){
		if(!old_MPPTD){
			due_MPPTD = now + 20;
		}else if(now > due_MPPTD){
			MPPTDQ = 1;
		}else{
			MPPTDQ = 0;
		}
	}else{
		MPPTDQ = 0;
	}
	old_MPPTD = MPPTDSet;
	TIMOUT = TDQ || MPPTDQ;
	
	STU = (integer)OPN + (integer)CLS * 2 + (integer)(!(ONCMD + OFFCMD) && !(OPN + CLS) && !TIMOUT && !MPPT) * 3 + (integer)(DIS || TIMOUT) * 4;
	
}