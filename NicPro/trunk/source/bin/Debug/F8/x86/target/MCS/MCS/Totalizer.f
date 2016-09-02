fblock Totalizer;
uuid{a429-353c-d013-e17a};
var
	IN     :float;  in;	//被积分的变量（必须>0）
	M_hold :bool;   in;	//积分挂起
	M_rst  :bool;   in;	//重新初始化功能块
	OUT    :float;  out;	//对IN积分的极值
	
	
implementation
static date old_T;
static float old_IN;
void main()
{	static bool first_time=1;
	date now,dt;
	now = gettime();
	
	if(first_time)
	 {old_T=gettime();
	  old_IN=IN;
	  first_time=0;}
	dt = fabs(now - old_T);
	
	if(M_rst){
		OUT = 0;
		return;
	}
	
	
	if(!M_hold){
		OUT = OUT + (old_IN+IN) * dt / 7200;
	}
	old_T = now;
	old_IN=IN;
}

