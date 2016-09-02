fblock POLYNOM;
uuid{d93f-b2eb-aac4-07b4};
var
     X:float;in;	//输入值
     C0:float;in;	//系数1
     C1:float;in;	//系数2
     C2:float;in;	//系数3
     C3:float;in;	//系数4
     C4:float;in;	//系数5
     C5:float;in;	//系数6
     
     Y:float;out;	//输出值

implementation

void  main()
{
	Y = C0 + C1*X + C2*X*X + C3*X*X*X + C4*X*X*X*X + C5*X*X*X*X*X;
}