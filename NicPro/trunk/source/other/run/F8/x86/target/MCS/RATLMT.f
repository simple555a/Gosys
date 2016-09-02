fblock RATLMT;               //����������,����Concept�ٶ�����������
attribute(autoscan);
uuid{045d-9f52-51ae-b8b0};

var
	IN : float;in;       //��������������
	RATE : float;in;     //������ٶ�����
	OUT_MIN : float;in;  //�������
	OUT_MAX : float;in;  //�������
	TR_I : float;in;     //��ʼ������
	TR_S : bool;in;      //��ʼ�����ͣ�1������ģʽ0���Զ�ģʽ��
	
	OUT : float;out;     //�������������
	QMIN : bool;out;     //���������
	QMAX : bool;out;     //���������
	ERR : bool;out;      //�����޴���
	
implementation
static date due_T;

void main()
{
	static bool first_time = true;
	date now;
	float dt;
	float dy;
	now = gettime();

	if(first_time){
		due_T = now;
		first_time = false;
		return;
	}
	
	dt = (float)(now - due_T);
	due_T = now;
		
	if(OUT_MIN > OUT_MAX){
		ERR = 1;
		QMIN = 0;
		QMAX = 0;
		return;
	}else{
		ERR = 0;
	}

	if(TR_S){
		OUT = TR_I;
		if(OUT >= OUT_MAX){
			OUT = OUT_MAX;
			QMAX = 1;
		}
		if(OUT <= OUT_MIN){
			OUT = OUT_MIN;
			QMIN = 1;
		}
		if(OUT > OUT_MIN){
			QMIN = 0;
		}
		if(OUT < OUT_MAX){
			QMAX = 0;
		}
	}

	if(!TR_S){
		if(RATE == 0){
			OUT = IN;
			if(OUT >= OUT_MAX){
				OUT = OUT_MAX;
				QMAX = 1;	
			}
			if(OUT <= OUT_MIN){
				OUT = OUT_MIN;
				QMIN = 1;
			}
			if(OUT > OUT_MIN){
				QMIN = 0;
			}
			if(OUT < OUT_MAX){
				QMAX = 0;
			}
		}else if(OUT != IN){
			if(OUT > IN){
				dy = dt * RATE;
				OUT -= dy;
				QMIN = 0;
				QMAX = 0;
				if(OUT <= IN){
					OUT = IN;
				}
				if(OUT <= OUT_MIN){
					OUT = OUT_MIN;
					QMIN = 1;
				}
			}else{
				dy = dt * RATE;
				OUT += dy;
				QMIN = 0;
				QMAX = 0;
				if(OUT >= IN){
					OUT = IN;
				}
				if(OUT >= OUT_MAX){
					OUT = OUT_MAX;
					QMAX = 1;
				}
			}
		}else{
			OUT = IN;
			QMAX = 0;
			QMIN = 0;
			if(OUT >= OUT_MAX){  
				OUT = OUT_MAX;
				QMAX = 1;
				QMIN = 0;
			}else if(OUT <= OUT_MIN){
				OUT = OUT_MIN;
				QMIN = 1;
				QMAX = 0;
			}
		}
	}
}