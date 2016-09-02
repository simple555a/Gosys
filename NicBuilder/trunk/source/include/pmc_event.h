#ifndef __event_h_
#define __event_h_

#include <pshpack1.h>

struct CPMCEvent{
	char valid;
	char reserved[3];
	int eClass;
	PMCTIME rTime;
	PMCTIME eTime;
	RTK_HOST src;
	int rank;
	char data[256];
};

struct CEvtQryPkt{
	RTK_GUID gid;
	PMCTIME stm;
	PMCTIME etm;
	int eClass;
	unsigned int count;
};

#define SIZE_OF_ONE_PKT	(1024*8)

#define COUNT_IN_ONE_PKT ((SIZE_OF_ONE_PKT-sizeof(RTK_GUID)-sizeof(unsigned int))/sizeof(CPMCEvent))

struct CEvtAswPkt
{
	RTK_GUID gid;
	int id;
	int count;
	// CPMCEvent events[0];
	CPMCEvent * events()
	{
		return (CPMCEvent*)(this + 1);
	}
};

#define EVT_ARRAY_OF(e) (CPMCEvent*)((char *)&(e) + sizeof(CEvtAswPkt))

#include <poppack.h>

#endif