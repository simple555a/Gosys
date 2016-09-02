fblock DM412;
attribute(autoscan);
uuid{0afb-1562-aae4-f155};
var
	ADDR : word;in;
	Out1 : float;in;
	Out2 : float;in;
	Out3 : float;in;
	Out4 : float;in;
	Disable : bool;in;
	Sel_CH  : byte;in;
		
	Dis_Out1 : float;in;
	Dis_Out2 : float;in;
	Dis_Out3 : float;in;
	Dis_Out4 : float;in;	
	VF : byte;out;
	O1 : word;out;
	O2 : word;out;
	O3 : word;out;
	O4 : word;out;
	
implementation
static float aa1,aa2,aa3,aa4;
void main()
{
	word * pVal;
	pVal = (byte*)io_mem(ADDR);
	if(!pVal){
		return;
	}
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
	}else{
		aa1 = Out1;
		aa2 = Out2;
		aa3 = Out3;
		aa4 = Out4;
	}	
		
	#define _trans(n) pVal[n-1]=aa##n*65535
	_trans(1);
	_trans(2);
	_trans(3);
	_trans(4);
	O1 = pVal[0];
	O2 = pVal[1];
	O3 = pVal[2];
	O4 = pVal[3];
	
}