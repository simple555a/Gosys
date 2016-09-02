fblock RMP;               //斜坡信号模块
attribute(autoscan);
uuid{e4e6-8155-de70-694b};
var
	Base : float;in;  //基点
	End : float;in;   //终点
	Rate : float;in;  //变化率
	Hold : bool;in;   //保持
	Rst : bool;in;    //复位
	
	Y : float;out;    //发生器输出
	D : bool;out;     //到达终点信号
implementation
static bool old_Rst;
static date due_T;

void main()
{
	static bool first_time = true;
	date now;
	float dt;
	float dy;
	now = gettime();

	if(first_time){
		Y = Base;
		D = 0;
		due_T = now;
		first_time = false;
		return;
	}
	
	dt = (float)(now - due_T);
	due_T = now;
	
	if(!Rst){
		Y = Base;
		D = 0;
	}else{
		if(!Hold){
			dy = dt * Rate;
			Y += dy;
		}
		if(Y >= End){
			Y = End;
			D = 1;
		}else{
			D = 0;
		}
	}
}