fblock RATALM;            //���ʱ�����
attribute(autoscan);
uuid{0d30-49ca-3818-a5b2};

var
	X : float;in;     //��������
	PL :float;in;	  //����������
	NL :float;in;	  //����������
	D :bool;out; 	  //Խ������ָʾ
	D1 :bool;out;	  //Խ����������ָʾ
	D2 :bool;out;	  //Խ����������ָʾ

implementation
static float old_x,nPL,nNL;
static date due_T;

void main()
{
	static bool first_time = true;
	date now;
	float dt,dx;
	now = gettime();

	if(first_time){
		due_T = now;
		old_x = X;
		first_time = false;
		return;
	}
	
	dt = (float)(now - due_T);
	due_T = now;
	dx = X - old_x;
	
	
	if(dx >= 0){
		nPL = dx / dt * 60000;
		if(nPL > (PL + 0.5)){
			D1 = 1;
		}else if(nPL < (PL - 0.5)){
			D1 = 0;
		}
	}else{
		nNL = - dx / dt * 60000;
		if(nNL > (PL + 0.5)){
			D2 = 1;
		}else if(nNL < (PL - 0.5)){
			D2 = 0;
		}
	}
	old_x  = X;
	D = D1 + D2;
	
}