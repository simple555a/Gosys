fblock DM422;
attribute(autoscan);
uuid{12d9-5522-7771-bd62};
var
	ADDR : word;in;
	Out1 : float;in;
	Out2 : float;in;
	Out3 : float;in;
	Out4 : float;in;
	Out5 : float;in;
	Out6 : float;in;
	Out7 : float;in;
	Out8 : float;in;
	Disable : bool;in;
	Sel_CH  : byte;in;
		
	Dis_Out1 : float;in;
	Dis_Out2 : float;in;
	Dis_Out3 : float;in;
	Dis_Out4 : float;in;
	Dis_Out5 : float;in;
	Dis_Out6 : float;in;
	Dis_Out7 : float;in;
	Dis_Out8 : float;in;	
	VF : byte;out;
	O1 : word;out;
	O2 : word;out;
	O3 : word;out;
	O4 : word;out;
	O5 : word;out;
	O6 : word;out;
	O7 : word;out;
	O8 : word;out;
	
	ERR1 : bool;out;
	ERR2 : bool;out;
	ERR3 : bool;out;
	ERR4 : bool;out;
	ERR5 : bool;out;
	ERR6 : bool;out;
	ERR7 : bool;out;
	ERR8 : bool;out;
	
implementation
static float aa1,aa2,aa3,aa4,aa5,aa6,aa7,aa8;
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
	word *pwo;
	
	pco = (byte*)io_mem(ADDR);	
	pwo = (word*)io_mem(ADDR);
	
	if(Disable){
		if(Sel_CH & 0x0001){
			aa1 = Dis_Out1;
		}else{
			aa1 = Out1;
		}
		if(Sel_CH & 0x0002){
			aa2 = Dis_Out2;
		}else{
			aa2 = Out2;
		}
		if(Sel_CH & 0x0004){
			aa3 = Dis_Out3;
		}else{
			aa3 = Out3;
		}
		if(Sel_CH & 0x0008){
			aa4 = Dis_Out4;
		}else{
			aa4 = Out4;
		}
		if(Sel_CH & 0x0010){
			aa5 = Dis_Out5;
		}else{
			aa5 = Out5;
		}
		if(Sel_CH & 0x0020){
			aa6 = Dis_Out6;
		}else{
			aa6 = Out6;
		}
		if(Sel_CH & 0x0040){
			aa7 = Dis_Out7;
		}else{
			aa7 = Out7;
		}
		if(Sel_CH & 0x0080){
			aa8 = Dis_Out8;
		}else{
			aa8 = Out8;
		}
	}else{
		aa1 = Out1;
		aa2 = Out2;
		aa3 = Out3;
		aa4 = Out4;
		aa5 = Out5;
		aa6 = Out6;
		aa7 = Out7;
		aa8 = Out8;
	}	
		
	#define _trans(n) pwo[n-1]=aa##n*65535
	_trans(1);
	_trans(2);
	_trans(3);
	_trans(4);
	_trans(5);
	_trans(6);
	_trans(7);
	_trans(8);
	O1 = pwo[0];
	O2 = pwo[1];
	O3 = pwo[2];
	O4 = pwo[3];
	O5 = pwo[4];
	O6 = pwo[5];
	O7 = pwo[6];
	O8 = pwo[7];

	for(i=0;i<24;i++){
		buffer[i]=pco[i];
	}
}

static void get_value()
{
	byte * pci;
	pci = (byte*)io_mem(ADDR);
	
	if(pci[0]==0){
		ERR1=1;
	}else{
		ERR1=0;
	}
	if(pci[1]==0){
		ERR2=1;
	}else{
		ERR2=0;
	}
	if(pci[2]==0){
		ERR3=1;
	}else{
		ERR3=0;
	}
	if(pci[3]==0){
		ERR4=1;
	}else{
		ERR4=0;
	}
	if(pci[4]==0){
		ERR5=1;
	}else{
		ERR5=0;
	}
	if(pci[5]==0){
		ERR6=1;
	}else{
		ERR6=0;
	}
	if(pci[6]==0){
		ERR7=1;
	}else{
		ERR7=0;
	}
	if(pci[7]==0){
		ERR8=1;
	}else{
		ERR8=0;
	}
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