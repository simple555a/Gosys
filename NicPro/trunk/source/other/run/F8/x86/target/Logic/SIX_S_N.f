fblock SIX_S_N;
uuid{41de-8563-40f6-dfbf};
var
	IN1:bool;in;
	IN2:bool;in;
	IN3:bool;in;
	IN4:bool;in;
	IN5:bool;in;
	IN6:bool;in;	
	SEL:integer; in;
	OUT:bool;out;

implementation
static integer in;


void main()
{ 
	in = (integer)IN1 + (integer)IN2 + (integer)IN3 + (integer)IN4 + (integer)IN5 + (integer)IN6;
	if((SEL <=0 ) || (SEL > 6)){
		OUT = 0;
	}else{
		if(in >= SEL )
		{ 
			OUT = 1;
		}else{
			OUT = 0;
		}
	}
}