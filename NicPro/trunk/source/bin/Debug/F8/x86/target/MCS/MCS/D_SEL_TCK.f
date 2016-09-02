fblock D_SEL_TCK;	//Êý×ÖÁ¿¸ú×ÙÄ£¿é
uuid{ccbf-6bfc-6605-e407};
var
     G:bool;in;
     IN:bool;in;
     MAN:bool;in;
     Q:bool;out;
     Qc:bool;out;  

implementation

void main()
{
 	if(G){
		Q = MAN;
		Qc = MAN;
		return;
	}else{
		Q = IN;
		Qc = IN;
 	}
}