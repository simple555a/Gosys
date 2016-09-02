fblock SFT;             //无扰切换模块
attribute(autoscan);
uuid{e45d-9f52-81ae-b8b0};

var
	X1 : float;in;  //第一路输入
	X2 : float;in;  //第二路输入
	Z : bool;in;    //选择开关
	DY1 : float;in;  //X2至X1变化率
	DY2 : float;in; //X1至X2变化率
	
	Y : float;out;  //无扰切换输出
	ERR: bool;out;  //模式错误
	
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
	
	if(!DY1 && !DY2){
		ERR = 0;
		if(!Z){
			Y = X2;
		}else{
			Y = X1;
		}
		
	}else if(DY1 && !DY2){
		ERR = 0;
		if(Z && (Y != X1)){
			if(X1 >= X2){
				dy = dt * (DY1/60);
				Y += dy;
				if(Y >= X1){
					Y = X1;
					}		
			}else{
				dy = dt * (DY1/60);
				Y -= dy;
				if(Y <= X1){
					Y = X1;
					}
			}
		}else if(!Z){
			Y = X2;
		}else{
			Y = X1;
		}
	
	}else if(!DY1 && DY2){
		ERR = 0;
		if(!Z && (Y != X2)){
			if(X2 >= X1){
				dy = dt * (DY2/60);
				Y += dy;
				if(Y >= X2){
					Y = X2;
					}		
			}else{
				dy = dt * (DY2/60);
				Y -= dy;
				if(Y <= X2){
					Y = X2;
					}
			}
		}else if(Z){
			Y = X1;
		}else{
			Y = X2;
		}
	
	}else{
		ERR = 1;
		if(!Z){
			Y = X2;
		}else{
			Y = X1;
		}
	}
}