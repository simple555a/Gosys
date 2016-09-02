#
# ���õ������ģ��
#
fblock MTR_LTE;
attribute(autoscan);
uuid{f880-fa5c-fb2e-3601};
var
  	L_R:bool;in;	//Զ��/�͵��л�
	AM:bool;in;	//�Զ�/�ֶ��л�
	KO:bool;in;	//����Ա��բָ��
	AO:bool;in; 	//�Զ���բָ��
	PO:bool;in;	//������բָ��
	FO:bool;in;	//�����բ����
	RUN:bool;in;	//�����з���
	DIS:bool;in;	//�豸����
	STOP:bool;in;	//��ֹͣ����
	FZ:bool;in;	//������բ����
	PC:bool;in;	//������բָ��
	PZ:bool;in;	//������բָ��
	AC:bool;in;	//�Զ���բָ��
	KC:bool;in;	//����Ա��բָ��
	T1:date;in; 	//��բ��ʱʱ��
	T2:date;in;	//��բ��ʱʱ��

	STRENO:bool;out;	//��բ����������
	STRCMD:bool;out;	//��բ����
	MOTRDIS:bool;out;	//�豸��������
	STPCMD:bool;out;	//��բ����
	STPENO:bool;out;	//��բ����������
	ET1:date;out;		
	ET2:date;out;
		
implementation

static bool OP,CL,Q1,Q2,Q3,R1,R2,R3,OVERTIME;
static date dueTime1,dueTime2,myT1,myT2;
static bool old_T1,old_T2,T1Q,T2Q;
static bool lastRun,runFallingEdge;

void main()
{
	//
	if(!RUN && lastRun)
		runFallingEdge=1;
	else
		runFallingEdge=0;
	lastRun=RUN;
	//��բ�ۺ�ָ��
	OP = (((AM && AO) || KO || PO) && FO && !PC && !DIS);
	STRENO = (((AM && AO) || KO || PO) && !OP);
	R1 = (STPCMD || MOTRDIS || L_R || runFallingEdge);
	if(R1){
		Q1 = 0;
	}else if((OP || RUN)){
		Q1 = 1;
	}
	
	if(R1){
		Q2 = 0;
	}else if(RUN){
		Q2 = 1;
	}
	STRCMD = ((Q1 && !RUN) && !Q2);
	//��բ�ۺ�ָ��
	CL = ((((AM && AC) || KC || PZ) && FZ && !DIS) || PC);
	STPENO = (((AM && AC) || KC || PZ) && !(((AM && AC) || KC || PZ) && FZ && !DIS)); 
	R2 = (OP || CL);
	R3 = (STOP || Q3 || STRCMD || L_R);
	
	if(R2){
		Q3 = 0;
	}else if(OVERTIME){
		Q3 = 1;
	}
	
	if(R3){
		STPCMD = 0;
	}else if(CL){
		STPCMD = 1;
	}
	//���������ж�
	date now;
	now = gettime();
	myT1 = T1;
	myT2 = T2;
	if(STRCMD){
		if(!old_T1){
			dueTime1 = now + myT1;
			ET1 = 0;
		}else if(now > dueTime1){
			T1Q = 1;
			ET1 = myT1;
		}else{
			T1Q = 0;
			ET1 = myT1 + now - dueTime1; 
		}
	}else{
		T1Q = 0;
		ET1 = 0;
	}
	old_T1 = STRCMD;
	if(STPCMD){
		if(!old_T2){
			dueTime2 = now + myT2;
			ET2 = 0;
		}else if(now > dueTime2){
			T2Q = 1;
			ET2 = myT2;
		}else{
			T2Q = 0;
			ET2 = myT2 + now - dueTime2;
		}
	}else{
		T2Q = 0;
		ET2 = 0;
	}
	old_T2 = STPCMD;
	
	if(R2){
		MOTRDIS = 0;
	}else if(((STOP && RUN) || OVERTIME || T1Q)){
		MOTRDIS = 1;
	}	
	OVERTIME = T2Q;
}
