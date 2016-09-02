fblock DM114;
attribute(autoscan);
uuid{3e2b-74d7-d1ec-82cf};
var
	ADDR : word;in;
	CLEARI1: bool;in;
	CLEARI2: bool;in;
	CLEARI3: bool;in;
	CLEARI4: bool;in;
	CLEARI5: bool;in;
	CLEARI6: bool;in;
	CLEARI7: bool;in;
	CLEARI8: bool;in;
	CLEARI9: bool;in;
	CLEARI10: bool;in;
	CLEARI11: bool;in;
	CLEARI12: bool;in;
	CLEARI13: bool;in;
	CLEARI14: bool;in;
	CLEARI15: bool;in;
	CLEARI16: bool;in;
	
	I1 : integer;out;
	I2 : integer;out;
	I3 : integer;out;
	I4 : integer;out;
	I5 : integer;out;
	I6 : integer;out;
	I7 : integer;out;
	I8 : integer;out;
	I9 : integer;out;
	I10 : integer;out;
	I11 : integer;out;
	I12 : integer;out;
	I13 : integer;out;
	I14 : integer;out;
	I15 : integer;out;
	I16 : integer;out;	
	
implementation


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
