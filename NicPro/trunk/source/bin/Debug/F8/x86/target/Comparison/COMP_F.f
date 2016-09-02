fblock COMP_FLOAT;
uuid{8f83-95dc-a925-fe31};
var
	IN:float; in;

	Max3:float; in;
	Max2:float; in;
	Max1:float; in;
	Min1:float; in;
	Min2:float; in;
	Min3:float; in;
	Bad:bool;in;
	
	OUT:float;out;
	H3:bool; out;
	H2:bool; out;
	H1:bool; out;
	L1:bool; out;
	L2:bool; out;
	L3:bool; out;

implementation
void main()
{
	if(!Bad){
		if(IN > Max3){
			H3 = 1;
		}else{
			H3 = 0;
		}
		if(IN > Max2){
			H2 = 1;
		}else{
			H2 = 0;
		}
		if(IN > Max1){
			H1 = 1;
		}else{
			H1 = 0;
		}
		
		if(IN < Min1){
			L1 = 1;
		}else{
			L1 = 0;
		}
		if(IN < Min2){
			L2 = 1;
		}else{
			L2 = 0;
		}
		if(IN < Min3){
			L3 = 1;
		}else{
			L3 = 0;
		}
	}else{
		H3 = 0;
		H2 = 0;
		H1 = 0;
		L1 = 0;
		L2 = 0;
		L3 = 0;
	}
	OUT = IN;
}
