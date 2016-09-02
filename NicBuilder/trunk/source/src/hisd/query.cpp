/*
	module : query.cpp
	handle client/server communication
	jhunter, 2004/7/30
*/
/*
改动记录
Date      Version  Modified by    Modify
2012-7-9  1.0.0.4  Zander Sun     1.增加对查询单步间隔的错误判断--answerQuery()
2012-8-15 1.0.0.5  Zander Sun     1.增加查询时间和打开文件次数的记录--answerQuery()
*/
#pragma warning(disable:4018)
#include "precomp.h"
#pragma hdrstop
#include <rtk_packet.h>
#include <rtk_thread.h>

static RTK_HANDLE queryThreadPool;
static HVBUS vbusHislog;

static void answerQuery(
	hisqry_pkt_t * query, 
	RTK_ADDR & clientAddress,
	RTK_GUID & clientGuid,
	RTK_TIME queueTime
	)
{
	RTK_TIME t;
	__uint i;
	hisack_pkt_t * ack = 0;
	__uint pktSize;
	__u8  smallBuffer[4096];
	PCTAG_NAME names;
	hislog_item_t * values;
	hislog_key_t startTime, endTime, time;
	CBuffer * buffer;
	__uint n;
	__uint left;
	HBUFFER_CURSOR cursor;

	//2012-7-9  
	if (query->m_step < 1)
	{
		query->m_step = 1;
	}

#if 1
	{
		DEBUG_PRINTF((
			"Requesting %d tags from %s to %s\n",
			query->m_count,
			timeAsString(query->m_time).c_str(),
			timeAsString(query->m_time + query->m_count * query->m_step).c_str()
			));
	}
#endif

	if(!vbusHislog){
		// hisd shutdown in progress
		goto __failed;
	}

	pktSize = query->m_count * sizeof(hislog_item_t) + sizeof(hisack_pkt_t);
	if(pktSize < sizeof(smallBuffer)){		
		ack = (hisack_pkt_t*)smallBuffer;
	}else{
		ack = (hisack_pkt_t*)new __u8[pktSize];
	}
	if(!ack){
		goto __failed;
	}

	memset(ack, 0, pktSize);
	
	//utils_debug(
	//	"Answering %s:%d with %d entries, %s,%d*%d\n",
	//	(char*)CHostName(clientAddress.host),
	//	clientAddress.port,
	//	query->m_tagcount,
	//	timeAsString(query->m_time).c_str(),
	//	query->m_step,
	//	query->m_count
	//	);
	
	ack->m_gid = clientGuid;
	startTime = query->m_time;
	endTime = startTime + query->m_step * query->m_count - 1;

	names = (PCTAG_NAME)(query + 1);
	
	// added by cj @ 2009-6-2 //deleted by Zander on May 27th 2011
	bufferMgr.listWriteLock();

	//TEST 2012-8-15
	FILETIME ft0, ft1;
	DWORD dt, dt_sum;
	SYSTEMTIME st;
	clock_t stt, fih;
	double dur, dur_sum;

	__uint oFNum, tagNum;
	oFNum = 0;
	dt_sum = 0;
	dur_sum = 0;
	tagNum = 0;

	for(i=0; i<query->m_tagcount; i++, names++){
		ack->m_tagName = *names;
		ack->m_gid = clientGuid;
		ack->m_count = query->m_count;
		values = (hislog_item_t*)(ack + 1);
		n = 0;
		// go get values for tagNames[i] from startTime to endTime
		time = startTime;
		left = query->m_count;

		if (names->node.Data[0] == '\0')
		{
			continue;
		}
        
		//TEST 2012-8-15
		GetLocalTime(&st);
		SystemTimeToFileTime(&st, &ft0);
		stt = clock();
        tagNum++;
		/*
			find the first buffer containing data in question
		*/
		cursor = bufferMgr.openBuffer(time, endTime);
		DEBUG_PRINTF(("cursor got %08x\n",cursor));
		if(cursor){
			while(!bufferMgr.cursorIsEnd(cursor) && left){
				buffer = bufferMgr.getBuffer(cursor);
				assert(buffer);

				if(buffer->m_endTime - buffer->m_startTime > 15*60)
				{
					bufferMgr.cursorMoveNext(cursor);
					continue;
				}
				
			//	if(!buffer->isValid()){
			//		break;
			//	}

				//utils_debug("Rlock 21\n");
				//buffer->ReadLock();
				//utils_debug("Wlock 21\n");
	//			buffer->WriteLock();

				if(buffer->m_startTime > endTime){
					// pointer go beyond the end time
					// assert(0);
					// utils_debug("Release 22\n");
			//		buffer->Release();
					break;
				}
				
				if(buffer->m_startTime > time){
					// fill gap
					n = (__uint)((buffer->m_startTime - time + query->m_step - 1) / 
						query->m_step);
					memset(values, 0, sizeof(hislog_item_t) * n);
					values += n;
					left -= n;
					time += n * query->m_step;
				}

				if(time <= buffer->m_endTime){				
					// copy values out of this buffer

					//added by Zander on May 27th 2011
                    bufferMgr.load(cursor, &oFNum);
						

					n = buffer->getBufferValues(
						names, 
						time, 
						left, 
						query->m_step, 
						values
						);
									
					left -= n;
					values += n;
					time += query->m_step * n;

					// TODO: back filling code here
					/*
						if there is a gap between current and last buffer,
						and the tail of last buffer and the head of current buffer
						both contains valid data, we shall interpolate the intermediate
						values, instead of rending them invalid.
						
						incomplete source here
					*/
				}

				// utils_debug("Release 23\n");
			    
				//deleted by Zander on May 27th 2011
				//	buffer->Release();                 
				
				bufferMgr.cursorMoveNext(cursor);
			}
		
			close_handle(cursor);
		}
		//TEST 2012-8-15
		GetLocalTime(&st);
	    SystemTimeToFileTime(&st, &ft1);
		dt = ft1.dwLowDateTime - ft0.dwLowDateTime;
		dt_sum = dt_sum + dt;
		fih = clock();
		dur = (double)(fih - stt)/CLOCKS_PER_SEC;
		dur_sum = dur_sum + dur;

		ack->m_count = query->m_count - left;
		assert(ack->m_count <= query->m_count);

		//if(ack->m_count){
			send_rtk_data_with_guid_ex(
				vbusHislog,
				&clientAddress,
				&clientGuid,
				PF_PacketGroup | PT_QueryArchivedData | PF_ACK,
				i,
				ack,
				ack->m_count * sizeof(hislog_item_t) + sizeof(hisack_pkt_t)
				);
		//}

		{
			char tn[128];
			tagname_to_text(names, tn, sizeof(tn));
		//	utils_debug("%d archived values sent for %s\n", ack->m_count, tn);
		}
			
	}
	//TEST 2012-8-15
	utils_trace("查询%d个标签。\n", tagNum);
    utils_trace("消耗时间： %d * 1e-7 s\n", dt_sum);
	utils_trace("消耗时间： %f s\n", dur_sum);
	utils_trace("打开文件的次数： %d\n", oFNum);

	// added by cj @ 2009-6-2   //deleted by Zander on May 27th 2011
	bufferMgr.listUnlock();

	rtk_time_mark(&t);
	//utils_debug(
	//	"Done with the query, time elapsed is %.3f milliseconds.\n",
	//	rtk_time_diff(&t, &queueTime)*1000
	//	);
__failed:	
	if(ack && (__u8*)ack != smallBuffer){
		delete ack;
	}
}

