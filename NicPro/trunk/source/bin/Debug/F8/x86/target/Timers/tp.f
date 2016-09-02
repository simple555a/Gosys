#
# Positive pulse module
#
fblock TP;
attribute(autoscan);
uuid{19b8-8155-d278-694b};
var
	S : bool;in;
	T : date;in;
	Q : bool;out;
	ET: date;out;
implementation
static bool old_S;
static date due_T;
static date due_T1;

void main()
{
	date now;
	now = gettime();
	if(S){
		if(!old_S && (ET == 0)){
			// positive edge detected
			Q = 1;
			due_T = now + T;
			ET = 0;
		}else if(now > due_T){
			if(S){
				Q = 0;
				ET = T;
			}else{
				Q = 0;
				ET = 0;
			}
		}else{
			ET = T + now - due_T;
		}
	}
	if(!S){
		if(ET < T){
			if(old_S){
				// negative edge detected
				Q = 1;
				due_T1 = now + T - ET;
			}else if(now < due_T1){
				ET = T + now - due_T1;
			}else{
				if(S){
					Q = 0;
					ET = T;
				}else{
					Q = 0;
					ET = 0;
				}
			}			
		}else{			
			Q = 0;
			ET = 0;
		}
	}
	old_S = S;
}