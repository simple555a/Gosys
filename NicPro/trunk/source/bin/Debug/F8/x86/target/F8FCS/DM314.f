fblock DM314;
attribute(autoscan);
uuid{2e0d-d68a-3b62-272c};
var
	ADDR : word;in;
	FA1 : float;in;
	FA2 : float;in;
	FA3 : float;in;
	FA4 : float;in;
	FA5 : float;in;
	FA6 : float;in;
	FA7 : float;in;
	FA8 : float;in;
	Force: bool;in;
	Sel_CH : byte;in;
	
	VF : byte;out;
	A1 : float;out;
	A2 : float;out;
	A3 : float;out;
	A4 : float;out;
	A5 : float;out;
	A6 : float;out;
	A7 : float;out;
	A8 : float;out;
	Err1:bool; out;
	Err2:bool; out;
	Err3:bool; out;
	Err4:bool; out;
	Err5:bool; out;
	Err6:bool; out;
	Err7:bool; out;
	Err8:bool; out;
		
implementation
int main()
{
	word * pVal;
	byte * pCfg;
	pVal = (word*)io_mem(ADDR);
	pCfg = (byte*)dev_cfg(ADDR);
	if(!pVal || !pCfg){
		VF=0xff;
		return -1;
	}
	VF=0;

#define aa(n) \
	if(pCfg[n-1] == 0){\
		if(pVal[n-1]<4096/5){\
			if(pVal[n-1]<4096/16){\
				VF |= 1<<(n-1);\
			}\
			A##n = 0;\
		}else{\
			A##n = (pVal[n-1]*5-4096)/(4*4096.);\
		}\
	}else{\
		A##n=pVal[n-1]/4096.;\
	}

		if(Force && Sel_CH & 0x0001){
			A1 = FA1;
		}else{ 
	       		aa(1);
	       	}
		if(Force && Sel_CH & 0x0002){
			A2 = FA2;
		}else{ 
	       		aa(2);
	       	}
		if(Force && Sel_CH & 0x0004){
			A3 = FA3;
		}else{ 
	       		aa(3);
	       	}
		if(Force && Sel_CH & 0x0008){
			A4 = FA4;
		}else{ 
	       		aa(4);
	       	}
		if(Force && Sel_CH & 0x0010){
			A5 = FA5;
		}else{ 
	       		aa(5);
	       	}
		if(Force && Sel_CH & 0x0020){
			A6 = FA6;
		}else{ 
	       		aa(6);
	       	}
		if(Force && Sel_CH & 0x0040){
			A7 = FA7;
		}else{ 
	       		aa(7);
	       	}
		if(Force && Sel_CH & 0x0080){
			A8 = FA8;
		}else{ 
	       		aa(8);
	       	}
	
	Err1 = VF & 0x01;
	Err2 = VF & 0x02;
	Err3 = VF & 0x04;
	Err4 = VF & 0x08;
	Err5 = VF & 0x10;
	Err6 = VF & 0x20;
	Err7 = VF & 0x40;
	Err8 = VF & 0x80;
	
}