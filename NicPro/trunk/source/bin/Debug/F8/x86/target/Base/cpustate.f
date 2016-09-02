fblock CPUSTATE;
uuid{551d-ced0-91ba-de54};
var
	PSTA: char; out;
	SSTA: char; out;
	PFLG: integer; out;
	SFLG: integer; out;
	CC1: float; out;
	CC2: float; out;
	CC3: float; out;
	CC4: float; out;
	CC5: float; out;
	CC6: float; out;
	CC7: float; out;
	CC8: float; out;
	CC9: float; out;
	CC10: float; out;
	CC11: float; out;
	CC12: float; out;
	CC13: float; out;
	CC14: float; out;
	CC15: float; out;
	CC16: float; out;
	
implementation

int main()
{
	char *p_cpu_stat;
	char *p_peer_stat;
	char *p_sta_stat;
	int *p_int;
	float *p_float;
	
	p_cpu_stat = io_mem(0) - (6 + 6 + 1) * 4 - 256 * 1024 - 4096 * 36 - 4096 * 4 - 184;
	// sizeof(jmp_buf) = 52;
	p_peer_stat = p_cpu_stat - 52 - 16 - 8 - 4 - 184;
	p_sta_stat = io_mem(0) + 128 * 64 + 128 * 44 + 128 * 120;
	
	PSTA = *(p_cpu_stat + 2);
	SSTA = *(p_peer_stat + 2);
	p_int = (int*)(p_cpu_stat + 4);	
	PFLG = *p_int;
	p_int = (int*)(p_peer_stat + 4);	
	SFLG = *p_int;
	
	p_float = (float*)(p_sta_stat + 4 + 56);
	CC1 = *p_float + *(p_float + 2);
	p_float += 30;
	CC2 = *p_float + *(p_float + 2);
	p_float += 30;
	CC3 = *p_float + *(p_float + 2);
	p_float += 30;
	CC4 = *p_float + *(p_float + 2);
	p_float += 30;
	CC5 = *p_float + *(p_float + 2);
	p_float += 30;
	CC6 = *p_float + *(p_float + 2);
	p_float += 30;
	CC7 = *p_float + *(p_float + 2);
	p_float += 30;
	CC8 = *p_float + *(p_float + 2);
	p_float += 30;
	CC9 = *p_float + *(p_float + 2);
	p_float += 30;
	CC10 = *p_float + *(p_float + 2);
	p_float += 30;
	CC11 = *p_float + *(p_float + 2);
	p_float += 30;
	CC12 = *p_float + *(p_float + 2);
	p_float += 30;
	CC13 = *p_float + *(p_float + 2);
	p_float += 30;
	CC14 = *p_float + *(p_float + 2);
	p_float += 30;
	CC15 = *p_float + *(p_float + 2);
	p_float += 30;
	CC16 = *p_float + *(p_float + 2);

	return 0;
}

