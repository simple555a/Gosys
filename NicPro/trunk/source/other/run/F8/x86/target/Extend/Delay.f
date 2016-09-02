fblock Delay;
attribute(autoscan);
uuid{fd70-e13c-1e73-c029};
var
	IN: float; in;		//����ֵ
	T_D: date; in;		//�ӳ�ʱ��
	T_CY: date; in;		//����ʱ��(Ϊ��֤���ȣ�һ����Ϊɨ�����ڵ�5������)
	Set: bool;in;		//��ʱ��λ1:��ʱ���״̬0:����״̬
	Hold: bool;in;		//�������
	OUT: float; out;	//���ֵ

implementation
static integer N,i;
static date due_T;
static bool a,old_a;
static float x[100000];		//�ڲ�����Ĵ�������Ϊ10��,���T_D/T_CY���ܳ���10��

void main()
{
	static bool first_time = true;

	if(first_time){
		a = 1;
		old_a = 0;
		first_time = false;
		return;
	}
	
	N = (int)(T_D/T_CY);
	date now;
	now = gettime();

	if(i >= N){
		i = 0;
	}

	if(a && !old_a){
		due_T = now;
	}else if(a){
		if((now - due_T) >= T_CY){
			x[i] = IN;
			i++;
			due_T = now;
		}
	}
	old_a = a;

	if(Set && !Hold){	//��ֵ����
		if(i < (N - 2)){
			OUT = (x[i + 2] - x[i + 1]) * (now - due_T) / T_CY + x[i + 1];
		}else if(i == (N - 2)){
			OUT = (x[0] - x[i + 1]) * (now - due_T) / T_CY + x[i + 1];
		}else if(i == (N - 1)){
			OUT = (x[1] - x[0]) * (now - due_T) / T_CY + x[0];
		}
	}else if(!Set && !Hold){
		OUT = IN;
	}else{			//�������
		return;
	}
}