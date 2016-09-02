/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	E:\vss\pmc2\src\rtknet\packman.cpp
	autho r: Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Packet management code
	history:
	created:	10:11:2002	 16:50
*********************************************************************/
#pragma warning(disable:4018)
#include "precomp.h"
#pragma hdrstop

#define rtkm_send_fragments_life	2.5	// 0.1 seconds 
#define rtkm_receive_fragments_life	0.5	// 0.1 seconds per fragments

__uint CPacketMan::m_mtu;				/* maximum transmission unit */
__uint CPacketMan::m_dwEffectiveLoad;	// 
__uint CPacketMan::m_dwHeaderSize;

#if 1
#define RESEND_DEBUG
#else
#undef RESEND_DEBUG
#endif

CPacketMan::CPacketMan()
{
}

CPacketMan::~CPacketMan()
{
	// purge send and receive queue
	FlushQueue();
}

// tested : 10/15/2002
// tested : 11/01/2004
__bool CPacketMan::IsFragment(RTK_PACKET * packet)
{
	return packet->total_frag >1 ? __true : __false;
}

/*
	MergePacket()
	
	if an incoming packet belongs to a large packet(thus a fragment), 
	merge it with previously received fragments, then make a second 
	decision whether all fragments of the large packet has been received.
*/
RECEIVE_ITEM * CPacketMan::MergePacket(RTK_PACKET * input_packet)
{
	RECEIVE_ITEM *ri;
	RITEM_LIST::iterator p;
	std::pair<RITEM_LIST::iterator, bool> pr;
	__uint packetSize;
	PRTK_LIST_ENTRY headEntry, entry;
	
	assert(IsFragment(input_packet));

	packetSize = input_packet->total_frag * m_mtu + sizeof(RECEIVE_ITEM);
	
	p = r_lst.find(input_packet->guid);
	
	if(p != r_lst.end()){
		headEntry = entry = p->second.Flink;
		do{
			ri = RTK_CONTAINING_RECORD(entry, RECEIVE_ITEM, li);
			entry = entry->Flink;
		}while(entry != headEntry && ri->header->grp_id != input_packet->grp_id);
			
		if(ri->header->grp_id != input_packet->grp_id){
			ri = 0;
		}
	}else{
		ri = 0;
		headEntry = 0;
	}

	if(!ri){
		/*
			ok, this is the first arriving fragment of the large packet
			perhaps also the first group, if existed

			a new node must be allocated
		*/
		ri = (RECEIVE_ITEM*)Alloc(packetSize);
		if(!ri){
			return 0;
		}

		memset(ri, 0, sizeof(RECEIVE_ITEM));

		if(headEntry){
			// new member of the group, chain them
			RtkInsertTailList(headEntry, &ri->li);
		}else{
			static RTK_LIST_ENTRY theFirstEntry;
			// first packet in the group, add to list
			pr = r_lst.insert(RITEM_LIST::value_type(input_packet->guid, theFirstEntry));
			if(!pr.second){
				Free(ri);
				return 0;
			}
			headEntry = &pr.first->second;
			RtkInitializeListHead(headEntry);
			RtkInsertTailList(headEntry, &ri->li);
		}
		
		ri->header = (PRTK_PACKET)(ri + 1);

		// copy input_packet to header
		*ri->header = *input_packet;
		// and modify some fields
		rtk_time_mark(&ri->birthday);
		ri->life = rtkm_receive_fragments_life * input_packet->total_frag;
		ri->unreceived_fragments = ri->header->total_frag;
		ri->header->data_size = 0;
		ri->header->frgmt_id = 0;
	}
	
	// put the packet into the RECEIVE_ITEM
	
	ri->max_frgmt_id_received >= input_packet->frgmt_id?
		0 : ri->max_frgmt_id_received = input_packet->frgmt_id;

	if(ri->header->total_frag > input_packet->frgmt_id){
		ri->header->data_size += input_packet->data_size;
		ri->bitmap[input_packet->frgmt_id] = 1;
		memcpy(
			ri->header->data + input_packet->offset, 
			input_packet->data, 
			input_packet->data_size
			);
		ri->unreceived_fragments--;
	}else{
		// a fragment claiming a segid larger than the total
		// segment count, simply discard this arrogant
		// fragment.

		// 2004/11/1, we should have already checked against 
		// this in WillingToAccept, so, just assert here
		assert(0);
	}
	
	return ri;
}

void * CPacketMan::Alloc(__uint size)
{
	return (void *)new char[size];
}

void CPacketMan::Free(void * mem)
{
	delete (char*)mem;
}

