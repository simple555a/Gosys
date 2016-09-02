fblock WGFLOAT;
uuid{a46d-bb3d-c18b-b304};
var
	SADDR: word; in;
	TADDR: word; in;
	IN1: float; in;
	IN2: float; in;
	IN3: float; in;
	IN4: float; in;
	IN5: float; in;
	IN6: float; in;
	IN7: float; in;
	IN8: float; in;
	OUT1: float; out;
	OUT2: float; out;
	OUT3: float; out;
	OUT4: float; out;
	OUT5: float; out;
	OUT6: float; out;
	OUT7: float; out;
	OUT8: float; out;	
implementation

int main()
{
	char *p;
	word *pw;
	float *pf;
	p = io_mem(0) - (6 + 6 + 1) * 4 - 136 * 1024;
	
	if(SADDR >= 32){
		return -1;
	}
	
	p += SADDR * 36;
	
	pw = (word*)p;
	*pw = 0x55aa;
	pw++;
	*pw = TADDR;
	
	pf = (float*)p + 1;
	*pf = IN1;
	OUT1 = IN1;
	pf++;
	*pf = IN2;
	OUT2 = IN2;
	pf++;
	*pf = IN3;
	OUT3 = IN3;
	pf++;
	*pf = IN4;
	OUT4 = IN4;
	pf++;
	*pf = IN5;
	OUT5 = IN5;
	pf++;
	*pf = IN6;
	OUT6 = IN6;
	pf++;
	*pf = IN7;
	OUT7 = IN7;
	pf++;
	*pf = IN8;
	OUT8 = IN8;

	return 0;	
}
