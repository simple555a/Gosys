fblock Totalizer;
uuid{a429-353c-d013-e17a};
var
	IN     :float;  in;	//�����ֵı���������>0��
	M_hold :bool;   in;	//���ֹ���
	M_rst  :bool;   in;	//���³�ʼ�����ܿ�
	OUT    :float;  out;	//��IN���ֵļ�ֵ
	
	
implementation
static date old_T;
static float old_IN;
void main()
{	static bool first_time=1;
	date now,dt;
	now = gettime();
	
	if(first_time)
	 {old_T=gettime();
	  old_IN=IN;
	  first_time=0;}
	dt = fabs(now - old_T);
	
	if(M_rst){
		OUT = 0;
		return;
	}
	
	
	if(!M_hold){
		OUT = OUT + (old_IN+IN) * dt / 7200;
	}
	old_T = now;
	old_IN=IN;
}

