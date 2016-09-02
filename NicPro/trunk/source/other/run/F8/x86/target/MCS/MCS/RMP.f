fblock RMP;               //б���ź�ģ��
attribute(autoscan);
uuid{e4e6-8155-de70-694b};
var
	Base : float;in;  //����
	End : float;in;   //�յ�
	Rate : float;in;  //�仯��
	Hold : bool;in;   //����
	Rst : bool;in;    //��λ
	
	Y : float;out;    //���������
	D : bool;out;     //�����յ��ź�
implementation
static bool old_Rst;
static date due_T;

void main()
{
	static bool first_time = true;
	date now;
	float dt;
	float dy;
	now = gettime();

	if(first_time){
		Y = Base;
		D = 0;
		due_T = now;
		first_time = false;
		return;
	}
	
	dt = (float)(now - due_T);
	due_T = now;
	
	if(!Rst){
		Y = Base;
		D = 0;
	}else{
		if(!Hold){
			dy = dt * Rate;
			Y += dy;
		}
		if(Y >= End){
			Y = End;
			D = 1;
		}else{
			D = 0;
		}
	}
}