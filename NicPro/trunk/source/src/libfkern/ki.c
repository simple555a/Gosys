/*
	kernel internal procedures
*/
#include "precomp.h"
#pragma hdrstop

int saveConfigWatchDog;

void ki_halt(struct kernel_t * kernel, f8_status code)
{
	f8_u32 c1,c2,c3;

	c1=c2=c3=0;
	if(kernel->current_blk){
		c1 = (f8_u32)(kernel->current_blk->uuid >> 32);
		c2 = (f8_u32)(kernel->current_blk->uuid & 0xffffffff);
		if(kernel->current_pin){
			c3 = __vcall__(
				kernel->current_blk, 
				id_by_pin, 
				(kernel->current_pin)
				);
		}else{
			c3 = -1;
		}
	}

	/*
	KD_PRINT((
		"Halt %d, block %08x-%08x, pin %d\n", 
		-code, 
		c1, c2, c3
		));
	*/
	ki_log(kernel, F8_EVENT(MODNUM_KERNEL, EVT_CLASS_FATAL, code), c1, c2, c3);

	/*
		׳ʿһȥ��
	*/
	kernel->status.state = KERN_S_HALTED;
	assert(0);
	longjmp(kernel->g_jmpbuf, -1);
}

void ki_log(struct kernel_t * kernel, f8_event code, int c1, int c2, int c3)
{
	struct klog_ent_t * le;
	
	le = &kernel->klog.logs[kernel->klog.tail];
	le->tm = ke_get_time();
	le->c[0] = (code & 0xffffff) | (MODNUM_KERNEL << 24);
	le->c[1] = c1;
	le->c[2] = c2;
	le->c[3] = c3;
	kernel->klog.tail++;
	if(kernel->klog.tail == KERN_LOG_BUF_SIZE){
		kernel->klog.tail = 0;
	}
}

void ki_logblk(struct kernel_t * kernel, f8_event code, IBlk * b, int c3)
{
	int c1, c2;
	c1 = (int)(b->uuid >> 32);
	c2 = (int)(b->uuid & 0xffffffff);
	ki_log(kernel, code, c1, c2, c3);
}

/*
	delete user program, keep configuration untouched
*/
void ki_clear_user_program(struct kernel_t * kernel)
{
	int i;
	ITreeNode_umount_children(__ucast__(IBlk, ITreeNode, kernel->sections));

	kernel->status.n_blocks = 0;
	kernel->status.n_memory = 0;
	kernel->status.n_cycleTime = 0;

	RtkInitializeListHead(&kernel->tmr_busy);
	RtkInitializeListHead(&kernel->tmr_free);
	for(i=0; i<TIMER_Q_LENGTH-1; i++){
		RtkInsertTailList(&kernel->tmr_free, &kernel->timers[i].link);
		kernel->timers[i].hint = TIMER_H_FREE;
	}
	
	kernel->eq_end = kernel->eq_head = 0;
	memset(&kernel->event_q, 0, sizeof(kernel->event_q));
	
	RtkInitializeListHead(&kernel->m_1131_blocks);
}

/*
	remove active program and revert to default configuration
*/
void ki_reset(struct kernel_t * kernel)
{
	ki_clear_user_program(kernel);
	ki_load_default_config(kernel);
	ki_init_mem_pointers(kernel);
	if(kernel == &g_kernel){
		ki_reset_var_table();
	}
}

f8_u8 ki_get_state()
{
	return kernelState;
}

