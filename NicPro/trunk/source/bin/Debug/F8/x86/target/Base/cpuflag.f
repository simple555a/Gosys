fblock CPUFLAG;
uuid{9a17-ab40-fbcd-1e05};
var
	FLAG: integer; in;
	RUN: bool; out;
	STOP: bool; out;
	STANDBY: bool; out;
	SYNC: bool; out;
	PROG: bool; out;
	BUSA: bool; out;
	BUSB: bool; out;
	
implementation

/*
	kernel flags constants
*/
#define KERN_F_ENABLE_INPUT			(0x1 << 0)
#define KERN_F_ENABLE_OUTPUT		(0x1 << 1)
#define KERN_F_ENABLE_IO			(KERN_F_ENABLE_INPUT | KERN_F_ENABLE_OUTPUT)
#define KERN_F_ENABLE_STANDBY		(0x1 << 2)
#define KERN_F_ENABLE_SCHEDULER		(0x1 << 3)
#define KERN_F_ENABLE_AGENT			(0x1 << 4)
#define KERN_F_SERIALIZED_IO (0x1<<5)
#define KERN_F_ENABLE_WEB (0x1<<6)
#define KERN_F_ENABLE_WATCHDOG (0x1<<7)
#define KERN_FEATURES_MASK 0xff

/* normal startup flag, four-phase basic processor cycle, 0x1f */
#define KERN_F_NORMAL (KERN_F_ENABLE_IO | KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_AGENT \
		| KERN_F_ENABLE_STANDBY | KERN_F_ENABLE_WATCHDOG | KERN_F_SERIALIZED_IO)
/* w/o IO phase, 0x1c */
#define KERN_F_HEADLESS (KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_AGENT | \
		KERN_F_ENABLE_STANDBY | KERN_F_ENABLE_WATCHDOG)
/* w/o standby phase, 0x1b */
#define KERN_F_SINGLE (KERN_F_ENABLE_IO | KERN_F_ENABLE_SCHEDULER | KERN_F_ENABLE_AGENT | \
		KERN_F_ENABLE_WATCHDOG)

/* 
* PANEL LED indication and key switch position 
* bit offset in kernel.flags
**/

#define FKERN_LED_RUNNING 8
#define FKERN_LED_STOP 9
#define FKERN_LED_STANDBY 10
#define FKERN_LED_BUSA 11
#define FKERN_LED_BUSB 12
#define FKERN_LED_SOFT_LOCK 13

#define FKERN_LED_DBG1 14
#define FKERN_LED_DBG2 15

#define FKERN_KEY_A_B  16
#define FKERN_KEY_RUN 17
#define FKERN_KEY_STOP 18
#define FKERN_KEY_LOCK 19

#define FKERN_LED_EX_RUNNING 20
#define FKERN_LED_SOFT_STOP 21
#define FKERN_LED_SYNCHRONIZED 22
int main()
{
	if(FLAG & (0x1 << FKERN_LED_RUNNING)){
		RUN = true;
	}else{
		RUN = false;
	}
	
	if(FLAG & (0x1 << FKERN_LED_STOP)){
		STOP = true;
	}else{
		STOP = false;
	}
	
	if(FLAG & (0x1 << FKERN_LED_STANDBY)){
		STANDBY = true;		
	}else{
		STANDBY = false;
	}

	if(FLAG & (0x1 << FKERN_LED_SYNCHRONIZED)){
		SYNC = true;
	}else{
		SYNC = false;
	}

	if(FLAG & (0x1 << FKERN_LED_SOFT_LOCK)){
		PROG = true;
	}else{
		PROG = false;
	}

	if(FLAG & (0x1 << FKERN_LED_BUSA)){
		BUSA = true;
	}else{
		BUSA = false;
	}

	if(FLAG & (0x1 << FKERN_LED_BUSB)){
		BUSB = true;
	}else{
		BUSB = false;
	}

	return 0;
}	
