fblock FIRST;		//�����׳����ָʾ
uuid{db30-49ca-3818-a5bc};
var
	Z1:bool;in;	//#1��#16��������
	Z2:bool;in;
	Z3:bool;in;
	Z4:bool;in;
	Z5:bool;in;
	Z6:bool;in;
	Z7:bool;in;
	Z8:bool;in;
	Z9:bool;in;
	Z10:bool;in;
	Z11:bool;in;
	Z12:bool;in;
	Z13:bool;in;
	Z14:bool;in;
	Z15:bool;in;
	Z16:bool;in;
	Num:integer;in;	//�����뿪���и������ڵ��ڸ�ֵ����D1��1
	R:bool;in;	//��������1ʱ����λ
	Y:integer;out;	//�仯���������
	Yp:word;out;	//�����źŴ�����
	D:bool;out;	//�׳�ָʾ
	D1:bool;out;	//����������ʾ
implementation

static int set_first(int z)
{
	if(Y == 0){
		Y = z;
	}
}

static void clear_first()
{
	Y = 0;
}

void main()
{
	int n;
	
	n = 0;
	Yp = 0;
	if(Z1){
		set_first(1);
		Yp = Yp | 0x0001;
		n++;
	}
	if(Z2){
		set_first(2);
		Yp = Yp | 0x0002;
		n++;
	}
	if(Z3){
		set_first(3);
		Yp = Yp | 0x0004;
		n++;
	}
	if(Z4){
		set_first(4);
		Yp = Yp | 0x0008;
		n++;
	}
	if(Z5){
		set_first(5);
		Yp = Yp | 0x0010;
		n++;
	}
	if(Z6){
		set_first(6);
		Yp = Yp | 0x0020;
		n++;
	}
	if(Z7){
		set_first(7);
		Yp = Yp | 0x0040;
		n++;
	}
	if(Z8){
		set_first(8);
		Yp = Yp | 0x0080;
		n++;
	}
	if(Z9){
		set_first(9);
		Yp = Yp | 0x0100;
		n++;
	}
	if(Z10){
		set_first(10);
		Yp = Yp | 0x0200;
		n++;
	}
	if(Z11){
		set_first(11);
		Yp = Yp | 0x0400;
		n++;
	}
	if(Z12){
		set_first(12);
		Yp = Yp | 0x0800;
		n++;
	}
	if(Z13){
		set_first(13);
		Yp = Yp | 0x1000;
		n++;
	}
	if(Z14){
		set_first(14);
		Yp = Yp | 0x2000;
		n++;
	}
	if(Z15){
		set_first(15);
		Yp = Yp | 0x4000;
		n++;
	}
	if(Z16){
		set_first(16);
		Yp = Yp | 0x8000;
		n++;
	}
	
	if(n == 0 && R){
		clear_first();
	}
	
	if(n >= Num){
		D1 = true;
	}else{
		D1 = false;
	}
	
	if(Y == 0){
		D = false;
	}else{
		D = true;
	}
}