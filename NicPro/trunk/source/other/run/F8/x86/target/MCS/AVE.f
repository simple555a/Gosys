fblock AVE;		//������˼�Ȩƽ��ֵ

uuid{d530-4cca-3818-a5bc};
var
	IN1:float;in;	//����ֵ1
	IN2:float;in;	//����ֵ2
	IN3:float;in;	//����ֵ3
	IN4:float;in;	//����ֵ4
	K1 :float;in;	//��Ȩϵ��1
	K2 :float;in;	//��Ȩϵ��2
	K3 :float;in;	//��Ȩϵ��3
	K4 :float;in;	//��Ȩϵ��4
	
	OUT:float;out;		//��Ȩƽ��ֵ
	W_SUM:float;out;	//��Ȩ��
	K_SUM:float;out;	//ϵ����
	ERR:bool;out;		//ϵ������
implementation

void main()
{
	K_SUM = K1+K2+K3+K4��
	if(K_SUM == 0){
		ERR = 1;
	}else{
		ERR = 0;
		OUT = (K1*IN1+K2*IN2+K3*IN3+K4*IN4) / K_SUM;
	}
	W_SUM = K1*IN1+K2*IN2+K3*IN3+K4*IN4;
}