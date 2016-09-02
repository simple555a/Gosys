fblock LIMV;                    //一阶速度限制器
attribute(autoscan);
uuid{939c-df17-e255-6432};
var
	MAN:bool;in;            //手动端
	HALT:bool;in;           //保持端
	X:float;in;             //输入值
	RATE:float;in;          //变化率单位秒
	YMAX:float;in;          //上限
	YMIN:float;in;          //下限
	YMAN:float;in;          //手动给定
	
	Y:float;out;            //输出
	QMAX:bool;out;          //上限溢出
	QMIN:bool;out;          //下限溢出

implementation
static err;
static old_X;

void main()
{
	err = X - Y;
	date now;
	now = gettime();
	if(!MAN && !HALT){
		Y = X + err * RATE /100.0;
		old_X = Y;
		
	}else if(MAN){
		Y = YMAN;
		old_X = Y;
	}else if(HALT){
		Y = old_X;
	}
	old_X = Y;
	
	if(Y > YMAX){
		QMAX = 1;
		Y = YMAX;
	}else{
		QMAX = 0;
	}
	if(Y < YMIN){
		QMIN = 1;
		Y = YMIN;
	}else{
		QMIN = 0;
	}
		
		
}		