void CPacketMan::DeleteReceiveItem(RECEIVE_ITEM * item)
{
	RITEM_LIST::iterator p;
	p = r_lst.find(item->header->guid);
	__bool bLastEntry;
	
	if(p == r_lst.end()){
		// assert(0);
		// not found
		return;
	}

	RtkRemoveEntryList(&item->li);
	bLastEntry = RtkIsListEmpty(&p->second)? __true : __false;
	Free(item);

	if(bLastEntry){
		r_lst.erase(p);
	}
}

__bool CPacketMan::IsFragmentationNeeded(const RTK_PACKET * input)
{
	return input->data_size > m_dwEffectiveLoad ? __true : __false;
}

/*
	generate a fragment list for a large data packet
	
	2004/8/2 jackie
	----------------------------------------------------
	now we're to fix a bug when multiple calls of SplitPacket
	with the same guid is being made, this is not impossible as it
	might look like. The server process is often in a loop servicing
	some clients, in each round of the loop it gives out some data,
	note the data is sent over vbus layer using the same GUID that
	the client is expecting. In this case, if the sent data is large
	enough it must go to this place to be split, so we can see the send
	operation is bound to fail because the s_lst won't permit another
	send_item with a same GUID.

	If we got another packet with a same GUID, we append the
	latter one to the previous ones in the queue.
	
	Because the client might be incidently calling send_rtk_data_with_guid
	twice, in which case the appending is not desired, so we need
	the client's explicit statement that appending is permited, we
	use a new flag in packet_type.

	The new flag also serves as a guarding flag to the s_lst reaper
	that the packet is not to be deleted, because some other fragments
	is just on the way to send_rtk_data_with_guid.
*/
SEND_ITEM *CPacketMan::SplitPacket(const RTK_PACKET *input)
{
	char *heap, *p;
	__uint fragments, header_size, last_data_size, heap_size;
	int i, offset, effective_load;
	RTK_PACKET * frag;
	SEND_ITEM * si, * previousItem;
	SITEM_LIST::iterator pit;
	std::pair<SITEM_LIST::iterator, bool> pr;
	PRTK_LIST_ENTRY headEntry, entry;
	__u16 groupId;
	pit = s_lst.find(input->guid);
	if(pit != s_lst.end()){
		headEntry = &pit->second;
		entry = headEntry->Blink;
		assert(!RtkIsListEmpty(entry));
		previousItem = RTK_CONTAINING_RECORD(entry, SEND_ITEM, li);
		if(!(previousItem->header->packet_type & PF_PacketGroup)){
			/*
				sending another packet of the same GUID without
				group flag set is not permited.
			*/
			return NULL;
		}
		groupId = previousItem->header->grp_id + 1;
	}else{
		previousItem = NULL;
		si = 0;
		headEntry = 0;
		groupId = 0;
	}
	
	// calculate size needed
	header_size = sizeof(RTK_PACKET) - sizeof(input->data);
	effective_load = m_mtu - header_size;
	fragments = (input->data_size + effective_load - 1) / effective_load;
	heap_size = fragments * m_mtu;
	heap_size += sizeof(SEND_ITEM);

	// create the send_item
	heap = (char *)Alloc(heap_size);
	if(!heap){
		return 0;
	}
	
	si = (SEND_ITEM*)heap;
	memset(si, 0, sizeof(SEND_ITEM));

	si->header = (RTK_PACKET*)(si + 1);

	// insert the send_item into the s_lst
	if(!headEntry){
		static RTK_LIST_ENTRY theFirstEntry;
		pr = s_lst.insert(SITEM_LIST::value_type(input->guid, theFirstEntry));
		if(!pr.second){
			Free(heap);
			return 0;
		}
		headEntry = &pr.first->second;
		RtkInitializeListHead(headEntry);
		RtkInsertTailList(headEntry, &si->li);
	}else{
		// appending the fragments to the existing item
		RtkInsertTailList(headEntry, &si->li);
	}
	
	last_data_size = input->data_size % effective_load;
	offset = 0;
	for(i = 0, p = (char*)si->header, frag = (RTK_PACKET*)p; 
		i<fragments; 
		i++, p += m_mtu, frag = (RTK_PACKET*)p, offset += effective_load
	){
		// copy contents of input_packet to fragment
		*frag			= *input;
		// then modify some fields
		frag->frgmt_id		= i;
		frag->total_frag 	= fragments;
		frag->data_size 	= i==fragments-1? last_data_size : effective_load;
		frag->offset		= offset;
		frag->grp_id		= groupId;
		memcpy(frag->data, input->data + offset, frag->data_size);
	}

	rtk_time_mark(&si->birthday);
	si->life = rtkm_send_fragments_life * fragments;
	if(input->packet_type & PF_PacketGroup){
		si->life *= 5;
	}

	return si;
}

