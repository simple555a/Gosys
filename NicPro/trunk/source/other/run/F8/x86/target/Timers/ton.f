#
#Delay Startup
#
fblock TON;
attribute(autoscan);
uuid{1976-0524-bb8c-070a};
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
	if(S){
		if(!old_S){
			// positive edge detected
			due_T = now + T;
			ET = 0;
		}else if(now > due_T){
			Q = 1;
			ET = T;
		}else{
			Q = 0;
			ET = T + now - due_T;
		}
	}else{
		Q = 0;
		ET = 0;
	}
	old_S = S;
}