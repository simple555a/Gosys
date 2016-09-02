fblock ERR_HL;

uuid{4d16-237e-1450-bcd1};

var
     	X :float;in;
     	H :float;in;
     	L :float;in;
     	HDB :float;in;
	LDB :float;in;
	X_Bad:bool;in;
		
	ERR:float;out;
	D1 :bool;out;
	D2 :bool;out;
		
implementation

void main()
{
 	ERR = X;	
 	if((HDB < 0) || (LDB < 0) || ((H - L) < HDB) || ((H - L) < LDB) || (H < L) || X_Bad){
 		D1 = 0;
 		D2 = 0;
 		return;
     	}
     	if(X > (H + HDB)){
     		D1 = 1;
	}else if(X < (H - HDB)){
		D1 = 0;
	}
	
	if(X < (L - LDB)){
     		D2 = 1;
     	}else if(X > (L + LDB)){
		D2 = 0;
	}
}