/*
	F8 kernel state automata
*/
f8_status ki_switch_to(f8_u8 mode)
{
	if(kernelState == mode){
		return F8_INVALID_OPERATION;
	}
	
#if defined(__VXWORKS__)
#ifndef NDEBUG
{
	char tbuf[64];
	ke_time_to_string(ke_get_time(),tbuf,sizeof(tbuf));
	logMsg("%s : mode switch %s->%s\n", tbuf,ke_get_mode_name(kernelState), ke_get_mode_name(mode));
}
#endif
#endif
	
	/* turn on the POWER LED indicator */
	switch(mode){
	case KERN_S_RUNNING:
		//if(kernelState == KERN_S_STOPPED){
		//	ki_start(&g_kernel);
		//}
		ke_set_flag(FKERN_LED_RUNNING, 1);
		ke_set_flag(FKERN_LED_STOP, 0);
		ke_set_flag(FKERN_LED_STANDBY, 0);
		break;
		
	case KERN_S_STOPPED:
		//ki_stop(&g_kernel);
		ke_set_flag(FKERN_LED_RUNNING, 0);
		ke_set_flag(FKERN_LED_STOP, 1);
		ke_set_flag(FKERN_LED_STANDBY, 0);
		break;
		
	case KERN_S_STANDBY:
		ke_set_flag(FKERN_LED_RUNNING, 0);
		ke_set_flag(FKERN_LED_STOP, 0);
		ke_set_flag(FKERN_LED_STANDBY, 1);
		break;
	}

	ki_log(&g_kernel, F8_MODE_SWITCH, kernelState, mode, 0);
	kernelState = mode;
	return F8_SUCCESS;
}

LOCAL void _kkkk()
{
	static char lastRunFlag;
	static ktime_t t0;
	static f8_bool lastStartHolding;

	ktime_t t;	
	f8_bool runFlag;
	f8_bool startHolding=f8_false;
	f8_bool startReq=f8_false;

	/* check key switch events */
	runFlag = ke_get_flag(FKERN_KEY_RUN) || ke_get_flag(FKERN_KEY_LOCK);

/*

startHolding=TON(runFlag,1000)
startReq=TP(startHolding,-1)

the following code fragments is the standard way to implement the TP and
TON timers
*/

	t=ke_get_time();
	if(!runFlag){
		startHolding=f8_false;
		if(kernelState != KERN_S_STOPPED)
			ki_switch_to(KERN_S_STOPPED);
	}else{
		if(!lastRunFlag){
			t0=ke_get_time();
		}
		if(t-t0 > 10*1000*1000)
			startHolding=f8_true;
	}

	if(startHolding){
		if(!lastStartHolding)
			startReq=f8_true;
	}else
		startReq=f8_false;
	
	lastRunFlag=runFlag;
	lastStartHolding=startHolding; 

	if(startReq==f8_true && kernelState == KERN_S_STOPPED && !ke_get_flag(FKERN_LED_SOFT_STOP)){
		if(g_kernel.status.flags & KERN_F_ENABLE_STANDBY)
			ki_switch_to(KERN_S_ARBITRATING);
		else
			ki_switch_to(KERN_S_RUNNING);
	}
	// reset startReq after this cycle, making it a pulse signal
	startReq=f8_false;

	g_kernel.status.curr_time = t;
	g_kernel.status.length = sizeof(g_kernel.status);
	g_kernel.status.prog_id = g_kernel.sections->h.uuid;
}

