#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[3];
static BLK_SERVICE_PROC * __blk_services__;
#define IN1 ((&__dpins__[0])->flt + 0)
#define IN2 ((&__dpins__[1])->flt + 0)
#define OUT ((&__dpins__[2])->flt)

#line 6 "D:/Actech/Conductor/f8/x86/target/Arithmetic/DIV_FLOAT.f"

void main()
{
	if(fabs(IN2) < 0.0000001){
		OUT = 0;
	}else{
		OUT = IN1 / IN2;
	}
}

