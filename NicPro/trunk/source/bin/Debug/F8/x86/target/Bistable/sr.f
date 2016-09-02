fblock SR;
uuid{ef6f-b286-a687-1142};
var
	S : bool; in;
	R : bool; in;
	Q : bool; out;
implementation
void main()
{
	if(S){
		Q = 1;
	}else if(R){
		Q = 0;
	}
}
