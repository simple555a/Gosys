fblock SS_MA;		//标准模拟量手操器
uuid{2d54-455e-2c62-d8ef};
var
	X: float; in;		//输入量
	OV: float; in;		//超驰输入量
	OS: bool; in;		//超驰开关
	M_A: bool; in;		//手自动切换
	YH: float; in;		//Y上限
	YL: float; in;		//Y下限
	L: bool; in;		//屏蔽操作(用于检修等状态)
	Yman: float; in;	//手动值给定

	Y: float; out;		//MA站输出，带上下限
	y_out:float;out;
	S: bool; out;		//状态输出1:自动,0:手动
	Lock: bool;out;		//检修状态

implementation

static float limit_y(float t)
{
	if(t > YH){
		t = YH;
	}else if(t < YL){
		t = YL;
	}
	return t;
}

void main()
{
	//超驰信号优先级最高
	if(OS){
		Y = limit_y(OV);
		y_out = limit_y(OV);
		return;
	}


	//手自动状态判断
	if(M_A){
		S = 1;
	}else{
		S = 0;
	}


	//检修挂起状态	
	if(L){
		Lock = 1;
		return;
	}
	
	Lock = 0;
	

	//手动状态
	if(!S){
		Y = limit_y(Yman);
		y_out = limit_y(Yman);
		return;
	}
	//自动状态
	if(S){
		Y = limit_y(X);
		y_out = limit_y(X);
	}
}	