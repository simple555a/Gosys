fblock DM123;
attribute(autoscan);
uuid{b0de-948e-2bc2-3562};
var
	ADDR : word;in;
	ID_CLR: byte;in;
	VF  : word;out;
	I1 : bool;out;
	I2 : bool;out;
	I3 : bool;out;
	I4 : bool;out;
	I5 : bool;out;
	I6 : bool;out;
	I7 : bool;out;
	I8 : bool;out;
	I9 : bool;out;
	I10 : bool;out;
	I11 : bool;out;
	I12 : bool;out;
	FLAG: byte;out;
	ID: byte; out;
	YY: word; out;
	MM: word; out;
	DD: word; out;
	HH: word; out;
	NN: word; out;
	SS: word; out;
	MS: word; out;
	CH: byte; out;
	VAL: byte; out;
implementation

static char buffer[24];
static int check_buffer()
{
	int i;
	byte * pci;
	pci = (byte*)io_mem(ADDR);
	for(i=0; i<24; i++){
		if(pci[i]!=buffer[i]){
			return 1;
		}
	}
	
	return 0;
}

static void write_buffer()
{
	int i;
	byte *pco;
	pco = (byte*)io_mem(ADDR);

	*pco = ID_CLR;	

	for(i=0;i<24;i++){
		buffer[i]=pco[i];
	}
}

static word a2w(byte *pc)
{
	byte l, h;
	
	h = *pc - 48;
	pc++;
	l = *pc - 48;
	
	return h * 10 + l;
}

static void get_value()
{
	int * pVal;
	byte *pci;
	
	pVal = (int*)io_mem(ADDR);
	pci = (char*)pVal;
	
#define _(n) I##n = *pVal & (0x1 << (n-1))?1:0;

	_(1);
	_(2);
	_(3);
	_(4);
	_(5);
	_(6);
	_(7);
	_(8);
	_(9);
	_(10);
	_(11);
	_(12);

	VF=0xffff;

	pci += 2;
	FLAG = *pci;
	pci++;
	ID = *pci;
	pci++;
	YY = a2w(pci);
	pci += 2;
	MM = a2w(pci);
	pci += 2;
	DD = a2w(pci);
	pci += 2;
	HH = a2w(pci);
	pci += 2;
	NN = a2w(pci);
	pci += 2;
	SS = a2w(pci);
	pci += 2;
	MS = *((word*)pci);
	pci += 2;
	CH = *pci;
	pci++;
	VAL = *pci;
}

void main()
{
	byte * pVal;
	pVal = (byte*)io_mem(ADDR);
	if(!pVal){
		return;
	}

	if(check_buffer()==1){
		get_value();
	}
	write_buffer();				
}
