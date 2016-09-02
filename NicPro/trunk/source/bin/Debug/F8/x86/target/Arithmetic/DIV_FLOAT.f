fblock DIV_FLOAT;
uuid{cc51-26b4-8617-da9d};
var
	IN1:float; in;
	IN2:float; in;
	OUT:float; out;
implementation
void main()
{
	if(fabs(IN2) < 0.0000001){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
