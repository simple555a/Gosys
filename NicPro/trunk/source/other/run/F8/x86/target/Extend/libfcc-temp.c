#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[6];
static BLK_SERVICE_PROC * __blk_services__;
#define dP ((&__dpins__[0])->flt + 0)
#define Pt ((&__dpins__[1])->flt + 0)
#define Ph ((&__dpins__[2])->flt + 0)
#define Tt ((&__dpins__[3])->flt + 0)
#define Q ((&__dpins__[4])->flt)
#define dPo ((&__dpins__[5])->flt)

#line 11 "D:/Actech/Conductor/f8/x86/target/Extend/WP_FIX.f"

void  main()
{       
 	dPo = dP;
 	if(((Ph + Pt) < 0.0000000001)||((dP * (Ph + 7600) * (273.15 + Tt))<0))
 	{
 		Q = 0.0;
 	}else{
 		Q = (81.447285 + 0.002094 * dP - 0.000000622371 * dP * dP) * sqrt((dP * (Ph + 7600) * (273.15 + Tt)) / (Ph + Pt));
 	}
}
