fblock FILTER;			//�˲�ģ��
uuid{2f4b-b97c-de81-eb4d};
var
	X: float; in;		//����ֵ
	Mode: integer; in;	//�˲���ʽ0:���˲�1:ƽ�����˲�2:�ͽ��˲�3:�߽��˲�
	Y: float; out;		//���ֵ
implementation
static float old_X;
static float x[7];

void main()
{
	x[6] = X/8 + 7*x[5]/8;
	x[5] = X/7 + 6*x[4]/7;
	x[4] = X/6 + 5*x[3]/6;
	x[3] = X/5 + 4*x[2]/5;
	x[2] = X/4 + 3*x[1]/4;
	x[1] = X/3 + 2*x[0]/3;
	x[0] = X/2 + old_X/2;
	
	if(Mode == 0){			//���˲�ֱ�����
		Y = X;	
	}else if(Mode == 1){		//ƽ�����˲�ģʽ
		if(X >= 0.0){
			Y = sqrt(X);
		}else{
			Y = (-1) * sqrt((-1) * X);
		}	
	}else if(Mode == 2){		//�ͽ�(2)�˲�
		Y = x[0];
	}else if(Mode == 3){		//�߽�(8)�˲�
		Y = x[6];
	}else{				//��������,����
		return;
	}	 
	 old_X = X;
}