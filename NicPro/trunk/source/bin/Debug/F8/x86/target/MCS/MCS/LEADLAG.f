fblock LEADLAG;		//��ǰ�ͺ�ģ��
uuid{dd30-49ca-3818-a5bc};
var
	X :float;in;	//����ֵ
	MAN :bool;in;	//�����л�����
	HALT :bool;in;  //�жϲ���
	GAIN :float;in;	//����
	LEAD :date;in;	//��ǰʱ�䳣��
	LAG :date;in;	//�ͺ�ʱ�䳣��
	YMAX :float;in; //�������
	YMIN :float;in; //�������
	YMAN :float;in;	//��������
	
	Y :float;out;	//���ֵ
	QMAX :bool;out;	//���ޱ���
	QMIN :bool;out;	//���ޱ���
	ERR :bool;out;	//�ߵ��޴���

implementation
static float K1;
static float K2;
static float K3;
static float old_X;
static float old_Y;
static date due_T;

void main()
{
	static bool first_time = true;
	date now;
	float dt;
	now = gettime();

	if(first_time){
		due_T = now;
		first_time = false;
		return;
	}
	
	dt = (float)(now - due_T);

	if(!MAN && !HALT){
		K1 = GAIN*(dt + LEAD)/(LAG + dt);
		K2 = GAIN*LEAD/(LAG + dt);
		K3 = LAG /(LAG + dt);
		Y = K1*X - K2*old_X + K3*old_Y;
	}else if(MAN){
		Y = YMAN;
	}
	
	if(Y > YMAX){
		Y = YMAX;
		QMAX = 1;
	}else{
		QMAX = 0;
	}
	
	if(Y < YMIN){
		Y = YMIN;
		QMIN = 1;
	}else{
		QMIN = 0;	
	}
	
	ERR = 0;
	if(YMAX < YMIN){
		ERR = 1;
		QMAX = 0;
		QMIN = 0;
		Y = 0;
	}
		
	old_X = X;
	old_Y = Y;
}