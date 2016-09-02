fblock Delay;
attribute(autoscan);
uuid{fd70-e13c-1e73-c029};
var
	IN: float; in;		//输入值
	T_D: date; in;		//延迟时间
	T_CY: date; in;		//采样时间(为保证精度，一般设为扫描周期的5倍以上)
	Set: bool;in;		//延时置位1:延时输出状态0:跟随状态
	Hold: bool;in;		//保持输出
	OUT: float; out;	//输出值

implementation
static integer N,i;
static date due_T;
static bool a,old_a;
static float x[100000];		//内部定义寄存器长度为10万,因此T_D/T_CY不能超过10万

void main()
{
	static bool first_time = true;

	if(first_time){
		a = 1;
		old_a = 0;
		first_time = false;
		return;
	}
	
	N = (int)(T_D/T_CY);
	date now;
	now = gettime();

	if(i >= N){
		i = 0;
	}

	if(a && !old_a){
		due_T = now;
	}else if(a){
		if((now - due_T) >= T_CY){
			x[i] = IN;
			i++;
			due_T = now;
		}
	}
	old_a = a;

	if(Set && !Hold){	//插值计算
		if(i < (N - 2)){
			OUT = (x[i + 2] - x[i + 1]) * (now - due_T) / T_CY + x[i + 1];
		}else if(i == (N - 2)){
			OUT = (x[0] - x[i + 1]) * (now - due_T) / T_CY + x[i + 1];
		}else if(i == (N - 1)){
			OUT = (x[1] - x[0]) * (now - due_T) / T_CY + x[0];
		}
	}else if(!Set && !Hold){
		OUT = IN;
	}else{			//输出保持
		return;
	}
}