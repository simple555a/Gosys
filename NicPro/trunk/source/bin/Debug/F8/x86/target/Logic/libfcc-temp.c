#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[3];
static BLK_SERVICE_PROC * __blk_services__;
#define S ((&__dpins__[0])->i8 + 0)
#define IN ((&__dpins__[1])->flt + 0)
#define OUT ((&__dpins__[2])->flt)

#line 9 "D:/Actech/Conductor/f8/x86/target/Logic/DC_BA.f"


void main()
{
	if(S)
	{ OUT=IN; }
	
}
