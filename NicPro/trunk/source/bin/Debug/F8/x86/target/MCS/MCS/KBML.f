fblock KBML;
uuid{d530-49ca-3818-b5bf};
var
     TS:bool;in;
     TR:float;in;
     PB:float;in;
     YH:float;in;
     YL:float;in;
     OUT:float;out;
     outc:float;out;  

implementation

void main()
{
 	if(G){
		OUT = MAN;
		outc = MAN;
		return;
	}else{
		OUT = X;
		outc = X;
 	}
}