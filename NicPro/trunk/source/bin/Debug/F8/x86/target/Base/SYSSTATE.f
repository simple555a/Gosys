fblock SYSSTATE;		//ϵͳ״̬
attribute(autoscan);
uuid{d7cc-1db9-c362-4453};
var
	RUN : bool; out;
	RP : bool; out;
	CYCLES : dword; out;
	TIME : date; out;
	
implementation
static date due_T;

void on_start()
{
	RUN = 1;
	RP = 1;
	CYCLES = 0;
	// __asm("int $3");
}

void on_stop()
{
	// __asm("int $3");
	RUN = 0;
	RP = 0;
	CYCLES=0;
}

void main()
{
	static bool first_time = true;
	date now;
	now = gettime();

	if(first_time){
		due_T = now;
		first_time = false;
		return;
	}
	
	TIME = now - due_T;
	due_T = now;
	
	CYCLES++;
	if(CYCLES > 10){
		RP = 0;
	}
}
