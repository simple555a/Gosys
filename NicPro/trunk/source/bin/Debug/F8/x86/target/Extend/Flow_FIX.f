fblock FLOW_FIX; //带小信号切除的流量换算模块
uuid{67aa-7b74-1378-8bdc};
var	
	IN:float; in;
	Pmin:float;in;
	Pmax:float;in;
	Tmin:float;in;
	Tmax:float;in;
	Kt:float;in;	//小信号切除比（%）
	OUT:float;out;
	P:float;out;
	
implementation
void main()
{	
	P = IN * (Pmax - Pmin) + Pmin;
	if(P < 0){
		P = 0;
	}
	OUT = sqrt(P) * (Tmax - Tmin) / sqrt(Pmax - Pmin); 
	if(OUT < (Tmin + (Tmax - Tmin) * Kt)){
		OUT = 0;
	}
	if(OUT > Tmax){
		OUT = Tmax;
	}
}