fblock CAL;		//增减计数器
uuid{f150-19e2-8735-888d};
var
	IN_H: bool;in;	//增计数端
	IN_L: bool;in;	//减计数端
	SP: integer;in;	//设定初始值
	H: integer;in;	//高限
	L: integer;in;	//低限
	S: bool;in;	//置位端，强制输出为SP
	R: bool;in;	//复位端，强制输出全为0

	OUT:float;out;	//输出计数值(已直接转换为浮点型，便于连接)
	QH: bool;out;	//越高限
	QL: bool;out;	//越低限

implementation

static bool old_h,old_l;
static integer sum;

void main()
{
	if(R){			//本算法为复位优先
		OUT = 0;
		QH = 0;
		QL = 0;
		old_h = 0;
		old_l = 0;
		sum = 0;
		return;
	}
	
	if(S){
		OUT = SP;
		QH = 0;
		QL = 0;
		old_h = 0;
		old_l = 0;
		sum = 0;
		return;
	}
	
	if(!R){
		if(IN_H && !old_h && !QH){
			sum = sum + 1;			
		}
		
		if(IN_L && !old_l && !QL){
			sum = sum - 1;	
		}
	}
	old_h = IN_H;
	old_l = IN_L;
	
	if((sum + SP) >= H){
		OUT = (float)H;
		QH = 1;
		QL = 0;
	}else if((sum + SP) <= L){
		OUT = (float)L;
		QH = 0;
		QL = 1;
	}else{
		OUT = (float)(sum + SP);
		QH = 0;
		QL = 0;
	}
}