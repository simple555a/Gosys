#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[3];
static BLK_SERVICE_PROC * __blk_services__;
#define S ((&__dpins__[0])->i8 + 0)
#define R ((&__dpins__[1])->i8 + 0)
#define Q ((&__dpins__[2])->i8)

#line 6 "D:/Actech/Conductor/f8/x86/target/Bistable/rs.f"

void main()
{
	if(R){
		Q = 0;
	}else if(S){
		Q = 1;
	}
}