struct query_work_item_t{
	RTK_ADDR	clientAddress;
	RTK_GUID	clientGuid;
	hisqry_pkt_t	*query;
	RTK_TIME	queueTime;

	query_work_item_t();
	virtual ~query_work_item_t();
	bool init(PCRTK_PACKET p);	
	void run();
};

static void PMC_API queryWorkItemProc(__uint parameters)
{
	query_work_item_t * it;
	it = (query_work_item_t*)parameters;
	it->run();
}

void PMC_API onClientData(const RTK_PACKET * data, HVBUS b)
{
	hisqry_pkt_t *p;
	query_work_item_t * broker;
	__uint expectedSize;

	
	switch(PACKET_TYPE(data)){
	case PT_QueryArchivedData:
		if(!queryThreadPool){
			// oops, nobody is here
			return;
		}
		
		p = (hisqry_pkt_t*)data->data;
		expectedSize = sizeof(hisqry_pkt_t) + p->m_tagcount * sizeof(TAG_NAME);
		if(expectedSize != data->data_size
			||
			p->m_tagcount > HISLOG_MAX_QUERY_TAG_COUNT
			||
			p->m_count > HISLOG_MAX_QUERY_COUNT
			||
			!p->m_count
			||
			!p->m_step
		){
			utils_debug("Invalid hisq request got.\n");
			break;
		}
		
		broker = new query_work_item_t;

		if(!broker){
			break;
		}
		if(!broker->init(data)){
			delete broker;
			break;
		}
		rtk_time_mark(&broker->queueTime);

		{
			CLock lk;
			lk.Lock();
			//Sleep(2000);
			broker->run();
			lk.UnLock();
		}
		//if(!queue_pool_item(queryThreadPool, -1, queryWorkItemProc, (__uint)broker)){
			/*2006-1-16 prevent possible memory leakage*/
		//	delete broker;
		//}
		break;
	case PT_System:
		// trojan hook
		extern void hislogDebugShell(PCRTK_PACKET d);
		hislogDebugShell(data);
		break;
	}
}

bool query_work_item_t::init(PCRTK_PACKET p)
{
	query = (hisqry_pkt_t*)new char[p->data_size];
	if(!query){
		return false;
	}
	memcpy(query, p->data, p->data_size);
	clientAddress = p->src;
	clientGuid = query->m_gid;
	return true;
}

query_work_item_t::~query_work_item_t()
{
	if(query){
		delete query;
	}
}

query_work_item_t::query_work_item_t()
{
	query = 0;
}

/*
	carries out the query task.
*/
void query_work_item_t::run()
{
	answerQuery(query, clientAddress, clientGuid, queueTime);
	delete this;

	// added by chenj, 2007-10-20
//	bufferMgr.unloadAgedBuffers();	
}

void initQueryProcessor()
{
	HVBUS vbus;

	queryThreadPool = create_rthread_pool(hislogNumQueryThreads);
	
	vbus = connect_vbus(
		BUSID_HISLOG, 
		VBUS_CONNECT_AS_SERVER, 
		onClientData, 
		default_server_filter
		);

	if(!vbus){
		utils_error("Cannot connect to HISLOG virtual bus.");
		return;
	}
	
	vbusHislog = vbus;	// create the thread pool
}

void uninitQueryProcessor()
{
	HVBUS vbus;
	vbus = vbusHislog;
	vbusHislog = 0;
	disconnect_vbus(vbus);
	
	close_handle(queryThreadPool);
}
