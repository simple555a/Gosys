#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[4];
static BLK_SERVICE_PROC * __blk_services__;
#define IN ((&__dpins__[0])->flt + 0)
#define M_hold ((&__dpins__[1])->i8 + 0)
#define M_rst ((&__dpins__[2])->i8 + 0)
#define OUT ((&__dpins__[3])->flt)

#line 9 "D:/Actech/Conductor/f8/x86/target/MCS/Totalizer.f"

static date old_T;
static float old_IN;
void main()
{	static bool first_time=1;
	date now,dt;
	now = gettime();
	
	if(first_time)
	 {old_T=gettime();
	  old_IN=IN;
	  first_time=0;}
	dt = fabs(now - old_T);
	
	if(M_rst){
		OUT = 0;
		return;
	}
	
	
	if(!M_hold){
		OUT = OUT + (old_IN+IN) * dt / 7200;
	}
	old_T = now;
	old_IN=IN;
}


