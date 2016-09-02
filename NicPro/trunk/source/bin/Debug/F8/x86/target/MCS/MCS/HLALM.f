fblock HLALM;
attribute(autoscan);
uuid{bdb9-ab23-05a2-c1cc};

var
     	X :float;in;
     	Max3 :float;in;
     	Max2 :float;in;
     	Max1 :float;in;
     	Min1 :float;in;
     	Min2 :float;in;
     	Min3 :float;in;
       	HDB :float;in;
	LDB :float;in;
	X_Bad:bool;in;
	
	Y  :float;out;
	H3 :bool;out;
	H2 :bool;out;
	H1 :bool;out;
	L1 :bool;out;
	L2 :bool;out;
	L3 :bool;out;
	
implementation

void main()
{
 	Y = X;
 	if(X_Bad || ((Max1 - Min1) <= HDB) || ((Max1 - Min1) <= LDB)){
 		H3 = 0;
 		H2 = 0;
 		H1 = 0;
 		L1 = 0;
 		L2 = 0;
 		L3 = 0;
 	}else{
 	  	if((X > Max3) && (Max3 > Max2)){
     			H3 = 1;
     		}else if(X < (Max3 - HDB)){
     			H3 = 0;
     		}
		
		if((X > Max2) && (Max2 > Max1)){
     			H2 = 1;
     		}else if(X < (Max2 - HDB)){
     			H2 = 0;
     		}
		
		if(X > Max1){
     			H1 = 1;
     		}else if(X < (Max1 - HDB)){
     			H1 = 0;
     		}
		
		if(X < Min1){
     			L1 = 1;
     		}else if(X < (Min1 + LDB)){
     			L1 = 0;
     		}
		
		if((X <	Min2) && (Min2 < Min1)){
     			L2 = 1;
     		}else if(X < (Min2 + HDB)){
     			L2 = 0;
     		}
	
		if((X <	Min3) && (Min3 < Min2)){
     			L3 = 1;
     		}else if(X < (Min3 + HDB)){
     			L3 = 0;
     		}
     	}
}