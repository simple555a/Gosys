#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[17];
static BLK_SERVICE_PROC * __blk_services__;
#define IN ((&__dpins__[0])->i32 + 0)
#define Bit1 ((&__dpins__[1])->i8)
#define Bit2 ((&__dpins__[2])->i8)
#define Bit3 ((&__dpins__[3])->i8)
#define Bit4 ((&__dpins__[4])->i8)
#define Bit5 ((&__dpins__[5])->i8)
#define Bit6 ((&__dpins__[6])->i8)
#define Bit7 ((&__dpins__[7])->i8)
#define Bit8 ((&__dpins__[8])->i8)
#define Bit9 ((&__dpins__[9])->i8)
#define Bit10 ((&__dpins__[10])->i8)
#define Bit11 ((&__dpins__[11])->i8)
#define Bit12 ((&__dpins__[12])->i8)
#define Bit13 ((&__dpins__[13])->i8)
#define Bit14 ((&__dpins__[14])->i8)
#define Bit15 ((&__dpins__[15])->i8)
#define Bit16 ((&__dpins__[16])->i8)

#line 22 "D:/Actech/Conductor/f8/x86/target/Converter/BCD_Bit.f"


void main()
	{
	if(IN == 1){
		Bit1 = 1;
	}else{
		Bit1 = 0;
	}
	
	if(IN == 2){
		Bit2 = 1;
	}else{
		Bit2 = 0;
	}

	if(IN == 3){
		Bit3 = 1;
	}else{
		Bit3 = 0;
	}
	
	if(IN == 4){
		Bit4 = 1;
	}else{
		Bit4 = 0;
	}
	
	if(IN == 5){
		Bit5 = 1;
	}else{
		Bit5 = 0;
	}
	
	if(IN == 6){
		Bit6 = 1;
	}else{
		Bit6 = 0;
	}
	
	if(IN == 7){
		Bit7 = 1;
	}else{
		Bit7 = 0;
	}
	
	if(IN == 8){
		Bit8 = 1;
	}else{
		Bit8 = 0;
	}
	
	if(IN == 9){
		Bit9 = 1;
	}else{
		Bit9 = 0;
	}
	
	if(IN == 10){
		Bit10 = 1;
	}else{
		Bit10 = 0;
	}

	if(IN == 11){
		Bit11 = 1;
	}else{
		Bit11 = 0;
	}
	
	if(IN == 12){
		Bit12 = 1;
	}else{
		Bit12 = 0;
	}
	
	if(IN == 13){
		Bit13 = 1;
	}else{
		Bit13 = 0;
	}
	
	if(IN == 14){
		Bit14 = 1;
	}else{
		Bit14 = 0;
	}
	
	if(IN == 15){
		Bit15 = 1;
	}else{
		Bit15 = 0;
	}
	
	if(IN == 16){
		Bit16 = 1;
	}else{
		Bit16 = 0;
	}
}

