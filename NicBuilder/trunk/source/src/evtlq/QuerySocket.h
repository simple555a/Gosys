#ifndef __query_socket_h_
#define __query_socket_h_
#include "pmc.h"
#include "mcudp.h"

class CQuerySocket : public CMulticastSocket{
	RTK_GUID gid;
public:
    CQuerySocket(){};
	void Query(CEvtQryPkt &pkt);
	void ClearList();
    virtual int OnData(void * buf,int len,int port,char * src_addr);
};

#endif