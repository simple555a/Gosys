#
#时间累计模块
#
fblock TIM_TOL;
attribute(autoscan);
uuid{b279-5efc-e810-b0ac};
var
	S : bool;in;	//累计变量
	R : bool;in;	//累计清零
	P : bool;in;	//累计暂停
	T : date;in;	//间隔时间参数
	OUT : float;out;	//累计输出
implementation
static bool old_SS,SS;
static float old_OUT;
static date due_T,ET;
void main()
{
	date now;
	now = gettime();
	SS = S && !P;
	if(R){
		OUT = 0;
	}else{
		if(SS){
			OUT = old_OUT + ET;
			if(!old_SS){
				due_T = now + T;
				ET = 0;
			}else if(now > due_T){
				ET = T;
			}else{
				ET = T + now - due_T;
			}
					
		}else{
			ET = 0;
			old_OUT = OUT;
		}
		old_SS = SS;
	}
}