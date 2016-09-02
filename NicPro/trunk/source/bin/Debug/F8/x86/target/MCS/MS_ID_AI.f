fblock MS_ID_AI;
attribute(autoscan);
uuid{b608-0830-a388-fb8d};
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
	dt	:float;out;
	DD	:date;out;
	
implementation
static date due_T;
static float dx;

static float limit_y(float t)
{
	if(t >= YMAX){
		QMAX = 1;
		t = YMAX;
	}else{
		QMAX = 0;
	}
	if(t <= YMIN){
		QMIN = 1;
		t = YMIN;
	}else{
		QMIN = 0;
	}  
	return t;
}

void main()
{
	static bool first_time = true;
	date now;
// 	float dt;
	float dy;
	
	
	now = gettime();
	if(first_time){
		due_T = now;
		first_time = false;
		return;
	}
	DD = now - due_T;
	dt = (float)(now - due_T);
	due_T = now;
	if(!SW){
	        if(IC || DC){
		      	if(IC){
		             if(!DC){
		                     dy = dt*RATE;
		                     dx = dx + dy;
		             }
		      	}else{
		             if(DC){
		                    dy = dt*RATE;
		                    dx = dx - dy;
		             }
		      	}
		      	
		}else{
			dx = YMAN;
		}
		Y = limit_y(dx);
		Yout = limit_y(dx);
		return;	
	}else{

		dx = IN;
		Y = limit_y(dx);
		Yout = limit_y(dx);
	}
}