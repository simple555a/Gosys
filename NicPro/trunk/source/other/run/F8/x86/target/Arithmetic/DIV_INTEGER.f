fblock DIV_INTEGER;
uuid{9882-6752-9849-e190};
var
	IN1:integer; in;
	IN2:integer; in;
	OUT:integer; out;
implementation
void main()
{
	if(IN2 == 0){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
