fblock SUM_W;		//�������Ȩ�ӷ���
uuid{d530-49ca-3a18-a5bc};

var
     IN1:float;in;	//����ֵ1
     IN2:float;in;	//����ֵ2
     IN3:float;in;	//����ֵ3
     k1 :float;in;	//��Ȩϵ��1
     k2 :float;in;	//��Ȩϵ��2
     k3 :float;in;	//��Ȩϵ��3
     c1 :float;in;	//����ϵ��
     OUT:float;out;	//���ֵ
implementation

void main()
{
	OUT = k1*IN1 + k2*IN2 + k3*IN3 + c1;
}