fblock DIV_WORD;
uuid{74c2-b6d8-a882-a10b};
var
	IN1:word; in;
	IN2:word; in;
	OUT:word; out;
implementation
void main()
{
	if(IN2 == 0){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}
