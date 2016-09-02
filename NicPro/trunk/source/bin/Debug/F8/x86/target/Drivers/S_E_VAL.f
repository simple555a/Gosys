fblock S_E_VAL;
uuid{f880-fa5c-fb2e-3602};
var
  	L_R:bool;in;	//远程/就地切换
	AM:bool;in;	//自动/手动模式
	
	KO:bool;in;	//操作员开指令
	AO:bool;in; 	//自动开指令
	PO:bool;in;	//联锁开指令
	
	LO:bool;in;
	FO:bool;in;	//允许开条件
	
	LC:bool;in;
	FZ:bool;in;	//允许关条件
	
	PC:bool;in;	//联锁及保护关指令
	AC:bool;in;	//自动关指令
	KC:bool;in;	//操作员关指令
	SAFE:bool;in;   //安全状态(1:开安全/0:关安全)
	
	ONENO:bool;out;	//打开条件不满足
	ONCMD:bool;out;	//打开（1）命令
implementation
static bool OP,CL,S,R;

void main()
{
	//合闸综合指令
	OP = (((AM && AO && !AC) || KO)&& FO && !LO && !PC && L_R);
	ONENO = (((AM && AO && !AC) || KO )&& !OP);
	S = OP || PO || (SAFE && !L_R);
	//分闸综合指令
	CL = (((AM && !AO && AC)|| KC)&& FZ && !LC && L_R);
	R = CL || PC || (!SAFE && !L_R);
	if(SAFE){
		if(S){
			ONCMD = 1;
		}else if(R){
			ONCMD = 0;
		}
	}else{
		if(R){
			ONCMD = 0;
		}else if(S){
			ONCMD = 1;
		}
	}
}

