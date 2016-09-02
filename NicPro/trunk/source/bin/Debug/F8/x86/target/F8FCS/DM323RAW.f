fblock DM323RAW;
attribute(autoscan);
uuid{74ac-cc70-dd10-bc55};
var
	ADDR : word;in;
        M1 : byte;in;
        M2 : byte;in;
        M3 : byte;in;
        M4 : byte;in;
        M5 : byte;in;
        M6 : byte;in;
        M7 : byte;in;
        M8 : byte;in;
        VF  : word;out;
	A1 : float;out;
	A2 : float;out;
	A3 : float;out;
	A4 : float;out;
	A5 : float;out;
	A6 : float;out;
	A7 : float;out;
	A8 : float;out;
	
implementation
static float aa1,aa2,aa3,aa4,aa5,aa6,aa7,aa8;
static float get_rtd_val(word val_ch)
{
	float val;

	if(val_ch == 0xffff){
		return -999.;
	}else{

	
	val = (float)val_ch * 4000 / 32768 / 8;
	}

	return val;
}

int main()
{
	int i;
	word * pVal;
	byte * pco;
	byte * pci;
	pVal = (word*)io_mem(ADDR);
	pco = (byte*)io_mem(ADDR);
	pci = (byte*)io_mem(ADDR);
	if(!pVal){
		return -1;
	}
	if(M1 == *pci && M2 == *(pci+1) && M3 == *(pci+2)&& M4 == *(pci+3)){
	A1 = aa1;
	A2 = aa2;
	A3 = aa3;
	A4 = aa4;
	A5 = aa5;
	A6 = aa6;
	A7 = aa7;
	A8 = aa8;
	}else{
	A1 = get_rtd_val(pVal[0]);
	A2 = get_rtd_val(pVal[1]);
	A3 = get_rtd_val(pVal[2]);
	A4 = get_rtd_val(pVal[3]);
	A5 = get_rtd_val(pVal[4]);
	A6 = get_rtd_val(pVal[5]);
	A7 = get_rtd_val(pVal[6]);
	A8 = get_rtd_val(pVal[7]);
	aa1 = A1;
	aa2 = A2;
	aa3 = A3;
	aa4 = A4;
	aa5 = A5;
	aa6 = A6;
	aa7 = A7;
	aa8 = A8;
	}
	VF=0xFFFF;
	#define _trans(n) pco[n-1]=M##n
	_trans(1);
	_trans(2);
	_trans(3);
	_trans(4);
	_trans(5);
	_trans(6);
	_trans(7);
	_trans(8);
	
}