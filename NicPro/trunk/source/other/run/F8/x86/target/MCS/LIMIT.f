fblock LIMIT;            //限幅输出模块
attribute(autoscan);
uuid{147d-9f52-81a2-b8b0};

var
	MIN : float;in;  //输出下限
	IN : float;in;   //输入值
	MAX : float;in;	 //输出上限
	OUT : float;out; //输出值
	ERR : bool;out;  //上下限错误
	
implementation

void main()
{
	if(MIN > MAX){
		ERR = 1;
		OUT = 0;
	}else{	
		ERR = 0;
		if(IN <= MAX){
			if(IN >= MIN){
				OUT = IN;
			}else{
				OUT = MIN;
			}
		}else{
			OUT = MAX;
		}
	}
}