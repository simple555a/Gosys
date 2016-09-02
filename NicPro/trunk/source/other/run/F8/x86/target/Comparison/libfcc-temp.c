#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[15];
static BLK_SERVICE_PROC * __blk_services__;
#define IN ((&__dpins__[0])->flt + 0)
#define Max3 ((&__dpins__[1])->flt + 0)
#define Max2 ((&__dpins__[2])->flt + 0)
#define Max1 ((&__dpins__[3])->flt + 0)
#define Min1 ((&__dpins__[4])->flt + 0)
#define Min2 ((&__dpins__[5])->flt + 0)
#define Min3 ((&__dpins__[6])->flt + 0)
#define Bad ((&__dpins__[7])->i8 + 0)
#define OUT ((&__dpins__[8])->flt)
#define H3 ((&__dpins__[9])->i8)
#define H2 ((&__dpins__[10])->i8)
#define H1 ((&__dpins__[11])->i8)
#define L1 ((&__dpins__[12])->i8)
#define L2 ((&__dpins__[13])->i8)
#define L3 ((&__dpins__[14])->i8)

#line 21 "D:/Actech/Conductor/f8/x86/target/Comparison/COMP_F.f"

void main()
{
	if(!Bad){
		if(IN > Max3){
			H3 = 1;
		}else{
			H3 = 0;
		}
		if(IN > Max2){
			H2 = 1;
		}else{
			H2 = 0;
		}
		if(IN > Max1){
			H1 = 1;
		}else{
			H1 = 0;
		}
		
		if(IN < Min1){
			L1 = 1;
		}else{
			L1 = 0;
		}
		if(IN < Min2){
			L2 = 1;
		}else{
			L2 = 0;
		}
		if(IN < Min3){
			L3 = 1;
		}else{
			L3 = 0;
		}
	}else{
		H3 = 0;
		H2 = 0;
		H1 = 0;
		L1 = 0;
		L2 = 0;
		L3 = 0;
	}
	OUT = IN;
}

