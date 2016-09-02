fblock MS_ID;
attribute(autoscan);
uuid{49e6-1f0c-d7bd-a247};
var
	IN     : float; in;
	IC     : bool;  in;
	DC     : bool;  in;
	SW     : bool; in;
	RATE   : float; in;
	YMAX   : float; in;
	YMIN   : float; in;
	YMAN   : float; in;
	
	Y      : float; out;
	Yout   : float; out;
	QMAX   : bool;  out;
	QMIN   : bool;  out;
	
implementation
static date due_T;
static float dx;

void main()
{
	static bool first_time = true;
	date now;
	float dt;
	float dy;
	
	now = gettime();

	if(first_time){
		due_T = now;
		first_time = false;
		return;
	}
	
	dt = (float)(now - due_T);
	due_T = now;	
	
	if(SW){
		dx = IN;
	}else{
	      if(IC){
	             if(!DC){
	                     dy = dt*RATE;
	                     dx = dx + dy;
	             }
	      }else{
	             if(DC){
	                    dy = dt*RATE;
	                    dx = dx-dy;
	             }
	           }
	}
	
	Y=dx;
	
	if(Y >= YMAX){
		QMAX = 1;
		Y = YMAX;
		dx = YMAX;
	}else{
		QMAX = 0;
	}
	if(Y <= YMIN){
		QMIN = 1;
		Y = YMIN;
		dx = YMIN;
	}else{
		QMIN = 0;
	}  
	first_time = true; 
}