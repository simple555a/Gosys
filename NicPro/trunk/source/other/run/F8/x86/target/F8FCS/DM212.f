fblock DM212;
attribute(autoscan);
uuid{a0b1-e625-5ef8-5e4d};
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
	Q1 : bool;in;
	Q2 : bool;in;
	Q3 : bool;in;
	Q4 : bool;in;
	Q5 : bool;in;
	Q6 : bool;in;
	Q7 : bool;in;
	Q8 : bool;in;
	Q9 : bool;in;
	Q10 : bool;in;
	Q11 : bool;in;
	Q12 : bool;in;
	Q13 : bool;in;
	Q14 : bool;in;
	Q15 : bool;in;
	Q16 : bool;in;	
	Disable: bool;in;
	Sel_CH : word;in;
	
	VF  : word;out;

	
implementation
static bool SS1,SS2,SS3,SS4,SS5,SS6,SS7,SS8,SS9,SS10,SS11,SS12,SS13,SS14,SS15,SS16;

void main()
{
	f8_u16 value;
	value = 0;
	SS1 = S1;

	#define _trans(n) if(SS##n){value |=  (1 << n-1);}


		if((Disable == 1) && (Sel_CH & 0x0001)){
			SS1 = S1;
		}else{ 
	       		SS1 = Q1;
		}
		if((Disable == 1) && (Sel_CH & 0x0002)){
			SS2 = S2;
		}else{ 
	       		SS2 = Q2;
	       	}
		if((Disable == 1) && (Sel_CH & 0x0004)){
			SS3 = S3;
		}else{ 
	       		SS3 = Q3;
	       	}
		if((Disable == 1) && (Sel_CH & 0x0008)){
			SS4 = S4;
		}else{ 
	       		SS4 = Q4;
	       	}
		if((Disable == 1) && (Sel_CH & 0x0010)){
			SS5 = S5;
		}else{ 
	       		SS5 = Q5;
	       	}
		if((Disable == 1) && (Sel_CH & 0x0020)){
			SS6 = S6;
		}else{ 
	       		SS6 = Q6;
	       	}
		if((Disable == 1) && (Sel_CH & 0x0040)){
			SS7 = S7;
		}else{ 
	       		SS7 = Q7;
	       	}
		if((Disable == 1) && (Sel_CH & 0x0080)){
			SS8 = S8;
		}else{ 
	       		SS8 = Q8;
	       	}
	       	if((Disable == 1) && (Sel_CH & 0x0100)){
			SS9 = S9;
		}else{ 
	       		SS9 = Q9;
	       	}
	       	if((Disable == 1) && (Sel_CH & 0x0200)){
			SS10 = S10;
		}else{ 
	       		SS10 = Q10;
	       	}
	       	if((Disable == 1) && (Sel_CH & 0x0400)){
			SS11 = S11;
		}else{ 
	       		SS11 = Q11;
	       	}
	       	if((Disable == 1) && (Sel_CH & 0x0800)){
			SS12 = S12;
		}else{ 
	       		SS12 = Q12;
	       	}
	       	if((Disable == 1) && (Sel_CH & 0x1000)){
			SS13 = S13;
		}else{ 
	       		SS13 = Q13;
	       	}
	       	if((Disable == 1) && (Sel_CH & 0x2000)){
			SS14 = S14;
		}else{ 
	       		SS14 = Q14;
	       	}
	       	if((Disable == 1) && (Sel_CH & 0x4000)){
			SS15 = S15;
		}else{ 
	       		SS15 = Q15;
	       	}
	       	if((Disable == 1) && (Sel_CH & 0x8000)){
			SS16 = S16;
		}else{ 
	       		SS16 = Q16;
	       	}
		
		if(Disable == 1){
			VF=Sel_CH;
		}else{ 
	       		VF=0xffff;
		}
		_trans(1);
		_trans(2);
		_trans(3);
		_trans(4);
		_trans(5);
		_trans(6);
		_trans(7);
		_trans(8);
		_trans(9);
		_trans(10);
		_trans(11);
		_trans(12);
		_trans(13);
		_trans(14);
		_trans(15);
		_trans(16);
		
	*((f8_u16*)io_mem(ADDR)) = value;

}