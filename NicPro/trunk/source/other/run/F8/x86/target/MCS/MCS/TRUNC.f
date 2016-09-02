fblock TRUNC;
uuid{d530-49ca-3818-a5b0};
var
     
     X:float;in;
     R_MAX:float;in;
     MAX:float;in;
     MIN:float;in;
     R_MIN:float;in;
     OUT:float;out;
     QMAX:bool;out;
     QMIN:bool;out;
    
implementation

void main()
{
 	if(X > MAX){
		OUT = R_MAX;
		QMAX = 1;
		QMIN = 0;
	}else if(X < MIN){
		OUT = R_MIN;
		QMAX = 0;
		QMIN = 1;
	}else{
		OUT = X;
		QMAX = 0;
		QMIN = 0;
 	}
}