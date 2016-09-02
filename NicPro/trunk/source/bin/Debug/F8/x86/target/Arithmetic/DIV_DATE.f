fblock DIV_DATE;
uuid{d61d-6c40-53d5-d169};
var
	IN1:date; in;
	IN2:date; in;
	OUT:date; out;
implementation
void main()
{
	if(fabs(IN2) < 0.0000001){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
