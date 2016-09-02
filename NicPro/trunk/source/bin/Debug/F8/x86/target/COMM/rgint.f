fblock RGINT;
uuid{7b02-54e8-fb3b-1cb0};
var
	ADDR: word; in;
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
	integer *pi;

	p = io_mem(0) - (6 + 6 + 1) * 4 - 128 * 1024;
	
	if(ADDR >= 1024){
		return -1;
	}
	
	p += ADDR * 32;
	pi = (integer*)p;
	
	OUT1 = *pi;
	pi++;
	OUT2 = *pi;
	pi++;
	OUT3 = *pi;
	pi++;
	OUT4 = *pi;
	pi++;
	OUT5 = *pi;
	pi++;
	OUT6 = *pi;
	pi++;
	OUT7 = *pi;
	pi++;
	OUT8 = *pi;
	
	return 0;	
}
