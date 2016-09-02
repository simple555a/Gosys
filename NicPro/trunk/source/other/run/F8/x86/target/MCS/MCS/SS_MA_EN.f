fblock SS_MA_EN;		//��׼ģ�����ֲ���
uuid{3c35-9d45-b95c-af2e};
var
	X	: float; 	in;		//������
	OV	: float; 	in;		//����������
	OS	: bool; 	in;		//���ۿ���
	MA	: bool; 	in;		//���Զ��л�
	Ymax	: float; 	in;		//Y����
	Yrat	: float;	in;
	Ymin	: float;	in;		//Y����
	L	: bool; 	in;		//���β���(���ڼ��޵�״̬)
	Yman	: float; 	in;		//�ֶ�ֵ����
	

	Y	: float; 	out;		//MAվ�������������
	MSOUT	: float;	out;
	MSAM	: bool; 	out;		//״̬���1:�Զ�,0:�ֶ�
	Lock	: bool;		out;		//����״̬
	RateErr	: bool;		out;		//�ֶ�����Խ��

implementation
static float old_Yman,yt;

static float limit_y(float t)
{
	if(t > Ymax){
		t = Ymax;
	}else if(t < Ymin){
		t = Ymin;
	}
	return t;
}

void main()
{
	MSAM = MA;
	Lock = L;
	
	if(OS){
		Y = limit_y(OV);
		MSOUT = limit_y(OV);
		old_Yman = OV;
		RateErr = 0;
	}else{
	
		if(Lock){
			return;
		}else{
			//�ֶ�״̬
			if(!MSAM){
				yt = fabs(Yman - old_Yman);
				if(yt > Yrat){
					Y = limit_y(old_Yman);
					MSOUT = limit_y(old_Yman);
					RateErr = 1;
				}else{
					Y = limit_y(Yman);
					MSOUT = limit_y(Yman);
					RateErr = 0;
					old_Yman = Yman;
				}
				return;
			}
			//�Զ�״̬
			if(MSAM){
				Y = limit_y(X);
				MSOUT = limit_y(X);
				old_Yman = X;
				RateErr = 0;
			}
		}
	}
}
