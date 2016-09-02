#
#Delay Startup
#
fblock TON_T;
attribute(autoscan);
uuid{06c3-08b1-487a-6fed};
var
	S : bool;in;
	T : date;in;
	Q : bool;out;
	ET: date;out;	
implementation
static bool old_S;
static date old_time;
void main()
{
	date now;
	now = gettime();
	if(S){
		if(!old_S){
			// positive edge detected
			old_time=gettime();
			ET = 0;
		}else if(now > (old_time + T)){
			Q = 1;
			ET = T;
		}else{
			Q = 0;
			ET = now - old_time;
		}
	}else{
		Q = 0;
		ET = 0;
	}
	old_S = S;
}