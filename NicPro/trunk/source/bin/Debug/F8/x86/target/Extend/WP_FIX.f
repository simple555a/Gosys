fblock WP_FIX;			//	��������
uuid{e10d-68cc-49d4-4398};
var
     	dP	:float;in;	//��ѹ��ѹ����
     	Pt	:float;in;	//����ѹ��
     	Ph	:float;in;	//ʹ�õش���ѹ
     	Tt	:float;in;	//�����¶�
    
	Q	:float;out;	//�����������
     	dPo	:float;out;	//��ѹ��ѹ���
     
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