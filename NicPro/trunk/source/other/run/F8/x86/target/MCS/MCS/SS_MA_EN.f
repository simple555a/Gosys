fblock SS_MA_EN;		//标准模拟量手操器
uuid{3c35-9d45-b95c-af2e};
var
	X	: float; 	in;		//输入量
	OV	: float; 	in;		//超驰输入量
	OS	: bool; 	in;		//超驰开关
	MA	: bool; 	in;		//手自动切换
	Ymax	: float; 	in;		//Y上限
	Yrat	: float;	in;
	Ymin	: float;	in;		//Y下限
	L	: bool; 	in;		//屏蔽操作(用于检修等状态)
	Yman	: float; 	in;		//手动值给定
	

	Y	: float; 	out;		//MA站输出，带上下限
	MSOUT	: float;	out;
	MSAM	: bool; 	out;		//状态输出1:自动,0:手动
	Lock	: bool;		out;		//检修状态
	RateErr	: bool;		out;		//手动输入越限

implementation
static float old_Yman,yt;

static float limit_y(float t)
{
	if(t > Ymax){
		t = Ymax;
	}else if(t < Ymin){
		t = Ymin;
	}
	return t;
}

void main()
{
	MSAM = MA;
	Lock = L;
	
	if(OS){
		Y = limit_y(OV);
		MSOUT = limit_y(OV);
		old_Yman = OV;
		RateErr = 0;
	}else{
	
		if(Lock){
			return;
		}else{
			//手动状态
			if(!MSAM){
				yt = fabs(Yman - old_Yman);
				if(yt > Yrat){
					Y = limit_y(old_Yman);
					MSOUT = limit_y(old_Yman);
					RateErr = 1;
				}else{
					Y = limit_y(Yman);
					MSOUT = limit_y(Yman);
					RateErr = 0;
					old_Yman = Yman;
				}
				return;
			}
			//自动状态
			if(MSAM){
				Y = limit_y(X);
				MSOUT = limit_y(X);
				old_Yman = X;
				RateErr = 0;
			}
		}
	}
}

