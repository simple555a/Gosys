fblock WIND_FIX;
uuid{d530-49ca-5858-b5bf};
var
     X1:float;in;	//风压输入
     Kx1:float;in;	//风压修正系数
     X2:float;in;	//风温输入
     Kx2:float;in;	//风温修正系数
     X3:float;in;	//风粉温度
     Kx:float;in;	//风粉计算修正输入
     kT:float;in;	//绝对温度系数
     Kc:float;in;	//操作修正系数
     Km:float;in;	//操作站修正系数输入
     OUT:float;out;	//修正计算输出
     

implementation
void  main()
{
 	OUT = Kc * Km * sqrt(X1 * Kx1 * (X2 + kT)/ (kT * Kx2))* (X3 + kT) / (X2 + kT);
 	
}