fblock WGBOOL;
uuid{ce62-1686-609a-6df2};
var
	SADDR: word; in;
	TADDR: word; in;
	IN1: bool; in;
	IN2: bool; in;
	IN3: bool; in;
	IN4: bool; in;
	IN5: bool; in;
	IN6: bool; in;
	IN7: bool; in;
	IN8: bool; in;
	IN9: bool; in;
	IN10: bool; in;
	IN11: bool; in;
	IN12: bool; in;
	IN13: bool; in;
	IN14: bool; in;
	IN15: bool; in;
	IN16: bool; in;
	IN17: bool; in;
	IN18: bool; in;
	IN19: bool; in;
	IN20: bool; in;
	IN21: bool; in;
	IN22: bool; in;
	IN23: bool; in;
	IN24: bool; in;
	IN25: bool; in;
	IN26: bool; in;
	IN27: bool; in;
	IN28: bool; in;
	IN29: bool; in;
	IN30: bool; in;
	IN31: bool; in;
	IN32: bool; in;
	
implementation

int main()
{
	char *p;
	word *pw;
	bool *pb;
	p = io_mem(0) - (6 + 6 + 1) * 4 - 136 * 1024;
	
	if(SADDR >= 32){
		return -1;
	}
	
	p += SADDR * 36;
	
	pw = (word*)p;
	*pw = 0x55aa;
	pw++;
	*pw = TADDR;
	
	pb = (bool*)p + 4;
	*pb = IN1;
	pb++;
	*pb = IN2;
	pb++;
	*pb = IN3;
	pb++;
	*pb = IN4;
	pb++;
	*pb = IN5;
	pb++;
	*pb = IN6;
	pb++;
	*pb = IN7;
	pb++;
	*pb = IN8;

	pb++;
	*pb = IN9;
	pb++;
	*pb = IN10;
	pb++;
	*pb = IN11;
	pb++;
	*pb = IN12;
	pb++;
	*pb = IN13;
	pb++;
	*pb = IN14;
	pb++;
	*pb = IN15;
	pb++;
	*pb = IN16;
	
	pb++;
	*pb = IN17;
	pb++;
	*pb = IN18;
	pb++;
	*pb = IN19;
	pb++;
	*pb = IN20;
	pb++;
	*pb = IN21;
	pb++;
	*pb = IN22;
	pb++;
	*pb = IN23;
	pb++;
	*pb = IN24;

	pb++;
	*pb = IN25;
	pb++;
	*pb = IN26;
	pb++;
	*pb = IN27;
	pb++;
	*pb = IN28;
	pb++;
	*pb = IN29;
	pb++;
	*pb = IN30;
	pb++;
	*pb = IN31;
	pb++;
	*pb = IN32;

	return 0;	
}
