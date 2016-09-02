/*
	capture call-stack at runtime
	6/24/2005
	j.hunter
*/
#ifndef __call_stk_h__
#define __call_stk_h__

struct call_frame_t{
	int retAddress;
};

struct call_stack_t{
	int count;
	/* a variable length array */
	struct call_frame_t frames[1];
};

#endif

