fblock D_AS_VAL;
attribute(autoscan);
uuid{f1df-84ff-432a-4081};
var
  	AM:bool;in;	//自动/手动模式
	KO_S:bool;in;	//操作员开短指令
	KO_L:bool;in;	//操作员开长指令
	AO:bool;in; 	//自动开指令
	PO:bool;in;	//联锁开指令
	DPO:bool;in;	//保护性直接开指令
	KO_FB:bool;in;	//禁止操作员开指令
	LO:bool;in;	//禁止开指令
	FO:bool;in;	//允许开条件
	OPN:bool;in;	//开反馈
	AI:float;in;	//阀位反馈信号
	MaxR:float;in;	//位置信号上限
	MinR:float;in;	//位置信号下限
	CLS:bool;in;	//关反馈	
	MPP:bool;in;	//中停/故障复位指令
	FZ:bool;in;	//允许关条件
	LC:bool;in;	//禁止关指令
	KC_FB:bool;in;	//禁止操作员关指令
	PC:bool;in;	//联锁及保护关指令
	AC:bool;in;	//自动关指令
	KC_L:bool;in;	//操作员关长指令
	KC_S:bool;in;	//操作员关短指令
	PS:bool;in;	//电源故障
	TD:date;in;	//驱动超时设定
	
	ONENO:bool;out;		//打开条件不满足
	ONCMD:bool;out;		//打开（1）命令
	OFFCMD:bool;out;	//关闭条件不满足
	OFFENO:bool;out;	//关闭（1）命令
	TIMOUT:bool;out;	//驱动超时故障
	ETD:date;out;
			
implementation
static bool OP,CL,OpenSet,OpenReset,OpenQ,CloseSet,CloseReset,CloseQ,R1,R2,R3,R4;
static bool TDSet,KO_SSet,KC_SSet,old_TD,old_KO_STP,TDQ,KO_STPQ,old_KC_STP,KC_STPQ;
static date due_TD,due_KO_STP,due_KC_STP;
static bool MPPSet,MPPQ,DPOQ;

void main()
{
	//综合指令
	OP = (((AM && AO && !AC) || ((KO_S || KO_L) && !KO_FB && !(KC_L || KC_S))|| PO) && FO && !PS && !LO && !PC);
	CL = (((AM && AC && !AO) || ((KC_S || KC_L) && !KC_FB && !(KO_L || KO_S))|| PC) && FZ && !PS && !LC && !DPO);
	ONENO = (((AM && AO && !AC) || ((KO_S || KO_L) && !KO_FB && !(KC_L || KC_S))|| PO)&& !OP);
	OFFENO = (((AM && AC && !AO) || ((KC_S || KC_L) && !KC_FB && !(KO_L || KO_S))|| PC)&& !CL);
	R1 = (KO_S || PC || (AI > MaxR) || MPP || TIMOUT || CL || OPN);
	R2 = (OP || TIMOUT || (MinR > AI) || KC_S || PO || MPP || CLS);
	R3 = ((AI > MaxR) || (MinR > AI));
	R4 = ((AI > MaxR) || OPN || MPP);
	OpenSet = (OP && !CloseSet);
	CloseSet = (CL && !OpenSet);
		
	//打开命令
	if(OpenSet){
		OpenQ = 1;
	}else if(R1){
		OpenQ = 0;
	}
	if(DPO) {
		DPOQ = 1;
	}else if(R4){
		DPOQ = 0;
	}
	
	ONCMD = (OpenQ || DPOQ || (KO_STPQ && OP));
	
	//关闭命令
	if(CloseSet){
		CloseQ = 1;
	}else if(R2){
		CloseQ = 0;
	}
	
	OFFCMD = (CloseQ || (KC_STPQ && CL));
	
	//驱动故障判断
	MPPSet = MPP;
	if(MPPSet){
		MPPQ = 1;
	}else if(R3){
		MPPQ = 0;
	}
	
	TDSet = (OpenQ || CloseQ || ((OpenQ && !MPPQ) || (CloseQ && !MPPQ)));
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
	
	if(KO_SSet){
		if(!old_KO_STP){
			due_KO_STP = now + 100;
		}else if(now > due_KO_STP){
			KO_STPQ = 1;
		}else{
			KO_STPQ = 0;
		}
	}else{
		KO_STPQ = 0;
	}
	old_KO_STP = KO_S;
	
	if(KC_SSet){
		if(!old_KC_STP){
			due_KC_STP = now + 100;
		}else if(now > due_KC_STP){
			KC_STPQ = 1;
		}else{
			KC_STPQ = 0;
		}
	}else{
		KC_STPQ = 0;
	}
	old_KC_STP = KC_S;
	
	if(TDQ){
		TIMOUT = 1;
	}else if(MPP){
		TIMOUT = 0;
	}
}