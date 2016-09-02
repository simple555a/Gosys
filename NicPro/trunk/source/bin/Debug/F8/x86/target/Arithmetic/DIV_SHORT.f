fblock DIV_SHORT;
uuid{7f04-e880-6149-8877};
var
	IN1:short; in;
	IN2:short; in;
	OUT:short; out;
implementation
void main()
{
	if(IN2 == 0){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
