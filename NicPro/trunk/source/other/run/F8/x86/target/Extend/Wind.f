fblock WIND_FIX;
uuid{d530-49ca-5858-b5bf};
var
     X1:float;in;	//��ѹ����
     Kx1:float;in;	//��ѹ����ϵ��
     X2:float;in;	//��������
     Kx2:float;in;	//��������ϵ��
     X3:float;in;	//����¶�
     Kx:float;in;	//��ۼ�����������
     kT:float;in;	//�����¶�ϵ��
     Kc:float;in;	//��������ϵ��
     Km:float;in;	//����վ����ϵ������
     OUT:float;out;	//�����������
     

implementation
void  main()
{
 	OUT = Kc * Km * sqrt(X1 * Kx1 * (X2 + kT)/ (kT * Kx2))* (X3 + kT) / (X2 + kT);
 	
}