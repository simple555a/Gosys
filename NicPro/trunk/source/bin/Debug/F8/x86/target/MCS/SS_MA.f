fblock SS_MA;		//��׼ģ�����ֲ���
uuid{2d54-455e-2c62-d8ef};
var
	X: float; in;		//������
	OV: float; in;		//����������
	OS: bool; in;		//���ۿ���
	M_A: bool; in;		//���Զ��л�
	YH: float; in;		//Y����
	YL: float; in;		//Y����
	L: bool; in;		//���β���(���ڼ��޵�״̬)
	Yman: float; in;	//�ֶ�ֵ����

	Y: float; out;		//MAվ�������������
	y_out:float;out;
	S: bool; out;		//״̬���1:�Զ�,0:�ֶ�
	Lock: bool;out;		//����״̬

implementation

static float limit_y(float t)
{
	if(t > YH){
		t = YH;
	}else if(t < YL){
		t = YL;
	}
	return t;
}

void main()
{
	//�����ź����ȼ����
	if(OS){
		Y = limit_y(OV);
		y_out = limit_y(OV);
		return;
	}


	//���Զ�״̬�ж�
	if(M_A){
		S = 1;
	}else{
		S = 0;
	}


	//���޹���״̬	
	if(L){
		Lock = 1;
		return;
	}
	
	Lock = 0;
	

	//�ֶ�״̬
	if(!S){
		Y = limit_y(Yman);
		y_out = limit_y(Yman);
		return;
	}
	//�Զ�״̬
	if(S){
		Y = limit_y(X);
		y_out = limit_y(X);
	}
}	