/*
	update variables to indicate curr.s_addrent status of the kernel.
	and check for key-switch events, trigger status switch
	if necessary
*/
void ki_update_flags()
{
	/* write led status and get key status */
#if defined(ETX_BOARD)
	sysOutByte(0x280,ke_get_flag(FKERN_LED_SOFT_LOCK)?0:1);
	sysOutByte(0x281,ke_get_flag(FKERN_LED_STANDBY)?0:1);
	sysOutByte(0x282,ke_get_flag(FKERN_LED_RUNNING)?0:1);
	sysOutByte(0x283,ke_get_flag(FKERN_LED_STOP)?0:1);
	
	{
		f8_u8 b1,b2;
		b1=(sysInByte(0x284)&1)?0:1;
		b2=(sysInByte(0x285)&1)?0:1;
		ke_set_flag(FKERN_KEY_STOP, b1);
		ke_set_flag(FKERN_KEY_LOCK, b2);
		ke_set_flag(FKERN_KEY_RUN, !b1 && !b2);
	}
#elif defined(ETX_2)
	sysOutByte(0x243,ke_get_flag(FKERN_LED_SOFT_LOCK)?0:1);
	//sysOutByte(0x242,ke_get_flag(FKERN_LED_STANDBY)?0:1);
	sysOutByte(0x240,ke_get_flag(FKERN_LED_RUNNING)?0:1);
	sysOutByte(0x241,ke_get_flag(FKERN_LED_STOP)?0:1);	
	{
		f8_u8 b;
		b = sysInByte(0x245) & 0x01;
		if(b){
			ke_set_flag(FKERN_KEY_STOP, 1);
			ke_set_flag(FKERN_KEY_RUN, 0);
		}else{
			ke_set_flag(FKERN_KEY_STOP, 0);
			ke_set_flag(FKERN_KEY_RUN, 1);
		}	
		ke_set_flag(FKERN_KEY_LOCK, 0);
	}
	if(kernelState==KERN_S_STANDBY && ke_get_flag(FKERN_LED_SYNCHRONIZED)){
		sysOutByte(0x242, 0);
	}else{
		sysOutByte(0x242, 1);
	}


#elif defined(EM615_BOARD)
{
	f8_byte b;
	b=0;
	if(ke_get_flag(FKERN_LED_STANDBY)||(kernelState==KERN_S_STOPPED && ke_get_flag(FKERN_LED_SOFT_STOP)))
		b |= 0x4;
	if(ke_get_flag(FKERN_LED_SOFT_LOCK))
		b |= 0x8;
	if(ke_get_flag(FKERN_LED_STOP))
		b |= 0x10;
	if(kernelState==KERN_S_RUNNING && ke_get_flag(FKERN_LED_BUSB))
		b |= 0x20;
	if(ke_get_flag(FKERN_LED_RUNNING))
		b |= 0x40;
	if((kernelState==KERN_S_RUNNING && ke_get_flag(FKERN_LED_BUSA)) || 
		(kernelState==KERN_S_STANDBY && ke_get_flag(FKERN_LED_SYNCHRONIZED))
	)
		b |= 0x80;
	sysOutByte(0x208,b);
	b=sysInByte(0x200);	
	ke_set_flag(FKERN_KEY_STOP, b & 0x1? 1 : 0);
	ke_set_flag(FKERN_KEY_RUN, b & 0x1? 0 : 1);
	ke_set_flag(FKERN_KEY_LOCK, 0);
}
#endif

#ifdef __VXWORKS__
	ke_set_flag(FKERN_KEY_A_B, sysGetA_BSetting());
#endif

	_kkkk();
}

void ki_init_mem_pointers(struct kernel_t *kernel)
{
	int i;
	kernel->x_mem_pointers[0] = kernel->x_memory;
	kernel->total_x_mem_size = kernel->x_mem_sizes[0];
	for(i=1; i<KERN_NUM_SECTIONS; i++){
		kernel->x_mem_pointers[i] = 
			kernel->x_mem_pointers[i-1] 
			+ 
			kernel->x_mem_sizes[i-1];
		kernel->total_x_mem_size += kernel->x_mem_sizes[i];
	}
}

void ki_load_default_config(struct kernel_t * kernel)
{
	/* load default memory layout */
	int i;
	kernel->x_mem_sizes[MEM_SECTION_BYTE] = 12*1024;
	kernel->x_mem_sizes[MEM_SECTION_WORD] = 12*1024;
	kernel->x_mem_sizes[MEM_SECTION_R4] = 32*1024;
	kernel->x_mem_sizes[MEM_SECTION_R8] = 32*1024;
	kernel->x_mem_sizes[MEM_SECTION_ANYSIZE] = 0*1024;
	kernel->x_mem_sizes[MEM_SECTION_BIT] = 32*1024;
	memset(kernel->devices, 0, sizeof(kernel->devices));
	for(i=0;i<IO_MAX_DEVICES;i++){
		kernel->devices[i].addr=i;
	}
}

