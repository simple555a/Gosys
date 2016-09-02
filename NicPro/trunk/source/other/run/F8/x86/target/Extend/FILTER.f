fblock FILTER;			//滤波模块
uuid{2f4b-b97c-de81-eb4d};
var
	X: float; in;		//输入值
	Mode: integer; in;	//滤波方式0:不滤波1:平方根滤波2:低阶滤波3:高阶滤波
	Y: float; out;		//输出值
implementation
static float old_X;
static float x[7];

void main()
{
	x[6] = X/8 + 7*x[5]/8;
	x[5] = X/7 + 6*x[4]/7;
	x[4] = X/6 + 5*x[3]/6;
	x[3] = X/5 + 4*x[2]/5;
	x[2] = X/4 + 3*x[1]/4;
	x[1] = X/3 + 2*x[0]/3;
	x[0] = X/2 + old_X/2;
	
	if(Mode == 0){			//不滤波直接输出
		Y = X;	
	}else if(Mode == 1){		//平方根滤波模式
		if(X >= 0.0){
			Y = sqrt(X);
		}else{
			Y = (-1) * sqrt((-1) * X);
		}	
	}else if(Mode == 2){		//低阶(2)滤波
		Y = x[0];
	}else if(Mode == 3){		//高阶(8)滤波
		Y = x[6];
	}else{				//不做处理,返回
		return;
	}	 
	 old_X = X;
}