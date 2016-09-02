fblock WP_FIX;			//	风量计算
uuid{e10d-68cc-49d4-4398};
var
     	dP	:float;in;	//风压差压输入
     	Pt	:float;in;	//工况压力
     	Ph	:float;in;	//使用地大气压
     	Tt	:float;in;	//工况温度
    
	Q	:float;out;	//修正计算输出
     	dPo	:float;out;	//风压差压输出
     
implementation
void  main()
{       
 	dPo = dP;
 	if(((Ph + Pt) < 0.0000000001)||((dP * (Ph + 7600) * (273.15 + Tt))<0))
 	{
 		Q = 0.0;
 	}else{
 		Q = (81.447285 + 0.002094 * dP - 0.000000622371 * dP * dP) * sqrt((dP * (Ph + 7600) * (273.15 + Tt)) / (Ph + Pt));
 	}
}