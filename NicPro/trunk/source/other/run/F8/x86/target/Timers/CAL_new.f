fblock CAL;		//����������
uuid{f150-19e2-8735-888d};
var
	IN_H: bool;in;	//��������
	IN_L: bool;in;	//��������
	SP: integer;in;	//�趨��ʼֵ
	H: integer;in;	//����
	L: integer;in;	//����
	S: bool;in;	//��λ�ˣ�ǿ�����ΪSP
	R: bool;in;	//��λ�ˣ�ǿ�����ȫΪ0

	OUT:float;out;	//�������ֵ(��ֱ��ת��Ϊ�����ͣ���������)
	QH: bool;out;	//Խ����
	QL: bool;out;	//Խ����

implementation

static bool old_h,old_l;
static integer sum;

void main()
{
	if(R){			//���㷨Ϊ��λ����
		OUT = 0;
		QH = 0;
		QL = 0;
		old_h = 0;
		old_l = 0;
		sum = 0;
		return;
	}
	
	if(S){
		OUT = SP;
		QH = 0;
		QL = 0;
		old_h = 0;
		old_l = 0;
		sum = 0;
		return;
	}
	
	if(!R){
		if(IN_H && !old_h && !QH){
			sum = sum + 1;			
		}
		
		if(IN_L && !old_l && !QL){
			sum = sum - 1;	
		}
	}
	old_h = IN_H;
	old_l = IN_L;
	
	if((sum + SP) >= H){
		OUT = (float)H;
		QH = 1;
		QL = 0;
	}else if((sum + SP) <= L){
		OUT = (float)L;
		QH = 0;
		QL = 1;
	}else{
		OUT = (float)(sum + SP);
		QH = 0;
		QL = 0;
	}
}