/*
	save user program and configuration to non-volatile memory.

	Enrivonment :

	kernel lock un-held.
*/
f8_status ki_save_nvram(HF8_KERNEL _k)
{	
	IFileStream *ss;
	IF8Stream * s;
	struct kernel_t * k = (struct kernel_t*)_k;

	if(!k->bModified){
		return F8_SUCCESS;
	}

	/* save user program */
	ss = IFileStream_from_file(F8_PROG_FILE, "wb");
	if(ss){
		s = __ucast__(IFileStream, IF8Stream, (ss));
		__vcall__(k->sections, save, (k->sections, s));
		__delete__(ss);
	}

	/* save system configuration */
	ss = IFileStream_from_file(F8_CFG_FILE, "wb");
	if(ss){
		struct kern_conf_cmd cmd;
		struct ctrller_config cfg;
		s = __ucast__(IFileStream, IF8Stream, (ss));
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd = F8_GET_SYS_CONFIG;
		OLD_ID(&cmd) = BLK_ID_ANY;
		NEW_ID(&cmd) = BLK_ID_NONE;
		cmd.osize = sizeof(cfg);
		cmd.obuffer = (char *)&cfg;
		ke_config(_k, &cmd, __true);
		if(F8_SUCCESSED(cmd.retcode)){
			__vcall__(
				s, 
				put, 
				(s, &cfg, sizeof(cfg))
				);
		}
		__delete__(ss);
	}

	ss = IFileStream_from_file(F8_VARTAB_FILE, "wb");
	if(ss){
		s = __ucast__(IFileStream, IF8Stream, (ss));
		ki_save_var_table(s);
		__delete__(ss);
	}
	
	k->bModified = f8_false;
	
	return F8_SUCCESS;
}

/*
	load user program and configuration from non-volatile memory.
*/
f8_status ki_load_nvram(HF8_KERNEL _k)
{
	IMemStream * ms;
	struct kern_conf_cmd cmd;
	struct blk_hdr_t *hdr;
	struct kernel_t * k;


	memset(&cmd,0,sizeof(struct kern_conf_cmd));

	k = (struct kernel_t *)_k;
	
	ms = IMemStream_load_file(F8_CFG_FILE);
	
	if(ms){
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd = F8_SET_SYS_CONFIG;
		OLD_ID(&cmd) = BLK_ID_ANY;
		NEW_ID(&cmd) = BLK_ID_NONE;
		cmd.isize = ms->length;
		cmd.ibuffer = ms->buffer;
		cmd.obuffer = 0;
		cmd.osize = 0;
		ke_config(_k, &cmd, __true);
		__delete__(ms);
	}

	ms = IMemStream_load_file(F8_PROG_FILE);
	
	if(ms){
		hdr = (struct blk_hdr_t*)ms->buffer;
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd = F8_PUT_PROG;
		OLD_ID(&cmd) = BLK_ID_ANY;
		NEW_ID(&cmd) = hdr->uuid;
		cmd.isize = ms->length;
		cmd.ibuffer = ms->buffer;
		cmd.obuffer = 0;
		cmd.osize = 0;
		ke_config(_k, &cmd, __true);
		__delete__(ms);
	}

	ms = IMemStream_load_file(F8_VARTAB_FILE);
	if(ms){
		__uint count;
		count = ms->length / sizeof(struct blk_var_t);
		ki_load_var_table(count, (struct blk_var_t*)ms->buffer);
		__delete__(ms);
	}
	
	return cmd.retcode;
}

/*	
	start user program execution
*/
void ki_start(struct kernel_t * kernel)
{
//	int i;
	int jret;

	/* clear exchange memory */
	//for(i=0; i<KERN_NUM_SECTIONS; i++){
	//	memset(kernel->x_mem_pointers[i], 0, kernel->x_mem_sizes[i]);
	//}


	if(kernel != &g_kernel){
		return;
	}

	ki_load_variable_values();

	current_time = ke_get_time();
	jret = setjmp(kernel->g_jmpbuf);
	if(!jret){
		/* start IEC-61499 module */
		ki_start_timers(kernel);

		/* start 61131-3 FBD module */
		fbd_start(kernel);
	}else{
		/* halted */
	}
}

void ki_start_1()
{
	//ki_load_variable_values();
	/* start 61131-3 FBD module */
	fbd_start(&g_kernel);
}

/*
	stop user program execution
	stop kernel processing cycle
*/
void ki_stop(struct kernel_t * kernel)
{
	int jret;

	if(kernel == &g_kernel){
		jret = setjmp(kernel->g_jmpbuf);
		if(!jret){
			fbd_stop(kernel);
		}else{
			/* halted */
		}
		ki_stop_timers(kernel);
	}
#if defined(CLEAR_MEM_ON_STOP)
{
	------not used
	int i;
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		memset(kernel->x_mem_pointers[i], 0, kernel->x_mem_sizes[i]);
	}
}
#endif
}

