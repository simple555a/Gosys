fblock POLYNOM;
uuid{d93f-b2eb-aac4-07b4};
var
     X:float;in;	//����ֵ
     C0:float;in;	//ϵ��1
     C1:float;in;	//ϵ��2
     C2:float;in;	//ϵ��3
     C3:float;in;	//ϵ��4
     C4:float;in;	//ϵ��5
     C5:float;in;	//ϵ��6
     
     Y:float;out;	//���ֵ

implementation

void  main()
{
	Y = C0 + C1*X + C2*X*X + C3*X*X*X + C4*X*X*X*X + C5*X*X*X*X*X;
}