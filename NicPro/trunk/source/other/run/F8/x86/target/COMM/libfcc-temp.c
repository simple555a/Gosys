#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[18];
static BLK_SERVICE_PROC * __blk_services__;
#define SADDR ((&__dpins__[0])->ui16 + 0)
#define TADDR ((&__dpins__[1])->ui16 + 0)
#define IN1 ((&__dpins__[2])->i32 + 0)
#define IN2 ((&__dpins__[3])->i32 + 0)
#define IN3 ((&__dpins__[4])->i32 + 0)
#define IN4 ((&__dpins__[5])->i32 + 0)
#define IN5 ((&__dpins__[6])->i32 + 0)
#define IN6 ((&__dpins__[7])->i32 + 0)
#define IN7 ((&__dpins__[8])->i32 + 0)
#define IN8 ((&__dpins__[9])->i32 + 0)
#define OUT1 ((&__dpins__[10])->i32)
#define OUT2 ((&__dpins__[11])->i32)
#define OUT3 ((&__dpins__[12])->i32)
#define OUT4 ((&__dpins__[13])->i32)
#define OUT5 ((&__dpins__[14])->i32)
#define OUT6 ((&__dpins__[15])->i32)
#define OUT7 ((&__dpins__[16])->i32)
#define OUT8 ((&__dpins__[17])->i32)

#line 22 "F:/Actech/Conductor/f8/x86/target/COMM/wgint.f"


int main()
{
	char *p;
	word *pw;
	integer *pi;
	p = io_mem(0) - (6 + 6 + 1) * 4 - 136 * 1024;
	
	if(SADDR >= 32){
		return -1;
	}
	
	p += SADDR * 36;
	
	pw = (word*)p;
	*pw = 0x55aa;
	pw++;
	*pw = TADDR;
	
	pi = (integer*)p + 1;
	*pi = IN1;
	OUT1 = IN1;
	pi++;
	*pi = IN2;
	OUT2 = IN2;
	pi++;
	*pi = IN3;
	OUT3 = IN3;
	pi++;
	*pi = IN4;
	OUT4 = IN4;
	pi++;
	*pi = IN5;   
	OUT5 = IN5;
	pi++;
	*pi = IN6;
	OUT6 = IN6;
	pi++;
	*pi = IN7;  
	OUT7 = IN8;
	pi++;
	*pi = IN8;
	OUT8 = IN8;
	
	return 0;	
}

