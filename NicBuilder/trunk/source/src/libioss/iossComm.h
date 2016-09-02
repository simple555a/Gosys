#ifndef _ioss_comm_h_
#define _ioss_comm_h_
#include "precomp.h"

class ExitHandler : public ACE_Event_Handler
{
public:
	virtual int handle_exception (ACE_HANDLE) {
		reactor ()->end_reactor_event_loop ();
		return 0; // Trigger call to handle_close() method.
	}
};

ACE_THR_FUNC_RETURN tp_event_loop_0 (void *arg);
ACE_THR_FUNC_RETURN tp_event_loop_1 (void *arg);
ACE_THR_FUNC_RETURN tp_event_loop_2 (void *arg);
ACE_THR_FUNC_RETURN tp_event_loop_3 (void *arg);
ACE_THR_FUNC_RETURN tp_event_loop_4 (void *arg);
ACE_THR_FUNC_RETURN tp_event_loop_5 (void *arg);



#endif //_ioss_comm_h_