fblock D_DS_VAL;
attribute(autoscan);
uuid{f880-fa5c-fb2e-3603};
var
  	AM:bool;in;	//�Զ�/�ֶ�ģʽ
	KO:bool;in;	//����Ա��ָ��
	AO:bool;in; 	//�Զ���ָ��
	PO:bool;in;	//������ָ��
	OPN:bool;in;	//������
	KO_FB:bool;in;	//��ֹ����Ա��ָ��
	LO:bool;in;	//��ֹ��ָ��
	FO:bool;in;	//��������
	MPP:bool;in;	//��ͣ/���ϸ�λָ��
	LC:bool;in;	//��ֹ��ָ��
	KC_FB:bool;in;	//��ֹ����Ա��ָ��
	FZ:bool;in;	//���������
	CLS:bool;in;	//�ط���
	PC:bool;in;	//������������ָ��
	AC:bool;in;	//�Զ���ָ��
	KC:bool;in;	//����Ա��ָ��
	PS:bool;in;	//��Դ����
	TD:date;in;	//������ʱ�趨
	
	ONENO:bool;out;		//������������
	ONCMD:bool;out;		//�򿪣�1������
	OFFCMD:bool;out;	//�ر�����������
	OFFENO:bool;out;	//�رգ�1������
	TIMOUT:bool;out;	//������ʱ����
	ETD:date;out;
			
implementation
static bool OP,CL,OpenSet,OpenReset,OpenQ,CloseSet,CloseReset,CloseQ;
static bool TDSet,MPPTDSet,old_TD,old_MPPTD,TDQ,MPPTDQ;
static date due_TD,due_MPPTD;
static bool MPPSet,MPPReset,MPPQ;

void main()
{
	//�ۺ�ָ��
	OP = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO) && FO && !PS && !LO && !PC) && !OPN;
	CL = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC) && FZ && !PS && !LC) && !CLS;
	ONENO = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO)&& !OP);
	OFFENO = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC)&& !CL);
	OpenSet = (OP && !CloseSet);
	OpenReset = (OPN || MPP || KC);
	CloseSet = (CL && !OpenSet);
	CloseReset = (CLS || MPP || KO);
	
	//������
	if(OpenSet){
		OpenQ = 1;
	}else if(OpenReset){
		OpenQ = 0;
	}
	ONCMD = OpenQ && !TIMOUT;
	//�ر�����
	if(CloseSet){
		CloseQ = 1;
	}else if(CloseReset){
		CloseQ = 0;
	}
	OFFCMD = CloseQ && !TIMOUT;
	
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
	
	if(MPPTDSet){
		if(!old_MPPTD){
			due_MPPTD = now + 20000;
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
}