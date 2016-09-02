fblock RGBOOL;
uuid{14b2-dca9-e4ea-7fd2};
var
	ADDR: word; in;
	OUT1: bool; out;
	OUT2: bool; out;
	OUT3: bool; out;
	OUT4: bool; out;
	OUT5: bool; out;
	OUT6: bool; out;
	OUT7: bool; out;
	OUT8: bool; out;
	OUT9: bool; out;
	OUT10: bool; out;
	OUT11: bool; out;
	OUT12: bool; out;
	OUT13: bool; out;
	OUT14: bool; out;
	OUT15: bool; out;
	OUT16: bool; out;
	OUT17: bool; out;
	OUT18: bool; out;
	OUT19: bool; out;
	OUT20: bool; out;
	OUT21: bool; out;
	OUT22: bool; out;
	OUT23: bool; out;
	OUT24: bool; out;
	OUT25: bool; out;
	OUT26: bool; out;
	OUT27: bool; out;
	OUT28: bool; out;
	OUT29: bool; out;
	OUT30: bool; out;
	OUT31: bool; out;
	OUT32: bool; out;
	
implementation

int main()
{
	char *p;
	bool *pb;

	p = io_mem(0) - (6 + 6 + 1) * 4 - 128 * 1024;
	
	if(ADDR >= 1024){
		return -1;
	}
	
	p += ADDR * 32;
	pb = (bool*)p;
	
	OUT1 = *pb;
	pb++;
	OUT2 = *pb;
	pb++;
	OUT3 = *pb;
	pb++;
	OUT4 = *pb;
	pb++;
	OUT5 = *pb;
	pb++;
	OUT6 = *pb;
	pb++;
	OUT7 = *pb;
	pb++;
	OUT8 = *pb;
	
	pb++;
	OUT9 = *pb;
	pb++;
	OUT10 = *pb;
	pb++;
	OUT11 = *pb;
	pb++;
	OUT12 = *pb;
	pb++;
	OUT13 = *pb;
	pb++;
	OUT14 = *pb;
	pb++;
	OUT15 = *pb;
	pb++;
	OUT16 = *pb;

	pb++;
	OUT17 = *pb;
	pb++;
	OUT18 = *pb;
	pb++;
	OUT19 = *pb;
	pb++;
	OUT20 = *pb;
	pb++;
	OUT21 = *pb;
	pb++;
	OUT22 = *pb;
	pb++;
	OUT23 = *pb;
	pb++;
	OUT24 = *pb;

	pb++;
	OUT25 = *pb;
	pb++;
	OUT26 = *pb;
	pb++;
	OUT27 = *pb;
	pb++;
	OUT28 = *pb;
	pb++;
	OUT29 = *pb;
	pb++;
	OUT30 = *pb;
	pb++;
	OUT31 = *pb;
	pb++;
	OUT32 = *pb;

	return 0;	
}
