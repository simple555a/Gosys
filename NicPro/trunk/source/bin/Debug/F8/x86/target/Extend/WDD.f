fblock WDD_FIX;
uuid{cf54-097f-3865-498d};
var
     T1:float;in;	//燃烧器一次风温
     T2:float;in;	//燃烧器煤粉温度
     T3:float;in;	//混合温度
     a1:float;in;	//Qp系数
     a2:float;in;	//Qp系数
     a3:float;in;	//Qp系数
     a4:float;in;	//Qp系数
     a5:float;in;	//Qp系数
     b1:float;in;	//Qc系数
     b2:float;in;	//Qc系数
     b3:float;in;	//Qc系数
     b4:float;in;	//Qc系数
     NTc:float;in;	//浓度修正系数
     NT:float;out;	//修正计算输出
     

implementation
static float tlp1,tlp3,tlc2,tlc3,QpT1,QpT3,QcT2,QcT3;

void  main()
{
 	tlp1 = 0.001 * T1 + 0.27313;
 	tlp3 = 0.001 * T3 + 0.27313;
 	tlc2 = T2 / 273.13;
 	tlc3 = T3 / 273.13;
 	
 	QpT1 = a1 * tlp1 * tlp1 * tlp1 * tlp1 + a2 * tlp1 * tlp1 * tlp1 + a3 * tlp1 * tlp1 + a4 * tlp1 + a5;
 	QpT3 = a1 * tlp3 * tlp3 * tlp3 * tlp3 + a2 * tlp3 * tlp3 * tlp3 + a3 * tlp3 * tlp3 + a4 * tlp3 + a5;
 	
 	QcT2 = b1 * tlc2 * tlc2 * tlc2 + b2 * tlc2 * tlc2 + b3 * tlc2 + b4 + 0.23;
 	QcT3 = b1 * tlc3 * tlc3 * tlc3 + b2 * tlc3 * tlc3 + b3 * tlc3 + b4 + 0.23;
 	
 	if((QcT3 * T3 - QcT2 * T2) <= 0){
 		NT = 0;
 	}else{
 	 	NT = (QpT1 * T1 - QpT3 * T3) / (QcT3 * T3 - QcT2 * T2) + NTc;
 	}
}