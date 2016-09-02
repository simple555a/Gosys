fblock D_DT_VAL;
attribute(autoscan);
uuid{a2c2-c36c-8c7e-9cb9};
var
  	AM	:bool;in;	//�Զ�/�ֶ�ģʽ
	KO	:bool;in;	//����Ա��ָ��
	AO	:bool;in; 	//�Զ���ָ��
	PO	:bool;in;	//������ָ��
	OPN	:bool;in;	//������
	FO	:bool;in;	//����������
	KO_FB	:bool;in;	//��ֹ����Ա��ָ��
	LO	:bool;in;	//��ֹ��ָ��
	MPP	:bool;in;	//��ͣ/���ϸ�λָ��
	LC	:bool;in;	//��ֹ��ָ��
	KC_FB	:bool;in;	//��ֹ����Ա��ָ��
	FZ	:bool;in;	//����������
	CLS	:bool;in;	//�ط���
	PC	:bool;in;	//������������ָ��
	AC	:bool;in;	//�Զ���ָ��
	KC	:bool;in;	//����Ա��ָ��
	DIS	:bool;in;	//�豸����
	L_R	:bool;in;	//1:�͵أ�0��Զ��
	TS	:bool;in;	//1:����ָ�0��ָ�������TD����
	TP	:date;in;	//����ָ�����
	TD	:date;in;	//������ʱ�趨
	
	ONENO	:bool;out;	//������������
	ONCMD	:bool;out;	//�򿪣�1������
	OFFCMD	:bool;out;	//�ر�����������
	OFFENO	:bool;out;	//�رգ�1������
	MPPT	:bool;out;	//��ͣ/��λ���
	TIMOUT	:bool;out;	//������ʱ����
	STU	:integer;out;	//�豸״̬
	V_FO	:bool;out;	//������
	V_FZ	:bool;out;	//������
	V_PO	:bool;out;	//���ۿ�
	V_PC	:bool;out;	//���۹�
	ETD	:date;out;	//������ʱ
			
implementation
static bool OP,CL,OpenSet,OpenReset,OpenQ,CloseSet,CloseReset,CloseQ;
static bool TDSet,MPPTDSet,old_TD,old_TP,old_MPPTD,TDQ,TPQ,MPPTDQ;
static date due_TD,due_MPPTD,due_TP;
static bool MPPSet,MPPReset,MPPQ;

void main()
{
	//�ۺ�ָ��
	OP = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO) && FO && !DIS && !L_R && !LO && !PC) && !OPN;
	CL = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC) && FZ && !DIS && !L_R && !LC) && !CLS;
	ONENO = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO)&& !OP);
	OFFENO = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC)&& !CL);
	V_FO = FO;
	V_FZ = FZ;
	
	
	OpenSet = (OP && !CloseSet);
	CloseSet = (CL && !OpenSet);
			
	//������
	if(OpenSet && !CloseSet){
		OpenQ = 1;
	}else if(OpenReset || CloseSet){
		OpenQ = 0;
	}
	V_PO = PO;
	
	//�ر�����
	if(CloseSet && !OpenSet){
		CloseQ = 1;
	}else if(CloseReset || OpenSet){
		CloseQ = 0;
	}
	if(!TS){
		ONCMD = OpenQ && !TIMOUT;
		OFFCMD = CloseQ && !TIMOUT;
	}else{
		ONCMD = OpenQ && TPQ;
		OFFCMD = CloseQ && TPQ;
	}
	V_PC = PC;
	
	//��ָͣ��
	MPPT = MPP;
	
	//���������ж�
	MPPSet = MPP;
	MPPReset = OpenSet || CloseSet;
	if(MPPSet){
		MPPQ = 1;
	}else if(MPPReset){
		MPPQ = 0;
	}
	
	TDSet = OpenQ || CloseQ;
	MPPTDSet = OPN && CLS && !MPPQ;
	OpenReset = ((OPN && !MPPTDSet) || MPP || KC || TIMOUT);
	CloseReset = ((CLS && !MPPTDSet) || MPP || KO || TIMOUT);
	
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
		if(!old_TP){
			due_TP = now + TP;
		}else if(now > due_TP){
			TPQ = 0;
		}else{
			TPQ = 1;
		}
	}else{
		TDQ = 0;
		TPQ = 1;
		ETD = 0;
	}
	old_TD = TDSet;
	old_TP = TDSet;
	
	if(MPPTDSet){
		if(!old_MPPTD){
			due_MPPTD = now + 20;
		}else if(now > due_MPPTD){
			MPPTDQ = 1;
		}else{
			MPPTDQ = 0;
		}
	}else{
		MPPTDQ = 0;
	}
	old_MPPTD = MPPTDSet;
	TIMOUT = TDQ || MPPTDQ;
	
	STU = (integer)OPN + (integer)CLS * 2 + (integer)(!(ONCMD + OFFCMD) && !(OPN + CLS) && !TIMOUT && !MPPT) * 3 + (integer)(DIS || TIMOUT) * 4;
	
}