fblock DIV_BYTE;
uuid{dbe6-bbc4-66a8-3bd5};
var
	IN1:byte; in;
	IN2:byte; in;
	OUT:byte; out;
implementation
void main()
{
	if(IN2 == 0){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
