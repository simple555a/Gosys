#
#Delay Startup（延时启动功能块）
#
fblock TON_P;
attribute(autoscan);
uuid{cead-6418-660a-781f};
var
	S : bool;in;			//延时启动条件
	Pm: bool;in;			//延时暂停
	T : date;in;			//初始化延时时间
	
	Q : bool;out;			//延时时间到电平输出
	P : bool;out;			//延时时间到脉冲输出（一个程序周期）
	ET: date;out;			//已进行时间
	Tm: date;out;			//剩余时间

implementation
static bool old_S,Ps;
static date due_T,Ts,old_ET;
void main()
{
	date now;
	now = gettime();
	if(Pm){
		old_ET = ET;
		due_T = gettime();
		return;
	}
	if(S){
		if(Q){
			if(!Ps){
				P = 1;
				Ps = 1;
			}else{
				P = 0;
			}
			return;
		}else{
			Tm = T - ET;
			P = 0;
			Ps = 0;
		}
		if(!old_S){
			due_T = gettime();
			Ts = T;
			ET = 0;
			old_ET = 0;
		}else if((now > (due_T +Ts)) || (Tm < 0)){
			Q = 1;
			ET = T;
			Tm = 0;
		}else{
			Q = 0;
			Ts = T - old_ET;
			ET = old_ET + now - due_T;
		}
	}else{
		Q = 0;
		ET = 0;
		Ts = 0;
		Tm = T;
	}
	old_S = S;
}