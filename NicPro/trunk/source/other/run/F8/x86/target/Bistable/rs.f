fblock RS;
uuid{7a2c-cd54-730f-9ef1};
var
	S : bool; in;
	R : bool; in;
	Q : bool; out;
implementation
void main()
{
	if(R){
		Q = 0;
	}else if(S){
		Q = 1;
	}
}
