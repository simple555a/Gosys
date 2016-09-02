fblock RGFLOAT;
uuid{33df-6ca0-cf0f-ec4a};
var
	ADDR: word; in;
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
	float *pf;

	p = io_mem(0) - (6 + 6 + 1) * 4 - 128 * 1024;
	
	if(ADDR >= 1024){
		return -1;
	}
	
	p += ADDR * 32;
	pf = (float*)p;
	
	OUT1 = *pf;
	pf++;
	OUT2 = *pf;
	pf++;
	OUT3 = *pf;
	pf++;
	OUT4 = *pf;
	pf++;
	OUT5 = *pf;
	pf++;
	OUT6 = *pf;
	pf++;
	OUT7 = *pf;
	pf++;
	OUT8 = *pf;
	
	return 0;	
}
