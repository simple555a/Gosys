fblock DIV_DWORD;
uuid{216f-0b64-bf5b-6988};
var
	IN1:dword; in;
	IN2:dword; in;
	OUT:dword; out;
implementation
void main()
{
	if(IN2 == 0){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
