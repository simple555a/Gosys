#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[4];
static BLK_SERVICE_PROC * __blk_services__;
#define S ((&__dpins__[0])->i8 + 0)
#define T ((&__dpins__[1])->dbl + 0)
#define Q ((&__dpins__[2])->i8)
#define ET ((&__dpins__[3])->dbl)

#line 11 "D:/Actech/Conductor/f8/x86/target/Timers/ton_t.f"

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
