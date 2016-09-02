fblock D_AS_VAL;
attribute(autoscan);
uuid{f1df-84ff-432a-4081};
var
  	AM:bool;in;	//�Զ�/�ֶ�ģʽ
	KO_S:bool;in;	//����Ա����ָ��
	KO_L:bool;in;	//����Ա����ָ��
	AO:bool;in; 	//�Զ���ָ��
	PO:bool;in;	//������ָ��
	DPO:bool;in;	//������ֱ�ӿ�ָ��
	KO_FB:bool;in;	//��ֹ����Ա��ָ��
	LO:bool;in;	//��ֹ��ָ��
	FO:bool;in;	//��������
	OPN:bool;in;	//������
	AI:float;in;	//��λ�����ź�
	MaxR:float;in;	//λ���ź�����
	MinR:float;in;	//λ���ź�����
	CLS:bool;in;	//�ط���	
	MPP:bool;in;	//��ͣ/���ϸ�λָ��
	FZ:bool;in;	//���������
	LC:bool;in;	//��ֹ��ָ��
	KC_FB:bool;in;	//��ֹ����Ա��ָ��
	PC:bool;in;	//������������ָ��
	AC:bool;in;	//�Զ���ָ��
	KC_L:bool;in;	//����Ա�س�ָ��
	KC_S:bool;in;	//����Ա�ض�ָ��
	PS:bool;in;	//��Դ����
	TD:date;in;	//������ʱ�趨
	
	ONENO:bool;out;		//������������
	ONCMD:bool;out;		//�򿪣�1������
	OFFCMD:bool;out;	//�ر�����������
	OFFENO:bool;out;	//�رգ�1������
	TIMOUT:bool;out;	//������ʱ����
	ETD:date;out;
			
implementation
static bool OP,CL,OpenSet,OpenReset,OpenQ,CloseSet,CloseReset,CloseQ,R1,R2,R3,R4;
static bool TDSet,KO_SSet,KC_SSet,old_TD,old_KO_STP,TDQ,KO_STPQ,old_KC_STP,KC_STPQ;
static date due_TD,due_KO_STP,due_KC_STP;
static bool MPPSet,MPPQ,DPOQ;

void main()
{
	//�ۺ�ָ��
	OP = (((AM && AO && !AC) || ((KO_S || KO_L) && !KO_FB && !(KC_L || KC_S))|| PO) && FO && !PS && !LO && !PC);
	CL = (((AM && AC && !AO) || ((KC_S || KC_L) && !KC_FB && !(KO_L || KO_S))|| PC) && FZ && !PS && !LC && !DPO);
	ONENO = (((AM && AO && !AC) || ((KO_S || KO_L) && !KO_FB && !(KC_L || KC_S))|| PO)&& !OP);
	OFFENO = (((AM && AC && !AO) || ((KC_S || KC_L) && !KC_FB && !(KO_L || KO_S))|| PC)&& !CL);
	R1 = (KO_S || PC || (AI > MaxR) || MPP || TIMOUT || CL || OPN);
	R2 = (OP || TIMOUT || (MinR > AI) || KC_S || PO || MPP || CLS);
	R3 = ((AI > MaxR) || (MinR > AI));
	R4 = ((AI > MaxR) || OPN || MPP);
	OpenSet = (OP && !CloseSet);
	CloseSet = (CL && !OpenSet);
		
	//������
	if(OpenSet){
		OpenQ = 1;
	}else if(R1){
		OpenQ = 0;
	}
	if(DPO) {
		DPOQ = 1;
	}else if(R4){
		DPOQ = 0;
	}
	
	ONCMD = (OpenQ || DPOQ || (KO_STPQ && OP));
	
	//�ر�����
	if(CloseSet){
		CloseQ = 1;
	}else if(R2){
		CloseQ = 0;
	}
	
	OFFCMD = (CloseQ || (KC_STPQ && CL));
	
	//���������ж�
	MPPSet = MPP;
	if(MPPSet){
		MPPQ = 1;
	}else if(R3){
		MPPQ = 0;
	}
	
	TDSet = (OpenQ || CloseQ || ((OpenQ && !MPPQ) || (CloseQ && !MPPQ)));
	date now;
	now = gettime();
	if(TDSet){
		if(!old_TD){
			due_TD = now + TD;
			ETD = 0;
		}else if(now > due_TD){
			TDQ = 1;
			ETD = TD;
		}else{
			TDQ = 0;
			ETD = TD + now - due_TD; 
		}
	}else{
		TDQ = 0;
		ETD = 0;
	}
	old_TD = TDSet;
	
	if(KO_SSet){
		if(!old_KO_STP){
			due_KO_STP = now + 100;
		}else if(now > due_KO_STP){
			KO_STPQ = 1;
		}else{
			KO_STPQ = 0;
		}
	}else{
		KO_STPQ = 0;
	}
	old_KO_STP = KO_S;
	
	if(KC_SSet){
		if(!old_KC_STP){
			due_KC_STP = now + 100;
		}else if(now > due_KC_STP){
			KC_STPQ = 1;
		}else{
			KC_STPQ = 0;
		}
	}else{
		KC_STPQ = 0;
	}
	old_KC_STP = KC_S;
	
	if(TDQ){
		TIMOUT = 1;
	}else if(MPP){
		TIMOUT = 0;
	}
}