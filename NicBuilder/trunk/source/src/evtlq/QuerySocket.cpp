#include "StdAfx.h"
#define EVENT_LIST
#include "MainFrm.h"
#include "evtlq.h"

extern CPMCEvent evtsList[MAX_GET_RECORD];

int CQuerySocket::OnData(void * buf,int len,int port,char * src_addr)
{
	if(len < sizeof(CEvtAswPkt)){
		utils_debug("wrong answer pkt!\n");
		return 0;
	}

	CEvtAswPkt *p = (CEvtAswPkt*)buf;

	if(len != p->count * sizeof(CPMCEvent) + sizeof(CEvtAswPkt)){
		utils_debug("wrong answer pkt!\n");
		return 0;
	}

	utils_debug(
		"Got partial answer from %s:%d with %d events staring at %d.\n",
		src_addr,
		port,
		p->count,
		p->id
		);
	
	if(!(p->gid == gid)){
		return 0;
	}

	if(p->id == 0 && p->count == 0){
		::AfxMessageBox("在查询的时间段内没有事件",MB_OK|MB_ICONSTOP);
	}else{
		int id = p->id;
		for(int i = 0; i < p->count; i++){
			evtsList[id] = p->events()[i];
			id++;
		}
	}

	CMainFrame *pmf=(CMainFrame*)::AfxGetMainWnd();
	pmf->m_wndView.RedrawWindow();

	return 0;
}

void CQuerySocket::ClearList()
{
	for(int i = 0; i < MAX_GET_RECORD; i++){
		evtsList[i].valid=false;
	}
}

void CQuerySocket::Query(CEvtQryPkt &pkt)
{
	gid = pkt.gid;
	SendTo(&pkt, sizeof(pkt), 15426 + 100, "230.13.15.19");
}
