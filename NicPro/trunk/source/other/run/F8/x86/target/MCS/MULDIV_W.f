fblock MULDIV_W;            //三输入加权乘除法器
uuid{d5d1-49ca-3818-a5C1};

var
	IN1 :float;in;	   //输入值1
	IN2 :float;in;	   //输入值2
	IN3 :float;in;     //输入值3
	K :float;in;       //加权系数
	c1 :float;in;	   //计算系数1
	c2 :float;in;      //计算系数2
	c3 :float;in;      //计算系数3
	c4 :float;in;      //计算系数4
	OUT :float;out;    //输出值
	ERR :bool;out;     //除数为零，即计算系数3与输入值3之和为零
	
implementation

void main()
{
	if(IN3 == (-1)*c3){
		ERR = 1;
	}else{
		ERR = 0;
		OUT =(K * (IN1 + c1) * (IN2 + c2))/(IN3 + c3) + c4;
	}
}