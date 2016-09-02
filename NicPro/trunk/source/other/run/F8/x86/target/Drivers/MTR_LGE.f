#
# ���õ������ģ��һ
#
fblock MTR_LGE;
attribute(autoscan);
uuid{ff29-8bbb-ab11-8be2};
var
  	L_R	:bool;in;	//Զ��/�͵��л�:1=�͵�
	AM	:bool;in;	//�Զ�/�ֶ��л�
	KO	:bool;in;	//����Ա��բָ��
	AO	:bool;in; 	//�Զ���բָ��
	PO	:bool;in;	//������բָ��
	FO	:bool;in;	//�����բ����
	LO	:bool;in;	//��ֹ����
	RUN	:bool;in;	//�����з���
	DIS	:bool;in;	//�豸����
	STOP	:bool;in;	//��ֹͣ����
	LC	:bool;in;	//��ֹֹͣ
	FZ	:bool;in;	//������բ����
	PC	:bool;in;	//������բָ��
	PZ	:bool;in;	//������բָ��
	AC	:bool;in;	//�Զ���բָ��
	KC	:bool;in;	//����Ա��բָ��
	MPP	:bool;in;	//���ϼ���ʱȷ��
	FIX	:bool;in;	//����״̬
	MT	:bool;in;	//1:��֤״̬������������������֤״̬��������
	TON	:date;in; 	//��բ��ʱʱ��
	TOFF	:date;in;	//��բ��ʱʱ��

	STRENO	:bool;out;	//��բ����������
	STRCMD	:bool;out;	//��բ����
	MOTRDIS	:bool;out;	//�豸��������
	STPCMD	:bool;out;	//��բ����
	STPENO	:bool;out;	//��բ����������
	DTrip	:bool;out;	//������բ������жϣ�
	Status	:integer;out;	//�豸״̬
	V_FO	:bool;out;	//������
	V_FZ	:bool;out;	//����ͣ
	V_PO	:bool;out;	//������
	V_PC	:bool;out;	//����ͣ
	ETon	:date;out;	//��������ִ��ʱ��
	EToff	:date;out;	//ֹͣ����ִ��ʱ��
		
implementation

static bool OP,CL,Q1,Q2,Q3,R1,R2,R3,OVERTIME;
static date dueTime1,dueTime2,myT1,myT2,dueTimeDTrip,TimeDTrip;
static bool old_T1,old_T2,T1Q,T2Q,set_DTrip,old_RUN;
static bool lastRun,runFallingEdge;
static bool old_DTrip;

void main()
{
	//
	if(!RUN && lastRun){
		runFallingEdge = 1;
	}else{
		runFallingEdge = 0;
	}
	lastRun = RUN;
	
	//��բ�ۺ�ָ��
	if(!MT){
		OP = (((AM && AO) || KO || PO) && FO && !PC && !DIS && !LO && !FIX);
	}else{
		OP = (((AM && AO) || KO || PO) && FO && !PC && !DIS && !LO && !FIX && STOP);
	}
	STRENO = (((AM && AO) || KO || PO) && !OP);
	R1 = (STPCMD || MOTRDIS || L_R || runFallingEdge || (MPP && !OP) || (Status > 2));
	V_FO = FO;
	V_FZ = FZ;
	
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
	V_PO = PO;
	
	//��բ�ۺ�ָ��
	if(!MT){
		CL = ((((AM && AC) || KC || PZ) && FZ && !DIS && !LC && !FIX) || PC);
	}else{
		CL = ((((AM && AC) || KC || PZ) && FZ && !DIS && !LC && !FIX && RUN) || PC);
	}
	STPENO = (((AM && AC) || KC || PZ) && !(((AM && AC) || KC || PZ) && FZ && !DIS && !LC && !FIX)); 
	
	R2 = (OP || CL);
	R3 = (STOP || Q3 || STRCMD || L_R || (MPP && !CL) || (Status > 2));
	
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
	V_PC = PC || PZ;
	
	//���������ж�
	date now;
	now = gettime();
	myT1 = TON;
	myT2 = TOFF;
	if(STRCMD){
		if(!old_T1){
			dueTime1 = now + myT1;
			ETon = 0;
		}else if(now > dueTime1){
			T1Q = 1;
			ETon = myT1;
		}else{
			T1Q = 0;
			ETon = myT1 + now - dueTime1; 
		}
	}else{
		T1Q = 0;
		ETon = 0;
	}
	old_T1 = STRCMD;
	if(STPCMD){
		if(!old_T2){
			dueTime2 = now + myT2;
			EToff = 0;
		}else if(now > dueTime2){
			T2Q = 1;
			EToff = myT2;
		}else{
			T2Q = 0;
			EToff = myT2 + now - dueTime2;
		}
	}else{
		T2Q = 0;
		EToff = 0;
	}
	old_T2 = STPCMD;
	
	if(MPP && !OP && !CL){
		MOTRDIS = 0;
	}else if(((STOP && RUN) || OVERTIME || T1Q)){
		MOTRDIS = 1;
	}	
	OVERTIME = T2Q;
	
	//������բ�ж�
	set_DTrip = !CL && old_RUN && STOP && !MPP;
	if(MPP){
		DTrip = 0;
	}else{
		if(set_DTrip){
			if(!old_DTrip){
				dueTimeDTrip = now + 500;
				TimeDTrip = 0;
			}else if(now > dueTimeDTrip){
				DTrip = 1;
				TimeDTrip = 500;
			}else{
				DTrip = 0;
				TimeDTrip = 500 + now - dueTimeDTrip; 
			}
		}else{
			DTrip = 0;
			TimeDTrip = 0;
		}
		old_DTrip = set_DTrip;		
	}
	old_RUN = RUN;	
	
	Status = (integer)RUN * 1 + (integer)STOP * 2 + (integer)(DIS || MOTRDIS || DTrip) * 3;
	
}
