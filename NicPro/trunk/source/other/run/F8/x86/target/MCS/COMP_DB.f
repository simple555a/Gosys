fblock COMP_DB;		//带死区和磁滞的比较器
uuid{d566-49ca-3818-a5bc};
var
     IN1:float;in;	//输入1
     IN2:float;in;	//输入2
     DBAND:float;in;	//死区限
     HYST:float;in;	//磁滞限
     
     GREATER:bool;out;	//输入1大于输入2
     EQUAL:bool;out;	//输入1等于输入2
     LESS:bool;out;	//输入1小于输入2 
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