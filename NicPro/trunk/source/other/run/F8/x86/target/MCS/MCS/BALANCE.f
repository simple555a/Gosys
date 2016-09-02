fblock BALANCE;      //二路输出平衡模块
uuid{1530-49ca-3818-a5bc};
var
     X:float;in;     //输入量
     DB:float;in;    //差值输入
     TR1:float;in;   //第一路跟踪量
     TS1:bool;in;    //第一路跟踪切换开关
     TR2:float;in;   //第二路跟踪量
     TS2:bool;in;    //第二路跟踪切换开关
     YH:float;in;    //Y1和Y2的上限
     YL:float;in;    //Y1和Y2的下限   
     	
     Y1:float;out;   //第一路平衡值输出
     Y2:float;out;   //第二路平衡值输出
     ERR:bool;out;   //上下限错误
implementation

void main()
{	
	if(YH < YL){
		Y1 = 0;
		Y2 = 0;
		ERR = 1;
	}else{
		ERR = 0;
		if((YL < Y1 < YH) && (YL < Y2 < YH)){
			if((TS1 == 1)&&(TS2 == 1)){
				Y1 = TR1;
				Y2 = TR2;
			}
			if((TS1 == 0)&&(TS2 == 0)){
				Y1 = X + DB;
				Y2 = X - DB;
			}
			if((TS1 == 1)&&(TS2 == 0)){
				Y1 = TR1;
				Y2 = 2 * X - Y1;
			}
			if((TS1 == 0)&&(TS2 == 1)){
				Y2 = TR2;
				Y1 = 2 * X - Y2;
			}
		}
		if(Y1 >= YH){
			Y1 = YH;	
		}
		if(Y2 >= YH){
			Y2 = YH;	
		}
		if(Y1 <= YL){
			Y1 = YL;	
		}
		if(Y2 <= YL){
			Y2 = YL;	
		}
	}
}