fblock COMP_DB;		//�������ʹ��͵ıȽ���
uuid{d566-49ca-3818-a5bc};
var
     IN1:float;in;	//����1
     IN2:float;in;	//����2
     DBAND:float;in;	//������
     HYST:float;in;	//������
     
     GREATER:bool;out;	//����1��������2
     EQUAL:bool;out;	//����1��������2
     LESS:bool;out;	//����1С������2 
implementation

void main()
{
	if(DBAND <= 0){
		if(IN1 > IN2){
			GREATER = 1;
			EQUAL = 0;
			LESS = 0;
		}else if(IN1 == IN2){
			GREATER = 0;
			EQUAL = 1;
			LESS = 0;			
		}else{
			GREATER = 0;
			EQUAL = 0;
			LESS = 1;
		}
	}else if((HYST <= 0) || (HYST >= DBAND)){
		if((IN1 - IN2) > DBAND){
			GREATER = 1;
			EQUAL = 0;
		}else{
			GREATER = 0;
			if((IN2 - IN1) > DBAND){
				LESS = 1;
				EQUAL = 0;
			}else{
				LESS = 0;
				EQUAL = 1;
			}
		}
	}else{
		if((IN1 - IN2) >= DBAND){
			GREATER = 1;
			LESS = 0;
			EQUAL = 0;
		}else if((IN2 - IN1) >= DBAND){
			GREATER = 0;
			LESS = 1;
			EQUAL = 0;
		}else if((IN1 - IN2) > (DBAND - HYST)){
			GREATER = 0;
			LESS = 0;
			EQUAL = 0;
		}else if((IN2 - IN1) > (DBAND - HYST)){
			GREATER = 0;
			LESS = 0;
			EQUAL = 0;
		}else{
			GREATER = 0;
			LESS = 0;
			EQUAL = 1;
		}
	}
}