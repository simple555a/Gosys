#
#ʱ���ۼ�ģ��
#
fblock TIM_TOL;
attribute(autoscan);
uuid{b279-5efc-e810-b0ac};
var
	S : bool;in;	//�ۼƱ���
	R : bool;in;	//�ۼ�����
	P : bool;in;	//�ۼ���ͣ
	T : date;in;	//���ʱ�����
	OUT : float;out;	//�ۼ����
implementation
static bool old_SS,SS;
static float old_OUT;
static date due_T,ET;
void main()
{
	date now;
	now = gettime();
	SS = S && !P;
	if(R){
		OUT = 0;
	}else{
		if(SS){
			OUT = old_OUT + ET;
			if(!old_SS){
				due_T = now + T;
				ET = 0;
			}else if(now > due_T){
				ET = T;
			}else{
				ET = T + now - due_T;
			}
					
		}else{
			ET = 0;
			old_OUT = OUT;
		}
		old_SS = SS;
	}
}