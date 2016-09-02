#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[8];
static BLK_SERVICE_PROC * __blk_services__;
#define CU ((&__dpins__[0])->i8 + 0)
#define CD ((&__dpins__[1])->i8 + 0)
#define R ((&__dpins__[2])->i8 + 0)
#define LD ((&__dpins__[3])->i8 + 0)
#define PV ((&__dpins__[4])->i32 + 0)
#define QU ((&__dpins__[5])->i8)
#define QD ((&__dpins__[6])->i8)
#define CV ((&__dpins__[7])->i32)

#line 13 "F:/Actech/Conductor/f8/x86/target/Counter/CTUD.f"


static bool old_CU,old_CD;

void main()
{
	if(R && !LD){			//本算法为复位优先
		CV = 0;
		QU = 0;
		QD = 1;
		old_CU = 0;
	}
	
	if(LD && !R){
		CV = PV;
		QU = 1;
		QD = 0;
		old_CD = 0;
	}
	
	if(!R){
		if(CU && !old_CU){
			CV = CV + 1;			
		}
		
		if(CD && !old_CD){
			CV = CV - 1;	
		}
	}
	old_CU = CU;
	old_CD = CD;
	
	if(CV >= PV){
		QU = 1;
	}else if(CV <= 0){
		QD = 1;
	}else{
		QU = 0;
		QD = 0;
	}
}