/*
	gather timers/events/instance data for each block
	into a big array.

	on output:

	peerHdr is updated accordingly.
	peerPointer is updated to indicate the end of volatile data

	Note : peerData might be compressed, while the header is not.
*/
f8_status ki_save_volatile(struct kernel_t * kernel)
{
	struct kpeer_hdr_t *h;
	f8_status ret;

	/* write header */
	h = &peerHdr;
	memset(h, 0, sizeof(struct kpeer_hdr_t));
	h->progId = kernel->sections->h.uuid;
	
	memcpy(h->x_mem_sizes, kernel->x_mem_sizes, sizeof(h->x_mem_sizes));
	peerPointer = peerData;
	peerGuardian = peerData + F8_VOLATILE_MEM_SIZE;
	
	/* 61499 timers and events */
	ret = ki_marshel_timers_events(kernel);
	if(F8_FAILED(ret)){
		return ret;
	}

#if 0
	*((f8_uuid*)peerPointer) = kernel->status.prog_id;
	peerPointer += sizeof(f8_uuid);
#endif

	/* write x-memory */
	if(peerPointer + kernel->total_x_mem_size > peerGuardian){
		return F8_LOW_MEMORY;
	}
	memcpy(
		peerPointer, 
		ke_get_x_memory(kernel), 
		kernel->total_x_mem_size
		);
	peerPointer += kernel->total_x_mem_size;

	/* write block level volatile memory */
	if(!fbd_marshel_i_memory(kernel)){
		return F8_INTERNAL_ERROR;
	}

	peerHdr.zipped_data_len=0;
	
	return F8_SUCCESS;
}

f8_status ki_suspend_to_nvram(struct kernel_t * kernel)
{
	IFileStream *ss;
	IF8Stream * s;
	f8_uint size;
	f8_status ret;
	
	if(kernel != &g_kernel){
		return F8_SUCCESS;
	}
	
	ret = ki_save_volatile(kernel);
	if(F8_FAILED(ret)){
		return ret;
	}
	
	ss = IFileStream_from_file(F8_VOLATILE_FILE, "wb");
	if(!ss){
		return F8_LOW_MEMORY;
	}
	s = __ucast__(IFileStream, IF8Stream, (ss));

	/* write header to file */	
	__vcall__(s, put, (s, &peerHdr, sizeof(struct kpeer_hdr_t)));

	/* save volatile memory */
	size = peerPointer - peerData;
	__vcall__(s, put, (s, peerData, size));
	
	__delete__(ss);
	
	return F8_SUCCESS;
}


/*
	unmarshal volatile memory into blocks and x-memory

	Environment:
	1) kernel locked non-exclusively
	2) volatile header set correctly
	3) peerGuardian set correctly
*/
__bool ki_load_volatile(struct kernel_t * kernel)
{
	peerPointer = peerData;
	/* load timers and events */
	if(!ki_unmarshel_timers_events(kernel)){
		return __false;
	}

	/* load x-memory */
	memcpy(
		ke_get_x_memory(kernel), 
		peerPointer, 
		kernel->total_x_mem_size
		);
	peerPointer += kernel->total_x_mem_size;

	/* load block's instance memory */
	if(!fbd_unmarshel_i_memory(kernel)){
		return __false;
	}
	return __true;
}

