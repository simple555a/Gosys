fblock S_E_VAL;
uuid{f880-fa5c-fb2e-3602};
var
  	L_R:bool;in;	//Զ��/�͵��л�
	AM:bool;in;	//�Զ�/�ֶ�ģʽ
	
	KO:bool;in;	//����Ա��ָ��
	AO:bool;in; 	//�Զ���ָ��
	PO:bool;in;	//������ָ��
	
	LO:bool;in;
	FO:bool;in;	//��������
	
	LC:bool;in;
	FZ:bool;in;	//���������
	
	PC:bool;in;	//������������ָ��
	AC:bool;in;	//�Զ���ָ��
	KC:bool;in;	//����Ա��ָ��
	SAFE:bool;in;   //��ȫ״̬(1:����ȫ/0:�ذ�ȫ)
	
	ONENO:bool;out;	//������������
	ONCMD:bool;out;	//�򿪣�1������
implementation
static bool OP,CL,S,R;

void main()
{
	//��բ�ۺ�ָ��
	OP = (((AM && AO && !AC) || KO)&& FO && !LO && !PC && L_R);
	ONENO = (((AM && AO && !AC) || KO )&& !OP);
	S = OP || PO || (SAFE && !L_R);
	//��բ�ۺ�ָ��
	CL = (((AM && !AO && AC)|| KC)&& FZ && !LC && L_R);
	R = CL || PC || (!SAFE && !L_R);
	if(SAFE){
		if(S){
			ONCMD = 1;
		}else if(R){
			ONCMD = 0;
		}
	}else{
		if(R){
			ONCMD = 0;
		}else if(S){
			ONCMD = 1;
		}
	}
}

