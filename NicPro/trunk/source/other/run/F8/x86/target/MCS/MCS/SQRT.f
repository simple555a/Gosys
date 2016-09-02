fblock SQRT;
uuid{c0a2-0255-b1c3-0fdb};
var
IN:float; in;
OUT:float; out;
implementation
void main()
{
	if(IN<=0){
		OUT = 0;	
	}else{
		OUT = sqrt(IN);
	}
}
