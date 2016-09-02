fblock RATLMT;               //速率限制器,参照Concept速度限制器编制
attribute(autoscan);
uuid{045d-9f52-51ae-b8b0};

var
	IN : float;in;       //速率限制器输入
	RATE : float;in;     //积最大速度限制
	OUT_MIN : float;in;  //输出下限
	OUT_MAX : float;in;  //输出上限
	TR_I : float;in;     //初始化输入
	TR_S : bool;in;      //初始化类型（1：跟踪模式0：自动模式）
	
	OUT : float;out;     //速率限制器输出
	QMIN : bool;out;     //输出达下限
	QMAX : bool;out;     //输出达上限
	ERR : bool;out;      //上下限错误
	
implementation
static date due_T;

void main()
{
	static bool first_time = true;
	date now;
	float dt;
	float dy;
	now = gettime();

	if(first_time){
		due_T = now;
		first_time = false;
		return;
	}
	
	dt = (float)(now - due_T);
	due_T = now;
		
	if(OUT_MIN > OUT_MAX){
		ERR = 1;
		QMIN = 0;
		QMAX = 0;
		return;
	}else{
		ERR = 0;
	}

	if(TR_S){
		OUT = TR_I;
		if(OUT >= OUT_MAX){
			OUT = OUT_MAX;
			QMAX = 1;
		}
		if(OUT <= OUT_MIN){
			OUT = OUT_MIN;
			QMIN = 1;
		}
		if(OUT > OUT_MIN){
			QMIN = 0;
		}
		if(OUT < OUT_MAX){
			QMAX = 0;
		}
	}

	if(!TR_S){
		if(RATE == 0){
			OUT = IN;
			if(OUT >= OUT_MAX){
				OUT = OUT_MAX;
				QMAX = 1;	
			}
			if(OUT <= OUT_MIN){
				OUT = OUT_MIN;
				QMIN = 1;
			}
			if(OUT > OUT_MIN){
				QMIN = 0;
			}
			if(OUT < OUT_MAX){
				QMAX = 0;
			}
		}else if(OUT != IN){
			if(OUT > IN){
				dy = dt * RATE;
				OUT -= dy;
				QMIN = 0;
				QMAX = 0;
				if(OUT <= IN){
					OUT = IN;
				}
				if(OUT <= OUT_MIN){
					OUT = OUT_MIN;
					QMIN = 1;
				}
			}else{
				dy = dt * RATE;
				OUT += dy;
				QMIN = 0;
				QMAX = 0;
				if(OUT >= IN){
					OUT = IN;
				}
				if(OUT >= OUT_MAX){
					OUT = OUT_MAX;
					QMAX = 1;
				}
			}
		}else{
			OUT = IN;
			QMAX = 0;
			QMIN = 0;
			if(OUT >= OUT_MAX){  
				OUT = OUT_MAX;
				QMAX = 1;
				QMIN = 0;
			}else if(OUT <= OUT_MIN){
				OUT = OUT_MIN;
				QMIN = 1;
				QMAX = 0;
			}
		}
	}
}