fblock AVE;		//四输入端加权平均值

uuid{d530-4cca-3818-a5bc};
var
	IN1:float;in;	//输入值1
	IN2:float;in;	//输入值2
	IN3:float;in;	//输入值3
	IN4:float;in;	//输入值4
	K1 :float;in;	//加权系数1
	K2 :float;in;	//加权系数2
	K3 :float;in;	//加权系数3
	K4 :float;in;	//加权系数4
	
	OUT:float;out;		//加权平均值
	W_SUM:float;out;	//加权和
	K_SUM:float;out;	//系数和
	ERR:bool;out;		//系数错误
implementation

void main()
{
	K_SUM = K1+K2+K3+K4；
	if(K_SUM == 0){
		ERR = 1;
	}else{
		ERR = 0;
		OUT = (K1*IN1+K2*IN2+K3*IN3+K4*IN4) / K_SUM;
	}
	W_SUM = K1*IN1+K2*IN2+K3*IN3+K4*IN4;
}