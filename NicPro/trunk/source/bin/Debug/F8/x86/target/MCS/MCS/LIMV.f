fblock LIMV;                    //һ���ٶ�������
attribute(autoscan);
uuid{939c-df17-e255-6432};
var
	MAN:bool;in;            //�ֶ���
	HALT:bool;in;           //���ֶ�
	X:float;in;             //����ֵ
	RATE:float;in;          //�仯�ʵ�λ��
	YMAX:float;in;          //����
	YMIN:float;in;          //����
	YMAN:float;in;          //�ֶ�����
	
	Y:float;out;            //���
	QMAX:bool;out;          //�������
	QMIN:bool;out;          //�������

implementation
static err;
static old_X;

void main()
{
	err = X - Y;
	date now;
	now = gettime();
	if(!MAN && !HALT){
		Y = X + err * RATE /100.0;
		old_X = Y;
		
	}else if(MAN){
		Y = YMAN;
		old_X = Y;
	}else if(HALT){
		Y = old_X;
	}
	old_X = Y;
	
	if(Y > YMAX){
		QMAX = 1;
		Y = YMAX;
	}else{
		QMAX = 0;
	}
	if(Y < YMIN){
		QMIN = 1;
		Y = YMIN;
	}else{
		QMIN = 0;
	}
		
		
}		