/*
	load volatile memory contents from NVRAM then resume execution
*/
f8_status ki_resume_from_nvram(struct kernel_t * kernel)
{
	IFileStream * fs = 0;
	IF8Stream * s;

	int i;
	f8_uint size, ldsize;
	f8_status ret;
	
	if(kernel != &g_kernel){
		kernel->status.state = KERN_S_RUNNING;
		return F8_SUCCESS;
	}

	ki_switch_to(KERN_S_STOPPED);
	
	fs = IFileStream_from_file(F8_VOLATILE_FILE, "rb");
	if(!fs){
		return F8_FILE_NOT_FOUND;
	}
	s = __ucast__(IFileStream, IF8Stream, fs);

	if(__vcall__(s, get, (s, &peerHdr, sizeof(struct kpeer_hdr_t))) != sizeof(peerHdr)){
		ret = F8_INVALID_DATA;
		goto __done;
	}

	if(peerHdr.progId != g_kernel.status.prog_id){
		ret = F8_VERSION_MISMATCH;
		goto __done;
	}
	if(peerHdr.event_q_len > EVENT_Q_LENGTH){
		ret = F8_INVALID_DATA;
		goto __done;
	}
	if(peerHdr.timer_q_len > TIMER_Q_LENGTH){
		ret = F8_INVALID_DATA;
		goto __done;
	}
	size = 0;
	for(i=0; i<KERN_NUM_SECTIONS; i++){
		if(peerHdr.x_mem_sizes[i] != kernel->x_mem_sizes[i]){
			ret = F8_VERSION_MISMATCH;
			goto __done;
		}
		size += peerHdr.x_mem_sizes[i];
	}
	size += sizeof(struct marshalled_timer_t) * peerHdr.timer_q_len +
		sizeof(struct marshalled_event_t) * peerHdr.event_q_len +
		peerHdr.i_mem_size;
	if(size + sizeof(struct kpeer_hdr_t) > F8_VOLATILE_MEM_SIZE){
		ret = F8_LOW_MEMORY;
		goto __done;
	}

	ldsize = __vcall__(s, get, (s, peerData, size));
	if(ldsize != size){
		ret = F8_INVALID_DATA;
		goto __done;
	}

	peerGuardian = peerData + ldsize;

	/* ok, now un-marshall the volatile state */
	if(!ki_load_volatile(kernel)){
		ret = F8_INTERNAL_ERROR;
		goto __done;
	}

	current_time = ke_get_time();
	ki_log(kernel, F8_PROG_RESUMED, 0, 0, 0);

	/* start the controller at curr.s_addrent state*/
#if 0	
	if(g_kernel.status.flags & KERN_F_ENABLE_IO){
		start_io();
	}
	kernel->status.state = KERN_S_RUNNING;
#endif
	/* power up */
	if(g_kernel.status.flags & KERN_F_ENABLE_STANDBY)
		ki_switch_to(KERN_S_ARBITRATING);
	else
		ki_switch_to(KERN_S_RUNNING);
	
	ret = F8_SUCCESS;

__done:
	/* all done, free the memory stream object */
	if(fs){
		__delete__(fs);
	}
	return ret;
}

/*
	for any given statistical variable, this function calculates the
	time-average value for it over a series of `a->len' sample values,
	a->buf is used to record the most recent sample values of the
	process variable. 
	
parameters:
	a - a buffer describing the variable whose average value is to be calculated

	v - the snapshot value, if v is outside of the range denoted by (a->m1,a->M1), 
		the value is discared
*/
f8_r8 ki_average(struct average_array_t * a, f8_r8 v)
{
	int jj;

// printf("ki_avarage(%08x,%f)\n",a,v);

	if(v<a->m1 || v>a->M1) 
		return a->rt;
	
	a->rt=a->buf[a->p]=v;
	a->a=0;
	for(jj=0;jj<a->len;jj++){
		a->a += a->buf[jj];
	}
	a->a /= a->len;

	a->p++;
	if(a->p == a->len){
		a->p=0;
	}

	if(v>a->M && v<a->M1)
		a->M=v;
	if(v<a->m && v>a->m1)
		a->m=v;
	return a->a;
}

void ki_init_average(struct average_array_t * a, f8_r8 * arr, int len, f8_r8 M, f8_r8 m)
{
	memset(a,0,sizeof(*a));
	memset(arr,0,sizeof(*arr)*len);
	a->buf=arr;
	a->m1=m;
	a->M1=M;
	a->len=len;
	a->m=M;
	a->M=m;
	a->p=0;
}

void avgShow(struct average_array_t * a, int idx)
{
	int i;
	printf("m=%f,M=%f\n",a->m,a->M);
	for(i=0;i<a->len;i++){
		printf("a[%d]=%f[%08x-%08x]\n",i,a->buf[i],*((unsigned int*)&a->buf[i]),*(((unsigned int*)&a->buf[i])+1));
	}
	printf("avg=%f[%08x,%08x]\n",a->rt,*((unsigned*)&a->rt),*(((unsigned*)&a->rt+1)));
	printf("p=%d\n",a->p);
}

