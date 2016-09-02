fblock D_DS_VAL;
attribute(autoscan);
uuid{f880-fa5c-fb2e-3603};
var
  	AM:bool;in;	//自动/手动模式
	KO:bool;in;	//操作员开指令
	AO:bool;in; 	//自动开指令
	PO:bool;in;	//联锁开指令
	OPN:bool;in;	//开反馈
	KO_FB:bool;in;	//禁止操作员开指令
	LO:bool;in;	//禁止开指令
	FO:bool;in;	//允许开条件
	MPP:bool;in;	//中停/故障复位指令
	LC:bool;in;	//禁止关指令
	KC_FB:bool;in;	//禁止操作员关指令
	FZ:bool;in;	//允许关条件
	CLS:bool;in;	//关反馈
	PC:bool;in;	//联锁及保护关指令
	AC:bool;in;	//自动关指令
	KC:bool;in;	//操作员关指令
	PS:bool;in;	//电源故障
	TD:date;in;	//驱动超时设定
	
	ONENO:bool;out;		//打开条件不满足
	ONCMD:bool;out;		//打开（1）命令
	OFFCMD:bool;out;	//关闭条件不满足
	OFFENO:bool;out;	//关闭（1）命令
	TIMOUT:bool;out;	//驱动超时故障
	ETD:date;out;
			
implementation
static bool OP,CL,OpenSet,OpenReset,OpenQ,CloseSet,CloseReset,CloseQ;
static bool TDSet,MPPTDSet,old_TD,old_MPPTD,TDQ,MPPTDQ;
static date due_TD,due_MPPTD;
static bool MPPSet,MPPReset,MPPQ;

void main()
{
	//综合指令
	OP = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO) && FO && !PS && !LO && !PC) && !OPN;
	CL = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC) && FZ && !PS && !LC) && !CLS;
	ONENO = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO)&& !OP);
	OFFENO = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC)&& !CL);
	OpenSet = (OP && !CloseSet);
	OpenReset = (OPN || MPP || KC);
	CloseSet = (CL && !OpenSet);
	CloseReset = (CLS || MPP || KO);
	
	//打开命令
	if(OpenSet){
		OpenQ = 1;
	}else if(OpenReset){
		OpenQ = 0;
	}
	ONCMD = OpenQ && !TIMOUT;
	//关闭命令
	if(CloseSet){
		CloseQ = 1;
	}else if(CloseReset){
		CloseQ = 0;
	}
	OFFCMD = CloseQ && !TIMOUT;
	
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
	}else{
		TDQ = 0;
		ETD = 0;
	}
	old_TD = TDSet;
	
	if(MPPTDSet){
		if(!old_MPPTD){
			due_MPPTD = now + 20000;
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
}