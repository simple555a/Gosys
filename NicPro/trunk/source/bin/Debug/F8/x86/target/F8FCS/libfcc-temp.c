#define FCC_INVOKED
#include <f8_kprocs.h>


static struct pin __dpins__[33];
static BLK_SERVICE_PROC * __blk_services__;
#define ADDR ((&__dpins__[0])->ui16 + 0)
#define CLEARI1 ((&__dpins__[1])->i8 + 0)
#define CLEARI2 ((&__dpins__[2])->i8 + 0)
#define CLEARI3 ((&__dpins__[3])->i8 + 0)
#define CLEARI4 ((&__dpins__[4])->i8 + 0)
#define CLEARI5 ((&__dpins__[5])->i8 + 0)
#define CLEARI6 ((&__dpins__[6])->i8 + 0)
#define CLEARI7 ((&__dpins__[7])->i8 + 0)
#define CLEARI8 ((&__dpins__[8])->i8 + 0)
#define CLEARI9 ((&__dpins__[9])->i8 + 0)
#define CLEARI10 ((&__dpins__[10])->i8 + 0)
#define CLEARI11 ((&__dpins__[11])->i8 + 0)
#define CLEARI12 ((&__dpins__[12])->i8 + 0)
#define CLEARI13 ((&__dpins__[13])->i8 + 0)
#define CLEARI14 ((&__dpins__[14])->i8 + 0)
#define CLEARI15 ((&__dpins__[15])->i8 + 0)
#define CLEARI16 ((&__dpins__[16])->i8 + 0)
#define I1 ((&__dpins__[17])->i32)
#define I2 ((&__dpins__[18])->i32)
#define I3 ((&__dpins__[19])->i32)
#define I4 ((&__dpins__[20])->i32)
#define I5 ((&__dpins__[21])->i32)
#define I6 ((&__dpins__[22])->i32)
#define I7 ((&__dpins__[23])->i32)
#define I8 ((&__dpins__[24])->i32)
#define I9 ((&__dpins__[25])->i32)
#define I10 ((&__dpins__[26])->i32)
#define I11 ((&__dpins__[27])->i32)
#define I12 ((&__dpins__[28])->i32)
#define I13 ((&__dpins__[29])->i32)
#define I14 ((&__dpins__[30])->i32)
#define I15 ((&__dpins__[31])->i32)
#define I16 ((&__dpins__[32])->i32)

#line 39 "D:/Actech/Conductor/f8/x86/target/F8FCS/DM124.f"



void main()
{
	int *pco;
	byte *pVal;
	word *pci;
	
	pco = (int*)io_mem(ADDR);
	pVal = (byte*)io_mem(ADDR);
	pci = (word*)io_mem(ADDR);
	
#define _(n) I##n = *pco ;

       pVal=pVal+16;
       if(*pVal==0)
       {
	_(1);
	pco++;
	_(2);
	pco++;
	_(3);
	pco++;
	_(4);
	}
	
	if(*pVal==1)
       {
	_(5);
	pco++;
	_(6);
	pco++;
	_(7);
	pco++;
	_(8);
	}
	
	if(*pVal==2)
       {
	_(9);
	pco++;
	_(10);
	pco++;
	_(11);
	pco++;
	_(12);
	
	}
	
	if(*pVal==3)
       {
	_(13);
	pco++;
	_(14);
	pco++;
	_(15);
	pco++;
	_(16);
	}
	
	
	*pci =CLEARI1|(CLEARI2 << 1)|(CLEARI3 << 2)|(CLEARI4 << 3)|(CLEARI5 << 4)|(CLEARI6 << 5)|(CLEARI7 << 6)|(CLEARI8 << 7)|(CLEARI9 << 8)|(CLEARI10 << 9)|(CLEARI11 << 10)|(CLEARI12 << 11)|(CLEARI13 << 12)|(CLEARI14 << 13)|(CLEARI15 << 14)|(CLEARI16 << 15);	
	
}

