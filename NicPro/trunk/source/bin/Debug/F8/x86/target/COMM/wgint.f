fblock WGINT;
uuid{3024-d597-84e9-f479};
var
	SADDR: word; in;
	TADDR: word; in;
	IN1: integer; in;
	IN2: integer; in;
	IN3: integer; in;
	IN4: integer; in;
	IN5: integer; in;
	IN6: integer; in;
	IN7: integer; in;
	IN8: integer; in;
	OUT1: integer; out;
	OUT2: integer; out;
	OUT3: integer; out;
	OUT4: integer; out;
	OUT5: integer; out;
	OUT6: integer; out;
	OUT7: integer; out;
	OUT8: integer; out;
	
implementation

int main()
{
	char *p;
	word *pw;
	integer *pi;
	p = io_mem(0) - (6 + 6 + 1) * 4 - 136 * 1024;
	
	if(SADDR >= 32){
		return -1;
	}
	
	p += SADDR * 36;
	
	pw = (word*)p;
	*pw = 0x55aa;
	pw++;
	*pw = TADDR;
	
	pi = (integer*)p + 1;
	*pi = IN1;
	OUT1 = IN1;
	pi++;
	*pi = IN2;
	OUT2 = IN2;
	pi++;
	*pi = IN3;
	OUT3 = IN3;
	pi++;
	*pi = IN4;
	OUT4 = IN4;
	pi++;
	*pi = IN5;   
	OUT5 = IN5;
	pi++;
	*pi = IN6;
	OUT6 = IN6;
	pi++;
	*pi = IN7;  
	OUT7 = IN8;
	pi++;
	*pi = IN8;
	OUT8 = IN8;
	
	return 0;	
}
