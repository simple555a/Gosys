fblock PID;
attribute(autoscan);
uuid{a429-353c-d013-e179};
var
	MAN    : bool;  in;
	HALT   : bool;  in;
	SP     : float; in;
	PV     : float; in;
	BIAS   : float; in;
	EN_P   : bool;  in;
	EN_I   : bool;  in;
	EN_D   : bool;  in;
	D_ON_X : bool;  in;
	GAIN   : float; in;
	TI     : date;  in;
	TD     : date;  in;
	TD_LAG : date;  in;
	YMAX   : float; in;
	YMIN   : float; in;
	YMAN   : float; in;
	Y      : float; out;
	ERR    : float; out;
	QMAX   : bool;  out;
	QMIN   : bool;  out;
	
implementation
static date old_T;

static float YP;
static float YI;
static float YD; 
static date now;
static date dt;

static float old_YI;
static float old_YD;
static float old_ERR;
static float old_PV;

static bool bFirstCycle;

void main(on_op)
{	

	now = gettime();
	if(bFirstCycle){
		old_T = now;
		old_YI = YI;
		old_YD = YD;
		old_PV = PV;
		old_ERR = ERR;
		bFirstCycle = false;
		return;
	}
	
	dt = fabs(now - old_T);
	ERR = SP - PV;
		

	if(EN_P == 1){
		YP = GAIN * ERR;
	}else{
		YP = 0;
	}

	if((MAN == 0) && (HALT == 0)){
		if(EN_I){
			if(!QMAX && !QMIN){
				if((YI <= (YMAX - YP -BIAS)) && (YI >= (YMIN - YP -BIAS))){
					YI = old_YI + GAIN * dt / TI * (ERR + old_ERR) / 2.0;
				}else if(YI > (YMAX - YP -BIAS)){
					YI = YMAX - YP - BIAS;
				}else if(YI < (YMIN - YP -BIAS)){
					YI = YMIN - YP -BIAS;
				}				
			}
		}else{
			YI = 0;
		}
		if(EN_D){
		 	if(D_ON_X){
				YD = (old_YD * TD_LAG + TD * GAIN * (ERR-old_ERR)) / (dt + TD_LAG);
			}else{
				YD = (old_YD * TD_LAG + TD * GAIN * (old_PV -PV)) / (dt + TD_LAG);
			}
		}else{
			YD = 0;
		}
	}
	//
	if(!MAN){
		if(!HALT){
			Y = YP + YI + YD;
		}else{
			if(EN_I){
				if((YI <= (YMAX - YP -BIAS)) && (YI >= (YMIN - YP -BIAS))){
					YI = YMAN - YP - BIAS;
				}else if(YI > (YMAX - YP -BIAS)){
					YI = YMAX - YP - BIAS;
				}else if(YI < (YMIN - YP -BIAS)){
					YI = YMIN - YP -BIAS;
				}
			}else{
				YI = 0;
			}
		YD = 0;
		}
	}else{
		Y = YMAN;
		if(EN_I){
			if((YI <= (YMAX - YP -BIAS)) && (YI >= (YMIN - YP -BIAS))){
				YI = Y - YP - BIAS;
			}else if(YI > (YMAX - YP -BIAS)){
				YI = YMAX - YP - BIAS;
			}else if(YI < (YMIN - YP -BIAS)){
				YI = YMIN - YP -BIAS;
			}
		}else{
			YI = 0;
		}
		YD = 0;
	}	
	
	
	if(Y > YMAX){
		QMAX = 1;
		Y = YMAX;
	}else{
		QMAX = 0;
	}
	if(Y < YMIN){
		QMIN = 1;
		Y = YMIN;
	}else{
		QMIN = 0;
	}
	
	old_T = now;
	old_YI = YI;
	old_YD = YD;
	old_PV = PV;
	old_ERR = ERR;
	
}

void on_start()
{
	bFirstCycle = true;
}