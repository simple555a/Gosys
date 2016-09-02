#
# 常用电机驱动模块一
#
fblock MTR_LGE;
attribute(autoscan);
uuid{ff29-8bbb-ab11-8be2};
var
  	L_R	:bool;in;	//远程/就地切换:1=就地
	AM	:bool;in;	//自动/手动切换
	KO	:bool;in;	//操作员合闸指令
	AO	:bool;in; 	//自动合闸指令
	PO	:bool;in;	//联锁合闸指令
	FO	:bool;in;	//允许合闸条件
	LO	:bool;in;	//禁止启动
	RUN	:bool;in;	//已运行反馈
	DIS	:bool;in;	//设备故障
	STOP	:bool;in;	//已停止反馈
	LC	:bool;in;	//禁止停止
	FZ	:bool;in;	//允许跳闸条件
	PC	:bool;in;	//保护跳闸指令
	PZ	:bool;in;	//联锁跳闸指令
	AC	:bool;in;	//自动跳闸指令
	KC	:bool;in;	//操作员分闸指令
	MPP	:bool;in;	//故障及超时确认
	FIX	:bool;in;	//检修状态
	MT	:bool;in;	//1:验证状态反馈启动；０：无验证状态反馈启动
	TON	:date;in; 	//合闸超时时间
	TOFF	:date;in;	//分闸超时时间

	STRENO	:bool;out;	//合闸条件不满足
	STRCMD	:bool;out;	//合闸命令
	MOTRDIS	:bool;out;	//设备驱动故障
	STPCMD	:bool;out;	//分闸命令
	STPENO	:bool;out;	//分闸条件不满足
	DTrip	:bool;out;	//故障跳闸（软件判断）
	Status	:integer;out;	//设备状态
	V_FO	:bool;out;	//允许启
	V_FZ	:bool;out;	//允许停
	V_PO	:bool;out;	//超驰启
	V_PC	:bool;out;	//超驰停
	ETon	:date;out;	//启动命令执行时间
	EToff	:date;out;	//停止命令执行时间
		
implementation

static bool OP,CL,Q1,Q2,Q3,R1,R2,R3,OVERTIME;
static date dueTime1,dueTime2,myT1,myT2,dueTimeDTrip,TimeDTrip;
static bool old_T1,old_T2,T1Q,T2Q,set_DTrip,old_RUN;
static bool lastRun,runFallingEdge;
static bool old_DTrip;

void main()
{
	//
	if(!RUN && lastRun){
		runFallingEdge = 1;
	}else{
		runFallingEdge = 0;
	}
	lastRun = RUN;
	
	//合闸综合指令
	if(!MT){
		OP = (((AM && AO) || KO || PO) && FO && !PC && !DIS && !LO && !FIX);
	}else{
		OP = (((AM && AO) || KO || PO) && FO && !PC && !DIS && !LO && !FIX && STOP);
	}
	STRENO = (((AM && AO) || KO || PO) && !OP);
	R1 = (STPCMD || MOTRDIS || L_R || runFallingEdge || (MPP && !OP) || (Status > 2));
	V_FO = FO;
	V_FZ = FZ;
	
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
	V_PO = PO;
	
	//分闸综合指令
	if(!MT){
		CL = ((((AM && AC) || KC || PZ) && FZ && !DIS && !LC && !FIX) || PC);
	}else{
		CL = ((((AM && AC) || KC || PZ) && FZ && !DIS && !LC && !FIX && RUN) || PC);
	}
	STPENO = (((AM && AC) || KC || PZ) && !(((AM && AC) || KC || PZ) && FZ && !DIS && !LC && !FIX)); 
	
	R2 = (OP || CL);
	R3 = (STOP || Q3 || STRCMD || L_R || (MPP && !CL) || (Status > 2));
	
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
	V_PC = PC || PZ;
	
	//驱动故障判断
	date now;
	now = gettime();
	myT1 = TON;
	myT2 = TOFF;
	if(STRCMD){
		if(!old_T1){
			dueTime1 = now + myT1;
			ETon = 0;
		}else if(now > dueTime1){
			T1Q = 1;
			ETon = myT1;
		}else{
			T1Q = 0;
			ETon = myT1 + now - dueTime1; 
		}
	}else{
		T1Q = 0;
		ETon = 0;
	}
	old_T1 = STRCMD;
	if(STPCMD){
		if(!old_T2){
			dueTime2 = now + myT2;
			EToff = 0;
		}else if(now > dueTime2){
			T2Q = 1;
			EToff = myT2;
		}else{
			T2Q = 0;
			EToff = myT2 + now - dueTime2;
		}
	}else{
		T2Q = 0;
		EToff = 0;
	}
	old_T2 = STPCMD;
	
	if(MPP && !OP && !CL){
		MOTRDIS = 0;
	}else if(((STOP && RUN) || OVERTIME || T1Q)){
		MOTRDIS = 1;
	}	
	OVERTIME = T2Q;
	
	//故障跳闸判断
	set_DTrip = !CL && old_RUN && STOP && !MPP;
	if(MPP){
		DTrip = 0;
	}else{
		if(set_DTrip){
			if(!old_DTrip){
				dueTimeDTrip = now + 500;
				TimeDTrip = 0;
			}else if(now > dueTimeDTrip){
				DTrip = 1;
				TimeDTrip = 500;
			}else{
				DTrip = 0;
				TimeDTrip = 500 + now - dueTimeDTrip; 
			}
		}else{
			DTrip = 0;
			TimeDTrip = 0;
		}
		old_DTrip = set_DTrip;		
	}
	old_RUN = RUN;	
	
	Status = (integer)RUN * 1 + (integer)STOP * 2 + (integer)(DIS || MOTRDIS || DTrip) * 3;
	
}
