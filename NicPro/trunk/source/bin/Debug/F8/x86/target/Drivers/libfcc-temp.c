#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[33];
static BLK_SERVICE_PROC * __blk_services__;
#define AM ((&__dpins__[0])->i8 + 0)
#define KO ((&__dpins__[1])->i8 + 0)
#define AO ((&__dpins__[2])->i8 + 0)
#define PO ((&__dpins__[3])->i8 + 0)
#define OPN ((&__dpins__[4])->i8 + 0)
#define FO ((&__dpins__[5])->i8 + 0)
#define KO_FB ((&__dpins__[6])->i8 + 0)
#define LO ((&__dpins__[7])->i8 + 0)
#define MPP ((&__dpins__[8])->i8 + 0)
#define LC ((&__dpins__[9])->i8 + 0)
#define KC_FB ((&__dpins__[10])->i8 + 0)
#define FZ ((&__dpins__[11])->i8 + 0)
#define CLS ((&__dpins__[12])->i8 + 0)
#define PC ((&__dpins__[13])->i8 + 0)
#define AC ((&__dpins__[14])->i8 + 0)
#define KC ((&__dpins__[15])->i8 + 0)
#define DIS ((&__dpins__[16])->i8 + 0)
#define L_R ((&__dpins__[17])->i8 + 0)
#define TS ((&__dpins__[18])->i8 + 0)
#define TP ((&__dpins__[19])->dbl + 0)
#define TD ((&__dpins__[20])->dbl + 0)
#define ONENO ((&__dpins__[21])->i8)
#define ONCMD ((&__dpins__[22])->i8)
#define OFFCMD ((&__dpins__[23])->i8)
#define OFFENO ((&__dpins__[24])->i8)
#define MPPT ((&__dpins__[25])->i8)
#define TIMOUT ((&__dpins__[26])->i8)
#define STU ((&__dpins__[27])->i32)
#define V_FO ((&__dpins__[28])->i8)
#define V_FZ ((&__dpins__[29])->i8)
#define V_PO ((&__dpins__[30])->i8)
#define V_PC ((&__dpins__[31])->i8)
#define ETD ((&__dpins__[32])->dbl)

#line 39 "D:/Actech/Conductor/f8/x86/target/Drivers/D_DT_VAL.f"

static bool OP,CL,OpenSet,OpenReset,OpenQ,CloseSet,CloseReset,CloseQ;
static bool TDSet,MPPTDSet,old_TD,old_TP,old_MPPTD,TDQ,TPQ,MPPTDQ;
static date due_TD,due_MPPTD,due_TP;
static bool MPPSet,MPPReset,MPPQ;

void main()
{
	//×ÛºÏÖ¸Áî
	OP = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO) && FO && !DIS && !L_R && !LO && !PC) && !OPN;
	CL = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC) && FZ && !DIS && !L_R && !LC) && !CLS;
	ONENO = (((AM && AO && !AC) || (KO && !KO_FB && !KC)|| PO)&& !OP);
	OFFENO = (((AM && AC && !AO) || (KC && !KC_FB && !KO)|| PC)&& !CL);
	V_FO = FO;
	V_FZ = FZ;
	
	
	OpenSet = (OP && !CloseSet);
	CloseSet = (CL && !OpenSet);
			
	//´ò¿ªÃüÁî
	if(OpenSet && !CloseSet){
		OpenQ = 1;
	}else if(OpenReset || CloseSet){
		OpenQ = 0;
	}
	V_PO = PO;
	
	//¹Ø±ÕÃüÁî
	if(CloseSet && !OpenSet){
		CloseQ = 1;
	}else if(CloseReset || OpenSet){
		CloseQ = 0;
	}
	if(!TS){
		ONCMD = OpenQ && !TIMOUT;
		OFFCMD = CloseQ && !TIMOUT;
	}else{
		ONCMD = OpenQ && TPQ;
		OFFCMD = CloseQ && TPQ;
	}
	V_PC = PC;
	
	//ÖÐÍ£Ö¸Áî
	MPPT = MPP;
	
	//Çý¶¯¹ÊÕÏÅÐ¶Ï
	MPPSet = MPP;
	MPPReset = OpenSet || CloseSet;
	if(MPPSet){
		MPPQ = 1;
	}else if(MPPReset){
		MPPQ = 0;
	}
	
	TDSet = OpenQ || CloseQ;
	MPPTDSet = OPN && CLS && !MPPQ;
	OpenReset = ((OPN && !MPPTDSet) || MPP || KC || TIMOUT);
	CloseReset = ((CLS && !MPPTDSet) || MPP || KO || TIMOUT);
	
	date now;
	now = gettime();
	if(TDSet){
		if(!old_TD){
			due_TD = now + TD;
			ETD = 0;
		}else if(now > due_TD){
			TDQ = 1;
			ETD = TD;
		}else{
			TDQ = 0;
			ETD = TD + now - due_TD; 
		}
		if(!old_TP){
			due_TP = now + TP;
		}else if(now > due_TP){
			TPQ = 0;
		}else{
			TPQ = 1;
		}
	}else{
		TDQ = 0;
		TPQ = 1;
		ETD = 0;
	}
	old_TD = TDSet;
	old_TP = TDSet;
	
	if(MPPTDSet){
		if(!old_MPPTD){
			due_MPPTD = now + 20;
		}else if(now > due_MPPTD){
			MPPTDQ = 1;
		}else{
			MPPTDQ = 0;
		}
	}else{
		MPPTDQ = 0;
	}
	old_MPPTD = MPPTDSet;
	TIMOUT = TDQ || MPPTDQ;
	
	STU = (integer)OPN + (integer)CLS * 2 + (integer)(!(ONCMD + OFFCMD) && !(OPN + CLS) && !TIMOUT && !MPPT) * 3 + (integer)(DIS || TIMOUT) * 4;
	
}
