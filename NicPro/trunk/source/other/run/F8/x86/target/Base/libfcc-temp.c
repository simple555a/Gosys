#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[10];
static BLK_SERVICE_PROC * __blk_services__;
#define ADDR ((&__dpins__[0])->i32 + 0)
#define IN1 ((&__dpins__[1])->i32 + 0)
#define IN2 ((&__dpins__[2])->i32 + 0)
#define IN3 ((&__dpins__[3])->i32 + 0)
#define IN4 ((&__dpins__[4])->i32 + 0)
#define EXIST ((&__dpins__[5])->i8)
#define OUT1 ((&__dpins__[6])->i32)
#define OUT2 ((&__dpins__[7])->i32)
#define OUT3 ((&__dpins__[8])->i32)
#define OUT4 ((&__dpins__[9])->i32)

#line 15 "D:/Actech/Conductor/f8/x86/target/Base/modstate.f"


int main()
{
	unsigned short *p;
	
	p = io_mem(0) + 13824 + ADDR * 120;
	OUT1 = *p;
	p++;
	OUT2 = *p;
	p++;
	OUT3 = *p;
	p++;
	OUT4 = *p;
	p++;
	
	if((IN1 == OUT1) && (IN2 == OUT2) &&(IN3 == OUT3) &&(IN4 == OUT4)){
		EXIST = 1;
	}else{
		EXIST = 0;
	}
}
		
	
	

