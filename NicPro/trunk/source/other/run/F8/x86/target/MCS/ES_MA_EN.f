fblock ES_MA_EN;		//标准模拟量手操器
uuid{6381-affd-3316-aeaf};
var
	X	: float; 	in;		//输入量
	OV	: float; 	in;		//超驰输入量
	OS	: bool; 	in;		//超驰开关
	MA	: bool; 	in;		//手自动切换
	Ymax	: float; 	in;		//Y上限
	Ymin	: float; 	in;		//Y下限
	L	: bool; 	in;		//屏蔽操作(用于检修等状态)
	YB	: float;	in;		//偏置输入
	Yman	: float; 	in;		//手动值给定
	YURate	: float;	in;		//增指令输入限幅
	YDRate	: float;	in;		//减指令输入限幅
	Ex1	: float;	in;		//特定值输入1
	Ex2	: float;	in;		//特定值输入2
	Ex3	: float;	in;		//特定值输入3
	

	Y	: float; 	out;		//MA站输出，带上下限
	y_man	: float;	out;		//指令跟踪输出端
	MSAM	: bool; 	out;		//状态输出1:自动,0:手动
	YBo	: float;	out;		//偏置输出
	ybo_out	: float;	out;		//当前偏置
	Lock	: bool;		out;		//检修状态
	RateErr	: bool;		out;		//输入错误

implementation
static float old_Yman,yu,yd;


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
		
	//超驰信号优先级最高
	if(OS){
		Y = limit_y(OV);
		y_man = limit_y(OV);
		old_Yman = OV;
		RateErr = 0;
	}else{

		if(Lock){
			return; 		//检修挂起状态	
		}else{
			if(!MSAM){		//手动状态
				yu = Yman - old_Yman;
				yd = old_Yman - Yman;
				if((Yman == Ex1) ||(Yman == Ex2) || (Yman == Ex3)){
					Y = limit_y(Yman);
					y_man = limit_y(Yman);
					YBo = Y - X;
					ybo_out = Y - X;
					RateErr = 0;
					old_Yman = Yman;
				}else if((yu > YURate) || (yd > YDRate)){
					Y = limit_y(old_Yman);
					y_man = limit_y(old_Yman);
					RateErr = 1;
				}else{
					Y = limit_y(Yman);
					y_man = limit_y(Yman);
					YBo = Y - X;
					ybo_out = Y - X;
					RateErr = 0;
					old_Yman = Yman;
				}
				return;
			}
			if(MSAM){		//自动状态
				YBo = YB;
				ybo_out = YB;
				Y = limit_y(X + YB);
				y_man = limit_y(X + YB);
				RateErr = 0;
				old_Yman = X + YB;
			}
		}
	}
}	