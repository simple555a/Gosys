#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[14];
static BLK_SERVICE_PROC * __blk_services__;
#define IN ((&__dpins__[0])->flt + 0)
#define IC ((&__dpins__[1])->i8 + 0)
#define DC ((&__dpins__[2])->i8 + 0)
#define SW ((&__dpins__[3])->i8 + 0)
#define RATE ((&__dpins__[4])->flt + 0)
#define YMAX ((&__dpins__[5])->flt + 0)
#define YMIN ((&__dpins__[6])->flt + 0)
#define YMAN ((&__dpins__[7])->flt + 0)
#define Y ((&__dpins__[8])->flt)
#define Yout ((&__dpins__[9])->flt)
#define QMAX ((&__dpins__[10])->i8)
#define QMIN ((&__dpins__[11])->i8)
#define dt ((&__dpins__[12])->flt)
#define DD ((&__dpins__[13])->dbl)

#line 20 "D:/Actech/Conductor/f8/x86/target/MCS/MS_ID_AI.f"

static date due_T;
static float dx;

static float limit_y(float t)
{
	if(t >= YMAX){
		QMAX = 1;
		t = YMAX;
	}else{
		QMAX = 0;
	}
	if(t <= YMIN){
		QMIN = 1;
		t = YMIN;
	}else{
		QMIN = 0;
	}  
	return t;
}

void main()
{
	static bool first_time = true;
	date now;
// 	float dt;
	float dy;
	
	
	now = gettime();
	if(first_time){
		due_T = now;
		first_time = false;
		return;
	}
	DD = now - due_T;
	dt = (float)(now - due_T);
	due_T = now;
	if(!SW){
	        if(IC || DC){
		      	if(IC){
		             if(!DC){
		                     dy = dt*RATE;
		                     dx = dx + dy;
		             }
		      	}else{
		             if(DC){
		                    dy = dt*RATE;
		                    dx = dx - dy;
		             }
		      	}
		      	
		}else{
			dx = YMAN;
		}
		Y = limit_y(dx);
		Yout = limit_y(dx);
		return;	
	}else{

		dx = IN;
		Y = limit_y(dx);
		Yout = limit_y(dx);
	}
}
