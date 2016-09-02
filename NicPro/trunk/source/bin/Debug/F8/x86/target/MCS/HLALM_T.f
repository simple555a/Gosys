fblock HLALM_T;
attribute(autoscan);
uuid{a96f-3623-4d66-041f};

var
     	X :float;in;
     	H :float;in;
     	L :float;in;
     	HDB :float;in;
	LDB :float;in;
	X_Bad:bool;in;
	HTm:date;in;
	LTm:date;in;
	
	D1 :bool;out;
	D2 :bool;out;

	HEt:date;out;
	LEt:date;out;
	
implementation

static date due_HTm,due_LTm;
static bool S1,S2,old_S1,old_S2;

void main()
{
 	date now;
	now = gettime();
	
 	if((HDB < 0) || (LDB < 0) || (H < L) || X_Bad){
 		D1 = 0;
 		D2 = 0;
 		S1 = 0;
		S2 = 0;
 		return;
     	}
     	if(X > (H + HDB)){
     		S1 = 1;
     		S2 = 0;
     		D2 = 0;
	}else if(X < (H - HDB)){
		S1 = 0;
		D1 = 0;
	}
	
	if(X < (L - LDB)){
     		S2 = 1;
     			
	}else if(X > (L + LDB)){
		S2 = 0;
		D2 = 0;
	}
	
	if(S1){
		if(!old_S1){
			// positive edge detected
			due_HTm = now + HTm;
			HEt = 0;
		}else if(now > due_HTm){
				D1 = 1;
			HEt = HTm;
		}else{
			D1 = 0;
			HEt = HTm + now - due_HTm;
		}
		}else{
			D1 = 0;
			HEt = 0;
		}
	old_S1 = S1;
	if(S2){
		if(!old_S2){
			// positive edge detected
			due_LTm = now + LTm;
			LEt = 0;
		}else if(now > due_LTm){
			D2 = 1;
			LEt = LTm;
		}else{
			D2 = 0;
			LEt = LTm + now - due_LTm;
		}
		}else{
			D2 = 0;
			LEt = 0;
		}
	old_S2 = S2;
}