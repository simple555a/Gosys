
fblock STEP_INI;
attribute(autoscan);
uuid{86c4-50a4-7654-9229};
var
	Mod_Aut	:bool;	in;
	Mod_Mah	:bool;	in;
	Ctrl_Str:bool;	in;
	Prg_Rst	:bool;	in;
	Prg_Halt:bool;	in;
	End_Prg	:bool;	in;
	Flag_Stp:bool;	in;
	
	
	Flag_ST :bool;	out;
	Halt_ST :bool;	out;
	Cmd_ST	:bool;	out;
			
implementation

static bool rs1,rr1,rs2,rr2,Prg_Halt_R_Trig;
static bool tp,old_tp;
static integer n;
static date due_tp,due_tp1,ET;

void main()
{
	rs1 = (Mod_Aut || Mod_Mah) && (End_Prg || Prg_Rst);
	rr1 = Ctrl_Str || (!Mod_Aut && !Mod_Mah);
	
	if(rr1){
		Flag_ST = 0;
	}else if(rs1){
		Flag_ST = 1;
	}
	
	if(Prg_Halt){
		n++;
	}else{
		n = 0;
	}
	if(n == 1){
		Prg_Halt_R_Trig = 1;
	}else{
		Prg_Halt_R_Trig = 0;
	}
		
	rs2 = Prg_Halt_R_Trig && !Halt_ST && Flag_Stp;
	rr2 = (Prg_Halt_R_Trig && Halt_ST) || Prg_Rst;
	if(rr2){
		Halt_ST = 0;
	}else if(rs2){
		Halt_ST = 1;
	}
	
	tp = Halt_ST || Flag_ST;
	
	date now;
	now = gettime();
	
	if(tp){
		if(!old_tp && (ET == 0)){
			Cmd_ST = 1;
			due_tp = now + 2;
			ET = 0;
		}else if(now > due_tp){
			if(tp){
				Cmd_ST = 0;
				ET = 2;
			}else{
				Cmd_ST = 0;
				ET = 0;
			}
		}else{
			ET = 2 + now - due_tp;
		}
	}
	if(!tp){
		if(ET < 2){
			if(old_tp){
				Cmd_ST = 1;
				due_tp1 = now + 2 - ET;
			}else if(now < due_tp1){
				ET = 2 + now - due_tp1;
			}else{
				if(tp){
					Cmd_ST = 0;
					ET = 2;
				}else{
					Cmd_ST = 0;
					ET = 0;
				}
			}			
		}else{			
			Cmd_ST = 0;
			ET = 0;
		}
	}
	old_tp = tp;
	
}