void CPacketMan::request_packets(RECEIVE_ITEM * h)
{
	char	packet_buffer[MAX_PACKET_SIZE];
	PRTK_PACKET packet;
	__u16 *rsids;
	__uint items;
	int j, i;

	packet = (PRTK_PACKET)packet_buffer;
	ZeroMemory(packet, sizeof(RTK_PACKET));
	items = h->unreceived_fragments;
	if(items > 10){
		items = 10;
	}
	packet->version = RTK_VERSION;
	packet->src.host= g_ThisNode->key;
	packet->dest	= h->header->src;
	packet->guid.create();
	packet->total_frag = 1;
	packet->data_size = sizeof(__u32)*items
		+ sizeof(RTK_GUID);
	/*
	packet->packet_size = packet->data_size + sizeof(*packet) - 
		sizeof(packet->data);
	*/
	packet->packet_type = PT_RequestFragment;
	*((PRTK_GUID)packet->data) = h->header->guid;
	rsids = (__u16*)(packet->data + sizeof(RTK_GUID));
	
#ifdef RESEND_DEBUG
	char dbg_msg[1024], *dbg_p;
	dbg_msg[0] = 0;
	dbg_p = dbg_msg;
	dbg_p += sprintf(
		dbg_p, "_P: Need %d of %d, grp %d, SOURCE=%s:%d, type=%08x", 
		items, 
		h->header->total_frag,
		h->header->grp_id,
		(char *)CHostName(h->header->src.host),
		h->header->src.port,
		h->header->packet_type
		);
#endif

	*rsids = h->header->grp_id;
	rsids++;
	j = 0;
	for(i=0; i<h->header->total_frag; i++){
		if(!h->bitmap[i]){
			*rsids = i; 					
#ifdef RESEND_DEBUG
			dbg_p += sprintf(dbg_p, "%d ", *rsids);
#endif
			j++;
			assert(j<=h->unreceived_fragments);
			if(j >= items){
				break;
			}
			rsids++;
		}
	}

	if(h->unreceived_fragments <= 10){
		assert(j == h->unreceived_fragments);
	}
	
#ifdef RESEND_DEBUG
	utils_debug("%s\n", dbg_msg);
#endif

	SendPacket(packet);
}

/*
	reap_received()

	for each half-packets in receive queue, if it's been there for time
	longer than expected reconstruction time(rtkm_receive_fragments_life
	*total_frag), then a re-send packet is generated to request the original
	sender to re-send that fragments that're missing; if the packet has
	been there for 3 times longer than expected reconstruction time, then
	delete it, and output an error message.

	2004/11/1, grouped packets can stay 5 times as long, because
	the sender might take some time between sending each member
	of the grouped packets.
*/
void CPacketMan::reap_received(PCRTK_TIME now)
{
	RITEM_LIST::iterator hit, nextIt;
	PRTK_LIST_ENTRY headEntry, entry;
	RECEIVE_ITEM *h;
	double 		diff;

	hit = r_lst.begin();
	while(hit != r_lst.end()){
		nextIt = hit;
		nextIt++;
		/*
			for each entry, we check if it's a grouped packet

			if yes, we traverse the group list, and delete the
			aged nodes.
			
			care should be taken if the head node is to be 
			deleted.
		*/
		headEntry = &hit->second;
		entry = headEntry->Flink;
		while(entry != headEntry){
			h = RTK_CONTAINING_RECORD(entry, RECEIVE_ITEM, li);
			entry = entry->Flink;
			diff = rtk_time_diff(now, &h->birthday);
			if(diff > h->life*3 || !h->unreceived_fragments){
				if(h->unreceived_fragments){
					assert(h->unreceived_fragments > 0);
					utils_debug(">>P from %s:%d,%d of %d lost,aging,discarded\n", 
						(char*)CHostName(h->header->src.host),
						h->header->src.port,
						h->unreceived_fragments,
						h->header->total_frag
						);
				}
				DeleteReceiveItem(h);
			}else if((diff > h->life) || (h->max_frgmt_id_received==h->header->total_frag-1)){
				// request for resend
				if(h->unreceived_fragments){
					request_packets(h);
				}
			}
		}
		
		hit = nextIt;
	}
}

