#
# 常用电机驱动模块
#
fblock MTR_LTE;
attribute(autoscan);
uuid{f880-fa5c-fb2e-3601};
var
  	L_R:bool;in;	//远程/就地切换
	AM:bool;in;	//自动/手动切换
	KO:bool;in;	//操作员合闸指令
	AO:bool;in; 	//自动合闸指令
	PO:bool;in;	//联锁合闸指令
	FO:bool;in;	//允许合闸条件
	RUN:bool;in;	//已运行反馈
	DIS:bool;in;	//设备故障
	STOP:bool;in;	//已停止反馈
	FZ:bool;in;	//允许跳闸条件
	PC:bool;in;	//保护跳闸指令
	PZ:bool;in;	//联锁跳闸指令
	AC:bool;in;	//自动跳闸指令
	KC:bool;in;	//操作员分闸指令
	T1:date;in; 	//合闸超时时间
	T2:date;in;	//分闸超时时间

	STRENO:bool;out;	//合闸条件不满足
	STRCMD:bool;out;	//合闸命令
	MOTRDIS:bool;out;	//设备驱动故障
	STPCMD:bool;out;	//分闸命令
	STPENO:bool;out;	//分闸条件不满足
	ET1:date;out;		
	ET2:date;out;
		
implementation

static bool OP,CL,Q1,Q2,Q3,R1,R2,R3,OVERTIME;
static date dueTime1,dueTime2,myT1,myT2;
static bool old_T1,old_T2,T1Q,T2Q;
static bool lastRun,runFallingEdge;

void main()
{
	//
	if(!RUN && lastRun)
		runFallingEdge=1;
	else
		runFallingEdge=0;
	lastRun=RUN;
	//合闸综合指令
	OP = (((AM && AO) || KO || PO) && FO && !PC && !DIS);
	STRENO = (((AM && AO) || KO || PO) && !OP);
	R1 = (STPCMD || MOTRDIS || L_R || runFallingEdge);
	if(R1){
		Q1 = 0;
	}else if((OP || RUN)){
		Q1 = 1;
	}
	
	if(R1){
		Q2 = 0;
	}else if(RUN){
		Q2 = 1;
	}
	STRCMD = ((Q1 && !RUN) && !Q2);
	//分闸综合指令
	CL = ((((AM && AC) || KC || PZ) && FZ && !DIS) || PC);
	STPENO = (((AM && AC) || KC || PZ) && !(((AM && AC) || KC || PZ) && FZ && !DIS)); 
	R2 = (OP || CL);
	R3 = (STOP || Q3 || STRCMD || L_R);
	
	if(R2){
		Q3 = 0;
	}else if(OVERTIME){
		Q3 = 1;
	}
	
	if(R3){
		STPCMD = 0;
	}else if(CL){
		STPCMD = 1;
	}
	//驱动故障判断
	date now;
	now = gettime();
	myT1 = T1;
	myT2 = T2;
	if(STRCMD){
		if(!old_T1){
			dueTime1 = now + myT1;
			ET1 = 0;
		}else if(now > dueTime1){
			T1Q = 1;
			ET1 = myT1;
		}else{
			T1Q = 0;
			ET1 = myT1 + now - dueTime1; 
		}
	}else{
		T1Q = 0;
		ET1 = 0;
	}
	old_T1 = STRCMD;
	if(STPCMD){
		if(!old_T2){
			dueTime2 = now + myT2;
			ET2 = 0;
		}else if(now > dueTime2){
			T2Q = 1;
			ET2 = myT2;
		}else{
			T2Q = 0;
			ET2 = myT2 + now - dueTime2;
		}
	}else{
		T2Q = 0;
		ET2 = 0;
	}
	old_T2 = STPCMD;
	
	if(R2){
		MOTRDIS = 0;
	}else if(((STOP && RUN) || OVERTIME || T1Q)){
		MOTRDIS = 1;
	}	
	OVERTIME = T2Q;
}
