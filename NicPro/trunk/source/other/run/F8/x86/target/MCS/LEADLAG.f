fblock LEADLAG;		//超前滞后模块
uuid{dd30-49ca-3818-a5bc};
var
	X :float;in;	//输入值
	MAN :bool;in;	//跟踪切换开关
	HALT :bool;in;  //中断操作
	GAIN :float;in;	//增益
	LEAD :date;in;	//超前时间常数
	LAG :date;in;	//滞后时间常数
	YMAX :float;in; //输出高限
	YMIN :float;in; //输出低限
	YMAN :float;in;	//被跟踪量
	
	Y :float;out;	//输出值
	QMAX :bool;out;	//高限报警
	QMIN :bool;out;	//低限报警
	ERR :bool;out;	//高低限错误

implementation
static float K1;
static float K2;
static float K3;
static float old_X;
static float old_Y;
static date due_T;

void main()
{
	static bool first_time = true;
	date now;
	float dt;
	now = gettime();

	if(first_time){
		due_T = now;
		first_time = false;
		return;
	}
	
	dt = (float)(now - due_T);

	if(!MAN && !HALT){
		K1 = GAIN*(dt + LEAD)/(LAG + dt);
		K2 = GAIN*LEAD/(LAG + dt);
		K3 = LAG /(LAG + dt);
		Y = K1*X - K2*old_X + K3*old_Y;
	}else if(MAN){
		Y = YMAN;
	}
	
	if(Y > YMAX){
		Y = YMAX;
		QMAX = 1;
	}else{
		QMAX = 0;
	}
	
	if(Y < YMIN){
		Y = YMIN;
		QMIN = 1;
	}else{
		QMIN = 0;	
	}
	
	ERR = 0;
	if(YMAX < YMIN){
		ERR = 1;
		QMAX = 0;
		QMIN = 0;
		Y = 0;
	}
		
	old_X = X;
	old_Y = Y;
}