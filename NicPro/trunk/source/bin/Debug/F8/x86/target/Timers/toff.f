#
#Escape Delay
#
fblock TOFF;
attribute(autoscan);
uuid{c87d-88f6-0d60-fdb4};
var
	S : bool;in;
	T : date;in;
	Q : bool;out;
	ET: date;out;	
implementation
static bool old_S;
static date due_T;
void main()
{
	date now;
	now = gettime();	

	if(!S){
		if(old_S){
			// negative edge detected
			due_T = now + T;
			ET = 0;
		}else if(now > due_T){
			Q = 0;
			ET = T;
		}else{
			Q = 1;
			ET = T + now - due_T;
		}
	}else{
		Q = 1;
		ET = 0;
	}
	old_S = S;
}