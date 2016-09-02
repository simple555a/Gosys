/*
	IEC 61499 support routines.
	timers and events manipulations
	
	panxiaohui@actech-mc.com, Nov. 2003
*/
#include "precomp.h"

/*
	return a timer to the free timer list.
*/
int ki_free_timer(struct kernel_t * kernel, struct blk_timer_t * tm)
{
	assert(tm->hint == TIMER_H_BUSY);

	// remove from the busy list
	RtkRemoveEntryList(&tm->link);
	RtkInsertHeadList(&kernel->tmr_free, &tm->link);

	tm->hint = TIMER_H_FREE;	
	tm->pin->dp->u.e.tid = -1;
	tm->pin = 0;

	return 0;
}

/*
	allocate a timer slot from timer pool;
*/
int ki_alloc_timer(struct kernel_t * kernel)
{
	int tid;
	RTK_LIST_ENTRY *lk;
	struct	blk_timer_t * tm;

	if(RtkIsListEmpty(&kernel->tmr_free)){
		return -1;
	}

	lk = RtkRemoveHeadList(&kernel->tmr_free);
	tm = SAFE_CONTAINING_RECORD(lk, struct blk_timer_t, link);
	tid = tm - kernel->timers;

	assert(kernel->timers[tid].hint == TIMER_H_FREE);

	// place the timer at the tail of the busy list
	RtkInsertTailList(&kernel->tmr_busy, lk);

	kernel->timers[tid].hint = TIMER_H_BUSY;

	return tid;
}

int ki_alloc_timer_must_succeed(struct kernel_t * kernel)
{
	int tid;

	tid = ki_alloc_timer(kernel);
	if(tid < 0){
		ki_halt(kernel, F8_NO_TIMER);
	}

	return tid;
}

int ki_sched(struct kernel_t * kernel, double timeout, struct blk_pin_t * p, f8_u8 ttype)
{
	int tid;
	struct blk_timer_t * tm;

	if(p->_class != PIN_CLASS_EO && p->_class != PIN_CLASS_EI){
		ki_halt(kernel, F8_FIRE_NON_EVENT);
	}

	if(ttype != TIMER_T_ONESHOT){
		ttype = TIMER_T_PERIODIC;
	}

	if(timeout == 0){
		ki_eq(kernel, p);
		return -1;
	}

	tid = ki_alloc_timer_must_succeed(kernel);
	tm = _get_timer(kernel, tid);

	if(timeout < 0){
		// absolute time
		tm->duetime = (ktime_t)(-timeout * 1e7);
		if(ttype != TIMER_T_ONESHOT){
			ki_halt(kernel, F8_TIMER_TYPE);
		}
	}else{
		tm->period = (ktime_t)(timeout * 1e7);
		tm->duetime = tm->period + ke_get_time();		
	}
	
	tm->type = ttype;
	tm->pin = p;
	p->dp->u.e.tid = tid;

	return tid;
}

f8_bool ki_cancel(struct kernel_t * kernel, int handle, struct blk_pin_t * p)
{
	struct blk_timer_t * tm;

	if(handle < 0 || handle >= TIMER_Q_LENGTH){
		ki_halt(kernel, F8_INVALID_TIMER_ID);
	}
	tm = _get_timer(kernel, handle);
	if(tm->pin == p && (tm->hint == TIMER_H_BUSY)){
		ki_free_timer(kernel, tm);
		return f8_true;
	}	
	return f8_false;
}

struct blk_pin_t * ki_dq(struct kernel_t * kernel)
{
	struct blk_pin_t * p;
	if(kernel->eq_end == kernel->eq_head){
		return 0;
	}
	p = kernel->event_q[kernel->eq_head];
	kernel->eq_head++;
	kernel->eq_head %= sizeof(kernel->event_q) / sizeof(kernel->event_q[0]);
	return p;
}

__bool ki_eq(struct kernel_t * kernel, struct blk_pin_t * p)
{
	int next;
	next = kernel->eq_end;
	next++;
	next %= sizeof(kernel->event_q) / sizeof(kernel->event_q[0]);
	if(next == kernel->eq_head){
		ki_halt(kernel, F8_QUEUE_OVERFLOW);
	}
	kernel->event_q[kernel->eq_end] = p;
	kernel->eq_end = next;
	return __true;
}

void ki_start_timers(struct kernel_t * kernel)
{
	struct blk_timer_t * tm;
	RTK_LIST_ENTRY	*lk;

	lk = kernel->tmr_busy.Flink;
	while(lk != &kernel->tmr_busy){
		tm = RTK_CONTAINING_RECORD(lk, struct blk_timer_t, link);
		/* assert(tm->type == TIMER_T_PERIODIC); */
		tm->duetime = current_time + tm->period;
		/* */
		lk = lk->Flink;
	}
}

void ki_stop_timers(struct kernel_t * kernel)
{
	struct blk_timer_t * tm;
	RTK_LIST_ENTRY *lk;

	lk = kernel->tmr_busy.Flink;
	while(lk != &kernel->tmr_busy){
		tm = RTK_CONTAINING_RECORD(lk, struct blk_timer_t, link);
		lk = lk->Flink;
		if(tm->type != TIMER_T_PERIODIC){
			/* remove this entry */
			ki_free_timer(kernel, tm);
		}		
	}
}

