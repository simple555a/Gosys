fblock SCALING_X; //带修正的量程变换模块
uuid{51d9-993e-5f16-6666};
var	
IN:float; in;
	Min:float;in;
	Max:float;in;
	Kc:float;in;	//修正参数
	Kt:float;in;	//小信号切除比（%）
	OUT:float; out;
implementation
void main()
{
	OUT = IN * (Max - Min) + Min + Kc;
	if(OUT < (Min + (Max - Min) * Kt)){
		OUT = Min;
	}
	if(OUT > Max){
		OUT = Max;
	}
}