/*
	reap_sent()
	deletes all send-items in send-queue when no more re-send requests
	are pending.
*/
void CPacketMan::reap_sent(PCRTK_TIME now)
{
	SITEM_LIST::iterator sit, next;
	SEND_ITEM *si;
	PRTK_LIST_ENTRY headEntry, entry;
	double diff;
	
	sit = s_lst.begin();
	while(sit != s_lst.end()){
		next = sit;
		next++;
		headEntry = &sit->second;
		entry = headEntry->Flink;
		while(entry != headEntry){
			si = RTK_CONTAINING_RECORD(entry, SEND_ITEM, li);
			entry = entry->Flink;
			diff = rtk_time_diff(now, &si->birthday);
			if(diff > si->life){
				/*
					remove this send_item
				*/
				RtkRemoveEntryList(&si->li);
				Free(si);
			}
		}
		if(RtkIsListEmpty(headEntry)){
			s_lst.erase(sit);
		}
		sit = next;
	}
}

void CPacketMan::reap_packets()
{
	RTK_TIME	now;
	rtk_time_mark(&now);

	reap_sent(&now);
	reap_received(&now);
}

void CPacketMan::dbg_dump()
{
	RECEIVE_ITEM *h;
	RITEM_LIST::iterator p;
	RTK_TIME now;
	int i=0;
	SITEM_LIST::iterator sit;
	SEND_ITEM *si;
	PRTK_LIST_ENTRY entry, headEntry;

	rtk_time_mark(&now);
	if(r_lst.size()){
		utils_error("%d packets under construction:-->\n", r_lst.size());
		p = r_lst.begin();		
		
		while(p != r_lst.end()){
			headEntry = &(p->second);
			entry = headEntry->Flink;
			while(entry != headEntry){
				h = RTK_CONTAINING_RECORD(entry, RECEIVE_ITEM, li);
				utils_error("NO.%d, Age=%.3f, life=%.3f, ", 
					i,
					rtk_time_diff(&now, &h->birthday),
					h->life
					);
				utils_error("Got %d of Total %d fragments, size %d bytes\n", 
					h->header->total_frag - h->unreceived_fragments,
					h->header->total_frag,
					h->header->data_size
					);
				entry = entry->Flink;
			}
			p++;
			i++;
		}
	}
	if(s_lst.size()){
		utils_error("%d packets in send queue:------>\n", s_lst.size());
		sit = s_lst.begin();
		i = 0;
		while(sit != s_lst.end()){
			headEntry = &sit->second;
			entry = headEntry->Flink;
			while(entry != headEntry){
				si = RTK_CONTAINING_RECORD(entry, SEND_ITEM, li);
				entry = entry->Flink;
				utils_error("No.%d, %d fragments, age=%.3f, life=%.3f\n", 
					i, si->header->total_frag, rtk_time_diff(&now, &si->birthday),
					si->life
					);
				i++;
			}
			sit++;
		}
	}
}

/*
	get a send_item by it's guid and segmentation id
*/
SEND_ITEM * CPacketMan::GetSendItem(const RTK_GUID & Guid, __uint grpId)
{
	SITEM_LIST::iterator it;
	PRTK_LIST_ENTRY head, entry;
	SEND_ITEM * si;
	PCRTK_PACKET p;
	
	it = s_lst.find(Guid);
	if(it == s_lst.end()){
		return 0;
	}
	head = &it->second;
	entry = head->Flink;
	while(entry != head){
		si = RTK_CONTAINING_RECORD(entry, SEND_ITEM, li);
		p = si->header;
		if(p->grp_id == grpId){
			return si;
		}
		entry = entry->Flink;
	}

	return 0;
}

void CPacketMan::FlushQueue()
{
	// flush send queue
	SITEM_LIST::iterator sit, next;
	SEND_ITEM	*si;
	PRTK_LIST_ENTRY entry, headEntry;
	RECEIVE_ITEM * ri;

	sit = s_lst.begin();
	while(sit != s_lst.end()){
		next = sit;
		next++;
		headEntry = &sit->second;
		entry = headEntry->Flink;
		while(entry != headEntry){
			si = RTK_CONTAINING_RECORD(entry, SEND_ITEM, li);
			entry = entry->Flink;
			RtkRemoveEntryList(&si->li);
			Free(si);
		}
		s_lst.erase(sit);
		sit = next; 		   
	}

	// flush receive queue
	RITEM_LIST::iterator hit, next_hit;
	hit = r_lst.begin();
	while(hit != r_lst.end()){
		next_hit = hit;
		next_hit++;
		headEntry = &hit->second;
		entry = headEntry->Flink;
		while(entry != headEntry){
			ri = RTK_CONTAINING_RECORD(entry, RECEIVE_ITEM, li);
			entry = entry->Flink;
			RtkRemoveEntryList(&ri->li);
			Free(ri);
		}
		r_lst.erase(hit);
		hit = next_hit;
	}
}

