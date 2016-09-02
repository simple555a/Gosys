fblock DM122;
attribute(autoscan);
uuid{0e35-8894-0dca-dbd7};
var
	ADDR : word;in;
	S1 : bool;in;
	S2 : bool;in;
	S3 : bool;in;
	S4 : bool;in;
	S5 : bool;in;
	S6 : bool;in;
	S7 : bool;in;
	S8 : bool;in;
	S9 : bool;in;
	S10 : bool;in;
	S11 : bool;in;
	S12 : bool;in;
	S13 : bool;in;
	S14 : bool;in;
	S15 : bool;in;
	S16 : bool;in;	
	Disable: bool;in;
	Sel_CH : word;in;
	
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
implementation


void main()
{
	int * pVal;
	pVal = (int*)io_mem(ADDR);
	if(!pVal){
		VF=0;
	}else{
	
#define _(n) I##n = *pVal & (0x1 << (n-1))?1:0;


if(Disable == 1){
	
		if(Sel_CH & 0x0001){
			I1 = S1;
		}else{ 
	       		_(1);
	       	}
		if(Sel_CH & 0x0002){
			I2 = S2;
		}else{ 
	       		_(2);
	       	}
		if(Sel_CH & 0x0004){
			I3 = S3;
		}else{ 
	       		_(3);
	       	}
		if(Sel_CH & 0x0008){
			I4 = S4;
		}else{ 
	       		_(4);
	       	}
		if(Sel_CH & 0x0010){
			I5 = S5;
		}else{ 
	       		_(5);
	       	}
		if(Sel_CH & 0x0020){
			I6 = S6;
		}else{ 
	       		_(6);
	       	}
		if(Sel_CH & 0x0040){
			I7 = S7;
		}else{ 
	       		_(7);
	       	}
		if(Sel_CH & 0x0080){
			I8 = S8;
		}else{ 
	       		_(8);
	       	}
	       	if(Sel_CH & 0x0100){
			I9 = S9;
		}else{ 
	       		_(9);
	       	}
	       	if(Sel_CH & 0x0200){
			I10 = S10;
		}else{ 
	       		_(10);
	       	}
	       	if(Sel_CH & 0x0400){
			I11 = S11;
		}else{ 
	       		_(11);
	       	}
	       	if(Sel_CH & 0x0800){
			I12 = S12;
		}else{ 
	       		_(12);
	       	}
	       	if(Sel_CH & 0x1000){
			I13 = S13;
		}else{ 
	       		_(13);
	       	}
	       	if(Sel_CH & 0x2000){
			I14 = S14;
		}else{ 
	       		_(14);
	       	}
	       	if(Sel_CH & 0x4000){
			I15 = S15;
		}else{ 
	       		_(15);
	       	}
	       	if(Sel_CH & 0x8000){
			I16 = S16;
		}else{ 
	       		_(16);
	       	}
		VF=Sel_CH;
	}else{
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
		VF=0xffff;}
}}