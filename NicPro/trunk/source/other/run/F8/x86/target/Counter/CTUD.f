fblock CTUD;		//增减计数器
uuid{f150-19e2-8735-8888};
var
	CU: bool;in;	//递增触发器输入（上升沿）
	CD: bool;in;	//递减触发器输入（上升沿）
	R:  bool;in;	//复位
	LD: bool;in;	//加载数据
	PV: integer;in;	//预设值

	QU: bool;out;	//递增输出
	QD: bool;out;	//递减输出
	CV:integer;out;	//计数器输出（实际值）

implementation

static bool old_CU,old_CD;

void main()
{
	if(R && !LD){			//本算法为复位优先
		CV = 0;
		QU = 0;
		QD = 1;
		old_CU = 0;
	}
	
	if(LD && !R){
		CV = PV;
		QU = 1;
		QD = 0;
		old_CD = 0;
	}
	
	if(!R){
		if(CU && !old_CU){
			CV = CV + 1;			
		}
		
		if(CD && !old_CD){
			CV = CV - 1;	
		}
	}
	old_CU = CU;
	old_CD = CD;
	
	if(CV >= PV){
		QU = 1;
	}else if(CV <= 0){
		QD = 1;
	}else{
		QU = 0;
		QD = 0;
	}
}