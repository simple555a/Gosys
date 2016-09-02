fblock G_SEL_TCK;		//Ä£ÄâÁ¿¸ú×ÙÄ£¿é
uuid{d530-49ca-3818-a5bf};
var
     G:bool;in;
     X:float;in;
     MAN:float;in;
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