fblock DIV_DOUBLE;
uuid{81a0-1e4c-b0c7-8386};
var
	IN1:double; in;
	IN2:double; in;
	OUT:double; out;
implementation
void main()
{
	if(fabs(IN2) < 0.0000001){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