/*
	marshel events and timers into an array which can 
	be transferred among hosts on the network without loss
	of semantics.
*/
f8_status ki_marshel_timers_events(struct kernel_t * kernel)
{
	PRTK_LIST_ENTRY li;
	struct blk_timer_t * t;
	struct marshalled_timer_t * mt; 
	struct blk_pin_t * p;
	struct marshalled_event_t * me;
	int i;

	kernel->status.timer_q_len = 0;
	kernel->status.event_q_len = 0;
	
#ifdef ENABLE_61499
	li = kernel->tmr_busy.Flink;
	mt = (struct marshalled_timer_t*)peerPointer;
	while(li != &kernel->tmr_busy){
		t = RTK_CONTAINING_RECORD(li, struct blk_timer_t, link);		
		
		mt->blkid = t->pin->blk->uuid;
		mt->pid = __vcall__(t->pin->blk, id_by_pin, (t->pin));
		mt->duetime = t->duetime;
		mt->period = t->period;
		mt->type = t->type;

		kernel->status.timer_q_len++;
		mt++;
		if((f8_u8*)&mt[1] > peerGuardian){
			return F8_LOW_MEMORY;
		}

		li = li->Flink;
	}
	peerPointer = (f8_u8*)mt;

	i = kernel->eq_head;
	me = (struct marshalled_event_t*)peerPointer;
	while(i != kernel->eq_end){
		p = kernel->event_q[kernel->eq_head];
		
		me->blkid = p->blk->uuid;
		me->pid = __vcall__(p->blk, id_by_pin, (p));

		me++;
		if((f8_u8*)&me[1] > peerGuardian){
			return F8_LOW_MEMORY;
		}
		kernel->status.event_q_len++;
		i++;
		i %= sizeof(kernel->event_q) / sizeof(kernel->event_q[0]);
	}
	peerPointer = (f8_u8*)me;
#endif

	peerHdr.timer_q_len = kernel->status.timer_q_len;
	peerHdr.event_q_len = kernel->status.event_q_len;
	
	return F8_SUCCESS;
}

/*
	load timer queue and event queue from volatile memory.
	on return, the peerPointer is updated to reflect the bytes
	consumed.
*/
__bool ki_unmarshel_timers_events(struct kernel_t * kernel)
{
	struct marshalled_timer_t * mt;
	struct marshalled_event_t * me;
#ifdef ENABLE_61499
	mt = (struct marshalled_timer_t * )peerPointer;
	/* 
	incomplete source here, the kernel's timer queue should be loaded
	from volatile memory accordingly.
	*/
	mt += peerHdr.timer_q_len;
	peerPointer = (f8_u8*)mt;
	
	me = (struct marshalled_event_t * )peerPointer;
	/* incomplete source here */
	me += peerHdr.event_q_len;
	peerPointer = (f8_u8*)me;
#endif
	return __true;
}

static __inline void _exec_ei(struct blk_pin_t * p)
{
	g_kernel.current_blk = p->blk;
	g_kernel.current_bblk = __dcast__(IBlk, IBBlk, g_kernel.current_blk);
	g_kernel.current_pin = p;

	assert(g_kernel.current_blk->h.magic == BBLK_MAGIC);
	
	if(p->u2.invoke >= g_kernel.current_blk->h.u1.n_funcs){
		ki_halt(&g_kernel, F8_INVALID_FUNC_ID);
	}	
	ki_exec_algorithm(
		g_kernel.current_bblk, 
		g_kernel.current_bblk->funcs + p->u2.invoke
		);
}

/*
	cause an event to be excuted
	p must be an event input pin
*/
LOCAL void _execute_event(struct blk_pin_t * p)
{
	struct blk_pin_t * ip;
	PTRIPLE_LIST_ENTRY	l;

	_set_current(&g_kernel, p->blk);
	
	if(p->_class == PIN_CLASS_EI){
		_exec_ei(p);
		return;
	}else if(p->_class == PIN_CLASS_EO){
		l = p->u1.link.Flink;
		while(l != &p->u1.link){
			ip = RTK_CONTAINING_RECORD(l, struct blk_pin_t, u1.link);
			_exec_ei(ip);
			l = l->Flink;
		}
	}else{
		ki_halt(&g_kernel, F8_FIRE_NON_EVENT);
	}
}

LOCAL void _timer_expires()
{
	struct blk_timer_t * tm;
	PRTK_LIST_ENTRY lk;
	struct	blk_pin_t * p;

	lk = g_kernel.tmr_busy.Flink;
	while(lk != &g_kernel.tmr_busy){
		tm = RTK_CONTAINING_RECORD(lk, struct blk_timer_t, link);
		lk = lk->Flink;

		assert(tm->hint == TIMER_H_BUSY);
		
		if(tm->duetime >= current_time){
			continue;
		}
		/* timer expires */
		p = tm->pin;
		if(TIMER_T_ONESHOT == tm->type){
			ki_free_timer(&g_kernel, tm);
		}else{
			// set next duetime
			tm->duetime += tm->period;
		}			
		if(p->_class == PIN_CLASS_EI){
			ki_eq(&g_kernel, p);
		}else{
			TRIPLE_LIST_ENTRY *l;
			struct blk_pin_t *peer;
			l = p->u1.link.Flink;
			while(l != &p->u1.link){
				peer = RTK_CONTAINING_RECORD(l, struct blk_pin_t, u1.link);
				ki_eq(&g_kernel, peer);
				l = l->Flink;
			}
		}
	}
}


void ki_exec_61499()
{
	int i;	
	struct blk_pin_t *p;
	for(i=0; i<1024 && KERN_S_RUNNING == g_kernel.status.state; i++){
		p = ki_dq(&g_kernel);
		if(!p){
			break;
		}	
		if(PIN_OF_UNMOUNTED_BLOCK == p){
			continue;
		}
		_execute_event(p);
	}
	_timer_expires();
}

