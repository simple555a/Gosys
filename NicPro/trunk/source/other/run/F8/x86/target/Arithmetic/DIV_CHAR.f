fblock DIV_CHAR;
uuid{740f-96ba-76f5-c48d};
var
	IN1:char; in;
	IN2:char; in;
	OUT:char; out;
implementation
void main()
{
	if(IN2 == 0){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
