fblock CTUD;		//����������
uuid{f150-19e2-8735-8888};
var
	CU: bool;in;	//�������������루�����أ�
	CD: bool;in;	//�ݼ����������루�����أ�
	R:  bool;in;	//��λ
	LD: bool;in;	//��������
	PV: integer;in;	//Ԥ��ֵ

	QU: bool;out;	//�������
	QD: bool;out;	//�ݼ����
	CV:integer;out;	//�����������ʵ��ֵ��

implementation

static bool old_CU,old_CD;

void main()
{
	if(R && !LD){			//���㷨Ϊ��λ����
		CV = 0;
		QU = 0;
		QD = 1;
		old_CU = 0;
	}
	
	if(LD && !R){
		CV = PV;
		QU = 1;
		QD = 0;
		old_CD = 0;
	}
	
	if(!R){
		if(CU && !old_CU){
			CV = CV + 1;			
		}
		
		if(CD && !old_CD){
			CV = CV - 1;	
		}
	}
	old_CU = CU;
	old_CD = CD;
	
	if(CV >= PV){
		QU = 1;
	}else if(CV <= 0){
		QD = 1;
	}else{
		QU = 0;
		QD = 0;
	}
}