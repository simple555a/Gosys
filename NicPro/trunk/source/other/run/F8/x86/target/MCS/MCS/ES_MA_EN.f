fblock ES_MA_EN;		//��׼ģ�����ֲ���
uuid{6381-affd-3316-aeaf};
var
	X	: float; 	in;		//������
	OV	: float; 	in;		//����������
	OS	: bool; 	in;		//���ۿ���
	MA	: bool; 	in;		//���Զ��л�
	Ymax	: float; 	in;		//Y����
	Ymin	: float; 	in;		//Y����
	L	: bool; 	in;		//���β���(���ڼ��޵�״̬)
	YB	: float;	in;		//ƫ������
	Yman	: float; 	in;		//�ֶ�ֵ����
	YURate	: float;	in;		//��ָ�������޷�
	YDRate	: float;	in;		//��ָ�������޷�
	Ex1	: float;	in;		//�ض�ֵ����1
	Ex2	: float;	in;		//�ض�ֵ����2
	Ex3	: float;	in;		//�ض�ֵ����3
	

	Y	: float; 	out;		//MAվ�������������
	y_man	: float;	out;		//ָ����������
	MSAM	: bool; 	out;		//״̬���1:�Զ�,0:�ֶ�
	YBo	: float;	out;		//ƫ�����
	ybo_out	: float;	out;		//��ǰƫ��
	Lock	: bool;		out;		//����״̬
	RateErr	: bool;		out;		//�������

implementation
static float old_Yman,yu,yd;


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
		
	//�����ź����ȼ����
	if(OS){
		Y = limit_y(OV);
		y_man = limit_y(OV);
		old_Yman = OV;
		RateErr = 0;
	}else{

		if(Lock){
			return; 		//���޹���״̬	
		}else{
			if(!MSAM){		//�ֶ�״̬
				yu = Yman - old_Yman;
				yd = old_Yman - Yman;
				if((Yman == Ex1) ||(Yman == Ex2) || (Yman == Ex3)){
					Y = limit_y(Yman);
					y_man = limit_y(Yman);
					YBo = Y - X;
					ybo_out = Y - X;
					RateErr = 0;
					old_Yman = Yman;
				}else if((yu > YURate) || (yd > YDRate)){
					Y = limit_y(old_Yman);
					y_man = limit_y(old_Yman);
					RateErr = 1;
				}else{
					Y = limit_y(Yman);
					y_man = limit_y(Yman);
					YBo = Y - X;
					ybo_out = Y - X;
					RateErr = 0;
					old_Yman = Yman;
				}
				return;
			}
			if(MSAM){		//�Զ�״̬
				YBo = YB;
				ybo_out = YB;
				Y = limit_y(X + YB);
				y_man = limit_y(X + YB);
				RateErr = 0;
				old_Yman = X + YB;
			}
		}
	}
}	