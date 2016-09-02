fblock SUM_W;		//三输入加权加法器
uuid{d530-49ca-3a18-a5bc};

var
     IN1:float;in;	//输入值1
     IN2:float;in;	//输入值2
     IN3:float;in;	//输入值3
     k1 :float;in;	//加权系数1
     k2 :float;in;	//加权系数2
     k3 :float;in;	//加权系数3
     c1 :float;in;	//计算系数
     OUT:float;out;	//输出值
implementation

void main()
{
	OUT = k1*IN1 + k2*IN2 + k3*IN3 + c1;
}