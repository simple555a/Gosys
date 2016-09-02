fblock MODSTATE;
uuid{b8ec-e1cf-3606-b508};

var
  	ADDR:integer;in;
  	IN1:integer;in;
  	IN2:integer;in;
  	IN3:integer;in;
  	IN4:integer;in;
  	  	
  	EXIST:bool;out;
  	OUT1:integer;out;
  	OUT2:integer;out;
  	OUT3:integer;out;
  	OUT4:integer;out;
implementation

int main()
{
	unsigned short *p;
	
	p = io_mem(0) + 13824 + ADDR * 120;
	OUT1 = *p;
	p++;
	OUT2 = *p;
	p++;
	OUT3 = *p;
	p++;
	OUT4 = *p;
	p++;
	
	if((IN1 == OUT1) && (IN2 == OUT2) &&(IN3 == OUT3) &&(IN4 == OUT4)){
		EXIST = 1;
	}else{
		EXIST = 0;
	}
}
		
	
	
