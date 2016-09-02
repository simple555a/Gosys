fblock DM113;
attribute(autoscan);
uuid{6919-5a62-4037-7f88};
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
	I13 : bool;out;
	I14 : bool;out;
	I15 : bool;out;
	I16 : bool;out;	
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

static word a2w(byte *pc)
{
	byte l, h;
	
	h = *pc - 48;
	pc++;
	l = *pc - 48;
	
	return h * 10 + l;
}

void main()
{
	int * pVal;
	byte *pci;
	byte *pco;
	
	pVal = (int*)io_mem(ADDR);
	if(!pVal){
		VF=0;
		return;
	}
	pci = (char*)pVal;
	pco = pci;// + 24;
	
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
	_(13);
	_(14);
	_(15);
	_(16);

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
	
	if(FLAG){
		// 输入输出都是从0开始的，这是一个bug。
		// 判断一下FLAG的值，可以在网络故障时保持DI数值不变。
		*pco = ID_CLR;	
	}
}
