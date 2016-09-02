
fblock ST_CTRL;
attribute(autoscan);
uuid{41d7-968c-a4ee-5664};
var
	Mod_AUT	:bool;	in;
	Mod_MAH	:bool;	in;
	Step_AO	:bool;	in;
	Skip	:bool;	in;
	TRN	:bool;	in;
	HALT	:bool;	in;
	Prog_RST:bool;	in;
	Cond	:bool;	in;
	LC_Cond	:bool;	in;
	ER_PRE	:date;	in;
	TM_PRE	:date;	in;
	
	Step_FLG:bool;	out;
	CMD_P	:bool;	out;
	CMD_L	:bool;	out;
	CMD_LCP	:bool;	out;
	Step_End:bool;	out;
	RST_Step:bool;	out;
	ERR	:bool;	out;
	TM_ACC	:float;	out;
	
implementation

static bool rs1,rr1;
static bool T1,T2,T1Q,T2Q,old_T1,old_T2;
static bool TP1,TP2,TP3,old_TP1,old_TP2,old_TP3,TP1Q,TP2Q,TP3Q;
static bool TPN,TPNQ,TPNP,old_TPN,TPNP_R_Trig,Prog_RST_R_Trig;
static integer n,m;
static date due_T1,due_T2;
static date due_TP1,due_TP11,due_TP2,due_TP22,due_TP3,due_TP33,ETP1,ETP2,ETP3;
static date due_TPNT,TPNT,TPNET;
static float old_TM_ACC;

void main()
{
	rs1 = (Mod_AUT && Step_AO) || (Mod_MAH && !Skip && Step_AO);
	rr1 = Prog_RST || (!Mod_AUT && !Mod_MAH) || (T1Q && TRN);
	
	if(rr1){
		Step_FLG = 0;
	}else if(rs1){
		Step_FLG = 1;
	}
	
	RST_Step = rs1 || (Mod_MAH && Step_AO && Skip);
	CMD_L = Step_FLG && !HALT;
	
	T2 = Step_FLG && !Cond && !HALT;
	TP1 = T1Q && TRN;
	TP2 = CMD_L;
	TP3 = CMD_L && LC_Cond;
	TPNP = Step_FLG && (!Cond || HALT);
		
	date now;
	now = gettime();
	//Step_end
	if(Step_FLG){
		if(!old_T1){
			due_T1 = now + 0.3;
		}else if(now > due_T1){
			T1Q = 1;
		}else{
			T1Q = 0;
		}
	}else{
		T1Q = 0;
	}
	old_T1 = Step_FLG;
	
	if(T2){
		if(!old_T2){
			due_T2 = now + ER_PRE;
		}else if(now > due_T2){
			ERR = 1;
		}else{
			ERR = 0;
		}
	}else{
		ERR = 0;
	}
	old_T2 = T2;
	

	if(TP1){
		if(!old_TP1 && (ETP1 == 0)){
			TP1Q = 1;
			due_TP1 = now + 2;
			ETP1 = 0;
		}else if(now > due_TP1){
			if(TP1){
				TP1Q = 0;
				ETP1 = 2;
			}else{
				TP1Q = 0;
				ETP1 = 0;
			}
		}else{
			ETP1 = 2 + now - due_TP1;
		}
	}
	if(!TP1){
		if(ETP1 < 2){
			if(old_TP1){
				TP1Q = 1;
				due_TP11 = now + 2 - ETP1;
			}else if(now < due_TP11){
				ETP1 = 2 + now - due_TP11;
			}else{
				if(TP1){
					TP1Q = 0;
					ETP1 = 2;
				}else{
					TP1Q = 0;
					ETP1 = 0;
				}
			}			
		}else{			
			TP1Q = 0;
			ETP1 = 0;
		}
	}
	old_TP1 = TP1;
	Step_End = TP1Q || (Mod_MAH && Step_AO && Skip) || TPNQ;
	
	if(TP2){
		if(!old_TP2 && (ETP2 == 0)){
			CMD_P = 1;
			due_TP2 = now + 2;
			ETP2 = 0;
		}else if(now > due_TP2){
			if(TP2){
				CMD_P = 0;
				ETP2 = 2;
			}else{
				CMD_P = 0;
				ETP2 = 0;
			}
		}else{
			ETP2 = 2 + now - due_TP2;
		}
	}
	if(!TP2){
		if(ETP2 < 2){
			if(old_TP2){
				CMD_P = 1;
				due_TP22 = now + 2 - ETP2;
			}else if(now < due_TP22){
				ETP2 = 2 + now - due_TP22;
			}else{
				if(TP2){
					CMD_P = 0;
					ETP2 = 2;
				}else{
					CMD_P = 0;
					ETP2 = 0;
				}
			}			
		}else{			
			CMD_P = 0;
			ETP2 = 0;
		}
	}
	old_TP2 = TP2;

	if(TP3){
		if(!old_TP3 && (ETP3 == 0)){
			CMD_LCP = 1;
			due_TP3 = now + 2;
			ETP3 = 0;
		}else if(now > due_TP3){
			if(TP3){
				CMD_LCP = 0;
				ETP3 = 2;
			}else{
				CMD_LCP = 0;
				ETP3 = 0;
			}
		}else{
			ETP3 = 2 + now - due_TP3;
		}
	}
	if(!TP3){
		if(ETP3 < 2){
			if(old_TP3){
				CMD_LCP = 1;
				due_TP33 = now + 2 - ETP3;
			}else if(now < due_TP33){
				ETP3 = 2 + now - due_TP33;
			}else{
				if(TP3){
					CMD_LCP = 0;
					ETP3 = 2;
				}else{
					CMD_LCP = 0;
					ETP3 = 0;
				}
			}			
		}else{			
			CMD_LCP = 0;
			ETP3 = 0;
		}
	}
	old_TP3 = TP3;

	TPN = Step_FLG && !TPNP;
	
	if(Step_AO){
		n++;
	}else{
		n = 0;
	}
	if(n == 1){
		TPNP_R_Trig = 1;
	}else{
		TPNP_R_Trig = 0;
	}
	
	if(TPNP || TPNP_R_Trig){
		TPNT = TM_PRE - TM_ACC;
	}
	if(Prog_RST){
		m++;
	}else{
		m = 0;
	}
	if((m < 2) && Prog_RST) {
		Prog_RST_R_Trig = 1;
	}else{
		Prog_RST_R_Trig = 0;
	}
	if(Prog_RST_R_Trig){
		TM_ACC = 0;
		TPNQ = 0;
		old_TM_ACC = 0;
		TPNQ = 0;
	}else{
		if(TPN){
			TM_ACC = old_TM_ACC + TPNET;
			if(!old_TPN){
				due_TPNT = now + TPNT;
				TPNET = 0;
				TPNQ = 0;
			}else if(now > due_TPNT){
				TPNET = TPNT;
				TPNQ = 1;
			}else{
				TPNET = TPNT + now - due_TPNT;
				TPNQ = 0;
			}
					
		}else{
			TPNET = 0;
			TPNQ = 0;
			old_TM_ACC = TM_ACC;
		}
		old_TPN = TPN;
	}
}